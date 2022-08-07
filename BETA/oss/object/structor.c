/***************************************
*
* structor.c		Copyright (c) 2004 BALLARD
*
*******************/


#include "ATOMATRIX.h"
#include "unicode.h"
#include "machine/executable.h"
#include "object.h"
#include "objvar.h"
#include "program.h"
#include "sobjvar.h"
#include "varray.h"

atom			aModule ;
atom			aModuleSerialed ;
atom			aModuleSerinteger ;
atom			aModuleAlpha ;

/*
atom			aModuleBeta				// UPDATE: implement after aModuleXML nexus
atom			aModuleBetaPrecursor ;
atom			aModuleBetaIntramodule ;
atom			aModuleBetaIntramodules ;
atom			aModuleBetaOrbit ;
atom			aModuleBetaIntermodule ;
atom			aModuleBetaIntermodules ;
atom			aModuleBetaSerial ;
atom			aModuleBetaPrincipal ;
atom			aModuleBetaPrinciple ;
atom			aModuleBetaSource ;
atom			aModuleBetaMethods ;
atom			aModuleBetaVariants ;
*/

extern atom		aSObjVar ;

atom _lib		ATOMATRIX_ATOMICAL ;
atom			aAtomatrix ;
void*			(*oAtomatrixFunction)(string) ;
atom			(*oAtomatrixAtomical)(string) ;
atom			(*oAtomical)(atom,string,atom) ;
atom			(*oFunction)(atom,string,void*) ;
atom			(*oDestructor)(atom,void*) ;
void			(*destruct)(atom) ;
atom			(*atomize)(atom) ;
void*			(*allocm)(unsigned int) ;
void*			(*allocz)(unsigned int) ;
void*			(*allocr)(void*,unsigned int) ;
void			(*dealloc)(void*) ;
void			(*interatomize)(atom,atom) ;
void			(*intratomize)(atom,atom) ;
atom			(*atomethodize)(atom,string,atom(*)(atom,unsigned,atom*));
void			(*output)(string) ;
atom			aIllogical ;
void*			(*oLibraryFunction)(atom,string) ;
atom			(*oLibraryAtomical)(atom,string) ;
atom			atomine ;
atom			aUndefined ;
string			(*str_dup)(string) ;
string			(*str_len)(string) ;
atom			(*duplicate)(atom) ;
atom			aChronogram ;
atom			aAtomatrical ;

atom			aEquality ;
atom			aLibraryMachine ;
atom			aOprandEquals ;
atom			aOprandAddEquals ;
atom			aOprandAddEquate ;
atom			aOprandMinusEquals ;
atom			aOprandMinusEquate ;
atom			aOprandEvaluate ;
atom			aOprandIndex ;
atom			aOprandSplice ;
atom			aOprandCondition ;
atom			aOprandPopLeft ;
atom			aOprandPushLeft ;
atom			aOprandPull ;
atom			aOprandPushRight ;
atom			aOprandPopRight ;
atom			aOprandCompareEqual ;
atom			aExecutionJump ;
atom			(*sframe_return)(int) ;
int			(*sframe_datum)(atom*) ;
atom			(*sframe_scope)(void) ;
atom			(*evaluate)(atom,unsigned,atom*) ;	// UPDATE: prototypes
atom			(*inherit)(atom,unsigned,atom*) ;		// UPDATE: prototypes
atom			(*execute_a_s_a)(atom,string,atom) ;

atom			aVarray ;
atom			(*aVarray_atomize_a_n)(atom,int) ;
atom			(*aVarray_append_a_a)(atom,atom) ;
atom			(*aVarray_append_v_a)(atom,atom) ;

