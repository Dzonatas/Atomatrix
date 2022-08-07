/***************************************
*
* tkn.c
*
*******************/


// UPDATE: add operand "mod"  (i.e. remainder "C style"  5%4 = 1 ; 5%5 = 0 )

#include <stdlib.h>

#include "ATOMATRIX.h"
#include "avei.h"
#include "executable.h"
#include "program.h"
#include "method.h"

#include "cmp_s.h"

#include <string.h>

#include <ctype.h>

#ifdef DEBUG
#include <stdio.h>
#include <windows.h>
#endif


#define CONTINUE	256
#define NUMBER		257
#define PULL		258
#define PUSHR		259
#define PUSHL		260
#define POPR		261
#define POPL		262

#if 1
# define LINE_P_ON
					// UPDATE: implement 2 phase compiler with seperate procedure instances
					// 1st phase: interpret, error/debug check, allocation enumeration
					// 2nd phase: allocate, compile
					// * current implementation uses sizeOnly to seperate the phases in the same procedure
#endif



#ifdef LINE_P_ON
#define INST1(A)	if(!sizeOnly) {	Exec->exec[Exec->size++]A;		\
					Exec->linep[Exec->size-1] = tkn_line_p(_) ; }	\
			else		++Exec->size;

#define INST2(A,B)	if(!sizeOnly) { Exec->exec[Exec->size++]A;		\
					Exec->exec[Exec->size++]B;		\
					Exec->linep[Exec->size-2] = tkn_line_p(_) ;		\
					Exec->linep[Exec->size-1] = tkn_line_p(_) ; }	\
			else		Exec->size+=2;

#define INST3(A,B,C)	if(!sizeOnly) {	Exec->exec[Exec->size++]A;		\
					Exec->exec[Exec->size++]B;		\
					Exec->exec[Exec->size++]C;		\
					Exec->linep[Exec->size-3] = tkn_line_p(_) ;		\
					Exec->linep[Exec->size-2] = tkn_line_p(_) ;		\
					Exec->linep[Exec->size-1] = tkn_line_p(_) ; }	\
			else		Exec->size+=3;

#else
#define INST1(A,L)	if(!sizeOnly)	Exec->exec[Exec->size++]A;	\
			else		++Exec->size;
#define INST2(A,B,L)	if(!sizeOnly) { Exec->exec[Exec->size++]A;	\
					Exec->exec[Exec->size++]B; }	\
			else		Exec->size+=2;
#define INST3(A,B,C,L)	if(!sizeOnly) {	Exec->exec[Exec->size++]A;	\
					Exec->exec[Exec->size++]B;	\
					Exec->exec[Exec->size++]C; }	\
			else		Exec->size+=3;

#endif

atom	_tkn;
atom	_tkn_error;
atom	_tkn_number;
atom	_tkn_string;
atom	_tkn_str;
atom	_tkn_text;
atom	_tkn_continue;
atom	_tkn_pull;
atom	_tkn_pushr;
atom	_tkn_pushl;
atom	_tkn_popr;
atom	_tkn_popl;
atom	_tkn_begin;
atom	_tkn_stmt;
atom	_tkn_stmtlist;
atom	_tkn_ref;
atom	_tkn_var;
atom	_tkn_func;
atom	_tkn_assign;
atom	_tkn_lvar;
atom	_tkn_ovar;
atom	_tkn_expr;
atom	_tkn_end;
atom	_tkn_to;
atom	_tkn_x2b3d;
atom	_tkn_x2d3d;
atom	_tkn_x2d3e;
atom	_tkn_x2626;
atom	_tkn_x3636;
atom	_tkn_x3d3d;
atom	_tkn_x3d3e;
atom	_tkn_x3c3d;
atom	_tkn_x3c3e;
atom	_tkn_x407e;
atom	_tkn_x4023;
atom	_tkn_x7c7c;
atom	_tkn_x7e7e;
atom	_tkn_loop;
atom	_tkn_loop1;
atom	_tkn_lvarlist;
atom	_tkn_andor;
atom	_tkn_expr_stmt;
atom	_tkn_array;
atom	_tkn_aryexpr;
atom	_tkn_xxa;
atom	_tkn_xxb;
atom	_tkn_xxc;
atom	_tkn_xxd;
atom	_tkn_xxe;
atom	_tkn_xxf;
atom	_tkn_xxg;
atom	_tkn_brf;
atom	_tkn_pos;
atom	_tkn_ascii2c;
atom	_tkn_bra;
atom	_tkn_endloop;
atom	_tkn_ascii[128];
atom	_tkn_line_p ;
atom	_tkn_code ;
atom	_tkn_stack ;

atom	aToken_sNotInALoop ;
atom	aToken_sCommentNotTerminated ;
atom	aToken_sQuoteNotTerminated ;
atom	aToken_sInvalidCharacter ;
atom	aToken_sUndefinedKeyword ;
atom	aToken_sUndefinedProcedure ;
atom	aToken_sSyntax ;

#define		PSIZ		Exec->size
#define ReCode(a,b)	if(!sizeOnly) Exec->exec[(a)].i=(b)
#define ReCodeInst(a,b)	if(!sizeOnly) Exec->exec[(a)]=(b)

atom	ntkn	= atominul;

//int		errorToggle;
int		sizeOnly;
executable	Exec;
string		bufferp;
string		line_p;
/* union _avei	ti;*/
short		loopBeginCache[20];
short		loopType[20];
short		numloop;
short		last_i;
char		resetparse;
char		linederr;
//void            err_s(string);
string		ptkn(atom);
string          pptkn(atom);
atom             doParse(void);

typedef struct KEYWORD
	{
	char	*name;
	int	type;
	int	code;
	} KEYWORD;

extern KEYWORD keywords[];

#define GetChar()	*bufferp++
#define UnGetChar(c)	bufferp--

long	lineno;

atom *	_t		= 0 ;
int *	_n		= 0 ;



/*
void  dump_a( atom A )
	{
//	if( A && ((executable)A->datum)->prog->name )
//		{
//		if( cmp_s( ((executable)A->datum)->prog->name , "_input" ) == 0 ) _asm int 3 ;
//		output( ((executable)A->datum)->prog->name ) ;
//		output( "\n" ) ;
//		}
//		if( ( sizeof(union _avei) * Exec->size ) >= GRAB_MEM ) _asm int 3 ;
//		if( ( sizeof(string) * Exec->size ) >= GRAB_MEM ) _asm int 3 ;
//		Exec->exec		= allocr( Exec->exec , sizeof(union _avei) * Exec->size );
//		Exec->linep		= allocr( Exec->linep , sizeof(string) * Exec->size );

	extern void		(*destruct)(atom) ;
	FILE*	fmem ;
	char			b[256] ;
	char			fn[256] ;
	unsigned x = 0 ;
	atom		a ;

//	if( A && ((executable)A->datum)->prog->name && cmp_s( ((executable)A->datum)->prog->name , "_input" ) == 0 )
//		output( ((executable)A->datum)->prog->name ) ;
//		output( "\n" ) ;
	GetEnvironmentVariable( "_ATOMATRIX", b, sizeof( b ) );
	sprintf( fn, "%smem.txt", b );

	fmem = fopen( fn, "w" );
	fprintf( fmem , "---- %s\n" , ((executable)A->datum)->prog->name ) ;
	fflush( fmem ) ;
	a	= A->interatom ;
	while( a )
		{
		fprintf( fmem , "%8x %8x %d \n" , a , a->precursor , a->allusions ) ;
		a = a->interatoms ;
		}
	fclose(fmem) ;
//	_asm int 3 ;
	}
		

*/


int parseBuffer( string str, atom A )
	{
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	extern void		(*dealloc)(void*) ;
	extern void		(*interatomize)(atom,atom) ;
	extern void		(*destruct)(atom) ;
#ifdef DEBUG
//	avei			e ;
	extern void (*output)(string) ;
#endif
	atom			a ;

	if( ! str )	
		return 0;


	/*
	bufferp			= str;
	resetparse		= 1;
	linederr		= 0;
	errorToggle		= 0;
	Exec			= A->datum;
	lineno			= 1;
	line_p			= bufferp;
	numloop			= 0;
	last_i			= 0;
	loopBeginCache[0]	= 0;

//
//	e = Exec->exec		= allocm( GRAB_MEM );	// UPDATE: GRAB_MEM is temporary, transition to atomic stack of tokens
//	Exec->linep		= allocm( GRAB_MEM );
//	Exec->size		= 0;
	
//

	sizeOnly		= 1;
	doParse();
	if( errorToggle )
		return 0;
	while( A->interatom ) destruct( A->interatom ) ;		// !!!!
//		dump_a( A ) ;
//	*/
	bufferp			= str;
	resetparse		= 1;
	linederr		= 0;
//	errorToggle		= 0;
	Exec			= A->datum;
	lineno			= 1;
	line_p			= bufferp;
	numloop			= 0;
	last_i			= 0;
	loopBeginCache[0]	= 0;

//	if(Exec->size==0)
//		return 0;

#define GRAB_MEM 8192
	Exec->exec		= allocm( GRAB_MEM );	// UPDATE: GRAB_MEM is temporary, transition to atomic stack of tokens
	Exec->linep		= allocm( GRAB_MEM );
	Exec->size		= 0;

	sizeOnly		= 0;

	a = doParse() ;
	if( a->precursor == _tkn_error || ! Exec->size )
		{
		destruct( a ) ;
		dealloc( Exec->exec ) ;
		dealloc( Exec->linep ) ;
		Exec->linep		= 0 ;
		Exec->exec		= 0 ;
		Exec->size		= 0 ;
		return 0 ;
		}
	interatomize( a , A ) ;
	Exec->exec		= allocr( Exec->exec , sizeof(union _avei) * Exec->size );
	Exec->linep		= allocr( Exec->linep , sizeof(string) * Exec->size );
	return 1 ;
	}

void tkn_number_destruct_a( atom A )
	{
	extern void		(*destruct)(atom) ;
	atom			a ;
	if( ( a = A->datum ) )
		if( --a->allusions == 0 )
			destruct( a ) ;
	}


void tkn_string_destruct_a( atom A )
	{
	extern void		(*destruct)(atom) ;
	atom			a ;
	if( ( a = A->datum ) )
		if( --a->allusions == 0 )
			destruct( a ) ;
	}

void tkn_text_destruct_a( atom A )
	{
	extern void		(*destruct)(atom) ;
	atom			a ;
	if( ( a = A->datum ) )
		if( --a->allusions == 0 )
			destruct( a ) ;
	}


void tkn_constructor(void)
	{
	extern atom		atomine ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void*		(*allocm)(unsigned int) ;
	extern atom		(*oDestructor)(atom,void(*)(atom)) ;
	extern string		(*str_dup)(string) ;
	int			x;

	_tkn			= atomize( atomine );
	_tkn_error		= atomize( _tkn );
	_tkn_number		= atomize( _tkn );
	_tkn_string		= atomize( _tkn );			// UPDATE: may use another name besides _tkn_string to better describe single character and multiple character strings
	_tkn_str		= atomize( _tkn );
	_tkn_text		= atomize( _tkn );
	_tkn_continue		= atomize( _tkn );
	_tkn_pull		= atomize( _tkn );
	_tkn_pushr		= atomize( _tkn );
	_tkn_pushl		= atomize( _tkn );
	_tkn_popr		= atomize( _tkn );
	_tkn_popl		= atomize( _tkn );
	_tkn_begin		= atomize( _tkn );
	_tkn_stmt		= atomize( _tkn );
	_tkn_stmtlist		= atomize( _tkn );
	_tkn_ref		= atomize( _tkn );
	_tkn_var		= atomize( _tkn );
	_tkn_func		= atomize( _tkn );
	_tkn_assign		= atomize( _tkn );
	_tkn_lvar		= atomize( _tkn );
	_tkn_ovar		= atomize( _tkn );
	_tkn_expr		= atomize( _tkn );
	_tkn_end		= atomize( _tkn );
	_tkn_to			= atomize( _tkn );
	_tkn_x2b3d		= atomize( _tkn );
	_tkn_x2d3d		= atomize( _tkn );
	_tkn_x2d3e		= atomize( _tkn );
	_tkn_x2626		= atomize( _tkn );
	_tkn_x3636		= atomize( _tkn );
	_tkn_x3d3d		= atomize( _tkn );
	_tkn_x3d3e		= atomize( _tkn );
	_tkn_x3c3d		= atomize( _tkn );
	_tkn_x3c3e		= atomize( _tkn );
	_tkn_x407e		= atomize( _tkn );
	_tkn_x4023		= atomize( _tkn );
	_tkn_x7c7c		= atomize( _tkn );
	_tkn_x7e7e		= atomize( _tkn );
	_tkn_loop		= atomize( _tkn );
	_tkn_loop1		= atomize( _tkn );
	_tkn_lvarlist		= atomize( _tkn );
	_tkn_andor		= atomize( _tkn );
	_tkn_expr_stmt		= atomize( _tkn );
	_tkn_array		= atomize( _tkn );
	_tkn_aryexpr		= atomize( _tkn );
	_tkn_xxa		= atomize( _tkn );
	_tkn_xxb		= atomize( _tkn );
	_tkn_xxc		= atomize( _tkn );
	_tkn_xxd		= atomize( _tkn );
	_tkn_xxe		= atomize( _tkn );
	_tkn_xxf		= atomize( _tkn );
	_tkn_xxg		= atomize( _tkn );
	_tkn_brf		= atomize( _tkn );
	_tkn_pos		= atomize( _tkn );
	_tkn_ascii2c		= atomize( _tkn );
	_tkn_bra		= atomize( _tkn );
	_tkn_endloop		= atomize( _tkn );

	for( x = 0 ; x < 128 ; x++ )
		_tkn_ascii[x]	= atomize( _tkn );

	_tkn_line_p		= atomize( atomine ) ;
	_tkn_code		= atomize( atomine ) ;
	_tkn_stack		= atomize( atomine ) ;


	oDestructor( _tkn_number , tkn_number_destruct_a ) ;
	oDestructor( _tkn_string , tkn_string_destruct_a ) ;
	oDestructor( _tkn_text	 , tkn_text_destruct_a ) ;

	_t = allocm( sizeof(struct _atom*)  * 256);
	_n = allocm( sizeof(int*) * 256);


	aToken_sNotInALoop		= aAsciiString_atomize( str_dup( "not in a loop" ) ) ;
	aToken_sCommentNotTerminated	= aAsciiString_atomize( str_dup( "comment not terminated" ) ) ;
	aToken_sQuoteNotTerminated	= aAsciiString_atomize( str_dup( "quote not terminated" ) ) ;
	aToken_sInvalidCharacter	= aAsciiString_atomize( str_dup( "invalid character" ) ) ;
	aToken_sUndefinedKeyword	= aAsciiString_atomize( str_dup( "undefined $keyword" ) ) ;
	aToken_sUndefinedProcedure	= aAsciiString_atomize( str_dup( "undefined procedure" ) ) ;
	aToken_sSyntax			= aAsciiString_atomize( str_dup( "syntax" ) ) ;
	}

void tkn_destructor(void)
	{
	extern void		(*dealloc)(void*) ;
	extern void		(*destruct)(atom) ;

	destruct( aToken_sNotInALoop ) ;
	destruct( aToken_sCommentNotTerminated ) ;
	destruct( aToken_sQuoteNotTerminated ) ;
	destruct( aToken_sInvalidCharacter ) ;
	destruct( aToken_sUndefinedKeyword ) ;
	destruct( aToken_sUndefinedProcedure ) ;
	destruct( aToken_sSyntax ) ;

	dealloc( _n ) ;
	dealloc( _t ) ;
	destruct( _tkn_stack ) ;
	destruct( _tkn_code ) ;
	destruct( _tkn_line_p ) ;
	destruct( _tkn ) ;
	}


