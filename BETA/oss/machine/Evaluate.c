/***************************************
*
* Evaluate.c
*
*******************/


#include "ATOMATRIX.h"
#include "avei.h"
#include "sframe.h"
#include "executable.h"
#include "varray.h"


atom Evaluate( string S , atom A )
	{
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		aError_COMP ;
	extern atom		aSignal_EXEC_CONTINUE ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;
	extern void*		(*allocm)(unsigned int) ;
	extern atom		execCreate(void);
	extern int		parseBuffer(string,atom);
	extern void		Add_Vars(short);
	extern void		Assign_Var(string,atom);
	extern avei		E_PC;
	extern atom*		E_SP;
	extern sframe		E_PF;
	extern short		E_VC;
	executable		e;
	sframe			lf;
	atom			a ;

	e	= ( a = execCreate() )->datum ;
	e->sigs	|= E_SIG_EVAL;
	if( ! parseBuffer( S , a ) )
		{
		destruct( a ) ;
		aError_COMP->allusions++ ;
		return aError_COMP ;
		}
	a->allusions++ ;		// UPDATE: [mem leak!] the exec should orbit the expression's source program exec, and destruct with it
	lf		= E_PF ;
	lf->pc		= E_PC ;
	E_PC			= e->exec;
	E_PF			= allocm( sizeof(struct _sframe) );
	E_PF->last		= lf;
	E_PF->exea		= e;
	E_PF->lvar		= E_VC ;
//	E_PF->this		= lf->this ;
	( E_PF->scope		= lf->scope )->allusions++ ;
	E_PF->from		= lf->scope ;
//	E_PF->pobj		= lf->this ;
	E_PF->pc		= (avei) 0;
	E_PF->datum		= 0;
	E_PF->stackbot		= E_SP;
	Add_Vars( e->varcnt ) ;
	if( A && A->precursor == aVarray )
		{
		A->allusions++ ;
		Assign_Var( "argv", A ) ;
		Assign_Var( "argc", oInteger( ((varray)A->datum)->nelem ) ) ;
		}
	else	{
		aUndefined->allusions++ ;
		Assign_Var( "argv", aUndefined ) ;
		Assign_Var( "argc", oInteger( 0 ) ) ;
		}
	lf->scope->allusions += 2 ;
	Assign_Var( "this", 	lf->scope ) ;
	Assign_Var( "caller",	lf->scope ) ;
	( lf->datum = aSignal_EXEC_CONTINUE )->allusions++ ;
	return atominul ;
	}
