#pragma once
#include "WndThunker.h"
#include "DataExchanger.h"

namespace ATLX {

	// Base class - CDialogx
	class CDialogx :
		public ATLX::CWndThunker
	{
	public:
		CDialogx(int id=0);
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
		HWND m_hParent;
	};
}
