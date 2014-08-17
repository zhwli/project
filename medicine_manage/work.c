#include"yaofang.h"
int main(void)
{
	COORD size={SCR_COL,SCR_ROW};
	gh_std_out=GetStdHandle(STD_OUTPUT_HANDLE);
	gh_std_in=GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleTitle(gp_sys_name);
	SetConsoleScreenBufferSize(gh_std_out,size);
	InitInterface();

	gi_sel_menu=3;
	TagMainMenu(gi_sel_menu);
	PopMenu(gi_sel_menu);
	CreatList(&gp_head);
	RunSys(&gp_head);
	getch();
	PopOff();
	getch();
	return 0;
}


void ShowMenu()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	CONSOLE_CURSOR_INFO lpCur;
	COORD size;
	COORD pos={0,0};
	int i,j;
	int PosA=2,PosB;
	char ch;
	GetConsoleScreenBufferInfo(gh_std_out,&bInfo);
	size.X=bInfo.dwSize.X;
	size.Y=1;
	SetConsoleCursorPosition(gh_std_out,pos);
	for(i=0;i<5;i++)
		printf("%s",ga_main_menu[i]);

	GetConsoleCursorInfo(gh_std_out,&lpCur);
	lpCur.bVisible=FALSE;
	SetConsoleCursorInfo(gh_std_out,&lpCur);
	gp_buff_menubar_info=(CHAR_INFO*)malloc(size.X*size.Y*sizeof(CHAR_INFO));
	SMALL_RECT rcMenu={0,0,size.X-1,0};
	ReadConsoleOutput(gh_std_out,gp_buff_menubar_info,size,pos,&rcMenu);
	for(i=0;i<size.X;i++)
	{
		(gp_buff_menubar_info+i)->Attributes=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
		ch=(char)((gp_buff_menubar_info+i)->Char.AsciiChar);
		if((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))
		{
			(gp_buff_menubar_info+i)->Attributes|=FOREGROUND_RED;
		}
	}
	WriteConsoleOutput(gh_std_out,gp_buff_menubar_info,size,pos,&rcMenu);
	TagMainMenu(gi_sel_menu);
	return;
}

void TagMainMenu(int num){
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD size;
	COORD pos={0,0};
	int i;
	int PosA=0,PosB;
	char ch;

	if(num==0)
	{
		PosA=0;
		PosB=0;
	}
	else{
		for(i=1;i<num;i++)
		{
			PosA+=(strlen(ga_main_menu[i-1]));
		}
		PosB=PosA+(strlen(ga_main_menu[num-1]));
	}
	GetConsoleScreenBufferInfo(gh_std_out,&bInfo);
	size.X=bInfo.dwSize.X;
	size.Y=1;

	for(i=0;i<PosA;i++)
	{
		(gp_buff_menubar_info+i)->Attributes=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
		ch=(gp_buff_menubar_info+i)->Char.AsciiChar;
		if((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))
		{
			(gp_buff_menubar_info+i)->Attributes|=FOREGROUND_RED;
		}
	}

	for(i=PosA;i<PosB;i++)
	{
		(gp_buff_menubar_info+i)->Attributes=FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
	}

	for(i=PosB;i<bInfo.dwSize.X;i++)
	{
		(gp_buff_menubar_info+i)->Attributes=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
		ch=(gp_buff_menubar_info+i)->Char.AsciiChar;
		if((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))
		{
			(gp_buff_menubar_info+i)->Attributes|=FOREGROUND_RED;
		}
	}
	SMALL_RECT rcMenu={0,0,size.X-1,0};
	WriteConsoleOutput(gh_std_out,gp_buff_menubar_info,size,pos,&rcMenu);
	return;
}

void ClearScreen(void)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD home={0,0};
	unsigned long size;
	DWORD xin;
	GetConsoleScreenBufferInfo(gh_std_out,&bInfo);
	size=bInfo.dwSize.X*bInfo.dwSize.Y;
	FillConsoleOutputAttribute(gh_std_out,bInfo.wAttributes,size,home,&xin);
	FillConsoleOutputCharacter(gh_std_out,' ',size,home,&xin);
	return;
}

void PopMenu(int num)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	DWORD xin;
	char*pCh;
	int i,j,loc=0;
	if(num!=gi_sel_menu)
	{
		if(gp_top_layer->LayerNo!=0)
		{
			PopOff();
			gi_sel_sub_menu=0;
		}
	}
	else if(gp_top_layer->LayerNo!=0)	return;

	gi_sel_menu=num;
	TagMainMenu(gi_sel_menu);
	LocSubMenu(gi_sel_menu,&rcPop);
	for(i=1;i<gi_sel_menu;i++)
	{
		loc+=ga_sub_menu_count[i-1];
	}
	labels.ppLabel=ga_sub_menu+loc;
	labels.num=ga_sub_menu_count[gi_sel_menu-1];
	COORD aLoc[6];

	for(i=0;i<labels.num;i++)
	{
		aLoc[i].X=rcPop.Left+2;
		aLoc[i].Y=rcPop.Top+1;
	}
	labels.pLoc=aLoc;
	areas.num=labels.num;
	SMALL_RECT aArea[8];
	char aSort[8];
	char aTag[8];
	for(i=0;i<areas.num;i++)
	{
		aArea[i].Left=rcPop.Left+2;
		aArea[i].Top=rcPop.Top+i+1;
		aArea[i].Right=rcPop.Right-2;
		aArea[i].Bottom=aArea[i].Top;
		aSort[i]=0;
		aTag[i]=i+1;
	}
	areas.pArea=aArea;
	areas.pSort=aSort;
	areas.pTag=aTag;
	att=BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_RED;
	PopUp(&rcPop,att,&labels,&areas);
	DrawBox(&rcPop);
	pos.X=rcPop.Left+2;
	for(pos.Y=rcPop.Top+1;pos.Y<rcPop.Bottom;pos.Y++)
	{
		pCh=ga_sub_menu[loc+pos.Y-rcPop.Top-1];
		if(strlen(pCh)==0)
		{
			FillConsoleOutputCharacter(gh_std_out,'-',rcPop.Right-rcPop.Left-3,pos,&xin);
			for(j=rcPop.Left+2;j<rcPop.Right-1;j++)
			{gp_scr_att[pos.Y*SCR_COL+j]&=3;
			}
		}
	}
	pos.X=rcPop.Left+3;
	att=BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_RED;
	for(pos.Y=rcPop.Top+1;pos.Y<rcPop.Bottom;pos.Y++)
	{
		if(strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)		continue;
		FillConsoleOutputAttribute(gh_std_out,att,1,pos,&xin);
	}
	return;
}

