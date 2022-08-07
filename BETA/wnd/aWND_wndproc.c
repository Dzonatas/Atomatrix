/********************************************
*
* aWND_wndproc.c
*
*************************/


#include "ATOMATRIX.h"
//#include "variant.h"
//#include "variable.h"
#include <windows.h>
//#include <process.h>
#include "wnd.h"


LRESULT CALLBACK aWND_wndproc( HWND H , UINT M , WPARAM W , LPARAM L )
	{
	extern void	aWND_sendMessage(wnd,UINT,WPARAM,LPARAM) ;
	extern void*	(*allocm)(unsigned) ;
	HDC		d ;
	wnd		w ;
	PAINTSTRUCT	ps ;

	w	= (wnd)GetWindowLongPtr( H , GWLP_USERDATA ) ;
#if 0
#undef __
#define __(_1,_2) \
	if( M == _2 ) dbgmsg( "M=" #_1 " ( %lx , %lx )\n" , (void*)W , (void*)L , 0 ) ;
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
	if( M == WM_PAINT )
		{
		if( w->caret.v && w->caret.f )
			{
			HideCaret( H ) ;
			w->caret.v = 0 ;
			}
		d	= BeginPaint( H , &ps ) ;
		BitBlt( d , ps.rcPaint.left , ps.rcPaint.top , ps.rcPaint.right , ps.rcPaint.bottom ,
			w->wDC , ps.rcPaint.left , ps.rcPaint.top ,
			SRCCOPY
			) ;
		EndPaint( H , &ps ) ;
		if( w->caret.f && w->caret.s )
			{
			if( w->caret.c == 0 )
				{
				CreateCaret( H , 0 , w->caret.w , w->caret.h ) ;
				w->caret.c = 1 ;
				}
			SetCaretPos( w->caret.x , w->caret.y ) ;
			ShowCaret( H ) ;
			w->caret.v = 1 ;
			}
		return 0 ;
		}
	if( M == WM_ERASEBKGND )
		return 0 ;
	if( M == WM_SIZING )
		return 1 ;
	if( M == WM_SYSCOMMAND )
		{
		if( ( W & 0xFFF0 ) == SC_SIZE )
			{
			aWND_sendMessage( w , M , W , L ) ;
			return 0 ;
			}
		if( ( W & 0xFFF0 ) == SC_CLOSE )
			{
			aWND_sendMessage( w , M , W , L ) ;
			return 0 ;
			}
		}
	if( M == WM_WINDOWPOSCHANGED )
		{
		WINDOWINFO	i ;
		RECT *		p = allocm( sizeof( RECT ) ) ;
		GetWindowInfo( H , &i ) ;
		i.rcClient.bottom -= i.rcClient.top ;
		i.rcClient.right -= i.rcClient.left ;
		i.rcWindow.bottom -= i.rcWindow.top ;
		i.rcWindow.right -= i.rcWindow.left ;
		*p = i.rcClient ;
		if( p->right != w->wRECT.right || p->bottom != w->wRECT.bottom )
			{
			HDC		dc ;
			HBITMAP		bm ;
			HDC		odc ;
			RECT		r ;
			d	= GetDC( H ) ;
			dc	= CreateCompatibleDC( d ) ;
			ReleaseDC( H , d ) ;
			bm	= CreateCompatibleBitmap( dc , p->right , p->bottom ) ;
			SelectObject( dc , bm ) ;
			r = *p ;
			r.top = r.left = 0 ;
			FillRect( dc, &r , (HBRUSH) (COLOR_WINDOW+1) ) ;
			BitBlt( dc , 0 , 0 , w->wRECT.right , w->wRECT.bottom ,	odc = w->wDC , 0 , 0 , SRCCOPY ) ;
			w->wDC	= dc ;
			DeleteObject( w->wBM ) ;
			w->wBM	= bm ;
			DeleteDC( odc ) ;
			}
		w->wRECT = *p ;
		w->position = i.rcWindow ;			// UPDATE: adjust .right and .bottom ?
		aWND_sendMessage( w , M , 0 , (LPARAM)p ) ;
		return 0 ;
		}
	if( M == WM_SIZE )
		{
		RECT r ;
		GetClientRect( H , &r ) ;
		aWND_sendMessage( w , M , r.right , r.bottom ) ;
		return 0 ;
		}
	if( M == WM_CHAR )
		{
		aWND_sendMessage( w , M , W , L ) ;
		return 0 ;
		}
	if( WM_MOUSEFIRST <= M && M <= WM_MOUSELAST )
		{
		aWND_sendMessage( w , M , W , L ) ;
		return 0 ;
		}
	if( M == WM_SETFOCUS )
		{
		if( w->caret.s )
			{
			CreateCaret( H , 0 , w->caret.w , w->caret.h ) ;
			SetCaretPos( w->caret.x , w->caret.y ) ;
			ShowCaret( H ) ;
			w->caret.v = 1 ;
			w->caret.c = 1 ;
			}
		w->caret.f = 1 ;
		aWND_sendMessage( w , M , W , L ) ;
		return 0 ;
		}
	if( M == WM_KILLFOCUS )
		{
		DestroyCaret( ) ;
		w->caret.c = 0 ;
		w->caret.f = 0 ;
		aWND_sendMessage( w , M , W , L ) ;
		return 0 ;
		}
	if( M == WM_DESTROY )
		{
		aWND_sendMessage( w , WM_QUIT , 0 , 0 ) ;
		SetEvent( w->oEvent ) ;
//		PostQuitMessage( 0 ) ;
		return 0 ;
		}
	if( M == WM_CLOSE )
		{
		aWND_sendMessage( w , M , W , L ) ;
		return 0 ;
		}
	if( M == WM_CAPTURECHANGED )
		{
		aWND_sendMessage( w , M , W , L ) ;
		return 0 ;
		}
	return DefWindowProc( H , M , W , L ) ;
	}
