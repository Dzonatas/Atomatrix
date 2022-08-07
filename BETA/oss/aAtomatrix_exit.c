/***************************************
*
* aAtomatrix_exit.c
*
*******************/


#include	"ATOMATRIX.h"


// UPDATE: deprecate


atom aAtomatrix_exit( atom A , unsigned T , atom* V )
	{
	extern void	EXIT(int) ;

	EXIT( 0 ) ;
	return 0 ;
	}


/*****************************
*
* UPDATE:
* Attempt to deallocate resources and return to precessant executent.
*
****************/
