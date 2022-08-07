/********************************************
*
* aIndexInteger
*
*************************/


#include "ATOMATRIX.h"


atom			aIndexInteger ;


atom oIndexInteger( int N )
	{
	extern atom		(*atomize)(atom) ;
	atom			a ;

	( a = atomize( aIndexInteger ) )->datum = (void*)N ;
	return a ;
	}


void aIndexInteger_preconstructent( atom A )
	{
	extern atom		atomine ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*oFunction)(atom,string,void*) ;
	extern atom		(*oAtomical)(atom,string,atom) ;

	oAtomical( A , "aIndexInteger" , aIndexInteger = atomize( atomine ) ) ;
	oFunction( A , "oIndexInteger" , oIndexInteger ) ;
	}

atom aIndexInteger_aOprandCondition( atom A )
	{
	extern atom		aInteger ;
	extern void		(*intratomize)(atom,atom) ;

	intratomize( A , aInteger ) ;
	return A ;
	}


atom aIndexInteger_aOprandEvaluate( atom A )
	{
	extern atom		aInteger ;
	extern void		(*intratomize)(atom,atom) ;

	intratomize( A , aInteger ) ;
	A->allusions++ ;
	return A ;
	}


atom aIndexInteger_integer( atom A , unsigned T , atom* V )
	{
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		(*oInteger)(unsigned) ;

	if( A->precursor != aIndexInteger )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	return oInteger( (unsigned)A->datum ) ;
	}


void aIndexInteger_constructent( void )
	{
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern atom		(*atomethodize)(atom,string,atom(*)(atom,unsigned,atom*));
	atom			_a ;

	atomethodize( aIndexInteger , "integer", aIndexInteger_integer ) ;
#	define __(_1)							\
		{							\
		extern atom		_1 ;				\
		( _a = atomize( _1 ) )->datum = aIndexInteger_##_1 ;	\
		interatomize( _a , aIndexInteger ) ;			\
		}
	__( aOprandEvaluate ) ;
	__( aOprandCondition ) ;
	}