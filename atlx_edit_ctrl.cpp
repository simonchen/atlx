#include "atlx.h"
#include "atlx_edit_ctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CEdit

BOOL ATLX::CEdit::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd, UINT nID/*=0xFFFF*/)
{
	CWndSuper* pWnd = this;
	return pWnd->Create(_T("EDIT"), lpszCaption, dwStyle, rect, pParentWnd, nID);
}

ATLX::CEdit::~CEdit()
{

}
