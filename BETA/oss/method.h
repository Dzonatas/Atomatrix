/***************************************
*
* method.h
*
*******************/


struct _method
	{
	string			name;
	atom	  		(*method)(atom,unsigned,atom*);
	};

typedef struct _method *	method;
