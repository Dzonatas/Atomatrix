/***************************************
*
* p_evaluate.c
*
*******************/


#include	"ATOMATRIX.h"


atom p_evaluate( atom A , unsigned T , atom* V )
	{
	extern atom		aProgram ;
	extern atom		aAsciiString ;
	extern atom		aError_INVARG ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_COMP ;
	extern atom		(*evaluate)(string,atom) ;

	if( A != aProgram )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	if( T != 1 )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( V[0]->precursor != aAsciiString )			// UPDATE: unicode
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	return evaluate( V[0]->datum , 0 ) ;
	}