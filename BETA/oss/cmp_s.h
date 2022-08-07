/***************************************
*
* cmp_s.h
*
*******************/

#include <string.h>


#define cmp_s(A,B)							\
	( (A)[0]!=(B)[0] ? 1 : 						\
		( (A)[0]==0 ? 0 :					\
			( (A)[1]!=(B)[1] ? 1 : 				\
				( (A)[1]==0 ? 0 : 			\
					strcmp(&((A)[2]),&((B)[2])) 	\
	)	)	)	)					


/***************************************
*
* C macro cmp_s() compares first three characterS of two strings then any
* further comparison of the two strings are passed to strcmp().
*
*******************/