int diagonal(int c)
	{
	switch ( tolower(c) )
		{
		case 'n': return '\n';
		case 'b': return '\b';
		case 't': return '\t';
		case 'r': return '\r';
		case 'f': return '\f';
		default : return c;
		}
	}


atom e_string( string S , executable E )
	{
	extern atom		aAsciiString ;
	atom			a ;
	string			s ;

	a = E->a->interatom ;
	while( a )
		{
		if( a->precursor == aAsciiString )
			{
			s = (string)a->datum ;
			if( cmp_s( s , S ) == 0 )
				return a ;
			}
		a = a->interatoms ;
		}
	return 0 ;
	}


atom e_number( unsigned N , executable E )
	{
	extern atom		aInteger ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void		(*interatomize)(atom,atom) ;
	extern string		(*str_dup)(string) ;
	atom			a ;

	a = E->a->interatom ;
	while( a )
		{
		if( a->precursor == aInteger )
			if( ( (unsigned)a->datum ) == N )
				return a ;
		a = a->interatoms ;
		}
	return 0 ;
	}


string tkn_line_p( atom A )
	{
	atom		a ;
	
	a = A->interatom ;
	do
		if( a->precursor == _tkn_line_p )
			return a->datum ;
		while( ( a = a->interatoms ) ) ;
	return 0 ;
	}

atom tkn_a_s( atom A , string S )
	{
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	atom			t ;
	atom			a ;

	t		= atomize( A ) ;
	a		= atomize( _tkn_line_p ) ;
	a->datum	= S ;
	interatomize( a , t ) ;
	return t ;
	}


atom tkn( atom A , atom B )
	{
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	atom			t ;
	atom			a ;
	atom			b ;

	t		= atomize( A ) ;
	if( B )
		{
		b		= B->interatom ;
		do
			if( b->precursor == _tkn_line_p )
				{
				a = atomize( _tkn_line_p ) ;
				a->datum = b->datum ;
				interatomize( a , t ) ;
				break ;
				}
			while( ( b = b->interatoms ) ) ;
		}
	return t ;
	}


#define	TKN(A)	tkn_a_s( _tkn_##A , line_p )


atom nxtkn(void)
	{
	extern atom		(*atomize)(atom) ;
	extern atom		(*oInteger)(int) ;
	extern void*		(*allocm)(unsigned int) ;
	extern void*		(*allocr)(void*,unsigned int) ;
	extern void		(*dealloc)(void*) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void		(*interatomize)(atom,atom) ;
	extern string		(*str_dup)(string) ;
	extern atom		(*aError_atomize_a)(atom) ;
	int			c ;
	atom			o ;
	atom			a ;
	char			buf[1024] ;
	string			s ;
	string			p ;
	string			n ;
	unsigned int		i ;

//	if(ntkn)
//		{ 
//		o	= ntkn;
//		ntkn	= atominul;
//		return o;
//		}

	START_LEX:
	while((c=GetChar())==' '||c=='\t'||c=='\n') 
		if(c=='\n') { lineno++; line_p=bufferp; }
	if(c=='/') {
		if((c=GetChar())=='*') {
			while(c!=0 && (c=GetChar())!=0) {
				if(c=='*') if((c=GetChar())=='/') break;
				if(c=='\n') { lineno++; line_p=bufferp; }
				}
			if(c==4 || c==0)
				{
				a = TKN( error ) ;
				interatomize( aError_atomize_a( aToken_sCommentNotTerminated ) , a ) ;
//				err_s("comment not terminated");
				return a ;
				}
			goto START_LEX;
			}
		else if(c=='/') {
			while(c!=0 && (c=GetChar())!=0) {
				if(c=='\n') { lineno++; line_p=bufferp; break; }
				}
			if(c==4 || c==0)
				{
				a = TKN( error ) ;
				interatomize( aError_atomize_a( aToken_sCommentNotTerminated ) , a ) ;
//				err_s("comment not terminated");
				return a ;
				}
			goto START_LEX;
			}
		else {
			UnGetChar(c);
			c='/';
			}
		}
	if(isdigit(c))
		{
		p	= buf ;
		i	= sizeof( buf ) ;
		do	*p++ = c;
			while( isdigit( c = GetChar() ) && --i ) ;
		if( i )
			*p = 0 , p = buf , n = 0 ;
		else	{
			s = n	= allocm( sizeof(buf) * 2 ) ;
			p	= buf ;
			i	= sizeof( buf ) ;
			do	*s++ = *p++ ;
				while( --i ) ;
			do	{
				i	= sizeof( buf ) ;
				do	*s++ = c ;
					while( isdigit( c = GetChar() ) && --i ) ;
				if( i )
					break ;
				i	= s - n ;
				n	= allocr( n , i + sizeof( buf ) ) ;
				s	= n + i ;
				} while( 1 ) ;
			*s++	= 0 ;
			p	= n ;
			}
		UnGetChar(c) ;
		p	= buf ;
		i	= *p - 0x30 ;
		while( *++p )
			{
			i	*= 10 ;
			i	+= *p - 0x30 ;
			// UPDATE: if( i & (2^31) )
			}
		if( n )
			dealloc( n ) ;
		o	= TKN( number ) ;
		if( ( a = e_number( i , Exec ) ) )
			((atom)o->datum = a)->allusions++ ;
		else	{
			o->datum = oInteger( i ) ;
			interatomize( o->datum , Exec->a ) ;
			}
		return o ;
		}
	if( isalpha(c) || c=='_' )
		{
		p	= buf ;
		i	= sizeof( buf ) ;
		do	*p++ = c ;
			while( ( ( c = GetChar() ) == '_' || isalnum(c) ) && --i ) ;
		if( i )
			*p = 0 , p = buf , n = 0 ;
		else	{
			s = n	= allocm( sizeof(buf) * 2 ) ;
			p	= buf ;
			i	= sizeof( buf ) ;
			do	*s++ = *p++ ;
				while( --i ) ;
			do	{
				i	= sizeof( buf ) ;
				do	*s++ = c ;
					while( ( ( c = GetChar() ) == '_' || isalnum(c) ) && --i ) ;
				if( i )
					break ;
				i	= s - n ;
				n	= allocr( n , i + sizeof( buf ) ) ;
				s	= n + i ;
				} while( 1 ) ;
			*s++	= 0 ;
			p	= n ;
			}
		UnGetChar(c);
		for(i=0;keywords[i].type!=0;i++)
			if(cmp_s(keywords[i].name,buf)==0) {
				if(keywords[i].type == NUMBER) {
					o = TKN( number ) ; // UPDATE:
					if( ( a = e_number( keywords[i].code , Exec ) ) )
						((atom)o->datum = a)->allusions++ ;
					else	{
						o->datum = oInteger(keywords[i].code);
						interatomize( o->datum , Exec->a ) ;
						}
					return o;
					}
				switch(keywords[i].code) {
  					case CONTINUE:	return TKN( continue ) ;
  					case PULL:	return TKN( pull ) ;
  					case PUSHR:	return TKN( pushr ) ;
  					case PUSHL:	return TKN( pushl ) ;
  					case POPR:	return TKN( popr ) ;
  					case POPL:	return TKN( popl ) ;
					}
				}
		o	 = TKN( string ) ;
		if( ( a = e_string( p , Exec ) ) )
			{
			((atom)o->datum = a)->allusions++ ;
			if( n )
				dealloc( n ) ;
			return o ;
			}
		if( n )
			o->datum = a = aAsciiString_atomize( allocr( n , s - n ) ) ;
		else
			o->datum = a = aAsciiString_atomize( str_dup( p ) ) ;
		interatomize( a , Exec->a ) ;
		return o ;
		}
	if(c=='"')
		{	/* quoted string */
		n	= 0 ;
		p	= buf ;
		i	= sizeof( buf ) ;
		while( (c=GetChar()) != '"' )
			{
			if( c == '\n' )
				lineno++ , c = '\n' ;
			if( c == '\\' )
				c=diagonal(GetChar()) ;
			if( c == 0 )
				{
				a = TKN( error ) ;
				interatomize( aError_atomize_a( aToken_sQuoteNotTerminated ) , a ) ;
//				err_s("Hit EOF while in string!") ;
				if( n )
					dealloc( n ) ;
				return a ;
				}
			*p++	= c;
			if( ! --i )
				if( n )	{
					i	= p - n ;
					n	= allocr( n , i + sizeof( buf ) ) ;
					p	= n + i ;
					i	= sizeof( buf ) ;
					}
				else	{
					s	= buf ;
					p = n	= allocm( sizeof(buf) * 2 ) ;
					i	= sizeof( buf ) ;
					do	*p++ = *s++ ;
						while( --i ) ;
					i	= sizeof( buf ) ;
					}
			}
		*p++	= 0 ;
		o	 = TKN( text ) ;
		if( n )	{
			if( ( a = e_string( n , Exec ) ) )
				{
				((atom)o->datum = a)->allusions++ ;
				dealloc( n ) ;
				return o ;
				}
			o->datum = a = aAsciiString_atomize( allocr( n , p - n ) ) ;
			}
		else	{
			if( ( a = e_string( buf , Exec ) ) )
				{
				((atom)o->datum = a)->allusions++ ;
				return o ;
				}
			o->datum = a = aAsciiString_atomize( str_dup( buf ) ) ;
			}
		interatomize( a , Exec->a ) ;
		return o ;
		}
	if(c=='\'') {	/* quoted char */
		c = GetChar();
		if(GetChar() != '\'')
			{
			a = TKN( error ) ;
			interatomize( aError_atomize_a( aToken_sInvalidCharacter ) , a ) ;
//			err_s("invalid character");
			return a ;
			}

		o = TKN( number ) ; // UPDATE: use ASCII
		if( ( a = e_number( c , Exec ) ) )
			((atom)o->datum = a)->allusions++ ;
		else	{
			o->datum = oInteger( c ) ;
			interatomize( o->datum , Exec->a ) ;
			}
		return o;
		}
#	define ifNext(_1,_2)							\
			if(*(bufferp++)==_1)					\
				return TKN( _2 ) ;				\
			else	{						\
				o= TKN( ascii[(int)*((--bufferp)-1)] ) ;	\
				return o ;					\
				}
#	define ifNext2(_1,_2,_3,_4)						\
			if(*(bufferp++)==_1)					\
			return TKN( _2 ) ; 			  		\
			else							\
			if(*(bufferp-1)==_3)					\
				return TKN( _4 ) ;				\
			else	{						\
				o= TKN( ascii[(int)*((--bufferp)-1)] ) ;	\
				return o ;					\
				}
	switch( c )
		{
	case '<': ifNext2('>',x3c3e,'=',x3c3d);
	case '&': ifNext('&',x2626);
	case '|': ifNext('|',x7c7c);
	case '~': ifNext('~',x7e7e);
	case '@': ifNext2('~',x407e,'#',x4023);
	case '=': ifNext2('=',x3d3d,'>',x3d3e);
	case '.': ifNext('.',to);
	case '+': ifNext('=',x2b3d);
	case '-': ifNext2('=',x2d3d,'>',x2d3e);
		}
	if( c == 4 || c <= 0 )
		return TKN( end ) ;  // 4 == ASCII EOF
	return TKN( ascii[c] ) ;
	}

/* ========================================================================= */
/* ========================================================================= */
/* ========================================================================= */
/* ========================================================================= */
/* ========================================================================= */

