
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

char * gp_sys_name="药房药品管理系统";

char *ga_main_menu[]={"文    件(F) ",
	                  "药品管理(M) ",
					  "查     询(Q) ",
	                  "帮     助(H) ",
	                  "关     于（G）",
                      };

char * ga_sub_menu[]={"[S]数据保存",
	                  "[B]数据备份",
					  "[R]数据恢复",
					  "[X]退  出",
					  "[N]新药入库",
					  "[O]药品出库",
					  "[P]新增药房",
					  "[U]药品移除",
					  "[T]药房药品总量",
					  "[C]药品现存量",
					  "[K]药品出货量",
					  "[G]药房基本信息",
					  "[E]药品基本信息",
					  "[H]药房负责人",
					  "[T]帮助主题",
					  "[A]关于..."
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

int CreatList(PHARMACY_NODE * * phead); /*创建链表*/
int FreeList(PHARMACY_NODE * * phead);  /*释放链表*/
BOOL InsertNewPhaNode(void); /*插入新药房*/
BOOL InsertNewMedNode(void); /*插入新药品*/
BOOL SeekPharmacyNode(void); /*根据药品ID寻找对应药房*/
BOOL ModfiMedIm(void);//药品进出
BOOL DelMedNode(void); /*删除药品*/
BOOL QurryPhaMedQun(void); /*查询药房药品储量*/
BOOL QurryMedRemAmo(void); /*清点药品现存量*/
BOOL QurryMedAmoOut(void); /*查询药品出货量*/
BOOL QurryPhaBasIn(void); /*查询药品信息*/
BOOL QurryMedBasIn(void); /*查询药品信息*/
BOOL QurryPhaCha(void); /*查询药房负责人*/

BOOL SaveData(void);
BOOL BackupData(void);
BOOL RestoreData(void);
BOOL ExitSys(void);
BOOL HelpTopic(void);
BOOL AboutPharmacy(void);
BOOL SM(void);
BOOL HELP(void);

#endif
