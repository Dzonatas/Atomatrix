/***************************************
*
* avei.h
*
*******************/

#pragma once


union _avei
	{
	enum	{
		C_STOP, C_RETURN0, C_RETURN, C_NOP, C_STACK ,
		C_BRA, C_POP_BRA, C_BRF, C_BRT, C_FOREACH, C_ARY_ITER,
		C_FUNC, C_VERB, C_TVERB, C_LVERB,
		C_ADD, C_SUB, C_MUL, C_DIV, C_SEARCH,
		C_GT, C_LT, C_GE, C_LE, C_EQ, C_NE,
		C_AND, C_OR, C_NOT, C_NEGATE,
		C_BAND, C_BOR, C_BNEGATE,
		C_POP, C_PUSH_VAR,
		C_INDEX, C_INDEX_VAR,
		C_EVAL, C_SPLIT, C_SPLICE, C_INDEX_STR, C_SPLIT_STR,
		C_MAKE_ARY, C_PUSH_ON_ARY, C_SPLIT_VAR,
		C_ASSIGN, C_ASSIGN_AE, C_ASSIGN_ME,
		C_PUSHR, C_PUSHL, C_POPR, C_POPL, C_PULL, 
		C_REF,
		C_PUSH_OV, C_PUSH_OVVS, C_PUSH_OVV,
		C_PUSH_OVTS, C_PUSH_OVT,
		C_BREAK, C_CONTINUE,
		C_OBJ_FUNC,
		C_IS, C_ISNOT,
		C_VERB_A_S , C_PUSH_OV_A_S
		}		opcode ;
	atom			(*func)(unsigned, atom*) ;
	atom			a ;
	long			i ;
	};

typedef union _avei * 		avei;


/***************************************
*
* AVEI:
* ATOMATRIX virtual engine instruction
*
*******************/
