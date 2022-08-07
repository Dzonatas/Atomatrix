/***************************************
*
* Creat_Obj.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"
#include "objvar.h"
#include "perms.h"


ObjID Create_Obj( ObjID precursor, ObjID principal )
	{
	extern atom		aObjVar ;
	extern atom		aModule ;
	extern atom		(*atomize)(atom) ;
	extern void*		(*allocz)(unsigned int) ;
	extern void		(*dup_v)(var,var) ;
	extern atom		(*duplicate)(atom) ;
	extern objvar		Obj_Var(ObjID,string);
	extern object		COBJ(ObjID);
	extern atom		POBJ(ObjID);
	extern atom		addObjVar(atom,string,atom*);
	extern atom		oModule(unsigned) ;
	ObjID			this;
	objvar			ov;
	objvar			par_ov;
	atom			a;
	object			o;
	object			p;

	ov		= Obj_Var( 0, "maxon" ) ;
	this		= ++(ObjID)ov->v->datum ;

	if( principal == -1 )
		principal	= this;

	a		= atomize( aModule );			// UPDATE: do not use anymore
	a->datum	= allocz( sizeof(struct _object) );

	p		= COBJ( precursor );

	o		= a->datum;
	o->serinteger	= this;
	o->perms	= 0;
	o->principal	= oModule( principal );
	o->precursor	= oModule( precursor );
	o->intraobject	= 0;
	o->intraobjects	= p->intraobject ;
	o->orbit	= 0 ;
	o->interobjects		= 0 ;
	o->interobject	= 0 ;

	p->intraobject	= a ;

	if( precursor )
		{
		atom	aa;
		aa = POBJ( precursor )->interatom;
		while( aa )
			{
			if( aa->precursor != aObjVar )
				{
				aa = aa->interatoms;
				continue;
				}
			par_ov = (objvar) aa->datum;
			if( ! P_INHERIT(precursor,par_ov) )
				{ 
				aa = aa->interatoms;
				continue;
				}
			ov = (objvar) addObjVar( a, par_ov->name, 0 )->datum;
			if( par_ov->perms & P_CHOWN)
				( ov->principal = a )->allusions++ ;
			else
				( ov->principal = par_ov->principal )->allusions++ ;
			ov->perms = par_ov->perms;
			ov->v = duplicate( par_ov->v ) ;
//			dup_v( &par_ov->v, &ov->v );
			aa = aa->interatoms;
			}
		}
	return this;
	}
