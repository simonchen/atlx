#include "atlx.h"
#include "atlx_tooltip_ctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrl

//MFC now enforces characters limit when updateing tooltip text.
//This is to prevent buffer overrun in GetText.
const size_t MAX_TIP_TEXT_LENGTH = 1024;

//BEGIN_MESSAGE_MAP(CToolTipCtrl, CWnd)
//	ON_MESSAGE(WM_DISABLEMODAL, &CToolTipCtrl::OnDisableModal)
//	ON_MESSAGE(TTM_WINDOWFROMPOINT, &CToolTipCtrl::OnWindowFromPoint)
//	ON_MESSAGE(TTM_ADDTOOL, &CToolTipCtrl::OnAddTool)
//	ON_WM_ENABLE()
//END_MESSAGE_MAP()

ATLX::CToolTipCtrl::CToolTipCtrl(HINSTANCE hInst/*=NULL*/) : ATLX::CWndSuper(hInst)
{
	m_pLastHit = NULL;
}

BOOL ATLX::CToolTipCtrl::Create(CWndSuper* pParentWnd, DWORD dwStyle /* = 0 */)
{
	// initialize common controls
	INITCOMMONCONTROLSEX init;
	init.dwICC = ICC_BAR_CLASSES;
	init.dwSize = sizeof(init);
	::InitCommonControlsEx(&init);

	BOOL bResult = CWndSuper::CreateEx(NULL, TOOLTIPS_CLASS,
		WS_POPUP | dwStyle, // force WS_POPUP
		NULL,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		pParentWnd);

	return bResult;
}

BOOL ATLX::CToolTipCtrl::CreateEx(CWndSuper* pParentWnd, DWORD dwStyle /* = 0 */,
	DWORD dwExStyle /* = 0 */)
{
	return Create(pParentWnd, dwStyle) &&
		ModifyStyleEx(0, dwExStyle);
}


ATLX::CToolTipCtrl::~CToolTipCtrl()
{
	DestroyWindow();
}

BOOL ATLX::CToolTipCtrl::DestroyToolTipCtrl()
{
	BOOL bDestroy = FALSE;

	if (bDestroy)
	{
		bDestroy = DestroyWindow();
		delete this;
	}
	return bDestroy;
}

LRESULT ATLX::CToolTipCtrl::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled)
{
	if (uMsg == TTM_ADDTOOL)
	{
		OnAddTool(wParam, lParam);
	}
	if (uMsg == WM_ENABLE)
	{
		OnEnable((BOOL)wParam);
	}
	if (uMsg == TTM_ACTIVATE)
	{
		OnEnable((BOOL)wParam);
	}

	return 0;
}

LRESULT ATLX::CToolTipCtrl::OnAddTool(WPARAM wParam, LPARAM lParam)
{
	TOOLINFO ti;
	LPTOOLINFO lpti = (LPTOOLINFO)lParam;
	memcpy_s(&ti, sizeof(TOOLINFO), lpti, lpti->cbSize);
	if ((ti.hinst == NULL) && (ti.lpszText != LPSTR_TEXTCALLBACK)
		&& (ti.lpszText != NULL))
	{
		void* pv;
		if (!m_mapString.Lookup(ti.lpszText, pv))
			m_mapString.SetAt(ti.lpszText, NULL);
		// set lpszText to point to the permanent memory associated
		// with the CString
		LPCTSTR lpszText = NULL;
		ATLX_ASSERT(m_mapString.LookupKey(ti.lpszText, lpszText));
		ti.lpszText = const_cast<LPTSTR>(lpszText);
	}

	return 0; // Default window proc.
}

LRESULT ATLX::CToolTipCtrl::OnDisableModal(WPARAM, LPARAM)
{
	SendMessage(TTM_ACTIVATE, FALSE);
	return FALSE;
}

void ATLX::CToolTipCtrl::OnEnable(BOOL bEnable)
{
	SendMessage(TTM_ACTIVATE, bEnable);
}

