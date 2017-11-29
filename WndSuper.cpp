#include "atlx.h"
#include "WndSuper.h"

BOOL ATLX::CWndSuper::PtInRect(const POINT& pt, const RECT& rect)
{
	if (pt.x >= rect.left &&
		pt.x <= rect.right &&
		pt.y >= rect.top &&
		pt.y <= rect.bottom)
		return TRUE;

	return FALSE;
}

HWND ATLX::CWndSuper::ChildWindowFromPoint(HWND hWnd, POINT& pt)
{
	ATLX_ASSERT(hWnd != NULL);

	// check child windows
	::ClientToScreen(hWnd, &pt);
	HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
	for (; hWndChild != NULL; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT))
	{
		if (::GetDlgCtrlID(hWndChild) != (WORD)-1 &&
			(::GetWindowLong(hWndChild, GWL_STYLE) & WS_VISIBLE))
		{
			// see if point hits the child window
			RECT rect;
			::GetWindowRect(hWndChild, &rect);
			if (CWndSuper::PtInRect(pt, rect))
				return hWndChild;
		}
	}

	return NULL;    // not found
}

HWND ATLX::CWndSuper::TopChildWindowFromPoint(HWND hWnd, POINT& pt)
{
	ATLX_ASSERT(hWnd != NULL);

	// ask Windows for the child window at the point
	HWND hWndRet = ::RealChildWindowFromPoint(hWnd, pt);
	if (hWndRet != NULL)
	{
		return ((hWndRet == hWnd) ? NULL : hWndRet);
	}

	// fallback: check child windows, return the topmost child that contains the point
	::ClientToScreen(hWnd, &pt);
	HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
	for (; hWndChild != NULL; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT))
	{
		if (::GetDlgCtrlID(hWndChild) != (WORD)-1 && (::GetWindowLong(hWndChild, GWL_STYLE) & WS_VISIBLE))
		{
			// see if point hits the child window
			RECT rect;
			::GetWindowRect(hWndChild, &rect);
			if (CWndSuper::PtInRect(pt, rect))
			{
				hWndRet = hWndChild;
			}
		}
	}

	return hWndRet;    // not found
}

////////////////////////////////////////////////////////////////////////////////
// CWndSuper
ATLX::CWndSuper::CWndSuper(HINSTANCE hInst/*=NULL*/) : 
m_pParent(NULL), m_hInst(hInst), m_pToolTip(NULL), m_bEnableToolTip(FALSE), m_nLastHit(-1), m_pLastHit(NULL), m_bTracking(FALSE), m_pLastInfo(NULL)
{
}

ATLX::CWndSuper::~CWndSuper(void)
{
	DestroyToolTip();
}

LRESULT ATLX::CWndSuper::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled)
{
	LRESULT ret = 0;

	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = uMsg;
	msg.wParam = wParam;
	msg.lParam = lParam;
	ATLX::CWndSuper::_FilterToolTipMessage(&msg, this);

	if (uMsg == WM_CREATE)
	{
		OnCreate((LPCREATESTRUCT)lParam);
	}
	if (uMsg == WM_DESTROY)
	{
		OnDestroy();
	}
	if (uMsg == WM_PAINT)
	{
		OnPaint();
	}
	if (uMsg == WM_ERASEBKGND)
	{
		ret = OnEraseBkgnd((HDC)wParam);
	}
	if (uMsg == WM_DRAWITEM)
	{
		DrawItem((LPDRAWITEMSTRUCT)lParam);
	}

	return ret;
}

LRESULT ATLX::CWndSuper::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
}

BOOL ATLX::CWndSuper::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const int x, const int y, const int nWidth, const int nHeight, CWndSuper* pParentWnd/*=NULL*/, UINT nID/*=0*/)
{
	HWND hWndParent = NULL;
	if (pParentWnd)
		hWndParent = pParentWnd->m_hWnd;
	m_hWnd = ::CreateWindow(lpszClassName,
		lpszWindowName,
		dwStyle,
		x,
		y,
		nWidth,
		nHeight,
		hWndParent,
		(HMENU)nID, m_hInst, NULL);

	if (::IsWindow(m_hWnd))
		m_pParent = pParentWnd;

	m_hWnd && SubclassWindow(m_hWnd);

	return ::IsWindow(m_hWnd);
}