atom doParse(void)
{
extern atom		aError ;
extern atom		(*atomize)(atom) ;
extern void		(*destruct)(atom) ;
extern void		(*interatomize)(atom,atom) ;
extern atom		(*aError_atomize_a)(atom) ;
extern short		Find_Or_Add_Var(atom);
extern void		loopBegin(short);
extern void		loopEnd(void);
extern atom		Find_Obj_KeyWord(string);
extern int		Code_Obj_Var(atom,string);
int			_z		= -1;			// UPDATE: eliminate _z, use _y, offset _t by 1, start _y @ 0
int			_y		= -1;
atom			_		= atominul;
int			_0		= 0;
union _avei		ti;
int	 		x;
atom			_p;
atom			a ;
atom			b ;

ntkn	= atominul;

#if defined( _debugParse_ ) || 1
# define	_cft(_1,_2)	if( _j(_1)->precursor != _2) goto _rr;
#else
# define	_cft(_1,_2)
#endif
#define		_crg(_1)	x = _1 ; goto _lp ;
#define		_ctc(_1)	{ _0 = _1 ; goto _ntkn_ ; }
#define		_css(_1)	if( _p == _1 )
#define		_csc(_1)	if( _p == _tkn_ascii[_1] )
#define		_cfc(_1)	{ ntkn = _ ;  _0 = _1 ; goto _rd ; }
//#define		_cfc(_1)	{ _0 = _1 ; goto _ntkn ; }		// UPDATE: optimize goto
#define		_cgt(_1)	{ _0 = _1 ; goto _ntkn_ ; }
#define		_crc(_1)	{ _0 = _1 ; goto _rd ; }
#define		_cpt()		goto _rt ;
#define		_rrr()		goto _rr ;
#define		___		goto _rr ;
#define		____		switch(_0) { case
#define		__		goto _rr ; case
#define		_____		}
#define		_n(_1)		(int)(_t[_z+_1]->datum)
#define		_s(_1)		_t[_z+_1]->datum
#define		_d(_1)		((atom)_t[_z+_1]->datum)->datum
#define		_j(_1)		_t[_z+_1]
#define		_crg_0		_z--, _y--; _crg0 ;
#define		_crg_(_1)	_z-=_1, _y-=_1; _crg0 ;
#define		_cds(_1)	if( --_t[_z+_1]->allusions == 0 )	\
					destruct( _t[_z+_1] ) ;
#define		_crg0		_0 = _n[_y] ; goto _rd ;
#define		_crg1		_0 = _n[--_y] ;	 interatomize( _t[_z--] , _ ) ; goto _rd ;

/*
#define		_crg1		_0 = _n[--_y] ;			\
				if( --_t[_z]->allusions == 0 )	\
					destruct( _t[_z] ) ;	\
				_z-- ;				\
				goto _rd ;
*/
goto _ntkn;

_lp:
_0 = _n[_y-=x] ;
while( x-- )
	{
	interatomize( _t[_z] , _ ) ;
//	if( --_t[_z]->allusions == 0 )
//		destruct( _t[_z] ) ;
	_z-- ;
	}
goto _rd ;

_ntkn_:
_t[++_z] = _ ;
_ntkn:

#ifdef DEBUG
	if( _0 < 0 ) _asm int 3 ;		// UPDATE: should not happen with _cfc optimized
#endif
//if( _0 >= 0 )
//	{
	_n[++_y] = _0 ;
	if( ! ntkn )
		_ = nxtkn() ;
	else	{
		_ = ntkn ;
		ntkn = atominul ;
		}
//	}
//else	ntkn = _ ;

_rd:
#if defined( DEBUG ) && 0
	{
	extern void (*output)(string) ;
	char s[100] ;
	if(_0 >= 0) {
		sprintf(s, "_0 = %4d\t\t%4d \t%s\n",_0,_y, ptkn(_));
		}
	else	{
		sprintf(s , "_0 = %4d\n",_0);
		}
	output(s) ;
	}
#endif
_p = _->precursor;
____ 0:
_css(_tkn_error)		_ctc(  1)
_css(_tkn_number)		_ctc(  2)
_css(_tkn_string)		_ctc(  3)
_css(_tkn_text)			_ctc(  4)
_csc('[')			_ctc(  5)
_csc('{')			_ctc(  6)
_csc('(')			_ctc(  7)
_csc('!')			_ctc(  8)
_csc('<')			_ctc(  9)
_css(_tkn_pushr)		_ctc( 10)
_css(_tkn_popr)			_ctc( 11)
_css(_tkn_pushl)		_ctc( 12)
_css(_tkn_popl)			_ctc( 13)
_css(_tkn_pull)			_ctc( 14)
_csc('-')			_ctc( 15)
_csc('~')			_ctc( 16)
_css(_tkn_x7e7e)		_ctc( 17)
_csc(':')			_ctc( 18)
_csc('.')			_ctc( 19)
_csc('$')			_ctc( 20)
_css(_tkn_begin)		_cgt( 21)
_css(_tkn_stmt)			_cgt( 22)
_css(_tkn_stmtlist)		_cgt( 23)
_css(_tkn_ref)			_cgt( 24)
_css(_tkn_var)			_cgt( 25)
_css(_tkn_func)			_cgt( 26)
_css(_tkn_assign)		_cgt( 27)
_css(_tkn_lvar)			_cgt( 28)
_css(_tkn_expr)			_cgt( 29)
_css(_tkn_ovar)			_cgt( 30)
_css(_tkn_str)			_cgt( 31)
_css(_tkn_end)			_cfc( -1)
__ 1:	 			_cfc( -3)
__ 2:				_cfc(-53)
__ 3:
_csc('(')			_ctc( 32)
_css(_tkn_string)		_cfc(-79)
_csc(';')			_cfc(-79)
_csc('[')			_cfc(-79)
_csc(']')			_cfc(-79)
_csc('}')			_cfc(-79)
_csc(')')			_cfc(-79)
_csc('!')			_cfc(-79)
_css(_tkn_to)			_cfc(-79)
_css(_tkn_x2d3e)		_cfc(-79)
_csc('=')			_cfc(-79)
_css(_tkn_x2b3d)		_cfc(-79)
_css(_tkn_x2d3d)		_cfc(-79)
_csc(',')			_cfc(-79)
_css(_tkn_x2626)		_cfc(-79)
_css(_tkn_x7c7c)		_cfc(-79)
_csc('&')			_cfc(-79)
_csc('|')			_cfc(-79)
_csc('<')			_cfc(-79)
_css(_tkn_x3d3e)		_cfc(-79)
_css(_tkn_x3c3d)		_cfc(-79)
_css(_tkn_x3d3d)		_cfc(-79)
_css(_tkn_x3c3e)		_cfc(-79)
_css(_tkn_x407e)		_cfc(-79)
_css(_tkn_x4023)		_cfc(-79)
_csc('>')			_cfc(-79)
_csc('+')			_cfc(-79)
_csc('-')			_cfc(-79)
_csc('*')			_cfc(-79)
_csc('/')			_cfc(-79)
_csc('@')			_cfc(-79)
_csc(':')			_cfc(-79)
_csc('.')			_cfc(-79)
_csc('?')			_cfc(-79)
__ 4:	  			_cfc(-54)
__ 5:
_css(_tkn_string)		_ctc(33)
_css(_tkn_number)  		_cfc(-23)
_css(_tkn_text)  		_cfc(-23)
_csc('{')  			_cfc(-23)
_csc('(')  			_cfc(-23)
_css(_tkn_pushr)  		_cfc(-23)
_css(_tkn_popr)  		_cfc(-23)
_css(_tkn_pushl)  		_cfc(-23)
_css(_tkn_popl)  		_cfc(-23)
_css(_tkn_pull)  		_cfc(-23)
_csc('-')  			_cfc(-23)
_csc('~')  			_cfc(-23)
_css(_tkn_x7e7e)  		_cfc(-23)
_csc(':')  			_cfc(-23)
_csc('.')  			_cfc(-23)
_csc('$')  			_cfc(-23)
_css(_tkn_loop)			_cgt(34)
__ 6:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('[')			_ctc(5)
_csc('{')			_ctc(6)
_csc('(')			_ctc(7)
_csc('!')			_ctc(8)
_csc('<')			_ctc(9)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(19)
_csc('$')			_ctc(20)
_csc('}')  			_cfc(-58)
_csc(',')  			_cfc(-58)
_csc('+')  			_cfc(-58)
_css(_tkn_stmt)			_cgt(22)
_css(_tkn_stmtlist)		_cgt(35)
_css(_tkn_ref)			_cgt(24)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(26)
_css(_tkn_assign)		_cgt(27)
_css(_tkn_lvar)			_cgt(28)
_css(_tkn_expr)			_cgt(29)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_xxg)			_cgt(36)
__ 7:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(42)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 8:	  			_cfc(-9)
__ 9: _csc(';')			_ctc(43)
__ 10: _csc('(')		_ctc(44)
__ 11: _csc('(')		_ctc(45)
__ 12: _csc('(')		_ctc(46)
__ 13: _csc('(')		_ctc(47)
__ 14: _csc('(')		_ctc(48)
__ 15:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(49)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 16:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(50)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 17:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(51)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 18:
_css(_tkn_string)		_ctc(52)
__ 19:
_css(_tkn_string)		_ctc(53)
_csc(';')			_ctc(54)
_csc('(')			_ctc(55)
__ 20:
_css(_tkn_string)		_ctc(56)
__ 21:
_css(_tkn_end)			_cpt()
__ 22:  			_cfc(-4)
__ 23:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('[')			_ctc(5)
_csc('{')			_ctc(6)
_csc('(')			_ctc(7)
_csc('!')			_ctc(8)
_csc('<')			_ctc(9)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(19)
_csc('$')			_ctc(20)
_css(_tkn_end) 			_cfc(-2)
_css(_tkn_stmt)			_cgt(57)
_css(_tkn_ref)			_cgt(24)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(26)
_css(_tkn_assign)		_cgt(27)
_css(_tkn_lvar)			_cgt(28)
_css(_tkn_expr)			_cgt(29)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 24:
_css(_tkn_string)		_ctc(58)
_css(_tkn_lvarlist)		_cgt(59)
_css(_tkn_lvar)			_cgt(60)
_css(_tkn_str)			_cgt(31)
__ 25:
_csc('[')			_ctc(61)
_csc('=')			_ctc(62)
_css(_tkn_x2b3d)		_ctc(63)
_css(_tkn_x2d3d)		_ctc(64)
_css(_tkn_string)  		_cfc(-55)
_csc(';')  			_cfc(-55)
_csc(']')  			_cfc(-55)
_csc('}')  			_cfc(-55)
_csc(')')  			_cfc(-55)
_csc('!')  			_cfc(-55)
_css(_tkn_to)  			_cfc(-55)
_csc(',')  			_cfc(-55)
_css(_tkn_x2626)  		_cfc(-55)
_css(_tkn_x7c7c)  		_cfc(-55)
_csc('&')  			_cfc(-55)
_csc('|')  			_cfc(-55)
_csc('<')  			_cfc(-55)
_css(_tkn_x3d3e)  		_cfc(-55)
_css(_tkn_x3c3d)  		_cfc(-55)
_css(_tkn_x3d3d)  		_cfc(-55)
_css(_tkn_x3c3e)  		_cfc(-55)
_css(_tkn_x407e)  		_cfc(-55)
_css(_tkn_x4023)  		_cfc(-55)
_csc('>')  			_cfc(-55)
_csc('+')  			_cfc(-55)
_csc('-')  			_cfc(-55)
_csc('*')  			_cfc(-55)
_csc('/')  			_cfc(-55)
_csc('@')  			_cfc(-55)
_csc(':')  			_cfc(-55)
_csc('.')  			_cfc(-55)
_csc('?')  			_cfc(-55)
__ 26:
_csc(';')			_ctc(65)
_csc('[')			_ctc(66)
_css(_tkn_string)  		_cfc(-57)
_csc('!')  			_cfc(-57)
_css(_tkn_x2626)  		_cfc(-57)
_css(_tkn_x7c7c)  		_cfc(-57)
_csc('&')  			_cfc(-57)
_csc('|')  			_cfc(-57)
_csc('<')  			_cfc(-57)
_css(_tkn_x3d3e)  		_cfc(-57)
_css(_tkn_x3c3d)  		_cfc(-57)
_css(_tkn_x3d3d)  		_cfc(-57)
_css(_tkn_x3c3e)  		_cfc(-57)
_css(_tkn_x407e)  		_cfc(-57)
_css(_tkn_x4023)  		_cfc(-57)
_csc('>')  			_cfc(-57)
_csc('+')  			_cfc(-57)
_csc('-')  			_cfc(-57)
_csc('*')  			_cfc(-57)
_csc('/')  			_cfc(-57)
_csc('@')  			_cfc(-57)
_csc(':')  			_cfc(-57)
_csc('.')  			_cfc(-57)
_csc('?')  			_cfc(-57)
__ 27:
_csc(';')			_ctc(67)
_css(_tkn_string)  		_cfc(-56)
_csc('!')  			_cfc(-56)
_css(_tkn_x2626)  		_cfc(-56)
_css(_tkn_x7c7c)  		_cfc(-56)
_csc('&')  			_cfc(-56)
_csc('|')  			_cfc(-56)
_csc('<')  			_cfc(-56)
_css(_tkn_x3d3e)  		_cfc(-56)
_css(_tkn_x3c3d)  		_cfc(-56)
_css(_tkn_x3d3d)  		_cfc(-56)
_css(_tkn_x3c3e)  		_cfc(-56)
_css(_tkn_x407e)  		_cfc(-56)
_css(_tkn_x4023)  		_cfc(-56)
_csc('>')  			_cfc(-56)
_csc('+')  			_cfc(-56)
_csc('-')  			_cfc(-56)
_csc('*')  			_cfc(-56)
_csc('/')  			_cfc(-56)
_csc('@')  			_cfc(-56)
_csc(':')  			_cfc(-56)
_csc('.')  			_cfc(-56)
_csc('?')  			_cfc(-56)
__ 28:
_css(_tkn_x2d3e)		_ctc(68)
_csc('.')			_ctc(69)
_css(_tkn_string)  		_cfc(-88)
_csc('[')  			_cfc(-88)
_csc('!')  			_cfc(-88)
_csc('=')  			_cfc(-88)
_css(_tkn_x2b3d)  		_cfc(-88)
_css(_tkn_x2d3d)  		_cfc(-88)
_css(_tkn_x2626)  		_cfc(-88)
_css(_tkn_x7c7c)  		_cfc(-88)
_csc('&')  			_cfc(-88)
_csc('|')  			_cfc(-88)
_csc('<')  			_cfc(-88)
_css(_tkn_x3d3e)  		_cfc(-88)
_css(_tkn_x3c3d)  		_cfc(-88)
_css(_tkn_x3d3d)  		_cfc(-88)
_css(_tkn_x3c3e)  		_cfc(-88)
_css(_tkn_x407e)  		_cfc(-88)
_css(_tkn_x4023)  		_cfc(-88)
_csc('>')  			_cfc(-88)
_csc('+')  			_cfc(-88)
_csc('-')  			_cfc(-88)
_csc('*')  			_cfc(-88)
_csc('/')  			_cfc(-88)
_csc('@')  			_cfc(-88)
_csc(':')  			_cfc(-88)
_csc('?')  			_cfc(-88)
__ 29:
_css(_tkn_string)		_ctc(70)
_csc('!')			_ctc(71)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc('?')			_ctc(91)
_css(_tkn_andor)		_cgt(92)
_css(_tkn_expr_stmt)		_cgt(93)
__ 30:  			_cfc(-89)
__ 31:  			_cfc(-78)
__ 32: _css(_tkn_xxa)		_cgt(94)
  				_cfc(-35)
__ 33: _csc(';')		_ctc(95)
				_crc(-100)
__ 34:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(96)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 35:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('[')			_ctc(5)
_csc('{')			_ctc(6)
_csc('}')			_ctc(97)
_csc('(')			_ctc(7)
_csc('!')			_ctc(8)
_csc('<')			_ctc(9)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(19)
_csc('$')			_ctc(20)
_css(_tkn_stmt)			_cgt(57)
_css(_tkn_ref)			_cgt(24)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(26)
_css(_tkn_assign)		_cgt(27)
_css(_tkn_lvar)			_cgt(28)
_css(_tkn_expr)			_cgt(29)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 36:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('+')			_ctc(98)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_csc('}')  			_cfc(-94)
_csc(',')  			_cfc(-94)
_css(_tkn_array)		_cgt(99)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(100)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_aryexpr)		_cgt(101)
__ 37: _css(_tkn_xxg)		_cgt(36)
	  			_cfc(-58)
__ 38:
_css(_tkn_string)		_ctc(53)
_csc('(')			_ctc(55)
__ 39:
_csc('[')			_ctc(66)
_css(_tkn_string)  		_cfc(-57)
_csc(';')  			_cfc(-57)
_csc(']')  			_cfc(-57)
_csc('}')  			_cfc(-57)
_csc(')')  			_cfc(-57)
_csc('!')  			_cfc(-57)
_css(_tkn_to)  			_cfc(-57)
_csc(',')  			_cfc(-57)
_css(_tkn_x2626)  		_cfc(-57)
_css(_tkn_x7c7c)  		_cfc(-57)
_csc('&')  			_cfc(-57)
_csc('|')  			_cfc(-57)
_csc('<')  			_cfc(-57)
_css(_tkn_x3d3e)  		_cfc(-57)
_css(_tkn_x3c3d)  		_cfc(-57)
_css(_tkn_x3d3d)  		_cfc(-57)
_css(_tkn_x3c3e)  		_cfc(-57)
_css(_tkn_x407e)  		_cfc(-57)
_css(_tkn_x4023)  		_cfc(-57)
_csc('>')  			_cfc(-57)
_csc('+')  			_cfc(-57)
_csc('-')  			_cfc(-57)
_csc('*')  			_cfc(-57)
_csc('/')  			_cfc(-57)
_csc('@')  			_cfc(-57)
_csc(':')  			_cfc(-57)
_csc('.')  			_cfc(-57)
_csc('?')  			_cfc(-57)
__ 40:  			_cfc(-56)
__ 41:
_csc('.')			_ctc(69)
_css(_tkn_string)  		_cfc(-88)
_csc(';')  			_cfc(-88)
_csc('[')  			_cfc(-88)
_csc(']')  			_cfc(-88)
_csc('}')  			_cfc(-88)
_csc(')')  			_cfc(-88)
_csc('!')  			_cfc(-88)
_css(_tkn_to)  			_cfc(-88)
_csc('=')  			_cfc(-88)
_css(_tkn_x2b3d)  		_cfc(-88)
_css(_tkn_x2d3d)  		_cfc(-88)
_csc(',')  			_cfc(-88)
_css(_tkn_x2626)  		_cfc(-88)
_css(_tkn_x7c7c)  		_cfc(-88)
_csc('&')  			_cfc(-88)
_csc('|')  			_cfc(-88)
_csc('<')  			_cfc(-88)
_css(_tkn_x3d3e)  		_cfc(-88)
_css(_tkn_x3c3d)  		_cfc(-88)
_css(_tkn_x3d3d)  		_cfc(-88)
_css(_tkn_x3c3e)  		_cfc(-88)
_css(_tkn_x407e)  		_cfc(-88)
_css(_tkn_x4023)  		_cfc(-88)
_csc('>')  			_cfc(-88)
_csc('+')  			_cfc(-88)
_csc('-')  			_cfc(-88)
_csc('*')  			_cfc(-88)
_csc('/')  			_cfc(-88)
_csc('@')  			_cfc(-88)
_csc(':')  			_cfc(-88)
_csc('?')  			_cfc(-88)
__ 42:
_css(_tkn_string)		_ctc(70)
_csc(')')			_ctc(102)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 43:  			_cfc(-14)
__ 44:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(103)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(104)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 45:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(105)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(104)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 46:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(106)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(104)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 47:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(107)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(104)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 48:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(108)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(104)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 49:
_css(_tkn_string)		_ctc(70)
_css(_tkn_andor)		_cgt(92)
	  			_cfc(-49)
