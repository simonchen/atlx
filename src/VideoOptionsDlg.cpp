#include "atlx.h"
#include "VideoOptionsDlg.h"


CVideoOptionsDlg::CVideoOptionsDlg(HINSTANCE hInst/*=NULL*/) :
ATLX::CDialogx(CVideoOptionsDlg::IDD, hInst)
{
}


CVideoOptionsDlg::~CVideoOptionsDlg()
{
}

BOOL CVideoOptionsDlg::OnInitDialog()
{
	BOOL ret = CDialogx::OnInitDialog();

	/*
	RECT rc;
	rc.left = 50;
	rc.top = 50;
	rc.right = 250;
	rc.bottom = 250;
	m_picCodec2.Create(WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, rc, this);
	m_picCodec2.LoadFromResource(m_hInst, MAKEINTRESOURCE(IDB_CODEC));
	*/

	return ret;
}
void CVideoOptionsDlg::DoDataExchange(ATLX::CDataExchanger* pdx)
{
	CDialogx::DoDataExchange(pdx);

	ATLX::DDX_Control(pdx, IDC_AUDIO_BTN, m_btnAudio);
	m_btnAudio.EnableToolTips(TRUE);

	ATLX::DDX_Control(pdx, IDC_V_CODEC_BTN, m_picCodec);
	m_picCodec.LoadFromResource(m_hInst, MAKEINTRESOURCE(IDB_CODEC));
}
