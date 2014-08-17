/*主程序*/
/*U201114317 CS1106 李志文*/
#include "resource.h"
#define ExtraSize 1000
#define TRUE 1
#define FALSE 0

long Size;
long *NumberOut=NULL;
HINSTANCE g_hInstance;       //全局实例句柄 
//默认常量 
TCHAR ClassName[] = _T("CLASS");
TCHAR MenuName[] = _T("MENU");
TCHAR MyIcon[] = _T("icon");
TCHAR MainDlgName[] = _T("DIALOG");                /* 主窗口 */
TCHAR AboutDlgName[] = _T("ABOUT");            /* 关于窗口 */


/* 窗口句柄 */
HWND hMainDialog;
HWND hAboutSoftwareDialog;

//冒泡排序O(n*n)
   void BubbleSort()
   {
       int i = Size, j;
       int temp;
		//依次两两比较，将找出大的数往后面放，最后i减1，对重复操作
       while(i > 0)
       {
          for(j = 0; j < i - 1; j++)
          {
              if(NumberOut[j] > NumberOut[j + 1])
              {   temp = NumberOut[j];
                  NumberOut[j] = NumberOut[j + 1];
                  NumberOut[j + 1] = temp;
              }
          }
          i--;
      }
 
  }

//简单选择法排序O（n*n）
void SelectSort()
{
    register int i, j, min, t;
    for(i = 0; i < Size - 1; i ++)
    {
        min = i;
        //查找最小值
        for(j = i + 1; j < Size; j ++)
            if(NumberOut[min] > NumberOut[j])
                min = j;
        //交换,将最小值放到当前排序数组的最前面
        if(min != i)
        {
            t = NumberOut[min];
            NumberOut[min] = NumberOut[i];
            NumberOut[i] = t;
        }
    }
}

//直接插入法排序O(n*n)
void InsertSort()
 {
        int i,j;
        int temp;
        for (i =2; i<=Size;i++)
        {
                temp = NumberOut[i];
                j=i-1;
                //与已排序的数逐一比较，大于temp时，该数向后移
                while((j>=1) && (NumberOut[j] > temp))  //j循环到-1时，由于短路求值，不会运算NumberOut[-1]
                {
                        NumberOut[j+1] =NumberOut[j];
                        j--;
                }
                NumberOut[j+1] = temp;      //被排序数放到正确的位置
 
        }
 }


//Shell排序算法O（n*logn）
int ShellSort()
{
     int i, j, temp; 
     int gap = 0;
     while (gap<=Size)
     {
          gap = gap * 3 + 1;
     } 
	 //（3*gap+1）超过最大值时终止，然后对小组排序
	 //重复取步长（gap = ( gap - 1 ) / 3），直至gap=1
	 //最后一轮为直接插入排序
     while (gap > 0) 
     {
         for ( i = gap; i < Size; i++ )
         {
             j = i - gap;
             temp = NumberOut[i];             
             while (( j >= 0 ) && ( NumberOut[j] > temp ))
             {
                 NumberOut[j + gap] = NumberOut[j];
                 j = j - gap;
             }
             NumberOut[j + gap] = temp;
         }
         gap = ( gap - 1 ) / 3;
     }    
 }





 //快速排序算法O(n*logn) S
// 数据交换
void swap(long *a,long *b)
{
    int tmp;
    tmp = *a; *a = *b; *b = tmp;
}
 
//快速排序算法E
void QuickSort(long a[], long left, long right)
{
    int i = left + 1, j = right;
    int  key = a[left];
 
    if (left >= right) return;
 
    //从i++和j--两个方向搜索不满足条件的值并交换
    //条件为：i++方向小于key，j--方向大于key
	//按递归不断细分
    while (1) {
       while (a[j] > key) j--;
       while (a[i] < key&&i<j) i++;
       if(i >= j) break;
       swap(&a[i],&a[j]);
       if(a[i]==key)j--;
       else  i++;
    }
    //关键数据放到‘中间’
    swap(&a[left],&a[j]);
    if(left  < i - 1)   QuickSort(a, left, i - 1);
    if(j + 1 < right)  QuickSort(a, j + 1 , right);
 
}


