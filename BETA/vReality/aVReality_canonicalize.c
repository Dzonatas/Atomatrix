/***************************************
*
* aVReality_canonicalize.c
*
*******************/


#include "ATOMATRIX.h"

#include <stdio.h>


// Char			::=    #x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD] | [#x10000-#x10FFFF]
// S			::=    (#x20 | #x9 | #xD | #xA)+
// NameChar		::=    Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender 
// Name			::=    (Letter | '_' | ':') (NameChar)* 
// Names		::=    Name (S Name)* 
// Nmtoken		::=    (NameChar)+ 
// Nmtokens		::=    Nmtoken (S Nmtoken)* 
// EntityValue		::=    '"' ([^%&"] | PEReference | Reference)* '"'  
//			|      "'" ([^%&'] | PEReference | Reference)* "'" 
// AttValue		::=    '"' ([^<&"] | Reference)* '"'  
//			|      "'" ([^<&'] | Reference)* "'" 
// SystemLiteral	::=    ('"' [^"]* '"') | ("'" [^']* "'")  
// PubidLiteral		::=    '"' PubidChar* '"' | "'" (PubidChar - "'")* "'" 
// PubidChar		::=    #x20 | #xD | #xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%] 
// CharData		::=    [^<&]* - ([^<&]* ']]>' [^<&]*)
// Comment		::=    '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->' 
// PI			::=    '<?' PITarget (S (Char* - (Char* '?>' Char*)))? '?>' 
// PITarget		::=    Name - (('X' | 'x') ('M' | 'm') ('L' | 'l'))
// CDSect		::=    CDStart CData CDEnd 
// CDStart		::=    '<![CDATA[' 
// CData		::=    (Char* - (Char* ']]>' Char*))  
// CDEnd		::=    ']]>' 
// prolog		::=    XMLDecl? Misc* (doctypedecl Misc*)? 
// XMLDecl		::=    '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>' 
// VersionInfo		::=    S 'version' Eq ("'" VersionNum "'" | '"' VersionNum '"')/* */ 
// Eq			::=    S? '=' S? 
// VersionNum		::=    ([a-zA-Z0-9_.:] | '-')+ 
// Misc			::=    Comment | PI | S 
// doctypedecl		::=    '<!DOCTYPE' S Name (S ExternalID)? S? ('[' (markupdecl | DeclSep)* ']' S?)? '>'
// DeclSep		::=    PEReference | S
// markupdecl		::=    elementdecl | AttlistDecl | EntityDecl | NotationDecl | PI | Comment  
// extSubset		::=    TextDecl? extSubsetDecl 
// extSubsetDecl	::=    ( markupdecl | conditionalSect | DeclSep)*
// SDDecl		::=    S 'standalone' Eq (("'" ('yes' | 'no') "'") | ('"' ('yes' | 'no') '"'))
// element		::=    EmptyElemTag 
//			| STag content ETag
// STag			::=    '<' Name (S Attribute)* S? '>'
// Attribute		::=    Name Eq AttValue [VC: Attribute Value Type]
// ETag			::=    '</' Name S? '>' 
// content		::=    CharData? ((element | Reference | CDSect | PI | Comment) CharData?)* 
// EmptyElemTag		::=    '<' Name (S Attribute)* S? '/>' 
// elementdecl		::=    '<!ELEMENT' S Name S contentspec S? '>' [VC: Unique Element Type Declaration] 
// contentspec		::=    'EMPTY' | 'ANY' | Mixed | intra








atom aVReality_canonicalize( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aError_INVARG ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	string			s ;
	string			p ;
	string			a ;
	unsigned		c ;
	unsigned		z ;
	char			temp[50];
	unsigned		i ;
	int			q ;
	

	if( T != 1 )
		{
		*( s = (string)allocm( 1 ) ) = 0 ;
		return aAsciiString_atomize( s ) ;
		}
	if( V[0]->precursor != aAsciiString )
		{
		aError_INVARG->allusions++;
		return aError_INVARG ;
		}

	c	= 0 ;
	a	= V[0]->datum ;
	q	= 0 ;
	while( *a++ )
		{
		c++ ;
		}
	if( c == 0 )
		{
		V[0]->allusions++ ;
		return V[0] ;
		}
	p = s	= allocm( c * 5 ) ;			// UPDATE: temporary implementation
	a	= V[0]->datum ;
	z	= 0 ;
	while( *a )
		{
		if( ( *a < ' ' && *a != '\n' ) || *a == '&' || *a == 127 || *a == '"' || *a == '<' || *a == '>' )
			{
			//sprintf(temp, "%ld", (unsigned)*a);
			sprintf_s(temp, 50, "%ld", (unsigned)*a);
			i	= 0 ;
			*(s++)	= '&' ;
			*(s++)	= '#' ;
			while( temp[i] )
				{
				*(s++) = temp[i++] ;
				z++ ;
				}
			*(s++)	= ';' ;
			a++ ;
			z	+= 3 ;
			continue ;
			}
		*(s++)	= *(a++) ;
		z++ ;
		}
	*(s++)	= 0 ;
	return aAsciiString_atomize( allocr( p , ++z ) ) ;
	}


atom aVReality_decanonicalize( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aError_INVARG ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	string			s ;
	string			p ;
	string			a ;
	unsigned		c ;
	unsigned		z ;
	unsigned		i ;
	int			q ;
	unsigned		v ;
	//char temp[50];
	

	if( T != 1 )
		{
		*( s = (string)allocm( 1 ) ) = 0 ;
		return aAsciiString_atomize( s ) ;
		}
	if( V[0]->precursor != aAsciiString )
		{
		aError_INVARG->allusions++;
		return aError_INVARG ;
		}

	c	= 0 ;
	a	= V[0]->datum ;
	q	= 0 ;
	if( *a == '&' )
		q = 1 ;
	while( *a++ )
		{
		c++ ;
		if( *a == '&' )
			q = 1 ;
		}
	if( ! q )
		{
		V[0]->allusions++ ;
		return V[0] ;
		}
	s = p	= allocm( c * 2 ) ;
	a	= V[0]->datum ;
	z	= 0 ;
	while( *a )
		{
		if( *a == '&' )
			{
			if( *(++a) != '#' )
				_asm int 3 ;
			++a ;
			//i = sscanf( a , "%ld;" , &v ) ;
			i = sprintf_s(a, (size_t)50, "%u;", v);
			if( i != 1 )
				_asm int 3 ;
			if( v > 127 )
				_asm int 3 ;
			*(s++)	= (char)v ;
			z++ ;
			while( *(a++) != ';' ) ;
			continue ;
			}
		*(s++)	= *(a++) ;
		z++ ;
		}
	*(s++)	= 0 ;
	return aAsciiString_atomize( allocr( p , ++z ) ) ;
	}