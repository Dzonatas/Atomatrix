/***************************************
*
* aAtomatrix_intput.c
*
*******************/


#include	"ATOMATRIX.h"

void* aAtomatrix_input__task( void * D )
	{
	extern string		console_input() ;

	return console_input() ;
	}


atom aAtomatrix_input( atom A , unsigned T , atom* V )
	{
	extern atom		aAsciiString_atomize(string) ;
	extern atom		(*oError)(string) ;
	extern void*		taskSplice(void*,void*) ;
	extern string		string_duplicate(string) ;
	string			s ;

	if( ( s = taskSplice( aAtomatrix_input__task , 0 ) ) )
		return aAsciiString_atomize( s ) ;
	return oError( string_duplicate( "read error" ) ) ;
	}
