// aModule.c		Ballard copyrighted this source circa 2004.

#include "ATOMATRIX.h"
#include "object.h"
#include "program.h"
#include "objvar.h"

#include "cmp_s.h"


extern atom		aModuleAlpha ;
extern atom		aModuleXML ;
extern atom		aModuleSerialed ;


atom oModule( unsigned S )
	{
	extern atom		aUndefined ;
	extern atom		aModuleSerialed ;
	extern atom		aModuleSerinteger ;
	extern atom		(*atomize)(atom) ;
	atom			a ;

	if( S == -1 || S == 0 ) 
		return atominul ;		// UPDATE: should not test for -1||0
	if( ( a = aModuleSerialed->intratom ) )
		do	if( S == (unsigned)((object)a->datum)->serinteger )
				{
				a->allusions++ ;
				return a ;
				}
			while( ( a = a->intratoms ) ) ;
	if( ( a = aModuleSerinteger->intratom ) )
		do	if( S == (unsigned)a->datum )
				{
				a->allusions++ ;
				return a ;
				}
			while( ( a = a->intratoms ) ) ;
	a	= atomize( aModuleSerinteger ) ;
	a->datum = (void*) S ;
	a->allusions = 10000 ;	// UPDATE:
	return a ;
	}


atom oModuleLoad( unsigned S , atom* R )
	{
	extern atom		aUndefined ;
	extern atom		aModuleSerialed ;
	extern atom		aModuleSerinteger ;
	extern atom		(*atomize)(atom) ;
	extern void		(*destruct)(atom) ;
	extern atom		loadModule(atom) ;
	atom			a ;
	atom			r ;

	if( S == -1 || S == 0 )
		return atominul ;
	if( ( a = aModuleSerialed->intratom ) )
		do	if( S == (unsigned)((object)a->datum)->serinteger )
				{
				a->allusions++ ;
				return a ;
				}
			while( ( a = a->intratoms ) ) ;
	if( ( a = aModuleSerinteger->intratom ) )
		do	if( S == (unsigned)(a->datum) )
				break ;
			while( ( a = a->intratoms ) ) ;
	if( ! a )
		{
		a	= atomize( aModuleSerinteger ) ;
		a->allusions = 10000 ;	// UPDATE:
		a->datum = (void*) S ;
		}
	r	= loadModule( a ) ;
	if( R )
		*R = r ;
	else
	if( r && --r->allusions == 0 )
		destruct( r ) ;
	if( a->precursor == aModuleSerialed || ! r )		// UPDATE: logic conditions
		return a ;
	if( --a->allusions == 0 )
		destruct( a ) ;
	return atominul ;
	}


object MOBJ( atom M )
	{
	extern atom		aModuleAlpha ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerialed ;
	extern atom		aModuleSerinteger ;
	extern void		(*destruct)(atom) ;
	extern atom		loadModule(atom) ;
	extern atom		aModuleXML_atomorphize(atom) ;
	atom			r ;

	if( ! M )
		return 0 ;
	if( M->precursor == aModuleSerialed || M->precursor == aModuleAlpha ) 
		return M->datum ;
	if( M->precursor == aModuleXML )
		{
		if( ( r = aModuleXML_atomorphize( M ) ) )
			{
			destruct( r ) ;
			return 0 ;				// UPDATE: inline MOBJ() to return any error, or redo return type
			}
		return M->datum ;
		}
	if( M->precursor == aModuleSerinteger && (int)M->datum == -1 ) 
		return 0 ;
	if( ( r = loadModule( M ) ) )
		{
		destruct( r ) ;
		return 0 ;
		}
	return M->datum ;
	}


