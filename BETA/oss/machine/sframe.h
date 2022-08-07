/***************************************
*
* sframe.h
*
*******************/

#pragma once


struct _sframe
	{
	struct _executable *	exea;	/* executable address */
	atom			scope;	// modular scope of active program
	atom			from;	// the module the program orginates from
	short			lvar;	/* where localvars start */
	union _avei *		pc;	/* stored count */
	atom*			stackbot; /* stack bottom for this program */
	struct _sframe *	last;	/* last current Frame (last used) */
	atom			datum ;
	};

typedef struct _sframe *	sframe;
