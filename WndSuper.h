#pragma once

#include "atlx_string.h"
#include "WndThunker.h"

namespace ATLX{

	class CToolTipCtrl;

	// Uses CWndSuper to create window or subclassing window
	class CWndSuper : public ATLX::CWndThunker
	{
	public:
		CWndSuper(HINSTANCE hInst=NULL);
	public:
		virtual ~CWndSuper(void);

		virtual LRESULT ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled);
		LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

		// Create window, when you don't need the window, you should call Destory()
		// If you're creating system class (such as Static, Button, ListCtrl, Listbox, etc.), you won't receive WM_CREATE message,
		// therefore, OnCreate event won't work in this case.
		BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, CWndSuper* pParentWnd = NULL, UINT nID = 0xFFFF);
		BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd = NULL, UINT nID = 0xFFFF);
		BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, CWndSuper* pParentWnd = NULL, UINT nID = 0xFFFF);
		BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWndSuper* pParentWnd = NULL, UINT nID = 0xFFFF);

		CWndSuper* m_pParent;

		BOOL SubclassWindow(HWND hWnd);
		virtual void PreSubclassWindow();
		BOOL UnSubclassWindow();

	public:
		DWORD GetStyle() const;
		DWORD GetExStyle() const;

		BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd);
		BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd);

		void SetWindowText(TCHAR* pszText);
		ATLX::CString GetWindowText() const;

		BOOL GetWindowRect(LPRECT lpRect) const
		{
			return ::GetWindowRect(m_hWnd, lpRect);
		}

		BOOL GetClientRect(LPRECT lpRect) const
		{
			return ::GetClientRect(m_hWnd, lpRect);
		}

		BOOL ClientToScreen(LPPOINT lpPoint) const 
		{
			return ::ClientToScreen(m_hWnd, lpPoint);
		}

		BOOL ClientToScreen(LPRECT lpRect) const 
		{
			if (!::ClientToScreen(m_hWnd, (LPPOINT)lpRect))
				return FALSE;
			return ::ClientToScreen(m_hWnd, ((LPPOINT)lpRect) + 1);
		}

		BOOL ScreenToClient(LPPOINT lpPoint) const
		{
			return ::ScreenToClient(m_hWnd, lpPoint);
		}

		BOOL ScreenToClient(LPRECT lpRect) const
		{
			if (!::ScreenToClient(m_hWnd, (LPPOINT)lpRect))
				return FALSE;
			return ::ScreenToClient(m_hWnd, ((LPPOINT)lpRect) + 1);
		}

	protected:
		virtual BOOL OnCreate(LPCREATESTRUCT pcs);

		virtual void OnDestroy();

		virtual void OnPaint();

		virtual BOOL OnEraseBkgnd(HDC hdc);

		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	protected: // Tooltip filters
		BOOL EnableToolTips(CToolTipCtrl* pToolTip, BOOL bEnable, BOOL bTracking=FALSE);
		static void _stdcall _FilterToolTipMessage(MSG* pMsg, CWndSuper* pWnd);
		void FilterToolTipMessage(MSG* pMsg);

		CToolTipCtrl* m_pToolTip;
		BOOL	m_bEnableToolTip;

	protected:
		HINSTANCE m_hInst; // The instance associated 
	};

}