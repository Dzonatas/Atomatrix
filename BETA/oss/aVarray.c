// aVarray.c			Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"
#include "varray.h"


atom			aVarray ;
atom			aVarrayIterinteger ;
atom			aVarrayIterumDatum ;


atom aVarray_atomize_a_n( atom A , unsigned S )
	{
	extern atom		aVarrayReferent ;
	extern atom		atomize(atom) ;
	extern void *		allocm(unsigned) ;
	extern void *		allocr(void*,unsigned) ;
	extern void		interatomize(atom,atom) ;
	extern atom		aVarray ;
	varray			v ;

	if( ! A )
		{
		A	= atomize( aVarray ) ;
		v	= allocm( sizeof(struct _varray) );
		if( v->nelem = S )
			v->ary	= allocm( sizeof(struct _atom*) * S );
		else
			v->ary	= 0;
		A->datum = v ;
		return	A ;
		}
	if( v = A->datum )
		{
		if( S )
			{
			if( v->ary )
				{
				unsigned	p ;
				atom		r ;
				atom *	a ;
				v->ary	= allocr( a=v->ary , sizeof(struct _atom*) * S );
				if( ( r = A->interatom ) )
					do	if( r->precursor == aVarrayReferent )
							{
							p = (unsigned)r->datum - (unsigned)a ;
							if( p < (S * sizeof(struct _atom*)) )
								r->datum = (atom*)( ((char*)v->ary) + p ) ;
							else	interatomize( r , atominul ) ;
							}
						while( r = r->interatoms ) ;
				// reindex( A , a ) ;
				}
			else
				v->ary	= allocm( sizeof(struct _atom*) * S );
			}
		else
			v->ary	= 0 ;
		v->nelem	= S ;
		return A ;
		}

	v	= allocm( sizeof(struct _varray) );
	if( v->nelem = S )
		v->ary	= allocm( sizeof(struct _atom*) * S );
	else
		v->ary	= 0 ;
	A->datum = v ;
	return A ;
	}


atom aVarray_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		aVarrayReferent_pregeneration(void) ;
	extern atom		oFunction(atom,string,void*) ;
	extern atom		oAtomical(atom,string,atom) ;

	aVarray			= atomize( atomine ) ;
	aVarrayIterinteger	= atomize( atomine ) ;
	aVarrayIterumDatum	= atomize( atomine ) ;

	oFunction( aVarray , "atomize_a_n",	aVarray_atomize_a_n ) ;
	oAtomical( aVarray , "referent" ,	aVarrayReferent_pregeneration() ) ;

	return aVarray ;
	}


void aVarray_empty( atom V )				// Clear_Array()
	{
	extern atom		aVarrayReferent ;
	extern void		interatomize(atom,atom) ;
	extern void		dealloc(void*) ;
	extern void		destruct(atom) ;
	unsigned int            i;
	atom*			v ;
	varray 			vary ;
	atom			a ;
	atom			b ;

	if( ( a = V->interatom ) )
		do	{
			b = a->interatoms ;
			if( a->precursor == aVarrayReferent )
				interatomize( a , atominul ) ;
			} while( ( a = b ) ) ;
	v	= (vary = V->datum)->ary ;
	if( ( i = vary->nelem ) )
		do	 if( *v )
				if( --(*v)->allusions == 0 )
					destruct( *v ) ;
				while( --i ) ;
	if( vary->ary )
		{
		dealloc( vary->ary ) ;
		vary->ary = 0 ;
		}
	vary->nelem = 0 ;
	}




void aVarray_copy( atom* F , atom* T )			// copy_array()
	{
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern void*		allocm(unsigned) ;
	extern atom		duplicate(atom) ;
	unsigned int            i ;
	atom*			t ;
	atom*			f ;

	if( (*F)->precursor != aVarray )
		{
		( *T	= aUndefined )->allusions++ ;		// UPDATE: aIllogical
		return ;
		}
	*T		= aVarray_atomize_a_n( 0 , i = ((varray)(*F)->datum)->nelem ) ;
	t		= ((varray)(*T)->datum)->ary ;
	f		= ((varray)(*F)->datum)->ary ;
	while( i )
		{
		*(t++)		= duplicate( *(f++) ) ;
		i--;
		}
	}



