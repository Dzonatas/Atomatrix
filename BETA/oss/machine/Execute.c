// Execute.c			Copyright (c) 2004 BALLARD

#include <stdlib.h>

#include "ATOMATRIX.h"
#include "object.h"
#include "avei.h"
#include "sframe.h"
#include "executable.h"
#include "varray.h"
#include "program.h"

/***************************************
*
* UPDATE:
* Function Execute() should control a flow from previous list.
*
*******************/


int execute_a_s_a( atom A , string S , atom B )
	{
	extern atom		aProgram ;
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		(*oInteger)(int) ;
	extern atom		(*oModule)(unsigned) ;
	extern void		(*destruct)(atom) ;
	extern void*		(*allocm)(unsigned int) ;
	extern atom		execCreate(void);
	extern int		parseBuffer(string,atom);
	extern void		Add_Vars(short);
	extern void		Assign_Var(string,atom);
	extern atom		(*aModule_grammance_a_s)(atom,string) ;
//	extern int		illogicated(atom) ;
	program                 pptr ;
	executable		e ;
	sframe			lf ;
	extern avei		E_PC ;
	extern atom*		E_SP ;
	extern sframe		E_PF ;
	extern short		E_VC ;
	atom			a ;
	atom			p ;

//	a	= A ;
	p = aModule_grammance_a_s( A , S ) ;
	if( p->precursor != aProgram )
		{
		if( --p->precursor == 0 )
			destruct( p ) ;
		return 0 ;
		}
	if( ( e = ( pptr = p->datum )->exep ) )
		( a = e->a )->allusions++ ;
	else	{
		( e = pptr->exep = (executable)( a = execCreate() )->datum )->prog = pptr ;
		if( ( ! pptr->psrc ) || ( ! parseBuffer( pptr->psrc , a ) ) )
			{
			destruct( a ) ;
			return 0;		// UPDATE: indicate compile error, not PROGNF
			}
		a->allusions++ ;
		} 
	if( ( lf = E_PF ) )
		lf->pc = E_PC ;
	E_PC			= e->exec;
	E_PF			= allocm( sizeof(struct _sframe) );		// UPDATE: should get from _sframe pool
	E_PF->last		= lf;
	E_PF->exea		= e;
	E_PF->lvar		= E_VC ;
//	E_PF->this		= o ;
	A->allusions += 2 ;			// UPDATE: does ++allusions above do too much ?
	E_PF->scope		= A ;
	E_PF->from		= A ;		// UPDATE: p->orbit ??
//	E_PF->pobj		= o ;
	E_PF->pc		= (avei) 0;
	E_PF->datum		= 0;
	E_PF->stackbot		= E_SP;
	Add_Vars( e->varcnt );
	if( B && B->precursor == aVarray )
		{
		B->allusions++ ;
		Assign_Var( "argv", B );
		Assign_Var( "argc", oInteger( ((varray)B->datum)->nelem ) ) ;
		}
	else	{
		aUndefined->allusions++ ;
		Assign_Var( "argv", aUndefined );
		Assign_Var( "argc", oInteger( 0 ) );
		}
	A->allusions++ ;
	Assign_Var( "this",	A ) ;
	if( lf )
		{
//		Assign_Var( "caller",	oModule( lf->this ) ) ;
		lf->scope->allusions++ ;
		Assign_Var( "caller",	lf->scope ) ;
		// UPDATE: if "caller" is not declared than we should not "allusions++"  ; and "this" above
		}
	return 1 ;
	}

/*************/

