#pragma once
#include <time.h>
#include "WndSuper.h"
#include "atlx_string.h"

namespace ATLX{
	int WINAPI AtlxMessageBox(ATLX::CString prompt, UINT type, TCHAR* lpszCaption=NULL);

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

		HWND m_hDlg;
		BOOL m_bSaveAndValidate;
		BOOL m_bFail;

		void Fail() { m_bFail = TRUE; }
	};

	// simple text operations
#define DDX_Text(pDX, nIDC, value) pDX->UpdateText(nIDC, value)
	
	// utility time function converting system time to time_t
	time_t TimeFromSystemTime(const SYSTEMTIME * pTime);
	// utility time function converting  time_t to system time
	void TimeToSystemTime(const time_t t, SYSTEMTIME& time);

	// special control types
	void WINAPI DDX_Check(ATLX::CDataExchanger* pDX, int nIDC, int& value);
	void WINAPI DDX_Radio(ATLX::CDataExchanger* pDX, int nIDC, int& value);
	void WINAPI DDX_LBString(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value);
	void WINAPI DDX_CBString(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value);
	void WINAPI DDX_LBIndex(ATLX::CDataExchanger* pDX, int nIDC, int& index);
	void WINAPI DDX_CBIndex(ATLX::CDataExchanger* pDX, int nIDC, int& index);
	void WINAPI DDX_LBStringExact(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value);
	void WINAPI DDX_CBStringExact(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value);
	void WINAPI DDX_Scroll(ATLX::CDataExchanger* pDX, int nIDC, int& value);
	void WINAPI DDX_Slider(ATLX::CDataExchanger* pDX, int nIDC, int& value);

	void WINAPI DDX_IPAddress(ATLX::CDataExchanger* pDX, int nIDC, DWORD& value);

	void WINAPI DDX_MonthCalCtrl(ATLX::CDataExchanger* pDX, int nIDC, time_t& value);
	void WINAPI DDX_MonthCalCtrl(ATLX::CDataExchanger* pDX, int nIDC, FILETIME& value);
	void WINAPI DDX_DateTimeCtrl(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString& value);
	void WINAPI DDX_DateTimeCtrl(ATLX::CDataExchanger* pDX, int nIDC, SYSTEMTIME& value);

	// for getting access to the actual controls
	void WINAPI DDX_Control(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CWndSuper& rControl);

	/////////////////////////////////////////////////////////////////////////////
	// Standard Dialog Data Validation routines

	// range - value must be >= minVal and <= maxVal
	// NOTE: you will require casts for 'minVal' and 'maxVal' to use the
	//   UINT, DWORD or float types
	void WINAPI DDV_MinMaxByte(ATLX::CDataExchanger* pDX, BYTE value, BYTE minVal, BYTE maxVal);
	void WINAPI DDV_MinMaxShort(ATLX::CDataExchanger* pDX, short value, short minVal, short maxVal);
	void WINAPI DDV_MinMaxInt(ATLX::CDataExchanger* pDX, int value, int minVal, int maxVal);
	void WINAPI DDV_MinMaxLong(ATLX::CDataExchanger* pDX, long value, long minVal, long maxVal);
	void WINAPI DDV_MinMaxUInt(ATLX::CDataExchanger* pDX, UINT value, UINT minVal, UINT maxVal);
	void WINAPI DDV_MinMaxDWord(ATLX::CDataExchanger* pDX, DWORD value, DWORD minVal, DWORD maxVal);
	void WINAPI DDV_MinMaxLongLong(ATLX::CDataExchanger* pDX, LONGLONG value, LONGLONG minVal, LONGLONG maxVal);
	void WINAPI DDV_MinMaxULongLong(ATLX::CDataExchanger* pDX, ULONGLONG value, ULONGLONG minVal, ULONGLONG maxVal);
	void WINAPI DDV_MinMaxFloat(ATLX::CDataExchanger* pDX, float const& value, float minVal, float maxVal);
	void WINAPI DDV_MinMaxDouble(ATLX::CDataExchanger* pDX, double const& value, double minVal, double maxVal);

	// special control types
	void WINAPI DDV_MinMaxSlider(ATLX::CDataExchanger* pDX, int nIDC, DWORD value, DWORD minVal, DWORD maxVal);
	void WINAPI DDV_MinMaxDateTime(ATLX::CDataExchanger* pDX, int nIDC, time_t& refValue, const time_t refMinRange, const time_t refMaxRange);

	// number of characters
	void WINAPI DDV_MaxChars(ATLX::CDataExchanger* pDX, int nIDC, ATLX::CString const& value, int nChars);

}
