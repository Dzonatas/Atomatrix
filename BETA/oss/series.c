#include "ATOMATRIX.h"
#include "version_matrix.h"


string seriestring( void )
	{
	return ATOMATRIX ;
	}


#if defined( MICROSOFT_win32 )
#include "..\alchematrix\alchematrix.h"

string seriesAlchematrix( void ) 
	{
	return ALCHEMATRIX ;
	}

#endif
