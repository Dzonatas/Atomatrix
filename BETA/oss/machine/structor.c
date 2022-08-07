/***************************************
*
* structor.c
*
*******************/

#include "ATOMATRIX.h"
#include "executable.h"
#include "variant.h"
#include "variable.h"
#include "object.h"
#include "objvar.h"
#include "sobjvar.h"
#include "program.h"
#include "method.h"
#include "avei.h"
#include "sframe.h"
#include "task.h"

atom			aExecutable ;		// UPDATE: aExecutant
atom			aOprandAdd ;
atom			aOprandDivide ;
atom			aOprandCondition ;
atom			aOprandCompareEqual ;
atom			aOprandIterate ;
atom			aOprandIterize ;
atom			aOprandEquals ;
atom			aOprandAddEquals ;
atom			aOprandMinusEquals ;
atom			aOprandAddEquate ;
atom			aOprandMinusEquate ;
atom			aOprandBinaryAnd ;
atom			aOprandBinaryOr ;
atom			aOprandBinaryNegation ;
atom			aOprandIndex ;
atom			aOprandPopLeft ;
atom			aOprandPopRight ;
atom			aOprandPull ;
atom			aOprandPushLeft ;
atom			aOprandPushRight ;
atom			aOprandSplice ;
atom			aOprandSplit ;
atom			aOprandEvaluate ;
atom			aOprandLessThan ;
atom			aOprandGreaterThan ;
atom			aOprandGreaterOrEqual ;
atom			aOprandLessOrEqual ;
atom			aIterum ;
atom			aEquality ;
atom			aSignal_REPEAT_FUNCTION;
atom			aSignal_EXEC_CONTINUE;
atom			aSignal_JUMP;
atom			aMacroprocessity ;

atom _lib		ATOMATRIX_ATOMICAL ;
atom			aAtomatrix ;
atom			atomine;
atom			aUndefined;
atom			aIllogical;
atom			aMethod ;
atom			aAtomineAsciiString ;
atom			aDirectionAtomatrix ;
atom			aAsciiCharacter ;
atom			aUnicodedEntity ;
atom			aUnicodedString ;
atom			aUnicode ;
atom			aUnicoded ;
atom			(*atomize)(atom);			// intratomize
void			(*destruct)(atom);
void			(*interatomize)(atom,atom);
void			(*intratomize)(atom,atom);
atom			(*atomethodate)(atom,string,unsigned,atom*);
atom			(*atomethodize)(atom,string,atom(*)(atom,unsigned,atom*));
void*			(*allocm)(unsigned int);
void*			(*allocz)(unsigned int);
void*			(*allocr)(void*,unsigned int);
void			(*dealloc)(void*);
int			(*check_memory)(void);
void			(*output)(string);
string			(*input)(void);
string			(*add_s)(string,string);
string			(*str_dup)(string);
string			(*str_cpy)(string,string);
string			(*str_cat)(string,string);
unsigned int		(*str_len)(string);
atom			(*oFunction)(atom,string,void*) ;
atom			(*oAtomical)(atom,string,atom) ;
void*			(*oAtomatrixFunction)(string) ;
void*			(*oLibraryFunction)(atom,string) ;
atom			(*oAtomatrixAtomical)(string) ;
atom			(*oLibraryAtomical)(atom,string) ;
atom			(*oDestructor)(atom,void*) ;
unsigned long		(*atomized)(void) ;
unsigned long		(*atomogened)(void) ;
long			(*reatomized)(void) ;
atom			(*duplicate)(atom) ;
void			(*construct)(atom) ;
atom			(*oLibrary)(string) ;
void			(*yield_task)(void) ;
task			(*getTask)(void) ;
task			(*new_task)(void) ;
string			(*aAtomatrixSeriestring)(void) ;
#if defined( MICROSOFT_win32 )
string			(*aAlchematrixVersion)(void) ;
#endif
atom			aUnicode ;
atom			aUnicoded ;
atom			aAscii ;
atom			aAsciiString  ;
atom			aUnicodedString  ;
atom			aAsciiCharacter  ;
atom			aUnicodedEntity  ;
atom			(*aAsciiString_atomize)(string) ;
atom			(*aAsciiCharacter_atomize)(byte) ;
atom			(*aUnicodedString_atomize)(unicodes) ;
atom			(*aUnicodedEntity_atomize)(unicode) ;

