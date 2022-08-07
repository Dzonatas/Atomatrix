/***************************************
*
* variant.h
*
*******************/


struct _variant
	{
	unsigned char			Type;
	union {
		atom			Object;
		int			I;
		unsigned short		Word;
		unsigned long		Long;
		unsigned char		Byte;
		string			Ptr;
		struct _varray *	Array;
		struct _variant *	Variant;
		} v;

	};

typedef struct _variant *	variant;


/***************************************
*
*******************/


/* primative _variant types */

#define v_NOTHING		0
#define v_ATOM			1 
#define v_ARRAY 		2
#define v_VARIANT		3
#define v_INTEGER		4
#define v_ASCIIZ		5
#define v_IASCIIZ		6
#define v_WORD			7
#define v_BYTE			8
#define v_LONG			9
#define v_ASCII			10
