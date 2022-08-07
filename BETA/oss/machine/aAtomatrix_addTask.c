/***************************************
*
* aAtomatrix_addTask.c
*
*******************/


#include	"ATOMATRIX.h"
#include	"avei.h"
#include	"sframe.h"
#include	"task.h"


atom aAtomatrix_addTask( atom A , unsigned T , atom* V )
	{
	extern atom		aProgram ;
	extern atom		aError_INVARG ;
	extern atom		aError_ARGC ;
	extern task		(*new_task)(void) ;
	extern atom		(*duplicate)(atom) ;
	task			t ;

	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( V[0]->precursor != aProgram )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	t		= new_task( ) ;
	t->programme	= *V ;
	t->programme->allusions++ ;
	if( T == 2 )
		{  // UPDATE: duplicate vars
		t->args		= duplicate( V[1]->datum ) ;
		}
	return atominul ;
	}