atom			aUnicode ;
atom			aUnicoded ;
atom			aAscii ;
atom			aAsciiString  ;
atom			aUnicodedString  ;
atom			aAsciiCharacter  ;
atom			aUnicodedEntity  ;
atom			(*aAsciiString_atomize)(string) ;
atom			(*aAsciiCharacter_atomize)(byte) ;
string			(*aAsciiString_canonicalize)(string) ;
atom			(*aUnicodedString_atomize)(unicodes) ;
atom			(*aUnicodedEntity_atomize)(unicode) ;
string			(*aUnicodedString_canonicalize)(unicodes) ;
unicodes		(*aUnicodedString_decanonicalize)(string) ;

atom			aInteger ;
atom			(*oInteger)(int) ;

atom			aLibraryError ;
atom			(*aError_pUndefined)(void) ;
atom			aError ;
atom			aError_EXISTS ;
atom			aError_INVARG ;
atom			aError_INVOBJ ;
atom			aError_ARGC ;
atom			aError_PERM ;
atom			aError_RANGE ;
atom			aError_VARNF ;
atom			aError_PROGNF ;
atom			aError_RECR ;


atom			aXML ;
atom			aXML_aElement ;
atom			aXML_aAttribute ;
atom			(*aXML_modulize)(atom) ;
atom			(*aXML_translatomize_XML)(atom) ;
atom			(*aXML_translatomize_a_p)(atom,atom(*)(atom)) ;
atom			(*aXML_atomize)(void) ;
atom			(*aXML_atomize_a)(atom) ;
atom			(*aXML_element_a_s)(atom,string) ;
atom			(*aXML_element_s)(string) ;
atom			(*aXML_interatomize_a_a)(atom,atom) ;
// atom			(*aXML_attribute_a_s_s)(atom,string,string) ;
atom			(*aXML_data_a)(atom) ;
atom			(*aXML_document_a)(atom) ;




