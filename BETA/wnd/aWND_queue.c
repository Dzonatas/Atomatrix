/*************************************************
*
* aWND_queue.c
*
***********************/


#include "ATOMATRIX.h"
#include <windows.h>
#include "wnd.h"

struct _queue
	{
	HANDLE			event ;
	atom*			array ;
	unsigned		size ;
	unsigned		input ;
	unsigned		output ;
	} ;
typedef struct _queue *		queue ;


extern atom			aWND_QUEUE ;


atom aWND_queue( atom A , int T , atom* V )
	{
	extern atom		(*atomize)(atom) ;
	extern void *		(*allocm)(unsigned) ;
	extern void *		(*allocz)(unsigned) ;
	atom			a ;
	queue			q ;

	q		= allocm( sizeof( struct _queue ) ) ;
	q->event	= CreateEvent( 0 , 1 , 0 , 0 ) ;
	q->size		= 1 ;
	q->input	= 0 ;
	q->output	= 0 ;
	q->array	= allocm( sizeof(struct _atom*) * q->size ) ;
	a		= atomize( aWND_QUEUE ) ;
	a->datum	= q ;
	return a ;
	}

void* aWND_QUEUE_task( queue Q  )
	{
	return (void*)WaitForSingleObject( Q->event , INFINITE ) ;
	}


atom aWND_QUEUE_item( atom A , int T , atom* V )
	{
	extern void*		(*allocr)(void*,unsigned) ;
	extern void*		(*taskSplice)(void*,void*) ;
	queue			q ;
	atom			a ;

	if( A == aWND_QUEUE )
		return atominul ;
	if( ( ! ( q = A->datum ) ) || ( ! q->array ) )
		return atominul ;			// UPDATE: aError or aWND_QUEUE_Empty
	if( ! T )
		{
		if( q->input != q->output )
			{
			a = q->array[ q->output++ ] ;
			if( q->output == q->size )
				q->output = 0 ;
			if( q->input == q->output )
				ResetEvent( q->event ) ;
			return a ;
			}
		taskSplice( aWND_QUEUE_task , q ) ;
		if( ( ! ( q = A->datum ) ) || ( ! q->array ) )
			return atominul ;			// UPDATE: aError or aWND_QUEUE_Empty
#ifdef DEBUG
		if( q->input == q->output ) _asm int 3 ;	// UPDATE: should not happen
#endif
		a = q->array[ q->output++ ] ;
		if( q->output == q->size )
			q->output = 0 ;
		if( q->input == q->output )
			ResetEvent( q->event ) ;
		return a ;
		}
	( q->array[ q->input++ ] = a = V[0] )->allusions += 2 ;
	if( q->input == q->size )
		if( q->output )
			q->input = 0 ;
		else
			q->array = (atom*)allocr( (void*)q->array , sizeof(struct _atom*) * ( q->size*=2 ) ) ;
	else
	if( q->input == q->output )
		{
		unsigned	s	= q->size ;
		unsigned	o	= q->output ;
		q->array = (atom*)allocr( (void*)q->array , sizeof(struct _atom*) * ( q->size *= 2 ) ) ;
		memcpy( q->array + ( q->output += s ) , q->array + o , sizeof(struct _atom*) * ( s - o ) ) ;
		}
	SetEvent( q->event ) ;
	return a ;
	}


atom aWND_QUEUE_destruct( atom A , int T , atom* V )
	{
	extern void		(*dealloc)(void*) ;
	queue			q ;
	if( A == aWND_QUEUE )
		return atominul ;
	q	= A->datum ;
	if( q->array ) 
		dealloc( q->array ) ;
	q->array = 0 ;
	SetEvent( (HANDLE) q->event ) ;
	return atominul ;
	}


void aWND_QUEUE_destruct_a( atom A )
	{
	extern void		(*dealloc)(void*) ;
	queue			q ;

	if( A == aWND_QUEUE )
		return ;
	q = A->datum ;
	A->datum = 0 ;
	CloseHandle( q->event ) ;
	if( q->array ) 
		dealloc( q->array ) ;
	dealloc( q ) ;
	}

void aWND_QUEUE_constructent( atom A )
	{
	extern atom		(*oDestructor)(atom,void(*)(atom)) ;
	extern atom		(*atomethodize)(atom,string,atom(*)(atom,int,atom*)) ;

#	define __(_1)							\
		{							\
		extern atom 		aWND_QUEUE_##_1(atom,int,atom*) ;	\
		atomethodize( aWND_QUEUE , #_1 , aWND_QUEUE_##_1 ) ;		\
		}
	__( item ) ;
	__( destruct ) ;

	oDestructor( aWND_QUEUE , aWND_QUEUE_destruct_a ) ;
	}