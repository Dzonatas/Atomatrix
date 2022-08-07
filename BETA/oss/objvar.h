/***************************************
*
* objvar.h
*
*******************/


struct _objvar
	{
	atom			v ;		// variable
	atom			a ;		/* atom address */
	string			name ;		// UPDATE: aAsciiString
	atom			principal ;
	char			flags ;
	unsigned short		perms ;
	};

typedef struct _objvar *	objvar;
