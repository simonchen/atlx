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
