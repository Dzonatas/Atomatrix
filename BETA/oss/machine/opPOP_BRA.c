
#include "opcode.h"

void opPOP_BRA()
	{
	extern atom		aUndefined ;
	extern void		(*destruct)(atom) ;

	if( *(--E_SP) != aUndefined )
		{
		if( --(*E_SP)->allusions == 0 )
			destruct( *E_SP );
		( *E_SP = aUndefined )->allusions++ ;
		}
	E_PC	+= E_PC->i ;
	}
