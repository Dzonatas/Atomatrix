struct _wnd_msg
	{
	struct _wnd *		w ;
	unsigned		M ;
	void*			P ;
	void*			L ;
	struct _wnd_msg *	n ;
	struct _wnd_msg *	p ;
	} ;
typedef struct _wnd_msg *	wnd_msg ;

struct _wnd
	{
        HWND			hWND ;
	HANDLE			hEvent ;
	HANDLE			oEvent ;
	TEXTMETRIC		textMetric ;
	unsigned		textWidth[256] ;
	RECT			position ;
	HBITMAP			rBM ;				// render area
	HDC			rDC ;
	RECT			rRECT ;
	HBITMAP			wBM ;				// display area
	HDC			wDC ;
	RECT			wRECT ;
	wnd_msg			msg ;
	struct caret
		{
		unsigned	w ;
		unsigned	h ;
		unsigned	x ;
		unsigned	y ;
		unsigned	v ;
		unsigned	f ;
		unsigned	c ;
		unsigned	s ;
		} caret ;
	} ;

typedef struct _wnd *		wnd ;

#define WM__NUL			WM_APP+0x00
#define	WM__DESTRUCT		WM_APP+0x01
#define	WM__SHOW		WM_APP+0x02
#define	WM__CLOSE		WM_APP+0x03
#define WM__ACTIVATE		WM_APP+0x04
#define WM__SETFOCUS		WM_APP+0x05
#define WM__FRONT		WM_APP+0x06
#define WM__QUIT		WM_APP+0x07
#define WM__CONSTRUCT		WM_APP+0x08
#define WM__POSITION		WM_APP+0x09
#define WM__SETCAPTURE		WM_APP+0x0A
#define WM__RELEASECAPTURE	WM_APP+0x0B
#define WM__SETTITLE		WM_APP+0x0C
