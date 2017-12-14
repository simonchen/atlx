#include "atlx.h"
#include "atlx_button_ctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CButton

BOOL ATLX::CButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd, UINT nID/*=0xFFFF*/)
{
	CWndSuper* pWnd = this;
	return pWnd->Create(_T("BUTTON"), lpszCaption, dwStyle, rect, pParentWnd, nID);
}

ATLX::CButton::~CButton()
{

}

void ATLX::CButton::PreSubclassWindow()
{
	CWndSuper::PreSubclassWindow();

	// If you want owner-draw button, please enable BS_OWNERDRAW style
	// ModifyStyle(0, BS_OWNERDRAW);
}

void ATLX::CButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// Do nothing
}

BOOL ATLX::CButton::OnEraseBkgnd(HDC hdc)
{
	// Do nothing
	return FALSE;
}
