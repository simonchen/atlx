#pragma once

#include "WndSuper.h"

namespace ATLX{

	class CEdit : public CWndSuper{
		// Constructors
	public:
		CEdit();
		BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd, UINT nID = 0xFFFF);

		// Attributes
		BOOL CanUndo() const;
		int GetLineCount() const;
		BOOL GetModify() const;
		void SetModify(BOOL bModified = TRUE);
		void GetRect(LPRECT lpRect) const;
		DWORD GetSel() const;
		void GetSel(int& nStartChar, int& nEndChar) const;
		HLOCAL GetHandle() const;
		void SetHandle(HLOCAL hBuffer);
		void SetMargins(UINT nLeft, UINT nRight);
		DWORD GetMargins() const;
		void SetLimitText(UINT nMax);
		UINT GetLimitText() const;
		void PosFromChar(UINT nChar, POINT& pt);
		int CharFromPos(POINT pt) const;

		// NOTE: first word in lpszBuffer must contain the size of the buffer!
		// NOTE: may not return null character
		int GetLine(int nIndex, LPTSTR lpszBuffer) const;
		// NOTE: may not return null character
		int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const;

#if defined(UNICODE)
		ATLX::CString GetCueBanner() const;

		BOOL ShowBalloonTip(LPCWSTR lpszTitle, LPCWSTR lpszText, INT ttiIcon = TTI_NONE);
		BOOL ShowBalloonTip(PEDITBALLOONTIP pEditBalloonTip);
		BOOL HideBalloonTip();
#endif  // (UNICODE)

#if defined(UNICODE)
		// REVIEW: Sets the characters in the edit control that are highlighted.
		void SetHighlight(int ichStart, int ichEnd);

		// REVIEW: Retrieves the characters in the edit control that are highlighted.
		BOOL GetHighlight(int* pichStart, int* pichEnd) const;
#endif  // defined(UNICODE)

		// Operations
		void EmptyUndoBuffer();
		BOOL FmtLines(BOOL bAddEOL);

		void LimitText(int nChars = 0);
		int LineFromChar(int nIndex = -1) const;
		int LineIndex(int nLine = -1) const;
		int LineLength(int nLine = -1) const;
		void LineScroll(int nLines, int nChars = 0);
		void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);
		void SetPasswordChar(TCHAR ch);
		void SetRect(LPCRECT lpRect);
		void SetRectNP(LPCRECT lpRect);
		void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE);
		void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE);
		BOOL SetTabStops(int nTabStops, LPINT rgTabStops);
		void SetTabStops();
		BOOL SetTabStops(const int& cxEachStop);    // takes an 'int'

		// Clipboard operations
		BOOL Undo();
		void Clear();
		void Copy();
		void Cut();
		void Paste();

		BOOL SetReadOnly(BOOL bReadOnly = TRUE);
		int GetFirstVisibleLine() const;
		TCHAR GetPasswordChar() const;

		// Implementation
	public:
		// virtual OK here - ~CWnd already virtual
		virtual ~CEdit();
	};

#ifdef UNICODE

	inline BOOL CEdit::ShowBalloonTip(LPCWSTR lpszTitle, LPCWSTR lpszText, INT ttiIcon)
	{
		EDITBALLOONTIP bt;
		::ZeroMemory(&bt, sizeof(EDITBALLOONTIP));

		bt.cbStruct = sizeof(EDITBALLOONTIP);
		bt.pszTitle = lpszTitle;
		bt.pszText = lpszText;
		bt.ttiIcon = ttiIcon;

		return Edit_ShowBalloonTip(m_hWnd, &bt); // EM_SHOWBALLOONTIP
	}

	inline BOOL CEdit::ShowBalloonTip(PEDITBALLOONTIP pEditBalloonTip)
	{
		if (pEditBalloonTip == NULL)
		{
			return FALSE;
		}
		return Edit_ShowBalloonTip(m_hWnd, pEditBalloonTip); // EM_SHOWBALLOONTIP
	}

	inline BOOL CEdit::HideBalloonTip()
	{
		return Edit_HideBalloonTip(m_hWnd); // EM_HIDEBALLOONTIP
	}
#endif // (UNICODE)

#if (NTDDI_VERSION >= NTDDI_VISTA) && defined(UNICODE)

	inline void CEdit::SetHighlight(int ichStart, int ichEnd)
	{
		return Edit_SetHilite(m_hWnd, ichStart, ichEnd); // EM_SETHILITE
	}

	inline BOOL CEdit::GetHighlight(_Out_ int* pichStart, _Out_ int* pichEnd) const
	{
		if (pichStart == NULL || pichEnd == NULL)
			return FALSE;

		DWORD dwHighlight = Edit_GetHilite(m_hWnd); // EM_GETHILITE
		*pichStart = (int)LOWORD(dwHighlight);
		*pichEnd = (int)HIWORD(dwHighlight);
		return TRUE;
	}

