/***************************************
*
* aCRTL_time.c
*
*******************/


#include	"ATOMATRIX.h"
#include	<time.h>

atom aCRTL_time( atom A , unsigned T , atom* V )
	{
	time_t			timed = 0 ;
	//clock_t			clocked = 0 ;
	extern atom		(*oInteger)(int) ;

	time((time_t *) &timed);     // Update TARGET (TESTING)
	return oInteger( (int) timed );
	}