int execute_a_a( atom M , atom B )
	{
	extern atom		aError_COMP ;
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;
	extern void*		(*allocm)(unsigned int) ;
	extern atom		execCreate(void);
	extern int		parseBuffer(string,atom);
	extern void		Add_Vars(short);
	extern void		Assign_Var(string,atom);
	program                 pptr ;
	executable		e ;
	sframe			lf ;
	extern avei		E_PC ;
	extern atom*		E_SP ;
	extern sframe		E_PF ;
	extern short		E_VC ;
	atom			a ;
//	ObjID			o ;

//	o	= ((object)M->orbit->datum)->this ;
	if( ( e = ( pptr = M->datum )->exep ) )
		( a = e->a )->allusions++ ;
	else	{
		( e = pptr->exep = (executable)( a = execCreate() )->datum )->prog = pptr ;
		if( ( ! pptr->psrc ) || ( ! parseBuffer( pptr->psrc , a ) ) )
			{
			destruct( a ) ;
			return 0;		// UPDATE: indicate compile error, not PROGNF
			}
		// a->allusions++ ;
		} 
	if( ( lf = E_PF ) )
		lf->pc = E_PC ;
	E_PC			= e->exec;
	E_PF			= allocm( sizeof(struct _sframe) );
	E_PF->last		= lf;
	E_PF->exea		= e;
	E_PF->lvar		= E_VC ;
//	E_PF->this		= o ;
	M->orbit->allusions += 2 ;
	E_PF->scope		= M->orbit ;
	E_PF->from		= M->orbit ;
//	E_PF->pobj		= o ;

	E_PF->pc		= (avei) 0;
	E_PF->datum		= 0;
	E_PF->stackbot		= E_SP;
	Add_Vars( e->varcnt );
	if( B && B->precursor == aVarray )
		{
		B->allusions++ ;
		Assign_Var( "argv", B );
		Assign_Var( "argc", oInteger( ((varray)B->datum)->nelem ) ) ;
		}
	else	{
		aUndefined->allusions++ ;
		Assign_Var( "argv", aUndefined );
		Assign_Var( "argc", oInteger( 0 ) );
		}
	M->orbit->allusions++ ;
	Assign_Var( "this",	M->orbit ) ;
	if( lf )
		{
		lf->scope->allusions++ ;
		Assign_Var( "caller",	lf->scope ) ;
		}
	return 1 ;
	}




// -----------------



atom execute_a_s_a_( atom A , string S , atom B )
	{
	extern atom		aError_COMP ;
	extern atom		aProgram ;
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		(*oInteger)(int) ;
	extern atom		(*oModule)(unsigned) ;
	extern void		(*destruct)(atom) ;
	extern void*		(*allocm)(unsigned int) ;
	extern atom		execCreate(void);
	extern int		parseBuffer(string,atom);
	extern void		Add_Vars(short);
	extern void		Assign_Var(string,atom);
	extern atom		(*aModule_grammance_a_s)(atom,string) ;
//	extern int		illogicated(atom) ;
	program                 pptr ;
	executable		e ;
	sframe			lf ;
	extern avei		E_PC ;
	extern atom*		E_SP ;
	extern sframe		E_PF ;
	extern short		E_VC ;
	atom			a ;
	atom			p ;

	p = aModule_grammance_a_s( A , S ) ;
	if( p->precursor != aProgram )
		return p ;
	if( ( e = ( pptr = p->datum )->exep ) )
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
	if( ( lf = E_PF ) )
		lf->pc = E_PC ;
	E_PC			= e->exec;
	E_PF			= allocm( sizeof(struct _sframe) );		// UPDATE: should get from _sframe pool
	E_PF->last		= lf;
	E_PF->exea		= e;
	E_PF->lvar		= E_VC ;
//	E_PF->this		= o ;
	A->allusions += 2 ;			// UPDATE: does ++allusions above do too much ?
	E_PF->scope		= A ;
	E_PF->from		= A ;		// UPDATE: p->orbit ??
//	E_PF->pobj		= o ;
	E_PF->pc		= (avei) 0;
	E_PF->datum		= 0;
	E_PF->stackbot		= E_SP;
	Add_Vars( e->varcnt );
	if( B && B->precursor == aVarray )
		{
		B->allusions++ ;
		Assign_Var( "argv", B );
		Assign_Var( "argc", oInteger( ((varray)B->datum)->nelem ) ) ;
		}
	else	{
		aUndefined->allusions++ ;
		Assign_Var( "argv", aUndefined );
		Assign_Var( "argc", oInteger( 0 ) );
		}
	A->allusions++ ;
	Assign_Var( "this",	A ) ;
	if( lf )
		{
//		Assign_Var( "caller",	oModule( lf->this ) ) ;
		lf->scope->allusions++ ;
		Assign_Var( "caller",	lf->scope ) ;
		// UPDATE: if "caller" is not declared than we should not "allusions++"  ; and "this" above
		}
	return atominul ;
	}
