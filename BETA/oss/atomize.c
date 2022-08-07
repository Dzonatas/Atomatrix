/***************************************
*
* atomize.c
*
*******************/


#include "ATOMATRIX.h"

#ifdef	DEBUG
atom	atomc	= (void *)0x00a43a20 ;
atom	atomp	= (void *)0x00000000 ;
#endif


atom atomize( atom precursor )
	{
	extern atom		atomogen ;
	extern atom		allocm(unsigned int);
	extern unsigned long	atomized;
	extern long		reatomized;
	atom			a ;

	if( ! precursor )
		return atominul ;
	if( a = atomogen->intratom )
		{
		reatomized++ ;
		// atomogen->intratom = a->intratoms
		a->precursor->intratom	= a->intratoms ;
		}
	else	{
		atomized++ ;
		a			= allocm( sizeof(struct _atom) ) ;
		}
#ifdef DEBUG
	if( a == atomc && ( ( ! atomp ) || precursor == atomp ) )
		_asm nop ;
#endif
	a->precursor		= precursor ;
	a->allusions		= 1 ;
	a->intratom		= atominul ;
	a->intratoms		= precursor->intratom ;
	a->orbit		= atominul ;
	a->interatom		= atominul ;
	a->interatoms		= atominul ;
	a->datum		= atominul ;
	precursor->intratom	= a ;
	return a;
	}


unsigned long _atomized( void )
	{
	extern unsigned long	atomized;

	return atomized ;
	}


unsigned long _atomogened( void )
	{
	extern unsigned long	atomogened ;

	return atomogened ;
	}


long _reatomized( void )
	{
	extern long		reatomized;

	return reatomized ;
	}