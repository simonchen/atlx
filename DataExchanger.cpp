#include "stdafx.h"
#include <stdlib.h>
#include <float.h>
#include "DataExchanger.h"
#include "atlx_string.h"

int WINAPI ATLX::AtlxMessageBox(ATLX::CString prompt, UINT type, TCHAR* lpszCaption/*=NULL*/)
{
	return MessageBox(NULL, prompt, lpszCaption, type);
}

time_t ATLX::TimeFromSystemTime(const SYSTEMTIME* pTime)
{
	struct tm tm;
	memset(&tm, 0, sizeof(tm));

	tm.tm_year = pTime->wYear - 1900;
	tm.tm_mon = pTime->wMonth - 1;
	tm.tm_mday = pTime->wDay;

	tm.tm_hour = pTime->wHour;
	tm.tm_min = pTime->wMinute;
	tm.tm_sec = pTime->wSecond;

	return mktime(&tm);
}

void ATLX::TimeToSystemTime(const time_t t, SYSTEMTIME& time)
{
	struct tm tm;
	memset(&tm, 0, sizeof(tm));
	gmtime_s(&tm, &t);

	time.wYear = tm.tm_year + 1900;
	time.wMonth = tm.tm_mon + 1;
	time.wDay = tm.tm_mday;

	time.wHour = tm.tm_hour;
	time.wMinute = tm.tm_min;
	time.wSecond = tm.tm_sec;
}

