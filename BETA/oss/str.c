/***************************************
*
* str.c
*
*******************/


#include "ATOMATRIX.h"
#include <stdlib.h>


string string_duplicate( string A )
{
extern void *		allocm(unsigned) ;
int			l;
string			s;
string			p;
string			r;

if( A )
	{
	l = 0;
	s = A;
	while( *(s++) )
		l++;
	r = p = allocm( l + 1 );
	s = A;
	while( *s ) 
		*(p++) = *(s++);
	*p = 0;
	}
else	{
	r = allocm( 1 );
	*r = 0;
	}

return r;
}



string string_copy( string A, string B )
{
string			a;
string			b;

if( A && B )
	{
	a = A;
	b = B;
	while( *b )
		*(a++) = *(b++);
	*a = 0;
	}
	
return A;
}



string string_concatenate( string A, string B )
{
string			a;
string			b;

if( A && B )
	{
	a = A;
	b = B;
	while( *a )
		a++;
	while( *b )
		*(a++) = *(b++);
	*a = 0;
	}

return A;
}



unsigned int  string_length( string A )
{
string			a;
unsigned int		l;

l = 0;

if( A )
	{
	a = A;
	while( *(a++) )
		l++;
	}

return l;
}
