/***************************************
*
* aAtomatrix_output.c
*
*******************/


#include	"ATOMATRIX.h"
#include <windows.h>


atom aAtomatrix_output( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aUndefined ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern void		console_output(string) ;
#	ifdef _DEBUG
	extern atom		aInteger ;
#	endif

	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
#	ifdef _DEBUG
	if( V[0]->precursor == aInteger )
		{
		Sleep( 1000 ) ;
		_asm int 3 ;
		return atominul ;
		}
#	endif
	if( V[0]->precursor != aAsciiString )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	console_output( V[0]->datum );
	aUndefined->allusions++ ;
	return aUndefined ;
	}
