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
;               /*��ɫǰ������ɫ����*/
    SetConsoleTextAttribute(gh_std_out, att);   /*���ÿ���̨��Ļ�������ַ�����*/
    ClearScreen();      /* ����*/
    /*��������������Ϣ��ջ������ʼ�������Ļ���ڵ�����һ�㵯������*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*��Ļ�ַ�����*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    gp_top_layer->LayerNo = 0;      /*�������ڵĲ��Ϊ0*/
    gp_top_layer->rcArea.Left = 0;  /*�������ڵ�����Ϊ������Ļ����*/
    gp_top_layer->rcArea.Top = 0;
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;
    gp_top_layer->pScrAtt = gp_scr_att;
    gp_top_layer->next = NULL;
    ShowMenu();     /*��ʾ�˵���*/
    ShowState();    /*��ʾ״̬��*/

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
    char *pCh[] = {"ȷ���˳�ϵͳ��", "ȷ��    ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE ;  /*�׵׺���*/
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
    int cNo, cTag, cSort;/*cNo:���, cTag:�������, cSort: ��������*/
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/

    SetHotPoint(pHotArea, *piHot);
    while (TRUE)
    {
        /*ѭ��*/
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
                    /*�����(���ϡ��ҡ���)�Ĵ���*/
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
                /*ESC��*/
                iRet = 27;
                break;
            }
            else if (vkc == 13 || vkc == 32)
            {
                /*�س�����ո��ʾ���µ�ǰ��ť*/
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

    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    for (i=0; i<pHotArea->num; i++)
    {
        /*����ť��������Ϊ�׵׺���*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {
            /*�����ǰ�ť��*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos,  &written);
        }
    }

    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {
        /*�����������ǰ�ť��*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos,  &written);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {
        /*�������������ı�����*/
        SetConsoleCursorPosition(gh_std_out, pos);
        GetConsoleCursorInfo(gh_std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(gh_std_out, &lpCur);
    }
}


int CreatList(PHARMACY_NODE * * phead) /*��������*/
{
	PHARMACY_NODE * hd=NULL,* pPharmacyNode;
	MED_NODE * pMedNode;
	IM_INFORMATION * pImInformation;
	FILE * pFile;
	int find;
	int re=0;
	if((pFile=fopen("ҩ����Ϣ.dat","rb"))==NULL)
	{
		printf("\nҩ����Ϣ�����ļ���ʧ��!\n");
		return re;
	}
	printf("\nҩ����Ϣ�����ļ��򿪳ɹ�!\n");

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
		printf("\nҩ����Ϣ�����ļ�����ʧ��!\n");
		return re;
	}
	printf("\nҩ����Ϣ�����ļ����سɹ�!\n");
	* phead=hd;
	re+=4;
	if((pFile=fopen("ҩƷ��Ϣ.dat","rb"))==NULL)
	{
		printf("ҩƷ��Ϣ�����ļ���ʧ��!\n");
		return re;
	}
	printf("ҩƷ��Ϣ�����ļ��򿪳ɹ�!\n");
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
	if((pFile=fopen("��ʱҩƷ��Ϣ.dat","rb"))==NULL)
	{
		printf("��ʱҩƷ��Ϣ�����ļ���ʧ��!\n");
		return re;
	}
	printf("��ʱҩƷ��Ϣ�����ļ��򿪳ɹ�!\n");
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

int FreeList(PHARMACY_NODE * * phead)  /*�ͷ�����*/
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

BOOL InsertNewPhaNode() /*������ҩ��*/
{
	PHARMACY_NODE * ps;
	ps=(PHARMACY_NODE *)malloc(sizeof(PHARMACY_NODE));
	printf("������ҩ�����ƣ�");
	gets(ps->pharmacy_name);
	printf("������ҩ����ţ�");
	gets(ps->pharmacy_id);
	printf("������ҩ��������������");
	gets(ps->pharmacy_charger_name);
	getchar();
	ps->snext=NULL;
	ps->next=gp_head;
	gp_head=ps;
	printf("��ӳɹ���");
	return TRUE;
}

BOOL InsertNewMedNode() /*������ҩƷ*/
{
	  PHARMACY_NODE * ppharmacy_node=NULL,* ps;
	  MED_NODE * pmed_node;
	  IM_INFORMATION * im_info;
	  pmed_node=(MED_NODE *)malloc(sizeof(MED_NODE));
	  printf("������ҩƷ��ţ�");
	  gets(pmed_node->med_id);
	  printf("������ҩƷ���֣�");
	  gets(pmed_node->med_name);
	  printf("������ҩƷ�������ڣ�");
	  gets(pmed_node->type);
	  printf("������ҩ����ţ�");
	  gets(pmed_node->pharmacy_id);
	  printf("���������ڣ�");
	  gets(pmed_node->date);
	  printf("�������������");
	  scanf("%d",&pmed_node->first_quantity);
	  getchar();
	  printf("�����뵥�ۣ�");
	  scanf("%f",&pmed_node->price);
	  getchar();
	  pmed_node->cost=(pmed_node->price)*(pmed_node->first_quantity);
	  printf("���������ۼۣ�");
	  scanf("%f",&pmed_node->sel_price);
	  getchar();
	  printf("������ҩƷ��Ч��");
	  gets(pmed_node->effect);
	  printf("������ҩƷ������˾��");
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
		  printf("��ӳɹ�!\n");
	  }
	  else
		  printf("���ʧ��!\n");
	  return TRUE;
}

BOOL SeekPharmacyNode() /*����ҩƷIDѰ�Ҷ�Ӧҩ��*/
{
	PHARMACY_NODE * pPha_node;
	int find=0;
	char pharmacy_id[5];
	printf("������ҩƷ��ţ�");
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
		printf("ҩ������/s%s\n",pPha_node->pharmacy_name);
		printf("ҩ����ţ�/s%s\n",pPha_node->pharmacy_id);
	}
	else
		printf("δ�ҵ���\n");
	return TRUE;
}
BOOL ModfiMedIm() /*ҩƷ�仯*/
{
	int find=0,num;
	char med_id[12];
	PHARMACY_NODE * pphar_node;
	MED_NODE * pmed_node_tem;
	printf("������ҩƷ��ţ�");
	gets(med_id);
	printf("������ҩƷ��������");
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
		printf("������̳ɹ�!\n");
	else
		printf("�������ʧ��!\n");
	return TRUE;
}


