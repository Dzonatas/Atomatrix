/***************************************
*
* varray.h
*
*******************/


struct _varray
	{
	unsigned long		nelem ;
	atom*			ary ;
	};

typedef struct _varray *	varray;