LRESULT ATLX::CToolTipCtrl::OnWindowFromPoint(WPARAM, LPARAM lParam)
{
	ASSERT(lParam != NULL);

	// the default implementation of tooltips just calls WindowFromPoint
	// which does not work for certain kinds of combo boxes
	POINT& pt = *(POINT*)lParam;
	HWND hWnd = ::WindowFromPoint(pt);
	if (hWnd == NULL)
		return 0;

	// try to hit combobox instead of edit control for CBS_DROPDOWN styles
	HWND hWndTemp = ::GetParent(hWnd);
	if (hWndTemp != NULL && (::GetWindowLong(hWndTemp, GWL_STYLE) & CBS_DROPDOWN))
		return (LRESULT)hWndTemp;

	// handle special case of disabled child windows
	::ScreenToClient(hWnd, &pt);
	hWndTemp = ::ChildWindowFromPoint(hWnd, pt);
	if (hWndTemp != NULL && !::IsWindowEnabled(hWndTemp))
		return (LRESULT)hWndTemp;

	return (LRESULT)hWnd;
}

BOOL ATLX::CToolTipCtrl::AddTool(CWndSuper* pWnd, LPCTSTR lpszText, LPCRECT lpRectTool,
	UINT_PTR nIDTool)
{
	ATLX_ASSERT(::IsWindow(m_hWnd));
	ATLX_ASSERT(pWnd != NULL);
	ATLX_ASSERT(lpszText != NULL);
	// the toolrect and toolid must both be zero or both valid
	ATLX_ASSERT((lpRectTool != NULL && nIDTool != 0) ||
		(lpRectTool == NULL) && (nIDTool == 0));

	TOOLINFO ti;
	FillInToolInfo(ti, pWnd, nIDTool);
	if (lpRectTool != NULL)
	{
		memcpy_s(&ti.rect, sizeof(RECT), lpRectTool, sizeof(RECT));
	}

	ti.lpszText = (LPTSTR)lpszText;
	return (BOOL) ::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
}

BOOL ATLX::CToolTipCtrl::AddTool(CWndSuper* pWnd, UINT nIDText, LPCRECT lpRectTool,
	UINT_PTR nIDTool)
{
	ATLX_ASSERT(::IsWindow(m_hWnd));
	ATLX_ASSERT(nIDText != 0);
	ATLX_ASSERT(pWnd != NULL);
	// the toolrect and toolid must both be zero or both valid
	ATLX_ASSERT((lpRectTool != NULL && nIDTool != 0) ||
		(lpRectTool == NULL) && (nIDTool == 0));

	TOOLINFO ti;
	FillInToolInfo(ti, pWnd, nIDTool);
	if (lpRectTool != NULL)
	{
		memcpy_s(&ti.rect, sizeof(RECT), lpRectTool, sizeof(RECT));
	}

	ti.hinst = ::FindResource(m_hInst, MAKEINTRESOURCE(nIDText), //MAKEINTRESOURCE((nIDText >> 4) + 1),
		RT_STRING) ? m_hInst : NULL;
	ASSERT(ti.hinst != NULL);
	ti.lpszText = (LPTSTR)MAKEINTRESOURCE(nIDText);
	return (BOOL) ::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
}

void ATLX::CToolTipCtrl::DelTool(CWndSuper* pWnd, UINT_PTR nIDTool)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pWnd != NULL);

	TOOLINFO ti;
	FillInToolInfo(ti, pWnd, nIDTool);
	::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)&ti);
}

