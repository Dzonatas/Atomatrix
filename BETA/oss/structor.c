// structor.c		Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"
#include <windows.h>

#include "machine/avei.h"
#include "machine/sframe.h"
#include "task.h"

unsigned long		atomized;
unsigned long		atomogened;
long			reatomized;
string			direction_ATOMATRIX ;
void			(*function)(void) ;

atom _lib		ATOMATRIX_ATOMICAL ;
atom			atomine ;
atom			atomogen ;
atom			aAtomatrix ;
atom			aConstructor ;
atom			aDestructor ;
atom			aUndefined ;
atom			aIllogical ;
atom			aFunction ;
atom			aAtomical ;
atom			aDuplicator ;
atom			aAtomineAsciiString ;
atom			aDirectionAtomatrix ;

atom			aSymbol ;

extern atom		aLibraryMachine ;
atom			aMethod ;
atom			aIterum ;			// UPDATE: perhaps interatomize to aAtomatrix
atom			aEquality ;			// UPDATE: interatomize to aAtomatrix
atom			aOprandIterate ;
atom			aOprandIterize ;
atom			aOprandSplice ;
atom			aOprandIndex ;
atom			aOprandCompareEqual ;
atom			aOprandCondition ;
atom			aOprandAddEquate ;
atom			aOprandAdd ;
atom			aOprandGreaterThan ;
atom			aOprandLessThan ;
atom			aOprandGreaterOrEqual ;
atom			aOprandLessOrEqual ;
atom			aOprandDivide ;
atom			aOprandMinusEquate ;
atom			aOprandBinaryNegation ;
atom			aOprandBinaryOr ;
atom			aOprandBinaryAnd ;
atom			aOprandEvaluate ;
atom			aOprandSplit ;
atom			aOprandPushRight ;
atom			aOprandPushLeft ;
atom			aOprandPull ;
atom			aOprandPopRight ;
atom			aOprandPopLeft ;
atom			aOprandEquals ;
atom			oIndexInteger ;

void			(*yieldTask)(void) ;

extern atom		aLibraryError ;
atom			(*oError)(string) ;
atom			aError_INVARG ;
atom			aError_ARGC ;
atom			aError_RANGE ;
atom			aError_TYPE ;
atom			aError_INVOBJ ;

extern atom		aLibraryObject ;
atom			(*oModule)(unsigned) ;
atom			(*getProgram)(atom,string,atom) ;

extern atom		aLibraryVariable ;
atom			aVariable ;
atom			(*addAtom)(atom,string,atom) ;

