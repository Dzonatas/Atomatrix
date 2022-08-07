/***************************************
*
* cod.c
*
*******************/


#include <stdlib.h>
#include <windows.h>

#include "ATOMATRIX.h"
#include "object.h"
#include "avei.h"
#include "sframe.h"
#include "executable.h"
#include "perms.h"
#include "varray.h"
#include "method.h"
#include "variant.h"
#include "objvar.h"
#include "variable.h"
#include "program.h"
#include "task.h"
#include "cmp_s.h"

#include <windows.h>
#include <ctype.h>

#define CONTINUE	256
#define NUMBER		257
#define PULL		258
#define PUSHR		259
#define PUSHL		260
#define POPR		261
#define POPL		262

extern int		sizeOnly;
extern executable	Exec;
extern short		loopBeginCache[20];
extern short		loopType[20];
extern short		numloop;
extern short		last_i;

typedef struct KEYWORD
	{
	char	*name;
	int	type;
	int	code;
	} KEYWORD;

KEYWORD keywords[] = {
  {	"pull",		-1,		PULL		},
  {	"pushr",	-1,		PUSHR		},
  {	"pushl",	-1,		PUSHL		},
  {	"popr",		-1,		POPR		},
  {	"popl",		-1,		POPL		},
  {	"p_or",		NUMBER,		P_OR		},
  {	"p_tr",		NUMBER,		P_TR		},
  {	"p_ar",		NUMBER,		P_AR		},
  {	"p_ow",		NUMBER,		P_OW		},
  {	"p_tw",		NUMBER,		P_TW		},
  {	"p_aw",		NUMBER,		P_AW		},
  {	"p_ox",		NUMBER,		P_OX		},
  {	"p_tx",		NUMBER,		P_TX		},
  {	"p_ax",		NUMBER,		P_AX		},
  {	"p_chown",	NUMBER,		P_CHOWN		},
  {	"p_unique",	NUMBER,		P_UNIQUE	},
  {	"p_wizard",	NUMBER,		P_WIZARD	},
  {	"p_pgmr",	NUMBER,		P_PGMR		},
  {	0,		0,		0		}
  };


short Find_Or_Add_Var( atom A )
	{
	extern void*		(*allocm)(unsigned int) ;
	extern void*		(*allocr)(void*,unsigned int) ;
	int			x;

	if(sizeOnly)
		return 0;
	if( A && Exec->varnames )
		for(x=0;x<Exec->varcnt;x++) 
			if( Exec->varnames[x] == A )
				return x ;
	if( Exec->varnames )
		Exec->varnames = allocr( Exec->varnames,
					sizeof(struct _atom) * (Exec->varcnt + 1) );
	else
		Exec->varnames = allocm( sizeof(struct _atom) * (Exec->varcnt + 1) );
	Exec->varnames[Exec->varcnt] = A ;
	return Exec->varcnt++;
	}


void loopBegin( short type )
	/* stores offsets for continue */
	{
	if(sizeOnly)
		{
		numloop++;
		return;
		}
	loopBeginCache[numloop]=Exec->size;
	loopType[numloop]=type;
	numloop++;
	}


void loopEnd()
	/* recodes C_BREAK and C_CONTINUE, set correct branch offset */
	{
	short			x;
	avei			p;
	avei			pn;

	if(sizeOnly)
		{
		numloop--;
		return;
		}
	numloop--;
	while(loopBeginCache[numloop]<last_i)
		{
		p	= &Exec->exec[last_i];
		pn	= &Exec->exec[last_i+1];
		x	= last_i;
		last_i	= (short) pn->i;
		switch(p->opcode) {
		case C_BREAK:   
			switch(loopType[numloop]) {
			case 0:	p->opcode=C_BRA; break; 
			case 1:	p->opcode=C_POP_BRA; break; /* clears FOREACH ary */
				}
			pn->i=Exec->size-(x+1);
			break;
		case C_CONTINUE:
			p->opcode=C_BRA;
			pn->i=loopBeginCache[numloop]-(x+1);
			break;
		default: // FIXME: Tell("Loop Code Error, %d %d",x, numloop);
			return;
			}
		}
	}


