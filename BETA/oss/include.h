#ifdef _DEBUG
#define DEBUG 1
#endif
#define MICROSOFT_win32
#define DSOEXT  ".DLL"
#if defined( _MSC_VER )
# define _lib		__declspec( dllexport )
# define _tls		__declspec( thread )
#elif defined( __BORLANDC__ )
# define _lib		_export
#else
# define _lib
#endif
