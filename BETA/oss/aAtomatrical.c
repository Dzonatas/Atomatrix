// aAtomatrical.c		Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"

atom				aAtomatrical ;

atom aAtomatrical_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		atomize(atom) ;

	return  aAtomatrical = atomize( atomine ) ;
	}


void aAtomatrical_destruct_a( atom A )
	{
	extern void		destruct(atom) ;

	if( A->datum )
		{
		if( --( (atom) A->datum )->allusions == 0 )
			destruct( A->datum ) ;
		A->datum = 0 ;
		}
	}


void aAtomatrical_constructent( void )
	{
	extern atom		oDestructor(atom,void*) ;

	oDestructor( aAtomatrical , aAtomatrical_destruct_a ) ;
	}


void aAtomatrical_destructor( void )
	{
	extern void		destruct(atom) ;

	destruct( aAtomatrical ) ;
	}
