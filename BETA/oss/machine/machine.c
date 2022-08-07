/***************************************
*
* machine.c
*
*******************/


#include "ATOMATRIX.h"
#include "avei.h"
#include "sframe.h"
#include "executable.h"

#include <windows.h>
#include "task.h"

#ifdef _DEBUG
# include <stdlib.h>
#endif

#define	STACKSIZE	100			// UPDATE: need to check stacksize in "push" routines

task				current_task ;
avei				E_PC;	// program counter
atom*				E_SP;	// stack pointer
sframe				E_PF;	// program stackframe
atom*				E_VA;	// local vars
short				E_VC;	// local vars count  UPDATE: int

string			pgmsl ;

/* --------------------------------------------------------------------- */

#include "op.h"

typedef void (*_routine)(void);

_routine op_code[] =
	{
	0,
	0,
	0,
	0,
	0,
	opBRA,
	opPOP_BRA,
	opBRF,
	opBRT,
	opFOREACH,
	opARY_ITER,
	opFUNC,
	opVERB,
	opTVERB,
	opLVERB,
	opADD,
	opSUB,
	opMUL,
	opDIV,
	opSEARCH,
	opGT,
	opLT,
	opGE,
	opLE,
	opEQ,
	opNE,
	opAND,
	opOR,
	opNOT,
	opNEGATE,
	opBAND,
	opBOR,
	opBNEGATE,
	opPOP,
	opPUSH_VAR,
//	opPUSH_STR,
//	opPUSH_NUM,
	opINDEX,
	0,
	opEVAL,
	opSPLIT,
	opSPLICE,
	0,
	0,
	opMAKE_ARY,
	opPUSH_ON_ARY,
	0,
	opASSIGN,
	opASSIGN_AE,
	opASSIGN_ME,
	opPUSHR,
	opPUSHL,
	opPOPR,
	opPOPL,
	opPULL,
	opREF,
	opPUSH_OV,
	opPUSH_OVVS,
	opPUSH_OVV,
	opPUSH_OVTS,
	opPUSH_OVT,
	0,
	0,
	opOBJ_FUNC,
//	opPUSH_OBJ,
	opIS,
	opIS_NOT,
	opVERB_A_S,
	opPUSH_OV_A_S
	};

void	yieldTask( void )
	{
	extern void		(*yield_task)(void) ;
	extern task		(*getTask)(void) ;
	if( current_task == current_task->taskp && ! current_task->thread )
		return ;
	current_task->pc = E_PC ;
	current_task->sf = E_PF ;
	current_task->sp = E_SP ;
	current_task->tv = E_VA ;
	current_task->vc = E_VC ;
	yield_task( ) ;
	current_task	= getTask( ) ;
	E_PC		= current_task->pc ;
	E_PF		= current_task->sf ;
	E_SP		= current_task->sp ;
	E_VA		= current_task->tv ;
	E_VC		= current_task->vc ;
	}

void machine(void)
	{
	extern atom		aMacroprocessity ;
	extern atom		aUndefined ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*oInteger)(int) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern string		(*str_dup)(string);
	extern void		(*output)(string) ;
	extern int		(*check_memory)(void) ;
	extern atom		(*duplicate)(atom) ;
	extern void*		(*allocz)(unsigned int) ;
	extern void		(*dealloc)(void*) ;
	extern void		(*destruct)(atom) ;
	extern task		(*getTask)(void) ;
	extern int		execute_a_a(atom,atom);
	extern void		Remove_Program(void);
	extern atom		(*addAtom)(atom,string,atom) ;
	extern atom		(*POBJ)(ObjID) ;
	extern string		(*aAtomatrixSeriestring)(void) ;
	atom			a;
	int			i;
	int			loop;
	atom*			stack ;
	task			t ;

	current_task = t = getTask( ) ;

	t->a		= atomize( aMacroprocessity ) ;		// UPDATE:: destruct where ??
	t->a->datum	= t ;
	a		= POBJ( 6 ) ;
	a->allusions++ ;
	addAtom( t->a , "avatar" , a ) ;
#ifdef _DEBUG
	addAtom( t->a , "debug" , oInteger( 1 ) ) ;
#endif
	addAtom( t->a , "atomatrix" , aAsciiString_atomize( str_dup( aAtomatrixSeriestring() ) ) ) ;
#if defined( MICROSOFT_win32 )
	{
	extern string		(*aAlchematrixVersion)(void) ;
	addAtom( t->a , "alchematrix" , aAsciiString_atomize( str_dup( aAlchematrixVersion() ) ) ) ;
	}
#endif

	stack		= allocz( sizeof(struct _atom*) * STACKSIZE );
	t->sp		= stack ;
	t->ss		= stack ;
	E_PC		= 0 ;
	E_PF		= 0 ;
	E_SP		= stack ;
	E_VA		= 0 ;
	E_VC		= 0 ;

	i	= STACKSIZE ;
	while( i-- )
		( stack[i] = aUndefined )->allusions++ ;

	if( ! execute_a_a( t->programme , t->args ) )
		{
		output( "root:_auto() failed!" );
		return ;			// UPDATE: dealloc stack
		}

	loop = 10 ;

	while( loop )
		{
		if( ! --loop )
			{
			yieldTask( ) ;
			loop = 10 ;
			}
		pgmsl	= E_PF->exea->linep[E_PC-E_PF->exea->exec] ;
		if( op_code[ i = E_PC++->opcode ] )
			(op_code[i])();
		else
		if( i == C_STACK )
			( *(E_SP++) = (*E_PC++).a )->allusions++ ;
		else
		if( i == C_RETURN0 )
			{
			Remove_Program();
			if( E_PF )
				( *(E_SP++) = aUndefined)->allusions++ ;
			else
				loop = 0;
			}
		else
		if( i == C_RETURN )
			{
			if( E_PF->last )
				{
				a		= *--E_SP ;
				Remove_Program() ;
				*(E_SP++)	= a ;
				}
			else	{
				Remove_Program();
				loop = 0 ;
				}
			}
		else
			_asm int 3 ;
		}

	while( E_SP != stack )
		if( ( a = *--E_SP ) )
			if( --a->allusions == 0 )
				destruct( stack[i] ) ;

/*		i	= STACKSIZE ;
	while( i-- )
		if( stack[i] )
			if( --stack[0]->allusions == 0 )
				destruct( stack[i] );
*/	dealloc( stack ) ;

	for( i = 0; i < E_VC ; i ++ )
		if( --E_VA[i]->allusions == 0 )
			destruct( E_VA[i] ) ;
	if( E_VA )
		dealloc( E_VA );
//	_asm int 3 ;
	}
