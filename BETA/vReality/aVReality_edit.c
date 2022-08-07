/***************************************
*
* aVReality_edit.c
*
*******************/


#include "ATOMATRIX.h"
#include "unicode.h"
#include <windows.h>
#include <stdio.h>

unsigned	editSequence ;
struct	_arg
	{
	string		s ;
	} ;

struct	_arg_unicode
	{
	unicodes	s ;
	} ;

void* aVReality_edit_ascii_task( struct _arg* A  )
	{
	extern int		(*str_len)(string) ;
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	string			s ;
	void *			p ;
	DWORD			x ;
	DWORD			z ;
	DWORD			e ;
	int			y ;
	HANDLE			f ;
	char path[250] ;
	char cmd[250] ;

	sprintf( path , "D:\\WINDOWS\\Temp\\ATOMATRIX %u.txt" , editSequence ) ;		// UPDATE: no drive
	sprintf( cmd , "notepad D:\\WINDOWS\\Temp\\ATOMATRIX %u.txt" , editSequence++ ) ;


	z	= 4096 ;
	f	= CreateFile( path , GENERIC_WRITE , 0 , 0 , CREATE_ALWAYS , 0 , 0 ) ;
	if( ! f )
		return atominul ;
	WriteFile( f , A->s  , str_len( A->s ) , &x , 0 ) ;
	CloseHandle( f ) ;
	system(  cmd ) ;
	f	= CreateFile( path , GENERIC_READ , 0 , 0 , OPEN_EXISTING , 0 , 0 ) ;
	s	= (string)allocm( z ) ;
	p	= s ;
	y	= 0 ;
	do	{
		if( ReadFile( f , p , z , &x , 0 ) )
			{
			if( x == 0 )
				break ;
			y	+= x ;
			s	= (string)allocr( s , y + z ) ;
			p	= s + y ;
			continue ;
			}
		if( ERROR_INSUFFICIENT_BUFFER == ( e = GetLastError() ) )
			{
			z *= 2 ;		// UPDATE: validity of max z
			s	= (string)allocr( s , y + z ) ;
			p	= s + y ;
			continue ;
			}
		else
			break ;
		} while( 1 ) ;
	*(char*)p = 0 ;
	y++ ;
	CloseHandle( f ) ;
	DeleteFile( path ) ;
	return allocr( s , y ) ;
	}


void* aVReality_edit_unicode_task( struct _arg_unicode* A  )
	{
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	unicodes		s ;
	void *			p ;
	DWORD			x ;
	DWORD			z ;
	DWORD			e ;
	int			y ;
	HANDLE			f ;
	unsigned		l ;
	unicode			unicodedPrefix = 0xFFFE ;
	char path[250] ;
	char cmd[250] ;

	sprintf( path , "D:\\WINDOWS\\Temp\\ATOMATRIX %u.txt" , editSequence ) ;		// UPDATE: no drive
	sprintf( cmd , "notepad D:\\WINDOWS\\Temp\\ATOMATRIX %u.txt" , editSequence++ ) ;


	z	= 4096 ;
	f	= CreateFile( path , GENERIC_WRITE , 0 , 0 , CREATE_ALWAYS , 0 , 0 ) ;
	if( ! f )
		return atominul ;
	s	= A->s ;
	l	= 0 ;
	while( *s )
		l++ , s++ ;
	WriteFile( f , &unicodedPrefix  , 2 , &x , 0 ) ;
	WriteFile( f , A->s  , l*2 , &x , 0 ) ;
	CloseHandle( f ) ;
	system(  cmd ) ;
	f	= CreateFile( path , GENERIC_READ , 0 , 0 , OPEN_EXISTING , 0 , 0 ) ;
	s	= (void*)allocm( z ) ;
	p	= s ;
	y	= 0 ;
	ReadFile( f , &unicodedPrefix , 2 , &x , 0 ) ;
	if( unicodedPrefix != 0xFFFE )
		_asm int 3 ;						// UPDATE: test
	do	{
		if( ReadFile( f , p , z , &x , 0 ) )
			{
			if( x == 0 )
				break ;
			y	+= x ;
			s	= (void*)allocr( s , y + z ) ;
			p	= s + y ;
			continue ;
			}
		if( ERROR_INSUFFICIENT_BUFFER == ( e = GetLastError() ) )
			{
			z *= 2 ;		// UPDATE: validity of max z
			s	= (void*)allocr( s , y + z ) ;
			p	= s + y ;
			continue ;
			}
		else
			break ;
		} while( 1 ) ;
	*(char*)p = 0 ;
	y++ ;
	CloseHandle( f ) ;
	DeleteFile( path ) ;
	return allocr( s , y ) ;
	}


atom aVReality_edit( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern atom		(*aUnicodedString_atomize)(string) ;
	extern void*		(*taskSplice)(void*,void*) ;

	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( V[0]->precursor == aAsciiString )
		{
		struct _arg		a ;
		a.s	= (string)V[0]->datum ;
		return	aAsciiString_atomize( taskSplice( aVReality_edit_ascii_task , &a ) ) ;
		}
	if( V[0]->precursor == aAsciiCharacter )
		{
		struct _arg		a ;
		char			b[2] ;
		b[0]	= (byte)V[0]->datum ;
		b[1]	= 0 ;
		a.s	= b ;
		return	aAsciiString_atomize( taskSplice( aVReality_edit_ascii_task , &a ) ) ;	// UPDATE: return aAsciiCharacter ?
		}
	if( V[0]->precursor == aUnicodedString )
		{
		struct _arg_unicode	a ;
		a.s	= (unicodes)V[0]->datum ;
		return	aUnicodedString_atomize( taskSplice( aVReality_edit_unicode_task , &a ) ) ;
		}
	if( V[0]->precursor == aUnicodedEntity )
		{
		struct _arg_unicode	a ;
		unicode			b[2] ;
		b[0]	= (unicode)V[0]->datum ;
		b[1]	= 0 ;
		a.s	= b ;
		return	aUnicodedString_atomize( taskSplice( aVReality_edit_unicode_task , &a ) ) ;
		}
	aError_INVARG->allusions++;
	return aError_INVARG;
	}
