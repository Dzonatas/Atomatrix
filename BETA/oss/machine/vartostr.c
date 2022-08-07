/***************************************
*
* VarToStr.c
*
*******************/

#define no_init_all deprecated
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include <stdio.h>

#include "ATOMATRIX.h"
#include "variant.h"  // needed by variable.h
#include "variable.h"

// UPDATE: asciiAtomicType( )


string VarToStr( atom A )
	{
	typedef int		(*asciiString)(atom,string,unsigned int,unsigned int*) ;
	extern atom		aAtomineAsciiString ;
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		aError ;
	extern atom		aError_VERBNF ;
	extern atom		aError_VARNF ;
	extern atom		aError_PROGNF ;
	extern atom		aError_TYPE ;
	extern atom		aError_RANGE ;
	extern atom		aError_ARGC ;
	extern atom		aError_PERM ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_INVARG ;
	extern atom		aError_EXISTS ;
	extern atom		aError_COMP ;
	extern atom		aError_RECR ;
	extern atom		aVarray ;
	extern atom		aVariable ;
	static char		buf[100];
	atom			a ;
	atom			aa ;
	unsigned int		i ;

/*
case V_ERROR:
	switch(V->v.err) {
	case E_VERBNF: return "Verb Not Found";
	case E_VARNF: return "Varible Not Found";
	case E_PROGNF: return "Program Not Found";
	case E_TYPE: return "Varible Type Mismatch";
	case E_RANGE: return "Out Of Range";
	case E_ARGC: return "Bad Arg Count";
	case E_PERM: return "Permission Denied";
	case E_INVOBJ: return "Invalid Object";
	case E_INVARG: return "Wrong Argument Type";
	case E_EXISTS: return "Exists Error";
	case E_COMP: return "Could not Compile";
	case E_RECR: return "Recursive Error";
	default: return "Unknown Error";
		}
	break;
*/
	if(A == atominul)		return "[ATOMINUL]";
	if(A == aUndefined)		return "[UNDEFINED]";
	if(A == aError)			return "Error";
	if(A == aError_VERBNF)		return "VERBNF";
	if(A == aError_VARNF)		return "VARNF";
	if(A == aError_PROGNF)		return "PROGNF";
	if(A == aError_TYPE)		return "TYPE";
	if(A == aError_RANGE)		return "RANGE";
	if(A == aError_ARGC)		return "ARGC";
	if(A == aError_PERM)		return "PERM";
	if(A == aError_INVOBJ)		return "INVOBJ";
	if(A == aError_INVARG)		return "INVARG";
	if(A == aError_EXISTS)		return "EXISTS";
	if(A == aError_COMP)		return "COMP";
	if(A == aError_RECR)		return "RECR";
	if(A->precursor == aVarray)	return "{array}";
	if(A->precursor == aInteger) { sprintf( buf , "%p" , A->datum ) ; return buf ; }
	if(A->precursor == aAsciiString)	return A->datum ;
	if(A->precursor == aVariable)
		{
		variable v = A->datum;
		switch(v->v.Type)
			{
	  		case v_NOTHING:		return "";
	  		case v_ATOM:		return "[ATOM]";
	  		case v_ARRAY:		return "[ARY]";
	  		case v_VARIANT:		return "[VAR]";
	  		case v_INTEGER:		sprintf(buf,"%d",v->v.v.I); return buf;
	  		case v_ASCIIZ:		sprintf(buf,"%s",v->v.v.Ptr); return buf;
	  		case v_IASCIIZ:		sprintf(buf,"%s",v->v.v.Ptr); return buf;
	  		case v_WORD:		sprintf(buf,"%d",v->v.v.Word); return buf;
	  		case v_BYTE:		sprintf(buf,"%d",v->v.v.Byte); return buf;
	  		case v_LONG:		sprintf(buf,"%ld",v->v.v.Long); return buf;
	  		case v_ASCII:		sprintf(buf,"%c",v->v.v.Byte); return buf;
	  		}
		}
	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aAtomineAsciiString )
				{
				((asciiString)aa->datum)( A , buf , sizeof(buf) , &i ) ;
				buf[i] = 0 ;
				return buf;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	sprintf(buf,"[ATOM %X]",(unsigned int)A);
	return buf;
	}