atom			aInteger ;
atom			(*oInteger)(int);

atom			aLibraryVariant ;
atom			aVariant ;
atom			(*oVariant)(atom,atom) ;

atom			aLibraryVariable ;
atom			aVariable;
atom			(*addAtom)(atom,string,atom) ;

atom			aVarray;
atom			aVarrayReferent;
atom			(*aVarray_duplicate)(atom) ;
atom			(*aVarray_atomize_a_n)(atom,int);
atom			(*aVarrayReferent_atomize_a_n)(atom,int);
void			(*aVarray_copy)(atom,atom);
void			(*aVarray_append_v)(atom,atom);
void			(*aVarray_append_v_a)(atom,atom);
atom			(*aVarray_append_a_a)(atom,atom);
void			(*aVarray_empty)(atom);

atom			aLibraryObject ;
atom			aObjVar;
atom			aSObjVar;
atom			(*objvar_a_s)(atom,string);
object			(*MOBJ)(atom);
object			(*COBJ)(ObjID);
atom			(*POBJ)(ObjID);
atom			(*getVariable_A_S)(atom,string) ;
atom			(*getProgram)(atom,string,string*) ;
atom			aModule;
atom			aProgram;
atom			(*aModule_grammance_a_s)(atom,string) ;

atom			aLibraryError ;
atom			aError;
atom			(*oError)(string) ;
atom			(*aError_atomize_a)(atom) ;
atom    		aError_VARNF;
atom    		aError_RANGE;
atom    		aError_TYPE;
atom    		aError_ARGC;
atom    		aError_INVOBJ;
atom    		aError_VERBNF;
atom    		aError_INVARG;
atom    		aError_EXISTS;
atom    		aError_PROGNF;
atom    		aError_COMP;
atom    		aError_RECR;
atom    		aError_PERM;
atom    		aError_CALL_FAILED;