#endif // (NTDDI_VERSION >= NTDDI_VISTA) && defined(UNICODE)

	inline CEdit::CEdit()
	{

	}

	inline BOOL CEdit::CanUndo() const
	{
		return (BOOL)::SendMessage(m_hWnd, EM_CANUNDO, 0, 0);
	}
	inline int CEdit::GetLineCount() const
	{
		return (int)::SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0);
	}
	inline BOOL CEdit::GetModify() const
	{
		return (BOOL)::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0);
	}
	inline void CEdit::SetModify(BOOL bModified)
	{
		::SendMessage(m_hWnd, EM_SETMODIFY, bModified, 0);
	}
	inline void CEdit::GetRect(LPRECT lpRect) const
	{
		::SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM)lpRect);
	}
	inline void CEdit::GetSel(int& nStartChar, int& nEndChar) const
	{
		::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&nStartChar, (LPARAM)&nEndChar);
	}
	inline DWORD CEdit::GetSel() const
	{
		return DWORD(::SendMessage(m_hWnd, EM_GETSEL, 0, 0));
	}
	inline HLOCAL CEdit::GetHandle() const
	{
		return (HLOCAL)::SendMessage(m_hWnd, EM_GETHANDLE, 0, 0);
	}
	inline void CEdit::SetHandle(HLOCAL hBuffer)
	{
		::SendMessage(m_hWnd, EM_SETHANDLE, (WPARAM)hBuffer, 0);
	}
#pragma warning(push)
#pragma warning(disable: 6001 6054)
	inline int CEdit::GetLine(int nIndex, LPTSTR lpszBuffer) const
	{
		 return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}
#pragma warning(pop)
	inline int CEdit::GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
	{
		*(LPWORD)lpszBuffer = (WORD)nMaxLength;
		return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}
	inline void CEdit::EmptyUndoBuffer()
	{
		::SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0);
	}
	inline BOOL CEdit::FmtLines(BOOL bAddEOL)
	{
		return (BOOL)::SendMessage(m_hWnd, EM_FMTLINES, bAddEOL, 0);
	}
	inline void CEdit::LimitText(int nChars)
	{
		::SendMessage(m_hWnd, EM_LIMITTEXT, nChars, 0);
	}
	inline int CEdit::LineFromChar(int nIndex) const
	{
		return (int)::SendMessage(m_hWnd, EM_LINEFROMCHAR, nIndex, 0);
	}
	inline int CEdit::LineIndex(int nLine) const
	{
		return (int)::SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0);
	}
	inline int CEdit::LineLength(int nLine) const
	{
		return (int)::SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0);
	}
	inline void CEdit::LineScroll(int nLines, int nChars)
	{
		::SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines);
	}
	inline void CEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)
	{
		::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)lpszNewText);
	}
	inline void CEdit::SetPasswordChar(TCHAR ch)
	{
		::SendMessage(m_hWnd, EM_SETPASSWORDCHAR, ch, 0);
	}
	inline void CEdit::SetRect(LPCRECT lpRect)
	{
		::SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM)lpRect);
	}
	inline void CEdit::SetRectNP(LPCRECT lpRect)
	{
		::SendMessage(m_hWnd, EM_SETRECTNP, 0, (LPARAM)lpRect);
	}
	inline void CEdit::SetSel(DWORD dwSelection, BOOL bNoScroll)
	{
		::SendMessage(m_hWnd, EM_SETSEL,
			LOWORD(dwSelection), HIWORD(dwSelection));
		if (!bNoScroll)
			::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0);
	}
	inline void CEdit::SetSel(int nStartChar, int nEndChar, BOOL bNoScroll)
	{
		::SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
		if (!bNoScroll)
			::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0);
	}
	inline BOOL CEdit::SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops,
			(LPARAM)rgTabStops);
	}
	inline void CEdit::SetTabStops()
	{
		::SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0);
	}
	inline BOOL CEdit::SetTabStops(const int& cxEachStop)
	{
		return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS,
			1, (LPARAM)(LPINT)&cxEachStop);
	}
	inline BOOL CEdit::Undo()
	{
		return (BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0);
	}
	inline void CEdit::Clear()
	{
		::SendMessage(m_hWnd, WM_CLEAR, 0, 0);
	}
	inline void CEdit::Copy()
	{
		::SendMessage(m_hWnd, WM_COPY, 0, 0);
	}
	inline void CEdit::Cut()
	{
		::SendMessage(m_hWnd, WM_CUT, 0, 0);
	}
	inline void CEdit::Paste()
	{
		::SendMessage(m_hWnd, WM_PASTE, 0, 0);
	}
	inline BOOL CEdit::SetReadOnly(BOOL bReadOnly)
	{
		return (BOOL)::SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
	}
	inline int CEdit::GetFirstVisibleLine() const
	{
		return (int)::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
	}
	inline TCHAR CEdit::GetPasswordChar() const
	{
		return (TCHAR)::SendMessage(m_hWnd, EM_GETPASSWORDCHAR, 0, 0L);
	}
	inline void CEdit::SetMargins(UINT nLeft, UINT nRight)
	{
		::SendMessage(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG(nLeft, nRight));
	}
	inline DWORD CEdit::GetMargins() const
	{
		return (DWORD)::SendMessage(m_hWnd, EM_GETMARGINS, 0, 0);
	}
	inline void CEdit::SetLimitText(UINT nMax)
	{
		::SendMessage(m_hWnd, EM_SETLIMITTEXT, nMax, 0);
	}
	inline UINT CEdit::GetLimitText() const
	{
		return (UINT)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0);
	}
	inline void CEdit::PosFromChar(UINT nChar, POINT& pt)
	{
		DWORD dwRet = (DWORD)::SendMessage(m_hWnd, EM_POSFROMCHAR, nChar, 0);
		pt.x = LOWORD(dwRet); pt.y = HIWORD(dwRet);
	}
	inline int CEdit::CharFromPos(POINT pt) const
	{
		return (int)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
	}
}
