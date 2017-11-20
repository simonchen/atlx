#include "stdafx.h"
#include "Dialogx.h"


ATLX::CDialogx::CDialogx(int id/*=0*/)
{
}

ATLX::CDialogx::~CDialogx()
{
}

BOOL ATLX::CDialogx::DoModal(HWND hParent, BOOL bModeless/*=FALSE*/)
{
	if (hParent && !::IsWindow(hParent))
		return FALSE;

	m_hParent = hParent;
	m_thunk->Init((DWORD_PTR)StartWindowProc, this);
	DLGPROC proc = (DLGPROC)m_thunk->GetCodeAddress();
	if (bModeless)
	{
		::DialogBox(g_hInst, MAKEINTRESOURCE(IDD), hParent, proc);//COptionsDlg::DialogProc);
	}
	else
	{
		m_hWnd = ::CreateDialog(NULL, MAKEINTRESOURCE(IDD), m_hParent, proc);
		::ShowWindow(m_hWnd, SW_SHOW);
	}

	return TRUE;
}

LRESULT ATLX::CDialogx::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled)
{
	LRESULT ret = 0;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		//int i = 0;
	}
	break;
	case WM_INITDIALOG:
	{
		bMsgHandled = OnInitDialog();
	}
	break;
	case WM_COMMAND:
	{
		WORD ctlID = LOWORD(wParam);
		switch (ctlID)
		{
		case IDOK:
			ret = OnOK();
			break;
		case IDCANCEL:
			ret = OnCancel();
			break;
		default:
			break;
		}

	}
	break;
	case WM_KEYDOWN:
	{
		
	}
	break;
	case WM_CLOSE:
	{
		ret = OnClose();
	}
	break;
	}

	::DefWindowProc(m_hWnd, uMsg, wParam, lParam);

	return ret;
}

void ATLX::CDialogx::Center()
{
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	RECT rcDlg;
	GetWindowRect(m_hWnd, &rcDlg);

	int dx = 0, dy = 0;
	if (m_hParent && ::IsWindowVisible(m_hParent))
	{
		RECT rcParent;
		GetWindowRect(m_hParent, &rcParent);
		dx = rcParent.left + (rcParent.right - rcParent.left) / 2 - (rcDlg.right - rcDlg.left) / 2;
		dy = rcParent.top + (rcParent.bottom - rcParent.top) / 2 - (rcDlg.bottom - rcDlg.top) / 2;
	}
	else
	{
		dx = (w - rcDlg.right + rcDlg.left) / 2;
		dy = (h - rcDlg.bottom + rcDlg.top) / 2;
	}

	SetWindowPos(m_hWnd, NULL, dx, dy, 0, 0, SWP_NOSIZE);
}

BOOL ATLX::CDialogx::UpdateData(BOOL bSaveAndValidate/*=FALSE*/)
{
	CDataExchanger de(m_hWnd, bSaveAndValidate);
	DoDataExchange(&de);

	return !de.m_bFail;
}

void ATLX::CDialogx::DoDataExchange(ATLX::CDataExchanger* pdx)
{
	// Do nothing.
}

BOOL ATLX::CDialogx::OnInitDialog()
{
	SetClassLong(m_hWnd, GCL_HICON, 0);

	Center();

	UpdateData();

	return TRUE;
}

BOOL ATLX::CDialogx::OnOK()
{
	if (!UpdateData(TRUE))
		return FALSE;

	return ::EndDialog(m_hWnd, 0);
}

BOOL ATLX::CDialogx::OnCancel()
{
	return ::EndDialog(m_hWnd, 0);
}

BOOL ATLX::CDialogx::OnClose()
{
	return ::EndDialog(m_hWnd, 0);
}