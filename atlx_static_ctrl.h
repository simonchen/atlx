#pragma once

#include "WndSuper.h"

namespace ATLX{

	class CStatic : public CWndSuper{
	public:
		CStatic();
		virtual ~CStatic();

		virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd = NULL, UINT nID = 0xFFFF);

	protected:
		virtual void PreSubclassWindow();
	};

	class CPictureCtrl :
		public CStatic
	{
	public:

		//Constructor
		CPictureCtrl(void);

		//Destructor
		~CPictureCtrl(void);

		virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd = NULL, UINT nID = 0xFFFF);

	public:
		// Set Icon for static control
		BOOL SetIcon(HICON hIcon);

		// Set Bitmap for static control
		BOOL SetBitmap(HBITMAP hBitmap);

		//Loads an image from a file (OwnerDraw)
		BOOL LoadFromFile(CString &szFilePath);

		//Loads an image from an IStream interface (OwnerDraw)
		BOOL LoadFromStream(IStream* piStream);

		//Loads an image from a byte stream  (OwnerDraw)
		BOOL LoadFromStream(BYTE* pData, size_t nSize);

		//Loads an image from a Resource  (OwnerDraw)
		//Note: GDI+ can not load RT_BITMAP resouce, because they are predefined resource, they don't contains the image file header.
		//You should always put BMP, PNG, JPG resource in RCDATA section .
		BOOL LoadFromResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType = RT_RCDATA);

		//Overload - Single load function  (OwnerDraw)
		BOOL Load(CString &szFilePath);
		BOOL Load(IStream* piStream);
		BOOL Load(BYTE* pData, size_t nSize);
		BOOL Load(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType);

		//Frees the image data
		void FreeData();

	protected:
		virtual void PreSubclassWindow();

		//Draws the Control
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual BOOL OnEraseBkgnd(HDC hdc);

	private:
		//Internal image stream buffer
		IStream* m_pStream;

		//Control flag if a pic is loaded
		BOOL m_bIsPicLoaded;

		//GDI Plus Token
		ULONG_PTR m_gdiplusToken;
	};
}