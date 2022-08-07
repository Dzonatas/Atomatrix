/***************************************
*
* destruct.c
*
*******************/


#include "ATOMATRIX.h"

#ifdef DEBUG
#include "varray.h"
#endif

void destruct( atom A )
	{
	typedef	void		(*f)(atom) ;
	extern atom		atomogen ;
	extern atom		aDestructor ;
	extern unsigned long	atomogened ;
	extern void		intratomize(atom,atom) ;
	extern void		interatomize(atom,atom) ;
	extern long		reatomized;
	atom 			a ;
	atom 			aa ;

#ifdef DEBUG
	{
	extern atom atomc, atomp ;
	if( A == atomc && ( ( ! atomp ) || A->precursor == atomp ) )
		_asm nop ;
	}
#endif
	if( ( aa = A )->precursor == atomogen )
		return ;
	while( aa )
		{
		a = aa->interatom ;
		while( a )
			{
			if( a->precursor == aDestructor )
				{
				((f)a->datum)( A ) ;
				break ;
				}
			a = a->interatoms ;
			}
		aa = aa->precursor ;
		}
	interatomize( A , atominul ) ;
	intratomize(  A, atomogen ) ;
	atomogened++ ;
	reatomized-- ;
	}



atom oDestructor( atom A , void* B )
	{
	extern atom		aDestructor ;
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;

	a		= atomize( aDestructor ) ;
	a->datum	= B ;
	interatomize( a , A ) ;
	return a ;
	}


/****************************************
*
* UPDATE:
* - ATOMATRIX.h file should not be included yet "atom.h" should be.
*
****************************/