void constructor( void )
	{
	extern void		atomine_preconstructor(void);
	extern void		atomogen_constructor(void);
	extern void		atomine_constructor(void);
	extern atom		atomize(atom);
	extern atom		atomorphize(string);
	extern void		destruct(atom);
	extern string		add_s(string,string);
	extern atom		atomethodate(atom,string,int,atom*);
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*));
	extern void		interatomize(atom,atom);
	extern void		intratomize(atom,atom);
	extern atom		atomSignal_gro();
	extern string		string_duplicate(string);
	extern string		string_copy(string,string);
	extern string		string_concatenate(string,string);
	extern unsigned int	string_length(string);
	extern atom		addSymbol(atom,string);
	extern atom		getSymbol(atom,string);
	extern void *		allocm(unsigned int);
	extern void *		allocz(unsigned int);
	extern void *		allocr(void*,unsigned int);
	extern void		dealloc(void*);
	extern int		check_memory(void);
	extern void		library_destructor(atom);
	extern void		method_destructor(atom);
	extern void		console_output(string);
	extern string		console_input(void);
	extern atom		oDestructor(atom,void*) ;
	extern atom		oDuplicator(atom,void*) ;
	extern atom		oFunction(atom,string,void*) ;
	extern atom		oAtomical(atom,string,atom) ;
	extern void*		oAtomatrixFunction(string);
	extern void*		oLibraryFunction(atom,string);
	extern atom		oAtomatrixAtomical(string);
	extern atom		oLibraryAtomical(atom,string);
	extern void		libraries_preconstructor(void);
	extern void		libraries_constructor(void);
	extern atom		duplicate(atom) ;
	extern void		construct(atom) ;
	extern atom		oLibrary(string) ;
	extern unsigned long	_atomized(void) ;
	extern long		_atomogened(void) ;
	extern long		_reatomized(void) ;
	extern atom		aAtomatrix_output(atom,int,atom*) ;
	extern atom		aAtomatrix_input(atom,int,atom*) ;
	extern atom		aAtomatrix_exit(atom,int,atom*) ;
	extern atom		aAtomatrix_memory(atom,int,atom*) ;
	extern atom		aAtomatrix_atomogenBalance(atom,int,atom*) ;
	extern task		getTask(void) ;
	extern void		yield_task(void) ;
	extern task		new_task(void) ;
	extern void*		taskSplice(void*,void*) ;
	extern string		seriestring(void) ;
	int			x ;

	atomized		= 0;
	atomogened		= 0;
	reatomized		= 0;

	atomine_preconstructor() ;
	atomogen_constructor() ;

	aAtomatrix		= atomize( atomine ) ;
	aAtomatrix->datum	= oAtomatrixFunction ;
	aAtomical		= atomize( atomine ) ;
	aConstructor		= atomize( atomine ) ;
	aDestructor		= atomize( atomine ) ;
	aUndefined		= atomize( atomine ) ;
	aUndefined->allusions	+= 10000 ;	// UPDATE: delete line
	aIllogical		= atomize( atomine ) ;
	aSymbol			= atomize( atomine ) ;
	aFunction		= atomize( atomine ) ;
	aDuplicator		= atomize( atomine ) ;
	aAtomineAsciiString	= atomize( atomine ) ;
	aDirectionAtomatrix	= atomize( atomine ) ;

	atomine_constructor() ;
	atomSignal_gro() ;

	oAtomical( aAtomatrix , "atomine" ,		atomine ) ;
	oAtomical( aAtomatrix , "aConstructor" ,	aConstructor ) ;
	oAtomical( aAtomatrix , "aDestructor" ,		aDestructor ) ;
	oAtomical( aAtomatrix , "aUndefined" ,		aUndefined ) ;
	oAtomical( aAtomatrix , "aIllogical" ,		aIllogical ) ;
	oAtomical( aAtomatrix , "aDuplicator" ,		aDuplicator ) ;
	oAtomical( aAtomatrix , "aAtomineAsciiString" ,	aAtomineAsciiString ) ;
	oAtomical( aAtomatrix , "aDirectionAtomatrix" ,	aDirectionAtomatrix ) ;

	x	= GetEnvironmentVariable( "_ATOMATRIX" , 0 , 0 ) ;
	direction_ATOMATRIX	= allocm( x ) ;
	GetEnvironmentVariable( "_ATOMATRIX" , direction_ATOMATRIX , x ) ;
	aDirectionAtomatrix->datum = direction_ATOMATRIX ;
	oAtomical( aAtomatrix , "aDirectionAtomatrix" , aDirectionAtomatrix ) ;

	oFunction( aAtomatrix , "atomize" ,		atomize ) ;
	oFunction( aAtomatrix , "oAtomatrixAtomical" ,	oAtomatrixAtomical ) ;
	oFunction( aAtomatrix , "oAtomical" ,		oAtomical ) ;
	oFunction( aAtomatrix , "oFunction" ,		oFunction ) ;
	oFunction( aAtomatrix , "seriestring" ,		seriestring ) ;
#if defined( MICROSOFT_win32 )
	{
	extern string		seriesAlchematrix(void) ;
	oFunction( aAtomatrix , "seriesAlchematrix" ,	seriesAlchematrix ) ;		// UPDATE: temporary, need aAlchematrix
	}
#endif

