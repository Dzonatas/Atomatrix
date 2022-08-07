/***************************************
*
* program.h
*
*******************/


struct _program
	{
	atom			a;		/* atom address */
	char *			name;		/* program name */
	char *			psrc;		/* space for source */
	struct _executable *	exep; 		/* compiled program */
//	struct _program	*	pprog;
//	struct _program *	nprog;
        };

typedef struct _program *	program;
