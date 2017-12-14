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

	BOOL bResult = CWndSuper::CreateEx(TOOLTIPS_CLASS, NULL,
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
	if (uMsg == TTM_WINDOWFROMPOINT)
	{
		LRESULT ret = OnWindowFromPoint(wParam, lParam);
		if (ret)
		{
			bMsgHandled = TRUE;
			return ret;
		}
	}
	if (uMsg == WM_ENABLE)
	{
		OnEnable((BOOL)wParam);
	}
	if (uMsg == TTM_ACTIVATE)
	{
		//OnEnable((BOOL)wParam);
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
	ATLX_ASSERT(lParam != NULL);

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
	hWndTemp = CWndSuper::ChildWindowFromPoint(hWnd, pt);
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
	ATLX_ASSERT(ti.hinst != NULL);
	ti.lpszText = (LPTSTR)MAKEINTRESOURCE(nIDText);
	return (BOOL) ::SendMessage(m_hWnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
}

void ATLX::CToolTipCtrl::DelTool(CWndSuper* pWnd, UINT_PTR nIDTool)
{
	ATLX_ASSERT(::IsWindow(m_hWnd));
	ATLX_ASSERT(pWnd != NULL);

	TOOLINFO ti;
	FillInToolInfo(ti, pWnd, nIDTool);
	::SendMessage(m_hWnd, TTM_DELTOOL, 0, (LPARAM)&ti);
}

void ATLX::CToolTipCtrl::GetText(CString& str, CWndSuper* pWnd, UINT_PTR nIDTool) const
{
	ATLX_ASSERT(::IsWindow(m_hWnd));
	ATLX_ASSERT(pWnd != NULL);

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
	ATLX_ASSERT(::IsWindow(m_hWnd));
	ATLX_ASSERT(pWnd != NULL);

	FillInToolInfo(ToolInfo, pWnd, nIDTool);
	return (BOOL)::SendMessage(m_hWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ToolInfo);
}

BOOL ATLX::CToolTipCtrl::HitTest(CWndSuper* pWnd, const POINT& pt, LPTOOLINFO lpToolInfo) const
{
	ATLX_ASSERT(::IsWindow(m_hWnd));
	ATLX_ASSERT(pWnd != NULL);
	ATLX_ASSERT(lpToolInfo != NULL);

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
	ATLX_ASSERT(::IsWindow(m_hWnd));
	ATLX_ASSERT(pWnd != NULL);
	ATLX_ASSERT(nIDTool != 0);

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
	ATLX_ASSERT(nIDText != 0);

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
// extra defined TTF_ flags for TOOLINFO::uFlags
#define TTF_NOTBUTTON       0x80000000L // no status help on buttondown
#define TTF_ALWAYSTIP       0x40000000L // always show the tip even if not active

void ATLX::CWndSuper::DestroyToolTip()
{
	if (m_pToolTip)
	{
		delete m_pToolTip;
		m_pToolTip = NULL;
	}

	if (m_pLastInfo)
	{
		delete m_pLastInfo;
		m_pLastInfo = NULL;
	}
}

INT_PTR ATLX::CWndSuper::OnToolHitTest(POINT& point, TOOLINFO* pTI) const
{
	// find child window which hits the point
	// (don't use WindowFromPoint, because it ignores disabled windows)
	HWND hWndChild = CWndSuper::TopChildWindowFromPoint(m_hWnd, point);
	if (hWndChild != NULL)
	{
		// return positive hit if control ID isn't -1
		INT_PTR nHit = ::GetDlgCtrlID(hWndChild);

		// hits against child windows always center the tip
		if (pTI != NULL && pTI->cbSize >= sizeof(TOOLINFO))
		{
			// setup the TOOLINFO structure
			pTI->hwnd = m_hWnd;
			pTI->uId = (UINT_PTR)hWndChild;
			pTI->uFlags |= TTF_IDISHWND;
			pTI->lpszText = LPSTR_TEXTCALLBACK;

			// set TTF_NOTBUTTON and TTF_CENTERTIP if it isn't a button
			if (!(::SendMessage(hWndChild, WM_GETDLGCODE, 0, 0) & DLGC_BUTTON))
				pTI->uFlags |= TTF_NOTBUTTON | TTF_CENTERTIP;
		}
		return nHit;
	}
	return -1;  // not found
}

BOOL ATLX::CWndSuper::EnableToolTips(BOOL bEnable, BOOL bTracking)
{
	if (!bEnable)
	{
		CWndSuper* pTopWnd = GetTopLevelParent();
		if (!pTopWnd) pTopWnd = this;
		if (pTopWnd && pTopWnd->m_pToolTip)
		{
			// cancel tooltip if this window is active
			if (pTopWnd->m_pToolTip->m_pLastHit == this)
				pTopWnd->m_pToolTip->SendMessage(TTM_ACTIVATE, FALSE);

			// remove "dead-area" toolbar
			if (pTopWnd->m_pToolTip->m_hWnd != NULL)
			{
				TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));
				ti.cbSize = sizeof(TOOLINFO);
				ti.uFlags = TTF_IDISHWND;
				ti.hwnd = m_hWnd;
				ti.uId = (UINT_PTR)m_hWnd;
				pTopWnd->m_pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)&ti);
			}
		}

		m_bEnableToolTip = FALSE;
		m_bTracking = bTracking;

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
		m_bTracking = bTracking;
	}
	return TRUE;
}

