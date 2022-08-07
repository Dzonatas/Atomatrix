/***************************************
*
* SaveStr.c
*
*******************/


#include <stdio.h>


#include "ATOMATRIX.h"


int SaveStr( string S, FILE * F )
	{
	extern void		(*output)(string) ;
	unsigned long		x;
	unsigned long		r;

	r = 1;
	if( S )
		while( r > 0 && *S )
			{
			for( x=0 ; x < BUFSIZ ; x++ )
				if( ! S[x] )
					break;
			r	= fwrite( S, x, 1, F );
			S	= &S[ x ];
			}

	if( r <= 0 )
		{
		output( "Troubled save of core file!!!\n");
		return 0;
		}

	fputc( 3, F );
	fputc( '\n', F );

	return 1;
	}