void PopUp(SMALL_RECT * pRc,WORD att,LABEL_BUNDLE * pLabel,HOT_AREA * pHotArea)
{
	LAYER_NODE*nextLayer;
	COORD size;
	COORD pos={0,0};
	char * pCh;
	int i,j,row;
	size.X=pRc->Right-pRc->Left;
	size.Y=pRc->Bottom-pRc->Top+1;

	nextLayer=(LAYER_NODE*)malloc(sizeof(LAYER_NODE));
	nextLayer->next=gp_top_layer;
	nextLayer->LayerNo=gp_top_layer->LayerNo+1;
	nextLayer->rcArea=*pRc;
	nextLayer->pContent=(CHAR_INFO*)malloc(size.X*size.Y*sizeof(CHAR_INFO));
	nextLayer->pScrAtt=(char*)malloc(size.X*size.Y*sizeof(char));
	pCh=nextLayer->pScrAtt;

	ReadConsoleOutput(gh_std_out,nextLayer->pContent,size,pos,pRc);
	for(i=pRc->Top;i<=pRc->Bottom;i++)
	{
		for(j=pRc->Left;j<=pRc->Right;j++)
		{
			*pCh=gp_scr_att[i*SCR_COL+j];
			pCh++;
		}
	}
	gp_top_layer=nextLayer;
	pos.X=pRc->Left;
	pos.Y=pRc->Top;
	DWORD xin;
	for(i=pRc->Top;i<=pRc->Bottom;i++)
	{

		FillConsoleOutputAttribute(gh_std_out,att,size.X,pos,&xin);
		pos.Y++;
	}

	for(i=0;i<pLabel->num;i++)
	{
		pCh=pLabel->ppLabel[i];

		if(strlen(pCh)!=0)
		{

         	WriteConsoleOutputCharacter(gh_std_out,pCh,strlen(pCh),pLabel->pLoc[0],&xin);
			pLabel->pLoc[0].Y++;
		}
	}

	for(i=pRc->Top;i<=pRc->Bottom;i++)
	{
		for(j=pRc->Left;j<=pRc->Right;j++)
		{
			gp_scr_att[i*SCR_COL+j]=gp_top_layer->LayerNo;
		}
	}

	for(i=0;i<pHotArea->num;i++)
	{
		row=pHotArea->pArea[i].Top;
		for(j=pHotArea->pArea[i].Left;j<=pHotArea->pArea[i].Right;j++)
		{
			gp_scr_att[row*SCR_COL+j]|=(pHotArea->pSort[i]<<6)|(pHotArea->pTag[i]<<2);
		}
	}
	return;
}

void PopOff(void)
{
	LAYER_NODE * nextLayer;
	COORD size;
	COORD pos={0,0};
	char* pCh;
	int i,j;
	if((gp_top_layer->next==NULL)||(gp_top_layer->pContent==NULL))
	{
		return;
	}
	nextLayer=gp_top_layer->next;
	ClearScreen();
	TagMainMenu(gi_sel_menu);
	pCh=gp_top_layer->pScrAtt;
	for(i=gp_top_layer->rcArea.Top;i<=gp_top_layer->rcArea.Bottom;i++)
	{
		for(j=gp_top_layer->rcArea.Left;j<=gp_top_layer->rcArea.Right;j++)
		{
			gp_scr_att[i*SCR_COL+j]=*pCh;
			pCh++;
		}
	}
	free(gp_top_layer->pContent);
	free(gp_top_layer->pScrAtt);
	free(gp_top_layer);
	gp_top_layer=nextLayer;
	gi_sel_sub_menu=0;
	return;
}

void DrawBox(SMALL_RECT*pRc)
{
	char chBox[]={'+','-','*'};
	DWORD xin;
	COORD pos={pRc->Left,pRc->Top};
	WriteConsoleOutputCharacter(gh_std_out,&chBox[0],1,pos,&xin);
	for(pos.X=pRc->Left+1;pos.X<pRc->Right;pos.X++)
		WriteConsoleOutputCharacter(gh_std_out,&chBox[1],1,pos,&xin);
	pos.X=pRc->Right;
	WriteConsoleOutputCharacter(gh_std_out,&chBox[0],1,pos,&xin);
	for(pos.Y=pRc->Top+1;pos.Y<pRc->Bottom;pos.Y++)
	{
		pos.X=pRc->Left;
		WriteConsoleOutputCharacter(gh_std_out,&chBox[2],1,pos,&xin);
		pos.X=pRc->Right;
		WriteConsoleOutputCharacter(gh_std_out,&chBox[2],1,pos,&xin);
	}
	pos.X=pRc->Left;
	pos.Y=pRc->Bottom;

	WriteConsoleOutputCharacter(gh_std_out,&chBox[0],1,pos,&xin);
	for(pos.X=pRc->Left+1;pos.X<pRc->Right;pos.X++)
		WriteConsoleOutputCharacter(gh_std_out,&chBox[1],1,pos,&xin);
	pos.X=pRc->Right;

	WriteConsoleOutputCharacter(gh_std_out,&chBox[0],1,pos,&xin);
	return;
}

