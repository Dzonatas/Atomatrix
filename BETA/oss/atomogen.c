/***************************************
*
* atomogen_gro.c
*
*******************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#include "ATOMATRIX.h"

#define MEGABYTE	1048576		// 2^20

#define PREGEN		((MEGABYTE*2)/sizeof(struct _atom))

#ifdef PREGEN
void*			atomogen_mem ;
#endif

byte*			atomogen_scalar ;

void atomogen_constructor( void )
	{
	extern atom		atomogen ;
	extern void *		allocm(unsigned) ;
	extern void *		allocz(unsigned) ;
	extern unsigned long	atomized;
	atom			a;
#	ifdef PREGEN
	atom			b ;
	int i ;
#	endif

#	ifndef PREGEN

	a			= allocz( sizeof(struct _atom) );
	a->allusions		= 1;
	atomogen		= a;

#	else

	atomogen_mem = a	= allocz( sizeof(struct _atom) * PREGEN ) ;
	a->allusions		= 1;
	atomogen		= a;

	b			= a+1 ;
	a->intratom		= b ;
	atomized = i		= PREGEN-1 ;
	while( --i )
		{
		b->intratoms	= b+1 ;
		b->precursor	= a ;
		b++ ;
		}
	(b-1)->intratoms	= 0 ;

#	endif
	}


void atomogen_destructor( void )
	{
	extern atom		atomogen ;
	extern void		dealloc(void*);
	extern unsigned long	atomized;

#ifdef PREGEN
	dealloc( atomogen_mem ) ;
#else
	atom			a ;
	atom			n ;

	a = atomogen->intratom ;
	while( a )
		{
		n = a->intratoms ;
		dealloc( a ) ;
		atomized-- ;
		a = n ;
		}
	// dealloc( atomogen )			// UPDATE: dealloc needed ?
#endif
	}

atom aAtomatrix_atomogenBalance( atom A , int T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aAsciiString ;
	extern atom		atomogen ;
	extern void *		allocm(unsigned) ;
	extern void		console_output(string) ;
	atom			a ;
	unsigned		x ;
	unsigned		y = 1 ;
	unsigned		z1, z2 ;
	char			s[200] ;
	extern unsigned long	atomized;

	if( ! atomogen_scalar )
		{
		atomogen_scalar = allocm( PREGEN ) ;
		y = 0 ;
		}
	a = atomogen_mem ;
	x = 1 ;
	z1 = z2 = 0 ;
	while( x < PREGEN )
		{
		++a ;
		if( y && atomogen_scalar[x] == 0 && a->precursor != atomogen )
			{
			if( a->precursor == aInteger )
				sprintf( s , "+ %08x  %c %c %08x\n" , (unsigned int)a , 'I' ,	a->orbit ? 'O' : '.' , (unsigned int)a->precursor ) ;  //UPDATE: use hex pointer instead of (unsigned int)
			else
			if( a->precursor == aAsciiString )
				sprintf( s , "+ %08x  %c %c %08x  %.20s\n" , (unsigned int)a , 'S' ,	a->orbit ? 'O' : '.' , (unsigned int)a->precursor ,
				(string)a->datum
				) ;
			else
				sprintf( s , "+ %08x  %c %c %08x\n" , (unsigned int)a , '.' ,	a->orbit ? 'O' : '.' , (unsigned int)a->precursor ) ;
			console_output( s ) ;
			z1++ ;
			}
		if( y && atomogen_scalar[x] && a->precursor == atomogen )
			{
			sprintf( s , "- %x\n" , (unsigned int)a) ;
			console_output( s ) ;
			z2++ ;
			}
		atomogen_scalar[x++] = (byte)( a->precursor != atomogen ) ;
		}
	if( y ) {
		sprintf( s , "=%d   +=%d   -=%d\n" , z1-z2 , z1 , z2 ) ;
		console_output( s ) ;
		}
	return 0 ;
	}