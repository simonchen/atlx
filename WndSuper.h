/**********************************************************************
*
* Copyright (c) 2017 Simon Chen
*
***********************************************************************/

#pragma once

#include "WndThunker.h"

namespace ATLX{

	// Uses CWndSuper to create window or subclassing window
	class CWndSuper : public ATLX::CWndThunker
	{
	public:
		CWndSuper(void);
	public:
		virtual ~CWndSuper(void);

		virtual LRESULT ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled);

		virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd = NULL, UINT nID = 0xFFFF);

		CWndSuper* m_pParent;
		HWND m_hWnd;

		BOOL SubclassWindow(HWND hWnd);
		BOOL UnSubclassWindow();
	};

}