BOOL DelMedNode() /*ɾ��ҩƷ*/
{
	int find=0;
	char med_id[12];
	PHARMACY_NODE * pPha_node;
	MED_NODE * pmed_tem,* ps;
	printf("������Ҫɾ����ҩƷ��ţ�");
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
		printf("ɾ���ɹ�!\n");
	}
	else
		printf("ɾ��ʧ��!\n");
	return TRUE;
}

BOOL QurryPhaMedQun() /*��ѯҩ��ҩƷ����*/
{
	int find=0,type=0,b=0;
	char pha_id[5];
	IM_INFORMATION * im_info=NULL;
	PHARMACY_NODE * pha_node;
	printf("������Ҫ��ѯ��ҩ����ţ�");
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
		printf("\n���Ϊ%s��ҩ���ڴ�ҩƷ����%d��������%d!\n",pha_id,type,b);
	else
		printf("δ�ҵ���\n");
	return TRUE;
}

BOOL QurryMedRemAmo() /*���ҩƷ�ִ���*/
{
	int m,find=0;
	char med_id[12];
	PHARMACY_NODE * pha_node;
	MED_NODE * med_node;
	IM_INFORMATION * im_info;
	printf("������ҩƷ��ţ�");
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
		printf("\n���Ϊ%s��ҩƷ�ִ���Ϊ%d!\n",med_id,m);
	else
		printf("δ�ҵ���\n");
	return TRUE;
}