void TagSubMenu(int num)
{
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	int width;

	LocSubMenu(gi_sel_menu,&rcPop);
	if((num<1)||(num==gi_sel_sub_menu)||(num>rcPop.Bottom-rcPop.Top-1))
	{return;}
	pos.X=rcPop.Left+2;
	width=rcPop.Right-rcPop.Left-3;
	if(gi_sel_sub_menu!=0)
	{
		pos.Y=rcPop.Top+gi_sel_sub_menu;
		att=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED;
		FillConsoleOutputAttribute(gh_std_out,att,width,pos,&num_w);
	}

	pos.X=rcPop.Left+2;
	pos.Y=rcPop.Top+num;
	att=FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN;
	FillConsoleOutputAttribute(gh_std_out,att,width,pos,&num_w);
	gi_sel_sub_menu=num;
	return;
}

void LocSubMenu(int num,SMALL_RECT*rc)
{
	int i,len,loc=0;
	rc->Top=1;
	rc->Left=0;
	for(i=1;i<num;i++)
	{
		rc->Left+=strlen(ga_main_menu[i-1]);
		loc+=ga_sub_menu_count[i-1];
	}
	rc->Right=strlen(ga_sub_menu[loc]);
	for(i=1;i<ga_sub_menu_count[i-1];i++)
	{
		len=strlen(ga_sub_menu[loc]);
		if(rc->Right<len)
			rc->Right=len;
	}
	rc->Right+=rc->Left+3;
	rc->Bottom=rc->Top+ga_sub_menu_count[num-1]+1;
	if(rc->Right>=SCR_COL)
	{
		len=rc->Right-SCR_COL+1;
		rc->Left-=len;
		rc->Right=SCR_COL-1;
	}
	return;
}

void InitInterface()
{
    WORD att = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY|BACKGROUND_BLUE
;               /*黄色前景和蓝色背景*/
    SetConsoleTextAttribute(gh_std_out, att);   /*设置控制台屏幕缓冲区字符属性*/
    ClearScreen();      /* 清屏*/
    /*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*屏幕字符属性*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*弹出窗口的层号为0*/
    gp_top_layer->rcArea.Left = 0;  /*弹出窗口的区域为整个屏幕窗口*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;
    ShowMenu();     /*显示菜单栏*/
    ShowState();    /*显示状态栏*/

    return;
}

void ShowState(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int i;
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SMALL_RECT rcMenu = {0, bInfo.dwSize.Y-1, size.X-1, bInfo.dwSize.Y-1};
    if (gp_buff_stateBar_info == NULL)
    {
        gp_buff_stateBar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
        ReadConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    }
    for (i=0; i<size.X; i++)
    {
        (gp_buff_stateBar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
                                                | BACKGROUND_RED;
        /*
                ch = (char)((gp_buff_stateBar_info+i)->Char.AsciiChar);
                if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
                {
                    (gp_buff_stateBar_info+i)->Attributes |= FOREGROUND_RED;
                }
        */
    }
    WriteConsoleOutput(gh_std_out, gp_buff_stateBar_info, size, pos, &rcMenu);
    return;
}

BOOL ExitSys(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop;
    COORD pos;
    WORD att;
    char *pCh[] = {"确认退出系统吗？", "确定    取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE ;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{rcPop.Left+3, rcPop.Top+2},
        {rcPop.Left+5, rcPop.Top+5}
    };
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {{
            rcPop.Left + 5, rcPop.Top + 5,
            rcPop.Left + 8, rcPop.Top + 5
        },
        {
            rcPop.Left + 13, rcPop.Top + 5,
            rcPop.Left + 16, rcPop.Top + 5
        }
    };
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&rcPop, att, &labels, &areas);

    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
	DWORD xin;
    FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-1, pos,&xin);

    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }
    PopOff();

    return bRet;
}

int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cNo, cTag, cSort;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/

    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {
        /*循环*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if ((inRec.EventType == MOUSE_EVENT) &&
                (inRec.Event.MouseEvent.dwButtonState
                 == FROM_LEFT_1ST_BUTTON_PRESSED))
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
            cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;

            if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
            {
                *piHot = cTag;
                SetHotPoint(pHotArea, *piHot);
                if (cSort == 0)
                {
                    iRet = 13;
                    break;
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {
                    /*方向键(左、上、右、下)的处理*/
                case 37:
                    arrow = 1;
                    break;
                case 38:
                    arrow = 2;
                    break;
                case 39:
                    arrow = 3;
                    break;
                case 40:
                    arrow = 4;
                    break;
                }
                if (arrow > 0)
                {
                    num = *piHot;
                    while (TRUE)
                    {
                        if (arrow < 3)
                        {
                            num--;
                        }
                        else
                        {
                            num++;
                        }
                        if ((num < 1) || (num > pHotArea->num) ||
                                ((arrow % 2) && (pHotArea->pArea[num-1].Top
                                                 == pHotArea->pArea[*piHot-1].Top)) || ((!(arrow % 2))
                                                         && (pHotArea->pArea[num-1].Top
                                                             != pHotArea->pArea[*piHot-1].Top)))
                        {
                            break;
                        }
                    }
                    if (num > 0 && num <= pHotArea->num)
                    {
                        *piHot = num;
                        SetHotPoint(pHotArea, *piHot);
                    }
                }
            }
            else if (vkc == 27)
            {
                /*ESC键*/
                iRet = 27;
                break;
            }
            else if (vkc == 13 || vkc == 32)
            {
                /*回车键或空格表示按下当前按钮*/
                iRet = 13;
                break;
            }
        }
    }
    return iRet;
}

