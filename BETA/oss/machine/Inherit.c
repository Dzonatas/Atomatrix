/***************************************
*
* Inherit()
*
*******************/


#include <stdlib.h>

#include "ATOMATRIX.h"
#include "object.h"
#include "avei.h"
#include "sframe.h"
#include "executable.h"
#include "varray.h"
#include "program.h"

atom Inherit( atom A )
	{
	extern atom		aProgram ;
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		aError_PROGNF ;
	extern atom		aError_COMP ;
	extern atom		aSignal_EXEC_CONTINUE ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;
	extern void*		(*allocm)(unsigned int) ;
	extern object		(*MOBJ)(atom) ;
	extern atom		execCreate(void);
	extern int		parseBuffer(string,atom);
	extern void		Assign_Var(string,atom);
	extern void		Add_Vars(short);
	extern atom		(*aModule_grammance_a_s)(atom,string) ;
	extern avei		E_PC;
	extern atom*		E_SP;
	extern sframe		E_PF;
	extern short		E_VC;
	program                 pptr;
	executable		e;
	sframe			lf;
	atom			a;
	atom			prg ;
	atom			m ;

	if( ! E_PF->exea->prog )
		{
		aError_PROGNF->allusions++;
		return aError_PROGNF;
		}
	if( ! ( m = MOBJ( E_PF->from )->precursor ) )	// UPDATE:
		{
		aError_PROGNF->allusions++;
		return aError_PROGNF;
		}
	prg =	aModule_grammance_a_s( m , E_PF->exea->prog->name ) ;		// UPDATE: aModule_grammance_a( program )
	if( prg->precursor != aProgram )
		return prg ;
	if( ( e = ( pptr = prg->datum )->exep ) )
		( a = e->a )->allusions++ ;
	else	{
		( e = pptr->exep = (executable)( a = execCreate() )->datum )->prog = pptr ;
		if( ( ! pptr->psrc ) || ( ! parseBuffer( pptr->psrc , a ) ) )
			{
			destruct( a ) ;
			aError_COMP->allusions++ ;
			return aError_COMP ;
			}
		a->allusions++ ;
		} 
	lf		= E_PF ;
	lf->pc		= E_PC ;
	E_PC			= e->exec;
	E_PF			= allocm( sizeof(struct _sframe) );
	E_PF->last		= lf;
	E_PF->exea		= e;
	E_PF->lvar		= E_VC ;
	( E_PF->scope		= lf->scope )->allusions++ ;
	( E_PF->from		= m )->allusions++ ;
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
	lf->scope->allusions++ ;
	Assign_Var( "this",	lf->scope ) ;
	Assign_Var( "caller",	lf->scope ) ;  //UPDATE: lf->from ? nah
	( lf->datum = aSignal_EXEC_CONTINUE )->allusions++ ;
	return atominul ;
	}
