#pragma once
#include "atlx_string.h"

namespace ATLX{

	class CDataExchanger
	{
	public:
		CDataExchanger(HWND hDlg, BOOL bSaveAndValidate = FALSE);
		virtual ~CDataExchanger();

		inline void UpdateText(int nIDC, BYTE& value);
		inline void UpdateText(int nIDC, short& value);
		inline void UpdateText(int nIDC, unsigned short& value);
		inline void UpdateText(int nIDC, int& value);
		inline void UpdateText(int nIDC, unsigned int& value);
		inline void UpdateText(int nIDC, long& value);
		inline void UpdateText(int nIDC, DWORD& value);
		inline void UpdateText(int nIDC, LONGLONG& value);
		inline void UpdateText(int nIDC, ULONGLONG& value);
		inline void UpdateText(int nIDC, ATLX::CString& value);
		inline void UpdateText(int nIDC, TCHAR* value, size_t size);
		inline void UpdateText(int nIDC, float& value);
		inline void UpdateText(int nIDC, double& value);

	private:
		HWND m_hDlg;
		BOOL m_bSaveAndValidate;
	};

	// simple text operations
#define DDX_Text(pDX, nIDC, value) pDX->UpdateText(nIDC, value);
	/*

	// special control types
	void AFXAPI DDX_Check(CDataExchange* pDX, int nIDC, int& value);
	void AFXAPI DDX_Radio(CDataExchange* pDX, int nIDC, int& value);
	void AFXAPI DDX_LBString(CDataExchange* pDX, int nIDC, CString& value);
	void AFXAPI DDX_CBString(CDataExchange* pDX, int nIDC, CString& value);
	void AFXAPI DDX_LBIndex(CDataExchange* pDX, int nIDC, int& index);
	void AFXAPI DDX_CBIndex(CDataExchange* pDX, int nIDC, int& index);
	void AFXAPI DDX_LBStringExact(CDataExchange* pDX, int nIDC, CString& value);
	void AFXAPI DDX_CBStringExact(CDataExchange* pDX, int nIDC, CString& value);
	void AFXAPI DDX_Scroll(CDataExchange* pDX, int nIDC, int& value);
	void AFXAPI DDX_Slider(CDataExchange* pDX, int nIDC, int& value);

	void AFXAPI DDX_IPAddress(CDataExchange* pDX, int nIDC, DWORD& value);

	void AFXAPI DDX_MonthCalCtrl(CDataExchange* pDX, int nIDC, CTime& value);
	void AFXAPI DDX_MonthCalCtrl(CDataExchange* pDX, int nIDC, COleDateTime& value);
	void AFXAPI DDX_MonthCalCtrl(CDataExchange* pDX, int nIDC, FILETIME& value);
	void AFXAPI DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CString& value);
	void AFXAPI DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CTime& value);
	void AFXAPI DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, COleDateTime& value);
	void AFXAPI DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, FILETIME& value);

	// for getting access to the actual controls
	void AFXAPI DDX_Control(CDataExchange* pDX, int nIDC, CWnd& rControl);

	/////////////////////////////////////////////////////////////////////////////
	// Standard Dialog Data Validation routines

	// range - value must be >= minVal and <= maxVal
	// NOTE: you will require casts for 'minVal' and 'maxVal' to use the
	//   UINT, DWORD or float types
	void AFXAPI DDV_MinMaxByte(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
	void AFXAPI DDV_MinMaxShort(CDataExchange* pDX, short value, short minVal, short maxVal);
	void AFXAPI DDV_MinMaxInt(CDataExchange* pDX, int value, int minVal, int maxVal);
	void AFXAPI DDV_MinMaxLong(CDataExchange* pDX, long value, long minVal, long maxVal);
	void AFXAPI DDV_MinMaxUInt(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
	void AFXAPI DDV_MinMaxDWord(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
	void AFXAPI DDV_MinMaxLongLong(CDataExchange* pDX, LONGLONG value, LONGLONG minVal, LONGLONG maxVal);
	void AFXAPI DDV_MinMaxULongLong(CDataExchange* pDX, ULONGLONG value, ULONGLONG minVal, ULONGLONG maxVal);
	void AFXAPI DDV_MinMaxFloat(CDataExchange* pDX, float const& value, float minVal, float maxVal);
	void AFXAPI DDV_MinMaxDouble(CDataExchange* pDX, double const& value, double minVal, double maxVal);

	// special control types
	void AFXAPI DDV_MinMaxSlider(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
	void AFXAPI DDV_MinMaxDateTime(CDataExchange* pDX, CTime& refValue, const CTime* refMinRange, const CTime* refMaxRange);
	void AFXAPI DDV_MinMaxDateTime(CDataExchange* pDX, COleDateTime& refValue, const COleDateTime* refMinRange, const COleDateTime* refMaxRange);
	void AFXAPI DDV_MinMaxMonth(CDataExchange* pDX, CTime& refValue, const CTime* pMinRange, const CTime* pMaxRange);
	void AFXAPI DDV_MinMaxMonth(CDataExchange* pDX, COleDateTime& refValue, const COleDateTime* refMinRange, const COleDateTime* refMaxRange);

	*/
	// number of characters
	void DDV_MaxChars(CDataExchanger* pDX, ATLX::CString const& value, int nChars);

}