void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    DWORD written;
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;

    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    for (i=0; i<pHotArea->num; i++)
    {
        /*将按钮类热区置为白底黑字*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {
            /*热区是按钮类*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos,  &written);
        }
    }

    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {
        /*被激活热区是按钮类*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos,  &written);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {
        /*被激活热区是文本框类*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}


int CreatList(PHARMACY_NODE * * phead) /*创建链表*/
{
	PHARMACY_NODE * hd=NULL,* pPharmacyNode;
	MED_NODE * pMedNode;
	IM_INFORMATION * pImInformation;
	FILE * pFile;
	int find;
	int re=0;
	if((pFile=fopen("药房信息.dat","rb"))==NULL)
	{
		printf("\n药房信息数据文件打开失败!\n");
		return re;
	}
	printf("\n药房信息数据文件打开成功!\n");

	while(!feof(pFile))
	{
		pPharmacyNode=(PHARMACY_NODE * )malloc(sizeof(PHARMACY_NODE));
		fread(pPharmacyNode,sizeof(PHARMACY_NODE),1,pFile);
		pPharmacyNode->snext=NULL;
		pPharmacyNode->next=hd;
		hd=pPharmacyNode;
	}
	fclose(pFile);
	if(hd==NULL)
	{
		printf("\n药房信息数据文件加载失败!\n");
		return re;
	}
	printf("\n药房信息数据文件加载成功!\n");
	* phead=hd;
	re+=4;
	if((pFile=fopen("药品信息.dat","rb"))==NULL)
	{
		printf("药品信息数据文件打开失败!\n");
		return re;
	}
	printf("药品信息数据文件打开成功!\n");
	re+=8;

	while(!feof(pFile))
	{
		pMedNode=(MED_NODE *)malloc(sizeof(MED_NODE));
		fread(pMedNode,sizeof(MED_NODE),1,pFile);
		pMedNode->cnext=NULL;
		pPharmacyNode=hd;
		while(pPharmacyNode!=NULL&&strcmp(pPharmacyNode->pharmacy_id,pMedNode->pharmacy_id)!=0)
		{
			pPharmacyNode=pPharmacyNode->next;
		}
		if(pPharmacyNode!=NULL)
		{
			pMedNode->next=pPharmacyNode->snext;
			pPharmacyNode->snext=pMedNode;
		}
		else
		{
			free(pMedNode);
		}
	}
	fclose(pFile);
	if((pFile=fopen("即时药品信息.dat","rb"))==NULL)
	{
		printf("即时药品信息数据文件打开失败!\n");
		return re;
	}
	printf("即时药品信息数据文件打开成功!\n");
	re+=16;
	while(!feof(pFile))
	{
		pImInformation=(IM_INFORMATION * )malloc(sizeof(IM_INFORMATION));
		fread(pImInformation,sizeof(IM_INFORMATION),1,pFile);
		pPharmacyNode=hd;
		find=0;
		while(pPharmacyNode!=NULL&&find==0)
		{
			pMedNode=pPharmacyNode->snext;
			while(pMedNode!=NULL&&find==0)
			{
				if(strcmp(pMedNode->med_id,pImInformation->med_id)==0)
				{
					find=1;
					break;
				}
				pMedNode=pMedNode->next;
		    }
			pPharmacyNode=pPharmacyNode->next;
		}
		if(find)
		{
			pImInformation->next=pMedNode->cnext;
			pMedNode->cnext=pImInformation;
		}
		else
		{
			free(pImInformation);
		}
	}
	fclose(pFile);
	return re;
}

int FreeList(PHARMACY_NODE * * phead)  /*释放链表*/
{
	PHARMACY_NODE * p1,* p1f;
	MED_NODE * p2,* p2f;
	IM_INFORMATION * p3,* p3f;
	for(p1=* phead;p1!=NULL;)
	{
		p1f=p1->next;
		for(p2=p1->snext;p2!=NULL;)
		{
			p2f=p2->next;
			free(p2->cnext);
			free(p2);
			p2=p2f;
		}
		free(p1);
		p1=p1f;
	}
	* phead=NULL;
	return 1;
}

BOOL InsertNewPhaNode() /*插入新药房*/
{
	PHARMACY_NODE * ps;
	ps=(PHARMACY_NODE *)malloc(sizeof(PHARMACY_NODE));
	printf("请输入药房名称：");
	gets(ps->pharmacy_name);
	printf("请输入药房编号：");
	gets(ps->pharmacy_id);
	printf("请输入药房负责人姓名：");
	gets(ps->pharmacy_charger_name);
	getchar();
	ps->snext=NULL;
	ps->next=gp_head;
	gp_head=ps;
	printf("添加成功！");
	return TRUE;
}

BOOL InsertNewMedNode() /*插入新药品*/
{
	  PHARMACY_NODE * ppharmacy_node=NULL,* ps;
	  MED_NODE * pmed_node;
	  IM_INFORMATION * im_info;
	  pmed_node=(MED_NODE *)malloc(sizeof(MED_NODE));
	  printf("请输入药品编号：");
	  gets(pmed_node->med_id);
	  printf("请输入药品名字：");
	  gets(pmed_node->med_name);
	  printf("请输入药品生产日期：");
	  gets(pmed_node->type);
	  printf("请输入药房编号：");
	  gets(pmed_node->pharmacy_id);
	  printf("请输入日期：");
	  gets(pmed_node->date);
	  printf("请输入进货量：");
	  scanf("%d",&pmed_node->first_quantity);
	  getchar();
	  printf("请输入单价：");
	  scanf("%f",&pmed_node->price);
	  getchar();
	  pmed_node->cost=(pmed_node->price)*(pmed_node->first_quantity);
	  printf("请输入零售价：");
	  scanf("%f",&pmed_node->sel_price);
	  getchar();
	  printf("请输入药品功效：");
	  gets(pmed_node->effect);
	  printf("请输入药品生产公司：");
	  gets(pmed_node->product_firm);
	  pmed_node->cnext=NULL;
	  pmed_node->next=NULL;
	  for(ps=gp_head;ps!=NULL;ps=ps->next)
	  {
		  if(strcmp(ps->pharmacy_id,pmed_node->pharmacy_id)==0)
		  {
			  ppharmacy_node=ps;
			  break;
		  }
	  }
	  if(ppharmacy_node!=NULL)
	  {
		  im_info=(IM_INFORMATION *)malloc(sizeof(IM_INFORMATION));
		  strcpy(im_info->med_id,pmed_node->med_id);
		  strcpy(im_info->med_name,pmed_node->med_name);
		  im_info->rem_amount=pmed_node->first_quantity;
		  im_info->amount_out=0;
		  im_info->next=NULL;
		  pmed_node->cnext=im_info;
		  if(ppharmacy_node->snext!=NULL)
		  {
			  im_info->next=ppharmacy_node->snext->cnext;
		      pmed_node->next=ppharmacy_node->snext;
		      ppharmacy_node->snext=pmed_node;
		  }
		  else
			  ppharmacy_node->snext=pmed_node;
		  printf("添加成功!\n");
	  }
	  else
		  printf("添加失败!\n");
	  return TRUE;
}

BOOL SeekPharmacyNode() /*根据药品ID寻找对应药房*/
{
	PHARMACY_NODE * pPha_node;
	int find=0;
	char pharmacy_id[5];
	printf("请输入药品编号：");
	gets(pharmacy_id);
	for(pPha_node=gp_head;pPha_node!=NULL;pPha_node=pPha_node->next)
	{
		if(strcmp(pPha_node->pharmacy_id,pharmacy_id)==0)
		{
			find=1;
		    break;
		}
		if(find)
			break;
	}
	if(find)
	{
		printf("药房名：/s%s\n",pPha_node->pharmacy_name);
		printf("药房编号：/s%s\n",pPha_node->pharmacy_id);
	}
	else
		printf("未找到！\n");
	return TRUE;
}
BOOL ModfiMedIm() /*药品变化*/
{
	int find=0,num;
	char med_id[12];
	PHARMACY_NODE * pphar_node;
	MED_NODE * pmed_node_tem;
	printf("请输入药品编号：");
	gets(med_id);
	printf("请输入药品出库量：");
	scanf("%d",&num);
	getchar();
	for(pphar_node=gp_head;pphar_node!=NULL;pphar_node=pphar_node->next)
	{
		pmed_node_tem=pphar_node->snext;
		while(pmed_node_tem!=NULL)
	    {
		     if(strcmp(pmed_node_tem->med_id ,med_id)==0)
		     {
			      if((pmed_node_tem->cnext->rem_amount-num)>=0)
			      {
				        find=1;
				        pmed_node_tem->cnext->rem_amount-=num;
				        pmed_node_tem->cnext->amount_out+=num;
						break;
			       }
		      }
		      pmed_node_tem=pmed_node_tem->next;
	    }
		if(find)
			break;
	}
	if(find)
		printf("出库过程成功!\n");
	else
		printf("出库过程失败!\n");
	return TRUE;
}


BOOL DelMedNode() /*删除药品*/
{
	int find=0;
	char med_id[12];
	PHARMACY_NODE * pPha_node;
	MED_NODE * pmed_tem,* ps;
	printf("请输入要删除的药品编号：");
	gets(med_id);
	for(pPha_node=gp_head;pPha_node!=NULL;pPha_node=pPha_node->next)
	{
		pmed_tem=pPha_node->snext;
		if(pmed_tem!=NULL)
		{
			if(strcmp(pmed_tem->med_id ,med_id)==0)
			{
				pPha_node->snext=pmed_tem->next;
				find=1;
				break;
			}
		}
		else
			continue;
		while(pmed_tem!=NULL)
		{
			ps=pmed_tem;
			pmed_tem=pmed_tem->next;
			if(pmed_tem!=NULL)
			{
				if(strcmp(pmed_tem->med_id ,med_id)==0)
			    {
					ps->next=pmed_tem->next;
					if(pmed_tem->next!=NULL)
						ps->cnext->next=pmed_tem->next->cnext;
					else
						ps->cnext->next=NULL;
					find=1;
					break;
			    }
			}
			else
				break;
		}
		if(find)
			break;
	}
	if(find)
	{
		free(pmed_tem->cnext);
		free(pmed_tem);
		printf("删除成功!\n");
	}
	else
		printf("删除失败!\n");
	return TRUE;
}

BOOL QurryPhaMedQun() /*查询药房药品储量*/
{
	int find=0,type=0,b=0;
	char pha_id[5];
	IM_INFORMATION * im_info=NULL;
	PHARMACY_NODE * pha_node;
	printf("请输入要查询的药房编号：");
	gets(pha_id);
	for(pha_node=gp_head;pha_node!=NULL;pha_node=pha_node->next)
	{
		if(strcmp(pha_id,pha_node->pharmacy_id)==0)
		{
			if(pha_node->snext!=NULL)
				for(im_info=pha_node->snext->cnext;im_info!=NULL;im_info=im_info->next)
				{
					type++;
				    b+=im_info->rem_amount;
			    }
			find=1;
		}
		if(find)
			break;
	}
	if(find)
		printf("\n编号为%s的药房内存药品种类%d，总数量%d!\n",pha_id,type,b);
	else
		printf("未找到！\n");
	return TRUE;
}

BOOL QurryMedRemAmo() /*清点药品现存量*/
{
	int m,find=0;
	char med_id[12];
	PHARMACY_NODE * pha_node;
	MED_NODE * med_node;
	IM_INFORMATION * im_info;
	printf("请输入药品编号：");
	gets(med_id);
	for(pha_node=gp_head;pha_node!=NULL;pha_node=pha_node->next)
	{
		for(med_node=pha_node->snext;med_node!=NULL;med_node=med_node->next)
		{
			if(strcmp(med_node->med_id,med_id)==0)
			{
				find=1;
				m=med_node->cnext->rem_amount;
				break;
			}
		}
		if(find)
			break;
	}
	if(find)
		printf("\n编号为%s的药品现存量为%d!\n",med_id,m);
	else
		printf("未找到！\n");
	return TRUE;
}

BOOL QurryMedAmoOut()/*查询药品出货量*/
{
	int m,find=0;
	char med_id[12];
	PHARMACY_NODE * pha_node;
	MED_NODE * med_node;
	IM_INFORMATION * im_info;
	printf("请输入药品编号：");
	gets(med_id);
	for(pha_node=gp_head;pha_node!=NULL;pha_node=pha_node->next)
	{
		for(med_node=pha_node->snext;med_node!=NULL;med_node=med_node->next)
		{
			if(strcmp(med_node->med_id,med_id)==0)
			{
				find=1;
				m=med_node->cnext->amount_out;
				break;
			}
		}
		if(find)
			break;
	}
	if(find)
		printf("\n编号为%s的药品出货量为%d!\n",med_id,m);
	else
		printf("未找到！\n");
	return TRUE;
}

BOOL QurryPhaBasIn()/*查询药房信息*/
{
	int find=0;
	char pha_id[5];
	PHARMACY_NODE * pha_node;
	printf("请输入药房编号：");
	gets(pha_id);
	for(pha_node=gp_head;pha_node!=NULL;pha_node=pha_node->next)
	{
		if(strcmp(pha_node->pharmacy_id,pha_id)==0)
		{
			find=1;
			break;
		}
	}
	if(find)
	{
		printf("药房名:    %s\n",pha_node->pharmacy_name);
		printf("药房编号:    %s\n",pha_node->pharmacy_id);
		printf("负责人:%s\n",pha_node->pharmacy_charger_name);
	}
	else
	    printf("未找到！\n");
	return TRUE;
}

BOOL QurryMedBasIn()/*查询药品信息*/
{
	int find=0;
	char med_id[12];
	MED_NODE * med_node;
	PHARMACY_NODE * pha_node;
	printf("请输入药品编号：");
	gets(med_id);
	for(pha_node=gp_head;pha_node!=NULL;pha_node=pha_node->next)
	{
		for(med_node=pha_node->snext;med_node!=NULL;med_node=med_node->next)
		{
			if(strcmp(med_id,med_node->med_id)==0)
			{
				find=1;
				break;
			}
		}
		if(find)
			break;
	}
	if(find)
	{
		printf("名称:        %s\n",med_node->med_name);
		printf("编号:          %s\n",med_node->med_id);
		printf("生产日期:    %s\n",med_node->inven_date);
		printf("存放药房:    %s\n",med_node->pharmacy_id);
		printf("入库时间:    %s\n",med_node->date);
		printf("进货量:      %d\n",med_node->first_quantity);
		printf("进货价:        %f\n",med_node->price);
		printf("零售价:        %f\n",med_node->sel_price);
		printf("总价:        %f\n",med_node->cost);
		printf("功效:        %s\n",med_node->effect);
		printf("生产厂商:    %s\n",med_node->product_firm);
	}
	else
	    printf("未找到！\n");
	return TRUE;
}

BOOL QurryPhaCha() /*查询药房负责人*/
{
	int find=0;
	char pha_id[5];
	PHARMACY_NODE * pha_node;
	printf("请输入药房编号：");
	gets(pha_id);
	for(pha_node=gp_head;pha_node!=NULL;pha_node=pha_node->next)
	{
		if(strcmp(pha_node->pharmacy_id,pha_id)==0)
		{
			find=1;
			break;
		}
	}
	if(find)
	{
		printf("负责人:  %s\n",pha_node->pharmacy_charger_name);
    }
	else
	    printf("未找到!\n");
	return TRUE;
}


void RunSys(PHARMACY_NODE * * pphd)
{
	INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt,no;      /*cNo:字符单元层号, cAtt:字符单元属性*/
    char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/

    while (bRet)
    {
        /*从控制台输入缓冲区中读一条记录*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        pos=inRec.Event.MouseEvent.dwMousePosition;
         if(pos.X>2&&pos.X<9&&pos.Y==0)no=1;
            if(pos.X>13&&pos.X<24&&pos.Y==0)no=2;
            if(pos.X>28&&pos.X<39&&pos.Y==0)no=3;
            if(pos.X>43&&pos.X<54&&pos.Y==0)no=4;
            if(pos.X>58&&pos.X<69&&pos.Y==0)no=5;
        if(inRec.EventType==MOUSE_EVENT)
        {
            switch(no)
            {
                case 1:TagMainMenu(no);  PopMenu(no);
                       break;
                case 2:TagMainMenu(no);  PopMenu(no);
                       break;
                case 3:TagMainMenu(no);  PopMenu(no);
                       break;
                case 4:TagMainMenu(no);  PopMenu(no);
                       break;
                case 5:TagMainMenu(no);  PopMenu(no);
                       break;
            }
        }

        if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*取该位置的层号*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/
            if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
            {
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    PopMenu(cAtt);       /*弹出鼠标所在主菜单项对应的子菜单*/
                }
            }
            else if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
            {
                TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
            }

            if (inRec.Event.MouseEvent.dwButtonState
                == FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左边第一键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(主菜单项字符单元)*/
                    {
                        PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
                    }
                    /*如果该位置不属于主菜单项字符单元，且有子菜单弹出*/
                    else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                    }
                }
                else /*层号不为0，表明该位置被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区(子菜单项字符单元)*/
                    {
                        PopOff(); /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/

                        /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*如果记录由按键产生*/
                 && inRec.Event.KeyEvent.bKeyDown) /*且键被按下*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*获取按键的虚拟键码*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*获取按键的ASC码*/

            /*系统快捷键的处理*/
            if (vkc == 112) /*如果按下F1键*/
            {
                if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
                bRet = ExeFunction(5, 1);  /*运行帮助主题功能函数*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*如果按下左或右Alt键*/
                switch (vkc)  /*判断组合键Alt+字母*/
                {
                    case 88:  /*Alt+X 退出*/
                        if (gp_top_layer->LayerNo != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
                        bRet = ExeFunction(1,4);
                        break;
                    case 70:  /*Alt+F*/
                        PopMenu(1);
                        break;
                    case 77: /*Alt+M*/
                        PopMenu(2);
                        break;
                    case 81: /*Alt+Q*/
                        PopMenu(3);
                        break;
                    case 83: /*Alt+S*/
                        PopMenu(4);
                        break;
                    case 72: /*Alt+H*/
                        PopMenu(5);
                        break;
                }
            }
            else if (asc == 0) /*其他控制键的处理*/
            {
                if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
                {
                    switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu == 0)
                            {
                                gi_sel_menu = 5;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu == 6)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            break;
                        case 40:
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else  /*已弹出子菜单时*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                    switch (vkc) /*方向键(左、右、上、下)的处理*/
                    {
                        case 37:
                            gi_sel_menu--;
                            if (gi_sel_menu < 1)
                            {
                                gi_sel_menu = 5;
                            }
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 38:
                            num = gi_sel_sub_menu - 1;
                            if (num < 1)
                            {
                                num = ga_sub_menu_count[gi_sel_menu-1];
                            }
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)
                            {
                                num--;
                            }
                            TagSubMenu(num);
                            break;
                        case 39:
                            gi_sel_menu++;
                            if (gi_sel_menu > 5)
                            {
                                gi_sel_menu = 1;
                            }
                            TagMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                        case 40:
                            num = gi_sel_sub_menu + 1;
                            if (num > ga_sub_menu_count[gi_sel_menu-1])
                            {
                                num = 1;
                            }
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)
                            {
                                num++;
                            }
                            TagSubMenu(num);
                            break;
                    }
                }
            }
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*按下普通键*/
                if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
                {
                    switch (vkc)
                    {
                        case 70: /*f或F*/
                            PopMenu(1);
                            break;
                        case 77: /*m或M*/
                            PopMenu(2);
                            break;
                        case 81: /*q或Q*/
                            PopMenu(3);
                            break;
                        case 83: /*s或S*/
                            PopMenu(4);
                            break;
                        case 72: /*h或H*/
                            PopMenu(5);
                            break;
                        case 13: /*回车*/
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*已弹出子菜单时的键盘输入处理*/
                {
                    if (vkc == 27) /*如果按下ESC键*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*如果按下回车键*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*其他普通键的处理*/
                    {
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }

                        /*依次与当前子菜单中每一项的代表字符进行比较*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*如果匹配成功*/
                                PopOff();
                                gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i-loc+1);
                            }
                        }
                    }
                }
            }
        }
    }

}