int Code_Obj_Var( atom this , char *name )
	{
	extern atom		aVariable ;
	atom		oo, o;

	if( sizeOnly )
		{
		Exec->size+=2;
		return 1;
		}

	oo = this;
	while(oo)
		{
		o = oo->interatom;
		while(o)
			{
			if(o->precursor == aVariable)
				{
				if( cmp_s(((variable)o->datum)->name,name) == 0 )
					{
//					Exec->exec[Exec->size++].opcode	= C_PUSH_OBJ;
					Exec->exec[Exec->size++].opcode	= C_STACK;
					Exec->exec[Exec->size++].a	= o;
					o->allusions++ ;
					return 1;
					}
				}
			o = o->interatoms;
			}
		oo = oo->precursor;
		}
	return 0;
	}


atom Find_Obj_KeyWord( char *name )
	{
	extern atom		aAtomatrix ;
	extern atom		atomine ;
	extern atom		aError ;
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aVarray ;
	extern atom		aObjVar ;
	extern atom		aModule ;
	extern atom		aProgram ;
	extern atom		aUndefined ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;		// UPDATE: remove entry
	extern atom		aUnicodedString ;		// UPDATE: remove entry
	extern atom		aUnicode ;
	extern atom		aUnicoded ;
	extern object		(*MOBJ)(atom) ;
	extern task		current_task ;
	objvar			ov;
	unsigned int            i;
	varray			va;
	varray			vaa;
	atom			a;
	atom			aa ;

	if(cmp_s("atomatrix",name)==0)
		return aAtomatrix ;
	if(cmp_s("error",name)==0)
		return aError ;
	if(cmp_s("varray",name)==0)
		return aVarray ;
	if(cmp_s("string",name)==0)
		return aAsciiString ;
	if(cmp_s("asciistring",name)==0)
		return aAsciiString ;
	if(cmp_s("integer",name)==0)
		return aInteger ;
	if(cmp_s("atomine",name)==0)
		return atomine ;
//	if(cmp_s("object",name)==0)
//		return aObject ;
	if(cmp_s("module",name)==0)
		return aModule ;
	if(cmp_s("program",name)==0)
		return aProgram ;
	if(cmp_s("undefined",name)==0)
		return aUndefined ;
	if(cmp_s("processity",name)==0)
		return current_task->a ;
	if(cmp_s("asciicharacter",name)==0)
		return aAsciiCharacter ;
	if(cmp_s("unicodedEntity",name)==0)
		return aUnicodedEntity ;
	if(cmp_s("unicodedString",name)==0)
		return aUnicodedString ;
	if(cmp_s("unicoded",name)==0)
		return aUnicoded ;
	if(cmp_s("unicode",name)==0)
		return aUnicode ;
	if( Exec->prog == (program) 0 )
		return atominul ;
//	o = ((object)Exec->prog->a->orbit->datum)->this;
	aa	= Exec->prog->a->orbit ;
	do	{
//		a = POBJ(o)->interatom;
		a = aa->interatom ;
		while(a)
			{
			if(a->precursor != aObjVar)
				{
				a = a->interatoms;
				continue;
				}
			ov = (objvar) a->datum;
			if(cmp_s(ov->name,"_keywords")!=0)
				{
				a = a->interatoms;
				continue;
				}
			if( ov->v->precursor == aVarray )
				{
				va = ov->v->datum ;
				for(i=0;i<va->nelem;i++)
					{
					if( va->ary[i]->precursor != aVarray )
						continue;
					vaa = va->ary[i]->datum ;
					if(vaa->nelem < 2)
						continue;
					if( vaa->ary[0]->precursor == aAsciiString )
						{
						string	s	= vaa->ary[0]->datum ;
						if( cmp_s(s,name) == 0 )
							return vaa->ary[1] ;
						}
					}
				}
			a = a->interatoms;
			}
		} while( MOBJ( aa = ((object)aa->datum)->precursor ) ) ;
	return atominul;
	}
