#pragma once

#include "WndSuper.h"
#include "DataExchanger.h"

namespace ATLX {

	// Base class - CDialogx
	class CDialogx :
		public ATLX::CWndThunker
	{
	public:
		// If you use Dialog in Dynamic-Link library, you should set hInst.
		CDialogx(UINT IDD=0, HINSTANCE hInst=NULL);
		virtual ~CDialogx();

		enum { IDD = 0 }; // Overrides to assigning resource ID.

		virtual LRESULT ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled);

		BOOL DoModal(HWND hParent, BOOL bModeless=FALSE);

		BOOL UpdateData(BOOL bSaveAndValidate = TRUE);

	protected:
		virtual BOOL OnInitDialog();
		virtual BOOL OnOK();
		virtual BOOL OnCancel();
		virtual BOOL OnClose();

		void Center();

		// You should override this function if you want to save / validate data.
		virtual void DoDataExchange(ATLX::CDataExchanger* pdx);

	protected:
		UINT m_IDD;
		BOOL m_bModeless;
		HWND m_hParent;
		HINSTANCE m_hInst;
	};
}
