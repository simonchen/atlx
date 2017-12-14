#pragma once

#include "WndSuper.h"

namespace ATLX{

	class CButton : public CWndSuper{
		// Constructors
	public:
		CButton();
		virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle,
			const RECT& rect, CWndSuper* pParentWnd, UINT nID = 0xFFFF);

		// Attributes
		UINT GetState() const;
		void SetState(BOOL bHighlight);
		int GetCheck() const;
		void SetCheck(int nCheck);
		UINT GetButtonStyle() const;
		void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE);

		HICON SetIcon(HICON hIcon);
		HICON GetIcon() const;
		HBITMAP SetBitmap(HBITMAP hBitmap);
		HBITMAP GetBitmap() const;
		HCURSOR SetCursor(HCURSOR hCursor);
		HCURSOR GetCursor();

	protected:
		virtual void PreSubclassWindow();

		// Overridables (for owner draw only)
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual BOOL OnEraseBkgnd(HDC hdc);

		// Implementation
	public:
		virtual ~CButton();
	};

	inline CButton::CButton()
	{

	}

	inline UINT CButton::GetState() const
	{
		return (UINT)::SendMessage(m_hWnd, BM_GETSTATE, 0, 0);
	}
	inline void CButton::SetState(BOOL bHighlight)
	{
		::SendMessage(m_hWnd, BM_SETSTATE, bHighlight, 0);
	}
	inline int CButton::GetCheck() const
	{
		return (int)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0);
	}
	inline void CButton::SetCheck(int nCheck)
	{
		::SendMessage(m_hWnd, BM_SETCHECK, nCheck, 0);
	}
	inline UINT CButton::GetButtonStyle() const
	{
		return (UINT)GetWindowLong(m_hWnd, GWL_STYLE) & 0xFFFF;
	}
	inline void CButton::SetButtonStyle(UINT nStyle, BOOL bRedraw)
	{
		::SendMessage(m_hWnd, BM_SETSTYLE, nStyle, (LPARAM)bRedraw);
	}
	// Win4
	inline HICON CButton::SetIcon(HICON hIcon)
	{
		return (HICON)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}
	inline HICON CButton::GetIcon() const
	{
		return (HICON)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0L);
	}
	inline HBITMAP CButton::SetBitmap(HBITMAP hBitmap)
	{
		return (HBITMAP)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}
	inline HBITMAP CButton::GetBitmap() const
	{
		return (HBITMAP)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0L);
	}
	inline HCURSOR CButton::SetCursor(HCURSOR hCursor)
	{
		return (HCURSOR)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}
	inline HCURSOR CButton::GetCursor()
	{
		return (HCURSOR)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_CURSOR, 0L);
	}
}
