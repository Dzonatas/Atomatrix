/***************************************
*
* aVReality_match.c
*
*******************/


#include	"ATOMATRIX.h"

#include "cmp_s.h"
#include <ctype.h>

#undef NO_STRNCMP


atom aVReality_match( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern string		(*str_dup)(string) ;
	extern string		patternMatch(string,string);
	string			str;

	if( T != 2 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}

		if( V[0]->precursor != aAsciiString )		// UPDATE: unicode
			{
			aError_INVARG->allusions++;
			return aError_INVARG;
			}
		if( V[1]->precursor != aAsciiString )
			{
			aError_INVARG->allusions++;
			return aError_INVARG;
			}

	str = patternMatch( V[1]->datum, V[0]->datum ) ;

	if( ! str )
		str="";

	return aAsciiString_atomize( str_dup( str ) );
	}


string patternMatch( string str , string mstr )
	{
	extern unsigned int	(*str_len)(string) ;
	static char		TmpStr[100];
	string			ptr;
	int			max;
	int			cnt;

	#ifdef NO_STRNCMP
	string			t ;
	string			m ;
	int			x ;
	#endif

	ptr = str;
	while( *ptr != 0 )
		{
		max = 0;
		cnt = 0;
		if( *ptr == '|' )
			{
			ptr++;
			continue;
			}
		if( *ptr == '*' )
			{
			ptr++;
			while( mstr[cnt] && *ptr != mstr[cnt] )
				{
				TmpStr[cnt] = mstr[cnt];
				cnt++;
				}
			}
		while( *ptr != 0 && *ptr != '|')
			{
			if( ! isprint(*ptr) )
				{
				ptr++;
				continue;
				}
			if( *ptr == '*' )
				{
				max = cnt;
				ptr++;
				continue;
				}
			TmpStr[cnt] = *ptr;
			cnt++;
			ptr++;
			}
		TmpStr[cnt] = 0;
		if( max == 0 )
			{
			if( cmp_s(TmpStr,mstr) == 0 )
				return TmpStr;
			continue;
			}
		/* extra speed up test */
		if( *TmpStr == *mstr )
			{
#			ifdef NO_STRNCMP		// not functional
			t = TmpStr ;
			m = mstr ;
			x = 0 ;
			while( x < max && *t && *m && *t == *m )
				t++ , m++ , x-- ;
			if( x == cnt )
				return mstr ;
			while( *t && *m && *t == *m )
				t++ , m++ ;
			if( *t == 0 && *m == 0 )
				return TmpStr ;

#			else
			if( strncmp( TmpStr , mstr , max ) == 0 )
				{
				if( max == cnt )
					return mstr ;
				else
					if( strncmp( &TmpStr[max], &mstr[max], str_len(&mstr[max]) ) == 0 )
					return TmpStr ;
				}
#			endif
			}
		}

	return (string) 0;
	}


/***************************************
*
* UPDATE:
* Function patternMatch() should use _>strncmp();
*
*******************/
