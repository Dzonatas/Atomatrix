/***************************************
*
* aCRTL_time.c
*
*******************/


#include	"ATOMATRIX.h"

#include <time.h>


atom aCRTL_time( atom A , unsigned T , atom* V )
	{
	time_t			clocked;
	extern atom		(*oInteger)(int) ;

	time(&clocked);
	return oInteger( (int) clocked );
	}