//堆排序实现算法O(n*logn) S
//建堆函数
int HeapAdjust(long s,long m) 
{
	long j,rc;
	for(;(s*2)<=m;s=j)
	{
		j=s*2;    //左子节点的位置=2*(父节点的位置)
		rc=NumberOut[s]; //保存父节点的值
			if(j<m && NumberOut[j+1]>NumberOut[j]) j++; //j指向较大的叶节点
			if(rc<NumberOut[j]) NumberOut[s]=NumberOut[j];  
				//较大的叶子跟父节点比较，如果叶节点大则把叶节点给父节点
			else break; //否则退出循环

		NumberOut[j]=rc;
	}
	return 0;
}

//堆排序实现算法 E
int HeapSort()	
{
	long i,t;
	for(i=Size/2;i>0;--i) //第一个非叶子节点为Size/2
		HeapAdjust(i,Size); 
	for(i=Size;i>1;--i)
	{  //把排好的数据逐个往最后放
		t=NumberOut[i];
		NumberOut[i]=NumberOut[1];
		NumberOut[1]=t;//交换第i个与第一个，使最大的到最后
		HeapAdjust(1,i-1);
	}
	return 0;
}


 //产生随机数函数
int CreateRand(HWND hWnd)
{
	char ch[10]={'\0'};
	long flag,i;
	GetDlgItemText(hWnd,IDC_EDIT3,ch,10);//从随机数个数输入框提取输入数据
	for(flag=0;ch[flag]!='\0'&&flag<10;flag++)
		if(ch[flag]<48 | ch[flag]>57)
		{
			MessageBox(hWnd,TEXT("请输入正确的数字"),TEXT("提示"),MB_OK | MB_ICONERROR);
			return 0;
		}
	i=atoi(ch);//将char类型转化为十进制数
	if(!i) return 0;
	Size=i;
	if( NumberOut != NULL)//NumberOut非空，就释放它
	{
		free(NumberOut);

	}
	NumberOut=(long *)malloc(sizeof(long)*(Size+1));//给NumberOut分配存储空间
	if(!NumberOut)//判断内存分配是否正确
	{
		MessageBox(hWnd,TEXT("内存分配错误"),TEXT("提示"),MB_OK | MB_ICONERROR);
		exit(1);
	}
	srand((unsigned)time(NULL)); //生成随机数的种子
	for(flag=1;flag<=Size;flag++)//生成随机数并存储到NumberOut中
	{
		NumberOut[flag] = rand()*rand()%100000;
	}
	return 0;
}


//输出函数
int OutPut(HWND hWnd)
{
	TCHAR *p=NULL,ch[7]={"\0"};//构造数组p以存放要输出的数据
	long i=0,j=0,k=0;
	p=(TCHAR *)malloc(sizeof(TCHAR)*(Size+1)*6);//最多能够显示六位数
	for(i=1;i<=Size;i++)
	{
		wsprintf(ch,_T("%ld"),NumberOut[i]);//将NumberOut[i]放入ch中
		for(k=0;ch[k]!='\0';k++,j++)//将ch放入数组p中
			p[j]=ch[k];
		p[j++]=' ';
	}
	p[j]='\0';
	SetDlgItemText(hWnd,IDC_EDIT2,p);//输出数组p
	free(p);//释放数组p
	return 0;
}


//"关于..."窗口回调函数 
BOOL CALLBACK ProcWinAbout(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
		//关闭窗口
	case WM_CLOSE:
		{
			EndDialog(hWnd,0);
		}
		break;
	}
	return 0;
}

