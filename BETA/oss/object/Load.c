/***************************************
*
* Load.c		// UPDATE: loadModule.c
*
*******************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "ATOMATRIX.h"
#include "object.h"
#include "objvar.h"

#ifdef MICROSOFT_win32
#include <windows.h>
#endif


atom loadModule( atom M )
	{
	extern atom		aModuleSerialed ;
	extern atom		aError_INVOBJ ;
	extern string		(*str_dup)(string) ;
	extern int		(*str_len)(string) ;
	extern void*		(*allocr)(void*,unsigned int) ;
	extern atom		(*atomize)(atom) ;
	extern void*		(*allocm)(unsigned int) ;
	extern void		(*intratomize)(atom,atom) ;
	extern atom		addObjVar(atom,string,atom*);
	extern atom		addProgram(atom,string,string);
	extern int		ReadVar(FILE*,atom*);
	extern string		ReadStr(FILE*);
	extern atom		oModule(unsigned) ;
	long			i,x,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9;
	ObjID			oid;
	objvar 			ov;
	atom			a;
	object			o;
	FILE *			fp;
	int			c;
	string			s ;
	string			p ;
	int			y ;
	int			z ;

	oid = y	= (unsigned) M->datum ;
	if( y == 0 )
		_asm int 3 ;
	z	= 0 ;
	while( y /= 10 )
		z++ ;

#	ifdef MICROSOFT_win32
	x	= GetEnvironmentVariable( "_ATOMATRIX" , 0 , 0 ) ;
	s	= HeapAlloc( GetProcessHeap() , 0 , x + z + 7 ) ;
	GetEnvironmentVariable( "_ATOMATRIX" , s , x ) ;

#	else
	s	= str_dup( getenv( "_ATOMATRIX" ) ) ;
	x	= str_len( s ) ;
	s	= allocr( s , x + z + 7 ) ;

#	endif
	y	= oid ;
	p	= s + x - 1 ;

	c	= 0 ;
	while( *p = *("db\\t\\"+c++) ) 
		p++ ;
	*(1+(p+=z)) = 0 ;
	while( z-- )
		{
		*(p--) = '0' + y % 10 ;
		y /= 10 ;
		} ;
	*p	= '0' + y ;
	if( 0 == ( fp = fopen( s , "rb" ) ) )
		{
		( a = aError_INVOBJ )->allusions++ ;
		return a ;
		}

/*
	{
		//---------------------------------------------------
		char buf[1048576];
		long int pos = ftell(fp);
		long g = 0;
		while ((c = getc(fp)) != EOF)
			{
			buf[g++] = c;
			if(g>= 1048576)
				_asm int 3
			}
		buf[g] = 0;
		fseek(fp, pos, SEEK_SET);
		//---------------------------------------------------
	}
*/


	fscanf(fp,"o%ld p%ld c%ld s%ld l%ld c%ld w%ld p%ld v%ld\n",
		&arg1,&arg2,&arg3,&arg4,&arg5,&arg6,&arg7,&arg8,&arg9);

//	a		= atomize( aModule );
//	a->datum	= allocm( sizeof(struct _object) );

	o		= allocm( sizeof(struct _object) ) ;
	o->serinteger	= oid;
	o->principal	= oModule( arg1 ) ;	// UPDATE: ? arg - 0 || arg == 0
	o->precursor	= oModule( arg2 ) ;
	o->intraobject	= oModule( arg3 ) ;
	o->intraobjects	= oModule( arg4 ) ;
	o->orbit	= oModule( arg5 ) ;
	o->interobject	= oModule( arg6 ) ;
	o->interobjects		= oModule( arg7 ) ;
	o->perms	= (unsigned short) arg8;

	for( i = 0 ; i < arg9 ; i++ )
		{
		fscanf( fp, "%s	(%ld,%ld)	", s , &arg1, &arg2 ) ;
		ov		= (objvar) addObjVar( M , s , 0 )->datum ;
		ov->principal	= oModule( (ObjID) arg1 ) ;
		ov->perms	= (unsigned short) arg2 ;
		if( ! ReadVar( fp, &ov->v ) )
			{
			fclose(fp) ;
#			ifdef MICROSOFT_win32
			HeapFree( GetProcessHeap() , 0 , s ) ;
#			endif
			( a = aError_INVOBJ )->allusions++ ;
			return a ;
			}
		}

	if( getc( fp ) != '.' ) 
		{
		/* dealloc above stuff */
		fclose(fp);
#		ifdef MICROSOFT_win32
		HeapFree( GetProcessHeap() , 0 , s ) ;
#		endif
		( a = aError_INVOBJ )->allusions++ ;
		return a ;
		} 
	getc( fp );

	/*
	{
		//---------------------------------------------------
		char buf[1048576];
		long int pos = ftell(fp);
		long g = 0;
		while ((c = getc(fp)) != EOF)
			{
			buf[g++] = c;
			if (g >= 1048576)
				_asm int 3
			}
		buf[g] = 0;
		fseek(fp, pos, SEEK_SET);
		//---------------------------------------------------
	}
	*/

	while( ( c = getc(fp) ) != EOF)
		{
		ungetc( c, fp );
		/* FIXME: next line scan arg.. should be "%s()\n" */
		x = fscanf( fp , "%s (%ld)\n", s , &arg1 );
		if( x != 2 )
			{
			/* dealloc above stuff */
			fclose( fp );
#			ifdef MICROSOFT_win32
			HeapFree( GetProcessHeap() , 0 , s ) ;
#			endif
			( a = aError_INVOBJ )->allusions++ ;
			return a ;
			} 
		addProgram( M , s , ReadStr( fp ) ) ;
		}

	fclose( fp );

#	ifdef MICROSOFT_win32
	HeapFree( GetProcessHeap() , 0 , s ) ;
#	endif

	intratomize( M , aModuleSerialed ) ;
	M->datum = o ;
	M->allusions++ ;		// UPDATE: this stops the possible destruct
	return atominul ;
	}


/***************************************
*
* UPDATE:
* Function loadModule() should deallocate and return illogicals where atominul is
* returned.
*
*******************/
