#include "atlx.h"
#include "atlx_slider_ctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrl

BOOL ATLX::CSliderCtrl::Create(DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd, UINT nID/*=0xFFFF*/)
{
	// initialize common controls
	INITCOMMONCONTROLSEX init;
	init.dwICC = ICC_BAR_CLASSES;
	init.dwSize = sizeof(init);
	::InitCommonControlsEx(&init);

	CWndSuper* pWnd = this;
	return pWnd->Create(TRACKBAR_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL ATLX::CSliderCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect,
	CWndSuper* pParentWnd, UINT nID/*=0xFFFF*/)
{
	return Create(dwStyle, rect, pParentWnd, nID) &&
		ModifyStyleEx(0, dwExStyle);
}

ATLX::CSliderCtrl::~CSliderCtrl()
{

}