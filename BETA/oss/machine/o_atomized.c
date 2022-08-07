/***************************************
*
* o_atomized.c
*
*******************/


#include "ATOMATRIX.h"
#include "varray.h"


atom o_atomized( atom A , unsigned T , atom* V )
	{
	extern unsigned long	(*atomized)(void) ;
	extern unsigned long	(*atomogened)(void) ;
	extern long		(*reatomized)(void) ;
	extern atom		(*aVarray_atomize_a_n)(atom,int) ;
	extern atom		(*oInteger)(int) ;
	extern void*		(*allocm)(unsigned int) ;
	atom*			v ;
	atom			a ;
	unsigned		x = atomized() ;
	unsigned		y = atomogened() ;
	unsigned		z = reatomized() ;

	a		= aVarray_atomize_a_n( 0 , 3 ) ;
	v		= ((varray)a->datum)->ary ;
//	*v		= allocm( sizeof( struct _var ) ) ;
	*(v++)		= oInteger( x ) ;
//	*v		= allocm( sizeof( struct _var ) ) ;
	*(v++)		= oInteger( y ) ;
//	*v		= allocm( sizeof( struct _var ) ) ;
	*v		= oInteger( z ) ;
	return a ;
	}