//菜单调用函数
LRESULT CALLBACK ProcWinMain(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	TCHAR ch[10]={0};
	clock_t start,end;//开始、结束时间定义
	switch(Msg)
	{

	case WM_COMMAND:
		{
			if  (lParam == 0)
			{
				switch(LOWORD(wParam))
				{

						//调用堆排序
				case IDM_HEAP:
					{
						CreateRand(hWnd);//调用创建随机数函数，创建随机数
						if(!NumberOut) //判断是否输入了随机数个数
						{
							MessageBox(hWnd,TEXT("请输入随机数个数"),TEXT("提示"),MB_OK);
							return 0;
						}

						start=clock();//开始计时
						HeapSort();//执行堆排序算法
						end=clock();//计时结束
						wsprintf(ch,_T("%ld"),end-start);//得出排序时间，并将其存入ch中
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //输出ch，显示时间
						OutPut(hWnd);//输出排序完成的随机数
					}
					break;

					//调用Shell排序
					case IDM_SHELL:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("请输入随机数个数"),TEXT("提示"),MB_OK);
							return 0;
						}

						start=clock();
						ShellSort();
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //显示时间
						OutPut(hWnd);
					}
					break;

					//调用快速排序
					case IDM_QUICK:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("请输入随机数个数"),TEXT("提示"),MB_OK);
							return 0;
						}

						start=clock();
						QuickSort(NumberOut,0,Size);
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //显示时间
						OutPut(hWnd);
					}
					break;

					//调用冒泡排序
					case IDM_BUBBLE:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("请输入随机数个数"),TEXT("提示"),MB_OK);
							return 0;
						}

						start=clock();
						BubbleSort();
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //显示时间
						OutPut(hWnd);
					}
					break;

					//调用直接插入排序
					case IDM_INSERT:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("请输入随机数个数"),TEXT("提示"),MB_OK);
							return 0;
						}

						start=clock();
						InsertSort();
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //显示时间
						OutPut(hWnd);
					}
					break;

					//调用选择排序
					case IDM_SELECT:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("请输入随机数个数"),TEXT("提示"),MB_OK);
							return 0;
						}

						start=clock();
						SelectSort();
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //显示时间
						OutPut(hWnd);
					}
					break;

					//调用菜单栏退出项
				case IDM_EXIT:
					{
						DestroyWindow(hWnd);//销毁执行窗口
					}
					break;

					//打开菜单栏“关于..."窗口
				case IDM_ABOUT:
					{
						hAboutSoftwareDialog = (HWND)DialogBox(g_hInstance,AboutDlgName,hWnd,(DLGPROC)ProcWinAbout);
					}
					break;
				}

				//判断NumberOut为空，不为空就将其置空
				if(NumberOut)
				{
					free(NumberOut);
					NumberOut=NULL;
				}
			}
		}
		break;

		//退出
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;  
	
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam );
	}

	return 0;
}

//主窗口
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	HICON hIcon;  /* 图标资源句柄 */
	HCURSOR hCursor; /* 光标资源句柄 */
	HACCEL hAccelerator; /* 快捷键资源句柄 */
	MSG stMsg;  /* 系统消息 */
	WNDCLASSEX stdWndClass; /* 窗口类 */
	g_hInstance = hInstance; //实例句柄

	setlocale(LC_ALL, "chs");/* 中文文本输出设置 */

	hAccelerator = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDA_MAIN));
	hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(ID_ICON));
	hCursor = LoadCursor(NULL,IDC_ARROW);

	//注册窗口类
	RtlZeroMemory(&stdWndClass, sizeof(stdWndClass));
	stdWndClass.hCursor = LoadCursor(0,IDC_ARROW);
	stdWndClass.cbSize = sizeof(stdWndClass);
	stdWndClass.style = CS_HREDRAW|CS_VREDRAW;
	stdWndClass.lpfnWndProc = ProcWinMain;
	stdWndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	stdWndClass.lpszClassName = ClassName;
	stdWndClass.hInstance = hInstance;
	stdWndClass.lpszMenuName = MenuName;
	stdWndClass.cbClsExtra = 0;
	stdWndClass.cbWndExtra = DLGWINDOWEXTRA;
	stdWndClass.hIcon = hIcon;
    stdWndClass.hIconSm = hIcon;
	stdWndClass.hCursor = hCursor;

	RegisterClassEx(&stdWndClass);
	if ((hMainDialog = (HWND)CreateDialogParam(hInstance,MainDlgName,NULL,NULL,0)) == 0)
		return 0;

	ShowWindow(hMainDialog,SW_SHOWNORMAL);
	UpdateWindow(hMainDialog);

	//消息循环
	while(GetMessage(&stMsg,NULL,0,0))
	{
		if(!TranslateAccelerator(hMainDialog,hAccelerator,&stMsg))
		{
			TranslateMessage(&stMsg);
			DispatchMessage(&stMsg);
		}
	}
	return stMsg.wParam;

}