BOOL QurryMedAmoOut()/*��ѯҩƷ������*/
{
	int m,find=0;
	char med_id[12];
	PHARMACY_NODE * pha_node;
	MED_NODE * med_node;
	IM_INFORMATION * im_info;
	printf("������ҩƷ��ţ�");
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
		printf("\n���Ϊ%s��ҩƷ������Ϊ%d!\n",med_id,m);
	else
		printf("δ�ҵ���\n");
	return TRUE;
}

BOOL QurryPhaBasIn()/*��ѯҩ����Ϣ*/
{
	int find=0;
	char pha_id[5];
	PHARMACY_NODE * pha_node;
	printf("������ҩ����ţ�");
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
		printf("ҩ����:    %s\n",pha_node->pharmacy_name);
		printf("ҩ�����:    %s\n",pha_node->pharmacy_id);
		printf("������:%s\n",pha_node->pharmacy_charger_name);
	}
	else
	    printf("δ�ҵ���\n");
	return TRUE;
}

BOOL QurryMedBasIn()/*��ѯҩƷ��Ϣ*/
{
	int find=0;
	char med_id[12];
	MED_NODE * med_node;
	PHARMACY_NODE * pha_node;
	printf("������ҩƷ��ţ�");
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
		printf("����:        %s\n",med_node->med_name);
		printf("���:          %s\n",med_node->med_id);
		printf("��������:    %s\n",med_node->inven_date);
		printf("���ҩ��:    %s\n",med_node->pharmacy_id);
		printf("���ʱ��:    %s\n",med_node->date);
		printf("������:      %d\n",med_node->first_quantity);
		printf("������:        %f\n",med_node->price);
		printf("���ۼ�:        %f\n",med_node->sel_price);
		printf("�ܼ�:        %f\n",med_node->cost);
		printf("��Ч:        %s\n",med_node->effect);
		printf("��������:    %s\n",med_node->product_firm);
	}
	else
	    printf("δ�ҵ���\n");
	return TRUE;
}

BOOL QurryPhaCha() /*��ѯҩ��������*/
{
	int find=0;
	char pha_id[5];
	PHARMACY_NODE * pha_node;
	printf("������ҩ����ţ�");
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
		printf("������:  %s\n",pha_node->pharmacy_charger_name);
    }
	else
	    printf("δ�ҵ�!\n");
	return TRUE;
}