BOOL ATLX::CWndSuper::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd/*=NULL*/, UINT nID/*=0*/)
{
	return Create(lpszClassName,
		lpszWindowName,
		dwStyle,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		pParentWnd,
		nID);
}

BOOL ATLX::CWndSuper::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, CWndSuper* pParentWnd/*=NULL*/, UINT nID/*=0*/)
{
	HWND hWndParent = NULL;
	if (pParentWnd)
		hWndParent = pParentWnd->m_hWnd;

	m_hWnd = ::CreateWindowEx(dwExStyle,
		lpszClassName,
		lpszWindowName,
		dwStyle,
		x,
		y,
		nWidth,
		nHeight,
		hWndParent,
		(HMENU)nID, m_hInst, NULL);

	if (::IsWindow(m_hWnd))
		m_pParent = pParentWnd;

	m_hWnd && SubclassWindow(m_hWnd);

	return ::IsWindow(m_hWnd);
}

BOOL ATLX::CWndSuper::CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWndSuper* pParentWnd/*=NULL*/, UINT nID/*=0*/)
{
	return CreateEx(lpszClassName,
		lpszWindowName,
		dwStyle,
		dwExStyle,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		pParentWnd,
		nID);
}

BOOL ATLX::CWndSuper::SubclassWindow(HWND hWnd)
{
	if (m_oldWndProc)
		return FALSE; // Already subclassed?

	m_thunk2->Init((DWORD_PTR)WindowProc, this);
	WNDPROC pProc = (WNDPROC)m_thunk2->GetCodeAddress();
	m_oldWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pProc);
	if (m_oldWndProc != NULL)
	{
		m_hWnd = hWnd;
		PreSubclassWindow(); // Prepare subclassing window
	}

	return (m_oldWndProc != NULL);
}

void ATLX::CWndSuper::PreSubclassWindow()
{
	// Do nothing.
}

BOOL ATLX::CWndSuper::UnSubclassWindow()
{
	if (!m_oldWndProc || !::IsWindow(m_hWnd))
		return FALSE;

	WNDPROC prevWndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_oldWndProc);
	if (prevWndProc) m_oldWndProc = NULL; // Reset old window proc = null

	return (prevWndProc != NULL);
}

DWORD ATLX::CWndSuper::GetStyle() const
{
	if (!IsWindow(m_hWnd)) return 0L;

	return (DWORD)GetWindowLong(m_hWnd, GWL_STYLE);
}

DWORD ATLX::CWndSuper::GetExStyle() const
{
	if (!IsWindow(m_hWnd)) return 0L;

	return (DWORD)GetWindowLong(m_hWnd, GWL_EXSTYLE);
}

BOOL ATLX::CWndSuper::ModifyStyle(DWORD dwRemove, DWORD dwAdd)
{
	if (!IsWindow(m_hWnd)) return FALSE;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	if (dwStyle == dwNewStyle)
		return FALSE;

	::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
}

BOOL ATLX::CWndSuper::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd)
{
	if (!IsWindow(m_hWnd)) return FALSE;

	DWORD dwStyleEx = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	DWORD dwNewStyleEx = (dwStyleEx & ~dwRemove) | dwAdd;
	if (dwStyleEx == dwNewStyleEx)
		return FALSE;

	::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewStyleEx);
}

void ATLX::CWndSuper::SetWindowText(TCHAR* pszText)
{
	if (!IsWindow(m_hWnd) || !pszText)
		return;

	::SetWindowText(m_hWnd, pszText);
}

ATLX::CString ATLX::CWndSuper::GetWindowText() const
{
	CString str;
	int nLens = GetWindowTextLength(m_hWnd) + 1; // Window text length + 1 (terminating null)
	TCHAR* szBuff = str.get_buffer(nLens);
	::GetWindowText(m_hWnd, szBuff, nLens);

	return str;
}

BOOL ATLX::CWndSuper::OnCreate(LPCREATESTRUCT pcs)
{
	return TRUE;
}

void ATLX::CWndSuper::OnDestroy()
{
	// Safely un-subclassing window if that had one.
	UnSubclassWindow();
}

void ATLX::CWndSuper::OnPaint()
{
	// Do nothing.
}

BOOL ATLX::CWndSuper::OnEraseBkgnd(HDC hdc)
{
	// Do nothing.
	return FALSE;
}

void ATLX::CWndSuper::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// Do nothing.
}