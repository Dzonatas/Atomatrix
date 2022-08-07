/***************************************
*
* sgnl.c
*
*******************/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "ATOMATRIX.h"

void atomSignal_gro( void )
	{
	extern atom		atomine ;
	extern atom		atomize(atom);

	return ;
	}

#if 0
// -------------------------------------------------

void sgnl01(int A)	{ printf("OSS: sgnl01\n"); EXIT(0); }
void sgnl02(int A)	{ printf("OSS: sgnl02\n"); EXIT(0); }
void sgnl03(int A)	{ printf("OSS: sgnl03\n"); EXIT(0); }
void sgnl04(int A)	{ printf("OSS: sgnl04\n"); abort(); }
void sgnl05(int A)	{ printf("OSS: sgnl05\n"); /* abort(); */ }
#if defined(SIGIOT) && defined(SIGABRT)
void sgnl06(int A)	{ printf("OSS: sgnl06\n");
		#ifdef SIGIOT
		signal(SIGIOT, (sig_t) SIG_DFL);
		#else
		signal(SIGABRT, (int)SIG_DFL);
		#endif
		abort();
		}
#else
void sgnl06(int A)   { printf("OSS: sgnl06\n"); abort(); }
#endif
void sgnl07(int A)	{ printf("OSS: sgnl07\n"); abort(); }
void sgnl08(int A)	{ printf("OSS: sgnl08\n"); abort(); }
void sgnl09(int A)	{ printf("OSS: sgnl09\n"); abort(); }
void sgnl10(int A)	{ printf("OSS: sgnl10\n"); abort(); }
void sgnl11(int A)	{ printf("OSS: sgnl11\n"); abort(); }
void sgnl12(int A)	{ printf("OSS: sgnl12\n"); abort(); }
void sgnl13(int A)	{ printf("OSS: sgnl13\n"); abort(); }
void sgnl14(int A)	{ printf("OSS: sgnl14\n"); abort(); }
void sgnl15(int A)	{ printf("OSS: sgnl15\n"); abort(); }
void sgnl16(int A)	{ printf("OSS: sgnl16\n"); abort(); }
#if SIGCHLD == 17
void sgnl17(int A)	{ return; }
#else
void sgnl17(int A)	{ printf("OSS: sgnl17\n"); abort(); }
#endif
void sgnl18(int A)	{ printf("OSS: sgnl18\n"); abort(); }
void sgnl19(int A)	{ printf("OSS: sgnl19\n"); abort(); }
#if SIGCHLD == 20
void sgnl20(int A)	{ return; }
#else
void sgnl20(int A)	{ printf("OSS: sgnl20\n"); }
#endif

void sgnl_init(void) 
{
signal(  1,	sgnl01);
signal(  2,	sgnl02);
signal(  3,	sgnl03);
signal(  4,	sgnl04);
signal(  5,	sgnl05);
signal(  6,	sgnl06);
signal(  7,	sgnl07);		
signal(  8,	sgnl08);
signal(  9,	sgnl09);
signal( 10,	sgnl10);
signal( 11,	sgnl11);
signal( 12,	sgnl12);	
signal( 13,	sgnl13);
signal( 14,	sgnl14);
signal( 15,	sgnl15);
signal( 16,	sgnl16);
signal( 17,	sgnl17);
signal( 18,	sgnl18);
signal( 19,	sgnl19);
signal( 20,	sgnl20);
}


#endif


/***************************************
*
* UPDATE:
* This module should be redesigned.
*
*******************/