void _lib preconstructor( atom A )
	{
	extern objvar		Obj_Var(ObjID,string);
	extern atom		objvar_a_s(atom,string);
	extern object		MOBJ(atom);
	extern object		COBJ(ObjID);
	extern atom		POBJ(ObjID);
	extern atom		oSObjVar(objvar);
	extern int		p_read(atom) ;
	extern int		p_write(atom) ;
	extern int		p_principal(objvar) ;
	extern int		p_principal_o(object) ;
	extern int		p_rw(atom) ;
	extern atom		addProgram(atom,string,string);
	extern atom		getProgram(atom,string,string*);
	extern atom		oModule(unsigned) ;
	extern atom		oModuleLoad(unsigned,atom*) ;
	extern void		aModuleXML_preconstructent(void) ;
	extern void		aModule_pregeneration(void) ;

	aAtomatrix		= ATOMATRIX_ATOMICAL ;
	oAtomatrixFunction	= aAtomatrix->datum ;
	oAtomatrixAtomical	= oAtomatrixFunction( "oAtomatrixAtomical" ) ;
	oAtomical		= oAtomatrixFunction( "oAtomical" ) ;
	oFunction		= oAtomatrixFunction( "oFunction" ) ;
	atomine			= oAtomatrixAtomical( "atomine" ) ;
	atomize			= oAtomatrixFunction( "atomize" ) ;
	oAtomical( aAtomatrix , "aLibraryObject" , A ) ;
#	define __(_1)	\
		oAtomical( A , #_1 , _1 = atomize( atomine ) )
	__( aSObjVar ) ;
	__( aModule ) ;
	aModule_pregeneration() ;
	aModuleSerinteger	= atomize( aModule ) ;
	aModuleSerialed		= atomize( aModule ) ;
	aModuleAlpha		= atomize( aModule ) ;
//	aModuleXML		= atomize( aModule ) ;
#undef __
#	define __(_1)	\
		{	\
		extern atom _1 ; \
		extern atom _1##_pregeneration(void) ; \
		oAtomical( A , #_1 , _1##_pregeneration() ) ; \
		}
        __( aProgram ) ;
	__( aObjVar ) ;
	aModuleXML_preconstructent() ;

	oFunction( A , "oSObjVar"	, oSObjVar ) ;
	oFunction( A , "addProgram"	, addProgram ) ;
	oFunction( A , "getProgram"	, getProgram ) ;
	oFunction( A , "Obj_Var"	, Obj_Var ) ;
	oFunction( A , "objvar_a_s"	, objvar_a_s ) ;
	oFunction( A , "MOBJ"		, MOBJ ) ;
	oFunction( A , "COBJ"		, COBJ ) ;
	oFunction( A , "POBJ"		, POBJ ) ;
	oFunction( A , "p_read"		, p_read ) ;
	oFunction( A , "p_write"	, p_write ) ;
	oFunction( A , "p_principal"	, p_principal ) ;
	oFunction( A , "p_principal_o"	, p_principal_o ) ;
	oFunction( A , "p_rw"		, p_rw ) ;
	oFunction( A , "oModule"	, oModule ) ;
	oFunction( A , "oModuleLoad"	, oModuleLoad ) ;
	}


void _lib destructor( atom A )
	{
	extern void		aModuleXML_destructent(void) ;
	extern void		aProgram_destructor(void) ;
	extern void		aObjVar_destructor(void) ;
	destruct( aSObjVar ) ;
	aObjVar_destructor() ;
	aProgram_destructor() ;
	aModuleXML_destructent() ;
	destruct( aModuleAlpha ) ;
	destruct( aModuleSerinteger ) ;
	destruct( aModuleSerialed ) ;
	destruct( aModule ) ;
	}




atom module_oprandCondition( atom A )
	{
	return A ;
	}


atom moduleSerinteger_oprandCondition( atom A )
	{
	extern atom		loadModule(atom) ;
	atom			a ;
	if( ( a = loadModule( A ) ) )
		{
		if( --a->allusions == 0 )
			destruct( a ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	return A ;
	}


void _lib constructor( atom A )
	{
	extern atom		o_atomize(atom,unsigned,atom*);
	extern atom		o_load(atom,unsigned,atom*);
	extern atom		o_save(atom,unsigned,atom*);
	extern atom		o_spawn(atom,unsigned,atom*);
	extern atom		o_active(atom,unsigned,atom*);
	extern atom		o_serialed(atom,unsigned,atom*);
	extern atom		o_addVariant(atom,unsigned,atom*);
	extern atom		o_addProgram(atom,unsigned,atom*);
	extern atom		o_precessants(atom,unsigned,atom*);
	extern atom		o_precessant(atom,unsigned,atom*);
	extern atom		o_successants(atom,unsigned,atom*);
	extern atom		o_successant(atom,unsigned,atom*);
	extern atom		o_serinteger(atom,unsigned,atom*);
	extern atom		o_principal(atom,unsigned,atom*);
	extern atom		o_security(atom,unsigned,atom*);
	extern atom		o_reprecessize(atom,unsigned,atom*);
	extern atom		o_variant(atom,unsigned,atom*);
	extern atom		o_variants(atom,unsigned,atom*);
	extern atom		o_program(atom,unsigned,atom*);
	extern atom		o_programs(atom,unsigned,atom*);
	extern atom		o_grammance(atom,unsigned,atom*);
	extern atom		o_variance(atom,unsigned,atom*);
	extern atom		o_precesses(atom,unsigned,atom*);
	extern atom		o_intermodulance(atom,unsigned,atom*);
	extern atom		o_reintermodulize(atom,unsigned,atom*);
//	extern void		objvar_destructor(atom);
	extern void		sobjvar_destructor(atom);
//	extern void		module_destructor(atom);
	extern void		aModule_pConstructent(void) ;
	extern void		aSObjVar_pConstructent(atom) ;
	atom			_a ;
	atom			a ;

#	undef __
#	define __(_1)		_1 = oAtomatrixAtomical( #_1 )
#	define ___(_1)		_1 = oAtomatrixFunction( #_1 )
	__( aUndefined ) ;
	__( aIllogical ) ;
	___( oFunction ) ;
	___( oDestructor ) ;
	___( interatomize ) ;
	___( intratomize ) ;
	___( allocm ) ;
	___( allocz ) ;
	___( allocr ) ;
	___( dealloc ) ;
	___( destruct ) ;
	___( atomethodize ) ;
	___( output ) ;
	___( oLibraryFunction ) ;
	___( oLibraryAtomical ) ;
	___( str_dup ) ;
	___( str_len ) ;
	___( duplicate ) ;
	__( aAtomatrical ) ;
	__( aChronogram ) ;
	__( aXML ) ;
	__( aUnicode ) ;
	__( aInteger ) ;
	__( aVarray ) ;
#	undef __
#	undef ___
#	define _(_1)		_a = _1 = oAtomatrixAtomical( #_1 )
#	define __(_1)		_1 = oLibraryAtomical( _a , #_1 )
#	define ___(_1)		_1 = oLibraryFunction( _a , #_1 )
	_( aLibraryMachine ) ;
		__( aEquality ) ;
		__( aOprandEquals ) ;
		__( aOprandAddEquals ) ;
		__( aOprandAddEquate ) ;
		__( aOprandMinusEquals ) ;
		__( aOprandMinusEquate ) ;
		__( aOprandEvaluate ) ;
		__( aOprandIndex ) ;
		__( aOprandSplice ) ;
		__( aOprandCondition ) ;
		__( aOprandPushLeft ) ;
		__( aOprandPushRight ) ;
		__( aOprandPopLeft ) ;
		__( aOprandPopRight ) ;
		__( aOprandPull ) ;
		__( aOprandCompareEqual ) ;
		__( aExecutionJump ) ;
		___( sframe_datum ) ;
		___( sframe_return ) ;
		___( sframe_scope ) ;
		___( evaluate ) ;
		___( inherit ) ;
		___( execute_a_s_a ) ;
	_( aLibraryError ) ;
		___( aError_pUndefined ) ;
		__( aError ) ;
		__( aError_EXISTS ) ;
		__( aError_INVARG ) ;
		__( aError_INVOBJ ) ;
		__( aError_PERM ) ;
		__( aError_ARGC ) ;
		__( aError_RANGE ) ;
		__( aError_VARNF ) ;
		__( aError_PROGNF ) ;
		__( aError_RECR ) ;

	aXML_aElement		= oLibraryAtomical( aXML , "aElement" ) ;
	aXML_aAttribute		= oLibraryAtomical( aXML , "aAttribute" ) ;
	aXML_modulize		= oLibraryFunction( aXML , "modulize" ) ;
	aXML_translatomize_XML	= oLibraryFunction( aXML , "translatomize_XML" ) ;
	aXML_translatomize_a_p	= oLibraryFunction( aXML , "translatomize_a_p" ) ;
	aXML_atomize		= oLibraryFunction( aXML , "atomize" ) ;
	aXML_atomize_a		= oLibraryFunction( aXML , "atomize_a" ) ;
	aXML_element_a_s	= oLibraryFunction( aXML , "element_a_s" ) ;
	aXML_element_s		= oLibraryFunction( aXML , "element_s" ) ;
	aXML_interatomize_a_a	= oLibraryFunction( aXML , "interatomize_a_a" ) ;
	aXML_data_a		= oLibraryFunction( aXML , "data_a" ) ;
	aXML_document_a		= oLibraryFunction( aXML , "document_a" ) ;
//	extern atom		(*aXML_attribute_a_s_s)(atom,string,string) ;

	aAscii		= oLibraryAtomical( aUnicode , "ascii" ) ;
	aUnicoded	= oLibraryAtomical( aUnicode , "unicoded" ) ;
	aAsciiString	= oLibraryAtomical( aAscii , "string" ) ;
	aAsciiCharacter	= oLibraryAtomical( aAscii , "character" ) ;
	aUnicodedString	= oLibraryAtomical( aUnicoded , "string" ) ;
	aUnicodedEntity	= oLibraryAtomical( aUnicoded , "entity" ) ;
	oInteger	= oLibraryFunction( aInteger , "atomize" ) ;
	aAsciiString_atomize = oLibraryFunction( aAsciiString , "atomize" ) ;
	aUnicodedString_atomize = oLibraryFunction( aUnicodedString , "atomize" ) ;
	aAsciiCharacter_atomize = oLibraryFunction( aAsciiCharacter , "atomize" ) ;
	aAsciiString_canonicalize = oLibraryFunction( aAsciiString , "canonicalize" ) ;
	aUnicodedEntity_atomize = oLibraryFunction( aUnicodedEntity , "atomize" ) ;
	aUnicodedString_decanonicalize = oLibraryFunction( aUnicodedString , "decanonicalize" ) ;
	aUnicodedString_canonicalize = oLibraryFunction( aUnicodedString , "canonicalize" ) ;


	aVarray			= oAtomatrixAtomical( "aVarray" ) ;
	aVarray_atomize_a_n	= oLibraryFunction( aVarray , "atomize_a_n" ) ;
	aVarray_append_v_a	= oLibraryFunction( aVarray , "append_v_a" ) ;
	aVarray_append_a_a	= oLibraryFunction( aVarray , "append_a_a" ) ;

	aModule_pConstructent() ;

	atomethodize( aModule , "load" ,	o_load ) ;  //UPDATE: aModuleReferent
	atomethodize( aModule , "spawn" ,	o_spawn ) ;
	atomethodize( aModule , "active" ,	o_active ) ;
	atomethodize( aModule , "serialed" ,	o_serialed ) ;

	atomethodize( aModule , "save" ,	o_save ) ;
	atomethodize( aModule , "addVariant" ,	o_addVariant ) ;
	atomethodize( aModule , "addProgram" ,	o_addProgram ) ;
	atomethodize( aModule , "serinteger" ,	o_serinteger ) ;
	atomethodize( aModule , "precessants" ,	o_precessants ) ;
	atomethodize( aModule , "precessant" ,	o_precessant ) ;
	atomethodize( aModule , "successants" ,	o_successants ) ;
	atomethodize( aModule , "successant" ,	o_successant ) ;
	atomethodize( aModule , "principal" ,	o_principal ) ;
	atomethodize( aModule , "security" ,	o_security ) ;
	atomethodize( aModule , "reprecessize",	o_reprecessize ) ;
	atomethodize( aModule , "program",	o_program ) ;  //UPDATE: grammant
	atomethodize( aModule , "grammance",	o_grammance ) ;
	atomethodize( aModule , "programs",	o_programs ) ;  // UPDATE: grammants
	atomethodize( aModule , "variant",	o_variant ) ;
	atomethodize( aModule , "variance",	o_variance ) ;
	atomethodize( aModule , "variants",	o_variants ) ;
	atomethodize( aModule , "precesses",	o_precesses ) ;
	atomethodize( aModule , "intermodulance", o_intermodulance ) ;
	atomethodize( aModule , "reintermodulize", o_reintermodulize ) ;
	atomethodize( aModule , "atomize" ,	o_atomize ) ;

#	undef __
#	define __(_1)	\
		(a=atomize( aOprand##_1 ))->datum=module_oprand##_1 ; \
		interatomize( a , aModule ) ;
	__( Condition ) ;
#	undef __
#	define __(_1)	\
		(a=atomize( aOprand##_1 ))->datum=moduleSerinteger_oprand##_1 ; \
		interatomize( a , aModuleSerinteger ) ;
	__( Condition ) ;

#	undef __
#	define __(_1)	\
		{ \
		extern void _1##_constructent(void) ; \
		_1##_constructent() ; \
		}
	__( aObjVar ) ;
	__( aProgram ) ;
	__( aModuleXML ) ;
	aSObjVar_pConstructent( A ) ;
	}