void _lib preconstructor( atom A )
	{
	extern void		aIndexInteger_preconstructent(atom) ;
	extern atom		Evaluate(string,atom*) ;
	extern atom		Inherit(string,atom*) ;
	extern atom		sframe_scope(void) ;
	extern int		sframe_datum(atom*) ;
	extern atom		sframe_return(int) ;
	extern string		VarToStr(atom) ;

	aAtomatrix		= ATOMATRIX_ATOMICAL ;
	oAtomatrixFunction	= aAtomatrix->datum ;
	oAtomatrixAtomical	= oAtomatrixFunction( "oAtomatrixAtomical" ) ;
	oAtomical		= oAtomatrixFunction( "oAtomical" ) ;
	oFunction		= oAtomatrixFunction( "oFunction" ) ;
	atomine			= oAtomatrixAtomical( "atomine" ) ;
	atomize			= oAtomatrixFunction( "atomize" ) ;
	aAtomatrixSeriestring	= oAtomatrixFunction( "seriestring" ) ;
#if defined( MICROSOFT_win32 )
	aAlchematrixVersion	= oAtomatrixFunction( "seriesAlchematrix" ) ;
#endif
	oAtomical( aAtomatrix , "aLibraryMachine" , A ) ;
#	define __(_1)	\
		oAtomical( A , "aOprand" #_1 , aOprand##_1 = atomize( atomine ) ) ;
	__( Add ) ;
	__( Divide ) ;
	__( Condition ) ;
	__( CompareEqual ) ;
	__( Iterate ) ;
	__( Iterize ) ;
	__( Equals ) ;
	__( AddEquals ) ;
	__( AddEquate ) ;
	__( MinusEquals ) ;
	__( MinusEquate ) ;
	__( BinaryAnd ) ;
	__( BinaryOr ) ;
	__( BinaryNegation ) ;
	__( Index ) ;
	__( PopLeft ) ;
	__( PopRight ) ;
	__( Pull ) ;
	__( PushLeft ) ;
	__( PushRight ) ;
	__( Splice ) ;
	__( Split ) ;
	__( Evaluate ) ;
	__( GreaterThan ) ;
	__( LessThan ) ;
	__( GreaterOrEqual ) ;
	__( LessOrEqual ) ;
#	undef __
#	define __(_1)	\
		oAtomical( A , #_1 , _1 = atomize( atomine ) ) ;
	__( aIterum ) ;
	__( aEquality ) ;
	__( aMethod ) ;
	aIndexInteger_preconstructent( A ) ;
	oFunction( A , "sframe_return", sframe_return ) ;
	oFunction( A , "sframe_datum" ,	sframe_datum ) ;
	oFunction( A , "sframe_scope" ,	sframe_scope ) ;
	oFunction( A , "evaluate" ,	Evaluate ) ;
	oFunction( A , "inherit" ,	Inherit ) ;
	oFunction( A , "VarToStr" ,	VarToStr ) ;
	}


void method_destructor( atom A )
	{
	method	m = A->datum ;
	if( m )
		dealloc( m ) ;
	A->datum = 0 ;
	}


atom aEquality_oprandCondition( atom A )
	{
	return A ;
	}


void _lib constructor( atom A )
	{
	extern void		tkn_constructor(void);
	extern void		machine(void);
	extern void		yieldTask(void) ;
	extern int		execute_a_s_a(atom,string,atom);
	extern void		executable_destructor(atom);
	extern atom		o_attach(atom,unsigned,atom*);
	extern atom		o_atomized(atom,unsigned,atom*);
	extern atom		aAtomatrix_addTask(atom,unsigned,atom*);
	extern void		aIndexInteger_constructent(void) ;
	atom			_a ;
	atom			a ;

#	undef __
#	define __(_1)		_1 = oAtomatrixAtomical( #_1 )
#	define ___(_1)		_1 = oAtomatrixFunction( #_1 )
	__( aUndefined ) ;
	__( aIllogical ) ;
	__( aAtomineAsciiString ) ;
	__( aDirectionAtomatrix ) ;
	___( oLibraryAtomical ) ;
	___( oLibraryFunction ) ;
	___( destruct ) ;
	___( interatomize ) ;
	___( intratomize ) ;
	___( atomethodate ) ;
	___( atomethodize ) ;
	___( allocm ) ;
	___( allocz ) ;
	___( allocr ) ;
	___( dealloc ) ;
	___( check_memory ) ;
	___( output ) ;
	___( input ) ;
	___( add_s ) ;
	___( atomized ) ;
	___( atomogened	) ;
	___( reatomized	) ;
	___( duplicate ) ;
	___( oDestructor ) ;
	___( str_dup ) ;
	___( str_cpy ) ;
	___( str_cat ) ;
	___( str_len ) ;
	___( construct ) ;
	___( oLibrary ) ;
	___( yield_task ) ;
	___( getTask ) ;
	___( new_task ) ;
	__( aInteger ) ;
	__( aUnicode ) ;
#	undef __
#	undef ___
#	define _(_1)		_a = _1 = oAtomatrixAtomical( #_1 )
#	define __(_1)		_1 = oLibraryAtomical( _a , #_1 )
#	define ___(_1)		_1 = oLibraryFunction( _a , #_1 )
	_( aLibraryVariant ) ;
		__( aVariant ) ;
		___( oVariant ) ;
	_( aLibraryVariable ) ;
		__( aVariable ) ;
		___( getVariable_A_S ) ;
		___( addAtom ) ;
	_( aLibraryError ) ;
		__( aError ) ;
		__( aError_VARNF ) ;
		__( aError_RANGE ) ;
		__( aError_TYPE ) ;
		__( aError_ARGC ) ;
		__( aError_INVOBJ ) ;
		__( aError_VERBNF ) ;
		__( aError_INVARG ) ;
		__( aError_EXISTS ) ;
		__( aError_PROGNF ) ;
		__( aError_COMP ) ;
		__( aError_RECR ) ;
		__( aError_PERM ) ;
		__( aError_CALL_FAILED ) ;
		___( oError ) ;
		aError_atomize_a = oLibraryFunction( aError , "atomize_a" ) ;	// UPDATE: make macro
	_( aLibraryObject ) ;
		__( aModule ) ;
		__( aProgram ) ;
		__( aObjVar ) ;
		__( aSObjVar ) ;
		___( objvar_a_s ) ;
		___( MOBJ ) ;
		___( COBJ ) ;
		___( POBJ ) ;
		___( getProgram ) ;
	aVarray			= oAtomatrixAtomical( "aVarray" ) ;
	aVarray_append_a_a	= oLibraryFunction( aVarray , "append_a_a" ) ;
	aVarray_atomize_a_n	= oLibraryFunction( aVarray , "atomize_a_n" ) ;
	aVarrayReferent		= oLibraryAtomical( aVarray , "referent" ) ;
	aVarrayReferent_atomize_a_n	= oLibraryFunction( aVarrayReferent , "atomize_a_n" ) ;
	aVarray_append_v_a	= oLibraryFunction( aVarray , "append_v_a" ) ;
	aVarray_append_v	= oLibraryFunction( aVarray , "append_v" ) ;
	aVarray_duplicate	= oLibraryFunction( aVarray , "duplicate" ) ;
	aVarray_copy		= oLibraryFunction( aVarray , "copy" ) ;
	aVarray_empty		= oLibraryFunction( aVarray , "empty" ) ;

	aModule_grammance_a_s	= oLibraryFunction( aModule , "grammance_a_s" ) ;

	aAscii		= oLibraryAtomical( aUnicode , "ascii" ) ;
	aUnicoded	= oLibraryAtomical( aUnicode , "unicoded" ) ;
	aAsciiString	= oLibraryAtomical( aAscii , "string" ) ;
	aAsciiCharacter	= oLibraryAtomical( aAscii , "character" ) ;
	aUnicodedString	= oLibraryAtomical( aUnicoded , "string" ) ;
	aUnicodedEntity	= oLibraryAtomical( aUnicoded , "entity" ) ;
	oInteger	= oLibraryFunction( aInteger , "atomize" ) ;
	aAsciiString_atomize = oLibraryFunction( aAsciiString , "atomize" ) ;

	aExecutable		= atomize( atomine ) ;  //UPDATE: aExecutant
	aSignal_REPEAT_FUNCTION	= atomize( atomine ) ;	//UPDATE: aExecutionRepeat
	aSignal_EXEC_CONTINUE	= atomize( atomine ) ;	//UPDATE: aExecutionContinue
	aSignal_JUMP		= atomize( atomine ) ;	//UPDATE: aExecutionJump
	aMacroprocessity	= atomize( atomine ) ;

	aError_VARNF->allusions++ ;
	addAtom( aError , "varnf" ,	aError_VARNF ) ;
	aError_RANGE->allusions++ ;
	addAtom( aError , "range" ,	aError_RANGE ) ;
	aError_TYPE->allusions++ ;
	addAtom( aError , "type" ,	aError_TYPE ) ;
	aError_ARGC->allusions++ ;
	addAtom( aError , "argc" ,	aError_ARGC ) ;
	aError_INVOBJ->allusions++ ;
	addAtom( aError , "invobj" ,	aError_INVOBJ ) ;
	aError_VERBNF->allusions++ ;
	addAtom( aError , "verbnf" ,	aError_VERBNF ) ;
	aError_INVARG->allusions++ ;
	addAtom( aError , "invarg" ,	aError_INVARG ) ;
	aError_EXISTS->allusions++ ;
	addAtom( aError , "exists" ,	aError_EXISTS ) ;
	aError_PROGNF->allusions++ ;
	addAtom( aError , "prognf" ,	aError_PROGNF ) ;
	aError_COMP->allusions++ ;
	addAtom( aError , "comp" ,	aError_COMP ) ;
	aError_RECR->allusions++ ;
	addAtom( aError , "recr" ,	aError_RECR ) ;
	aError_PERM->allusions++ ;
	addAtom( aError , "perm" ,	aError_PERM ) ;

	oFunction( aAtomatrix , "function" , machine ) ;

	oFunction( A , "yieldTask" ,		yieldTask ) ;
	oFunction( A , "execute_a_s_a" ,	execute_a_s_a ) ;
	oAtomical( A , "aExecutionJump" ,	aSignal_JUMP ) ;

	atomethodize( aAtomatrix , "atomized" ,		o_atomized ) ;
	atomethodize( aAtomatrix , "addTask" ,		aAtomatrix_addTask ) ;

	( a = atomize( aOprandCondition ) )->datum = aEquality_oprandCondition ;
	interatomize( a , aEquality ) ;

	oDestructor( aMethod , method_destructor ) ;

	oDestructor( aExecutable , executable_destructor ) ;
	aIndexInteger_constructent() ;
	tkn_constructor();
	}


void _lib destructor( atom A )
	{
	extern void		tkn_destructor(void);
	tkn_destructor() ;
	destruct( aMethod ) ;
	destruct( aExecutable ) ;
	}