#define _(_1)						\
	{						\
	extern atom _1##_pregeneration(void) ;		\
	oAtomical( aAtomatrix , #_1 , _1##_pregeneration() ) ; \
	}
	_( aAtomatrical ) ;
	_( aUnicode ) ;
	_( aInteger ) ;
	_( aVarray ) ;
	_( aChronograph ) ;
	_( aChronogram ) ;
	_( aXML ) ;
	libraries_preconstructor() ;


	oFunction( aAtomatrix , "destruct" ,		destruct ) ;
	oFunction( aAtomatrix , "interatomize" ,	interatomize ) ;
	oFunction( aAtomatrix , "intratomize" ,		intratomize ) ;
	oFunction( aAtomatrix , "oLibraryFunction" ,	oLibraryFunction ) ;
	oFunction( aAtomatrix , "oLibraryAtomical" ,	oLibraryAtomical ) ;
	oFunction( aAtomatrix , "add_s" ,		add_s ) ;
	oFunction( aAtomatrix , "atomethodate" ,	atomethodate ) ;
	oFunction( aAtomatrix , "atomethodize" ,	atomethodize ) ;
	oFunction( aAtomatrix , "str_dup" ,		string_duplicate ) ;
	oFunction( aAtomatrix , "str_cpy" ,		string_copy ) ;
	oFunction( aAtomatrix , "str_cat" ,		string_concatenate ) ;
	oFunction( aAtomatrix , "str_len" ,		string_length ) ;
	oFunction( aAtomatrix , "allocm" ,		allocm ) ;
	oFunction( aAtomatrix , "allocz" ,		allocz ) ;
	oFunction( aAtomatrix , "allocr" ,		allocr ) ;
	oFunction( aAtomatrix , "dealloc" ,		dealloc ) ;
	oFunction( aAtomatrix , "check_memory" ,	check_memory ) ;
	oFunction( aAtomatrix , "output" ,		console_output ) ;
	oFunction( aAtomatrix ,	"input" ,		console_input ) ;
	oFunction( aAtomatrix , "oDestructor" ,		oDestructor ) ;
	oFunction( aAtomatrix , "oDuplicator" ,		oDuplicator ) ;
	oFunction( aAtomatrix , "duplicate" ,		duplicate ) ;
	oFunction( aAtomatrix , "atomized" ,		_atomized ) ;
	oFunction( aAtomatrix , "atomogened" ,		_atomogened ) ;
	oFunction( aAtomatrix , "reatomized" ,		_reatomized ) ;
	oFunction( aAtomatrix , "construct" ,		construct ) ;
	oFunction( aAtomatrix , "oLibrary" ,		oLibrary ) ;
	oFunction( aAtomatrix , "getTask" ,		getTask ) ;
	oFunction( aAtomatrix , "yield_task" ,		yield_task ) ;
	oFunction( aAtomatrix , "new_task" ,		new_task ) ;
	oFunction( aAtomatrix , "taskSplice" ,		taskSplice ) ;

#	undef _
#	define _(_1,_2)		\
		{			\
		extern atom	_1 , _2 ; \
		_2 = oLibraryAtomical( _1 , #_2 ) ; \
		} 

	_( aLibraryError , aError_INVARG ) ;
	_( aLibraryError , aError_ARGC ) ; 
	_( aLibraryError , aError_RANGE ) ;
	_( aLibraryError , aError_TYPE ) ;
	_( aLibraryError , aError_INVOBJ ) ;
	_( aLibraryMachine , aMethod ) ;
	_( aLibraryMachine , aEquality ) ;
	_( aLibraryMachine , aIterum ) ;
	_( aLibraryMachine , aOprandIterate ) ;
	_( aLibraryMachine , aOprandIterize ) ;
	_( aLibraryMachine , aOprandSplice ) ;
	_( aLibraryMachine , aOprandIndex ) ;
	_( aLibraryMachine , aOprandCompareEqual ) ;
	_( aLibraryMachine , aOprandCondition ) ;
	_( aLibraryMachine , aOprandAddEquate ) ;
	_( aLibraryMachine , aOprandAdd ) ;
	_( aLibraryMachine , aOprandGreaterThan ) ;
	_( aLibraryMachine , aOprandGreaterOrEqual ) ;
	_( aLibraryMachine , aOprandLessThan ) ;
	_( aLibraryMachine , aOprandLessOrEqual ) ;
	_( aLibraryMachine , aOprandBinaryAnd ) ;
	_( aLibraryMachine , aOprandBinaryOr ) ;
	_( aLibraryMachine , aOprandBinaryNegation ) ;
	_( aLibraryMachine , aOprandMinusEquate ) ;
	_( aLibraryMachine , aOprandDivide ) ;
	_( aLibraryMachine , aOprandEvaluate ) ;
	_( aLibraryMachine , aOprandSplit ) ;
	_( aLibraryMachine , aOprandPushRight ) ;
	_( aLibraryMachine , aOprandPushLeft ) ;
	_( aLibraryMachine , aOprandPull ) ;
	_( aLibraryMachine , aOprandPopRight ) ;
	_( aLibraryMachine , aOprandPopLeft ) ;
	_( aLibraryMachine , aOprandEquals ) ;
	_( aLibraryVariable , aVariable ) ;

	oModule			= oLibraryFunction( aLibraryObject ,	"oModule" ) ;
	oIndexInteger		= oLibraryFunction( aLibraryMachine ,	"oIndexInteger" ) ;
	oError			= oLibraryFunction( aLibraryError ,	"oError" ) ;
	addAtom			= oLibraryFunction( aVariable , "addAtom" ) ;

#undef _
#define _(_1)						\
	{						\
	extern void _1##_constructent(atom) ;		\
	_1##_constructent( aAtomatrix ) ;		\
	}
	_( aAtomatrical ) ;
	_( aUnicode ) ;
	_( aInteger ) ;
	_( aVarray ) ;
	_( aChronograph ) ;
	_( aChronogram ) ;
	_( aXML ) ;

	libraries_constructor() ;

	atomethodize( aAtomatrix , "output" ,	aAtomatrix_output ) ;
	atomethodize( aAtomatrix , "input" ,	aAtomatrix_input ) ;
	atomethodize( aAtomatrix , "exit" ,	aAtomatrix_exit ) ;
	atomethodize( aAtomatrix , "memory" ,	aAtomatrix_memory ) ;
	atomethodize( aAtomatrix , "atomogenBalance" ,	aAtomatrix_atomogenBalance ) ;

#undef _
#define _(_1)								\
	{								\
	extern atom		aAtomatrix$##_1(atom,int,atom*) ;	\
	atomethodize( aAtomatrix , #_1 , aAtomatrix$##_1 ) ;		\
	}
	_( attach ) ;

	function	= oAtomatrixFunction( "function" ) ;
	yieldTask	= oLibraryFunction( aLibraryMachine ,	"yieldTask" ) ;
	getProgram	= oLibraryFunction( aLibraryObject ,	"getProgram" ) ;


#undef _
#define _(_1,_2)							\
	{								\
	extern atom		_2 ;					\
	addAtom( aAtomatrix , #_1 , _2 ) ;				\
	}
	_( chronograph , aChronograph ) ;
	}


void destructor( void )
	{
	extern void		atomine_destructor(atom) ;
	extern void		atomogen_destructor(void) ;
	extern void		dealloc(void*) ;
	extern void		destruct(atom) ;

#undef _
#define _(_1)						\
	{						\
	extern void _1##_destructor(void) ;		\
	_1##_destructor() ;		\
	}
	_( libraries ) ;
	_( aXML ) ;
	_( aChronogram ) ;
	_( aChronograph ) ;
	_( aVarray ) ;
	_( aInteger ) ;
	_( aUnicode ) ;
	_( aAtomatrical ) ;
	destruct( aSymbol ) ;
 	atomine_destructor( atomine ) ;
	atomogen_destructor() ;
	dealloc( atomine ) ;
	dealloc( direction_ATOMATRIX ) ;
	}


atom oFunction( atom A , string S , void* V )
	{
	atom			a ;
	atom			b ;
	string			s ;
	string			p ;

	a	= A->interatom;
	while( a )
		{
		if( a->precursor == aFunction )
			{
			p = a->interatom->datum ;
			s = S ;
			while( *s && *p && *s == *p )
				s++ , p++ ;
			if( *s == *p )
				{
				// a->allusions++ ;
				return a ;
				}
			}
		a = a->interatoms;
		}
	if( ! V )
		{
		_asm int 3 ;
		return atominul ;
		}
	a		= atomize( aFunction ) ;
	b		= atomize( aSymbol ) ;
	a->datum	= V ;
	b->datum	= S ;
	interatomize( b , a ) ;
	interatomize( a , A ) ;
	return a ;
	}


atom oAtomical( atom A , string S , atom B )
	{
	atom			a ;
	atom			b ;
	string			s ;
	string			p ;

	a	= A->interatom;
	while( a )
		{
		if( a->precursor == aAtomical )
			{
			p = a->interatom->datum ;
			s = S ;
			while( *s && *p && *s == *p )
				s++ , p++ ;
			if( *s == *p )
				{
				// a->allusions++ ;
				return a ;
				}
			}
		a = a->interatoms;
		}
	if( ! B )
		{
		_asm int 3 ;
		return atominul ;
		}
	a		= atomize( aAtomical ) ;
	b		= atomize( aSymbol ) ;
	a->datum	= B ;
	b->datum	= S ;
	interatomize( b , a ) ;
	interatomize( a , A ) ;
	return a ;
	}


void* oAtomatrixFunction( string S )
	{
	atom			a ;
	a	= oFunction( aAtomatrix , S , 0 ) ;
	return a->datum ;
	}


void* oLibraryFunction( atom A , string S )
	{
	atom			a ;
	a	= oFunction( A , S , 0 ) ;
	return a->datum ;
	}


atom oAtomatrixAtomical( string S )
	{
	atom			a ;
	a	= oAtomical( aAtomatrix , S , 0 ) ;
	return a->datum ;
	}


atom oLibraryAtomical( atom A , string S )
	{
	atom			a ;
	a	= oAtomical( A , S , 0 ) ;
	return a->datum ;
	}