void RunSys(PHARMACY_NODE * * pphd)
{
	INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num;
    int cNo, cAtt,no;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
    char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/

    while (bRet)
    {
        /*�ӿ���̨���뻺�����ж�һ����¼*/
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

        if (inRec.EventType == MOUSE_EVENT) /*�����¼������¼�����*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;  /*��ȡ�������λ��*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*ȡ��λ�õĲ��*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*ȡ���ַ���Ԫ����*/
            if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
            {
                if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    PopMenu(cAtt);       /*��������������˵����Ӧ���Ӳ˵�*/
                }
            }
            else if (cAtt > 0) /*�������λ��Ϊ�����Ӳ˵��Ĳ˵����ַ���Ԫ*/
            {
                TagSubMenu(cAtt); /*�ڸ��Ӳ˵�������ѡ�б��*/
            }

            if (inRec.Event.MouseEvent.dwButtonState
                == FROM_LEFT_1ST_BUTTON_PRESSED) /*������������ߵ�һ��*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������(���˵����ַ���Ԫ)*/
                    {
                        PopMenu(cAtt);   /*��������������˵����Ӧ���Ӳ˵�*/
                    }
                    /*�����λ�ò��������˵����ַ���Ԫ�������Ӳ˵�����*/
                    else if (gp_top_layer->LayerNo > 0)
                    {
                        PopOff();            /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                    }
                }
                else /*��Ų�Ϊ0��������λ�ñ������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������(�Ӳ˵����ַ���Ԫ)*/
                    {
                        PopOff(); /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/

                        /*ִ�ж�Ӧ���ܺ���:gi_sel_menu���˵����,cAtt�Ӳ˵����*/
                        bRet = ExeFunction(gi_sel_menu, cAtt);
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState
                     == RIGHTMOST_BUTTON_PRESSED) /*�����������Ҽ�*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT  /*�����¼�ɰ�������*/
                 && inRec.Event.KeyEvent.bKeyDown) /*�Ҽ�������*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode; /*��ȡ�������������*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar; /*��ȡ������ASC��*/

            /*ϵͳ��ݼ��Ĵ���*/
            if (vkc == 112) /*�������F1��*/
            {
                if (gp_top_layer->LayerNo != 0) /*�����ǰ���Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
                bRet = ExeFunction(5, 1);  /*���а������⹦�ܺ���*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*������������Alt��*/
                switch (vkc)  /*�ж���ϼ�Alt+��ĸ*/
                {
                    case 88:  /*Alt+X �˳�*/
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
            else if (asc == 0) /*�������Ƽ��Ĵ���*/
            {
                if (gp_top_layer->LayerNo == 0) /*���δ�����Ӳ˵�*/
                {
                    switch (vkc) /*�������(���ҡ���)������Ӧ�������Ƽ�*/
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
                else  /*�ѵ����Ӳ˵�ʱ*/
                {
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                    {
                        loc += ga_sub_menu_count[i-1];
                    }  /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                    switch (vkc) /*�����(���ҡ��ϡ���)�Ĵ���*/
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
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*������ͨ��*/
                if (gp_top_layer->LayerNo == 0)  /*���δ�����Ӳ˵�*/
                {
                    switch (vkc)
                    {
                        case 70: /*f��F*/
                            PopMenu(1);
                            break;
                        case 77: /*m��M*/
                            PopMenu(2);
                            break;
                        case 81: /*q��Q*/
                            PopMenu(3);
                            break;
                        case 83: /*s��S*/
                            PopMenu(4);
                            break;
                        case 72: /*h��H*/
                            PopMenu(5);
                            break;
                        case 13: /*�س�*/
                            PopMenu(gi_sel_menu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
                {
                    if (vkc == 27) /*�������ESC��*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*������»س���*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*������ͨ���Ĵ���*/
                    {
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                        {
                            loc += ga_sub_menu_count[i-1];
                        }

                        /*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*���ƥ��ɹ�*/
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
    fp=fopen("ҩ����Ϣ.dat","wb");
    fp1=fopen("ҩƷ��Ϣ.dat","wb");
    fp2=fopen("��ʱҩƷ��Ϣ.dat","wb");
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
    printf("\n\n\n\n\n\n\n\n\t\t\t\t���ݱ���ɹ�!\n");
	return TRUE;
}

BOOL BackupData()
{
	FILE *fp=NULL,*fp1=NULL,*fp2=NULL,*fp3;
    PHARMACY_NODE * p_roo=gp_head;
    MED_NODE * p_med=NULL;
    IM_INFORMATION * p_chg=NULL;
    InitInterface();
	fp=fopen("ҩ����Ϣ����.dat","wb");
	fp1=fopen("ҩƷ��Ϣ����.dat","wb");
	fp2=fopen("��ʱҩƷ��Ϣ����.dat","wb");
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
    printf("\n\n\n\n\n\n\n\n\t\t\t\t���ݱ��ݳɹ�!\n");
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
	if((pFile=fopen("ҩ����Ϣ����.dat","rb"))==NULL)
	{
		printf("ҩ����Ϣ�����ļ���ʧ��!\n");
		return re;
	}
	printf("ҩ����Ϣ�����ļ��򿪳ɹ�!\n");

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
		printf("ҩ����Ϣ�����ļ�����ʧ��!\n");
		return re;
	}
	printf("ҩ����Ϣ���ݱ��ݼ��سɹ�!\n");
	gp_head=hd;
	re+=4;
	if((pFile=fopen("ҩƷ��Ϣ����.dat","rb"))==NULL)
	{
		printf("ҩƷ��Ϣ�����ļ���ʧ��!\n");
		return re;
	}
	printf("ҩƷ��Ϣ�����ļ��򿪳ɹ�!\n");
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
	if((pFile=fopen("��ʱҩƷ��Ϣ����.dat","rb"))==NULL)
	{
		printf("��ʱҩƷ��Ϣ�����ļ���ʧ��!\n");
		return re;
	}
	printf("��ʱҩƷ��Ϣ�����ļ��򿪳ɹ�!\n");
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
	printf("\n\n\n\n\n\n\n\n\t\t\t\t�����ѻָ�!\n");
	return re;
}

BOOL SM()
{
    printf("\n                              ҽҩ����ϵͳ\n");
    printf("\n\n\n                              2012-10-12��\n");
    return getch();
}

BOOL HELP()
{
    printf("                          ϵͳʹ�÷�����˵��\n");
    printf(" �ļ�ѡ��ܣ����ݱ��棬���ݱ��ݣ����ݻָ����˳���\n");
    printf(" ��1�����ݱ��棺���浱ǰϵͳ�����ݵ�ָ���ļ��У�\n");
    printf(" ��2�����ݱ��ݣ�����ǰ�������ݱ��ݵ�ָ���ļ��У������һض�ʧ���ݣ�\n");
    printf(" ��3�����ݻָ�����ȡ�����ļ������ݲ����棬�ָ����ݣ�\n");
    printf(" ��4���˳�ϵͳ��\n");
    printf("ҩƷ����ѡ��ܣ���ҩ��⣬ҩƷ���⣬����ҩ����ҩƷ�Ƴ���\n");
    printf(" ��1����ҩ��⣺����ҩƷ��ҩ���У�����ҩƷ�����֡��������ڡ�����ҩ����š��������������ۡ����ۼۡ���Ч��������˾����Ϣ��\n");
    printf(" ��2��ҩƷ���⣺���۳�ҩƷ�󣬼��ٲֿ��ڸ�ҩƷ������\n");
    printf(" ��3������ҩ�����½�ҩ�����Դ洢ҩƷ��\n");
    printf(" ��4��ҩƷ�Ƴ�����ĳҩƷ�¼ܺ��Ƴ���ҩƷ��\n");
    printf(" ��ѯѡ��ܣ�ҩ��ҩƷ������ҩƷ�ִ�����ҩƷ��������ҩ��������Ϣ��ҩƷ������Ϣ��ҩ�������ˡ�\n");
    printf(" ��1��ҩ��ҩƷ����������ҩ����Ų�ѯĳҩ����ǰ����ҩƷ��������\n");
    printf(" ��2��ҩƷ�ִ���������ҩƷ��Ų�ѯĳҩƷ��ǰ��ʣ������\n");
    printf(" ��3��ҩƷ������������ҩƷ��Ų�ѯĳҩƷ������ǰ�Ѿ��۳�������\n");
    printf(" ��4��ҩ��������Ϣ������ҩ����ţ���ѯ��ҩ�����ơ������˵���Ϣ��\n");
    printf(" ��5��ҩƷ������Ϣ������ҩƷ��Ų�ѯҩƷ�����֡��������ڡ�����ҩ�������ʱ�䡢�������������ۡ����ۼۡ��ܼۡ���Ч��������˾����Ϣ��\n");
    printf("  ����ѡ��ܣ��������⡣\n");
    printf("   �������⣺���û��˽��ϵͳʹ�÷�����\n");
    printf(" ����ѡ��ܣ����ڡ���\n");
    printf(" ���ڡ�����ϵͳ�汾����Ȩ������\n");
    return getch();
}
