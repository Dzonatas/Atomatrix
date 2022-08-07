/***************************************
*
* SaveVar.c
*
*******************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#include "ATOMATRIX.h"
#include "unicode.h"
#include "varray.h"
#include "object.h"

int SaveVar( atom V, FILE *F )
	{
	extern atom		aModuleSerialed ;
	extern atom		aModuleSerinteger ;
	extern atom		aUndefined ;
	extern atom		aInteger ;
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aVarray ;
	extern atom		aChronogram ;
	extern int		SaveStr(string,FILE*);
	extern string		(*aUnicodedString_canonicalize)(unicodes) ;
	extern void		(*dealloc)(void*) ;
	unsigned int            i;
	int			err;
	varray			v ;

	if( V == aUndefined )
		{
		err = fprintf( F, "?\n" );
		}
	else
	if( V->precursor == aInteger )
		{
		err = fprintf( F, "# %p\n", V->datum );
		}
	else
	if( V->precursor == aAsciiString )
		{
		err = fprintf( F, "$ " );
		SaveStr( V->datum , F );
		}
	else
	if( V->precursor == aUnicodedString )
		{
		string		s ;
		s	= aUnicodedString_canonicalize( (unicodes) V->datum ) ;
		err = fprintf( F, "U " ) ;
		SaveStr( s , F ) ;
		dealloc( s ) ;
		}
	else
	if( V->precursor == aAsciiCharacter )
		{
		err = fprintf( F, "C %02x\n" , (byte)V->datum );
		}
	else
	if( V->precursor == aUnicodedEntity )
		{
		err = fprintf( F, "E %04x\n" , (unicode)V->datum );
		}
	else
	if( V->precursor == aVarray )
		{
		v = V->datum ;
		err = fprintf( F, "A %ld\n", v->nelem );
		if( err < 0 )
			return 0 ;
		for( i = 0 ; i < v->nelem ; i++ )
			if( ! SaveVar( v->ary[i] , F ) )
				return 0 ;
		}
	else
	if( V->precursor == aModuleSerialed )
		{
		err = fprintf( F, "M %ld\n", ((object)V->datum)->serinteger );
		}
	else
	if( V->precursor == aModuleSerinteger )
		{
		err = fprintf( F, "M %ld\n", (unsigned)V->datum );
		}
	else
	if( V->precursor == aChronogram )
		{
		err = fprintf( F, "T " );
		SaveStr( V->datum , F );
		}
	else	{
		err = fprintf( F, "?\n" );
		}
	if( err < 0 )
		return 0;
	return 1 ;
	}