ATLX::CDataExchanger::CDataExchanger(HWND hDlg, BOOL bSaveAndValidate/*=FALSE*/)
{
	m_hDlg = hDlg;
	m_bSaveAndValidate = bSaveAndValidate;
	m_bFail = FALSE;
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
void WINAPI ATLX::DDX_Check(ATLX::CDataExchanger* pDX, int nIDC, int& value)
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

void WINAPI ATLX::DDX_Radio(ATLX::CDataExchanger* pDX, int nIDC, int& value)
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

void WINAPI ATLX::DDX_LBString(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
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

void WINAPI ATLX::DDX_LBStringExact(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
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

void WINAPI ATLX::DDX_CBString(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
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

void WINAPI ATLX::DDX_CBStringExact(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
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

void WINAPI ATLX::DDX_LBIndex(ATLX::CDataExchanger* pDX, int nIDC, int& index)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		index = (int)::SendMessage(hCtrl, LB_GETCURSEL, 0, 0L);
	else
		::SendMessage(hCtrl, LB_SETCURSEL, (WPARAM)index, 0L);
}

void WINAPI ATLX::DDX_CBIndex(ATLX::CDataExchanger* pDX, int nIDC, int& index)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		index = (int)::SendMessage(hCtrl, CB_GETCURSEL, 0, 0L);
	else

		::SendMessage(hCtrl, CB_SETCURSEL, (WPARAM)index, 0L);
}

void WINAPI ATLX::DDX_Scroll(ATLX::CDataExchanger* pDX, int nIDC, int& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		value = GetScrollPos(hCtrl, SB_CTL);
	else
		SetScrollPos(hCtrl, SB_CTL, value, TRUE);
}

void WINAPI ATLX::DDX_Slider(ATLX::CDataExchanger* pDX, int nIDC, int& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		value = (int) ::SendMessage(hCtrl, TBM_GETPOS, 0, 0l);
	else
		::SendMessage(hCtrl, TBM_SETPOS, TRUE, value);
}


void WINAPI ATLX::DDX_IPAddress(ATLX::CDataExchanger* pDX, int nIDC, DWORD& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
		::SendMessage(hCtrl, IPM_GETADDRESS, 0, (LPARAM)&value);
	else
		::SendMessage(hCtrl, IPM_SETADDRESS, 0, (LPARAM)value);
}

void WINAPI ATLX::DDX_MonthCalCtrl(ATLX::CDataExchanger* pDX, int nIDC, time_t& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
	{
		SYSTEMTIME st;
		MonthCal_GetToday(hCtrl, &st);
		value = ATLX::TimeFromSystemTime(&st);
	}
	else
	{
		SYSTEMTIME st;
		TimeToSystemTime(value, st);
		MonthCal_SetToday(hCtrl, &st);
	}
}

void WINAPI ATLX::DDX_MonthCalCtrl(ATLX::CDataExchanger* pDX, int nIDC, FILETIME& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
	{
		SYSTEMTIME st;
		MonthCal_GetToday(hCtrl, &st);
		SystemTimeToFileTime(&st, &value);
	}
	else
	{
		SYSTEMTIME st;
		FileTimeToSystemTime(&value, &st);
		MonthCal_SetToday(hCtrl, &st);
	}
}

void WINAPI ATLX::DDX_DateTimeCtrl(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
	{
		SYSTEMTIME st;
		MonthCal_GetToday(hCtrl, &st);
		value.format(_T("%hu-%hu-%hu %hu:%hu:%hu"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else
	{
		SYSTEMTIME st;
		ZeroMemory(&st, sizeof(SYSTEMTIME));
		value.sscanf(_T("%hu-%hu-%hu %hu:%hu:%hu"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		MonthCal_SetToday(hCtrl, &st);
	}
}

void WINAPI ATLX::DDX_DateTimeCtrl(ATLX::CDataExchanger* pDX, int nIDC, SYSTEMTIME& value)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate)
	{
		MonthCal_GetToday(hCtrl, &value);
	}
	else
	{
		MonthCal_SetToday(hCtrl, &value);
	}
}

// for getting access to the actual controls
void WINAPI ATLX::DDX_Control(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CWndSuper& rControl)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);
	if (!hCtrl) return;

	rControl.SubclassWindow(hCtrl);
}

/////////////////////////////////////////////////////////////////////////////
// Standard Dialog Data Validation routines
#define ATLX_PARSE_INT_RANGE _T("Enter an integer between %s and %s.")
#define ATLX_PARSE_REAL_RANGE _T("Enter a number between %s and %s.")
#define ATLX_PARSE_STRING_SIZE _T("Enter no more than %s characters.")

void WINAPI _ATLX_FailMinMaxWithFormat(ATLX::CDataExchanger* pDX,
	LONGLONG minVal, LONGLONG maxVal, LPCTSTR lpszFormat, TCHAR* lpszPromptFormat)
	// error string must have '%1' and '%2' strings for min and max values
	// since minVal and maxVal are 64-bit, lpszFormat should be "%I64d" or "%I64u"
{
	if (!pDX->m_bSaveAndValidate)
	{
		printf_s("Warning: initial dialog data is out of range.\n");
		return;     // don't stop now
	}

	const int MINMAX_BUFFER_SIZE = 64;
	TCHAR szMin[MINMAX_BUFFER_SIZE];
	TCHAR szMax[MINMAX_BUFFER_SIZE];

	_sntprintf_s(szMin, _countof(szMin), _countof(szMin)-1, lpszFormat, minVal);
	_sntprintf_s(szMax, _countof(szMax), _countof(szMax)-1, lpszFormat, maxVal);

	ATLX::CString prompt;
	prompt.format(lpszPromptFormat, minVal, maxVal);
	ATLX::AtlxMessageBox(prompt, MB_ICONEXCLAMATION);

	pDX->Fail();
}

void WINAPI _ATLX_FailMinMaxReal(ATLX::CDataExchanger* pDX,
	double minVal, double maxVal, int precision, TCHAR* lpszPromptFormat)
	// error string must have '%1' and '%2' strings for min and max values
	// since minVal and maxVal are 64-bit, lpszFormat should be "%I64d" or "%I64u"
{
	if (!pDX->m_bSaveAndValidate)
	{
		printf_s("Warning: initial dialog data is out of range.\n");
		return;     // don't stop now
	}

	const int MINMAX_BUFFER_SIZE = 32;
	TCHAR szMin[MINMAX_BUFFER_SIZE];
	TCHAR szMax[MINMAX_BUFFER_SIZE];

	_sntprintf_s(szMin, _countof(szMin), _countof(szMin) - 1, _T("%.*g"), precision, minVal);
	_sntprintf_s(szMax, _countof(szMax), _countof(szMax) - 1, _T("%.*g"), precision, maxVal);

	ATLX::CString prompt;
	prompt.format(lpszPromptFormat, minVal, maxVal);
	ATLX::AtlxMessageBox(prompt, MB_ICONEXCLAMATION);

	pDX->Fail();
}


// range - value must be >= minVal and <= maxVal
// NOTE: you will require casts for 'minVal' and 'maxVal' to use the
//   UINT, DWORD or float types
void WINAPI DDV_MinMaxByte(ATLX::CDataExchanger* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	if (value < minVal || value > maxVal)
		_ATLX_FailMinMaxWithFormat(pDX, minVal, maxVal, _T("%I64u"),
		ATLX_PARSE_INT_RANGE);
}

void WINAPI DDV_MinMaxShort(ATLX::CDataExchanger* pDX, short value, short minVal, short maxVal)
{
	if (value < minVal || value > maxVal)
		_ATLX_FailMinMaxWithFormat(pDX, minVal, maxVal, _T("%I64d"),
		ATLX_PARSE_INT_RANGE);
}

void WINAPI DDV_MinMaxInt(ATLX::CDataExchanger* pDX, int value, int minVal, int maxVal)
{
	if (value < minVal || value > maxVal)
		_ATLX_FailMinMaxWithFormat(pDX, minVal, maxVal, _T("%I64d"),
		ATLX_PARSE_INT_RANGE);
}

void WINAPI DDV_MinMaxLong(ATLX::CDataExchanger* pDX, long value, long minVal, long maxVal)
{
	if (value < minVal || value > maxVal)
		_ATLX_FailMinMaxWithFormat(pDX, minVal, maxVal, _T("%I64d"),
		ATLX_PARSE_INT_RANGE);
}

void WINAPI DDV_MinMaxUInt(ATLX::CDataExchanger* pDX, UINT value, UINT minVal, UINT maxVal)
{
	if (value < minVal || value > maxVal)
		_ATLX_FailMinMaxWithFormat(pDX, minVal, maxVal, _T("%I64u"),
		ATLX_PARSE_INT_RANGE);
}

void WINAPI DDV_MinMaxDWord(ATLX::CDataExchanger* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	if (value < minVal || value > maxVal)
		_ATLX_FailMinMaxWithFormat(pDX, minVal, maxVal, _T("%I64u"),
		ATLX_PARSE_INT_RANGE);
}

void WINAPI DDV_MinMaxLongLong(ATLX::CDataExchanger* pDX, LONGLONG value, LONGLONG minVal, LONGLONG maxVal)
{
	if ((value < minVal) || (value > maxVal))
		_ATLX_FailMinMaxWithFormat(pDX, minVal, maxVal, _T("%I64d"),
		ATLX_PARSE_INT_RANGE);
}

void WINAPI DDV_MinMaxULongLong(ATLX::CDataExchanger* pDX, ULONGLONG value, ULONGLONG minVal, ULONGLONG maxVal)
{
	if ((value < minVal) || (value > maxVal))
		_ATLX_FailMinMaxWithFormat(pDX, minVal, maxVal, _T("%I64u"),
		ATLX_PARSE_INT_RANGE);
}

void WINAPI DDV_MinMaxFloat(ATLX::CDataExchanger* pDX, float const& value, float minVal, float maxVal)
{
	if ((value < minVal) || (value > maxVal))
		_ATLX_FailMinMaxReal(pDX, (double)minVal, (double)maxVal, FLT_DIG,
		ATLX_PARSE_REAL_RANGE);
}

void WINAPI DDV_MinMaxDouble(ATLX::CDataExchanger* pDX, double const& value, double minVal, double maxVal)
{
	if ((value < minVal) || (value > maxVal))
		_ATLX_FailMinMaxReal(pDX, minVal, maxVal, FLT_DIG,
		ATLX_PARSE_REAL_RANGE);
}

// special control types
void WINAPI DDV_MinMaxSlider(ATLX::CDataExchanger* pDX, int nIDC, DWORD value, DWORD minVal, DWORD maxVal)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (!pDX->m_bSaveAndValidate)
	{
		if (minVal > value || maxVal < value)
		{
			printf_s("Warning: initial dialog data is out of "
				"range in control ID %d.\n", nIDC);
			return;     // don't stop now
		}
	}

	::SendMessage(hCtrl, TBM_SETRANGEMIN, FALSE, (LPARAM)minVal);
	::SendMessage(hCtrl, TBM_SETRANGEMAX, TRUE, (LPARAM)maxVal);
}

void WINAPI DDV_MinMaxDateTime(ATLX::CDataExchanger* pDX, int nIDC, time_t& refValue, const time_t refMinRange, const time_t refMaxRange)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (!pDX->m_bSaveAndValidate)
	{
		if ((refMinRange != NULL && refMinRange > refValue) ||
			(refMaxRange != NULL && refMaxRange < refValue))
		{
			printf_s("Warning: initial dialog data is out of range in "
				"control ID %d.\n", nIDC);

			return;     // don't stop now
		}
	}

	SYSTEMTIME gst[2];
	ZeroMemory(gst, sizeof(SYSTEMTIME) * 2);
	ATLX::TimeToSystemTime(refMinRange, gst[0]);
	ATLX::TimeToSystemTime(refMaxRange, gst[1]);
	MonthCal_SetRange(hCtrl, GDTR_MIN | GDTR_MAX, gst);
}

void WINAPI DDV_MinMaxDateTime(ATLX::CDataExchanger* pDX, int nIDC, SYSTEMTIME& stValue, const SYSTEMTIME stMinRange, const SYSTEMTIME stMaxRange)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);
	
	time_t refValue = ATLX::TimeFromSystemTime(&stValue);
	time_t refMinRange = ATLX::TimeFromSystemTime(&stMinRange);
	time_t refMaxRange = ATLX::TimeFromSystemTime(&stMaxRange);

	if (!pDX->m_bSaveAndValidate)
	{
		if ((refMinRange != NULL && refMinRange > refValue) ||
			(refMaxRange != NULL && refMaxRange < refValue))
		{
			printf_s("Warning: initial dialog data is out of range in "
				"control ID %d.\n", nIDC);

			return;     // don't stop now
		}
	}

	MonthCal_SetRange(hCtrl, GDTR_MIN, &stMinRange);
	MonthCal_SetRange(hCtrl, GDTR_MAX, &stMaxRange);
}


// number of characters
void WINAPI DDV_MaxChars(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString const& value, int nChars)
{
	HWND hCtrl = GetDlgItem(pDX->m_hDlg, nIDC);

	if (pDX->m_bSaveAndValidate && value.length() > nChars)
	{
		TCHAR szT[32];
		_stprintf_s(szT, _countof(szT), _T("%d"), nChars);
		ATLX::CString prompt;
		prompt.format(ATLX_PARSE_STRING_SIZE, szT);
		AtlxMessageBox(prompt, MB_ICONEXCLAMATION);
		pDX->Fail();
	}
	else if (hCtrl != NULL)
	{
		// limit the control max-chars automatically
		// send messages for both an edit control and a combobox control--one will
		// be understood and one will be disregarded, but this is the only way to
		// ensure that the characters will be limited for both kinds of controls.
		::SendMessage(hCtrl, EM_SETLIMITTEXT, nChars, 0);
		::SendMessage(hCtrl, CB_LIMITTEXT, nChars, 0);
	}
}
