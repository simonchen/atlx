#include "stdafx.h"
#include "atlx_static_ctrl.h"
#include <Ole2.h>
#include <GdiPlus.h>
using namespace Gdiplus;

//////////////////////////////////////////////////////////////////////////
// CStatic
ATLX::CStatic::CStatic()
{

}

ATLX::CStatic::~CStatic()
{

}

BOOL ATLX::CStatic::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd/*=NULL*/, UINT nID/*=0xFFFF*/)
{
	return CWndSuper::Create(_T("STATIC"), lpszText, dwStyle, rect, pParentWnd, nID);
}

void ATLX::CStatic::PreSubclassWindow()
{
	CWndSuper::PreSubclassWindow();
	ModifyStyle(0, SS_CENTER);
}

//////////////////////////////////////////////////////////////////////////
// CPictureCtrl
#ifdef SAFE_RELEASE
#undef SAFE_RELEASE
#endif
#define SAFE_RELEASE(x) do{\
if((x) != NULL)\
{\
while((x)->Release() != 0);\
(x) = NULL;\
}\
}while(0)

ATLX::CPictureCtrl::CPictureCtrl(void)
	:CStatic()
	, m_pStream(NULL)
	, m_bIsPicLoaded(FALSE)
	, m_gdiplusToken(0)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

ATLX::CPictureCtrl::~CPictureCtrl(void)
{
	//Tidy up
	FreeData();
	GdiplusShutdown(m_gdiplusToken);
}

BOOL ATLX::CPictureCtrl::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd/*=NULL*/, UINT nID/*=0xFFFF*/)
{
	return CWndSuper::Create(_T("STATIC"), lpszText, dwStyle | SS_OWNERDRAW, rect, pParentWnd, nID);
}

BOOL ATLX::CPictureCtrl::LoadFromStream(IStream *piStream)
{
	//Set success error state
	SetLastError(ERROR_SUCCESS);

	FreeData();

	//Check for validity of argument
	if (piStream == NULL)
	{
		SetLastError(ERROR_INVALID_ADDRESS);
		return FALSE;
	}

	//Allocate stream
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}

	//Rewind the argument stream
	LARGE_INTEGER lInt;
	lInt.QuadPart = 0;
	piStream->Seek(lInt, STREAM_SEEK_SET, NULL);

	//Read the lenght of the argument stream
	STATSTG statSTG;
	dwResult = piStream->Stat(&statSTG, STATFLAG_DEFAULT);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}

	//Copy the argument stream to the class stream
	piStream->CopyTo(m_pStream, statSTG.cbSize, NULL, NULL);

	//Mark as loaded
	m_bIsPicLoaded = TRUE;

	InvalidateRect(m_hWnd, NULL, FALSE);
	RedrawWindow(m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_ERASENOW);

	return TRUE;
}

BOOL ATLX::CPictureCtrl::LoadFromStream(BYTE* pData, size_t nSize)
{
	//Set success error state
	SetLastError(ERROR_SUCCESS);
	FreeData();

	//Allocate stream
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}

	//Copy argument data to the stream
	dwResult = m_pStream->Write(pData, (ULONG)nSize, NULL);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}

	//Mark as loaded
	m_bIsPicLoaded = TRUE;

	InvalidateRect(m_hWnd, NULL, FALSE);
	RedrawWindow(m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_ERASENOW);

	return TRUE;
}

BOOL ATLX::CPictureCtrl::LoadFromFile(CString &szFilePath)
{
	//Set success error state
	SetLastError(ERROR_SUCCESS);
	FreeData();

	//Allocate stream
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}

	//Open the specified file
#ifdef UNICODE
	FILE* fp = _wfopen(szFilePath, _T("rb"));
#else
	FILE fp = fopen(szFilePath, "rb");
#endif
	if (!fp)
	{
		printf_s("Read file error: %s", szFilePath);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}

	//Copy the specified file's content to the stream
	BYTE pBuffer[1024] = { 0 };
	while (UINT dwRead = fread(pBuffer, 1, 1024, fp))
	{
		dwResult = m_pStream->Write(pBuffer, dwRead, NULL);
		if (dwResult != S_OK)
		{
			SetLastError(dwResult);
			SAFE_RELEASE(m_pStream);
			fclose(fp);
			return FALSE;
		}
	}

	//Close the file
	fclose(fp);

	//Mark as Loaded
	m_bIsPicLoaded = TRUE;

	InvalidateRect(m_hWnd, NULL, FALSE);
	RedrawWindow(m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_ERASENOW);

	return TRUE;
}