void RelayToolTipMessage(ATLX::CToolTipCtrl* pToolTip, MSG* pMsg)
{
	// transate the message based on TTM_WINDOWFROMPOINT
	MSG msg = *pMsg;
	msg.hwnd = (HWND)pToolTip->SendMessage(TTM_WINDOWFROMPOINT, 0, (LPARAM)&msg.pt);
	POINT& pt = pMsg->pt;
	if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
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

		// Created tool tip on the top level window
		CWndSuper* pTopWnd = GetTopLevelParent();
		if (!pTopWnd) pTopWnd = this; // if top level window doesn't exist, then tool tip will be created on this child window

		if (pTopWnd->m_pToolTip == NULL)
		{
			pTopWnd->m_pToolTip = new CToolTipCtrl;
			if (!pTopWnd->m_pToolTip->Create(pTopWnd, TTS_ALWAYSTIP))
			{
				delete pTopWnd->m_pToolTip;
				pTopWnd->m_pToolTip = NULL;
				return;
			}
			pTopWnd->m_pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
		}

		TOOLINFO ti; memset(&ti, 0, sizeof(TOOLINFO));

		// determine which tool was hit
		POINT& point = pMsg->pt;
		::ScreenToClient(m_hWnd, &point);
		TOOLINFO tiHit; memset(&tiHit, 0, sizeof(TOOLINFO));
		tiHit.cbSize = sizeof(TOOLINFO);
		INT_PTR nHit = OnToolHitTest(point, &tiHit);

		// build new toolinfo and if different than current, register it
		CWndSuper* pHitWnd = nHit == -1 ? NULL : this;
		if (pTopWnd->m_nLastHit != nHit || pTopWnd->m_pLastHit != pHitWnd)
		{
			if (nHit != -1)
			{
				// add new tool and activate the tip
				ti = tiHit;
				ti.uFlags &= ~(TTF_NOTBUTTON | TTF_ALWAYSTIP);
				if (m_bTracking)
					ti.uFlags |= TTF_TRACK;
				pTopWnd->m_pToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
				if ((tiHit.uFlags & TTF_ALWAYSTIP) || IsTopParentActive())
				{
					// allow the tooltip to popup when it should
					pTopWnd->m_pToolTip->SendMessage(TTM_ACTIVATE, TRUE);
					if (m_bTracking)
						pTopWnd->m_pToolTip->SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

					// bring the tooltip window above other popup windows
					::SetWindowPos(pTopWnd->m_pToolTip->m_hWnd, HWND_TOP, 0, 0, 0, 0,
						SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
				}
			}
			else
			{
				pTopWnd->m_pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
			}

			// relay mouse event before deleting old tool
			RelayToolTipMessage(pTopWnd->m_pToolTip, pMsg);

			// now safe to delete the old tool
			if (pTopWnd->m_pLastInfo != NULL &&
				pTopWnd->m_pLastInfo->cbSize >= sizeof(TOOLINFO))
				pTopWnd->m_pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)pTopWnd->m_pLastInfo);

			pTopWnd->m_pLastHit = pHitWnd;
			pTopWnd->m_nLastHit = nHit;
			if (pTopWnd->m_pLastInfo == NULL)
			{
				pTopWnd->m_pLastInfo = new TOOLINFO;
				memset(pTopWnd->m_pLastInfo, 0, sizeof(TOOLINFO));
			}
			*pTopWnd->m_pLastInfo = tiHit;
		}
		else
		{
			if (m_bTracking)
			{
				POINT pt;

				::GetCursorPos(&pt);
				pTopWnd->m_pToolTip->SendMessage(TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
			}
			else
			{
				// relay mouse events through the tooltip
				if (nHit != -1)
					RelayToolTipMessage(pTopWnd->m_pToolTip, pMsg);
			}
		}

		if ((tiHit.lpszText != LPSTR_TEXTCALLBACK) && (tiHit.hinst == 0))
			free(tiHit.lpszText);
	}
	else if (m_bEnableToolTip)
	{
		// make sure that tooltips are not already being handled
		BOOL bKeys = (message >= WM_KEYFIRST && message <= WM_KEYLAST) ||
			(message >= WM_SYSKEYDOWN && message <= WM_SYSDEADCHAR);
		if (!m_bTracking &&
			(bKeys ||
			(message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK) ||
			(message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK) ||
			(message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK) ||
			(message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK) ||
			(message == WM_NCRBUTTONDOWN || message == WM_NCRBUTTONDBLCLK) ||
			(message == WM_NCMBUTTONDOWN || message == WM_NCMBUTTONDBLCLK)))
		{
			//CancelToolTips(bKeys);
			CWndSuper* pTopWnd = GetTopLevelParent();
			if (!pTopWnd) pTopWnd = this;
			if (pTopWnd && pTopWnd->m_pToolTip)
				pTopWnd->m_pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
		}
	}
}
