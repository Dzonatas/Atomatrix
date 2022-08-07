/***************************************
*
* find_func.c
*
*******************/

#include "ATOMATRIX.h"

#include "cmp_s.h"
/*
extern atom BI_Notify		(int,var*,...);
//extern atom BI_SetPlayer	(int,var*,...);
extern atom BI_Connect		(int,var*,...);
extern atom BI_Disconnect	(int,var*,...);
extern atom BI_Create		(int,var*,...);
extern atom BI_Add_Obj_Var	(int,var*,...);
extern atom BI_Children		(int,var*,...);
extern atom BI_MoveObj		(int,var*,...);
extern atom BI_IsValid		(int,var*,...);
extern atom BI_IsChild		(int,var*,...);
extern atom BI_Program		(int,var*,...);
extern atom BI_Programs		(int,var*,...);
extern atom BI_ChParent		(int,var*,...);
extern atom BI_AddProg		(int,var*,...);
extern atom BI_DelProg		(int,var*,...);
extern atom BI_Logins		(int,var*,...);
extern atom BI_Kill		(int,var*,...);
extern atom BI_Read		(int,var*,...);
extern atom BI_List		(int,var*,...);
extern atom BI_ChVarOwn		(int,var*,...);
extern atom BI_ChVarPerm	(int,var*,...);
extern atom BI_Recycle		(int,var*,...);
extern atom BI_Vars		(int,var*,...);
extern atom BI_Delete_Var	(int,var*,...);
extern atom BI_Ancestors	(int,var*,...);
extern atom BI_Ticks		(int,var*,...);
extern atom BI_Sleep		(int,var*,...);
extern atom BI_Wait		(int,var*,...);
extern atom BI_ChObjOwner	(int,var*,...);
extern atom BI_ChObjPerm	(int,var*,...);
extern atom BI_HasVar		(int,var*,...);
extern atom BI_Debug		(int,var*,...);
extern atom BI_PObj		(int,var*,...);
extern atom BI_IsContainedIn	(int,var*,...);
extern atom BI_MaxObj		(int,var*,...);
extern atom BI_Inherit		(int,var*,...);
extern atom BI_StrSub		(int,var*,...);
extern atom BI_ToStr		(int,var*,...);
extern atom BI_ToFmt		(int,var*,...);
extern atom BI_ToNum		(int,var*,...);
extern atom BI_Split		(int,var*,...);
extern atom BI_Length		(int,var*,...);
extern atom BI_ToUpper		(int,var*,...);
extern atom BI_ToLower		(int,var*,...);
extern atom BI_String_Match	(int,var*,...);
extern atom BI_Join		(int,var*,...);
extern atom BI_Shutdown		(int,var*,...);
extern atom BI_Memory		(int,var*,...);
extern atom BI_MemInfo		(int,var*,...);
extern atom BI_Time		(int,var*,...);
extern atom BI_LocalTime	(int,var*,...);
extern atom BI_SubStr		(int,var*,...);
extern atom BI_Random		(int,var*,...);
extern atom BI_Index		(int,var*,...);
extern atom BI_RIndex		(int,var*,...);
extern atom BI_Eval		(int,var*,...);
extern atom BI_Percent		(int,var*,...);
extern atom BI_Edit		(int,var*,...);
extern atom BI_Load		(int,var*,...);

struct
{ 
char *name;
void *func;
}
bi_funcs[] =
{
  {	0,		0			}
  	};

void* Find_Func(string name)
{
int x;

if(name!=0)
	for(x=0;bi_funcs[x].name!=0;x++) if(cmp_s(bi_funcs[x].name,name)==0)
		return bi_funcs[x].func;
return (void*)0;
}

string FuncName(void* ptr)
{
int x;

for(x=0;bi_funcs[x].name!=0;x++) if(bi_funcs[x].func==ptr)
	return bi_funcs[x].name;
return "UNKNOWN";
}
*/

/***************************************
*
* UPDATE:
* Function Find_Func() should be redesigned in another fashion to
* accomplish built in function searches.
\*
*******************/