BOOL ExeFunction(int m,int s)
{
	BOOL bRet=TRUE;
	BOOL(* pFunction[17])();
	int i,loc;
	pFunction[0]=SaveData;
	pFunction[1]=BackupData;
	pFunction[2]=RestoreData;
	pFunction[3]=ExitSys;
	pFunction[4]=InsertNewMedNode;
	pFunction[5]=ModfiMedIm;
	pFunction[6]=InsertNewPhaNode;
	pFunction[7]=DelMedNode;
	pFunction[8]=QurryPhaMedQun;
	pFunction[9]=QurryMedRemAmo;
	pFunction[10]=QurryMedAmoOut;
	pFunction[11]=QurryPhaBasIn;
	pFunction[12]=QurryMedBasIn;
	pFunction[13]=QurryPhaCha;
	pFunction[14]=HELP;
	pFunction[15]=SM;

	for(i=1,loc=0;i<m;i++)
	{
		loc+=ga_sub_menu_count[i-1];
	}
	loc+=s-1;
	if(pFunction[loc]!=NULL)
	{
		bRet=(* pFunction[loc])();
	}
	return bRet;
}

BOOL SaveData()
{
    FILE *fp=NULL,*fp1=NULL,*fp2=NULL;
    PHARMACY_NODE * p_roo=gp_head;
    MED_NODE * p_med=NULL;
    IM_INFORMATION * p_chg=NULL;
    InitInterface();
    fp=fopen("药房信息.dat","wb");
    fp1=fopen("药品信息.dat","wb");
    fp2=fopen("即时药品信息.dat","wb");
    while(p_roo)
    {
        p_med=p_roo->snext;
        while(p_med)
        {
            p_chg=p_med->cnext;
            while(p_chg)
            {
                fwrite(p_chg,sizeof(IM_INFORMATION),1,fp2);
                p_chg=p_chg->next;
            }
            fwrite(p_med,sizeof(MED_NODE),1,fp1);
            p_med=p_med->next;
        }
        fwrite(p_roo,sizeof(PHARMACY_NODE),1,fp);
        p_roo=p_roo->next;
    }
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    printf("\n\n\n\n\n\n\n\n\t\t\t\t数据保存成功!\n");
	return TRUE;
}

