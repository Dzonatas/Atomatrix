/***************************************
* 
* aError_type.c
* 
*******************/


#include "ATOMATRIX.h"


atom aError_type( atom A , unsigned T , atom* V )
	{
	extern string		Error_type(atom) ;
	extern void		(*output)(string) ;

	output( "$error." ) ;
	output( Error_type( A ) ) ;
	output( "\n" ) ;
	return atominul ;
	}
