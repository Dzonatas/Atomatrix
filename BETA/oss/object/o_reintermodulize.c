/***************************************
*
* o_reintermodulize.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"


atom o_reintermodulize( atom A , unsigned T , atom* V )
	{
	extern atom		aUndefined ;
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aInteger ;
	extern atom		aIllogical ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_PERM ;
	extern atom		aError_RECR ;
	extern atom		aExecutionJump ;
	extern atom		(*oInteger)(int) ;
	extern atom		(*atomize)(atom) ;
	extern object		COBJ(ObjID) ;
	extern atom		POBJ(ObjID) ;
	extern int		(*sframe_datum)(atom*) ;
	extern atom		(*sframe_return)(int) ;
	extern void		(*destruct)(atom) ;
	extern atom		(*aVarray_append_a_a)(atom,atom) ;
	extern int		(*execute_a_s_a)(atom,string,atom);
	extern atom		loadModule(atom) ;
	extern object		MOBJ(atom) ;
	atom			from_m ;
	atom			a;
	atom			t ;
	object			o ;
	atom			r ;
	int			j ;
	atom			mm ;
	
	if( ( j = sframe_datum( &r ) ) == 0 )
		{
		if( T != 1 )
			{
			aError_ARGC->allusions++;
			return aError_ARGC;
			}
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
			return aError_INVOBJ ;
			}
		if( ( t = V[0] )->precursor == aUndefined )
			{			// UPDATE: t = aUndefined
			t	= 0 ;
			goto MOVEIT ;
			}
		else
		if( t->precursor == aModuleXML )
			{
			extern atom aModuleXML_atomorphize(atom) ;
			atom	r ;
			if( ( r = aModuleXML_atomorphize( t ) ) )
				return r ;
			}
		else
		if( t->precursor == aModuleSerinteger )
			{
			atom	r ;
			if( ( r = loadModule( t ) ) )
				return r ;
			}
		else
		if( ( ! t->precursor ) || t->precursor->precursor != aModule )
			{
			aError_INVARG->allusions++ ;
			return aError_INVARG ;
			}
		if( A->precursor != t->precursor )
			{
			aError_INVOBJ->allusions++;	// UPDATE: invalid request, not invobj
			return aError_INVOBJ ;
			}
		if( A == t )
			{
			aError_RECR->allusions++;
			return aError_RECR;
			}
							// UPDATE: ? from == to : return
		o	= (object)t->datum ;
		do	{
			if( ( mm = o->orbit ) == A )
				{
				aError_RECR->allusions++;
				return aError_RECR;
				}
			} while( mm && mm != aUndefined && ( o = MOBJ( mm ) ) ) ;
		A->allusions++ ;
		t->allusions++ ;
		a = aVarray_append_a_a( 0 ,	A ) ;
		if( ! ( from_m = ((object)A->datum)->orbit ) )
			from_m = aUndefined ;
		from_m->allusions++ ;
		aVarray_append_a_a( a ,		from_m ) ;
		aVarray_append_a_a( a ,		t ) ;
		if( ! execute_a_s_a( t , "_accept", a ) )
			{
			if( --a->allusions == 0 )
				destruct( a ) ;
			aError_PERM->allusions++;
			return aError_PERM;
			}
		if( --a->allusions == 0 )
			destruct( a ) ;
		return sframe_return( 1 ) ;
		}
	if( j == 1 )
		{
		if( r->precursor != aInteger
			|| (int)r->datum != 1 )
			{
			sframe_return( 0 ) ;
			aError_INVOBJ->allusions++;		// UPDATE: aError_PERM
			return aError_INVOBJ;
			}
		if( ( t = V[0] ) == aUndefined )
			t = 0 ;
	MOVEIT:
		if( ( from_m = ((object)A->datum)->orbit ) )
			{
			A->allusions++ ;
			from_m->allusions++ ;
			a = aVarray_append_a_a( 0 ,	A ) ;
			aVarray_append_a_a( a ,		from_m ) ;
			if( t )	{
				t->allusions++ ;
				aVarray_append_a_a( a ,		t ) ;
				}
			else	{
				aUndefined->allusions++ ;
				aVarray_append_a_a( a ,		aUndefined ) ;
				}
			if( execute_a_s_a( from_m , "_moved_out" , a ) )
				{
				if( --a->allusions == 0 )
					destruct( a ) ;
				return sframe_return( 2 ) ;
				}
			if( --a->allusions == 0 )
				destruct( a ) ;
			}
		}
	if( j == 2 )
		{
		from_m	= ((object)A->datum)->orbit ;
		if( ( t = V[0] ) == aUndefined )
			t = 0 ;
		}
	if( j <= 2 )
		{
		object		m ;
		m	= (object)A->datum ;
		if( from_m )
			{
			if( ( mm = (o=MOBJ(from_m))->interobject ) == A )
				o->interobject = m->interobjects ;
			else	{
				while( mm )
					{
					if( ( o = MOBJ(mm) )->interobjects == A )
						{
						o->interobjects = m->interobjects ;
						break ;
						}
					mm	= o->interobjects ;
					}
				}
			}
		if( ! ( m->orbit = t ) )
			{
			m->interobjects		= 0 ;
			sframe_return( 0 ) ;
			return oInteger( 1 ) ;
			}
		m->interobjects		= ( o = ((object)t->datum) )->interobject ;
		( o->interobject	= A )->allusions++ ;
		a = aVarray_append_a_a( 0 ,	A ) ;
		if( ! from_m )
			from_m = aUndefined ;
		from_m->allusions++ ;
		aVarray_append_a_a( a ,		from_m ) ;
		t->allusions++ ;
		aVarray_append_a_a( a ,		t ) ;
		if( execute_a_s_a( t , "_moved_in" , a ) )
			{
			if( --a->allusions == 0 )
				destruct( a ) ;
			return sframe_return( 3 ) ;
			}
		if( --a->allusions == 0 )
			destruct( a ) ;
		}
	if( j <= 3 )
		{
		sframe_return( 0 ) ;
		return oInteger( 1 ) ;
		}
	aIllogical->allusions++ ;
	return aIllogical ;
	}