/********************************************
*
* aCRTL.c
*
*************************/


#include "ATOMATRIX.h"


extern atom		aCRTL ;


void aCRTL_preconstructent( atom A )
	{
	extern atom		atomine ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*oFunction)(atom,string,void*) ;
	extern atom		(*oAtomical)(atom,string,atom) ;

	oAtomical( A , "aCRTL" , aCRTL = atomize( atomine ) ) ;
	}


void aCRTL_constructent( atom A )
	{
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern atom		(*atomethodize)(atom,string,atom(*)(atom,unsigned,atom*));

#	define __(_1)							\
		{							\
		extern atom		aCRTL_##_1(atom,unsigned,atom*) ;	\
		atomethodize( A , #_1 , aCRTL_##_1 ) ;		\
		}
	__( localTime ) ;
	__( time ) ;
	__( random ) ;
	__( sleep ) ;
	}
