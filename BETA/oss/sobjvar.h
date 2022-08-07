/***************************************
*
* sobjvar.h
*
*******************/


struct _sobjvar
	{
	objvar			ov;	/* objvar */
	atom*			v;	/* current var pointed to */
	};

typedef struct _sobjvar *	sobjvar;
