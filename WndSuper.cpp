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
		NULL, NULL, NULL);

	return ::IsWindow(m_hWnd);
}

BOOL ATLX::CWndSuper::SubclassWindow(HWND hWnd)
{
	if (m_oldWndProc)
		return FALSE; // Already subclassed?

	m_thunk2->Init((DWORD_PTR)WindowProc, this);
	WNDPROC pProc = (WNDPROC)m_thunk2->GetCodeAddress();
	m_oldWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pProc);
	if (m_oldWndProc != NULL)
		m_hWnd = hWnd;

	return (m_oldWndProc != NULL);
}

BOOL ATLX::CWndSuper::UnSubclassWindow()
{
	if (!m_oldWndProc || !::IsWindow(m_hWnd))
		return FALSE;

	WNDPROC prevWndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_oldWndProc);
	if (prevWndProc) m_oldWndProc = NULL; // Reset old window proc = null

	return (prevWndProc != NULL);
}