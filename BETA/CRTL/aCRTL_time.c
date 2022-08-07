/***************************************
*
* aCRTL_time.c
*
*******************/


#include	"ATOMATRIX.h"

#include <time.h>


atom aCRTL_time( atom A , unsigned T , atom* V )
	{
	extern atom		(*oInteger)(int) ;
	time_t			clocked;

	time( &clocked ) ;
	return oInteger( clocked );
	}