BOOL BackupData()
{
	FILE *fp=NULL,*fp1=NULL,*fp2=NULL,*fp3;
    PHARMACY_NODE * p_roo=gp_head;
    MED_NODE * p_med=NULL;
    IM_INFORMATION * p_chg=NULL;
    InitInterface();
	fp=fopen("药房信息备份.dat","wb");
	fp1=fopen("药品信息备份.dat","wb");
	fp2=fopen("即时药品信息备份.dat","wb");
	while(p_roo)
    {
        p_med=p_roo->snext;
        while(p_med)
        {
            p_chg=p_med->cnext;
            while(p_chg)
            {
                fwrite(p_chg,sizeof(IM_INFORMATION),1,fp2);
                p_chg=p_chg->next;
            }
            fwrite(p_med,sizeof(MED_NODE),1,fp1);
            p_med=p_med->next;
        }
        fwrite(p_roo,sizeof(PHARMACY_NODE),1,fp);
        p_roo=p_roo->next;
    }
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    printf("\n\n\n\n\n\n\n\n\t\t\t\t数据备份成功!\n");
	return TRUE;
}

BOOL RestoreData()
{
	FreeList(&gp_head);
	PHARMACY_NODE * hd=NULL,* pPharmacyNode;
	MED_NODE * pMedNode;
	IM_INFORMATION * pImInformation;
	FILE * pFile;
	int find;
	int re=0;
	if((pFile=fopen("药房信息备份.dat","rb"))==NULL)
	{
		printf("药房信息备份文件打开失败!\n");
		return re;
	}
	printf("药房信息备份文件打开成功!\n");

	while(!feof(pFile))
	{
		pPharmacyNode=(PHARMACY_NODE * )malloc(sizeof(PHARMACY_NODE));
		fread(pPharmacyNode,sizeof(PHARMACY_NODE),1,pFile);
		pPharmacyNode->snext=NULL;
		pPharmacyNode->next=hd;
		hd=pPharmacyNode;
	}
	fclose(pFile);
	if(hd==NULL)
	{
		printf("药房信息备份文件加载失败!\n");
		return re;
	}
	printf("药房信息数据备份加载成功!\n");
	gp_head=hd;
	re+=4;
	if((pFile=fopen("药品信息备份.dat","rb"))==NULL)
	{
		printf("药品信息备份文件打开失败!\n");
		return re;
	}
	printf("药品信息备份文件打开成功!\n");
	re+=8;

	while(!feof(pFile))
	{
		pMedNode=(MED_NODE *)malloc(sizeof(MED_NODE));
		fread(pMedNode,sizeof(MED_NODE),1,pFile);
		pMedNode->cnext=NULL;
		pPharmacyNode=hd;
		while(pPharmacyNode!=NULL&&strcmp(pPharmacyNode->pharmacy_id,pMedNode->pharmacy_id)!=0)
		{
			pPharmacyNode=pPharmacyNode->next;
		}
		if(pPharmacyNode!=NULL)
		{
			pMedNode->next=pPharmacyNode->snext;
			pPharmacyNode->snext=pMedNode;
		}
		else
		{
			free(pMedNode);
		}
	}
	fclose(pFile);
	if((pFile=fopen("即时药品信息备份.dat","rb"))==NULL)
	{
		printf("即时药品信息备份文件打开失败!\n");
		return re;
	}
	printf("即时药品信息备份文件打开成功!\n");
	re+=16;
	while(!feof(pFile))
	{
		pImInformation=(IM_INFORMATION * )malloc(sizeof(IM_INFORMATION));
		fread(pImInformation,sizeof(IM_INFORMATION),1,pFile);
		pPharmacyNode=hd;
		find=0;
		while(pPharmacyNode!=NULL&&find==0)
		{
			pMedNode=pPharmacyNode->snext;
			while(pMedNode!=NULL&&find==0)
			{
				if(strcmp(pMedNode->med_id,pImInformation->med_id)==0)
				{
					find=1;
					break;
				}
				pMedNode=pMedNode->next;
		    }
			pPharmacyNode=pPharmacyNode->next;
		}
		if(find)
		{
			pImInformation->next=pMedNode->cnext;
			pMedNode->cnext=pImInformation;
		}
		else
		{
			free(pImInformation);
		}
	}
	fclose(pFile);
	SaveData();
	printf("\n\n\n\n\n\n\n\n\t\t\t\t数据已恢复!\n");
	return re;
}

