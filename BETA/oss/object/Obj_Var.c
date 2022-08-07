/***************************************
*
* Obj_Var.c
*
*******************/


#include "ATOMATRIX.h"
#include "cmp_s.h"
#include "object.h"
#include "objvar.h"


objvar Obj_Var( ObjID obj, string name )  /* search obj only */
	{
	extern int		isValid(ObjID);
	extern atom		getObjVar(atom,string,objvar*);
	extern atom		POBJ(ObjID);
	objvar			ov;
	atom			a;

	if( ! isValid( obj ) )
		return (objvar) 0;

	a = getObjVar( POBJ( obj ), name, &ov );
	if(a)
		return ov;
	return 0;
	}


atom objvar_a_s( atom A , string S )
	{
	extern atom		aObjVar ;
	extern atom		aUndefined ;
	extern atom		aModuleSerinteger ;
	extern atom		aModuleXML ;
	extern atom		aUndefined ;
	extern atom		oModule(unsigned) ;
	extern atom		(*aVarray_atomize_a_n)(atom,int) ;
	extern atom		oSObjVar(objvar) ;
	extern void		(*aVarray_append_a_a)(atom,atom) ;
	extern object		MOBJ(atom) ;
	extern atom		POBJ(ObjID) ;
//	extern atom		getObjVar(atom,string,objvar*) ;
	extern atom		loadModule(atom) ;
	extern atom		aModuleXML_atomorphize(atom) ;
	extern void		(*destruct)(atom) ;
	extern atom		aModule_variance_a_s(atom,string) ;
//	atom			v ;
	atom			r ;
	atom			a ;

	if( A->precursor == aModuleSerinteger )
		if( ( r = loadModule( A ) ) )
			return r ;
	if( A->precursor == aModuleXML )
		if( ( r = aModuleXML_atomorphize( A ) ) )
			return r ;
	if( cmp_s( S , "orbit" ) == 0 )
		{
		if( ! ( a = ((object)A->datum)->orbit ) )
			a = aUndefined ;
		a->allusions++ ;
		return a ;
		}
	if( cmp_s( S , "location" ) == 0 )			// UPDATE: deprecated
		{
		if( ! ( a = ((object)A->datum)->orbit ) )
			a = aUndefined ;
		a->allusions++ ;
		return a ;
		}
	if( cmp_s( S , "precursor" ) == 0 )
		{
		if( ! ( a = ((object)A->datum)->precursor ) )
			a = aUndefined ;
		a->allusions++ ;
		return a ;
		}
//		return oModule( ((object)A->datum)->precursor ) ;
	if( cmp_s( S , "principal" ) == 0 )
		{
		if( ! ( a = ((object)A->datum)->principal ) )
			a = aUndefined ;
		a->allusions++ ;
		return a ;
		}
//		return oModule( ((object)A->datum)->principal ) ;
	if( cmp_s( S , "objects" ) == 0 )
		{
		atom		m ;
		a	= aVarray_atomize_a_n( 0 , 0 ) ;
		m	= ((object)A->datum)->interobject;
		while( m ) 
			{
			m->allusions++ ;
			aVarray_append_a_a( a , m ) ;
			m	= MOBJ(m)->interobjects;
			}
		return a ;
		}
	a = aModule_variance_a_s( A , S ) ;
	if( a->precursor == aObjVar )
		return oSObjVar( a->datum ) ;
	if( --a->allusions == 0 )		// UPDATE: return error
		destruct( a ) ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}