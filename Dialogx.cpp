#include "atlx.h"

ATLX::CDialogx::CDialogx(UINT IDD, HINSTANCE hInst)
{
	m_fDialog = TRUE;
	m_hInst = hInst;
	m_IDD = IDD;
	m_bModeless = FALSE;
}

ATLX::CDialogx::~CDialogx()
{
}

BOOL ATLX::CDialogx::DoModal(HWND hParent, BOOL bModeless/*=FALSE*/)
{
	if (hParent && !::IsWindow(hParent))
		return FALSE;

	m_bModeless = bModeless;

	m_hParent = hParent;
	m_thunk->Init((DWORD_PTR)StartWindowProc, this);
	DLGPROC proc = (DLGPROC)m_thunk->GetCodeAddress();
	if (!bModeless)
	{
		::DialogBox(m_hInst, MAKEINTRESOURCE(m_IDD), hParent, proc);//COptionsDlg::DialogProc);
	}
	else
	{
		m_hWnd = ::CreateDialog(m_hInst, MAKEINTRESOURCE(m_IDD), m_hParent, proc);
		if (::IsWindow(m_hWnd))
		{
			::ShowWindow(m_hWnd, SW_SHOW);
			BOOL bRet;
			MSG msg;
			while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0 &&
				IsWindow(m_hWnd))
			{
				if (bRet == -1)
				{
					// Handle the error and possibly exit
					break;
				}
				else if (!IsWindow(m_hWnd) || !IsDialogMessage(m_hWnd, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
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
	case WM_DRAWITEM:
	{
		// Owner-draw for Static, Button, ListCtrl, Listbox, Menu
		UINT nID = (UINT)wParam;
		LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
		OnDrawItem(pdis); // Handled by Dialog

		HWND hCtrl = ::GetDlgItem(m_hWnd, nID);
		if (hCtrl)
		// Reflecting same message to control to process further.
		{
			if (nID)
			{
				::SendMessage(hCtrl, WM_DRAWITEM, wParam, lParam);
			}
			else
			{
				// Draw menu item in here.
			}
		}
	}
	break;
	}

	//::DefWindowProc(m_hWnd, uMsg, wParam, lParam);

	return ret;
}

void ATLX::CDialogx::Center()
{
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	RECT rcDlg;
	::GetWindowRect(m_hWnd, &rcDlg);

	int dx = 0, dy = 0;
	if (m_hParent && ::IsWindowVisible(m_hParent))
	{
		RECT rcParent;
		::GetWindowRect(m_hParent, &rcParent);
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

	if (m_bModeless)
		return Destroy();

	return ::EndDialog(m_hWnd, 0);
}

BOOL ATLX::CDialogx::OnCancel()
{
	if (m_bModeless)
		return Destroy();

	return ::EndDialog(m_hWnd, 0);
}

BOOL ATLX::CDialogx::OnClose()
{
	if (m_bModeless)
		return Destroy();

	return ::EndDialog(m_hWnd, 0);
}

BOOL ATLX::CDialogx::OnDrawItem(LPDRAWITEMSTRUCT pdis)
{
	// Do nothing.

	return FALSE;
}