BOOL SM()
{
    printf("\n                              医药管理系统\n");
    printf("\n\n\n                              2012-10-12版\n");
    return getch();
}

BOOL HELP()
{
    printf("                          系统使用方法及说明\n");
    printf(" 文件选项功能：数据保存，数据备份，数据恢复，退出。\n");
    printf(" （1）数据保存：保存当前系统中数据到指定文件中；\n");
    printf(" （2）数据备份：将当前所有数据备份到指定文件夹，便于找回丢失数据；\n");
    printf(" （3）数据恢复：读取备份文件中数据并保存，恢复数据；\n");
    printf(" （4）退出系统。\n");
    printf("药品管理选项功能：新药入库，药品出库，新增药房和药品移除。\n");
    printf(" （1）新药入库：新增药品到药房中，包含药品的名字、生产日期、所属药房编号、进货量、进货价、零售价、功效、生产公司等信息；\n");
    printf(" （2）药品出库：在售出药品后，减少仓库内该药品数量；\n");
    printf(" （3）新增药房：新建药房，以存储药品；\n");
    printf(" （4）药品移除：在某药品下架后，移除该药品。\n");
    printf(" 查询选项功能：药房药品总量，药品现存量，药品出货量，药房基本信息，药品基本信息和药房负责人。\n");
    printf(" （1）药房药品总量：根据药房编号查询某药房当前所存药品总数量；\n");
    printf(" （2）药品现存量：根据药品编号查询某药品当前所剩总量；\n");
    printf(" （3）药品出货量：根据药品编号查询某药品截至当前已经售出数量；\n");
    printf(" （4）药房基本信息：根据药房编号，查询该药房名称、负责人等信息；\n");
    printf(" （5）药品基本信息：根据药品编号查询药品的名字、生产日期、所在药房、入库时间、进货量、进货价、零售价、总价、功效、生产公司等信息；\n");
    printf("  帮助选项功能：帮助主题。\n");
    printf("   帮助主题：供用户了解该系统使用方法。\n");
    printf(" 关于选项功能：关于…。\n");
    printf(" 关于…：本系统版本及版权声明。\n");
    return getch();
}
