#include "StdAfx.h"
#include "WndSuper.h"

ATLX::CWndSuper::CWndSuper(void) : m_pParent(NULL), m_hWnd(NULL)
{
}

ATLX::CWndSuper::~CWndSuper(void)
{
}

LRESULT ATLX::CWndSuper::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled)
{
	LRESULT ret = 0;
	if (uMsg == WM_DESTROY)
	{
		// Safely un-subclassing window if that had one.
		ret = UnSubclassWindow();
	}
	if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		OnPaint(ps.hdc);
		EndPaint(m_hWnd, &ps);
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

BOOL ATLX::CWndSuper::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd/*=NULL*/, UINT nID/*=0xFFFF*/)
{
	m_thunk->Init((DWORD_PTR)StartWindowProc, this);

	HWND hWndParent = NULL;
	if (pParentWnd)
	{
		hWndParent = pParentWnd->m_hWnd;
		m_pParent = pParentWnd;
	}
	m_hWnd = ::CreateWindow(lpszClassName, 
		lpszWindowName, 
		dwStyle, 
		rect.left, 
		rect.top, 
		rect.right - rect.left, 
		rect.bottom - rect.top, 
		hWndParent,
		(HMENU)nID, NULL, NULL);

	return ::IsWindow(m_hWnd);
}

BOOL ATLX::CWndSuper::Destory()
{
	if (!::IsWindow(m_hWnd))
		return FALSE;

	return DestroyWindow(m_hWnd);
}

BOOL ATLX::CWndSuper::SubclassWindow(HWND hWnd)
{
	if (m_oldWndProc)
		return FALSE; // Already subclassed?

	PreSubclassWindow();

	m_thunk2->Init((DWORD_PTR)WindowProc, this);
	WNDPROC pProc = (WNDPROC)m_thunk2->GetCodeAddress();
	m_oldWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pProc);
	if (m_oldWndProc != NULL)
		m_hWnd = hWnd;

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

void ATLX::CWndSuper::OnPaint(HDC hdc)
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