void ATLX::CToolTipCtrl::GetText(CString& str, CWndSuper* pWnd, UINT_PTR nIDTool) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pWnd != NULL);

	TOOLINFO ti;
	FillInToolInfo(ti, pWnd, nIDTool);
	//Dogtag to detect buffer overruns in TTM_GETTEXT (Cannot query for text len).
	const TCHAR _OverrunDetector[] = _T("M\0FC");
	ti.lpszText = str.GetBuffer(MAX_TIP_TEXT_LENGTH + _countof(_OverrunDetector));
	memset(ti.lpszText, 0, MAX_TIP_TEXT_LENGTH*sizeof(TCHAR));
	memcpy_s(ti.lpszText + MAX_TIP_TEXT_LENGTH, sizeof(_OverrunDetector),
		_OverrunDetector, sizeof(_OverrunDetector));
	::SendMessage(m_hWnd, TTM_GETTEXT, 0, (LPARAM)&ti);
	ATLX_ASSERT(memcmp(_OverrunDetector, ti.lpszText + MAX_TIP_TEXT_LENGTH, sizeof(_OverrunDetector)) == 0);
}

BOOL ATLX::CToolTipCtrl::GetToolInfo(TOOLINFO& ToolInfo, CWndSuper* pWnd,
	UINT_PTR nIDTool) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pWnd != NULL);

	FillInToolInfo(ToolInfo, pWnd, nIDTool);
	return (BOOL)::SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ToolInfo);
}

BOOL ATLX::CToolTipCtrl::HitTest(CWndSuper* pWnd, const POINT& pt, LPTOOLINFO lpToolInfo) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pWnd != NULL);
	ASSERT(lpToolInfo != NULL);

	TTHITTESTINFO hti;
	memset(&hti, 0, sizeof(hti));
	hti.ti.cbSize = sizeof(TOOLINFO);
	hti.hwnd = pWnd->m_hWnd;
	hti.pt.x = pt.x;
	hti.pt.y = pt.y;

	if ((BOOL)::SendMessage(m_hWnd, TTM_HITTEST, 0, (LPARAM)&hti))
	{
		memcpy_s(lpToolInfo, sizeof(TOOLINFO), &hti.ti, sizeof(TOOLINFO));
		return TRUE;
	}

	return FALSE;
}

void ATLX::CToolTipCtrl::SetToolRect(CWndSuper* pWnd, UINT_PTR nIDTool, LPCRECT lpRect)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pWnd != NULL);
	ASSERT(nIDTool != 0);

	TOOLINFO ti;
	FillInToolInfo(ti, pWnd, nIDTool);
	memcpy_s(&ti.rect, sizeof(RECT), lpRect, sizeof(RECT));
	::SendMessage(m_hWnd, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
}