__ 50:
_css(_tkn_string)		_ctc(70)
_css(_tkn_andor)		_cgt(92)
	  			_cfc(-50)
__ 51:
_css(_tkn_string)		_ctc(70)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
	  			_cfc(-51)
__ 52: _csc('(')		_ctc(109)
__ 53:  			_cfc(-84)
__ 54:  			_cfc(-13)
__ 55:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(110)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 56: _css(_tkn_string)	_ctc(111)
_csc('.')			_ctc(112)
	  			_cfc(-86)
__ 57:   			_cfc(-5)
__ 58:   			_cfc(-79)
__ 59:
_csc(';')			_ctc(113)
_csc(',')			_ctc(114)
__ 60:   			_cfc(-92)
__ 61:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(115)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 62:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(116)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 63:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(117)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 64:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(118)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 65:   			_cfc(-17)
__ 66:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(119)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 67:   			_cfc(-16)
__ 68:   			_cfc(-19)
__ 69:
_css(_tkn_string)		_ctc(120)
_csc('(')			_ctc(121)
__ 70: _csc('(')		_ctc(122)
__ 71:  			_cfc(-6)
__ 72:  			_cfc(-76)
__ 73:  			_cfc(-77)
__ 74:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(123)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 75:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(124)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 76:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(125)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 77:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(126)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 78:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(127)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 79:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(128)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 80:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(129)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 81:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(130)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 82:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(131)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 83:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(132)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 84:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(133)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 85:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(134)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 86:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(135)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 87:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(136)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 88:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(137)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 89: _csc('(')		_ctc(138)
__ 90:
_css(_tkn_string)		_ctc(139)
_csc('(')			_ctc(140)
__ 91:   		
_css(_tkn_brf)			_cgt(141)
				_cfc(-20)
__ 92:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(142)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 93:  			_cfc(-10)
__ 94:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('+')			_ctc(98)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_csc(')')  			_cfc(-94)
_csc(',')  			_cfc(-94)
_css(_tkn_array)		_cgt(143)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(100)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_aryexpr)		_cgt(101)
__ 95:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(144)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 96:
_css(_tkn_string)		_ctc(70)
_csc(']')			_ctc(145)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 97:   			_cfc(-18)
__ 98:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(146)
_css(_tkn_func)			_cgt(147)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(104)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 99:
_csc('}')			_ctc(148)
_csc(',')			_ctc(149)
__ 100:
_css(_tkn_string)		_ctc(70)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc('}')  			_cfc(-99)
_csc(')')  			_cfc(-99)
_csc(',')  			_cfc(-99)
_css(_tkn_andor)		_cgt(92)
__ 101:   			_cfc(-95)
__ 102:   			_cfc(-52)
__ 103:
_csc('[')			_ctc(61)
_csc('=')			_ctc(62)
_css(_tkn_x2b3d)		_ctc(63)
_css(_tkn_x2d3d)		_ctc(64)
_csc(',')			_ctc(150)
_css(_tkn_string)  		_cfc(-55)
_css(_tkn_x2626)  		_cfc(-55)
_css(_tkn_x7c7c)  		_cfc(-55)
_csc('&')  			_cfc(-55)
_csc('|')  			_cfc(-55)
_csc('<')  			_cfc(-55)
_css(_tkn_x3d3e)  		_cfc(-55)
_css(_tkn_x3c3d)  		_cfc(-55)
_css(_tkn_x3d3d)  		_cfc(-55)
_css(_tkn_x3c3e)  		_cfc(-55)
_css(_tkn_x407e)  		_cfc(-55)
_css(_tkn_x4023)  		_cfc(-55)
_csc('>')  			_cfc(-55)
_csc('+')  			_cfc(-55)
_csc('-')  			_cfc(-55)
_csc('*')  			_cfc(-55)
_csc('/')  			_cfc(-55)
_csc('@')  			_cfc(-55)
_csc(':')  			_cfc(-55)
_csc('.')  			_cfc(-55)
__ 104:
_css(_tkn_string)		_ctc(70)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 105:
_csc('[')			_ctc(61)
_csc(')')			_ctc(151)
_csc('=')			_ctc(62)
_css(_tkn_x2b3d)		_ctc(63)
_css(_tkn_x2d3d)		_ctc(64)
_css(_tkn_string)  		_cfc(-55)
_css(_tkn_x2626)  		_cfc(-55)
_css(_tkn_x7c7c)  		_cfc(-55)
_csc('&')  			_cfc(-55)
_csc('|')  			_cfc(-55)
_csc('<')  			_cfc(-55)
_css(_tkn_x3d3e)  		_cfc(-55)
_css(_tkn_x3c3d)  		_cfc(-55)
_css(_tkn_x3d3d)  		_cfc(-55)
_css(_tkn_x3c3e)  		_cfc(-55)
_css(_tkn_x407e)  		_cfc(-55)
_css(_tkn_x4023)  		_cfc(-55)
_csc('>')  			_cfc(-55)
_csc('+')  			_cfc(-55)
_csc('-')  			_cfc(-55)
_csc('*')  			_cfc(-55)
_csc('/')  			_cfc(-55)
_csc('@')  			_cfc(-55)
_csc(':')  			_cfc(-55)
_csc('.')  			_cfc(-55)
__ 106:
_csc('[')			_ctc(61)
_csc('=')			_ctc(62)
_css(_tkn_x2b3d)		_ctc(63)
_css(_tkn_x2d3d)		_ctc(64)
_csc(',')			_ctc(152)
_css(_tkn_string)  		_cfc(-55)
_css(_tkn_x2626)  		_cfc(-55)
_css(_tkn_x7c7c)  		_cfc(-55)
_csc('&')  			_cfc(-55)
_csc('|')  			_cfc(-55)
_csc('<')  			_cfc(-55)
_css(_tkn_x3d3e)  		_cfc(-55)
_css(_tkn_x3c3d)  		_cfc(-55)
_css(_tkn_x3d3d)  		_cfc(-55)
_css(_tkn_x3c3e)  		_cfc(-55)
_css(_tkn_x407e)  		_cfc(-55)
_css(_tkn_x4023)  		_cfc(-55)
_csc('>')  			_cfc(-55)
_csc('+')  			_cfc(-55)
_csc('-')  			_cfc(-55)
_csc('*')  			_cfc(-55)
_csc('/')  			_cfc(-55)
_csc('@')  			_cfc(-55)
_csc(':')  			_cfc(-55)
_csc('.')  			_cfc(-55)
__ 107:
_csc('[')			_ctc(61)
_csc(')')			_ctc(153)
_csc('=')			_ctc(62)
_css(_tkn_x2b3d)		_ctc(63)
_css(_tkn_x2d3d)		_ctc(64)
_css(_tkn_string)  		_cfc(-55)
_css(_tkn_x2626)  		_cfc(-55)
_css(_tkn_x7c7c)  		_cfc(-55)
_csc('&')  			_cfc(-55)
_csc('|')  			_cfc(-55)
_csc('<')  			_cfc(-55)
_css(_tkn_x3d3e)  		_cfc(-55)
_css(_tkn_x3c3d)  		_cfc(-55)
_css(_tkn_x3d3d)  		_cfc(-55)
_css(_tkn_x3c3e)  		_cfc(-55)
_css(_tkn_x407e)  		_cfc(-55)
_css(_tkn_x4023)  		_cfc(-55)
_csc('>')  			_cfc(-55)
_csc('+')  			_cfc(-55)
_csc('-')  			_cfc(-55)
_csc('*')  			_cfc(-55)
_csc('/')  			_cfc(-55)
_csc('@')  			_cfc(-55)
_csc(':')  			_cfc(-55)
_csc('.')  			_cfc(-55)
__ 108:
_csc('[')			_ctc(61)
_csc('=')			_ctc(62)
_css(_tkn_x2b3d)		_ctc(63)
_css(_tkn_x2d3d)		_ctc(64)
_csc(',')			_ctc(154)
_css(_tkn_string)  		_cfc(-55)
_css(_tkn_x2626)  		_cfc(-55)
_css(_tkn_x7c7c)  		_cfc(-55)
_csc('&')  			_cfc(-55)
_csc('|')  			_cfc(-55)
_csc('<')  			_cfc(-55)
_css(_tkn_x3d3e)  		_cfc(-55)
_css(_tkn_x3c3d)  		_cfc(-55)
_css(_tkn_x3d3d)  		_cfc(-55)
_css(_tkn_x3c3e)  		_cfc(-55)
_css(_tkn_x407e)  		_cfc(-55)
_css(_tkn_x4023)  		_cfc(-55)
_csc('>')  			_cfc(-55)
_csc('+')  			_cfc(-55)
_csc('-')  			_cfc(-55)
_csc('*')  			_cfc(-55)
_csc('/')  			_cfc(-55)
_csc('@')  			_cfc(-55)
_csc(':')  			_cfc(-55)
_csc('.')  			_cfc(-55)
__ 109: _css(_tkn_xxd)		_cgt(155)
	  			_cfc(-41)
__ 110:
_css(_tkn_string)		_ctc(70)
_csc(')')			_ctc(156)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 111: _csc('(')		_ctc(157)
__ 112: _css(_tkn_string)	_ctc(158)
__ 113:  			_cfc(-15)
__ 114:
_css(_tkn_string)		_ctc(58)
_css(_tkn_lvar)			_cgt(159)
_css(_tkn_str)			_cgt(31)
__ 115:
_css(_tkn_string)		_ctc(70)
_csc(']')			_ctc(160)
_css(_tkn_to)			_ctc(161)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 116:
_css(_tkn_string)		_ctc(70)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-27)
_csc(']')  			_cfc(-27)
_csc('}')  			_cfc(-27)
_csc(')')  			_cfc(-27)
_csc('!')  			_cfc(-27)
_css(_tkn_to)  			_cfc(-27)
_csc(',')  			_cfc(-27)
_csc('?')  			_cfc(-27)
_css(_tkn_andor)		_cgt(92)
__ 117:
_css(_tkn_string)		_ctc(70)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-29)
_csc(']')  			_cfc(-29)
_csc('}')  			_cfc(-29)
_csc(')')  			_cfc(-29)
_csc('!')  			_cfc(-29)
_css(_tkn_to)  			_cfc(-29)
_csc(',')  			_cfc(-29)
_csc('?')  			_cfc(-29)
_css(_tkn_andor)		_cgt(92)
__ 118:
_css(_tkn_string)		_ctc(70)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-28)
_csc(']')  			_cfc(-28)
_csc('}')  			_cfc(-28)
_csc(')')  			_cfc(-28)
_csc('!')  			_cfc(-28)
_css(_tkn_to)  			_cfc(-28)
_csc(',')  			_cfc(-28)
_csc('?')  			_cfc(-28)
_css(_tkn_andor)		_cgt(92)
__ 119:
_css(_tkn_string)		_ctc(70)
_csc(']')			_ctc(162)
_css(_tkn_to)			_ctc(163)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 120:   			_cfc(-80)
__ 121:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(164)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 122: _css(_tkn_xxe)		_cgt(165)
	 			_cfc(-43)