void aModuleAlpha_destructor( atom A )
	{
	extern void		(*destruct)(atom) ;
	extern void		(*dealloc)(void*) ;
	object			o ;
	object			x ;
	atom			a ;
	atom			b ;
										// UPDATE: these only work for aModuleAlpha, need to support others
	if( A->datum )
		{
		o = A->datum ;
		if( o->intraobject && o->intraobject->precursor == aModuleAlpha )
			{
			b = o->intraobject ;
			do	{
				if( b->precursor != aModuleAlpha )
					break ;
				( x = (object)(a=b)->datum )->precursor = 0 ;
				b = x->intraobjects ;
				if( --a->allusions == 0 )
					destruct( a ) ;
				} while( b ) ;
			}
		if( o->precursor && o->precursor->precursor == aModuleAlpha )
			{
			if( ( (x=(object)o->precursor->datum)->intraobject ) == A )
				x->intraobject = o->intraobjects ;
			else	{
				x = (object)x->intraobject->datum ;
				while( x->intraobjects != A )			// UPDATE: handles only aModuleAlpha types, needs to support others
					x = (object)x->intraobjects->datum ;
				x->intraobjects = o->intraobjects ;
				}
			}
		if( o->interobject && o->interobject->precursor == aModuleAlpha )
			{
			b = o->interobject ;
			do	{
				if( b->precursor != aModuleAlpha )
					break ;
				( x = (object)(a=b)->datum )->orbit = 0 ;
				b = x->interobjects ;
				if( --a->allusions == 0 )
					destruct( a ) ;
				} while( b ) ;
			}
		if( o->orbit && o->orbit->precursor == aModuleAlpha )
			{
			if( ( (x=(object)o->orbit->datum)->interobject ) == A )
				x->interobject = o->interobjects ;
			else	{
				x = (object)x->interobject->datum ;
				while( x->interobjects != A )			// UPDATE: handles only aModuleAlpha types, needs to support others
					x = (object)x->interobjects->datum ;
				x->interobjects = o->interobjects ;
				}
			}
//		if( o->principal && --o->principal->allusions == 0 )
//			destruct( o->principal ) ;
		dealloc( o ) ;
		A->datum = 0 ;
		}
	}

void aModuleSerialed_destructor( atom A )
	{
	extern void		(*dealloc)(void*) ;

	if( A->datum )
		{
		dealloc( A->datum ) ;
		A->datum = 0 ;
		}
	}


static unsigned		instiantiedTranslatomize = 0 ;


atom aModuleAlpha_translatomize_XML( atom M )
	{
	extern atom		aObjVar ;
	extern atom		aProgram ;
	extern atom		aModuleXML_serial ;
	extern atom		(*aError_pUndefined)(void) ;
	extern atom		(*destruct)(atom) ;
	extern atom		(*aXML_atomize)(void) ;
	extern atom		(*aXML_atomize_a)(atom) ;
	extern atom		(*aXML_interatomize_a_a)(atom,atom) ;
	extern atom		(*aXML_element_a_s)(atom,string) ;
	extern atom		(*aXML_element_s)(string) ;
//	extern atom		(*aXML_attribute_a_s_s)(atom,string,string) ;
	extern atom		(*aXML_data_a)(atom) ;
	extern atom		(*aXML_document_a)(atom) ;
//	extern atom		(*aAsciiString_atomize)(string) ;
//	extern string		(*aAsciiString_canonicalize)(string) ;
	extern string		(*str_dup)(string) ;
	atom			a ;
	atom			s ;
	atom			x ;
//	atom			e ;
	atom			aa ;
//	atom			z ;
	object			o ;

#ifdef DEBUG
	if( M->precursor != aModuleAlpha )
		_asm int 3 ;
#endif

	s	= M->interatom ;
	while( s->precursor != aModuleXML_serial )
		s = s->interatoms ;
	if( ! s )
		return aError_pUndefined() ;

	if( instiantiedTranslatomize )
		{
		aXML_interatomize_a_a( x = aXML_element_s( "serial" ) , s->datum ) ;
		return x ;
		}

	instiantiedTranslatomize = 1 ;

	x	= aXML_element_s( "module" ) ;
//	aXML_attribute_a_s_s( e , "id" , (string)s->datum ) ;

	o	= M->datum ;

	if( o->precursor )
		aXML_interatomize_a_a( aXML_element_a_s( x , "precursor" ) , o->precursor ) ;
	if( o->intraobject )
		aXML_interatomize_a_a( aXML_element_a_s( x , "intramodule" ) , o->intraobject ) ;
	if( o->intraobjects )
		aXML_interatomize_a_a( aXML_element_a_s( x , "intramodules" ) , o->intraobjects ) ;
	if( o->orbit )
		aXML_interatomize_a_a( aXML_element_a_s( x , "orbit" ) , o->orbit ) ;
	if( o->interobject )
		aXML_interatomize_a_a( aXML_element_a_s( x , "intermodule" ) , o->interobject ) ;
	if( o->interobjects )
		aXML_interatomize_a_a( aXML_element_a_s( x , "interamodules" ) , o->interobjects ) ;
	if( o->principal )
		aXML_interatomize_a_a( aXML_element_a_s( x , "principal" ) , o->principal ) ;
//	if( o->perms )
//		aXML_interatomize_a_a( aXML_element_a_s( x , "principle" ) , o->perms ) ;
	if( o->perms )
		{
		extern atom (*oInteger)(unsigned) ;
		atom	a = oInteger( o->perms ) ;
		aXML_interatomize_a_a( aXML_element_a_s( x , "principle" ) , a ) ;
		if( --a->allusions == 0 )
			destruct( a ) ;
		}

	aa = atominul ;
	a = M->interatom ;
	while( a && aa != M->interatom )
		{
		while( a->interatoms != aa )		// UPDATE: interatomize without interatomize()
			a = a->interatoms ;
		aa = a ;
		if( a->precursor == aProgram || a->precursor == aObjVar )
			aXML_interatomize_a_a( x , a ) ;
		a = M->interatom ;
		}

	instiantiedTranslatomize = 0 ;
	return x ;
	}


