#pragma once

#include "WndSuper.h"

namespace ATLX{

	class CToolTipCtrl : public CWndSuper
	{
		// Constructors
	public:
		CToolTipCtrl();

		// Generic creator
		virtual BOOL Create(CWndSuper* pParentWnd, DWORD dwStyle = 0);

		// Generic creator allowing extended style bits
		virtual BOOL CreateEx(CWndSuper* pParentWnd, DWORD dwStyle = 0, DWORD dwStyleEx = 0);

		// Attributes
		// Retrieves the text for a tool in the ToolTip control .
		void GetText(CString& str, CWndSuper* pWnd, UINT_PTR nIDTool = 0) const;

		// Retrieves the information that the ToolTip control maintains about a tool.
		BOOL GetToolInfo(TOOLINFO& ToolInfo, CWndSuper* pWnd, UINT_PTR nIDTool = 0) const;

		// Sets the information that the ToolTip control maintains about a tool.
		void SetToolInfo(LPTOOLINFO lpToolInfo);

		// Sets a new bounding rectangle for a tool.
		void SetToolRect(CWndSuper* pWnd, UINT_PTR nIDTool, LPCRECT lpRect);

		// Retrieves a count of the tools maintained by the ToolTip control.
		int GetToolCount() const;

		// Retrieves the initial, pop-up, or reshow duration currently set for the ToolTip control.
		int GetDelayTime(DWORD dwDuration) const;

		// Sets the initial, pop-up, or reshow duration for the ToolTip control.
		void SetDelayTime(DWORD dwDuration, int iTime);

		// Retrieves the top, left, bottom, and right margins set for the ToolTip window.
		void GetMargin(LPRECT lprc) const;

		// Sets the top, left, bottom, and right margins for the ToolTip window.
		void SetMargin(LPRECT lprc);

		// Retrieves the maximum width for the ToolTip window.
		int GetMaxTipWidth() const;

		// Sets the maximum width for the ToolTip window.
		int SetMaxTipWidth(int iWidth);

		// Retrieves the background color in the ToolTip window.
		COLORREF GetTipBkColor() const;

		// Sets the background color in the ToolTip window.
		void SetTipBkColor(COLORREF clr);

		// Retrieves the text color in the ToolTip window.
		COLORREF GetTipTextColor() const;

		// Sets the text color in the ToolTip window.
		void SetTipTextColor(COLORREF clr);

		// Retrieves the information for the current tool in the ToolTip control.
		BOOL GetCurrentTool(LPTOOLINFO lpToolInfo) const;

		// Returns the width and height of the ToolTip control.
		void GetBubbleSize(LPTOOLINFO lpToolInfo, SIZE& size);

		// Operations
		// Activates or deactivates the ToolTip control.
		void Activate(BOOL bActivate);

		// Registers a tool with the ToolTip control.
		BOOL AddTool(CWndSuper* pWnd, UINT nIDText, LPCRECT lpRectTool = NULL,
			UINT_PTR nIDTool = 0);
		BOOL AddTool(CWndSuper* pWnd, LPCTSTR lpszText = LPSTR_TEXTCALLBACK,
			LPCRECT lpRectTool = NULL, UINT_PTR nIDTool = 0);

		// Removes a tool from the ToolTip control.
		void DelTool(CWndSuper* pWnd, UINT_PTR nIDTool = 0);

		// Tests a point to determine whether it is within the bounding rectangle of
		// the specified tool and, if it is, retrieves information about the tool.
		BOOL HitTest(CWndSuper* pWnd, POINT pt, LPTOOLINFO lpToolInfo) const;

		// Passes a mouse message to the ToolTip control for processing.
		void RelayEvent(LPMSG lpMsg);

		void SetDelayTime(UINT nDelay);

		// Sets the ToolTip text for a tool.
		void UpdateTipText(LPCTSTR lpszText, CWndSuper* pWnd, UINT_PTR nIDTool = 0);
		void UpdateTipText(UINT nIDText, CWndSuper* pWnd, UINT_PTR nIDTool = 0);

		// Forces the current tool to be redrawn.
		void Update();

		// Removes the displayed ToolTip window from view.
		void Pop();

		// Calculates a ToolTip control's text display rectangle
		// from its window rectangle, or the ToolTip window rectangle
		// needed to display a specified text display rectangle.
		BOOL AdjustRect(LPRECT lprc, BOOL bLarger = TRUE);

		// Adds a standard icon and title string to the ToolTip.
		BOOL SetTitle(UINT uIcon, LPCTSTR lpstrTitle);

#if defined(UNICODE)
		// Causes the ToolTip to display at the coordinates of the last mouse message.
		void Popup();

		// Retrieves information concerning the title of a tooltip control.
		void GetTitle(PTTGETTITLE pTTGetTitle) const;
#endif

		// Implementation
	public:
		void FillInToolInfo(TOOLINFO& ti, CWndSuper* pWnd, UINT_PTR nIDTool) const;
		virtual ~CToolTipCtrl();
#ifndef _AFXDLL
		virtual BOOL DestroyToolTipCtrl();
#else
		BOOL DestroyToolTipCtrl();
#endif

	protected:
		LRESULT OnDisableModal(WPARAM, LPARAM);
		LRESULT OnWindowFromPoint(WPARAM, LPARAM);
		LRESULT OnAddTool(WPARAM, LPARAM);
		void OnEnable(BOOL bEnable);

		//CMapStringToPtr m_mapString;

		friend class CWnd;
		//friend class CToolBar; // later on we will enable it [CHEN]
	};

}