__ 123:
_css(_tkn_string)		_ctc(70)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-67)
_csc(']')  			_cfc(-67)
_csc('}')  			_cfc(-67)
_csc(')')  			_cfc(-67)
_csc('!')  			_cfc(-67)
_css(_tkn_to)  			_cfc(-67)
_csc(',')  			_cfc(-67)
_css(_tkn_x2626)  		_cfc(-67)
_css(_tkn_x7c7c)  		_cfc(-67)
_csc('&')  			_cfc(-67)
_csc('|')  			_cfc(-67)
_csc('?')  			_cfc(-67)
_css(_tkn_andor)		_cgt(92)
__ 124:
_css(_tkn_string)		_ctc(70)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-68)
_csc(']')  			_cfc(-68)
_csc('}')  			_cfc(-68)
_csc(')')  			_cfc(-68)
_csc('!')  			_cfc(-68)
_css(_tkn_to)  			_cfc(-68)
_csc(',')  			_cfc(-68)
_css(_tkn_x2626)  		_cfc(-68)
_css(_tkn_x7c7c)  		_cfc(-68)
_csc('&')  			_cfc(-68)
_csc('|')  			_cfc(-68)
_csc('?')  			_cfc(-68)
_css(_tkn_andor)		_cgt(92)
__ 125:
_css(_tkn_string)		_ctc(70)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-72)
_csc(']')  			_cfc(-72)
_csc('}')  			_cfc(-72)
_csc(')')  			_cfc(-72)
_csc('!')  			_cfc(-72)
_css(_tkn_to)  			_cfc(-72)
_csc(',')  			_cfc(-72)
_css(_tkn_x2626)  		_cfc(-72)
_css(_tkn_x7c7c)  		_cfc(-72)
_csc('&')  			_cfc(-72)
_csc('|')  			_cfc(-72)
_csc('?')  			_cfc(-72)
_css(_tkn_andor)		_cgt(92)
__ 126:
_css(_tkn_string)		_ctc(70)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-73)
_csc(']')  			_cfc(-73)
_csc('}')  			_cfc(-73)
_csc(')')  			_cfc(-73)
_csc('!')  			_cfc(-73)
_css(_tkn_to)  			_cfc(-73)
_csc(',')  			_cfc(-73)
_css(_tkn_x2626)  		_cfc(-73)
_css(_tkn_x7c7c)  		_cfc(-73)
_csc('&')  			_cfc(-73)
_csc('|')  			_cfc(-73)
_csc('?')  			_cfc(-73)
_css(_tkn_andor)		_cgt(92)
__ 127:
_css(_tkn_string)		_ctc(70)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-74)
_csc(']')  			_cfc(-74)
_csc('}')  			_cfc(-74)
_csc(')')  			_cfc(-74)
_csc('!')  			_cfc(-74)
_css(_tkn_to)  			_cfc(-74)
_csc(',')  			_cfc(-74)
_css(_tkn_x2626)  		_cfc(-74)
_css(_tkn_x7c7c)  		_cfc(-74)
_csc('&')  			_cfc(-74)
_csc('|')  			_cfc(-74)
_csc('?')  			_cfc(-74)
_css(_tkn_andor)		_cgt(92)
__ 128:
_css(_tkn_string)		_ctc(70)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-69)
_csc(']')  			_cfc(-69)
_csc('}')  			_cfc(-69)
_csc(')')  			_cfc(-69)
_csc('!')  			_cfc(-69)
_css(_tkn_to)  			_cfc(-69)
_csc(',')  			_cfc(-69)
_css(_tkn_x2626)  		_cfc(-69)
_css(_tkn_x7c7c)  		_cfc(-69)
_csc('&')  			_cfc(-69)
_csc('|')  			_cfc(-69)
_csc('?')  			_cfc(-69)
_css(_tkn_andor)		_cgt(92)
__ 129:
_css(_tkn_string)		_ctc(70)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-70)
_csc(']')  			_cfc(-70)
_csc('}')  			_cfc(-70)
_csc(')')  			_cfc(-70)
_csc('!')  			_cfc(-70)
_css(_tkn_to)  			_cfc(-70)
_csc(',')  			_cfc(-70)
_css(_tkn_x2626)  		_cfc(-70)
_css(_tkn_x7c7c)  		_cfc(-70)
_csc('&')  			_cfc(-70)
_csc('|')  			_cfc(-70)
_csc('?')  			_cfc(-70)
_css(_tkn_andor)		_cgt(92)
__ 130:
_css(_tkn_string)		_ctc(70)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-60)
_csc(']')  			_cfc(-60)
_csc('}')  			_cfc(-60)
_csc(')')  			_cfc(-60)
_csc('!')  			_cfc(-60)
_css(_tkn_to)  			_cfc(-60)
_csc(',')  			_cfc(-60)
_css(_tkn_x2626)  		_cfc(-60)
_css(_tkn_x7c7c)  		_cfc(-60)
_csc('&')  			_cfc(-60)
_csc('|')  			_cfc(-60)
_csc('?')  			_cfc(-60)
_css(_tkn_andor)		_cgt(92)
__ 131:
_css(_tkn_string)		_ctc(70)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-62)
_csc(']')  			_cfc(-62)
_csc('}')  			_cfc(-62)
_csc(')')  			_cfc(-62)
_csc('!')  			_cfc(-62)
_css(_tkn_to)  			_cfc(-62)
_csc(',')  			_cfc(-62)
_css(_tkn_x2626)  		_cfc(-62)
_css(_tkn_x7c7c)  		_cfc(-62)
_csc('&')  			_cfc(-62)
_csc('|')  			_cfc(-62)
_csc('?')  			_cfc(-62)
_css(_tkn_andor)		_cgt(92)
__ 132:
_css(_tkn_string)		_ctc(70)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-71)
_csc(']')  			_cfc(-71)
_csc('}')  			_cfc(-71)
_csc(')')  			_cfc(-71)
_csc('!')  			_cfc(-71)
_css(_tkn_to)  			_cfc(-71)
_csc(',')  			_cfc(-71)
_css(_tkn_x2626)  		_cfc(-71)
_css(_tkn_x7c7c)  		_cfc(-71)
_csc('&')  			_cfc(-71)
_csc('|')  			_cfc(-71)
_csc('?')  			_cfc(-71)
_css(_tkn_andor)		_cgt(92)
__ 133:
_css(_tkn_string)		_ctc(70)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-64)
_csc(']')  			_cfc(-64)
_csc('}')  			_cfc(-64)
_csc(')')  			_cfc(-64)
_csc('!')  			_cfc(-64)
_css(_tkn_to)  			_cfc(-64)
_csc(',')  			_cfc(-64)
_css(_tkn_x2626)  		_cfc(-64)
_css(_tkn_x7c7c)  		_cfc(-64)
_csc('&')  			_cfc(-64)
_csc('|')  			_cfc(-64)
_csc('<')  			_cfc(-64)
_css(_tkn_x3d3e)  		_cfc(-64)
_css(_tkn_x3c3d)  		_cfc(-64)
_css(_tkn_x3d3d)  		_cfc(-64)
_css(_tkn_x3c3e)  		_cfc(-64)
_css(_tkn_x407e)  		_cfc(-64)
_css(_tkn_x4023)  		_cfc(-64)
_csc('>')  			_cfc(-64)
_csc('+')  			_cfc(-64)
_csc('-')  			_cfc(-64)
_csc('?')  			_cfc(-64)
_css(_tkn_andor)		_cgt(92)
__ 134:
_css(_tkn_string)		_ctc(70)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-63)
_csc(']')  			_cfc(-63)
_csc('}')  			_cfc(-63)
_csc(')')  			_cfc(-63)
_csc('!')  			_cfc(-63)
_css(_tkn_to)  			_cfc(-63)
_csc(',')  			_cfc(-63)
_css(_tkn_x2626)  		_cfc(-63)
_css(_tkn_x7c7c)  		_cfc(-63)
_csc('&')  			_cfc(-63)
_csc('|')  			_cfc(-63)
_csc('<')  			_cfc(-63)
_css(_tkn_x3d3e)  		_cfc(-63)
_css(_tkn_x3c3d)  		_cfc(-63)
_css(_tkn_x3d3d)  		_cfc(-63)
_css(_tkn_x3c3e)  		_cfc(-63)
_css(_tkn_x407e)  		_cfc(-63)
_css(_tkn_x4023)  		_cfc(-63)
_csc('>')  			_cfc(-63)
_csc('+')  			_cfc(-63)
_csc('-')  			_cfc(-63)
_csc('?')  			_cfc(-63)
_css(_tkn_andor)		_cgt(92)
__ 135:
_css(_tkn_string)		_ctc(70)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-66)
_csc(']')  			_cfc(-66)
_csc('}')  			_cfc(-66)
_csc(')')  			_cfc(-66)
_csc('!')  			_cfc(-66)
_css(_tkn_to)  			_cfc(-66)
_csc(',')  			_cfc(-66)
_css(_tkn_x2626)  		_cfc(-66)
_css(_tkn_x7c7c)  		_cfc(-66)
_csc('&')  			_cfc(-66)
_csc('|')  			_cfc(-66)
_csc('<')  			_cfc(-66)
_css(_tkn_x3d3e)  		_cfc(-66)
_css(_tkn_x3c3d)  		_cfc(-66)
_css(_tkn_x3d3d)  		_cfc(-66)
_css(_tkn_x3c3e)  		_cfc(-66)
_css(_tkn_x407e)  		_cfc(-66)
_css(_tkn_x4023)  		_cfc(-66)
_csc('>')  			_cfc(-66)
_csc('+')  			_cfc(-66)
_csc('-')  			_cfc(-66)
_csc('*')  			_cfc(-66)
_csc('/')  			_cfc(-66)
_csc('?')  			_cfc(-66)
_css(_tkn_andor)		_cgt(92)
__ 136:
_css(_tkn_string)		_ctc(70)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-65)
_csc(']')  			_cfc(-65)
_csc('}')  			_cfc(-65)
_csc(')')  			_cfc(-65)
_csc('!')  			_cfc(-65)
_css(_tkn_to)  			_cfc(-65)
_csc(',')  			_cfc(-65)
_css(_tkn_x2626)  		_cfc(-65)
_css(_tkn_x7c7c)  		_cfc(-65)
_csc('&')  			_cfc(-65)
_csc('|')  			_cfc(-65)
_csc('<')  			_cfc(-65)
_css(_tkn_x3d3e)  		_cfc(-65)
_css(_tkn_x3c3d)  		_cfc(-65)
_css(_tkn_x3d3d)  		_cfc(-65)
_css(_tkn_x3c3e)  		_cfc(-65)
_css(_tkn_x407e)  		_cfc(-65)
_css(_tkn_x4023)  		_cfc(-65)
_csc('>')  			_cfc(-65)
_csc('+')  			_cfc(-65)
_csc('-')  			_cfc(-65)
_csc('*')  			_cfc(-65)
_csc('/')  			_cfc(-65)
_csc('?')  			_cfc(-65)
_css(_tkn_andor)		_cgt(92)
__ 137:
_css(_tkn_string)		_ctc(70)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-61)
_csc(']')  			_cfc(-61)
_csc('}')  			_cfc(-61)
_csc(')')  			_cfc(-61)
_csc('!')  			_cfc(-61)
_css(_tkn_to)  			_cfc(-61)
_csc(',')  			_cfc(-61)
_css(_tkn_x2626)  		_cfc(-61)
_css(_tkn_x7c7c)  		_cfc(-61)
_csc('&')  			_cfc(-61)
_csc('|')  			_cfc(-61)
_csc('<')  			_cfc(-61)
_css(_tkn_x3d3e)  		_cfc(-61)
_css(_tkn_x3c3d)  		_cfc(-61)
_css(_tkn_x3d3d)  		_cfc(-61)
_css(_tkn_x3c3e)  		_cfc(-61)
_css(_tkn_x407e)  		_cfc(-61)
_css(_tkn_x4023)  		_cfc(-61)
_csc('>')  			_cfc(-61)
_csc('+')  			_cfc(-61)
_csc('-')  			_cfc(-61)
_csc('*')  			_cfc(-61)
_csc('/')  			_cfc(-61)
_csc('@')  			_cfc(-61)
_csc('?')  			_cfc(-61)
_css(_tkn_andor)		_cgt(92)
__ 138:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(166)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 139:  			_cfc(-81)
__ 140:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(167)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 141:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('[')			_ctc(5)
_csc('{')			_ctc(6)
_csc('(')			_ctc(7)
_csc('!')			_ctc(8)
_csc('<')			_ctc(9)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(19)
_csc('$')			_ctc(20)
_css(_tkn_stmt)			_cgt(168)
_css(_tkn_ref)			_cgt(24)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(26)
_css(_tkn_assign)		_cgt(27)
_css(_tkn_lvar)			_cgt(28)
_css(_tkn_expr)			_cgt(29)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 142:
_css(_tkn_string)		_ctc(70)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_csc(';')  			_cfc(-75)
_csc(']')  			_cfc(-75)
_csc('}')  			_cfc(-75)
_csc(')')  			_cfc(-75)
_csc('!')  			_cfc(-75)
_css(_tkn_to)  			_cfc(-75)
_csc(',')  			_cfc(-75)
_csc('?')  			_cfc(-75)
_css(_tkn_andor)		_cgt(92)
__ 143:
_csc(')')			_ctc(169)
_csc(',')			_ctc(149)
__ 144:
_css(_tkn_string)		_ctc(70)
_csc(']')			_ctc(170)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 145:
_css(_tkn_brf)			_cgt(171)
  				_cfc(-20)
__ 146:
_csc('[')			_ctc(61)
_csc('=')			_ctc(62)
_css(_tkn_x2b3d)		_ctc(63)
_css(_tkn_x2d3d)		_ctc(64)
_css(_tkn_string)  		_cfc(-55)
_csc('}')  			_cfc(-97)
_csc(')')  			_cfc(-97)
_csc(',')  			_cfc(-97)
_css(_tkn_x2626)  		_cfc(-55)
_css(_tkn_x7c7c)  		_cfc(-55)
_csc('&')  			_cfc(-55)
_csc('|')  			_cfc(-55)
_csc('<')  			_cfc(-55)
_css(_tkn_x3d3e)  		_cfc(-55)
_css(_tkn_x3c3d)  		_cfc(-55)
_css(_tkn_x3d3d)  		_cfc(-55)
_css(_tkn_x3c3e)  		_cfc(-55)
_css(_tkn_x407e)  		_cfc(-55)
_css(_tkn_x4023)  		_cfc(-55)
_csc('>')  			_cfc(-55)
_csc('+')  			_cfc(-55)
_csc('-')  			_cfc(-55)
_csc('*')  			_cfc(-55)
_csc('/')  			_cfc(-55)
_csc('@')  			_cfc(-55)
_csc(':')  			_cfc(-55)
_csc('.')  			_cfc(-55)
__ 147:
_csc('[')			_ctc(66)
_css(_tkn_string)  		_cfc(-57)
_csc('}')  			_cfc(-98)
_csc(')')  			_cfc(-98)
_csc(',')  			_cfc(-98)
_css(_tkn_x2626)  		_cfc(-57)
_css(_tkn_x7c7c)  		_cfc(-57)
_csc('&')  			_cfc(-57)
_csc('|')  			_cfc(-57)
_csc('<')  			_cfc(-57)
_css(_tkn_x3d3e)  		_cfc(-57)
_css(_tkn_x3c3d)  		_cfc(-57)
_css(_tkn_x3d3d)  		_cfc(-57)
_css(_tkn_x3c3e)  		_cfc(-57)
_css(_tkn_x407e)  		_cfc(-57)
_css(_tkn_x4023)  		_cfc(-57)
_csc('>')  			_cfc(-57)
_csc('+')  			_cfc(-57)
_csc('-')  			_cfc(-57)
_csc('*')  			_cfc(-57)
_csc('/')  			_cfc(-57)
_csc('@')  			_cfc(-57)
_csc(':')  			_cfc(-57)
_csc('.')  			_cfc(-57)
__ 148:   			_cfc(-59)
__ 149:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('+')			_ctc(98)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(100)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_aryexpr)		_cgt(172)
__ 150:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(173)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 151:  			_cfc(-33)
__ 152:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(174)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 153:   			_cfc(-34)
__ 154:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(175)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 155:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('+')			_ctc(98)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_csc(')')  			_cfc(-94)
_csc(',')  			_cfc(-94)
_css(_tkn_array)		_cgt(176)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(100)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_aryexpr)		_cgt(101)
__ 156:   			_cfc(-85)
__ 157: _css(_tkn_xxb)		_cgt(177)
	  			_cfc(-37)
__ 158:
_csc('(')			_ctc(178)
_css(_tkn_string)  		_cfc(-87)
_csc(';')  			_cfc(-87)
_csc('[')  			_cfc(-87)
_csc(']')  			_cfc(-87)
_csc('}')  			_cfc(-87)
_csc(')')  			_cfc(-87)
_csc('!')  			_cfc(-87)
_css(_tkn_to)  			_cfc(-87)
_csc('=')  			_cfc(-87)
_css(_tkn_x2b3d)  		_cfc(-87)
_css(_tkn_x2d3d)  		_cfc(-87)
_csc(',')  			_cfc(-87)
_css(_tkn_x2626)  		_cfc(-87)
_css(_tkn_x7c7c)  		_cfc(-87)
_csc('&')  			_cfc(-87)
_csc('|')  			_cfc(-87)
_csc('<')  			_cfc(-87)
_css(_tkn_x3d3e)  		_cfc(-87)
_css(_tkn_x3c3d)  		_cfc(-87)
_css(_tkn_x3d3d)  		_cfc(-87)
_css(_tkn_x3c3e)  		_cfc(-87)
_css(_tkn_x407e)  		_cfc(-87)
_css(_tkn_x4023)  		_cfc(-87)
_csc('>')  			_cfc(-87)
_csc('+')  			_cfc(-87)
_csc('-')  			_cfc(-87)
_csc('*')  			_cfc(-87)
_csc('/')  			_cfc(-87)
_csc('@')  			_cfc(-87)
_csc(':')  			_cfc(-87)
_csc('.')  			_cfc(-87)
_csc('?')  			_cfc(-87)
__ 159:   			_cfc(-93)
__ 160:   			_cfc(-90)
__ 161:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(179)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 162:   			_cfc(-47)
__ 163:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(180)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 164:
_css(_tkn_string)		_ctc(70)
_csc(')')			_ctc(181)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 165:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('+')			_ctc(98)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_csc(')')  			_cfc(-94)
_csc(',')  			_cfc(-94)
_css(_tkn_array)		_cgt(182)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(100)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_aryexpr)		_cgt(101)
__ 166:
_css(_tkn_string)		_ctc(70)
_csc(')')			_ctc(183)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 167:
_css(_tkn_string)		_ctc(70)
_csc(')')			_ctc(184)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 168:
_csc(',')			_ctc(185)
_css(_tkn_end)  		_cfc(-22)
_css(_tkn_number)  		_cfc(-22)
_css(_tkn_string)  		_cfc(-22)
_css(_tkn_text)  		_cfc(-22)
_csc('[')  			_cfc(-22)
_csc('{')  			_cfc(-22)
_csc('}')  			_cfc(-22)
_csc('(')  			_cfc(-22)
_csc('!')  			_cfc(-22)
_csc('<')	 		_cfc(-22)
_css(_tkn_pushr)  		_cfc(-22)
_css(_tkn_popr)  		_cfc(-22)
_css(_tkn_pushl)  		_cfc(-22)
_css(_tkn_popl)  		_cfc(-22)
_css(_tkn_pull)  		_cfc(-22)
_csc('-')  			_cfc(-22)
_csc('~')  			_cfc(-22)
_css(_tkn_x7e7e)  		_cfc(-22)
_csc(':')  			_cfc(-22)
_csc('.')  			_cfc(-22)
_csc('$')  			_cfc(-22)
_css(_tkn_pos)			_cgt(186)
_css(_tkn_ascii2c)		_cgt(187)
__ 169:   			_cfc(-36)
__ 170: _css(_tkn_loop1)	_cgt(188)
   				_cfc(-24)
