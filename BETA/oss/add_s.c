/***************************************
*
* add_s.c
*
*******************/


#include <stdlib.h>

#include "ATOMATRIX.h"


string add_s(string str1, string str2)
{
extern void *	allocm(unsigned int) ;
string		p;
string		s;
int 		size		= 1;

if( str1 )
	{
	s	= str1;
	while( *(s++) )
		size++;
	}
else
	str1 = "";

if( str2 )
	{
	s	= str2;
	while( *(s++) )
		size++;
	}
else
	str2 = "";

p = (string) allocm( size );

s	= str1;
while( *s )
	*p++ = *s++;

s	= str2;
while( *s )
	*p++ = *s++;

*p++	= (char)0;

return &p[-size];
}
