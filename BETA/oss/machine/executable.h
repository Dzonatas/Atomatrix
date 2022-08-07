/***************************************
*
* executable.h
*
*******************/


struct _executable
	{
	atom			a;
	struct _program *	prog;		/* program pointer */
	short 			size;
	union _avei *		exec;		/* the compiled program */
	strings			linep ;
	short			varcnt;		/* how many local variables */
	atom*			varnames;	/* variable string pointers */
	unsigned long		stacksize;	/* stack needs this much */
	char			sigs;		/* signals */
	};

typedef struct _executable *	executable;

#define E_SIG_EVAL	1