__ 171:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('[')			_ctc(5)
_csc('{')			_ctc(6)
_csc('(')			_ctc(7)
_csc('!')			_ctc(8)
_csc('<')			_ctc(9)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(19)
_csc('$')			_ctc(20)
_css(_tkn_stmt)			_cgt(189)
_css(_tkn_ref)			_cgt(24)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(26)
_css(_tkn_assign)		_cgt(27)
_css(_tkn_lvar)			_cgt(28)
_css(_tkn_expr)			_cgt(29)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 172:  			_cfc(-96)
__ 173:
_css(_tkn_string)		_ctc(70)
_csc(')')			_ctc(190)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 174:
_css(_tkn_string)		_ctc(70)
_csc(')')			_ctc(191)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 175:
_css(_tkn_string)		_ctc(70)
_csc(')')			_ctc(192)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 176:
_csc(')')			_ctc(193)
_csc(',')			_ctc(149)
__ 177:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('+')			_ctc(98)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_csc(')')  			_cfc(-94)
_csc(',')  			_cfc(-94)
_css(_tkn_array)		_cgt(194)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(100)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_aryexpr)		_cgt(101)
__ 178: _css(_tkn_xxc)		_cgt(195)
  				_cfc(-39)
__ 179:
_css(_tkn_string)		_ctc(70)
_csc(']')			_ctc(196)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 180:
_css(_tkn_string)		_ctc(70)
_csc(']')			_ctc(197)
_css(_tkn_x2626)		_ctc(72)
_css(_tkn_x7c7c)		_ctc(73)
_csc('&')			_ctc(74)
_csc('|')			_ctc(75)
_csc('<')			_ctc(76)
_css(_tkn_x3d3e)		_ctc(77)
_css(_tkn_x3c3d)		_ctc(78)
_css(_tkn_x3d3d)		_ctc(79)
_css(_tkn_x3c3e)		_ctc(80)
_css(_tkn_x407e)		_ctc(81)
_css(_tkn_x4023)		_ctc(82)
_csc('>')			_ctc(83)
_csc('+')			_ctc(84)
_csc('-')			_ctc(85)
_csc('*')			_ctc(86)
_csc('/')			_ctc(87)
_csc('@')			_ctc(88)
_csc(':')			_ctc(89)
_csc('.')			_ctc(90)
_css(_tkn_andor)		_cgt(92)
__ 181:  			_cfc(-82)
__ 182:
_csc(')')			_ctc(198)
_csc(',')			_ctc(149)
__ 183:
_csc('(')			_ctc(199)
__ 184:  			_cfc(-83)
__ 185:  			_cfc(-25)
__ 186:  			_cfc(-7)
__ 187:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('[')			_ctc(5)
_csc('{')			_ctc(6)
_csc('(')			_ctc(7)
_csc('!')			_ctc(8)
_csc('<')			_ctc(9)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(19)
_csc('$')			_ctc(20)
_css(_tkn_stmt)			_cgt(200)
_css(_tkn_ref)			_cgt(24)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(26)
_css(_tkn_assign)		_cgt(27)
_css(_tkn_lvar)			_cgt(28)
_css(_tkn_expr)			_cgt(29)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 188:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('[')			_ctc(5)
_csc('{')			_ctc(6)
_csc('(')			_ctc(7)
_csc('!')			_ctc(8)
_csc('<')			_ctc(9)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(19)
_csc('$')			_ctc(20)
_css(_tkn_stmt)			_cgt(201)
_css(_tkn_ref)			_cgt(24)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(26)
_css(_tkn_assign)		_cgt(27)
_css(_tkn_lvar)			_cgt(28)
_css(_tkn_expr)			_cgt(29)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
__ 189: _css(_tkn_bra)		_cgt(202)
	  			_cfc(-21)
__ 190:   			_cfc(-31)
__ 191:   			_cfc(-32)
__ 192:  			_cfc(-30)
__ 193:  			_cfc(-42)
__ 194:
_csc(')')			_ctc(203)
_csc(',')			_ctc(149)
__ 195:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('+')			_ctc(98)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_csc(')')  			_cfc(-94)
_csc(',')  			_cfc(-94)
_css(_tkn_array)		_cgt(204)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(100)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_aryexpr)		_cgt(101)
__ 196:  			_cfc(-91)
__ 197:  			_cfc(-48)
__ 198:  			_cfc(-44)
__ 199: _css(_tkn_xxf)		_cgt(205)
  				_cfc(-45)
__ 200: _css(_tkn_pos)		_cgt(206)
 				_cfc(-22)
__ 201: _css(_tkn_bra)		_cgt(207)
  				_cfc(-21)
__ 202: _css(_tkn_endloop)	_cgt(208)
  				_cfc(-26)
__ 203:  			_cfc(-38)
__ 204:
_csc(')')			_ctc(209)
_csc(',')			_ctc(149)
__ 205:
_css(_tkn_number)		_ctc(2)
_css(_tkn_string)		_ctc(3)
_css(_tkn_text)			_ctc(4)
_csc('{')			_ctc(37)
_csc('(')			_ctc(7)
_css(_tkn_pushr)		_ctc(10)
_css(_tkn_popr)			_ctc(11)
_css(_tkn_pushl)		_ctc(12)
_css(_tkn_popl)			_ctc(13)
_css(_tkn_pull)			_ctc(14)
_csc('+')			_ctc(98)
_csc('-')			_ctc(15)
_csc('~')			_ctc(16)
_css(_tkn_x7e7e)		_ctc(17)
_csc(':')			_ctc(18)
_csc('.')			_ctc(38)
_csc('$')			_ctc(20)
_csc(')')  			_cfc(-94)
_csc(',')  			_cfc(-94)
_css(_tkn_array)		_cgt(210)
_css(_tkn_var)			_cgt(25)
_css(_tkn_func)			_cgt(39)
_css(_tkn_assign)		_cgt(40)
_css(_tkn_lvar)			_cgt(41)
_css(_tkn_expr)			_cgt(100)
_css(_tkn_ovar)			_cgt(30)
_css(_tkn_str)			_cgt(31)
_css(_tkn_aryexpr)		_cgt(101)
__ 206:  			_cfc(-8)
__ 207: _css(_tkn_endloop)	_cgt(211)
  				_cfc(-26)
__ 208: _css(_tkn_pos)		_cgt(212)
  				_cfc(-22)
__ 209:  			_cfc(-40)
__ 210:
_csc(')')			_ctc(213)
_csc(',')			_ctc(149)
__ 211:  			_cfc(-11)
__ 212:  			_cfc(-12)
__ 213:  			_cfc(-46)

__  -1:				// 1  begin :
#ifdef DEBUG
if( _->precursor != _tkn_end ) _asm int 3 ;
#endif
destruct( _ ) ;
_= tkn( _tkn_begin , atominul ) ;
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_RETURN0 ;
INST1(.opcode=C_RETURN0)
return _ ;

__  -2:				// 2        | _stmtlist
_cft( 0,_tkn_stmtlist)
#ifdef DEBUG
if( _->precursor != _tkn_end ) _asm int 3 ;
#endif
interatomize( _j(0) , _= tkn( _tkn_begin , _j(0) ) ) ;
INST1(.opcode=C_RETURN0)
_crg_0 ;

__  -3:				// 3        | error
_cft( 0,_tkn_error)
//_=_j(0) ;
_rrr();

__  -4:				// 4  _stmtlist : _stmt
_cft( 0,_tkn_stmt)
interatomize( _j(0) , _= tkn( _tkn_stmtlist , _j(0) ) ) ;
_->datum = (void*) _j(0) ;
_crg_0 ;

__  -5:				// 5            | _stmtlist _stmt
_cft(-1,_tkn_stmtlist)
_cft( 0,_tkn_stmt)
interatomize( a=_t[_z] , _=_j(-1) ) ;
do	a=a->interatoms ;
	while( a->precursor != _tkn_stmt ) ;
a->datum =(void*) _t[_z] ;
_crg_(2) ;

__  -6:				// 6  _expr_stmt_ : '!'
_cft( 0,_tkn_ascii['!'])
_= tkn( _tkn_expr_stmt , _j(0) );
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_RETURN ;
INST1(.opcode=C_RETURN)
_crg1;

__  -7:				// 7              | '?' _brf _stmt _pos
_cft(-3,_tkn_ascii['?'])
_cft(-2,_tkn_brf)
_cft(-1,_tkn_stmt)
_cft( 0,_tkn_pos)
_= tkn( _tkn_expr_stmt , _j(-3) ) ;
//interatomize( _j(0) , _ ) ;
//interatomize( _j(-1) , _ ) ;
//interatomize( _j(-2) , _ ) ;
//interatomize( _j(-3) , _ ) ;
ReCode(_n(-2),PSIZ - _n(-2));
_crg(4);

__  -8:				// 8              | '?' _brf _stmt _ascii2c _stmt _pos
_cft(-5,_tkn_ascii['?'])
_cft(-4,_tkn_brf)
_cft(-3,_tkn_stmt)
_cft(-2,_tkn_ascii2c)
_cft(-1,_tkn_stmt)
_cft( 0,_tkn_pos)
_= tkn( _tkn_expr_stmt , _j(-5) );
//interatomize( _j(0) , _ ) ;
//interatomize( _j(-1) , _ ) ;
//interatomize( _j(-2) , _ ) ;
//interatomize( _j(-3) , _ ) ;
//interatomize( _j(-4) , _ ) ;
//interatomize( _j(-5) , _ ) ;
ReCode(_n(-4),(_n(-2) - _n(-4))+1);
ReCode(_n(-2),PSIZ - _n(-2));
_crg(6);

__  -9:				// 9  _stmt : '!'
_cft( 0,_tkn_ascii['!'])
_= tkn( _tkn_stmt , _j(0) );
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_RETURN0 ;
INST1(.opcode=C_RETURN0);
_crg1;

__ -10:				// 10        | _expr _expr_stmt_
_cft(-1,_tkn_expr)
_cft( 0,_tkn_expr_stmt)
_= tkn( _tkn_stmt , _j(-1) );
//interatomize( _j(0)  , _ ) ;
//interatomize( _j(-1) , _ ) ;
_crg(2);

__ -11:				//        | '[' STRING ';' _expr ']' _loop1 _stmt _bra _endloop
_cft(-8,_tkn_ascii['['])
_cft(-7,_tkn_string)
_cft(-6,_tkn_ascii[';'])
_cft(-5,_tkn_expr)
_cft(-4,_tkn_ascii[']'])
_cft(-3,_tkn_loop1)
_cft(-2,_tkn_stmt)
_cft(-1,_tkn_bra)
_cft( 0,_tkn_endloop)
_= tkn( _tkn_stmt , _j(-8) );
ReCode(_n(-3)+1,Find_Or_Add_Var( _s(-7) ));
ReCode((_n(-3)+2),PSIZ-(_n(-3)+1));
ReCode(_n(-1),_n(-3)-_n(-1));
//if( ! sizeOnly )
//	((atom)_s(-7))->allusions++ ;
_crg(9);

__ -12:				//        | '[' _loop _expr ']' _brf _stmt _bra _endloop _pos
//__ -12:				//        | _loop _expr ']' _brf _stmt _bra _endloop _pos
_cft(-8,_tkn_ascii['['])
_cft(-7,_tkn_loop)
_cft(-6,_tkn_expr)
_cft(-5,_tkn_ascii[']'])
_cft(-4,_tkn_brf)
_cft(-3,_tkn_stmt)
_cft(-2,_tkn_bra)
_cft(-1,_tkn_endloop)
_cft( 0,_tkn_pos)
_= tkn( _tkn_stmt , _j(-8) );
ReCode(_n(-4),PSIZ - _n(-4));
ReCode(_n(-2),_n(-7) - _n(-2));
_crg(9);

__ -13:				//       | '.' ';'
_cft(-1,_tkn_ascii['.'])
_cft( 0,_tkn_ascii[';'])
_= tkn( _tkn_stmt , _j(-1) );
if(numloop<1)
	{
	interatomize( a=atomize( _tkn_code ) , _ ) ;
	a->datum = (void*) C_RETURN0 ;
	INST1(.opcode=C_RETURN0)
	}
else	{
	interatomize( a=atomize( _tkn_code ) , _ ) ;		// UPDATE: store last_i
	a->datum = (void*) C_BREAK ;
	INST2(.opcode=C_BREAK,.i=last_i)
	last_i=Exec->size-2; 
	}
_crg(2);

/*__  -14:			//        | CONTINUE ';'*/
/*_cft(-1,_tkn_continue)*/
__  -14:			//        | '<' ';'
_cft(-1,_tkn_ascii['<'])
_cft( 0,_tkn_ascii[';'])
if(numloop<1)
	{
	_=aError_atomize_a( aToken_sNotInALoop ) ;
//	err_s("CONTINUE not in loop!");
	_rrr();
	}
_= tkn( _tkn_stmt , _j(-1) );
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_CONTINUE ;
INST2(.opcode=C_CONTINUE,.i=last_i)
last_i=Exec->size-2; 
_crg(2);

__ -15:				//        | _ref _lvarlist ';'
_cft(-2,_tkn_ref)
_cft(-1,_tkn_lvarlist)
_cft( 0,_tkn_ascii[';'])
_= tkn( _tkn_stmt , _j(-2) );
ReCode(_n(-2),PSIZ-_n(-2));
_crg(3);

__ -16:				//        | assign ';'
_cft(-1,_tkn_assign)
_cft( 0,_tkn_ascii[';'])
_= tkn( _tkn_stmt , _j(-1) );
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_POP ;
INST1(.opcode=C_POP);
_crg(2);

__ -17:				//        | _func ';'
_cft(-1,_tkn_func)
_cft( 0,_tkn_ascii[';'])
_= tkn( _tkn_stmt , _j(-1) );
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_POP ;
INST1(.opcode=C_POP);
_crg(2);

__ -18:				//        | '{' _stmtlist '}'
_cft(-2,_tkn_ascii['{'])
_cft(-1,_tkn_stmtlist)
_cft( 0,_tkn_ascii['}'])
_= tkn( _tkn_stmt , _j(-2) );
_crg(3);

__ -19:				//	  _ref : _lvar X2D3E
_cft(-1,_tkn_lvar)
_cft( 0,_tkn_x2d3e)
_= tkn( _tkn_ref , _j(-1) );
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_REF ;
INST3(.opcode=C_REF,.opcode=C_STOP,.i=_n(-1))
_->datum= (void *)(Exec->size-2);
_crg(2);

__ -20:				//  _brf :
_= tkn( _tkn_brf , _j(0) ) ;			// UPDATE: arg2 should be the statement line_p
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_BRF ;
INST2(.opcode=C_BRF,.opcode=C_STOP)
_->datum= (void *)(Exec->size-1);
_crg0;

__ -21:				//  _bra :
_= tkn( _tkn_bra , _j(0) ) ;			// UPDATE: ASSERT validity of _j(0), _z != 0
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_BRA ;
INST2(.opcode=C_BRA,.opcode=C_STOP)
_->datum= (void *)(Exec->size-1);
_crg0;