atom aVarray_duplicate( atom A )			// dup_varray( )
	{
	extern atom		aUndefined ;
	extern void*		allocm(unsigned) ;
	extern atom		duplicate(atom) ;
	unsigned int            i ;
	atom*			v ;
	atom			a ;
	atom*			z ;

	a	= aVarray_atomize_a_n( 0 , i = ((varray)A->datum)->nelem ) ;
	v	= ((varray)A->datum)->ary ;
	z	= ((varray)a->datum)->ary ;
	if( i )
		do	*(z++)	= duplicate( *(v++) ) ;
			while( --i ) ;
	return a ;
	}

void aVarray_append_v( atom* T , atom* F )
	{
	extern atom		aUndefined ;
	extern atom		aVarray ;
	extern atom		aVarrayReferent ;
	extern void*		allocm(unsigned) ;
	extern atom		duplicate(atom) ;
	unsigned		nelem;
	atom*			v;
	atom			a ;

	if( (*T)->precursor == aVarrayReferent )
		{
		if( ! (*T)->orbit )
			return ;
		T = (*T)->datum ;
		}
	if( *T == aUndefined )
		{
		aUndefined->allusions-- ;
		nelem	= 0 ;
		a	= aVarray_atomize_a_n( 0 , 1 ) ;
		*T	= a ;
		}
	else
	if( (*T)->precursor == aVarray )
		{
		nelem	= ((varray)(*T)->datum)->nelem ;
		a	= aVarray_atomize_a_n( *T , nelem + 1 ) ;
		}
	else
		return ;

	v = ((varray)a->datum)->ary + nelem ;

	*v	= duplicate( *F ) ;
	}


void aVarray_append_v_a( atom* T , atom A )
	{
	extern atom		aUndefined ;
	extern atom		aVarray ;
	extern atom		aVarrayReferent ;
	extern void*		allocm(unsigned int) ;
	int                     nelem;
	atom*			v;
	atom			a ;

	if( (*T)->precursor == aVarrayReferent )
		{
		if( ! (*T)->orbit )
			return ;
		T = (*T)->datum ;
		}
	if( (*T) == aUndefined )
		{
		aUndefined->allusions-- ;
		nelem	= 0 ;
		a	= aVarray_atomize_a_n( 0 , 1 ) ;
		*T	= a ;
		}
	else
	if( (*T)->precursor == aVarray )
		{
		nelem	= ((varray)(*T)->datum)->nelem ;
		a	= aVarray_atomize_a_n( (*T) , nelem + 1 ) ;
		}
	else
		return ;

	v = ((varray)a->datum)->ary + nelem ;

	*v	= A ;
	}


atom aVarray_append_a_a( atom A , atom B )
	{
	extern void*		allocm(unsigned) ;
	int                     nelem;
	atom*			v;
	atom			a ;

	if( A )
		nelem	= ((varray)A->datum)->nelem ;
	else
		nelem	= 0 ;
	a	= aVarray_atomize_a_n( A , nelem + 1 ) ;
	v = ((varray)a->datum)->ary + nelem ;

	*v	= B ;
	return a ;
	}

/*
static void reindex( atom A , atom* V )			// UPDATE: does not reindex for array length differences, affects aVarrayReferent 
	{
	extern atom		aVarrayReferent ;
	varray			v ;
	atom			a ;
	unsigned		p ;

	if( (v=A->datum)->ary == V )
		return ;
	if( ( a = A->interatom ) )
		do	if( a->precursor == aVarrayReferent )
				{
				p = (unsigned)a->datum - (unsigned)V ;
				a->datum = (atom*)( ((char*)v->ary) + p ) ;
				}
			while( a = a->interatoms ) ;
	}
*/

atom aVarray_duplicator( atom A )		// UPDATE: remove
	{
	return aVarray_duplicate( A ) ;
	}


