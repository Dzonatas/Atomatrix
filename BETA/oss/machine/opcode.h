/***************************************
*
* opcode.h
*
*******************/


#include "ATOMATRIX.h"
#include "avei.h"
#include "sframe.h"
#include "executable.h"

#include "cmp_s.h"

//#define lvar(a)		(E_VA)[E_PF->lvar + (a)]

extern avei		E_PC;
extern atom*		E_SP;
extern sframe		E_PF;
extern atom*		E_VA;
extern short		E_VC;

#define v_objref(V, O)	(V)->v.a=(O); (O)->allusions++;
