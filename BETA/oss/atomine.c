#include "ATOMATRIX.h"


void atomine_destructor( atom A )
	{
	extern void		destruct(void*);
	atom			a;

	while( a = A->intratom )
		destruct( a ) ;
	while( a = A->interatom )
		destruct( a ) ;
	}


string	hexichar	= "0123456789ABCDEF" ;


int atomine_asciiString( atom A , string S , unsigned int L , unsigned int* Z )
	{
	extern atom		atomine ;
	string			s ;
	string			p ;
	unsigned int		i ;
	unsigned int		x ;
	unsigned int		l ;

	i	= 0 ;
	s	= S ;
	if( A == atomine )
		{
		p	= "ATOMINE" ;
		do	if( i++ < L )
				*s++  = *p ;
			while( *++p ) ;
		*Z	= i ;
		return ( i <= L ) ;
		}
	p	= "ATOM@" ;
	do	if( i++ < L )
			*s++  = *p ;
		while( *++p ) ;
	x	= (unsigned int)A ;
	l	= 0 ;
	do	l++ ;
		while( x /= 16 ) ;
	x	= (unsigned int)A ;

	if( (i+l) < L )
		{
		s	+= l ;
		do	*--s = hexichar[ x % 16 ] ;
			while( x /= 16 ) ;
		*Z	= i + l ;
		return 1 ;
		}
	if( i > L )
		{
		*Z	= i ;
		return 0 ;
		}
	do	x	/= 16 ;
		while( (i+--l) > L ) ;
	s	+= l ;
	do	*--s = hexichar[ x % 16 ] ;
		while( x /= 16 ) ;
	*Z	= i + l ;
	return 0 ;
	}


atom atomine_destruct(atom A , int T , atom* V )
	{
	extern void		destruct(void*) ;
	atom			a ;

	while( a = A->intratom )
		destruct( a ) ;
	while( a = A->interatom )
		destruct( a ) ;
	return atominul;
	}


void atomine_constructor( void )
	{
	extern atom		atomine ;
	extern atom		aDestructor ;
	extern atom		aAtomineAsciiString ;
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;

	a	= atomize( aDestructor ) ;
	a->datum = atomine_destructor ;
	interatomize( a , atomine ) ;

	a	= atomize( aAtomineAsciiString ) ;
	a->datum = atomine_asciiString ;
	interatomize( a , atomine ) ;
	}


void atomine_preconstructor( void )
	{
	extern atom		atomine ;
	extern atom		allocm(unsigned int);
	atom			a;

	a			= allocm( sizeof(struct _atom) );
	a->allusions		= 1;
	a->precursor		= atominul;
	a->intratom		= atominul;
	a->intratoms		= atominul;
	a->orbit		= atominul;
	a->interatom		= atominul;
	a->interatoms		= atominul;
	a->datum		= atominul;
	atomine			= a ;
	}