void ATLX::CToolTipCtrl::UpdateTipText(LPCTSTR lpszText, CWndSuper* pWnd, UINT_PTR nIDTool)
{
	ATLX_ASSERT(::IsWindow(m_hWnd));
	ATLX_ASSERT(pWnd != NULL);

	if (lpszText != LPSTR_TEXTCALLBACK)
	{
		ATLX_ASSERT(lstrlen(lpszText) <= MAX_TIP_TEXT_LENGTH);
	}

	TOOLINFO ti;
	FillInToolInfo(ti, pWnd, nIDTool);
	ti.lpszText = (LPTSTR)lpszText;
	::SendMessage(m_hWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
}

void ATLX::CToolTipCtrl::UpdateTipText(UINT nIDText, CWndSuper* pWnd, UINT_PTR nIDTool)
{
	ASSERT(nIDText != 0);

	CString str;
	str.LoadString(nIDText);
	UpdateTipText(str, pWnd, nIDTool);
}

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrl Implementation

void ATLX::CToolTipCtrl::FillInToolInfo(TOOLINFO& ti, CWndSuper* pWnd, UINT_PTR nIDTool) const
{
	memset(&ti, 0, sizeof(TOOLINFO));
	ti.cbSize = sizeof(TOOLINFO);
	HWND hwnd = pWnd->m_hWnd;
	if (nIDTool == 0)
	{
		ti.hwnd = ::GetParent(hwnd);
		ti.uFlags = TTF_IDISHWND;
		ti.uId = (UINT_PTR)hwnd;
	}
	else
	{
		ti.hwnd = hwnd;
		ti.uFlags = 0;
		ti.uId = nIDTool;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWndSuper tooltip support
BOOL ATLX::CWndSuper::EnableToolTips(CToolTipCtrl* pToolTip, BOOL bEnable, BOOL bTracking)
{
	ATLX_ASSERT(pToolTip);

	if (!bEnable)
	{
		// cancel tooltip if this window is active
		if (pToolTip->m_pLastHit == this)
			pToolTip->SendMessage(TTM_ACTIVATE, FALSE);

		// remove "dead-area" toolbar
		if (pToolTip->m_hWnd != NULL)
		{
			TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_IDISHWND;
			ti.hwnd = m_hWnd;
			ti.uId = (UINT_PTR)m_hWnd;
			pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)&ti);
		}

		m_bEnableToolTip = FALSE;

		// success
		return TRUE;
	}

	// if already enabled for tooltips, nothing to do
	if (!m_bEnableToolTip)
	{
		// success [CHEN] TODO.
		//AFX_MODULE_STATE* pModuleState = _AFX_CMDTARGET_GETSTATE();
		//pModuleState->m_pfnFilterToolTipMessage = &CWnd::_FilterToolTipMessage;
		m_bEnableToolTip = TRUE;
	}
	return TRUE;
}

void RelayToolTipMessage(ATLX::CToolTipCtrl* pToolTip, MSG* pMsg)
{
	// transate the message based on TTM_WINDOWFROMPOINT
	MSG msg = *pMsg;
	msg.hwnd = (HWND)pToolTip->SendMessage(TTM_WINDOWFROMPOINT, 0, (LPARAM)&msg.pt);
	POINT& pt = pMsg->pt;
	if (msg.message >= WM_MOUSEFIRST && msg.message <= AFX_WM_MOUSELAST)
		::ScreenToClient(msg.hwnd, &pt);
	msg.lParam = MAKELONG(pt.x, pt.y);

	// relay mouse event before deleting old tool
	pToolTip->SendMessage(TTM_RELAYEVENT, 0, (LPARAM)&msg);
}

void _stdcall ATLX::CWndSuper::_FilterToolTipMessage(MSG* pMsg, CWndSuper* pWnd)
{
	pWnd->FilterToolTipMessage(pMsg);
}

void ATLX::CWndSuper::FilterToolTipMessage(MSG* pMsg)
{
	// this CWnd has tooltips enabled
	UINT message = pMsg->message;
	if ((message == WM_MOUSEMOVE || message == WM_NCMOUSEMOVE ||
		message == WM_LBUTTONUP || message == WM_RBUTTONUP ||
		message == WM_MBUTTONUP) &&
		(GetKeyState(VK_LBUTTON) >= 0 && GetKeyState(VK_RBUTTON) >= 0 &&
		GetKeyState(VK_MBUTTON) >= 0))
	{
		// make sure that tooltips are not already being handled
		CWndSuper* pWnd = this;
		while (pWnd && pWnd->m_bEnableToolTip)
		{
			pWnd = pWnd->m_pParent;
		}
		if (pWnd != this)
		{
			if (pWnd == NULL)
			{
				// tooltips not enabled on this CWnd, clear last state data
			}
			return;
		}

		CToolTipCtrl* pToolTip = m_pToolTip;
		if (!pToolTip) return;

		// TODO...

		/*
		CWndSuper* pOwner = GetParentOwner();
		if (pToolTip != NULL && pToolTip->GetOwner() != pOwner)
		{
			pToolTip->DestroyWindow();
			delete pToolTip;
			pModuleThreadState->m_pToolTip = NULL;
			pToolTip = NULL;
		}

		if (pToolTip == NULL)
		{
			pToolTip = new CToolTipCtrl;
			if (!pToolTip->Create(pOwner, TTS_ALWAYSTIP))
			{
				delete pToolTip;
				return;
			}
			pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
			pModuleThreadState->m_pToolTip = pToolTip;
		}
		*/

		TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));

		// determine which tool was hit
		POINT& point = pMsg->pt;
		::ScreenToClient(m_hWnd, &point);
		TOOLINFO tiHit; memset(&tiHit, 0, sizeof(TOOLINFO));
		tiHit.cbSize = sizeof(TOOLINFO);
		INT_PTR nHit = OnToolHitTest(point, &tiHit);

		// build new toolinfo and if different than current, register it
		CWndSuper* pHitWnd = nHit == -1 ? NULL : this;
		if (pModuleThreadState->m_nLastHit != nHit || pModuleThreadState->m_pLastHit != pHitWnd)
		{
			if (nHit != -1)
			{
				// add new tool and activate the tip
				ti = tiHit;
				ti.uFlags &= ~(TTF_NOTBUTTON | TTF_ALWAYSTIP);
				if (m_nFlags & WF_TRACKINGTOOLTIPS)
					ti.uFlags |= TTF_TRACK;
				VERIFY(pToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti));
				if ((tiHit.uFlags & TTF_ALWAYSTIP) || IsTopParentActive())
				{
					// allow the tooltip to popup when it should
					pToolTip->SendMessage(TTM_ACTIVATE, TRUE);
					if (m_nFlags & WF_TRACKINGTOOLTIPS)
						pToolTip->SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

					// bring the tooltip window above other popup windows
					::SetWindowPos(pToolTip->m_hWnd, HWND_TOP, 0, 0, 0, 0,
						SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
				}
			}
			else
			{
				pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
			}

			// relay mouse event before deleting old tool
			RelayToolTipMessage(pToolTip, pMsg);

			// now safe to delete the old tool
			if (pModuleThreadState->m_pLastInfo != NULL &&
				pModuleThreadState->m_pLastInfo->cbSize >= sizeof(AFX_OLDTOOLINFO))
				pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)pModuleThreadState->m_pLastInfo);

			pModuleThreadState->m_pLastHit = pHitWnd;
			pModuleThreadState->m_nLastHit = nHit;
			if (pModuleThreadState->m_pLastInfo == NULL)
			{
				pModuleThreadState->m_pLastInfo = new TOOLINFO;
				memset(pModuleThreadState->m_pLastInfo, 0, sizeof(TOOLINFO));
			}
			*pModuleThreadState->m_pLastInfo = tiHit;
		}
		else
		{
			if (m_nFlags & WF_TRACKINGTOOLTIPS)
			{
				POINT pt;

				::GetCursorPos(&pt);
				pToolTip->SendMessage(TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
			}
			else
			{
				// relay mouse events through the tooltip
				if (nHit != -1)
					RelayToolTipMessage(pToolTip, pMsg);
			}
		}

		if ((tiHit.lpszText != LPSTR_TEXTCALLBACK) && (tiHit.hinst == 0))
			free(tiHit.lpszText);
	}
	else if (m_nFlags & (WF_TOOLTIPS | WF_TRACKINGTOOLTIPS))
	{
		// make sure that tooltips are not already being handled
		CWndSuper* pWnd = CWnd::FromHandle(pMsg->hwnd);
		while (pWnd != NULL && pWnd != this && !(pWnd->m_nFlags & (WF_TOOLTIPS | WF_TRACKINGTOOLTIPS)))
			pWnd = pWnd->GetParent();
		if (pWnd != this)
			return;

		BOOL bKeys = (message >= WM_KEYFIRST && message <= WM_KEYLAST) ||
			(message >= WM_SYSKEYFIRST && message <= WM_SYSKEYLAST);
		if ((m_nFlags & WF_TRACKINGTOOLTIPS) == 0 &&
			(bKeys ||
			(message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK) ||
			(message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK) ||
			(message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK) ||
			(message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK) ||
			(message == WM_NCRBUTTONDOWN || message == WM_NCRBUTTONDBLCLK) ||
			(message == WM_NCMBUTTONDOWN || message == WM_NCMBUTTONDBLCLK)))
		{
			CWnd::CancelToolTips(bKeys);
		}
	}
}