atom aVarray_pCondition( atom A )
	{
	extern atom		aUndefined ;

	if( ((varray)A->datum)->nelem )
		return A ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aVarrayIterum_pIterate( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aVarrayReferent_atomize_a_n(atom,int) ;
	extern void		destruct(atom) ;
	unsigned int		x ;
	atom			b ;

	if( ( x = ++((unsigned)(b=A->interatom->interatoms)->datum) ) < ((varray)((atom)A->datum)->datum )->nelem )
		{
		atom	a ;
		b	= b->interatoms->datum ;
#ifdef _DEBUG
		if( (atom)b != B )
			_asm int 3 ;
#endif
		if( ( a = b->datum ) )
			if( --a->allusions == 0 )
				destruct( a ) ;
		b->datum = aVarrayReferent_atomize_a_n( A->datum , x ) ;
		A->allusions++ ;
		return A ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aVarrayIterum_destruct_a( atom A )
	{
	extern void		destruct(atom) ;
	atom			a ;
	atom			b ;

	if( ( a = A->datum ) )
		{
		b = A->interatom->interatoms->interatoms->datum ;
		if( --a->allusions == 0 )
			destruct( a ) ;
		if( ( a = b->datum ) )
			{
			if( --a->allusions == 0 )
				destruct( a ) ;
			b->datum = 0 ;
			}
		}
	}


atom aVarray_pIterize( atom A , atom B )
	{
	extern atom		aIterum ;
	extern atom		aVarrayIterumDatum ;
	extern atom		aUndefined ;
	extern atom		aVarrayIterinteger ;
	extern atom		aOprandIterate ;
	extern atom		aVarrayReferent_atomize_a_n(atom,int) ;
	extern atom		oDestructor(atom,void*) ;
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	atom			z ;
	atom			y ;
	atom			x ;

	if( ((varray)A->datum)->nelem )
		{
		B->datum = aVarrayReferent_atomize_a_n( A , 0 ) ;
		z	= atomize( aIterum ) ;
		oDestructor( z , aVarrayIterum_destruct_a ) ;
		( x = atomize( aVarrayIterumDatum ) )->datum = B ;
		interatomize( x , z ) ;
		interatomize( atomize( aVarrayIterinteger ) , z ) ;
		( y = atomize( aOprandIterate ) )->datum = aVarrayIterum_pIterate ;
		interatomize( y , z ) ;
		((atom)( z->datum = A ))->allusions++ ;
		return z ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aVarray_pIndex( atom A , atom B )
	{
	extern atom		aError_RANGE ;
	extern atom		aVarrayReferent_atomize_a_n(atom,int) ;
	varray			v ;
	int			i ;

	v	= A->datum ;
	i	= (int)B->datum ;
	if( i >= 0 && (unsigned int)i < v->nelem )
		return aVarrayReferent_atomize_a_n( A , i ) ;
	aError_RANGE->allusions++ ;
	return aError_RANGE ;
	}


atom aVarray_pPopLeft( atom A )
	{
	extern atom		aIllogical ;
	extern atom		aVarrayReferent ;
	extern void		interatomize(atom,atom) ;
	extern void *		allocr(void*,unsigned) ;
	extern void		dealloc() ;
	varray			v ;
	unsigned int		l ;
	atom*			a ;
	atom			b ;
	atom			c ;
	atom			d ;

	if( ! (v=(varray)(A->datum))->nelem )
		{
		aIllogical->allusions++ ;
		return aIllogical ;
// UPDATE:	aError_RANGE->allusions++ ;
//		return aError_RANGE ;
		}
	b	= *(a=v->ary) ;
	if( ( c = A->interatom ) )
		do	{
			d = c->interatoms ;
			if( c->precursor == aVarrayReferent && c->datum == *a )	// UPDATE: allusion ?
				interatomize( c , atominul ) ;
			} while( ( c = d ) ) ;
	if( ( l = --v->nelem ) )
		{
		unsigned	p ;
		atom		r ;
		do	*((++a)-1) = *a ;
			while( --l ) ;
		v->ary = allocr( v->ary , sizeof(struct _atom*) * v->nelem ) ;
		if( ( r = A->interatom ) )
			do	if( r->precursor == aVarrayReferent )
					{
					if( (p = (unsigned)r->datum - (unsigned)a) )
						r->datum = (atom*)( ((char*)v->ary) + p - sizeof(struct atom*) ) ;
					else	interatomize( r , atominul ) ;
					}
				while( r = r->interatoms ) ;
//		reindex( A , a ) ;
		}
	else	{
		dealloc( a ) ;
		v->ary = 0 ;
		}
	return b ;
	}


atom aVarray_pPopRight( atom A )
	{
	extern atom		aVarrayReferent ;
	extern atom		aIllogical ;
	extern void		interatomize(atom,atom) ;
	extern void *		allocr(void*,unsigned) ;
	extern void		dealloc(void*) ;
	varray			v ;
	atom*			a ;
	atom			b ;
	atom			c ;
	atom			d ;
	atom*			z ;

	if( ! (v=(varray)(A->datum))->nelem )
		{
		aIllogical->allusions++ ;
		return aIllogical ;
// UPDATE:	aError_RANGE->allusions++ ;
//		return aError_RANGE ;
		}
	b	= *(a=(z=v->ary) + --v->nelem) ;
	if( ( c = A->interatom ) )
		do	{
			d = c->interatoms ;
			if( c->precursor == aVarrayReferent && c->datum == a )
				interatomize( c , atominul ) ;
			} while( ( c = d ) ) ;
//	dealloc( a ) ;
	if( v->nelem )
		{
		unsigned	p ;
		atom		r ;
		v->ary = allocr( v->ary , sizeof(struct _atom*) * v->nelem ) ;
		if( ( r = A->interatom ) )
			do	if( r->precursor == aVarrayReferent )
					{
					p = (unsigned)r->datum - (unsigned)z ;
					if( p < (v->nelem * sizeof(struct _atom*)) )
						r->datum = (atom*)( ((char*)v->ary) + p ) ;
					else	interatomize( r , atominul ) ;
					}
				while( r = r->interatoms ) ;
		}
	else	{
		dealloc( v->ary ) ;
		v->ary = 0 ;
		}
	return b ;
	}


atom aVarray_pPull( atom A , atom B )
	{
	extern atom		aIllogical ;
	extern atom		aVarrayReferent ;
	extern void		interatomize(atom,atom) ;
	extern void		dealloc(void*) ;
	extern void*		allocr(void*,unsigned) ;
	varray			v ;
	int			i ;
	unsigned		l ;
	atom*			a ;
	atom			b ;
	atom			c ;
	atom			d ;
	atom*			z ;

	if( ! ( l = (v=(varray)(A->datum))->nelem ) )
		{
		aIllogical->allusions++ ;
		return aIllogical ;
// UPDATE:	aError_RANGE->allusions++ ;
//		return aError_RANGE ;
		}
	i	= (int)B->datum ;
	if( i < 0 )
		{
		i = l + i ;
		if( i < 0 )
			i = 0 ;		// UPDATE: aError_RANGE
		}
	if( (unsigned int)i >= l )
		{
		aIllogical->allusions++ ;
		return aIllogical ;
// UPDATE:	aError_RANGE->allusions++ ;
//		return aError_RANGE ;
		}
	b	= *(a=(z=v->ary)+i) ;
	if( ( c = A->interatom ) )
		do	{
			d = c->interatoms ;
			if( c->precursor == aVarrayReferent && c->datum == *a )
				interatomize( c , atominul ) ;
			} while( ( c = d ) ) ;
//	dealloc( *a ) ;
	if( --v->nelem )
		{
		unsigned	p ;
		atom		r ;
		l	-= i ;
		while( --l )
			*((++a)-1) = *a ;
		v->ary = allocr( v->ary , sizeof(struct _atom*) * v->nelem ) ;
		i *= sizeof(struct atom*) ;
		if( ( r = A->interatom ) )
			do	if( r->precursor == aVarrayReferent )
					{
					p = (unsigned)r->datum - (unsigned)z ;
#ifdef DEBUG
					if( p == i )
						_asm int 3 ;
#endif
					if( p > (unsigned)i )
						{
						if( p <= (v->nelem * sizeof(struct _atom*)) )
							r->datum = (atom*)( ((char*)v->ary) + p - sizeof(struct atom*)) ;
						else	interatomize( r , atominul ) ;
						}
					}
				while( r = r->interatoms ) ;
//		reindex( A , z ) ;
		}
	else	{
		dealloc( a ) ;
		v->ary = 0 ;
		}
	return b ;
	}


atom aVarray_pPushLeft( atom A , atom B )
	{
	extern atom		aVarrayReferent ;
	extern atom		duplicate(atom) ;
	unsigned		nelem ;
	unsigned		i ;
	atom*			t ;
	atom			r ;
						//UPDATE: optimize
	aVarray_atomize_a_n( A , ( nelem = ((varray)A->datum)->nelem ) + 1 ) ;
	t	= ((varray)A->datum)->ary ;
	for( i = nelem ; i > 0 ; i-- )
		t[i] = t[i-1] ;
	if( ( r = A->interatom ) )
		do	if( r->precursor == aVarrayReferent )
				(unsigned)r->datum += sizeof(struct atom*) ;
			while( r = r->interatoms ) ;
	*t		= duplicate( B ) ;
	return 0 ;				//UPDATE: return aArrayItem ;
	}


atom aVarray_pPushRight( atom A , atom B )
	{
//	extern atom		append_a_a(atom,atom) ;
	extern atom		duplicate(atom) ;

	aVarray_append_a_a( A , duplicate( B ) ) ;	// NOTE #1 (bi_pushr.c)
	return 0 ;
	}


atom aVarray_pSplice( atom A , atom B , atom C )
	{
	varray			a ;
	int			b ;
	int			c ;
	int			i , l , max , maxf , maxt ;
	atom*			va ;
	atom			z ;

	a		= A->datum ;
	b		= (int)B->datum ;
	c		= (int)C->datum ;
	z		= aVarray_atomize_a_n( 0 , 0 ) ;
	if( 0 == ( l = a->nelem ) )
		return z ;
#	define MAX(_1_,_2_)	(_1_>_2_?_1_:_2_)
#	define FITRANGE(_1,_2) if( _1 < 0 ) { if( (_1 = _2 + _1) < 0 ) _1 = 0; }
	maxf	= b < 0 ? MAX(-b,l) : MAX(b+1,l);
	maxt	= c < 0 ? MAX(-c,l) : MAX(c+1,l);
	max	= MAX(maxf,maxt);
	FITRANGE(b,max);
	FITRANGE(c,max);
	if( b == c )
		{
		if( b >= l )
			return z ;
		aVarray_atomize_a_n( z , 1 ) ;
		va		= ((varray)z->datum)->ary ;
		*va		= a->ary[c] ;
		a->ary[c]->allusions++ ;
		return z ;
		}
	if( c > b )
		{
		if( b >= l )
			return z ;
		if( c >= l )
			c = l-1;
		aVarray_atomize_a_n( z , (c-b)+1 ) ;
		va		= ((varray)z->datum)->ary ;
		i		= b ;
		while( i <= c )
			{
			*(va++)	= a->ary[i] ;
			a->ary[i++]->allusions++ ;
			}
		return z ;
		}
	if( c >= l )
		return z ;
	if( b >= l )
		b = l - 1;
	aVarray_atomize_a_n( z , (b-c)+1 ) ;
	va		= ((varray)z->datum)->ary ;
	i		= b ;
	while( i >= c )
		{
		*(va++)		= a->ary[i] ;
		a->ary[i--]->allusions++ ;
		}
	return z ;
	}


atom aVarray_pEvaluate( atom A )
	{
	A->allusions++ ;
	return A ;
	}

static atom evaluate( atom A )
	{
	typedef atom		(*oprand)(atom) ;
	extern atom		aOprandEvaluate ;
	atom			aa ;
	atom			a	= A ;

	do	if( ( aa = a->interatom ) )
			do	if( aa->precursor == aOprandEvaluate )
					{
					if( ( a = ((oprand)aa->datum)( A ) ) )
						a->allusions-- ;
					return a ;
					}
				while( ( aa = aa->interatoms ) ) ;
		while( ( a = a->precursor ) ) ;
	return atominul ;
	}


atom aVarray_pSplit( atom A , atom B )
	{
	extern atom		aVarray_append_a_a(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aVarray ;
	atom			b ;
	unsigned		n ;
	unsigned		l ;
	atom*			z ;
	atom*			x ;

	if( ! ( b = evaluate( B ) ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( b->precursor != aVarray )
		{
		aVarray_append_a_a( A , b ) ;
		A->allusions++ ;
		return A ;
		}
	l	= ((varray)b->datum)->nelem ;
	x	= ((varray)b->datum)->ary ;
	n	= ((varray)A->datum)->nelem ;
	aVarray_atomize_a_n( A , n + l ) ;
	z	= ((varray)A->datum)->ary + n ;
	while( l-- )
		( *(z++) = *(x++) )->allusions++ ;		// UPDATE: duplicate() ?
	A->allusions++ ;
	return A ;
	}


#if 0
	v	= 0 ;
	if( B->precursor == aVariant )
		{
		if( ! ( b = B->datum ) )
			b = aUndefined ;
		}
	else
	if( B->precursor == aVarrayReferent )
		{
		if( ! B->orbit )
			_asm int 3 ;
		b	= *(atom*)B->datum ;
		}
	else
	if( B->precursor == aSObjVar )
		{
		if( ! p_read( B ) )
			{
			aError_PERM->allusions++ ;
			aVarray_append_v_a( v , aError_PERM ) ;		// UPDATE: v not set
			( *E_SP = aUndefined )->allusions++ ;
			if( --B->allusions == 0 )
				destruct( B ) ;
			}
		b	= *((sobjvar)B->datum)->v ;
		}
	else
	if( B->precursor == aVarray )
		b	= B ;
	else
	if( B == aUndefined )
		b	= B ;
	else	{
		_asm int 3 ;
		}
	if( b == aUndefined )
		{
		( *E_SP = b )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return ;
		}
	if( b->precursor == aVarray )
		{
		l	= ((varray)b->datum)->nelem ;
		if( l == 0 )
			{
			( *E_SP = aUndefined )->allusions++ ;
			if( --B->allusions == 0 )
				destruct( B ) ;
			return ;
			}
		}
	else
		l	= 1 ;
	a	= A ;
	if( a->precursor == aVarrayReferent )
		{
		_asm int 3 ;
		if( ! a->orbit )
			return ;
		a = *(v=(atom*)a->datum) ;
		}
	if( a == aUndefined )
		{
		_asm int 3 ;
		aUndefined->allusions-- ;
		n	= 0 ;
		a	= aVarray_atomize_a_n( 0 , n + l ) ;
		if( v )
			*v	= a ;
		else
			*(E_SP-1) = a ;
		}
	else
	if( a->precursor == aVarray )
		{
		n	= ((varray)a->datum)->nelem ;
		aVarray_atomize_a_n( a , n + l ) ;
		}
	else	{
#ifdef _DEBUG
		_asm int 3 ;
#endif
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return ;
		}
	z	= ((varray)a->datum)->ary + n ;
	if( b->precursor != aVarray )
		{
		( *z = b )->allusions++ ;
		}
	else	{
		atom*		x ;
		x	= ((varray)b->datum)->ary ;
		while( l-- )
			{
			( *(z++) = *(x++) ) ->allusions++ ;
			}
		}
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
#endif

void aVarray_destruct_a( atom A )
	{
	extern atom		aVarray ;
	extern atom		aVarrayReferent ;
	extern void		destruct(atom) ;
	extern void		interatomize(atom,atom) ;
	extern void		dealloc(void*) ;
	varray			v ;
	atom*			x ;
	unsigned		i ;
	atom			a ;
	atom			b ;

	if( A == aVarray )
		return ;
	v	= A->datum ;
	if( v->ary )
		{
		if( ( i = v->nelem ) )
			{
			x	= v->ary ;
			do	if( --(a=*(x++))->allusions == 0 )
#ifdef DEBUG
				{ *(x-1) = 0 ; destruct( a ) ; }
#else
					destruct( a ) ;
#endif
				while( --i ) ;
			}
		if( ( a = A->interatom ) )
			do	{
				b = a->interatoms ;
				if( a->precursor == aVarrayReferent )
					interatomize( a , atominul ) ;
				} while( ( a = b ) ) ; 
		dealloc( v->ary ) ;
		}
	dealloc( v ) ;
	A->datum = 0 ;
	}


atom aVarray$length( atom A , unsigned T , atom* V )
	{
	extern atom		aError_ARGC ;
	extern atom		aInteger_atomize(unsigned) ;

	if( T )	{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	return aInteger_atomize( ((varray)A->datum)->nelem ) ;
        }


atom aVarray$join( atom A , unsigned T , atom* V )
	{
	extern atom		aVarray ;
	extern atom		aAsciiString ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		aAsciiString_atomize(string) ;
	extern void*		allocm(unsigned) ;
	int			t ;
	unsigned		m ;
	unsigned		c ;
	string			s ;
	string			p ;
	string			a ;
	string			z ;
	atom			b ;
	atom*			v ;

	if( A->precursor != aVarray )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	if( T != 1 )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( (*V)->precursor != aAsciiString )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	if( 0 == ( t = ((varray)A->datum)->nelem ) )
		{
		*( s = allocm( 1 ) ) = 0 ;
		return aAsciiString_atomize( s ) ;		// UPDATE: aAsciiCharacter_atomize( 0 ) ;
		}
	c	= 0 ;
	a	= (*V)->datum ;
	while( *a++ )
		c++ ;
	m	= 0 ;
	v	= ((varray)A->datum)->ary ;
	if( t	> 1 )
		{
		while( --t )
			m += c ;
		t	= ((varray)A->datum)->nelem ;
		}
	do	{
		if( ( b = *v )->precursor == aAsciiString )
			{
			p	= b->datum ;
			while( *p++ )
				m++ ;
			}
		v++ ;
		} while( --t ) ;
	s = z	= allocm( m + 1 ) ;
	if( ( b = *(v=((varray)A->datum)->ary) )->precursor == aAsciiString )	// UPDATE: should preverify all elements as aUnicode else use aError
		{
		p	= b->datum ;
		while( *p )
			*s++ = *p++ ;
		}
	if( ( t = ((varray)A->datum)->nelem - 1 ) ) do
		{
		a	= (*V)->datum ;
		while( *a )
			*s++ = *a++ ;
		if( ( b = *(++v) )->precursor == aAsciiString )
			{
			p	= b->datum ;
			while( *p )
				*s++ = *p++ ;
			}
		} while( --t ) ;
	*s	= 0 ;
	return aAsciiString_atomize( z ) ;
	}


atom aVarray_translatomize( atom X ) 
	{
	extern atom		aXML ;
	extern atom		aXML_translatomize_XML(atom) ;
	extern atom		aXML_element_s(string) ;
	extern atom		aXML_interatomize_a_a(atom,atom) ;
        atom			x ;

	if( X->precursor == aXML )
		{
		atom		v ;
		
		v = aVarray_atomize_a_n( 0 , 0 ) ;
		if( ( x = X->interatom ) )
			while( ( x = x->interatoms ) )
				if( x->precursor == aXML )
					aVarray_append_a_a( v , aXML_translatomize_XML( x ) ) ;
		return v ;
		}
	else	{
		varray		v ;
		unsigned	n ;
		atom*		a ;

		v = (varray) X->datum ;
		x = aXML_element_s( "array" ) ;
		if( ( n = v->nelem ) )
			{
			a = v->ary ;
			do	aXML_interatomize_a_a( x , *(a++) ) ;
				while( --n ) ;
			}
		return x ;
		}
	}


void aVarray_constructent( atom A )
	{
	extern atom		oDestructor(atom,void*) ;
	extern atom		oDuplicator(atom,void*) ;
	extern atom		oFunction(atom,string,void*) ;
	extern void		aVarrayReferent_constructent(void) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern atom		aXML_translatomize_a_p(atom,atom(*)(atom)) ;
	atom			a ;

	oDestructor( aVarray , aVarray_destruct_a ) ;
	oDuplicator( aVarray , aVarray_duplicator ) ;

#	undef __
#	define __(_1)							\
		{							\
		extern atom	aVarray$##_1(atom,unsigned,atom*) ;	\
		extern void	atomethodize(atom,string,atom(*)(atom,unsigned,atom*)) ; \
		atomethodize( aVarray , #_1 , aVarray$##_1 ) ;		\
		}
	__( length ) ;
	__( join ) ;

	oFunction( aVarray , "copy"		, aVarray_copy ) ;
	oFunction( aVarray , "append_v"		, aVarray_append_v ) ;
	oFunction( aVarray , "append_v_a"	, aVarray_append_v_a ) ;
	oFunction( aVarray , "append_a_a"	, aVarray_append_a_a ) ;
	oFunction( aVarray , "duplicate"	, aVarray_duplicate ) ;
	oFunction( aVarray , "empty"		, aVarray_empty ) ;

#	undef __
#	define __(_1)	\
		{	\
		extern atom aOprand##_1 ; \
		atom	_a ; \
		(_a=atomize( aOprand##_1 ))->datum=aVarray_p##_1 ; \
		interatomize( _a , aVarray ) ; \
		}
	__( Condition ) ;
//	__( Iterate ) ;
	__( Iterize ) ;
//	__( Equals ) ;
//	__( AddEquals ) ;
//	__( MinusEquals ) ;
	__( Index ) ;
	__( PopLeft ) ;
	__( PopRight ) ;
	__( Pull ) ;
	__( PushLeft ) ;
	__( PushRight ) ;
	__( Splice ) ;
	__( Split ) ;
	__( Evaluate ) ;

	a	= aXML_translatomize_a_p( aAsciiString_atomize( string_duplicate( "array" )  ) , aVarray_translatomize ) ;
	interatomize( a , aVarray ) ;

	aVarrayReferent_constructent() ;
	}


void aVarray_destructor( void )
	{
	extern void		aVarrayReferent_destructor(void) ;
	extern void		destruct(atom) ;

	aVarrayReferent_destructor() ;
	destruct( aVarray ) ;
	}
