/***************************************
*
* aVReality_format.c
*
*******************/


#include "ATOMATRIX.h"

#include <stdlib.h>
#include <ctype.h>


#define F_DONE 0
#define F_NEXT 1
#define F_FIELD 2
#define F_DEC 3

atom aVReality_format( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void*		(*allocm)(unsigned int) ;
	extern void*		(*allocr)(void*,unsigned int) ;
	extern unsigned int	(*str_len)(string) ;
	extern string		(*VarToStr)(atom);
	int			i,c,s,l,len,x,f,t,nl;
	string			str;
	string			nstr;
	string			p;

	if( ! T >= 2 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}

		if( V[0]->precursor != aAsciiString )			// UPDATE: unicode
			{
			aError_INVARG->allusions++;
			return aError_INVARG;
			}

	str	= 0;
	i	= 1;
	len = l = c = 0;
	s	= F_NEXT;
	p	= V[0]->datum;

	while( s != F_DONE )
		{
		switch(s)
			{
			case F_NEXT:
				while(p[c]!=0 && p[c]!='^')
					c++;
				if( c != l )
					{
					if( str )
						str = allocr( str, len+(c-l)+1 );
					else
						str = allocm( len+(c-l)+1 );
					x	= len;
					len	+= (c-l);
					while( x < len )
						str[x++] = p[l++];
					}
				if( p[c]==0 )
					s = F_DONE;
				else
					s = F_FIELD;
				c++;
				l = c;
				continue;
			case F_FIELD:
				t = f = 0;
				if( p[c] == '^' )
					{
					c++;
					s = F_NEXT;
					continue;
					}
				if( p[c] == '-' )
					{
					t+=1;
					c++;
					}
				if( p[c] == '0' )
					{
					t+=2;
					c++;
					}
				while( isdigit(p[c]) )
					{
					f*=10;
					f+=(p[c++]-'0');
					}
				l = c;
				if( f > 100 )
					f=100;  			/* 1 */
				if( f > 0 && i < T )
					{
					if( str )
						str = allocr( str, len+f+1 );
					else
						str = allocm( len+f+1 );
					nstr	= VarToStr(V[i]);
					nl	= str_len(nstr);
					x	= len;
					len	+= f;
					if( ! ( t & 1 ) )
						{
						if( t & 2 )
							while( x < ( len-nl ) )
								str[x++] = '0';
						else
							while( x < ( len-nl ) )
								str[x++] = ' ';
						}
					while( x < len && *nstr != 0 )
						str[x++] = *(nstr++);
					while( x < len )
						str[x++] = ' ';
					}
				s	= F_NEXT;
				i++;
				continue;
			}
		}

	if( ! str )
		str= (string) allocm( 1 );

	str[len] = 0;

	return aAsciiString_atomize( str );
	}


/***************************************
*
* UPDATE:
* 1 - Should return error if too big.
*
*******************/