BOOL ATLX::CPictureCtrl::LoadFromResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType)
{
	//Set success error state
	SetLastError(ERROR_SUCCESS);
	FreeData();

	//Locate the resource
	HRSRC hResource = FindResource(hModule, lpName, lpType);
	if(hResource == NULL)
	{
		return FALSE;
	}

	//Get the size of the resource
	DWORD dwResourceSize = SizeofResource(hModule, hResource);
	if(dwResourceSize == 0)
	{
		return FALSE;
	}

	//Load the Resource
	HGLOBAL hGlobalResource = LoadResource(hModule, hResource);
	if(hGlobalResource == NULL)
	{
		return FALSE;
	}

	//Lock the resource and get the read pointer
	BYTE* pRecource = (BYTE*)LockResource(hGlobalResource);
	if(pRecource == NULL)
	{
		return FALSE;
	}

	//Allocate the Stream
	DWORD dwResult =  CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if(dwResult != S_OK)
	{
		FreeResource(hGlobalResource);
		SetLastError(dwResult);
		pRecource = NULL;
		return FALSE;
	}

	//Copy the resource data to the stream
	dwResult = m_pStream->Write(pRecource, dwResourceSize, NULL);
	if(dwResult != S_OK)
	{
		FreeResource(hGlobalResource);
		SAFE_RELEASE(m_pStream);
		SetLastError(dwResult);
		return FALSE;		
	}

	//Tidy up
	FreeResource(hGlobalResource);
	
	//Mark as loaded
	m_bIsPicLoaded = TRUE;

	InvalidateRect(m_hWnd, NULL, FALSE);
	RedrawWindow(m_hWnd, NULL, NULL, RDW_UPDATENOW | RDW_ERASENOW);

	return TRUE;
}

//Overload - Single load function
BOOL ATLX::CPictureCtrl::Load(CString &szFilePath)
{
	return LoadFromFile(szFilePath);
}

BOOL ATLX::CPictureCtrl::Load(IStream* piStream)
{
	return LoadFromStream(piStream);
}

BOOL ATLX::CPictureCtrl::Load(BYTE* pData, size_t nSize)
{
	return LoadFromStream(pData, nSize);
}

BOOL ATLX::CPictureCtrl::Load(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType)
{
	return LoadFromResource(hModule, lpName, lpType);
}

void ATLX::CPictureCtrl::FreeData()
{
	m_bIsPicLoaded = FALSE;
	SAFE_RELEASE(m_pStream);
}

void ATLX::CPictureCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_OWNERDRAW);
}

void ATLX::CPictureCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//Check if pic data is loaded
	if (m_bIsPicLoaded)
	{

		//Get control measures
		RECT rc;
		this->GetClientRect(&rc);

		Graphics graphics(lpDrawItemStruct->hDC);
		Image image(m_pStream);
		graphics.DrawImage(&image, (INT)rc.left, (INT)rc.top, (INT)(rc.right - rc.left), (INT)(rc.bottom - rc.top));

	}
}

BOOL ATLX::CPictureCtrl::OnEraseBkgnd(CDC *pDC)
{
	if (m_bIsPicLoaded)
	{

		//Get control measures
		RECT rc;
		this->GetClientRect(&rc);

		Graphics graphics(pDC->GetSafeHdc());
		LARGE_INTEGER liSeekPos;
		liSeekPos.QuadPart = 0;
		m_pStream->Seek(liSeekPos, STREAM_SEEK_SET, NULL);
		Image image(m_pStream);
		graphics.DrawImage(&image, (INT)rc.left, (INT)rc.top, (INT)(rc.right - rc.left), (INT)(rc.bottom - rc.top));
		return TRUE;
	}
	else
	{
		return CStatic::OnEraseBkgnd(pDC);
	}
}