atom aModuleSerialed_translatomize_a( atom M )
	{
	extern atom		aObjVar ;
	extern atom		aProgram ;
	extern atom		aModuleSerialed ;
	extern atom		(*aError_pUndefined)(void) ;
	extern atom		(*destruct)(atom) ;
	extern atom		(*aXML_atomize)(void) ;
	extern atom		(*aXML_atomize_a)(atom) ;
	extern atom		(*aXML_interatomize_a_a)(atom,atom) ;
	extern atom		(*aXML_element_a_s)(atom,string) ;
	extern atom		(*aXML_element_s)(string) ;
	extern atom		(*aXML_data_a)(atom) ;
	extern atom		(*aXML_document_a)(atom) ;
	extern string		(*str_dup)(string) ;
	extern atom		(*oInteger)(unsigned) ;
	atom			a ;
	atom			x ;
	atom			aa ;
	object			o ;

#ifdef DEBUG
	if( M->precursor != aModuleSerialed )
		_asm int 3 ;
#endif

	if( instiantiedTranslatomize )
		{
		o = M->datum ;
		a = oInteger( o->serinteger ) ;
		aXML_interatomize_a_a( x = aXML_element_s( "serinteger" ) , a = oInteger( o->serinteger ) ) ;
		if( --a->allusions == 0 )
			destruct( a ) ;
		return x ;
		}

	instiantiedTranslatomize = 1 ;

	x	= aXML_element_s( "module" ) ;
//	aXML_attribute_a_s_s( e , "id" , (string)s->datum ) ;

	o	= M->datum ;
	aXML_interatomize_a_a( aXML_element_a_s( x , "serinteger" ) , a = oInteger( o->serinteger ) ) ;
	if( --a->allusions == 0 )
		destruct( a ) ;

	if( o->precursor )
		aXML_interatomize_a_a( aXML_element_a_s( x , "precursor" ) , o->precursor ) ;
	if( o->intraobject )
		aXML_interatomize_a_a( aXML_element_a_s( x , "intramodule" ) , o->intraobject ) ;
	if( o->intraobjects )
		aXML_interatomize_a_a( aXML_element_a_s( x , "intramodules" ) , o->intraobjects ) ;
	if( o->orbit )
		aXML_interatomize_a_a( aXML_element_a_s( x , "orbit" ) , o->orbit ) ;
	if( o->interobject )
		aXML_interatomize_a_a( aXML_element_a_s( x , "intermodule" ) , o->interobject ) ;
	if( o->interobjects )
		aXML_interatomize_a_a( aXML_element_a_s( x , "interamodules" ) , o->interobjects ) ;
	if( o->principal )
		aXML_interatomize_a_a( aXML_element_a_s( x , "principal" ) , o->principal ) ;
	if( o->perms )
		{
		extern atom (*oInteger)(unsigned) ;
		atom	a = oInteger( o->perms ) ;
		aXML_interatomize_a_a( aXML_element_a_s( x , "principle" ) , a ) ;
		if( --a->allusions == 0 )
			destruct( a ) ;
		}

	aa = atominul ;
	a = M->interatom ;
	while( a && aa != M->interatom )
		{
		while( a->interatoms != aa )		// UPDATE: interatomize without interatomize()
			a = a->interatoms ;
		aa = a ;
		if( a->precursor == aProgram || a->precursor == aObjVar )
			aXML_interatomize_a_a( x , a ) ;
		a = M->interatom ;
		}


	instiantiedTranslatomize = 0 ;
	return x ;
	}



