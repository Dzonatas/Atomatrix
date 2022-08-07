/***************************************
*
* task.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"
#include "avei.h"
#include "sframe.h"
#include "executable.h"
#include "program.h"

#include <string.h>


void Remove_Program(void)
	{
	extern atom		aUndefined ;
	extern void		(*destruct)(atom) ;
	extern void		(*dealloc)(void*) ;
	extern void		Delete_Vars(short);
	extern void		Assign_Var(string,atom);
	extern avei		E_PC;
	extern atom*		E_SP;
	extern sframe		E_PF;
	extern atom*		E_VA;
	executable		exec;
	sframe			cfp;
	atom*			stkp;
	int			i;
	atom			a ;

	if( ! ( E_PF = ( cfp = E_PF )->last ) )
		E_PC = 0 ;
	else	{
		E_PC = E_PF->pc;
		if(cfp->exea->sigs & E_SIG_EVAL)
			{
			for(i=0;i<cfp->exea->varcnt;i++)
				if(cfp->exea->varnames[i])
					{
					if( a = E_VA[cfp->lvar+i]->datum )
						a->allusions++ ;
					Assign_Var( cfp->exea->varnames[i]->datum , a ) ;
					}
			}
		}
	for( stkp = cfp->stackbot ; stkp < E_SP ; stkp++ )  // UPDATE: perhaps stkp <= E_SP
		if( *stkp != aUndefined )		// UPDATE: redundent clear stack, should be not needed
			{
			if( --(*stkp)->allusions == 0 )
				destruct( *stkp );
			( *stkp = aUndefined )->allusions++ ;
			}
	if( --cfp->scope->allusions == 0 )
		destruct( cfp->scope ) ;
	if( --cfp->from->allusions == 0 )
		destruct( cfp->from ) ;
	E_SP	= cfp->stackbot;
	exec	= cfp->exea;
	dealloc( cfp ) ;
	Delete_Vars( exec->varcnt ) ;
//	if( exec->prog && strcmp( "_ascii" , exec->prog->name ) == 0 ) _asm int 3 ;
//	if( --exec->a->allusions == 0 && !exec->prog )
//		destruct( exec->a ) ;
	if( --exec->a->allusions == 0 )
		destruct( exec->a ) ;
	}


atom Find_Program_By_Location( atom A , string S )
	{
	extern atom		aProgram ;
	extern void		(*destruct)(atom) ;
	extern atom		(*aModule_grammance_a_s)(atom,string) ;
	atom			p;
	atom			a;

	a	= A ;
	do	{
		p = aModule_grammance_a_s( a , S ) ;
		if( p->precursor == aProgram )
			return p ;
		if( --p->allusions == 0 )
			destruct( p ) ;
		} while( a = ((object)a->datum)->orbit ) ;
	return 0 ;
	}

atom  aReturnDatum ;

atom sframe_return( int n )
	{
	extern atom		atomine ;
	extern atom		aSignal_JUMP ;
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern void		(*destruct)(atom) ;
	extern sframe		E_PF ;
	atom			a ;
//	atom			b ;
//	atom			c ;

	if( ! aReturnDatum )				// UPDATE:  move to structor.c
		aReturnDatum = atomize( atomine ) ;

	if( ! n )
		{
		if( ( a = E_PF->datum ) )
			{
//			if( c = ( b = a->interatom )->datum )
//				if( --c->allusions == 0 )
//					destruct( c ) ;		// UPDATE: not needed
			destruct( a ) ;
			return E_PF->datum = 0 ;
			}
		return 0  ;
		}
	if( ( a = E_PF->last->datum ) )
		{
//		if( c = ( b = a->interatom )->datum )
//			if( --c->allusions == 0 )
//				destruct( c ) ;		// UPDATE: not needed
		a->datum = (void*) n ;
		return 0 ;
		}
	( a = E_PF->last->datum = atomize( aSignal_JUMP ) )->datum = (void*) n ;
	interatomize( atomize( aReturnDatum ) , a ) ;
	return 0 ;
	}


int sframe_datum( atom* A )
	{
	extern sframe		E_PF ;
	atom			a ;
	atom			b ;

	if( ( a = E_PF->datum ) )
		{
		if( b = a->interatom )
			*A = b->datum ;
		return (int)a->datum ;
		}
	return 0 ;
	}


atom sframe_scope( void )
	{
	extern sframe		E_PF ;

	return E_PF->scope ;
	}


void executable_destructor( atom A )
	{
	extern atom		_tkn_begin ;
	extern void		(*dealloc)(void*) ;
	extern void		(*interatomize)(atom,atom) ;
	extern void		(*destruct)(atom) ;
	extern atom		aAsciiString ;
	executable		e ;
	atom			a ;
	
	if( ! ( e = A->datum ) )
		return ;
	if( e->varnames )
		{
		atom*	s ;
		int	x ;
		x	= e->varcnt ;
		s	= e->varnames ;
		while( x-- )
			{
			if( *s && --(*s)->allusions == 0 )
				destruct( *s ) ;
			s++ ;
			}
		dealloc( e->varnames ) ;
		}
	if( e->exec )
		dealloc( e->exec ) ;
	if( e->linep )
		dealloc( e->linep ) ;
	if( e->prog )
		e->prog->exep = (executable) 0 ;
	dealloc( e ) ;
	if( ( a = A->interatom ) )
		do	if( a->precursor == _tkn_begin )
				{
				destruct( a ) ;
				break ;
				}
		while( ( a = a->interatoms ) ) ;
	if( ( a = A->interatom ) )
		do	if( a->precursor == aAsciiString && a->allusions != 1 )		// UPDATE: should move any leftovers on A
				interatomize( a , 0 ) ;
		while( ( a = a->interatoms ) ) ;
	A->datum	= 0 ;
	}



int illogicated( atom A )
	{
	extern atom		aIllogical ;
	atom			a ;

	if( A == aIllogical )
		return 1 ;
	if( ( a = A->precursor ) )
		do	if( a == aIllogical )
				return 1 ;
			while( ( a = a->precursor ) ) ;
	return 0 ;
	}