__ -22:				//  _pos :
_= tkn( _tkn_pos , _j(0) );
_->datum= (char *)(long)Exec->size;
_crg0;

__ -23:				//  _loop : 
// __ -23:				//  _loop : '['
// _cft( 0,_tkn_ascii['['])
_= tkn( _tkn_loop , _j(0) ) ;
_->datum = (char *)(long)Exec->size;
loopBegin(0);
_crg0;

__ -24:				//  _loop1 :
_= tkn( _tkn_loop1 , _j(0) ) ;
INST1(.opcode=C_FOREACH)
loopBegin(1);
_->datum = (char *)(long)Exec->size ;
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_FOREACH ;
INST3(.opcode=C_ARY_ITER,.opcode=C_STOP,.opcode=C_STOP)
_crg0;
	
__ -25:				//  _ascii2c : ','
_cft( 0,_tkn_ascii[','])
_= tkn( _tkn_ascii2c ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_BRA ;
INST2(.opcode=C_BRA,.opcode=C_STOP)
_->datum = (void *)(Exec->size-1);
_crg1;

__ -26:				//  _endloop :
_= tkn( _tkn_endloop ,_j(0));
loopEnd(); 
_crg0;

	;
__ -27:				//  assign : _var '=' _expr
_cft(-2,_tkn_var)
_cft(-1,_tkn_ascii['='])
_cft( 0,_tkn_expr)
_= tkn( _tkn_assign ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_ASSIGN ;
INST1(.opcode=C_ASSIGN)
_crg(3);

__ -28:				//         | _var X2D3D _expr
_cft(-2,_tkn_var)
_cft(-1,_tkn_x2d3d)
_cft( 0,_tkn_expr)
_= tkn( _tkn_assign ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_ASSIGN_ME ;
INST1(.opcode=C_ASSIGN_ME)
_crg(3);

__ -29:				//         | _var X2B3D _expr
_cft(-2,_tkn_var)
_cft(-1,_tkn_x2b3d)
_cft( 0,_tkn_expr)
_= tkn( _tkn_assign ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_ASSIGN_AE ;
INST1(.opcode=C_ASSIGN_AE)
_crg(3);

__ -30:				//         | PULL '(' _var ',' _expr ')'
_cft(-5,_tkn_pull)
_cft(-4,_tkn_ascii['('])
_cft(-3,_tkn_var)
_cft(-2,_tkn_ascii[','])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_assign ,_j(-5));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PULL ;
INST1(.opcode=C_PULL)
_crg(6);

__ -31:				//         | PUSHR '(' _var ',' _expr ')'
_cft(-5,_tkn_pushr)
_cft(-4,_tkn_ascii['('])
_cft(-3,_tkn_var)
_cft(-2,_tkn_ascii[','])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_assign ,_j(-5));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSHR ;
INST1(.opcode=C_PUSHR)
_crg(6);

__ -32:				//         | PUSHL '(' _var ',' _expr ')'
_cft(-5,_tkn_pushl)
_cft(-4,_tkn_ascii['('])
_cft(-3,_tkn_var)
_cft(-2,_tkn_ascii[','])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_assign ,_j(-5));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSHL ;
INST1(.opcode=C_PUSHL)
_crg(6);

__ -33:				//         | POPR '(' _var ')'
_cft(-3,_tkn_popr)
_cft(-2,_tkn_ascii['('])
_cft(-1,_tkn_var)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_assign ,_j(-3));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_POPR ;
INST1(.opcode=C_POPR)
_crg(4);

__ -34:				//         | POPL '(' _var ')'
_cft(-3,_tkn_popl)
_cft(-2,_tkn_ascii['('])
_cft(-1,_tkn_var)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_assign ,_j(-3));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_POPL ;
INST1(.opcode=C_POPL)
_crg(4);

__ -35:
_= tkn( _tkn_xxa ,_j(0));
//INST3(.opcode=C_PUSH_STR,.a=_s(-1),.opcode=C_MAKE_ARY)
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_MAKE_ARY ;
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_STACK ;
INST3(.opcode=C_STACK,.a=_s(-1),.opcode=C_MAKE_ARY)
//---((atom)_s(-1))->allusions++ ;
_crg0;

__ -36:				//  _func : STRING '(' xxa _array ')'
_cft(-4,_tkn_string)
_cft(-3,_tkn_ascii['('])
_cft(-2,_tkn_xxa)
_cft(-1,_tkn_array)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_func ,_j(-4));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_TVERB ;
INST1(.opcode=C_TVERB)
_crg(5);

__ -37:
_= tkn( _tkn_xxb ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_MAKE_ARY ;
INST1(.opcode=C_MAKE_ARY)
_crg0;

__ -38:				//  _func : '$' STRING STRING '(' xxb _array ')'
_cft(-6,_tkn_ascii['$'])
_cft(-5,_tkn_string)
_cft(-4,_tkn_string)
_cft(-3,_tkn_ascii['('])
_cft(-2,_tkn_xxb)
_cft(-1,_tkn_array)
_cft( 0,_tkn_ascii[')'])
if(sizeOnly)
	{
	_= tkn( _tkn_func ,_j(-6));
	Exec->size+=3;	// UPDATE: min 2 : max 3
	_crg(7);
	}
if( ( ti.a = Find_Obj_KeyWord( _d(-5) ) ) )
	{
	_= tkn( _tkn_func ,_j(-6));
	interatomize( a=atomize( _tkn_code ) , _ ) ;
	a->datum = (void*) C_VERB_A_S ;
	INST3(.opcode=C_VERB_A_S,.a=ti.a,.a=_s(-4))
	ti.a->allusions++ ;			// UPDATE: ref++ but how to do ref-- ?
	//---((atom)_s(-4))->allusions++ ;
	_crg(7) ;
	}
_=aError_atomize_a( aToken_sUndefinedKeyword ) ;
//err_s("invalid $keyword");

__ -39:
_= tkn( _tkn_xxc ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_MAKE_ARY ;
INST1(.opcode=C_MAKE_ARY)
_crg0;

__ -40:				//  _func : '$' STRING '.' STRING '(' xxc _array ')'
_cft(-7,_tkn_ascii['$'])
_cft(-6,_tkn_string)
_cft(-5,_tkn_ascii['.'])	// UPDATE: ascii['_']
_cft(-4,_tkn_string)
_cft(-3,_tkn_ascii['('])
_cft(-2,_tkn_xxc)
_cft(-1,_tkn_array)
_cft( 0,_tkn_ascii[')'])
if(sizeOnly) {
	_= tkn( _tkn_func ,_j(-7));
	Exec->size+=2;
	_crg(8);
	}
ti.a = (atom)Find_Obj_KeyWord( _d(-6) );
if( ti.a )
	{
	extern atom	aMethod ;
	atom		oo		= ti.a ;
	atom		o ;
	string		n		= _d(-4) ;

	oo	= ti.a ;
//	while( oo )	// UPDATE: only works if aMethod is directly connected to A
//		{
		o = oo->interatom;
		while(o)
			{
			if(o->precursor == aMethod)
				{
				if( cmp_s(((method)o->datum)->name,n) == 0 )
					{
					_= tkn( _tkn_func ,_j(-7));
					interatomize( a=atomize( _tkn_code ) , _ ) ;
					a->datum = (void*) C_OBJ_FUNC ;
					Exec->linep[Exec->size] = line_p ;
					Exec->exec[Exec->size++].opcode = C_OBJ_FUNC ;
					Exec->linep[Exec->size] = line_p ;
					( Exec->exec[Exec->size++].a	= o )->allusions++ ;
					//o->allusions++ ;
					_crg(8) ;
					}
				}
			o = o->interatoms;
			}
//		oo = oo->precursor;
//		}
//	if(Code_Obj_Func(ti.a, _d(-4)))	
//		{
//		_crg(8);
//		}
	_=aError_atomize_a( aToken_sUndefinedProcedure ) ;
//	err_s("function not known");
	}
else	{
	_=aError_atomize_a( aToken_sUndefinedKeyword ) ;
//	err_s("invalid $keyword");
	}

__ -41:
_= tkn( _tkn_xxd ,_j(0));
//INST3(.opcode=C_PUSH_STR,.a=_s(-1),.opcode=C_MAKE_ARY)
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_MAKE_ARY ;
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_STACK ;
INST3(.opcode=C_STACK,.a=_s(-1),.opcode=C_MAKE_ARY)
//if( ! sizeOnly )
//	((atom)_s(-1))->allusions++ ;
_crg0;

__ -42:				// _func : ':' STRING '(' xxd _array ')'
_cft(-5,_tkn_ascii[':'])
_cft(-4,_tkn_string)
_cft(-3,_tkn_ascii['('])
_cft(-2,_tkn_xxd)
_cft(-1,_tkn_array)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_func ,_j(-5));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_LVERB ;
INST1(.opcode=C_LVERB)
_crg(6)

__ -43:
_= tkn( _tkn_xxe ,_j(0));
//INST3(.opcode=C_PUSH_STR,.a=_s(-1),.opcode=C_MAKE_ARY)
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_MAKE_ARY ;
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_STACK ;
INST3(.opcode=C_STACK,.a=_s(-1),.opcode=C_MAKE_ARY)
//---if( ! sizeOnly )
//---	((atom)_s(-1))->allusions++ ;
_crg0;

__ -44:				//  _func : _expr STRING '(' xxe _array ')'
_cft(-5,_tkn_expr)
_cft(-4,_tkn_string)
_cft(-3,_tkn_ascii['('])
_cft(-2,_tkn_xxe)
_cft(-1,_tkn_array)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_func ,_j(-5));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_VERB ;
INST1(.opcode=C_VERB)
_crg(6);

__ -45:
_= tkn( _tkn_xxf ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_MAKE_ARY ;
INST1(.opcode=C_MAKE_ARY)
_crg0;

__ -46:				//  _func : _expr ':' '(' _expr ')' '(' xxf _array ')'
_cft(-8,_tkn_expr)
_cft(-7,_tkn_ascii[':'])
_cft(-6,_tkn_ascii['('])
_cft(-5,_tkn_expr)
_cft(-4,_tkn_ascii[')'])
_cft(-3,_tkn_ascii['('])
_cft(-2,_tkn_xxf)
_cft(-1,_tkn_array)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_func ,_j(-8));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_VERB ;
INST1(.opcode=C_VERB)
_crg(9);

__ -47:				//        | _func '[' _expr ']'
_cft(-3,_tkn_func)
_cft(-2,_tkn_ascii['['])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[']'])
_= tkn( _tkn_func ,_j(0-3));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_INDEX ;
INST1(.opcode=C_INDEX)
_crg(4);

__ -48:				//        | _func '[' _expr TO _expr ']'
_cft(-5,_tkn_func)
_cft(-4,_tkn_ascii['['])
_cft(-3,_tkn_expr)
_cft(-2,_tkn_to)
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[']'])
_= tkn( _tkn_func ,_j(-5));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_SPLICE ;
INST1(.opcode=C_SPLICE)
_crg(6);

__ -49:				//  _expr : '-' _expr
_cft(-1,_tkn_ascii['-'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-1));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_NEGATE ;
INST1(.opcode=C_NEGATE)
_crg(2);

__ -50:				//        | '~' _expr
_cft(-1,_tkn_ascii['~'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-1));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_BNEGATE ;
INST1(.opcode=C_BNEGATE)
_crg(2);

__ -51:				//        | X7E7E _expr
_cft(-1,_tkn_x7e7e)
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-1));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_NOT ;
INST1(.opcode=C_NOT)
_crg(2);

__ -52:				//        | '(' _expr ')'
_cft(-2,_tkn_ascii['('])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_expr ,_j(-2));
_->datum = (char *)_n(-1);
_crg(3);

__ -53:				//        | NUMBER
_cft( 0,_tkn_number)
_= tkn( _tkn_expr ,_j(0));
//INST2(.opcode=C_PUSH_NUM,.a=_s(0))
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_STACK ;
INST2(.opcode=C_STACK,.a=_s(0))
//((atom)_s(0))->allusions++ ;
_crg1;

__ -54:				//        | TEXT
_cft( 0,_tkn_text)
_= tkn( _tkn_expr ,_j(0));
//INST2(.opcode=C_PUSH_STR,.a=_s(0))
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_STACK ;
INST2(.opcode=C_STACK,.a=_s(0))
//---if( ! sizeOnly )
//---	((atom)_s(0))->allusions++ ;
_crg1;

