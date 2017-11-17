#include "stdafx.h"
#include "DataExchanger.h"
#include "atlx_string.h"

ATLX::CDataExchanger::CDataExchanger(HWND hDlg, BOOL bSaveAndValidate/*=FALSE*/)
{
	m_hDlg = hDlg;
	m_bSaveAndValidate = bSaveAndValidate;
}


ATLX::CDataExchanger::~CDataExchanger()
{
}

void ATLX::CDataExchanger::UpdateText(int nIDC, BYTE& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%hhu"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%hhu"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, short& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%hi"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%hi"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, unsigned short& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%hu"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%hu"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, int& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%i"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%i"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, unsigned int& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%u"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%u"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, long& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%li"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%li"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, DWORD& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%u"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%u"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, LONGLONG& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%lli"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%lli"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, ULONGLONG& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%I64u"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%I64u"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, ATLX::CString& value)
{
	if (m_bSaveAndValidate){
		SetDlgItemText(m_hDlg, nIDC, value);
	}
	else{
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = value.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, float& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%f"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%f"), &value);
	}
}

void ATLX::CDataExchanger::UpdateText(int nIDC, double& value)
{
	if (m_bSaveAndValidate){
		CString str;
		str.format(_T("%f"), value);
		SetDlgItemText(m_hDlg, nIDC, str);
	}
	else{
		CString str;
		int nLens = GetWindowTextLength(GetDlgItem(m_hDlg, nIDC)) + 1; // Window text length + 1 (terminating null)
		TCHAR* szBuff = str.get_buffer(nLens);
		GetDlgItemText(m_hDlg, nIDC, szBuff, nLens);
		str.sscanf(_T("%f"), &value);
	}
}

// special control types
void WINAPI DDX_Check(ATLX::CDataExchanger* pDX, int nIDC, int& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		value = (int)::SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
	}
	else
	{
		if (value < 0 || value > 2)
		{
			printf_s("Warning: dialog data checkbox value (%d) out of range.\n",
				value);
			value = 0;  // default to off
		}
		::SendMessage(hCtrl, BM_SETCHECK, (WPARAM)value, 0L);
	}

}

void WINAPI DDX_Radio(ATLX::CDataExchanger* pDX, int nIDC, int& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		value = -1;     // value if none found

	// walk all children in group
	int iButton = 0;
	do
	{
		if (::SendMessage(hCtrl, WM_GETDLGCODE, 0, 0L) & DLGC_RADIOBUTTON)
		{
			// control in group is a radio button
			if (pDX->m_bSaveAndValidate)
			{
				if (::SendMessage(hCtrl, BM_GETCHECK, 0, 0L) != 0)
				{
					value = iButton;
				}
			}
			else
			{
				// select button
				::SendMessage(hCtrl, BM_SETCHECK, (iButton == value), 0L);
			}
			iButton++;
		}
		else
		{
			printf("Warning: skipping non-radio button in group.\n");
		}
		hCtrl = ::GetWindow(hCtrl, GW_HWNDNEXT);

	} while (hCtrl != NULL &&
		!(GetWindowLong(hCtrl, GWL_STYLE) & WS_GROUP)); // Control must be existing and can't be fallen in next group
}

void WINAPI DDX_LBString(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		int nIndex = (int)::SendMessage(hCtrl, LB_GETCURSEL, 0, 0L);
		if (nIndex != -1)
		{
			int nLens = (int)::SendMessage(hCtrl, LB_GETTEXTLEN, nIndex, 0L) + 1;
			::SendMessage(hCtrl, LB_GETTEXT, nIndex,
				(LPARAM)(LPVOID)value.get_buffer(nLens));
		}
		else
		{
			// no selection
			value.empty();
		}
	}
	else
	{
		// set current selection based on data string
		if (::SendMessage(hCtrl, LB_SELECTSTRING, (WPARAM)-1,
			(LPARAM)(LPCTSTR)value) == LB_ERR)
		{
			// no selection match
			printf_s("Warning: no listbox item selected.\n");
		}
	}
}

void WINAPI DDX_LBStringExact(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		ATLX::DDX_LBString(pDX, nIDC, value);
	}
	else
	{
		// set current selection based on data string
		int i = (int)::SendMessage(hCtrl, LB_FINDSTRINGEXACT, (WPARAM)-1,
			(LPARAM)(LPCTSTR)value);
		if (i < 0)
		{
			// no selection match
			printf_s("Warning: no listbox item selected.\n");
		}
		else
		{
			// select it
			SendMessage(hCtrl, LB_SETCURSEL, i, 0L);
		}
	}
}

