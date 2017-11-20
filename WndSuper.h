#pragma once

#include "WndThunker.h"
#include "atlx_string.h"

namespace ATLX{

	// Uses CWndSuper to create window or subclassing window
	class CWndSuper : public ATLX::CWndThunker
	{
	public:
		CWndSuper(void);
	public:
		virtual ~CWndSuper(void);

		virtual LRESULT ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled);

		// Create window, when you don't need the window, you should call Destory()
		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd = NULL, UINT nID = 0xFFFF);

		// Destory window
		virtual BOOL Destory();

		CWndSuper* m_pParent;
		HWND m_hWnd;

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

	protected:
		void OnPaint(HDC hdc);

		BOOL OnEraseBkgnd(HDC hdc);

		void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	};

}