__ -55:				//        | _var
_cft( 0,_tkn_var)
_= tkn( _tkn_expr ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_EVAL ;
INST1(.opcode=C_EVAL)
_crg1;

__ -56:				//        | assign
_cft( 0,_tkn_assign)
_= tkn( _tkn_expr ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_EVAL ;
INST1(.opcode=C_EVAL)
_crg1;

__ -57:				//        | _func
_cft( 0,_tkn_func)
_= tkn( _tkn_expr ,_j(0));
_crg1;

__ -58:
_= tkn( _tkn_xxg , _j(0) );
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_MAKE_ARY ;
INST1(.opcode=C_MAKE_ARY)
_crg0;

__ -59:				//  _expr : '{' xxg _array '}'
_cft(-3,_tkn_ascii['{'])
_cft(-2,_tkn_xxg)
_cft(-1,_tkn_array)
_cft( 0,_tkn_ascii['}'])
_= tkn( _tkn_expr ,_j(-3));
_crg(4);

__ -60:				//        | _expr X407E _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_x407e)
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_ISNOT ;
INST1(.opcode=C_ISNOT)
_crg(3);

__ -61:				//        | _expr '@' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['@'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_IS ;
INST1(.opcode=C_IS)
_crg(3);

__ -62:				//        | _expr X4023 _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_x4023)
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_SEARCH ;
INST1(.opcode=C_SEARCH)
_crg(3);

__ -63:				//        | _expr '-' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['-'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_SUB ;
INST1(.opcode=C_SUB)
_crg(3);

__ -64:				//        | _expr '+' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['+'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_ADD ;
INST1(.opcode=C_ADD)
_crg(3);

__ -65:				//        | _expr '/' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['/'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_DIV ;
INST1(.opcode=C_DIV)
_crg(3);

__ -66:				//        | _expr '*' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['*'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_MUL ;
INST1(.opcode=C_MUL);
_crg(3);

__ -67:				//        | _expr '&' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['&'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_BAND ;
INST1(.opcode=C_BAND);
_crg(3);

__ -68:				//        | _expr '|' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['|'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_BOR ;
INST1(.opcode=C_BOR);
_crg(3);

__ -69:				//        | _expr X3D3D _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_x3d3d)
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_EQ ;
INST1(.opcode=C_EQ);
_crg(3);

__ -70:				//        | _expr X3C3E _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_x3c3e)
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_NE ;
INST1(.opcode=C_NE);
_crg(3);

__ -71:				//        | _expr '>' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['>'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_GT ;
INST1(.opcode=C_GT);
_crg(3);

__ -72:				//        | _expr '<' _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['<'])
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_LT ;
INST1(.opcode=C_LT);
_crg(3);

__ -73:				//        | _expr X3D3E _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_x3d3e)
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_GE ;
INST1(.opcode=C_GE);
_crg(3);

__ -74:				//        | _expr X3C3D _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_x3c3d)
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_LE ;
INST1(.opcode=C_LE);
_crg(3);

__ -75:				//        | _expr _andor _expr
_cft(-2,_tkn_expr)
_cft(-1,_tkn_andor)
_cft( 0,_tkn_expr)
_= tkn( _tkn_expr ,_j(-2));
ReCode(_n(-1),PSIZ-_n(-1));
_crg(3);

__ -76:				//  _andor : X2626
_cft( 0,_tkn_x2626)
_= tkn( _tkn_andor ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_AND ;
INST2(.opcode=C_AND,.opcode=C_STOP)
_->datum= (void *)(Exec->size-1);
_crg1;

__ -77:				//         | X7C7C
_cft( 0,_tkn_x7c7c)
_= tkn( _tkn_andor ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_OR ;
INST2(.opcode=C_OR,.opcode=C_STOP)
_->datum= (char *)(Exec->size-1);
_crg1;

__ -78:				//  _lvar : _string
_cft( 0,_tkn_str)
_= tkn( _tkn_lvar ,_j(0));
_->datum= (void*)_n(0);
_crg1;

__ -79:				//  _string : STRING
_cft( 0,_tkn_string)
_= tkn( _tkn_str ,_j(0));
_->datum= (void*) (long) Find_Or_Add_Var( _s(0) );
//---if( ! sizeOnly )
//---	((atom)_s(0))->allusions++ ;
_crg1;

__ -80:				//  _ovar : _lvar '.' STRING
_cft(-2,_tkn_lvar)
_cft(-1,_tkn_ascii['.'])
_cft( 0,_tkn_string)
_= tkn( _tkn_ovar ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSH_OVVS ;
INST3(.opcode=C_PUSH_OVVS,.i=_n(-2),.a=_s(0))
//if( ! sizeOnly )
//	((atom)_s(0))->allusions++ ;
_crg(3);

__ -81:				//        | _expr '.' STRING
_cft(-2,_tkn_expr)
_cft(-1,_tkn_ascii['.'])
_cft( 0,_tkn_string)
_= tkn( _tkn_ovar ,_j(-2));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSH_OV ;
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_STACK ;
//interatomize( _j(0) , a ) ;
//interatomize( _j(-2) , _ ) ;
//INST3(.opcode=C_PUSH_STR,.a=_s(0),.opcode=C_PUSH_OV)
INST3(.opcode=C_STACK,.a=_s(0),.opcode=C_PUSH_OV)
//---if( ! sizeOnly )
//---	((atom)_s(0))->allusions++ ;
//_cds(-1) ;
_crg(3);

__ -82:				//        | _lvar '.' '(' _expr ')'
_cft(-4,_tkn_lvar)
_cft(-3,_tkn_ascii['.'])
_cft(-2,_tkn_ascii['('])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_ovar ,_j(-4));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSH_OVV ;
INST2(.opcode=C_PUSH_OVV,.i=_n(-4))
_crg(5);

__ -83:				//        | _expr '.' '(' _expr ')'
_cft(-4,_tkn_expr)
_cft(-3,_tkn_ascii['.'])
_cft(-2,_tkn_ascii['('])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_ovar ,_j(-4));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSH_OV ;
INST1(.opcode=C_PUSH_OV);
_crg(5);

__ -84:				//        | '.' STRING
_cft(-1,_tkn_ascii['.'])
_cft( 0,_tkn_string)
_= tkn( _tkn_ovar ,_j(-1));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSH_OVTS ;
INST2(.opcode=C_PUSH_OVTS,.a=_s(0))
//if( ! sizeOnly )
//	((atom)_s(0))->allusions++ ;
_crg(2);

__ -85:				//        | '.' '(' _expr ')'
_cft(-3,_tkn_ascii['.'])
_cft(-2,_tkn_ascii['('])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[')'])
_= tkn( _tkn_ovar ,_j(-3));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSH_OVT ;
INST1(.opcode=C_PUSH_OVT);
_crg(4);

__ -86:				//        | '$' STRING
_cft(-1,_tkn_ascii['$'])
_cft( 0,_tkn_string)
if(sizeOnly) {
	_= tkn( _tkn_ovar ,_j(-1));
	Exec->size+=2;
	_crg(2);
	}
ti.a = (atom)Find_Obj_KeyWord( _d(0) );
if( ti.a )
	{
	_= tkn( _tkn_ovar ,_j(-1));
	//INST2(.opcode=C_PUSH_OBJ,.a=ti.a)
	interatomize( a=atomize( _tkn_code ) , _ ) ;
	a->datum = (void*) C_STACK ;
	INST2(.opcode=C_STACK,.a=ti.a) ;
	ti.a->allusions++ ;			// UPDATE: ref++ but how to do ref-- ?
	_crg(2);
	}
_=aError_atomize_a( aToken_sUndefinedKeyword ) ;
//err_s("Invalid $keyword");

__ -87:				//        | '$' STRING '.' STRING
_cft(-3,_tkn_ascii['$'])
_cft(-2,_tkn_string)
_cft(-1,_tkn_ascii['.'])
_cft( 0,_tkn_string)
_= tkn( _tkn_ovar ,_j(-3));
if(sizeOnly) {
	Exec->size+=3;	// UPDATE: min 2 : max 3
	_crg(4);
	}
if( ( ti.a = (atom)Find_Obj_KeyWord( _d(-2) ) ) )
	{
	if( Code_Obj_Var(ti.a,_d(0)) )
		{
		interatomize( a=atomize( _tkn_code ) , _ ) ;
		a->datum = (void*) C_STACK ;
		_crg(4);
		}
	else	{
		interatomize( a=atomize( _tkn_code ) , _ ) ;
		a->datum = (void*) C_PUSH_OV_A_S ;
		INST3(.opcode=C_PUSH_OV_A_S,.a=ti.a,.a=_s(0))
		ti.a->allusions++ ;			// UPDATE: ref++ but how to do ref-- ?
		//((atom)_s(0))->allusions++ ;
		_crg(4) ;
		}
	}
_=aError_atomize_a( aToken_sUndefinedKeyword ) ;
//err_s("invalid $keyword");

__ -88:				//  _var : _lvar
_cft( 0,_tkn_lvar)
_= tkn( _tkn_var ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSH_VAR ;
INST2(.opcode=C_PUSH_VAR,.i=_n(0))
_crg1;

__ -89:				//       | _ovar
_cft( 0,_tkn_ovar)
_= tkn( _tkn_var ,_j(0));
_crg1;

__ -90:				//       | _var '[' _expr ']'
_cft(-3,_tkn_var)
_cft(-2,_tkn_ascii['['])
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[']'])
_= tkn( _tkn_var ,_j(-3));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_INDEX ;
INST1(.opcode=C_INDEX);
_crg(4);

__ -91:				//       | _var '[' _expr TO _expr ']'
_cft(-5,_tkn_var)
_cft(-4,_tkn_ascii['['])
_cft(-3,_tkn_expr)
_cft(-2,_tkn_to)
_cft(-1,_tkn_expr)
_cft( 0,_tkn_ascii[']'])
_= tkn( _tkn_var ,_j(-5));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_SPLICE ;
INST1(.opcode=C_SPLICE);
_crg(6);

__ -92:				//  _lvarlist : _lvar
_cft( 0,_tkn_lvar)
_= tkn( _tkn_lvarlist ,_j(0));
_->datum= (char *)1;
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) _n(0) ;
INST1(.i=_n(0));
_crg1;

__ -93:				//            | _lvarlist ',' _lvar
_cft(-2,_tkn_lvarlist)
_cft(-1,_tkn_ascii[','])
_cft( 0,_tkn_lvar)
_= tkn( _tkn_lvarlist ,_j(-2));
_->datum= (char *)_n(-2)+1;
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) _n(0) ;
INST1(.i=_n(0));
_crg(3);

__ -94:				//  _array :
_= tkn( _tkn_array ,_j(0));
_crg0;

__ -95:				//         | aryexpr
_cft( 0,_tkn_aryexpr)
_= tkn( _tkn_array ,_j(0));
_crg1;

__ -96:				//         | _array ',' aryexpr
_cft(-2,_tkn_array)
_cft(-1,_tkn_ascii[','])
_cft( 0,_tkn_aryexpr)
_= tkn( _tkn_array ,_j(-2));
_crg(3);

__ -97:				//  aryexpr : '+' _var
_cft(-1,_tkn_ascii['+'])
_cft( 0,_tkn_var)
_= tkn( _tkn_aryexpr ,_j(-1));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_SPLIT ;
INST1(.opcode=C_SPLIT);
_crg(2);

__ -98:				//          | '+' _func
_cft(-1,_tkn_ascii['+'])
_cft( 0,_tkn_func)
_= tkn( _tkn_aryexpr ,_j(-1));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_SPLIT ;
INST1(.opcode=C_SPLIT);
_crg(2);

__ -99:				//          | _expr
_cft( 0,_tkn_expr)
_= tkn( _tkn_aryexpr ,_j(0));
interatomize( a=atomize( _tkn_code ) , _ ) ;
a->datum = (void*) C_PUSH_ON_ARY ;
INST1(.opcode=C_PUSH_ON_ARY);
_crg1;

__ -100:
ntkn = _;
_ = _t[_z];
loopBegin(0);
_t[_z] = tkn ( _tkn_loop ,_j(0));
_t[_z]->datum= (char *)(long)Exec->size;
_0 = _n[_y] = 34;
goto _rd;
_____

_rr:
a = _ ;
while( a && ( a = a->precursor ) != aError ) ;
if( a )	{
	a = atomize( _tkn_error ) ;
	interatomize( a , _ ) ;
	_ = a ;
	}
if( _->precursor != _tkn_error )
	{
	a = aError_atomize_a( aToken_sSyntax ) ;
	b = atomize( _tkn_stack ) ;
	b->datum = (void*) -1 ;
        interatomize( _ , b ) ;
	_ = atomize( _tkn_error ) ;
	interatomize( b , _ ) ;
	interatomize( a , _ ) ;
	}
if( ntkn )
	{
	b = atomize( _tkn_stack ) ;
	b->datum = (void*) -2 ;
	interatomize( ntkn , _ ) ;
	}
#ifdef DEBUG
if( _z != _y-1 ) _asm int 3 ;			// UPDATE: would _z != _y
#endif
while( _z >= 0 )
	{
	b = atomize( _tkn_stack ) ;
	b->datum = (void*) _n[_y--] ;
	interatomize( _t[_z--] , b ) ;
	interatomize( b , _ ) ;
	}
return _ ;

_rt:
#ifdef DEBUG
	if( _->precursor != _tkn_end ) _asm int 3 ;	// UPDATE: does not always == _tkn_end when _0 == -1
	if( ntkn ) _asm int 3 ;
	if( _z > 0 ) _asm int 3 ;		// UPDATE: would it happen ?
#endif
destruct( _ ) ;
return *_t ;
}

/*
void err_s( string s )
	{
	extern void		(*output)(string) ;
	int			i ;
	char			z[100] ;

	errorToggle = 1 ;
	if(Exec && Exec->prog && Exec->prog->name) 
		{
		output( "ERROR: <" ) ;
		output( Exec->prog->name ) ;
		output( "> " ) ;
		output( s ) ;
		output( "\n" ) ;
		}
	else	{
		output( "ERROR: " ) ;
		output( s ) ;
		output( "\n" ) ;
		}
	output( _ltoa( lineno , z , 10 ) ) ;
	output( ") " ) ;
	for( i=0 ; line_p[i] && line_p[i] != '\n' && i < 99 ; i++ ) 
			z[i] = line_p[i] ;
	z[i] = 0 ;
	output( z ) ;
	output( "\n" ) ;
	}
*/

/***************************************
*
*******************/


string pptkn( atom tkn )
{
int		x;
static char	buf[200];

if(tkn==	_tkn_error		) return("error   ");
if(tkn==	_tkn_number		) return("number  ");
if(tkn==	_tkn_string		) return("string  ");
if(tkn==	_tkn_str		) return("str     ");
if(tkn==	_tkn_text		) return("text    ");
if(tkn==	_tkn_continue		) return("continu ");
if(tkn==	_tkn_pull		) return("pull    ");
if(tkn==	_tkn_pushr		) return("pushr   ");
if(tkn==	_tkn_pushl		) return("pushl   ");
if(tkn==	_tkn_popr		) return("popr    ");
if(tkn==	_tkn_popl		) return("popl    ");
if(tkn==	_tkn_begin		) return("begin   ");
if(tkn==	_tkn_stmt		) return("stmt    ");
if(tkn==	_tkn_stmtlist		) return("stmtlst ");
if(tkn==	_tkn_ref		) return("ref     ");
if(tkn==	_tkn_var		) return("var     ");
if(tkn==	_tkn_func		) return("func    ");
if(tkn==	_tkn_assign		) return("assign  ");
if(tkn==	_tkn_lvar		) return("lvar    ");
if(tkn==	_tkn_ovar		) return("ovar    ");
if(tkn==	_tkn_expr		) return("expr    ");
if(tkn==	_tkn_end		) return("end     ");
if(tkn==	_tkn_to			) return("to      ");
if(tkn==	_tkn_x2b3d		) return("x2b3d   ");
if(tkn==	_tkn_x2d3d		) return("x2d3d   ");
if(tkn==	_tkn_x2d3e		) return("x2d3e   ");
if(tkn==	_tkn_x2626		) return("x2626   ");
if(tkn==	_tkn_x3636		) return("x3636   ");
if(tkn==	_tkn_x3d3d		) return("x3d3d   ");
if(tkn==	_tkn_x3d3e		) return("x3d3e   ");
if(tkn==	_tkn_x3c3d		) return("x3c3d   ");
if(tkn==	_tkn_x3c3e		) return("x3c3e   ");
if(tkn==	_tkn_x407e		) return("x407e   ");
if(tkn==	_tkn_x4023		) return("x4023   ");
if(tkn==	_tkn_x7c7c		) return("x7c7c   ");
if(tkn==	_tkn_x7e7e		) return("x7e7e   ");
if(tkn==	_tkn_loop		) return("loop    ");
if(tkn==	_tkn_loop1		) return("loop1   ");
if(tkn==	_tkn_lvarlist		) return("lvrlst  ");
if(tkn==	_tkn_andor		) return("andor   ");
if(tkn==	_tkn_expr_stmt		) return("xprstmt ");
if(tkn==	_tkn_array		) return("array   ");
if(tkn==	_tkn_aryexpr		) return("aryexpr ");
if(tkn==	_tkn_xxa		) return("xxa     ");
if(tkn==	_tkn_xxb		) return("xxb     ");
if(tkn==	_tkn_xxc		) return("xxc     ");
if(tkn==	_tkn_xxd		) return("xxd     ");
if(tkn==	_tkn_xxe		) return("xxe     ");
if(tkn==	_tkn_xxf		) return("xxf     ");
if(tkn==	_tkn_xxg		) return("xxg     ");
if(tkn==	_tkn_brf		) return("brf     ");
if(tkn==	_tkn_pos		) return("pos     ");
if(tkn==	_tkn_ascii2c		) return("ascii2c ");
if(tkn==	_tkn_bra		) return("br      ");
if(tkn==	_tkn_endloop		) return("ndlp    ");

for(x=0;x<128;x++)
	if(tkn== _tkn_ascii[x])
		{
		buf[0] = '\'' ;
		buf[1] = x ;
		buf[2] = '\'' ;
		buf[3] = ' ' ;
		buf[4] = ' ' ;
		buf[5] = ' ' ;
		buf[6] = ' ' ;
		buf[7] = ' ' ;
		return buf;
		}

#ifdef DEBUG
	_asm int 3 ;
#endif
return "<unk>";
}

string ptkn( atom tkn )
{
if(tkn)
	return pptkn( tkn->precursor );
#ifdef DEBUG
	_asm int 3 ;
#endif
return "<unk>";
}
