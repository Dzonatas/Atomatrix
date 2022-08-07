/***************************************
*
* ReadVar.c
*
*******************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "ATOMATRIX.h"
#include "unicode.h"
#include "varray.h"

int ReadVar( FILE* F , atom* V )
	{
	extern atom		aUndefined ;
	extern atom		aChronogram ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*aChronogram_atomize)(string) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern atom		(*aAsciiCharacter_atomize)(byte) ;
	extern atom		(*aUnicodedEntity_atomize)(uncode) ;
	extern atom		(*aUnicodedString_atomize)(uncode) ;
	extern unicodes		(*aUnicodedString_decanonicalize)(string) ;
	extern atom		(*oInteger)(int) ;
	extern atom		(*aVarray_atomize_a_n)(atom,int) ;
	extern void*		(*allocm)(unsigned) ;
	extern void		(*output)(string) ;
	extern void		(*dealloc)(void*) ;
	extern atom		oModule(unsigned) ;
	extern string		ReadStr(FILE*);
	long			arg1 ;
	atom*			v;
	atom			a ;

	switch( getc( F ) )
		{
		case '?':
			*V		= aUndefined ;
			aUndefined->allusions++ ;
			getc( F );
			break;
		case '$':
			getc( F );
			*V		= aAsciiString_atomize( ReadStr( F ) ) ;
			break;
		case 'T':
			getc( F );
			*V		= atomize( aChronogram ) ;
			(*V)->datum	= ReadStr( F ) ;
			break;
		case 'U':
			{
			string		s ;
			unicodes	u ;

			getc( F );
			s	= ReadStr( F ) ;
			u	= aUnicodedString_decanonicalize( s ) ;
			dealloc( s ) ;
			if( ! u )
				{
				output( "ReadVar Error Bad Core!\n" ); 
				return 0;
				}
			*V		= aUnicodedString_atomize( u ) ;
			break;
			}
		case 'C':
			fscanf( F, " %02x\n", &arg1 );
			*V		= aAsciiCharacter_atomize( (byte)arg1 ) ;
			break;
		case 'E':
			fscanf( F, " %04x\n", &arg1 );
			*V		= aUnicodedEntity_atomize( (unicode)arg1 ) ;
			break;
		case '#':
			fscanf( F, " %ld\n", &arg1 );
			*V		= oInteger( arg1 ) ;
			break;
		case 'M':
			fscanf( F, " %ld\n", &arg1 );
			*V		= oModule( arg1 ) ;
			break;
		case 'A':
			fscanf( F, " %ld\n", &arg1 );
			a = aVarray_atomize_a_n( 0 , arg1 );
			*V	= a ;
			if( arg1 == 0 )
				break;
			v	= ((varray)a->datum)->ary ;
			while( arg1-- )
				{
//				*v = allocm( sizeof(struct _var) );
				if( ! ReadVar( F, v ) )
					return 0;		/* 1 */
				v++;
				}
			break;
		default:
			output( "ReadVar Error Bad Core!\n" ); 
			return 0;
		}

	return 1;
	}



/***************************************
*
* UPDATE:
* 1 - Function ReadVar() needs to deallocate when there is read errors.
*
*******************/
