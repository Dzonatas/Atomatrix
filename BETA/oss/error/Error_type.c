/***************************************
* 
* Error_type.c
* 
*******************/


#include "ATOMATRIX.h"


string Error_type( atom A )
	{
	extern atom			aError ;
	extern atom			aError_VARNF ;
	extern atom			aError_RANGE ;
	extern atom			aError_TYPE ;
	extern atom			aError_ARGC ;
	extern atom			aError_INVOBJ ;
	extern atom			aError_VERBNF ;
	extern atom			aError_INVARG ;
	extern atom			aError_EXISTS ;
	extern atom			aError_PROGNF ;
	extern atom			aError_COMP ;
	extern atom			aError_RECR ;
	extern atom			aError_PERM ;

	if( A == aError )			return "ERROR" ;
	if( A == aError_VERBNF )		return "VERBNF" ;
	if( A == aError_VARNF )			return "VARNF" ;
	if( A == aError_PROGNF )		return "PROGNF" ;
	if( A == aError_TYPE )			return "TYPE" ;
	if( A == aError_RANGE )			return "RANGE" ;
	if( A == aError_ARGC )			return "ARGC" ;
	if( A == aError_PERM )			return "PERM" ; 
	if( A == aError_INVOBJ )		return "INVOBJ" ;
	if( A == aError_INVARG )		return "INVARG" ;
	if( A == aError_EXISTS )		return "EXISTS" ;
	if( A == aError_COMP )			return "COMP" ;
	if( A == aError_RECR )			return "RECR" ;
	return (string) 0 ;
	}
