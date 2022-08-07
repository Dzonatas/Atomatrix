/***************************************
*
* o_security.c
*
*******************/


#include "ATOMATRIX.h"
#include "unicode.h"
#include "perms.h"
#include "object.h"

struct _arg
	{
	unsigned short		r ;
	unsigned short		w ;
	unsigned short		x ;
	unsigned		mode ;
	unsigned short		perms ;
	} ;

atom o_security( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString ;
	extern atom		aAsciiString ;
	extern atom		aUnicodedEntity ;
	extern atom		aAsciiCharacter ;
	extern atom		aInteger ;
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_INVARG ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_PERM ;
	extern atom		aError_RANGE ;
	extern atom		(*oInteger)(int) ;
	extern int		p_principal_o(object) ;
	extern atom		loadModule(atom) ;
	extern unsigned		v_security_mod(unsigned,struct _arg*) ;
	struct _arg		p ;
	object			o;
	atom			a ;

	if( A->precursor == aModuleXML )
		{
		extern atom aModuleXML_atomorphize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( A ) ) )
			return r ;
		}
	else
	if( A->precursor == aModuleSerinteger )
		{
		atom	r ;
		if( ( r = loadModule( A ) ) )
			return r ;
		}
	else
	if( ( ! A->precursor ) || A->precursor->precursor != aModule )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( ! T )
		return oInteger( ((object)A->datum)->perms ) ;
	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}

	if( ! p_principal_o( o = (object)A->datum ) )
		{
		aError_PERM->allusions++;
		return aError_PERM;
		}
	p.perms	= o->perms ;
	p.r	= P_TR ;
	p.w	= P_TW ;
	p.x	= P_TX ;
	p.mode	= 1 ;
	if( ( a	= V[0] )->precursor == aInteger )		// UPDATE: may use another type beside aInteger [like aPrincipal]
		{
		if( (int) a->datum >= 256 )
			{
			aError_RANGE->allusions++;
			return aError_RANGE;
			}
		return oInteger( (unsigned)( o->perms = (unsigned short) (unsigned) a->datum ) ) ;
		}
	if( a->precursor == aAsciiString )
		{
		string	s = a->datum ;
		while(*s)
			if( v_security_mod( *s++ , &p ) )
				{
				aError_INVARG->allusions++;
				return aError_INVARG;
				}
		return oInteger( (unsigned)( o->perms = p.perms ) ) ;
		}
	if( a->precursor == aUnicodedString )
		{
		unicodes s = a->datum ;
		while(*s)
			if( v_security_mod( *s++ , &p ) )
				{
				aError_INVARG->allusions++;
				return aError_INVARG;
				}
		return oInteger( (unsigned)( o->perms = p.perms ) ) ;
		}
	if( a->precursor == aAsciiCharacter )
		{
		if( v_security_mod( (byte)a->datum , &p ) )
			{
			aError_INVARG->allusions++;
			return aError_INVARG;
			}
		return oInteger( (unsigned)( o->perms = p.perms ) ) ;
		}
	if( a->precursor == aUnicodedEntity )
		{
		if( v_security_mod( (unicode)a->datum , &p ) )
			{
			aError_INVARG->allusions++;
			return aError_INVARG;
			}
		return oInteger( (unsigned)( o->perms = p.perms ) ) ;
		}
	aError_INVARG->allusions++;
	return aError_INVARG;
	}


static unsigned v_security_mod( unsigned c , struct _arg *p )
	{
	switch(c)
		{
		case '+':
			p->mode	 = 1 ;
			break;
		case '-':
			p->mode	 = 0 ;
			break;
		case '=':
			p->mode	 = 1 ;
			p->perms = 0 ;
			break;
		case 'o':
			p->r	= P_OR ;
			p->w	= P_OW ;
			p->x	= P_OX ;
			break;
		case 't':
			p->r	= P_TR ;
			p->w	= P_TW ;
			p->x	= P_TX ;
			break;
		case 'e':
			p->r	= P_AR ;
			p->w	= P_AW ;
			p->x	= P_AX ;
			break;
		case 'a':
			p->r	= (P_OR|P_TR|P_AR) ;
			p->w	= (P_OW|P_TW|P_AW) ; 
			p->x	= (P_OX|P_TX|P_AX) ;
			break;
		case 'r':
			if(p->mode)
				p->perms |= p->r ;
			else
				p->perms &= ~p->r ;
			break;
		case 'w':
			if(p->mode)
				p->perms |= p->w ;
			else
				p->perms &= ~p->w ;
			break;
		case 'i':
		case 'x':
			if(p->mode)
				p->perms |= p->x ;
			else
				p->perms &= ~p->x ;
			break;
		case 'c':
			if(p->mode)
				p->perms |= P_CHOWN;
			else
				p->perms &= ~P_CHOWN;
			break;
		case 'u':
			if(p->mode)
				p->perms |= P_UNIQUE;
			else
				p->perms &= ~P_UNIQUE;
			break;
		case 'p':
			if(p->mode)
				p->perms |= P_PGMR;
			else
				p->perms &= ~P_PGMR;
			break;
		case 'z':
			if(p->mode)
				p->perms |= P_WIZARD;
			else
				p->perms &= ~P_WIZARD;
			break;
		/*
		case '*':
			if(mode)
				perms |= P_RECYCLE;
			else
				perms &= ~P_RECYCLE;
			break;
		*/
		default:
			return 1 ;
		}
	return 0 ;
	}