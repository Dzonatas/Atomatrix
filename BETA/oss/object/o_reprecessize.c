/***************************************
*
* o_reprecessize.c
*
*******************/


#include	"ATOMATRIX.h"
#include "object.h"


atom o_reprecessize( atom A , unsigned T , atom* V )
	{
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_EXISTS ;
	extern object		MOBJ(atom) ;
	int			precesses(atom,atom) ;
	extern atom		loadModule(atom) ;
	extern void		(*destruct)(atom) ;
	object			f ;
	object			t ;
	object			o ;
	object			x ;

	if( A->precursor == aModuleXML )
		{
		extern atom aModuleXML_atomorphize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( A ) ) )
			return r ;
		}
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
	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( V[0]->precursor == aModuleXML )
		{
		extern atom aModuleXML_atomorphize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( V[0] ) ) )
			return r ;
		}
	else
	if( V[0]->precursor == aModuleSerinteger )
		{
		atom	r ;
		if( ( r = loadModule( V[0] ) ) )
			return r ;
		}
	else
	if( ( ! V[0]->precursor ) || V[0]->precursor->precursor != aModule )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( A->precursor != V[0]->precursor )
		{
		aError_INVOBJ->allusions++;		// UPDATE: invalid request, not invobj
		return aError_INVOBJ ;
		}
	o	= ((object)A->datum) ;			// what module to repreccess
	t	= ((object)V[0]->datum) ;		// "to" precessent module
	if( o == t )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( o->precursor == *V )
		{
		(*V)->allusions++ ;
		return *V ;
		}
	if( precesses( *V , A ) )
		{
		aError_EXISTS->allusions++;
		return aError_EXISTS;
		}
	if( o->precursor )
		{
		f	= MOBJ(o->precursor) ;			// "from" precessent module
		if( f->intraobject == A )
			f->intraobject = o->intraobjects ;
		else	{
			x	= MOBJ( f->intraobject ) ;
			while( x->intraobjects != A )
				x = MOBJ( x->intraobjects ) ;
			x->intraobjects = o->intraobjects ;
//			if( ! ( x->intraobjects = o->intraobjects ) )
//				if( --x->intraobjects->allusions == 0 )
//					destruct( x->intraobjects ) ;
			}
		//if( --o->precursor->allusions == 0 )
		//	destruct( o->precursor ) ;
		}
	o->precursor	= *V ;
	o->intraobjects	= t->intraobject ;
	t->intraobject	= A ;
	A->allusions++ ;
	(*V)->allusions++ ;
	return *V ;
	}