void WINAPI DDX_CBString(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if ((::GetWindowLong(hCtrl, GWL_STYLE) & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
	{
#if (WINVER >= 0x500)
		COMBOBOXINFO cbi;
		ZeroMemory(&cbi, sizeof(COMBOBOXINFO));
		cbi.cbSize = sizeof(COMBOBOXINFO);
		::GetComboBoxInfo(hCtrl, &cbi);
		hCtrl = cbi.hwndItem; // Edit box here it is.
#endif
	}

	if (pDX->m_bSaveAndValidate)
	{
		// just get current edit item text (or drop list static)
		int nLens = ::GetWindowTextLength(hCtrl) + 1;
		if (nLens > 0)
		{
			// get known length
			::GetWindowText(hCtrl, value.get_buffer(nLens), nLens);
		}
		else
		{
			// for drop lists GetWindowTextLength does not work - assume
			//  max of 255 characters with ternimating null.
			::GetWindowText(hCtrl, value.get_buffer(256), 256);
		}
	}
	else
	{
		// set current selection based on model string
		if (::SendMessage(hCtrl, CB_SELECTSTRING, (WPARAM)-1,
			(LPARAM)(LPCTSTR)value) == CB_ERR)
		{
			// just set the edit text (will be ignored if DROPDOWNLIST)
			SetWindowText(hCtrl, value);
		}
	}
}

void WINAPI DDX_CBStringExact(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if ((::GetWindowLong(hCtrl, GWL_STYLE) & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
	{
#if (WINVER >= 0x500)
		COMBOBOXINFO cbi;
		ZeroMemory(&cbi, sizeof(COMBOBOXINFO));
		cbi.cbSize = sizeof(COMBOBOXINFO);
		::GetComboBoxInfo(hCtrl, &cbi);
		hCtrl = cbi.hwndItem; // Edit box here it is.
#endif
	}

	if (pDX->m_bSaveAndValidate)
	{
		ATLX::DDX_CBString(pDX, nIDC, value);
	}
	else
	{
		// set current selection based on data string
		int i = (int)::SendMessage(hCtrl, CB_FINDSTRINGEXACT, (WPARAM)-1,
			(LPARAM)(LPCTSTR)value);
		if (i < 0)
		{
			// just set the edit text (will be ignored if DROPDOWNLIST)
			SetWindowText(hCtrl, value);
		}
		else
		{
			// select it
			SendMessage(hCtrl, CB_SETCURSEL, i, 0L);
		}
	}
}

void WINAPI DDX_LBIndex(ATLX::CDataExchanger* pDX, int nIDC, int& index)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		index = (int)::SendMessage(hCtrl, LB_GETCURSEL, 0, 0L);
	else
		::SendMessage(hCtrl, LB_SETCURSEL, (WPARAM)index, 0L);
}

void WINAPI DDX_CBIndex(ATLX::CDataExchanger* pDX, int nIDC, int& index)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		index = (int)::SendMessage(hCtrl, CB_GETCURSEL, 0, 0L);
	else

		::SendMessage(hCtrl, CB_SETCURSEL, (WPARAM)index, 0L);
}

void WINAPI DDX_Scroll(ATLX::CDataExchanger* pDX, int nIDC, int& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		value = GetScrollPos(hCtrl, SB_CTL);
	else
		SetScrollPos(hCtrl, SB_CTL, value, TRUE);
}

void WINAPI DDX_Slider(ATLX::CDataExchanger* pDX, int nIDC, int& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		value = (int) ::SendMessage(hCtrl, TBM_GETPOS, 0, 0l);
	else
		::SendMessage(hCtrl, TBM_SETPOS, TRUE, value);
}


void WINAPI DDX_IPAddress(ATLX::CDataExchanger* pDX, int nIDC, DWORD& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		::SendMessage(hCtrl, IPM_GETADDRESS, 0, (LPARAM)&value);
	else
		::SendMessage(hCtrl, IPM_SETADDRESS, 0, (LPARAM)value);
}

void WINAPI DDX_MonthCalCtrl(ATLX::CDataExchanger* pDX, int nIDC, time_t& value)
{

}

void WINAPI DDX_MonthCalCtrl(ATLX::CDataExchanger* pDX, int nIDC, FILETIME& value)
{

}

void WINAPI DDX_DateTimeCtrl(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
{

}

void WINAPI DDX_DateTimeCtrl(ATLX::CDataExchanger* pDX, int nIDC, SYSTEMTIME& value)
{

}

// for getting access to the actual controls
void WINAPI DDX_Control(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CWndSuper& rControl)
{

}