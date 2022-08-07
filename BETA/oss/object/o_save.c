/***************************************
*
* o_save.c
*
*******************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "ATOMATRIX.h"
#include "object.h"
#include "objvar.h"
#include "program.h"

#ifdef MICROSOFT_win32
#include <windows.h>
#endif

static unsigned _( atom A )
	{
	extern atom		aModuleSerinteger ;
	extern atom		aModuleSerialed ;

	if( ! A )
		return 0 ;
	if( A->precursor == aModuleSerialed ) 
		return ((object)A->datum)->serinteger ;
	if( A->precursor == aModuleSerinteger ) 
		return (unsigned) A->datum ;
	return 0 ;
	}



atom o_save( atom A , unsigned T , atom* V )
	{
	extern atom		aModuleAlpha ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerialed ;
	extern atom		aProgram ;
	extern atom		aObjVar ;
	extern atom		aError ;
	extern void		(*output)(string) ;
	extern int		SaveVar(atom,FILE*);
	extern atom		aModuleXML_alphaSave(atom) ;
	FILE*			fp;
	char			fn[256];
	long			tmp;
	long			err;
	program			pptr;
	objvar			ov;
	object			o;
	atom			a;
	atom			aa;
	extern int		SaveStr(string,FILE*);
	long			nvars			= 0;
#	ifdef MICROSOFT_win32
	char                    b[256];
#	endif


	if( A->precursor == aModuleXML )
		return atominul ;
	if( A->precursor == aModuleAlpha )
		return aModuleXML_alphaSave( A ) ;
#ifdef _DEBUG
	return atominul ;
#endif
	if( A->precursor != aModuleSerialed )
		{
		aError->allusions++;
		return aError;
		}
	o = A->datum;

#	ifdef MICROSOFT_win32
	GetEnvironmentVariable( "_ATOMATRIX", b, sizeof( b ) );
	sprintf( fn, "%sdb\\t\\%ld", b, (long) o->serinteger );
#	else
	sprintf( fn, "%s%ld", getenv("_ATOMATRIX"), (long) o->serinteger );
#	endif

	fp = fopen( fn, "wb" ); // UPDATE doublecheck 'b'

	if( ! fp )
		{
		printf( "save failed = %s.\n", fn );
		fclose( fp );
		aError->allusions++;
		return aError;
		}

	tmp = (unsigned) o->perms;

	a = A->interatom;
	while( a )
		{			/* FIXME: not needed, use EOF checksum */
		if( a->precursor == aObjVar )
			nvars++;
		a = a->interatoms;
		}

	err = fprintf( fp, "o%ld p%ld c%ld s%ld l%ld c%ld w%ld p%lu v%ld\n",
		_(o->principal) , _(o->precursor) , _(o->intraobject) , _(o->intraobjects) ,
		_(o->orbit) , _(o->interobject) , _(o->interobjects) ,
		tmp , nvars ) ;
	if( err < 0 )
		{
		output( "Obj-def save failed.\n" );
		fclose( fp );
		aError->allusions++;
		return aError;
		}

	aa = atominul;
	a = A->interatom;
	while( a && aa != A->interatom )
		{
		while( a->interatoms != aa )
			a = a->interatoms;
		aa = a;

		if( a->precursor != aObjVar )
			{
			a = A->interatom;
			continue;
			}
		ov = (objvar) a->datum;
		fprintf( fp, "%s	(%ld,%lu)	", ov->name, _( ov->principal ),
			(unsigned long) ov->perms );
		if( err < 0 )
			{ 
			output( "Core save failed.\n" ) ;
			fclose( fp ); 
			aError->allusions++;
			return aError;
			}
		if( ! SaveVar(ov->v,fp) )
			{
			fclose( fp );
			output( "Core save failed.\n" ) ;
			aError->allusions++;
			return aError;
			}
		a = A->interatom;
		}

	fprintf( fp, ".\n" );

	aa = atominul;
	a = A->interatom;
	while( a && aa != A->interatom )
		{
		while( a->interatoms != aa )
			a = a->interatoms;
		aa = a;
		if( a->precursor != aProgram )
			{
			a = A->interatom;
			continue;
			}
		pptr = (program) a->datum;
		fprintf( fp, "%s (0)\n", pptr->name );
		if( ! SaveStr( pptr->psrc, fp ) )
			{
			output( "Core save failed.\n" ) ;
			fclose( fp );
			aError->allusions++;
			return aError;
			}
		a = A->interatom;
		}
	fclose( fp );

	return atominul;
	}
