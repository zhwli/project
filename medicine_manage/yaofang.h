
#include "stdio.h"
#include "assert.h"
#include "time.h"
#include "stdlib.h"
#include "conio.h"
#include "string.h"
#include "math.h"
#include "windows.h"
#include "wincon.h"
#include "io.h"
#include "fcntl.h"
#include "sys\stat.h"
#include "ctype.h"
unsigned long num_w;

#ifndef TYPE_H_INCLUDE
#define TYPE_H_INCLUDE

#define SCR_ROW 25
#define SCR_COL 80

typedef struct pharmacy_node{
	char pharmacy_id[5];
	char pharmacy_name[10];
	char pharmacy_charger_name[10];
	struct med_node * snext;
	struct pharmacy_node * next;
}PHARMACY_NODE;

typedef struct med_node{
	char med_id[12];
	char med_name[10];
	char inven_date[20];
	char type[10];
	char pharmacy_id[5];
	char date[20];
	int first_quantity;
	float price;
	float cost;
	float sel_price;
	char effect[80];
	char way_of_taking[80];
	char product_firm [20];
	struct im_information * cnext;
	struct med_node * next;
}MED_NODE;

typedef struct im_information{
	char med_id[12];
	char med_name[10];
	int rem_amount;
	int amount_out;
	struct im_information * next;
}IM_INFORMATION;

typedef struct layer_node{
	char LayerNo;
	SMALL_RECT rcArea;
	CHAR_INFO * pContent;
	char * pScrAtt;
	struct layer_node * next;
}LAYER_NODE;

typedef struct label_bundle{
	char * * ppLabel;
	COORD * pLoc;
	int num;
}LABEL_BUNDLE;

typedef struct hot_area{
	SMALL_RECT * pArea;
	char *pSort;
	char *pTag;
	int num;
}HOT_AREA;

LAYER_NODE * gp_top_layer=NULL;
PHARMACY_NODE *gp_head=NULL;

char * gp_sys_name="ҩ��ҩƷ����ϵͳ";

char *ga_main_menu[]={"��    ��(F) ",
	                  "ҩƷ����(M) ",
					  "��     ѯ(Q) ",
	                  "��     ��(H) ",
	                  "��     �ڣ�G��",
                      };

char * ga_sub_menu[]={"[S]���ݱ���",
	                  "[B]���ݱ���",
					  "[R]���ݻָ�",
					  "[X]��  ��",
					  "[N]��ҩ���",
					  "[O]ҩƷ����",
					  "[P]����ҩ��",
					  "[U]ҩƷ�Ƴ�",
					  "[T]ҩ��ҩƷ����",
					  "[C]ҩƷ�ִ���",
					  "[K]ҩƷ������",
					  "[G]ҩ��������Ϣ",
					  "[E]ҩƷ������Ϣ",
					  "[H]ҩ��������",
					  "[T]��������",
					  "[A]����..."
                      };

int ga_sub_menu_count[]={4,4,6,1,1};
int gi_sel_menu=1;
int gi_sel_sub_menu=0;
CHAR_INFO * gp_buff_menubar_info=NULL;
CHAR_INFO * gp_buff_stateBar_info=NULL;
char *gp_scr_att=NULL;
char *gp_sys_state='\0';

HANDLE gh_std_out;
HANDLE gh_std_in;

int LoadCode(char * filename,char * * ppbuffer);
int LoadCode(char*filename,char**ppbuffer);
void InitInterface(void);
void ClearScreen(void);
void ShowMenu(void);
void PopMenu(int num);
void PopPrompt(int num);
void PopUp(SMALL_RECT*,WORD,LABEL_BUNDLE*,HOT_AREA*);

void PopOff(void);
void DrawBox(SMALL_RECT*parea);
void LocSubMenu(int num,SMALL_RECT*parea);
void ShowState(void);
void TagMainMenu(int num);
void TagSubMenu(int num);
int DealConInput(HOT_AREA * phot_area,int*pihot_num);
void SetHotPoint(HOT_AREA * phot_area,int hot_num);
void RunSys(PHARMACY_NODE * * pphd);
BOOL ExeFunction(int main_menu_num,int sub_menu_num);
void CloseSys(PHARMACY_NODE * phd);
int DealInput(HOT_AREA *pHotArea, int *piHot);
void SetHotPoint(HOT_AREA *pHotArea, int iHot);

int CreatList(PHARMACY_NODE * * phead); /*��������*/
int FreeList(PHARMACY_NODE * * phead);  /*�ͷ�����*/
BOOL InsertNewPhaNode(void); /*������ҩ��*/
BOOL InsertNewMedNode(void); /*������ҩƷ*/
BOOL SeekPharmacyNode(void); /*����ҩƷIDѰ�Ҷ�Ӧҩ��*/
BOOL ModfiMedIm(void);//ҩƷ����
BOOL DelMedNode(void); /*ɾ��ҩƷ*/
BOOL QurryPhaMedQun(void); /*��ѯҩ��ҩƷ����*/
BOOL QurryMedRemAmo(void); /*���ҩƷ�ִ���*/
BOOL QurryMedAmoOut(void); /*��ѯҩƷ������*/
BOOL QurryPhaBasIn(void); /*��ѯҩƷ��Ϣ*/
BOOL QurryMedBasIn(void); /*��ѯҩƷ��Ϣ*/
BOOL QurryPhaCha(void); /*��ѯҩ��������*/

BOOL SaveData(void);
BOOL BackupData(void);
BOOL RestoreData(void);
BOOL ExitSys(void);
BOOL HelpTopic(void);
BOOL AboutPharmacy(void);
BOOL SM(void);
BOOL HELP(void);

#endif
