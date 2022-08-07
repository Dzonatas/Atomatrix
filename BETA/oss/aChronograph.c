// aChronograph.c		Copyright (c) 2004 BALLARD	

#include "ATOMATRIX.h"
#include "windows.h"

atom				aChronograph ;
						// UPDATE:	atomize aChronological for a precursor

atom aChronograph_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		atomize(atom) ;

	return aChronograph = atomize( atomine ) ;
	}


atom aChronograph$presentime( atom A , int T , atom* V )
	{
	extern atom		aCronogram_atomize_utc(int,byte,byte,byte,byte,byte,unsigned) ;
	SYSTEMTIME		t ;

	GetSystemTime( &t ) ;
	return aCronogram_atomize_utc( t.wYear , (byte)t.wMonth , (byte)t.wDay , (byte)t.wHour , (byte)t.wMinute , (byte)t.wSecond , t.wMilliseconds ) ;
	}


void aChronograph_constructent( void )
	{
	extern atom		aAtomatrix ;
	extern atom		atomize(atom) ;
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*)) ;
	extern void		interatomize(atom,atom) ;

#	undef __
#	define __(_1)								\
		{								\
		extern atom	aChronograph$##_1(atom,int,atom*) ;		\
		atomethodize( aChronograph , #_1 , aChronograph$##_1 ) ;	\
		}
	__( presentime ) ;
	}


void aChronograph_destructor( void )
	{
	extern void		destruct(atom) ;

	destruct( aChronograph ) ;
	}