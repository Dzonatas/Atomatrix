/***************************************
*
* o_Set.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "unicode.h"

atom o_Set( atom A, int argc, atom* argv ) 
	{
	extern atom		aIllogical ;
	extern atom		aInteger ;
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	variable		v;

	v = A->datum;

	switch( v->v.Type )
		{
		case v_NOTHING:
		case v_ARRAY:
		case v_VARIANT:
		case v_IASCIIZ:
		case v_ASCIIZ:
		default:
			aIllogical->allusions++;
			return  aIllogical;
		case v_ATOM:
			v->v.v.Object = argv[0] ;
			break;
		case v_ASCII:
			if( argv[0]->precursor == aInteger )
				v->v.v.Byte = (char) argv[0]->datum ;
			else
			if( argv[0]->precursor == aAsciiString )
				v->v.v.Byte = ((string)argv[0]->datum)[0];
			else
			if( argv[0]->precursor == aAsciiCharacter )
				v->v.v.Byte = (byte)argv[0]->datum ;
			else
			if( argv[0]->precursor == aUnicodedEntity && (unicode)argv[0]->datum < 128 )
				v->v.v.Byte = (byte)((unicode)argv[0]->datum) ;
			else
			if( argv[0]->precursor == aUnicodedString && *(unicodes)argv[0]->datum < 128 )
				v->v.v.Byte = (byte)(*(unicodes)argv[0]->datum) ;
			else	{
				aIllogical->allusions++;
				return aIllogical;
				}
			break;
		case v_INTEGER:
			if( argv[0]->precursor != aInteger )
				{
				aIllogical->allusions++;
				return aIllogical;
				}
			v->v.v.Long = (long) argv[0]->datum ;
			break;
		case v_WORD:
			if( argv[0]->precursor != aInteger )
				{
				aIllogical->allusions++;
				return aIllogical;
				}
			v->v.v.Word = (short) argv[0]->datum ;
			break;
		case v_LONG:
			if( argv[0]->precursor != aInteger )
				{
				aIllogical->allusions++;
				return aIllogical;
				}
			v->v.v.Long = (long) argv[0]->datum ;
			break;
		case v_BYTE:
			if( argv[0]->precursor != aInteger )
				{
				aIllogical->allusions++;
				return aIllogical;
				}
			v->v.v.Byte = (char) argv[0]->datum ;
			break;
		}

	A->allusions++;
	return A;
	}
