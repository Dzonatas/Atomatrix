/********************************************
*
* aWND.c
*
*************************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include <windows.h>
#include <process.h>
#include "wnd.h"

extern HINSTANCE	dllInstance ;
CRITICAL_SECTION	mQUEUE ;

#ifdef _DEBUG
extern void dbgmsg( char*,void*,void*,void*) ;
#endif

LRESULT CALLBACK	aWND_wndproc(HWND,UINT,WPARAM,LPARAM) ;
unsigned _stdcall	aWND_thread(void*) ;

#ifdef __BORLANDC__
# pragma argsused
#endif

atom			aWND ;
atom			aWND_MSG ;
atom			aWND_RECT ;
atom			aWND_SIZE ;
atom			aWND_TEXTMETRIC ;
atom			aWND_EVENT ;
atom			aWND_FONT ;
atom			aWND_SET ;
atom			aWND_QUEUE ;

WNDCLASS		wndClass ;
string			wndClassName		= "ATOMATRIX.WND" ;
HANDLE			wndThread ;
DWORD			wndThreadIndentifier ;


void aWND_preconstructent( atom A )
	{
	extern atom		atomine ;
	extern atom		(*oAtomical)(atom,string,atom) ;
	extern atom		(*atomize)(atom) ;

	oAtomical( A , "aWND" ,			aWND = atomize( atomine ) ) ;
	oAtomical( A , "aWND_MSG" ,		aWND_MSG = atomize( atomine ) ) ;
	oAtomical( A , "aWND_RECT" ,		aWND_RECT = atomize( atomine ) ) ;
	oAtomical( A , "aWND_SIZE" ,		aWND_SIZE = atomize( atomine ) ) ;
	oAtomical( A , "aWND_TEXTMETRIC" ,	aWND_TEXTMETRIC = atomize( atomine ) ) ;
	oAtomical( A , "aWND_EVENT" ,		aWND_EVENT = atomize( atomine ) ) ;
	oAtomical( A , "aWND_FONT" ,		aWND_FONT = atomize( atomine ) ) ;
	oAtomical( A , "aWND_SET" ,		aWND_SET = atomize( atomine ) ) ;		// UPDATE: required ?
	oAtomical( A , "aWND_QUEUE" ,		aWND_QUEUE = atomize( atomine ) ) ;
	}


void aWND_constructent( atom A )
	{
	extern atom		aOprandCondition ;
	extern void		aWND_destruct_a(atom) ;
	extern void		aWND_FONT_destruct_a(atom) ;
	extern void		aWND_EVENT_constructent(atom) ;
	extern void		aWND_QUEUE_constructent(atom) ;
	extern atom		(*addAtom)(atom,string,atom) ;
	extern atom		(*oDestructor)(atom,void(*)(atom)) ;
	extern atom		(*atomethodize)(atom,string,atom(*)(atom,int,atom*)) ;
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern atom		(*oInteger)(unsigned) ;
	HANDLE			e ;
	DWORD			r ;
	atom			a ;

#	define __(_1)							\
		{							\
		extern atom 		aWND_##_1(atom,int,atom*) ;	\
		atomethodize( A , #_1 , aWND_##_1 ) ;		\
		}
	__( construct )
	__( event ) ;
	__( font ) ;
	__( rectangle ) ;
	__( queue ) ;
#	undef __
#	define __(_1)							\
		{							\
		extern atom 		aWND_##_1(atom,int,atom*) ;	\
		atomethodize( aWND , #_1 , aWND_##_1 ) ;		\
		}
	__( destruct ) ;
	__( getMessage )
	__( textOut ) ;
	__( update ) ;
	__( erase ) ;
	__( getTextMetrics ) ;
	__( getCharWidth ) ;
	__( getRectangle ) ;
	__( getTextExtentPoint ) ;
	__( caret ) ;
	__( show ) ;
	__( close ) ;
	__( activate ) ;
	__( focus ) ;
	__( front ) ;
	__( select ) ;
	__( resize ) ;
	__( position ) ;
	__( setCapture ) ;
	__( releaseCapture ) ;
	__( coordinates ) ;
	__( setTitle ) ;
#	undef __
#	define __(_1)	\
		addAtom( aWND_MSG , #_1 , oInteger( _1 ) ) ;
	__( WM_SIZE ) ;
	__( WM_NULL ) ;
	__( WM_MOVE ) ;
	__( WM_ACTIVATE ) ;
	__( WM_SETFOCUS ) ;
	__( WM_KILLFOCUS ) ;
	__( WM_CLOSE ) ;
	__( WM_QUIT ) ;
	__( WM_SHOWWINDOW ) ;
	__( WM_ACTIVATEAPP ) ;
	__( WM_WINDOWPOSCHANGED ) ;
	__( WM_CHAR ) ;
//	__( WM_SIZING ) ;
	__( WM_SYSCOMMAND ) ;
	__( WM_MOUSEFIRST ) ;
	__( WM_MOUSEMOVE ) ;
	__( WM_LBUTTONDOWN ) ;
	__( WM_LBUTTONUP ) ;
	__( WM_LBUTTONDBLCLK ) ;
	__( WM_RBUTTONDOWN ) ;
	__( WM_RBUTTONUP ) ;
	__( WM_RBUTTONDBLCLK ) ;
	__( WM_MBUTTONDOWN ) ;
	__( WM_MBUTTONUP ) ;
	__( WM_MBUTTONDBLCLK ) ;
	#ifdef WM_MOUSEWHEEL
	__( WM_MOUSEWHEEL ) ;
	#endif
	#ifdef WM_XBUTTONDOWN
	__( WM_XBUTTONDOWN ) ;
	__( WM_XBUTTONUP ) ;
	__( WM_XBUTTONDBLCLK ) ;
	#endif
	__( WM_MOUSELAST ) ;


	__( WMSZ_BOTTOM ) ;
	__( WMSZ_BOTTOMLEFT ) ;
	__( WMSZ_BOTTOMRIGHT ) ;
	__( WMSZ_LEFT ) ;
	__( WMSZ_RIGHT ) ;
	__( WMSZ_TOP ) ;
	__( WMSZ_TOPLEFT ) ;
	__( WMSZ_TOPRIGHT ) ;
	__( SC_CLOSE ) ;
	__( SC_CONTEXTHELP ) ;
	__( SC_DEFAULT ) ;
	__( SC_HOTKEY ) ;
	__( SC_HSCROLL ) ;
	__( SC_KEYMENU ) ;
	__( SC_MAXIMIZE ) ;
	__( SC_MINIMIZE ) ;
	__( SC_MONITORPOWER ) ;
	__( SC_MOUSEMENU ) ;
	__( SC_MOVE ) ;
	__( SC_NEXTWINDOW ) ;
	__( SC_PREVWINDOW ) ;
	__( SC_RESTORE ) ;
	__( SC_SCREENSAVE ) ;
	__( SC_SIZE ) ;
	__( SC_TASKLIST ) ;
	__( SC_VSCROLL ) ;
#	undef __
#	define __(_1)	\
		{	\
		extern atom aWND_MSG_oprand##_1(atom) ; \
		(a=atomize( aOprand##_1 ))->datum=aWND_MSG_oprand##_1 ; \
		interatomize( a , aWND_MSG ) ; \
		}
	__( Condition ) ;

	aWND_EVENT_constructent( A ) ;
	oDestructor( aWND , aWND_destruct_a ) ;

	aWND_QUEUE_constructent( A ) ;

	oDestructor( aWND_FONT , aWND_FONT_destruct_a ) ;

#	undef __
#	define __(_1)	\
		addAtom( A , #_1 , oInteger( _1 ) ) ;
	__( FW_DONTCARE ) ;
	__( FW_THIN ) ;
	__( FW_EXTRALIGHT ) ;
	__( FW_ULTRALIGHT ) ;
	__( FW_LIGHT ) ;
	__( FW_NORMAL ) ;
	__( FW_REGULAR ) ;
	__( FW_MEDIUM ) ;
	__( FW_SEMIBOLD ) ;
	__( FW_DEMIBOLD ) ;
	__( FW_BOLD ) ;
	__( FW_EXTRABOLD ) ;
	__( FW_ULTRABOLD ) ;
	__( FW_HEAVY ) ;
	__( FW_BLACK ) ;

	__( ANSI_CHARSET ) ;
	__( BALTIC_CHARSET ) ;
	__( CHINESEBIG5_CHARSET ) ;
	__( DEFAULT_CHARSET ) ;
	__( EASTEUROPE_CHARSET ) ;
	__( GB2312_CHARSET ) ;
	__( GREEK_CHARSET ) ;
	__( HANGUL_CHARSET ) ;
	__( MAC_CHARSET ) ;
	__( OEM_CHARSET ) ;
	__( RUSSIAN_CHARSET ) ;
	__( SHIFTJIS_CHARSET ) ;
	__( SYMBOL_CHARSET ) ;
	__( TURKISH_CHARSET ) ;
	__( VIETNAMESE_CHARSET ) ;
	__( JOHAB_CHARSET ) ;
	__( ARABIC_CHARSET ) ;
	__( HEBREW_CHARSET ) ;
	__( THAI_CHARSET ) ;
	__( OUT_CHARACTER_PRECIS ) ;
	__( OUT_DEFAULT_PRECIS ) ;
	__( OUT_DEVICE_PRECIS ) ;
	__( OUT_OUTLINE_PRECIS ) ;
	__( OUT_RASTER_PRECIS ) ;
	__( OUT_STRING_PRECIS ) ;
	__( OUT_STROKE_PRECIS ) ;
	__( OUT_TT_ONLY_PRECIS ) ;
	__( OUT_TT_PRECIS ) ;

	__( CLIP_DEFAULT_PRECIS ) ;
	__( CLIP_CHARACTER_PRECIS ) ;
	__( CLIP_STROKE_PRECIS ) ;
	__( CLIP_MASK ) ;
	__( CLIP_EMBEDDED ) ;
	__( CLIP_LH_ANGLES ) ;
	__( CLIP_TT_ALWAYS ) ;

	__( ANTIALIASED_QUALITY ) ;
#ifdef CLEARTYPE_QUALITY
	__( CLEARTYPE_QUALITY ) ;
#endif
	__( DEFAULT_QUALITY ) ;
	__( DRAFT_QUALITY ) ;
	__( NONANTIALIASED_QUALITY ) ;
	__( PROOF_QUALITY ) ;

	__( DEFAULT_PITCH ) ;
	__( FIXED_PITCH ) ;
	__( VARIABLE_PITCH ) ;

	__( FF_DECORATIVE ) ;
	__( FF_DONTCARE ) ;
	__( FF_MODERN ) ;
	__( FF_ROMAN ) ;
	__( FF_SCRIPT ) ;
	__( FF_SWISS ) ;

	//InitializeCriticalSectionAndSpinCount( &mQUEUE , 4000 ) ;
	InitializeCriticalSection( &mQUEUE ) ;
	wndClass.style		= CS_HREDRAW | CS_VREDRAW ;
	wndClass.lpfnWndProc	= aWND_wndproc ;
	wndClass.hInstance	= dllInstance ;
	wndClass.hCursor	= LoadCursor( NULL, IDC_ARROW ) ;
	wndClass.hbrBackground	= (HBRUSH) ( COLOR_WINDOW + 1 ) ;
	wndClass.lpszClassName	= wndClassName ;
	if( ! ( r = RegisterClass( &wndClass ) ) )
		{					// UPDATE: handle error
		MessageBox( 0 , "wndClass not registered" , "ATOMATRIX" , MB_OK | MB_ICONERROR ) ;
		return ;
		}
	e	= CreateEvent( 0 , 0 , 0 , 0 ) ;
	wndThread	= (HANDLE)_beginthreadex( 0 , 0  , aWND_thread , e , 0 , &wndThreadIndentifier ) ;
	if( ! wndThread )
		{					// UPDATE: handle error
		UnregisterClass( wndClassName , dllInstance ) ;
		MessageBox( 0 , "wndThread not begun" , "ATOMATRIX" , MB_OK | MB_ICONERROR ) ;
		return ;
		}
	WaitForSingleObject( e , INFINITE ) ;
	CloseHandle( e ) ;
	}

atom aWND_MSG_oprandCondition( atom A )
	{
	extern atom		aUndefined ;
	extern atom		(*getVariable)(atom,string,variable*) ;
	variable		v ;

	if( A != aWND_MSG ) 
		if( getVariable( A , "message", &v ) )
			if( (unsigned)(v->v.v.Object->datum) != WM_QUIT )
				return A ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aWND_destructent( atom A )
	{
	extern void		(*destruct)(atom) ;

	// aWND_destructor( ) ;

	destruct( aWND_QUEUE ) ;
	destruct( aWND_SET ) ;
	destruct( aWND_FONT ) ;
	destruct( aWND_RECT ) ;
	destruct( aWND_EVENT ) ;
	destruct( aWND_TEXTMETRIC ) ;
	destruct( aWND_MSG ) ;
	destruct( aWND_SIZE ) ;
	destruct( aWND ) ;

	if( WaitForSingleObject( wndThread , 0 ) == WAIT_TIMEOUT )
		{
		PostThreadMessage( wndThreadIndentifier , WM__QUIT , 0 , 0 ) ;
		WaitForSingleObject( wndThread , INFINITE ) ;
		}
	CloseHandle( wndThread ) ;
	UnregisterClass( wndClassName , dllInstance ) ;
	DeleteCriticalSection( &mQUEUE ) ;
	}



atom aWND_construct( atom A , int T , atom* V  )
	{
	extern void *	(*allocz)(unsigned) ;
	extern atom	(*atomize)(atom) ;
	wnd		w ;
	atom		a ;

	w		= allocz( sizeof( struct _wnd ) ) ;
	a		= atomize( aWND ) ;
	a->datum	= w ;
	w->hEvent	= CreateEvent( 0 , 0 , 0 , 0 ) ;
	w->oEvent	= CreateEvent( 0 , 0 , 0 , 0 ) ;

	PostThreadMessage( wndThreadIndentifier , WM__CONSTRUCT , (WPARAM)w , 0 ) ;
	WaitForSingleObject( w->oEvent , INFINITE ) ;

	return a ;
	}


atom aWND_destruct( atom A , int T , atom* V )
	{
	wnd			w ;

	if( A == aWND )
		return atominul ;

	w = A->datum ;
	PostMessage( w->hWND , WM__DESTRUCT , 0 , 0 ) ;
	WaitForSingleObject( w->oEvent , INFINITE ) ;
	SetEvent( w->oEvent ) ;
	w->hWND = 0 ;
	return atominul ;
	}


void aWND_destruct_a( atom A )
	{
	extern void		(*dealloc)(void*) ;
	wnd			w ;

	if( A == aWND )
		return ;

	w	= A->datum ;
	if( w->hWND )
		{
		PostMessage( w->hWND , WM__DESTRUCT , 0 , 0 ) ;
		WaitForSingleObject( w->oEvent , INFINITE ) ;
		w->hWND = 0 ;
		}
	dealloc( w ) ;
	}


void aWND_FONT_destruct_a( atom A )
	{
	if( A == aWND_FONT )
		return ;
	DeleteObject( (HFONT)A->datum ) ;
	}

unsigned _stdcall aWND_thread( void * D )
	{
	extern void	(*dealloc)(void*) ;
	wnd		w ;
	MSG		m ;
	HDC		d ;
	WINDOWINFO	i ;
	RECT		r ;

	PeekMessage( &m , 0 , WM_USER , WM_USER , PM_NOREMOVE ) ;
	SetEvent( (HANDLE)D ) ;

	while( GetMessage( &m , 0 , 0 , 0 ) != -1 )			// UPDATE: use PeekMessage to possibly avoid Windows hangups [on interrupt]
		if( ! ( m.message & WM_APP ) )
			if( m.message == WM_QUIT )
				break ;
			else	{
//				dbgmsg( "m=%lx\t" , (void*)m.message , 0 , 0 ) ;
#if 0
#undef __
#define __(_1,_2) \
	if( m.message == _2 ) dbgmsg( "MMM=" #_1 " ( %lx , %lx )\n" , (void*)m.wParam , (void*)m.lParam , 0 ) ;
__( WM_NULL ,                         0x0000 ) ;
__( WM_CREATE ,                       0x0001 ) ;
__( WM_DESTROY ,                      0x0002 ) ;
__( WM_MOVE ,                         0x0003 ) ;
__( WM_SIZE ,                         0x0005 ) ;
__( WM_ACTIVATE ,                     0x0006 ) ;
__( WM_SETFOCUS ,                     0x0007 ) ;
__( WM_KILLFOCUS ,                    0x0008 ) ;
__( WM_ENABLE ,                       0x000A ) ;
__( WM_SETREDRAW ,                    0x000B ) ;
__( WM_SETTEXT ,                      0x000C ) ;
__( WM_GETTEXT ,                      0x000D ) ;
__( WM_GETTEXTLENGTH ,                0x000E ) ;
__( WM_PAINT ,                        0x000F ) ;
__( WM_CLOSE ,                        0x0010 ) ;
__( WM_QUIT ,                         0x0012 ) ;
__( WM_ERASEBKGND ,                   0x0014 ) ;
__( WM_SYSCOLORCHANGE ,               0x0015 ) ;
__( WM_SHOWWINDOW ,                   0x0018 ) ;
__( WM_WININICHANGE ,                 0x001A ) ;
__( WM_SETTINGCHANGE ,                WM_WININICHANGE ) ;
__( WM_DEVMODECHANGE ,                0x001B ) ;
__( WM_ACTIVATEAPP ,                  0x001C ) ;
__( WM_FONTCHANGE ,                   0x001D ) ;
__( WM_TIMECHANGE ,                   0x001E ) ;
__( WM_CANCELMODE ,                   0x001F ) ;
__( WM_SETCURSOR ,                    0x0020 ) ;
__( WM_MOUSEACTIVATE ,                0x0021 ) ;
__( WM_CHILDACTIVATE ,                0x0022 ) ;
__( WM_QUEUESYNC ,                    0x0023 ) ;
__( WM_GETMINMAXINFO ,                0x0024 ) ;
__( WM_PAINTICON ,                    0x0026 ) ;
__( WM_ICONERASEBKGND ,               0x0027 ) ;
__( WM_NEXTDLGCTL ,                   0x0028 ) ;
__( WM_SPOOLERSTATUS ,                0x002A ) ;
__( WM_DRAWITEM ,                     0x002B ) ;
__( WM_MEASUREITEM ,                  0x002C ) ;
__( WM_DELETEITEM ,                   0x002D ) ;
__( WM_VKEYTOITEM ,                   0x002E ) ;
__( WM_CHARTOITEM ,                   0x002F ) ;
__( WM_SETFONT ,                      0x0030 ) ;
__( WM_GETFONT ,                      0x0031 ) ;
__( WM_SETHOTKEY ,                    0x0032 ) ;
__( WM_GETHOTKEY ,                    0x0033 ) ;
__( WM_QUERYDRAGICON ,                0x0037 ) ;
__( WM_COMPAREITEM ,                  0x0039 ) ;
__( WM_GETOBJECT ,                    0x003D ) ;
__( WM_COMPACTING ,                   0x0041 ) ;
__( WM_COMMNOTIFY ,                   0x0044 ) ;
__( WM_WINDOWPOSCHANGING ,            0x0046 ) ;
__( WM_WINDOWPOSCHANGED ,             0x0047 ) ;
__( WM_POWER ,                        0x0048 ) ;
__( WM_COPYDATA ,                     0x004A ) ;
__( WM_CANCELJOURNAL ,                0x004B ) ;
__( WM_NOTIFY ,                       0x004E ) ;
__( WM_INPUTLANGCHANGEREQUEST ,       0x0050 ) ;
__( WM_INPUTLANGCHANGE ,              0x0051 ) ;
__( WM_TCARD ,                        0x0052 ) ;
__( WM_HELP ,                         0x0053 ) ;
__( WM_USERCHANGED ,                  0x0054 ) ;
__( WM_NOTIFYFORMAT ,                 0x0055 ) ;
__( WM_CONTEXTMENU ,                  0x007B ) ;
__( WM_STYLECHANGING ,                0x007C ) ;
__( WM_STYLECHANGED ,                 0x007D ) ;
__( WM_DISPLAYCHANGE ,                0x007E ) ;
__( WM_GETICON ,                      0x007F ) ;
__( WM_SETICON ,                      0x0080 ) ;
__( WM_NCCREATE ,                     0x0081 ) ;
__( WM_NCDESTROY ,                    0x0082 ) ;
__( WM_NCCALCSIZE ,                   0x0083 ) ;
__( WM_NCHITTEST ,                    0x0084 ) ;
__( WM_NCPAINT ,                      0x0085 ) ;
__( WM_NCACTIVATE ,                   0x0086 ) ;
__( WM_GETDLGCODE ,                   0x0087 ) ;
__( WM_SYNCPAINT ,                    0x0088 ) ;
__( WM_NCMOUSEMOVE ,                  0x00A0 ) ;
__( WM_NCLBUTTONDOWN ,                0x00A1 ) ;
__( WM_NCLBUTTONUP ,                  0x00A2 ) ;
__( WM_NCLBUTTONDBLCLK ,              0x00A3 ) ;
__( WM_NCRBUTTONDOWN ,                0x00A4 ) ;
__( WM_NCRBUTTONUP ,                  0x00A5 ) ;
__( WM_NCRBUTTONDBLCLK ,              0x00A6 ) ;
__( WM_NCMBUTTONDOWN ,                0x00A7 ) ;
__( WM_NCMBUTTONUP ,                  0x00A8 ) ;
__( WM_NCMBUTTONDBLCLK ,              0x00A9 ) ;
__( WM_NCXBUTTONDOWN ,                0x00AB ) ;
__( WM_NCXBUTTONUP ,                  0x00AC ) ;
__( WM_NCXBUTTONDBLCLK ,              0x00AD ) ;
__( WM_INPUT ,                        0x00FF ) ;
__( WM_KEYFIRST ,                     0x0100 ) ;
__( WM_KEYDOWN ,                      0x0100 ) ;
__( WM_KEYUP ,                        0x0101 ) ;
__( WM_CHAR ,                         0x0102 ) ;
__( WM_DEADCHAR ,                     0x0103 ) ;
__( WM_SYSKEYDOWN ,                   0x0104 ) ;
__( WM_SYSKEYUP ,                     0x0105 ) ;
__( WM_SYSCHAR ,                      0x0106 ) ;
__( WM_SYSDEADCHAR ,                  0x0107 ) ;
__( WM_UNICHAR ,                      0x0109 ) ;
__( WM_KEYLAST ,                      0x0109 ) ;
__( WM_KEYLAST ,                      0x0108 ) ;
__( WM_IME_STARTCOMPOSITION ,         0x010D ) ;
__( WM_IME_ENDCOMPOSITION ,           0x010E ) ;
__( WM_IME_COMPOSITION ,              0x010F ) ;
__( WM_IME_KEYLAST ,                  0x010F ) ;
__( WM_INITDIALOG ,                   0x0110 ) ;
__( WM_COMMAND ,                      0x0111 ) ;
__( WM_SYSCOMMAND ,                   0x0112 ) ;
__( WM_TIMER ,                        0x0113 ) ;
__( WM_HSCROLL ,                      0x0114 ) ;
__( WM_VSCROLL ,                      0x0115 ) ;
__( WM_INITMENU ,                     0x0116 ) ;
__( WM_INITMENUPOPUP ,                0x0117 ) ;
__( WM_MENUSELECT ,                   0x011F ) ;
__( WM_MENUCHAR ,                     0x0120 ) ;
__( WM_ENTERIDLE ,                    0x0121 ) ;
__( WM_MENURBUTTONUP ,                0x0122 ) ;
__( WM_MENUDRAG ,                     0x0123 ) ;
__( WM_MENUGETOBJECT ,                0x0124 ) ;
__( WM_UNINITMENUPOPUP ,              0x0125 ) ;
__( WM_MENUCOMMAND ,                  0x0126 ) ;
__( WM_CHANGEUISTATE ,                0x0127 ) ;
__( WM_UPDATEUISTATE ,                0x0128 ) ;
__( WM_QUERYUISTATE ,                 0x0129 ) ;
__( WM_CTLCOLORMSGBOX ,               0x0132 ) ;
__( WM_CTLCOLOREDIT ,                 0x0133 ) ;
__( WM_CTLCOLORLISTBOX ,              0x0134 ) ;
__( WM_CTLCOLORBTN ,                  0x0135 ) ;
__( WM_CTLCOLORDLG ,                  0x0136 ) ;
__( WM_CTLCOLORSCROLLBAR ,            0x0137 ) ;
__( WM_CTLCOLORSTATIC ,               0x0138 ) ;
__( WM_MOUSEFIRST ,                   0x0200 ) ;
__( WM_MOUSEMOVE ,                    0x0200 ) ;
__( WM_LBUTTONDOWN ,                  0x0201 ) ;
__( WM_LBUTTONUP ,                    0x0202 ) ;
__( WM_LBUTTONDBLCLK ,                0x0203 ) ;
__( WM_RBUTTONDOWN ,                  0x0204 ) ;
__( WM_RBUTTONUP ,                    0x0205 ) ;
__( WM_RBUTTONDBLCLK ,                0x0206 ) ;
__( WM_MBUTTONDOWN ,                  0x0207 ) ;
__( WM_MBUTTONUP ,                    0x0208 ) ;
__( WM_MBUTTONDBLCLK ,                0x0209 ) ;
__( WM_MOUSEWHEEL ,                   0x020A ) ;
__( WM_XBUTTONDOWN ,                  0x020B ) ;
__( WM_XBUTTONUP ,                    0x020C ) ;
__( WM_XBUTTONDBLCLK ,                0x020D ) ;
__( WM_MOUSELAST ,                    0x020D ) ;
__( WM_MOUSELAST ,                    0x020A ) ;
__( WM_MOUSELAST ,                    0x0209 ) ;
__( WM_PARENTNOTIFY ,                 0x0210 ) ;
__( WM_ENTERMENULOOP ,                0x0211 ) ;
__( WM_EXITMENULOOP ,                 0x0212 ) ;
__( WM_NEXTMENU ,                     0x0213 ) ;
__( WM_SIZING ,                       0x0214 ) ;
__( WM_CAPTURECHANGED ,               0x0215 ) ;
__( WM_MOVING ,                       0x0216 ) ;
__( WM_POWERBROADCAST ,               0x0218 ) ;
__( WM_DEVICECHANGE ,                 0x0219 ) ;
__( WM_MDICREATE ,                    0x0220 ) ;
__( WM_MDIDESTROY ,                   0x0221 ) ;
__( WM_MDIACTIVATE ,                  0x0222 ) ;
__( WM_MDIRESTORE ,                   0x0223 ) ;
__( WM_MDINEXT ,                      0x0224 ) ;
__( WM_MDIMAXIMIZE ,                  0x0225 ) ;
__( WM_MDITILE ,                      0x0226 ) ;
__( WM_MDICASCADE ,                   0x0227 ) ;
__( WM_MDIICONARRANGE ,               0x0228 ) ;
__( WM_MDIGETACTIVE ,                 0x0229 ) ;
__( WM_MDISETMENU ,                   0x0230 ) ;
__( WM_ENTERSIZEMOVE ,                0x0231 ) ;
__( WM_EXITSIZEMOVE ,                 0x0232 ) ;
__( WM_DROPFILES ,                    0x0233 ) ;
__( WM_MDIREFRESHMENU ,               0x0234 ) ;
__( WM_IME_SETCONTEXT ,               0x0281 ) ;
__( WM_IME_NOTIFY ,                   0x0282 ) ;
__( WM_IME_CONTROL ,                  0x0283 ) ;
__( WM_IME_COMPOSITIONFULL ,          0x0284 ) ;
__( WM_IME_SELECT ,                   0x0285 ) ;
__( WM_IME_CHAR ,                     0x0286 ) ;
__( WM_IME_REQUEST ,                  0x0288 ) ;
__( WM_IME_KEYDOWN ,                  0x0290 ) ;
__( WM_IME_KEYUP ,                    0x0291 ) ;
__( WM_MOUSEHOVER ,                   0x02A1 ) ;
__( WM_MOUSELEAVE ,                   0x02A3 ) ;
__( WM_NCMOUSEHOVER ,                 0x02A0 ) ;
__( WM_NCMOUSELEAVE ,                 0x02A2 ) ;
__( WM_WTSSESSION_CHANGE ,            0x02B1 ) ;
__( WM_TABLET_FIRST ,                 0x02c0 ) ;
__( WM_TABLET_LAST ,                  0x02df ) ;
__( WM_CUT ,                          0x0300 ) ;
__( WM_COPY ,                         0x0301 ) ;
__( WM_PASTE ,                        0x0302 ) ;
__( WM_CLEAR ,                        0x0303 ) ;
__( WM_UNDO ,                         0x0304 ) ;
__( WM_RENDERFORMAT ,                 0x0305 ) ;
__( WM_RENDERALLFORMATS ,             0x0306 ) ;
__( WM_DESTROYCLIPBOARD ,             0x0307 ) ;
__( WM_DRAWCLIPBOARD ,                0x0308 ) ;
__( WM_PAINTCLIPBOARD ,               0x0309 ) ;
__( WM_VSCROLLCLIPBOARD ,             0x030A ) ;
__( WM_SIZECLIPBOARD ,                0x030B ) ;
__( WM_ASKCBFORMATNAME ,              0x030C ) ;
__( WM_CHANGECBCHAIN ,                0x030D ) ;
__( WM_HSCROLLCLIPBOARD ,             0x030E ) ;
__( WM_QUERYNEWPALETTE ,              0x030F ) ;
__( WM_PALETTEISCHANGING ,            0x0310 ) ;
__( WM_PALETTECHANGED ,               0x0311 ) ;
__( WM_HOTKEY ,                       0x0312 ) ;
__( WM_PRINT ,                        0x0317 ) ;
__( WM_PRINTCLIENT ,                  0x0318 ) ;
__( WM_APPCOMMAND ,                   0x0319 ) ;
__( WM_THEMECHANGED ,                 0x031A ) ;
__( WM_HANDHELDFIRST ,                0x0358 ) ;
__( WM_HANDHELDLAST ,                 0x035F ) ;
__( WM_AFXFIRST ,                     0x0360 ) ;
__( WM_AFXLAST ,                      0x037F ) ;
__( WM_PENWINFIRST ,                  0x0380 ) ;
__( WM_PENWINLAST ,                   0x038F ) ;
__( WM_APP ,                          0x8000 ) ;
__( WM_USER ,                         0x0400 ) ;
#endif
				TranslateMessage( &m ) ;
				DispatchMessage( &m ) ;
				continue ;
				}
		else
		switch( m.message )
			{
			case WM__NUL:
#ifdef _DEBUG
				dbgmsg( "NUL %lx\n" , (void*)m.wParam , 0 , 0 ) ;
#endif 
				break ;
			case WM__DESTRUCT:
				DestroyWindow( m.hwnd ) ;
				DeleteObject( w->rBM ) ;
				DeleteObject( w->wBM ) ;
				DeleteDC( w->rDC ) ;
				DeleteDC( w->wDC ) ;
				break ;
			case WM__SHOW:
				ShowWindow( m.hwnd , m.wParam ) ;
				break ;
			case WM__CLOSE:
				CloseWindow( m.hwnd ) ;
				break ;
			case WM__ACTIVATE:
				SetActiveWindow( m.hwnd ) ;
				break ;
			case WM__SETFOCUS:
				SetFocus( m.hwnd ) ;
				break ;
			case WM__FRONT:
				BringWindowToTop( m.hwnd ) ;
				break ;
			case WM__QUIT:
				PostQuitMessage( m.wParam ) ;
				break ;
			case WM__CONSTRUCT:
				w = (wnd)m.wParam ;
				w->hWND = CreateWindow( wndClassName , wndClassName , WS_OVERLAPPEDWINDOW ,
					CW_USEDEFAULT , 0 , CW_USEDEFAULT , 0 ,	0 , 0 ,	dllInstance , 0
					) ;
				if( ! w->hWND )
					{
					MessageBox( 0 , "createwindow" , "ATOMATRIX" , MB_OK | MB_ICONERROR ) ;
					return 0 ;
					}
				SetWindowLongPtr( w->hWND , GWLP_USERDATA , (LONG)w ) ;
				GetWindowInfo( w->hWND , &i ) ;
				i.rcClient.bottom -= i.rcClient.top ;
				i.rcClient.right -= i.rcClient.left ;
				i.rcWindow.bottom -= i.rcWindow.top ;
				i.rcWindow.right -= i.rcWindow.left ;
				d = GetDC( w->hWND ) ;
				GetTextMetrics( d , &w->textMetric ) ;		//----------
				w->rDC = CreateCompatibleDC( d ) ;
				w->wDC = CreateCompatibleDC( d ) ;
				ReleaseDC( w->hWND , d ) ;
				r = w->wRECT = w->rRECT = i.rcClient ;
				w->position = i.rcWindow ;
				r.left = 0 ;
				r.top = 0 ;
				w->rBM = CreateCompatibleBitmap( w->rDC , r.right , r.bottom ) ;
				w->wBM = CreateCompatibleBitmap( w->wDC , r.right , r.bottom ) ;
				SelectObject( w->rDC , w->rBM ) ;
				SelectObject( w->wDC , w->wBM ) ;
				FillRect( w->rDC, &r , (HBRUSH) (COLOR_WINDOW+1) ) ;
				FillRect( w->wDC, &r , (HBRUSH) (COLOR_WINDOW+1) ) ;
				w->caret.v	= 0 ;
				w->caret.c	= 0 ;
				w->caret.f	= 0 ;
				w->caret.s	= 0 ;
				w->caret.w	= 2 ;
				w->caret.h	= w->textMetric.tmHeight ;
				SetEvent( w->oEvent ) ;
				break ;
			case WM__POSITION:
				{
				RECT		p ;
				unsigned	f = 0 ;
				w = (wnd)GetWindowLongPtr( m.hwnd , GWLP_USERDATA ) ;
				p = *((RECT*)m.wParam) ;
				dealloc( (void*)m.wParam ) ;
				p.left -= ( w->wRECT.left - w->position.left ) ;
				p.top -= ( w->wRECT.top - w->position.top ) ;
				p.right += ( w->position.right - w->wRECT.right ) ;
				p.bottom += ( w->position.bottom - w->wRECT.bottom ) ;
				if( p.left == w->position.left && p.top == w->position.top )
					f |= SWP_NOMOVE ;
				if( p.right == w->position.right && p.bottom == w->position.bottom )
					f |= SWP_NOSIZE ;
				if( f != ( SWP_NOMOVE | SWP_NOSIZE ) )
					SetWindowPos( m.hwnd , 0 , p.left , p.top , p.right , p.bottom , SWP_DRAWFRAME | SWP_NOCOPYBITS | SWP_NOREPOSITION | SWP_NOZORDER | f ) ;
				break ;
			case WM__SETCAPTURE:
				SetCapture( m.hwnd ) ;
				break ;
			case WM__RELEASECAPTURE:
				ReleaseCapture( ) ;
				break ;
			case WM__SETTITLE:
				DefWindowProc( m.hwnd , WM_SETTEXT , 0 , m.lParam ) ;
				dealloc( (void*)m.lParam ) ;
				break ;
//			default:
//				if( ! m.hwnd ) _asm int 3 ;
//				TranslateMessage( &m ) ;
//				DispatchMessage( &m ) ;
				}
			}


	if( m.message != WM_QUIT )
		{
		MessageBox( 0 , "error" , "ATOMATRIX" , MB_OK | MB_ICONERROR ) ;
		}

	return 0 ;
	}


void aWND_sendMessage( wnd w , UINT M , WPARAM P , LPARAM L )
	{
	extern void *	(*allocm)(unsigned) ;
	wnd_msg		m ;

	m	= allocm( sizeof( struct _wnd_msg ) ) ;
	m->w	= w ;
	m->M	= M ;
	m->P	= (void*)P ;
	m->L	= (void*)L ;
	EnterCriticalSection( &mQUEUE ) ;
	if( ! w->msg )
		w->msg = m , m->p = m ,	m->n = m ;
	else
		m->p = w->msg->p , m->p->n = m , m->n = w->msg , w->msg->p = m ;
	SetEvent( w->hEvent ) ;
	LeaveCriticalSection( &mQUEUE ) ;
	}


atom aWND_front( atom A , int T , atom* V )
	{
	wnd			w ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	PostMessage( ((wnd)A->datum)->hWND , WM__FRONT , 0 , 0 ) ;
	return atominul ;
	}

atom aWND_focus( atom A , int T , atom* V )
	{
	wnd			w ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	PostMessage( ((wnd)A->datum)->hWND , WM__SETFOCUS , 0 , 0 ) ;
	return atominul ;
	}

atom aWND_show( atom A , int T , atom* V )
	{
	wnd			w ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	PostMessage( ((wnd)A->datum)->hWND , WM__SHOW , SW_SHOW , 0 ) ;
	return atominul ;
	}


atom aWND_close( atom A , int T , atom* V )
	{
	wnd			w ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	PostMessage( ((wnd)A->datum)->hWND , WM__CLOSE , 0 , 0 ) ;
	return atominul ;
	}

atom aWND_activate( atom A , int T , atom* V )
	{
	wnd			w ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	PostMessage( ((wnd)A->datum)->hWND , WM__ACTIVATE , 0 , 0 ) ;
	return atominul ;
	}

struct	_arg
	{
	HANDLE			h[2] ;
	} ;


void* aWND_getMessage_task( HANDLE h[2] )
	{
	return (void*)WaitForMultipleObjects( 2 , h , 0 , INFINITE ) ;
	}

atom aWND_getMessage( atom A , int T , atom* V )
	{
	extern atom		aWND_MSG ;
	extern atom		(*atomize)(atom) ;
	extern void		(*dealloc)(void*) ;
	extern void*		(*allocz)(unsigned) ;
	extern atom		(*addAtom)(atom,string,atom) ;
	extern atom		(*oInteger)(unsigned) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void*		(*taskSplice)(void*,void*) ;
	wnd			w ;
	wnd_msg			m ;
	atom			a ;
	HANDLE			h[2] ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	h[0]	= wndThread ;
	h[1]	= w->hEvent ;
	EnterCriticalSection( &mQUEUE ) ;
	if( ! ( m = w->msg ) )
		{
		ResetEvent( w->hEvent ) ;
		LeaveCriticalSection( &mQUEUE ) ;
		if( (DWORD)taskSplice( aWND_getMessage_task , h ) == WAIT_OBJECT_0 )
			return atominul ;
		EnterCriticalSection( &mQUEUE ) ;
		m = w->msg ;
		}
	m->p->n	= m->n ;
	m->n->p = m->p ;
	w->msg = m->n == m ? 0 : m->n ;
	LeaveCriticalSection( &mQUEUE ) ;
	a	= atomize( aWND_MSG ) ;
	A->allusions++ ;
	addAtom( a , "wnd" , A ) ;
	addAtom( a , "message" , oInteger( m->M ) ) ;
	addAtom( a , "pparam" , oInteger( (unsigned)m->P ) ) ;
	addAtom( a , "lparam" , oInteger( (unsigned)m->L ) ) ;
	if( m->M == WM_CHAR )
		{
		if( (unsigned) m->P >= 32 && (unsigned) m->P < 127 )
			{
			char*	s = allocz( 2 ) ;
			*s = (char)m->P ;
			addAtom( a , "ascii" , aAsciiString_atomize( s ) ) ;	// UPDATE: aAsciiCharacter_atomize( m->P )
			}
		}
	else if( m->M == WM_SIZING || m->M == WM_WINDOWPOSCHANGED )
		{
		RECT* r = (RECT*)m->L ;
		atom	z	= atomize( aWND_RECT ) ;
		addAtom( a , "position" , z ) ;
		addAtom( z , "left" , oInteger( r->left ) ) ;
		addAtom( z , "right" , oInteger( r->right ) ) ;
		addAtom( z , "top" , oInteger( r->top ) ) ;
		addAtom( z , "bottom" , oInteger( r->bottom ) ) ;
		dealloc( r ) ;
		}
	else if( m->M == WM_SYSCOMMAND || ( WM_MOUSEFIRST <= m->M && m->M <= WM_MOUSELAST ) )
		{
		addAtom( a , "x" , oInteger( (short)((long)m->L & 0xFFFF ) ) ) ;
		addAtom( a , "y" , oInteger( (short)((long)m->L >> 16 ) ) ) ;
		}
	dealloc( m ) ;
	return a ;
	}

atom aWND_update( atom A , int T , atom* V )
	{
	wnd		w ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	BitBlt( w->wDC , 0 , 0 , w->rRECT.right , w->rRECT.bottom ,
		w->rDC , 0 , 0 ,
		SRCCOPY
		) ;
	RedrawWindow( w->hWND , 0 , 0 , RDW_INVALIDATE|RDW_ERASE ) ;
	return atominul ;
	}

atom aWND_erase( atom A , int T , atom* V )
	{
	RECT		r ;
	wnd		w ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	r = w->rRECT ;
	r.left = r.top = 0 ;
	FillRect( w->rDC , &r , (HBRUSH) (COLOR_WINDOW+1) ) ;
	return atominul ;
	}


atom aWND_textOut( atom A , int T , atom* V )
	{
#	include "unicode.h"
	extern atom	aAsciiString ;
	extern atom	aAsciiCharacter ;
	extern atom	aUnicodedEntity ;
	extern atom	aUnicodedString ;
	extern atom	aError_INVARG ;
	extern int	(*str_len)(string) ;
	wnd		w ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	if( ! ( T == 3 || T == 4 ) )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	if( T == 4 && ! (unsigned)V[3]->datum )
		return atominul ;
	if( V[2]->precursor == aUnicodedEntity )
		{
		unicode	i = (unicode)V[2]->datum ;
		TextOutW( w->rDC , (int)V[0]->datum , (int)V[1]->datum , (LPCWSTR)&i , 1 ) ;
		}
	else
	if( V[2]->precursor == aAsciiCharacter )
		{
		byte	i = (byte)V[2]->datum ;
		TextOut( w->rDC , (int)V[0]->datum , (int)V[1]->datum , (LPCSTR)&i , 1 ) ;
		}
	else
	if( V[2]->precursor == aAsciiString )
		if( T == 3 )
	                TextOut( w->rDC , (int)V[0]->datum , (int)V[1]->datum , (string)V[2]->datum , str_len( (string)V[2]->datum ) ) ;
		else	{
			unsigned l = str_len( (string)V[2]->datum ) ;
			if( l <= (unsigned)V[3]->datum )
		                TextOut( w->rDC , (int)V[0]->datum , (int)V[1]->datum , (string)V[2]->datum , l ) ;
			else
		                TextOut( w->rDC , (int)V[0]->datum , (int)V[1]->datum , (string)V[2]->datum , (unsigned)V[2]->datum ) ;
			}
	else
	if( V[2]->precursor == aUnicodedString )
		{
		unsigned l = 0 ;
		unicodes s = V[2]->datum ;
		while( *s )
			s++ , l++ ;
		if( T == 3 )
			TextOutW( w->rDC , (int)V[0]->datum , (int)V[1]->datum , (LPCWSTR)V[2]->datum , l ) ;
		else
			if( l <= (unsigned)V[3]->datum )
		                TextOutW( w->rDC , (int)V[0]->datum , (int)V[1]->datum , (LPCWSTR)V[2]->datum , l ) ;
			else
		                TextOutW( w->rDC , (int)V[0]->datum , (int)V[1]->datum , (LPCWSTR)V[2]->datum , (unsigned)V[2]->datum ) ;

		}
	else	{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	return atominul ;
	}


atom aWND_getTextMetrics( atom A , int T , atom* V )
	{
	extern atom	aWND_TEXTMETRIC ;
	extern atom	(*atomize)(atom) ;
	extern atom	(*addAtom)(atom,string,atom) ;
	extern atom	(*oInteger)(unsigned) ;
	wnd		w ;
	atom		a ;
	TEXTMETRIC	tm ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	GetTextMetrics( w->rDC , &tm ) ;
	a	= atomize( aWND_TEXTMETRIC ) ;
	addAtom( a , "Ascent" ,		oInteger( tm.tmAscent ) ) ;
	addAtom( a , "AveCharWidth" ,	oInteger( tm.tmAveCharWidth ) ) ;
	addAtom( a , "BreakChar" ,	oInteger( tm.tmBreakChar ) ) ;		// UPDATE: wchar
	addAtom( a , "CharSet" ,	oInteger( tm.tmCharSet ) ) ;
	addAtom( a , "DefaultChar" ,	oInteger( tm.tmDefaultChar ) ) ;	// UPDATE: wchar
	addAtom( a , "Descent" ,	oInteger( tm.tmDescent ) ) ;
	addAtom( a , "DigitizedAspectX" , oInteger( tm.tmDigitizedAspectX ) ) ;
	addAtom( a , "DigitizedAspectY" , oInteger( tm.tmDigitizedAspectY ) ) ;
	addAtom( a , "ExternalLeading" , oInteger( tm.tmExternalLeading ) ) ;
	addAtom( a , "FirstChar" ,	oInteger( tm.tmFirstChar ) ) ;		// UPDATE: wchar
	addAtom( a , "Height" ,		oInteger( tm.tmHeight ) ) ;
	addAtom( a , "InternalLeading" , oInteger( tm.tmInternalLeading ) ) ;
	addAtom( a , "Italic" ,		oInteger( tm.tmItalic ) ) ;
	addAtom( a , "LastChar" ,	oInteger( tm.tmLastChar ) ) ;		// UPDATE: wchar
	addAtom( a , "MaxCharWidth" ,	oInteger( tm.tmMaxCharWidth ) ) ;
	addAtom( a , "MaxCharWidth" ,	oInteger( tm.tmMaxCharWidth ) ) ;
	addAtom( a , "Overhang" ,	oInteger( tm.tmOverhang ) ) ;
	addAtom( a , "PitchAndFamily" ,	oInteger( tm.tmPitchAndFamily ) );
	addAtom( a , "StruckOut" ,	oInteger( tm.tmStruckOut ) ) ;
	addAtom( a , "Underlined" ,	oInteger( tm.tmUnderlined ) ) ;
	addAtom( a , "Weight" ,		oInteger( tm.tmWeight ) ) ;
	return a ;
	}

atom aWND_getCharWidth( atom A , int T , atom* V )
	{
#	include		"unicode.h"
	extern atom	aAsciiString ;
	extern atom	aAsciiCharacter ;
	extern atom	aUnicodedEntity ;
	extern atom	aUnicodedString ;
	extern atom	(*aVarray_atomize_a_n)(atom,unsigned) ;
	extern atom	(*aVarray_append_a_a)(atom,atom) ;
	extern atom	(*oInteger)(unsigned) ;
	atom		a ;
	unsigned	i ;
	unsigned	t ;
	wnd		w ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	if( T == 1 )
		{
		if( V[0]->precursor == aAsciiString )
			{
			if( GetCharWidth32( w->rDC , i = *((string)V[0]->datum) , i , &t ) )
				return oInteger( t ) ;
			}
		else
		if( V[0]->precursor == aUnicodedEntity )
			{
			if( GetCharWidth32W( w->rDC , (unicode)V[0]->datum , (unicode)V[0]->datum , &t ) )
				return oInteger( t ) ;
			}
		else
		if( V[0]->precursor == aAsciiCharacter )
			{
			if( GetCharWidth32( w->rDC , (byte)V[0]->datum , (byte)V[0]->datum , &t ) )
				return oInteger( t ) ;
			}
		else
		if( V[0]->precursor == aUnicodedString )
			{
			if( GetCharWidth32W( w->rDC , i = *(unicodes)V[0]->datum , i , &t ) )
				return oInteger( t ) ;
			}
		return atominul ;
		}
	if( T != 2 )
		return atominul ;
	a	= aVarray_atomize_a_n( 0 , 0 ) ;
	if( ( i = (unsigned)V[0]->datum ) > 127 || (unsigned)V[1]->datum > 127 )
		while( i <= (unsigned)V[1]->datum )
			{
			GetCharWidth32W( w->rDC , i , i++ , &t ) ;
			aVarray_append_a_a( a , oInteger( t ) ) ;
			}
	else
		while( i <= (unsigned)V[1]->datum )
			{
			GetCharWidth32( w->rDC , i , i++ , &t ) ;
			aVarray_append_a_a( a , oInteger( t ) ) ;
			}
	return a ;
	}

atom aWND_getRectangle( atom A , int T , atom* V )
	{
	extern atom	aWND_RECT ;
	extern atom	(*atomize)(atom) ;
	extern atom	(*addAtom)(atom,string,atom) ;
	extern atom	(*oInteger)(unsigned) ;
	atom		a ;
	wnd		w ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	a	= atomize( aWND_RECT ) ;
	addAtom( a , "left" , oInteger( w->rRECT.left ) ) ;
	addAtom( a , "right" , oInteger( w->rRECT.right ) ) ;
	addAtom( a , "top" , oInteger( w->rRECT.top ) ) ;
	addAtom( a , "bottom" , oInteger( w->rRECT.bottom ) ) ;
	return a ;
	}

atom aWND_getTextExtentPoint( atom A , int T , atom* V )
	{
#	include		"unicode.h"
	extern atom	aWND_SIZE ;
	extern atom	aAsciiString ;
	extern atom	aUnicodedString ; 
	extern atom	(*atomize)(atom) ;
	extern atom	(*addAtom)(atom,string,atom) ;
	extern atom	(*oInteger)(unsigned) ;
	extern int	(*str_len)(string) ;
	atom		a ;
	wnd		w ;
	SIZE		s ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;				// UPDATE: aAsciiCharacter & aUnicodedEntity
	if( V[0]->precursor == aAsciiString )
		if( T == 1 )
			GetTextExtentPoint32( w->rDC , (string)V[0]->datum , str_len( V[0]->datum ) , &s ) ;
		else
		if( T == 2 )
			GetTextExtentPoint32( w->rDC , (string)V[0]->datum , (unsigned)V[1]->datum , &s ) ;	//UPDATE: overflow ? if 2nd arg > 1st length
		else												//      : suggest no 2nd arg
			return atominul ;
	else
	if( V[0]->precursor == aUnicodedString )
		if( T == 1 )
			{
			unsigned l = 0 ;
			unicodes u = V[0]->datum ;
			while( *u )
				u++ , l++ ;
			GetTextExtentPoint32W( w->rDC , (LPCWSTR)V[0]->datum , l , &s ) ;
			}
		else
		if( T == 2 )
			GetTextExtentPoint32W( w->rDC , (LPCWSTR)V[0]->datum , (unsigned)V[1]->datum , &s ) ;
		else
			return atominul ;
	a	= atomize( aWND_SIZE ) ;
	addAtom( a , "cx" , oInteger( s.cx ) ) ;
	addAtom( a , "cy" , oInteger( s.cy ) ) ;
	return a ;
	}


atom aWND_caret( atom A , int T , atom* V )
	{
	wnd		w ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	if( T == 0 )
		{
		w->caret.s = 0 ;
		}
	if( T == 2 )
		{
		GetTextMetrics( w->rDC , &w->textMetric ) ;
		w->caret.h = w->textMetric.tmHeight ;	// UPDATE:
		w->caret.w = 2 ;
		w->caret.y = (unsigned)V[0]->datum ;
		w->caret.x = (unsigned)V[1]->datum ;
		w->caret.s = 1 ;
		}
	return atominul ;
	}


atom aWND_font( atom A , int T , atom* V )
	{
	extern atom		(*atomize)(atom) ;
	HFONT			f ;
	atom			a ;

	if( A == aWND )
		return atominul ;
	if( T != 14 )
		return atominul ;
	f = CreateFont(
		(int)V[0]->datum ,		// nHeight
		(int)V[1]->datum ,		// nWidth
		(int)V[2]->datum ,		// nEscapement
		(int)V[3]->datum ,		// nOrientation
		(int)V[4]->datum ,		// fnWeight
		(DWORD)V[5]->datum ,		// fdwItalic
		(DWORD)V[6]->datum ,		// fdwUnderline
		(DWORD)V[7]->datum ,		// fdwStrikeOut
		(DWORD)V[8]->datum ,		// fdwCharSet
		(DWORD)V[9]->datum ,		// fdwOutputPrecision
		(DWORD)V[10]->datum ,		// fdwClipPrecision
		(DWORD)V[11]->datum ,		// fdwQuality
		(DWORD)V[12]->datum ,		// fdwPitchAndFamily
		(string)V[13]->datum		// lpszFace
		) ;
	if( ! f )
		return atominul ;
	a	= atomize( aWND_FONT ) ;
	a->datum = f ;
	return a ;
	}


atom aWND_select( atom A , int T , atom* V )
	{
	wnd			w ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	if( T != 1 )
		return atominul ;
	SelectObject( w->rDC , V[0]->datum ) ;
	return atominul ;
	}

atom aWND_resize( atom A , int T , atom* V )
	{
	wnd			w ;
	HDC	dc ;
	HBITMAP	bm ;
	HDC	odc ;
	RECT	r ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	if( T == 2 )
		{
		r.left	 = 0 ;
		r.top	 = 0 ;
		r.right	 = (LONG)V[0]->datum ;
		r.bottom = (LONG)V[1]->datum ;
		}
	else
		return atominul ;
	dc = CreateCompatibleDC( odc = w->rDC ) ;
	bm = CreateCompatibleBitmap( dc , r.right , r.bottom ) ;
	SelectObject( dc , bm ) ;
	SelectObject( dc , GetCurrentObject( odc , OBJ_BRUSH ) ) ;
	SelectObject( dc , GetCurrentObject( odc , OBJ_COLORSPACE ) ) ;
	SelectObject( dc , GetCurrentObject( odc , OBJ_FONT) ) ;
	SelectObject( dc , GetCurrentObject( odc , OBJ_PAL ) ) ;
	SelectObject( dc , GetCurrentObject( odc , OBJ_PEN ) ) ;
	FillRect( dc, &r , (HBRUSH) (COLOR_WINDOW+1) ) ;
	BitBlt( dc , 0 , 0 , w->rRECT.right , w->rRECT.bottom ,
		odc , 0 , 0 ,
		SRCCOPY
		) ;
	w->rDC	= dc ;
	w->rRECT = r ;
	DeleteObject( w->rBM ) ;
	w->rBM	= bm ;
	DeleteDC( odc ) ;
	return atominul ;
	}

atom aWND_coordinates( atom A , int T , atom* V )
	{
	extern atom		(*atomize)(atom) ;
	extern atom		(*addAtom)(atom,string,atom) ;
	extern atom		(*oInteger)(unsigned) ;
	wnd			w ;
	WINDOWINFO		i ;
	atom			a ;
	atom			z ;

	if( A == aWND || ! (w = A->datum) || T != 0 )
		return atominul ;
	GetWindowInfo( w->hWND , &i ) ;
	a	= atomize( aWND_SET ) ;
	z	= atomize( aWND_RECT ) ;
	addAtom( z , "left" , oInteger( i.rcClient.left ) ) ;
	addAtom( z , "right" , oInteger( i.rcClient.right - i.rcClient.left ) ) ;
	addAtom( z , "top" , oInteger( i.rcClient.top ) ) ;
	addAtom( z , "bottom" , oInteger( i.rcClient.bottom - i.rcClient.top ) ) ;
	addAtom( a , "client" , z ) ;
	z	= atomize( aWND_RECT ) ;
	addAtom( z , "left" , oInteger( i.rcWindow.left ) ) ;
	addAtom( z , "right" , oInteger( i.rcWindow.right - i.rcWindow.left ) ) ;
	addAtom( z , "top" , oInteger( i.rcWindow.top ) ) ;
	addAtom( z , "bottom" , oInteger( i.rcWindow.bottom - i.rcWindow.top ) ) ;
	addAtom( a , "window" , z ) ;
	return a ;
	}


atom aWND_position( atom A , int T , atom* V )
	{
	extern atom		(*addAtom)(atom,string,atom) ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*getVariable)(atom,string,variable*) ;
	extern atom		(*oInteger)(unsigned) ;
	extern void*		(*allocm)(unsigned) ;
	wnd			w ;
	RECT*			r ;

	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	if( T == 0 )
		{
		atom		a ;
		a	= atomize( aWND_RECT ) ;			// UPDATE: arrays (elsewhere) " a = { left: a , right: b , top: c } ; "
		addAtom( a , "left" , oInteger( w->rRECT.left ) ) ;
		addAtom( a , "right" , oInteger( w->rRECT.right ) ) ;
		addAtom( a , "top" , oInteger( w->rRECT.top ) ) ;
		addAtom( a , "bottom" , oInteger( w->rRECT.bottom ) ) ;
		return a ;
		}
	if( T == 1 && V[0]->precursor == aWND_RECT )
		{
		variable		v ;
		r = allocm( sizeof( RECT ) ) ;
		getVariable( V[0] , "right", &v ) ;
		r->right = (LONG)v->v.v.Object->datum ;
		getVariable( V[0] , "left", &v ) ;
		r->left = (LONG)v->v.v.Object->datum ;
		getVariable( V[0] , "bottom", &v ) ;
		r->bottom = (LONG)v->v.v.Object->datum ;
		getVariable( V[0] , "top", &v ) ;
		r->top = (LONG)v->v.v.Object->datum ;
		}
	else
		return atominul ;
	if( r->right != w->rRECT.right || r->bottom != w->rRECT.bottom )
		{
		HDC			dc ;
		HBITMAP			bm ;
		HDC			odc ;
		RECT			f ;
		dc = CreateCompatibleDC( odc = w->rDC ) ;
		bm = CreateCompatibleBitmap( dc , r->right , r->bottom ) ;
		SelectObject( dc , bm ) ;
		SelectObject( dc , GetCurrentObject( odc , OBJ_BRUSH ) ) ;
		SelectObject( dc , GetCurrentObject( odc , OBJ_COLORSPACE ) ) ;
		SelectObject( dc , GetCurrentObject( odc , OBJ_FONT) ) ;
		SelectObject( dc , GetCurrentObject( odc , OBJ_PAL ) ) ;
		SelectObject( dc , GetCurrentObject( odc , OBJ_PEN ) ) ;
		f = *r ;
		f.left = f.top = 0 ;
		FillRect( dc, &f , (HBRUSH) (COLOR_WINDOW+1) ) ;
		BitBlt( dc , 0 , 0 , w->rRECT.right , w->rRECT.bottom ,
			odc , 0 , 0 ,
			SRCCOPY
			) ;
		w->rDC	= dc ;
		DeleteObject( w->rBM ) ;
		w->rBM	= bm ;
		DeleteDC( odc ) ;
		}
	w->rRECT = *r ;
	PostMessage( w->hWND , WM__POSITION , (WPARAM)r , 0 ) ;
	return atominul ;
	} 

atom aWND_setCapture( atom A , int T , atom* V )
	{
	wnd			w ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	PostMessage( ((wnd)A->datum)->hWND , WM__SETCAPTURE , 0 , 0 ) ;
	return atominul ;
	}


atom aWND_releaseCapture( atom A , int T , atom* V )
	{
	wnd			w ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	PostMessage( ((wnd)A->datum)->hWND , WM__RELEASECAPTURE , 0 , 0 ) ;
	return atominul ;
	}


atom aWND_rectangle( atom A , int T , atom* V )
	{
	extern atom		(*addAtom)(atom,string,atom) ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*oInteger)(unsigned) ;
	atom		a ;

	if( A != aWND && T != 4 )
		return atominul ;
	a	= atomize( aWND_RECT ) ;			// UPDATE: arrays (elsewhere) " a = { left: a , right: b , top: c } ; "
	addAtom( a , "left" , oInteger( (unsigned)V[0]->datum ) ) ;
	addAtom( a , "top" , oInteger( (unsigned)V[1]->datum ) ) ;
	addAtom( a , "right" , oInteger( (unsigned)V[2]->datum ) ) ;
	addAtom( a , "bottom" , oInteger( (unsigned)V[3]->datum ) ) ;
	return a ;
	}

atom aWND_setTitle( atom A , int T , atom* V )
	{
	extern string		(*str_dup)(string) ;
	wnd			w ;
	string			s ;
	if( A == aWND || ! (w = A->datum) )
		return atominul ;
	s = str_dup( (string)V[0]->datum ) ;
	PostMessage( ((wnd)A->datum)->hWND , WM__SETTITLE , 0 , (LPARAM)s ) ;
	return atominul ;
	}