/********************************************
*
* aVReality.c
*
*************************/


#include "ATOMATRIX.h"


atom			aVReality ;


void aVReality_preconstructent( atom A )
	{
	extern atom		atomine ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*oFunction)(atom,string,void*) ;
	extern atom		(*oAtomical)(atom,string,atom) ;

	oAtomical( A , "aVReality" , aVReality = atomize( atomine ) ) ;
	}


void aVReality_constructent( atom A )
	{
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern atom		(*atomethodize)(atom,string,atom(*)(atom,int,atom*));

#	define __(_1)							\
		{							\
		extern atom		aVReality_##_1(atom,int,atom*) ;	\
		atomethodize( A , #_1 , aVReality_##_1 ) ;		\
		}
	__( format ) ;
	__( match ) ;
	__( edit ) ;
	__( repository ) ;
	__( canonicalize ) ;
	__( decanonicalize ) ;
	}