atom aModuleSerinteger_translatomize_a( atom A )
	{
	extern atom		aXML ;
	extern atom		aModuleSerinteger ;
	extern atom		(*aXML_interatomize_a_a)(atom,atom) ;
	extern atom		(*aXML_element_s)(string) ;
	extern atom		(*aXML_translatomize_XML)(atom) ;
	extern atom		(*aXML_element_a_s)(atom,string) ;
	extern atom		(*oInteger)(unsigned) ;
	extern atom		(*destruct)(atom) ;
	extern atom		(*aError_pUndefined)(void) ;
	object			o ;
	atom			a ;
	atom			x ;
	atom			s ;

	if( A->precursor == aModuleSerinteger )
		{
		o = A->datum ;
		if( instiantiedTranslatomize )
			aXML_interatomize_a_a( x = aXML_element_s( "serinteger" ) , a = oInteger( o->serinteger ) ) ;
		else	{
			_asm int 3 ;
			x	= aXML_element_s( "module" ) ;
			aXML_interatomize_a_a( aXML_element_a_s( x , "serinteger" ) , a = oInteger( o->serinteger ) ) ;
			}
		if( --a->allusions == 0 )
			destruct( a ) ;
		return x ;
		}
	if( A->precursor == aXML )
		{
		if( ( a = A->interatom ) )
			while( ( a = a->interatoms ) )
//				if( a->precursor == aXML_aAttribute )			// UPDATE: verify attribute name
//					return aModule_atomize_a( a->interatom->interatoms ) ;
//				else
				if( a->precursor == aXML )
					{
					extern atom aInteger ;
					extern atom oModule(unsigned) ;
					s = aXML_translatomize_XML( a ) ;
					if( s->precursor != aInteger )
						{
						if( --s->precursor == 0 )
							destruct( s ) ;
						return aError_pUndefined() ;
						}
					a = oModule( (unsigned) s->datum ) ;
					if( --s->allusions == 0 )
						destruct( s ) ;
					return a ;
					}
		return aError_pUndefined() ;
		}
	return aError_pUndefined() ;
	}


atom aModule_grammance_a_s( atom M , string S )
	{
	extern atom		aError_PROGNF ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aProgram ;
	extern atom		aModule ;
	extern void		(*destruct)(atom) ;
	extern atom		loadModule(atom) ;
	extern atom		aModuleXML_atomorphize(atom) ;
	atom			m ;
	atom			a ;
	atom			r ;

	m = M ;
	do	{
		if( m->precursor == aModuleSerinteger )
			if( ( r = loadModule( m ) ) )
				return r ;
		if( m->precursor == aModuleXML )
			if( ( r = aModuleXML_atomorphize( m ) ) )
				return r ;
		a	= m->interatom ;
		while( a )
			{
			if( a->precursor == aProgram )
				if( cmp_s( ((program)a->datum)->name, S ) == 0 )
					return a ;
			a = a->interatoms ;
			}
		} while( ( m = ((object)m->datum)->precursor ) ) ;
	aError_PROGNF->allusions++;
	return aError_PROGNF;
	}


atom aModule_variance_a_s( atom M , string S )
	{
	extern atom		aError_VARNF ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		loadModule(atom) ;
	extern void		(*destruct)(atom) ;
	extern atom		aModuleXML_atomorphize(atom) ;
	extern atom		aObjVar ;
	atom			a ;
	atom			m ;
	atom			r ;

	m = M ;
	do	{
		if( m->precursor == aModuleSerinteger )
			if( ( r = loadModule( m ) ) )
				return r ;
		if( m->precursor == aModuleXML )
			if( ( r = aModuleXML_atomorphize( m ) ) )
				return r ;
		a	= m->interatom ;
		while( a )
			{
			if( a->precursor == aObjVar )
				if( cmp_s( ((objvar)a->datum)->name, S ) == 0 )
					return a ;
			a = a->interatoms ;
			}
		} while( ( m = ((object)m->datum)->precursor ) ) ;
	aError_VARNF->allusions++ ;
	return aError_VARNF ;
	}




void aModule_pregeneration( void ) 
	{
	extern atom		aModule ;
	extern atom		(*oFunction)(atom,string,void*) ;

	oFunction( aModule , "grammance_a_s" , aModule_grammance_a_s ) ;
	}


void aModule_pConstructent( void )
	{
	extern atom		aModule ;
	extern atom		aModuleSerinteger ;
	extern atom		(*oDestructor)(atom,void*) ;
	extern atom		(*aXML_translatomize_a_p)(atom,atom(*)(atom)) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern string		(*str_dup)(string) ;
	extern void		(*interatomize)(atom,atom) ;
	extern atom		(*oFunction)(atom,string,void*) ;

	oDestructor( aModuleSerialed , aModuleSerialed_destructor ) ;
	oDestructor( aModuleAlpha , aModuleAlpha_destructor ) ;
	interatomize( aXML_translatomize_a_p( aAsciiString_atomize( str_dup( "module" ) ) , aModuleAlpha_translatomize_XML ) , aModuleAlpha ) ;
	interatomize( aXML_translatomize_a_p( atominul , aModuleSerialed_translatomize_a ) , aModuleSerialed ) ;
	interatomize( aXML_translatomize_a_p( aAsciiString_atomize( str_dup( "serinteger" ) ) , aModuleSerinteger_translatomize_a ) , aModuleSerinteger ) ;
	}