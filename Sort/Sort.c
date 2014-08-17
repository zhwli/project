/*������*/
/*U201114317 CS1106 ��־��*/
#include "resource.h"
#define ExtraSize 1000
#define TRUE 1
#define FALSE 0

long Size;
long *NumberOut=NULL;
HINSTANCE g_hInstance;       //ȫ��ʵ����� 
//Ĭ�ϳ��� 
TCHAR ClassName[] = _T("CLASS");
TCHAR MenuName[] = _T("MENU");
TCHAR MyIcon[] = _T("icon");
TCHAR MainDlgName[] = _T("DIALOG");                /* ������ */
TCHAR AboutDlgName[] = _T("ABOUT");            /* ���ڴ��� */


/* ���ھ�� */
HWND hMainDialog;
HWND hAboutSoftwareDialog;

//ð������O(n*n)
   void BubbleSort()
   {
       int i = Size, j;
       int temp;
		//���������Ƚϣ����ҳ������������ţ����i��1�����ظ�����
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

//��ѡ������O��n*n��
void SelectSort()
{
    register int i, j, min, t;
    for(i = 0; i < Size - 1; i ++)
    {
        min = i;
        //������Сֵ
        for(j = i + 1; j < Size; j ++)
            if(NumberOut[min] > NumberOut[j])
                min = j;
        //����,����Сֵ�ŵ���ǰ�����������ǰ��
        if(min != i)
        {
            t = NumberOut[min];
            NumberOut[min] = NumberOut[i];
            NumberOut[i] = t;
        }
    }
}

//ֱ�Ӳ��뷨����O(n*n)
void InsertSort()
 {
        int i,j;
        int temp;
        for (i =2; i<=Size;i++)
        {
                temp = NumberOut[i];
                j=i-1;
                //�������������һ�Ƚϣ�����tempʱ�����������
                while((j>=1) && (NumberOut[j] > temp))  //jѭ����-1ʱ�����ڶ�·��ֵ����������NumberOut[-1]
                {
                        NumberOut[j+1] =NumberOut[j];
                        j--;
                }
                NumberOut[j+1] = temp;      //���������ŵ���ȷ��λ��
 
        }
 }


//Shell�����㷨O��n*logn��
int ShellSort()
{
     int i, j, temp; 
     int gap = 0;
     while (gap<=Size)
     {
          gap = gap * 3 + 1;
     } 
	 //��3*gap+1���������ֵʱ��ֹ��Ȼ���С������
	 //�ظ�ȡ������gap = ( gap - 1 ) / 3����ֱ��gap=1
	 //���һ��Ϊֱ�Ӳ�������
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





 //���������㷨O(n*logn) S
// ���ݽ���
void swap(long *a,long *b)
{
    int tmp;
    tmp = *a; *a = *b; *b = tmp;
}
 
//���������㷨E
void QuickSort(long a[], long left, long right)
{
    int i = left + 1, j = right;
    int  key = a[left];
 
    if (left >= right) return;
 
    //��i++��j--������������������������ֵ������
    //����Ϊ��i++����С��key��j--�������key
	//���ݹ鲻��ϸ��
    while (1) {
       while (a[j] > key) j--;
       while (a[i] < key&&i<j) i++;
       if(i >= j) break;
       swap(&a[i],&a[j]);
       if(a[i]==key)j--;
       else  i++;
    }
    //�ؼ����ݷŵ����м䡯
    swap(&a[left],&a[j]);
    if(left  < i - 1)   QuickSort(a, left, i - 1);
    if(j + 1 < right)  QuickSort(a, j + 1 , right);
 
}


//������ʵ���㷨O(n*logn) S
//���Ѻ���
int HeapAdjust(long s,long m) 
{
	long j,rc;
	for(;(s*2)<=m;s=j)
	{
		j=s*2;    //���ӽڵ��λ��=2*(���ڵ��λ��)
		rc=NumberOut[s]; //���游�ڵ��ֵ
			if(j<m && NumberOut[j+1]>NumberOut[j]) j++; //jָ��ϴ��Ҷ�ڵ�
			if(rc<NumberOut[j]) NumberOut[s]=NumberOut[j];  
				//�ϴ��Ҷ�Ӹ����ڵ�Ƚϣ����Ҷ�ڵ�����Ҷ�ڵ�����ڵ�
			else break; //�����˳�ѭ��

		NumberOut[j]=rc;
	}
	return 0;
}

//������ʵ���㷨 E
int HeapSort()	
{
	long i,t;
	for(i=Size/2;i>0;--i) //��һ����Ҷ�ӽڵ�ΪSize/2
		HeapAdjust(i,Size); 
	for(i=Size;i>1;--i)
	{  //���źõ��������������
		t=NumberOut[i];
		NumberOut[i]=NumberOut[1];
		NumberOut[1]=t;//������i�����һ����ʹ���ĵ����
		HeapAdjust(1,i-1);
	}
	return 0;
}


 //�������������
int CreateRand(HWND hWnd)
{
	char ch[10]={'\0'};
	long flag,i;
	GetDlgItemText(hWnd,IDC_EDIT3,ch,10);//������������������ȡ��������
	for(flag=0;ch[flag]!='\0'&&flag<10;flag++)
		if(ch[flag]<48 | ch[flag]>57)
		{
			MessageBox(hWnd,TEXT("��������ȷ������"),TEXT("��ʾ"),MB_OK | MB_ICONERROR);
			return 0;
		}
	i=atoi(ch);//��char����ת��Ϊʮ������
	if(!i) return 0;
	Size=i;
	if( NumberOut != NULL)//NumberOut�ǿգ����ͷ���
	{
		free(NumberOut);

	}
	NumberOut=(long *)malloc(sizeof(long)*(Size+1));//��NumberOut����洢�ռ�
	if(!NumberOut)//�ж��ڴ�����Ƿ���ȷ
	{
		MessageBox(hWnd,TEXT("�ڴ�������"),TEXT("��ʾ"),MB_OK | MB_ICONERROR);
		exit(1);
	}
	srand((unsigned)time(NULL)); //���������������
	for(flag=1;flag<=Size;flag++)//������������洢��NumberOut��
	{
		NumberOut[flag] = rand()*rand()%100000;
	}
	return 0;
}


//�������
int OutPut(HWND hWnd)
{
	TCHAR *p=NULL,ch[7]={"\0"};//��������p�Դ��Ҫ���������
	long i=0,j=0,k=0;
	p=(TCHAR *)malloc(sizeof(TCHAR)*(Size+1)*6);//����ܹ���ʾ��λ��
	for(i=1;i<=Size;i++)
	{
		wsprintf(ch,_T("%ld"),NumberOut[i]);//��NumberOut[i]����ch��
		for(k=0;ch[k]!='\0';k++,j++)//��ch��������p��
			p[j]=ch[k];
		p[j++]=' ';
	}
	p[j]='\0';
	SetDlgItemText(hWnd,IDC_EDIT2,p);//�������p
	free(p);//�ͷ�����p
	return 0;
}


//"����..."���ڻص����� 
BOOL CALLBACK ProcWinAbout(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
		//�رմ���
	case WM_CLOSE:
		{
			EndDialog(hWnd,0);
		}
		break;
	}
	return 0;
}

//�˵����ú���
LRESULT CALLBACK ProcWinMain(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	TCHAR ch[10]={0};
	clock_t start,end;//��ʼ������ʱ�䶨��
	switch(Msg)
	{

	case WM_COMMAND:
		{
			if  (lParam == 0)
			{
				switch(LOWORD(wParam))
				{

						//���ö�����
				case IDM_HEAP:
					{
						CreateRand(hWnd);//���ô�����������������������
						if(!NumberOut) //�ж��Ƿ����������������
						{
							MessageBox(hWnd,TEXT("���������������"),TEXT("��ʾ"),MB_OK);
							return 0;
						}

						start=clock();//��ʼ��ʱ
						HeapSort();//ִ�ж������㷨
						end=clock();//��ʱ����
						wsprintf(ch,_T("%ld"),end-start);//�ó�����ʱ�䣬���������ch��
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //���ch����ʾʱ��
						OutPut(hWnd);//���������ɵ������
					}
					break;

					//����Shell����
					case IDM_SHELL:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("���������������"),TEXT("��ʾ"),MB_OK);
							return 0;
						}

						start=clock();
						ShellSort();
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //��ʾʱ��
						OutPut(hWnd);
					}
					break;

					//���ÿ�������
					case IDM_QUICK:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("���������������"),TEXT("��ʾ"),MB_OK);
							return 0;
						}

						start=clock();
						QuickSort(NumberOut,0,Size);
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //��ʾʱ��
						OutPut(hWnd);
					}
					break;

					//����ð������
					case IDM_BUBBLE:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("���������������"),TEXT("��ʾ"),MB_OK);
							return 0;
						}

						start=clock();
						BubbleSort();
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //��ʾʱ��
						OutPut(hWnd);
					}
					break;

					//����ֱ�Ӳ�������
					case IDM_INSERT:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("���������������"),TEXT("��ʾ"),MB_OK);
							return 0;
						}

						start=clock();
						InsertSort();
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //��ʾʱ��
						OutPut(hWnd);
					}
					break;

					//����ѡ������
					case IDM_SELECT:
					{
						CreateRand(hWnd);
						if(!NumberOut) 
						{
							MessageBox(hWnd,TEXT("���������������"),TEXT("��ʾ"),MB_OK);
							return 0;
						}

						start=clock();
						SelectSort();
						end=clock();
						wsprintf(ch,_T("%ld"),end-start);
						SetDlgItemText(hWnd,IDC_EDIT1,ch); //��ʾʱ��
						OutPut(hWnd);
					}
					break;

					//���ò˵����˳���
				case IDM_EXIT:
					{
						DestroyWindow(hWnd);//����ִ�д���
					}
					break;

					//�򿪲˵���������..."����
				case IDM_ABOUT:
					{
						hAboutSoftwareDialog = (HWND)DialogBox(g_hInstance,AboutDlgName,hWnd,(DLGPROC)ProcWinAbout);
					}
					break;
				}

				//�ж�NumberOutΪ�գ���Ϊ�վͽ����ÿ�
				if(NumberOut)
				{
					free(NumberOut);
					NumberOut=NULL;
				}
			}
		}
		break;

		//�˳�
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

//������
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	HICON hIcon;  /* ͼ����Դ��� */
	HCURSOR hCursor; /* �����Դ��� */
	HACCEL hAccelerator; /* ��ݼ���Դ��� */
	MSG stMsg;  /* ϵͳ��Ϣ */
	WNDCLASSEX stdWndClass; /* ������ */
	g_hInstance = hInstance; //ʵ�����

	setlocale(LC_ALL, "chs");/* �����ı�������� */

	hAccelerator = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDA_MAIN));
	hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(ID_ICON));
	hCursor = LoadCursor(NULL,IDC_ARROW);

	//ע�ᴰ����
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

	//��Ϣѭ��
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
