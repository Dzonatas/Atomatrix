#pragma once
#include "machine/avei.h"
#include "machine/sframe.h"

struct _task
	{
	atom			a ;

	avei			pc ;	// program counter
	atom*			sp ;	// stack pointer
	atom*			ss ;	// atomic stack
	sframe			sf ;	// program stackframe
	atom*			tv ;	// task vars
	short			vc ;	// task vars count  UPDATE: int

	atom			programme ;
	atom			args ;

	unsigned		ended ;

	unsigned		thread ;
	void *			(*function)() ;
	void *			result ;

	void *			fiber ;
	struct _task *		taskp ;
	struct _task *		taskn ;
	} ;

typedef struct _task *		task ;