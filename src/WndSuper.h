#pragma once

#include "atlx_colls.h"
#include "atlx_string.h"
#include "WndThunker.h"

namespace ATLX{

	// A module state class to manage Windows 
	class CHandleMap : public CMapHwndToPtr
	{
	public:
		CHandleMap(BOOL bFreePtr = FALSE) : m_fBkgThread(TRUE)
		{
			m_bFreePtr = bFreePtr;
			::InitializeCriticalSection(&m_cs);

			DWORD dwThreadId = 0;
			//m_hBkgThread = ::CreateThread(NULL, NULL, CHandleMap::IdleProc, this, THREAD_BASE_PRIORITY_IDLE, &dwThreadId);
		}

		virtual ~CHandleMap(void)
		{
			if (m_hBkgThread)
			{
				m_fBkgThread = FALSE;
				::WaitForSingleObject(m_hBkgThread, INFINITE);
			}

			RemoveAll();
		}

		static DWORD WINAPI IdleProc(PVOID pVoid)
		{
			CHandleMap* pThis = (CHandleMap*)pVoid;

			while (pThis->m_fBkgThread)
			{
				/*
				MSG msg;
				while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
				{
					//if (!::GetMessage(&msg, NULL, NULL, NULL))
					//{
					//	pThis->m_fBkgThread = FALSE;
					//	::PostQuitMessage(0);
					//	break;
					//}

					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				// idle processing to remove invalid window's handles.
				pThis->RemoveInvalidHandles();
				*/
				Sleep(1000);
			}

			return 0;
		}

		void* FromHandle(HWND hWnd)
		{
			::EnterCriticalSection(&m_cs);

			void* ptr = NULL;
			CMapHwndToPtr::Lookup(hWnd, ptr);

			::LeaveCriticalSection(&m_cs);

			return ptr;
		}

		void AddHandle(HWND hWnd, void* ptr)
		{
			::EnterCriticalSection(&m_cs);

			SetAt(hWnd, ptr);

			::LeaveCriticalSection(&m_cs);
		}

		void RemoveHandle(HWND hWnd)
		{
			::EnterCriticalSection(&m_cs);

			void* ptr = NULL;
			if (Lookup(hWnd, ptr) && m_bFreePtr && ptr)
				delete ptr;

			RemoveKey(hWnd);

			::LeaveCriticalSection(&m_cs);
		}

		virtual void RemoveAll()
		{
			::EnterCriticalSection(&m_cs);

			POSITION pos = GetStartPosition();
			HWND hWnd = NULL;
			void* ptr = NULL;

			while (pos)
			{
				GetNextAssoc(pos, hWnd, ptr);
				if (pos && m_bFreePtr && ptr)
					delete ptr;
			}

			CMapHwndToPtr::RemoveAll();

			::LeaveCriticalSection(&m_cs);
		}

		void RemoveInvalidHandles()
		{
			::EnterCriticalSection(&m_cs);

			if (m_nCount == 0) return;

			HWND* hdls = new HWND[m_nCount];
			for (int i = 0; i < m_nCount; i++)
				hdls[i] = NULL;

			POSITION pos = GetStartPosition();
			HWND hWnd = NULL;
			void* ptr = NULL;
			int nIndex = 0;

			while (pos)
			{
				GetNextAssoc(pos, hWnd, ptr);
				if (!::IsWindow(hWnd)) // check if window is invalid
				{
					hdls[nIndex++] = hWnd;
				}
			}

			for (int i = 0; i < m_nCount; i++)
			{
				HWND hWnd = hdls[i];
				void* ptr = NULL;
				Lookup(hWnd, ptr);
				if (hWnd)
				{
					RemoveKey(hWnd);
					if (m_bFreePtr && ptr)
						delete ptr;
				}
			}

			::LeaveCriticalSection(&m_cs);
		}

	private:
		BOOL m_bFreePtr;
		CRITICAL_SECTION m_cs;
		BOOL m_fBkgThread;
		HANDLE m_hBkgThread;
	};

	class CToolTipCtrl;

	// Uses CWndSuper to create window or subclassing window
	class CWndSuper : public ATLX::CWndThunker
	{
	public:
		CWndSuper(HINSTANCE hInst=NULL);
	public:
		virtual ~CWndSuper(void);

		virtual LRESULT ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled);
		LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

		// Create window, when you don't need the window, you should call Destory()
		// If you're creating system class (such as Static, Button, ListCtrl, Listbox, etc.), you won't receive WM_CREATE message,
		// therefore, OnCreate event won't work in this case.
		BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, CWndSuper* pParentWnd = NULL, UINT nID = 0);
		BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd = NULL, UINT nID = 0);
		BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, CWndSuper* pParentWnd = NULL, UINT nID = 0);
		BOOL CreateEx(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWndSuper* pParentWnd = NULL, UINT nID = 0);

	protected:
		// immediate parent window, 
		// Note, if you don't pass parent window pointer on create, it won't be able to find the window's parent by GetParent(),
		// in case, you have to use win32 API to search the real handle of parent window.
		CWndSuper* m_pParent; 

		// Global map of temporary windows
		static CHandleMap s_mapTemp;

		// Global map of permanent windows
		static CHandleMap s_mapPerm;

		static void AddPermanentHandle(HWND hWnd, void* ptr)
		{
			s_mapPerm.AddHandle(hWnd, ptr);
		}

		static void RemovePermanentHandle(HWND hWnd)
		{
			s_mapPerm.RemoveHandle(hWnd);
		}

		static void AddTempHandle(HWND hWnd, void* ptr)
		{
			s_mapTemp.AddHandle(hWnd, ptr);
		}

		static void RemoveTempHandle(HWND hWnd)
		{
			s_mapTemp.RemoveHandle(hWnd);
		}

	public:
		HWND GetSafeHwnd()
		{
			return (::IsWindow(m_hWnd) ? m_hWnd : NULL);
		}

		// Getting temporary window pointer by handle.
		static CWndSuper* FromHandle(HWND hWnd)
		{
			void* pWnd = s_mapPerm.FromHandle(hWnd);
			if (!pWnd) pWnd = s_mapTemp.FromHandle(hWnd);
			if (!pWnd)
			{
				pWnd = new CWndSuper();
				((CWndSuper*)pWnd)->m_hWnd = hWnd;
				CWndSuper::AddTempHandle(hWnd, pWnd);
			}

			return (CWndSuper*)pWnd;
		}

		// immediate parent window
		CWndSuper* GetParent() { 
			CWndSuper* pParent = m_pParent;
			if (!pParent)
			{
				HWND hParent = ::GetParent(m_hWnd);
				pParent = CWndSuper::FromHandle(hParent);
			}
			
			return pParent;
			
		}
		// immediate parent window or owner window without WS_CHILD style
		CWndSuper* GetParentOwner()
		{
			HWND hParent = m_hWnd;
			while (hParent && (::GetWindowLong(hParent, GWL_STYLE) & WS_CHILD))
			{
				hParent = ::GetParent(hParent);
			}

			CWndSuper* pParent = NULL;
			if (hParent)
				pParent = CWndSuper::FromHandle(hParent);

			return pParent;
		}
		// Get top level owner
		CWndSuper* GetTopLevelParent()
		{
			CWndSuper* pWnd = GetParentOwner();
			CWndSuper* pWndT = NULL;
			while ( pWnd && (pWndT = pWnd->GetParentOwner()) != NULL)
				pWnd = pWndT;

			return pWnd;
		}

		// See if top level parent is the last active
		BOOL IsTopParentActive(){
			CWndSuper* pWnd = GetTopLevelParent();
			if (!pWnd) return FALSE;

			return (::GetForegroundWindow() == ::GetLastActivePopup(pWnd->m_hWnd));
		}

		BOOL SubclassWindow(HWND hWnd);
		virtual void PreSubclassWindow();
		BOOL UnSubclassWindow();

	public:
		DWORD GetStyle() const;
		DWORD GetExStyle() const;

		BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd);
		BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd);

		void SetWindowText(TCHAR* pszText);
		ATLX::CString GetWindowText() const;

		BOOL GetWindowRect(LPRECT lpRect) const
		{
			return ::GetWindowRect(m_hWnd, lpRect);
		}

		BOOL GetClientRect(LPRECT lpRect) const
		{
			return ::GetClientRect(m_hWnd, lpRect);
		}

		BOOL ClientToScreen(LPPOINT lpPoint) const 
		{
			return ::ClientToScreen(m_hWnd, lpPoint);
		}

		BOOL ClientToScreen(LPRECT lpRect) const 
		{
			if (!::ClientToScreen(m_hWnd, (LPPOINT)lpRect))
				return FALSE;
			return ::ClientToScreen(m_hWnd, ((LPPOINT)lpRect) + 1);
		}

		BOOL ScreenToClient(LPPOINT lpPoint) const
		{
			return ::ScreenToClient(m_hWnd, lpPoint);
		}

		BOOL ScreenToClient(LPRECT lpRect) const
		{
			if (!::ScreenToClient(m_hWnd, (LPPOINT)lpRect))
				return FALSE;
			return ::ScreenToClient(m_hWnd, ((LPPOINT)lpRect) + 1);
		}

	protected:
		virtual INT_PTR InitThunker2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual BOOL OnCreate(LPCREATESTRUCT pcs);

		virtual void OnDestroy();

		virtual void OnPaint();

		virtual BOOL OnEraseBkgnd(HDC hdc);

		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// Utils
	public: 
		static BOOL PtInRect(const POINT& pt, const RECT& rect);

		static HWND ChildWindowFromPoint(HWND hWnd, POINT& pt);

		static HWND TopChildWindowFromPoint(HWND hWnd, POINT& pt);

	// Tool tip relates
	public:
		INT_PTR OnToolHitTest(POINT& point, TOOLINFO* pTI) const;

		BOOL EnableToolTips(BOOL bEnable, BOOL bTracking=FALSE);

	protected: // Tooltip filters
		static void _stdcall _FilterToolTipMessage(MSG* pMsg, CWndSuper* pWnd);
		void FilterToolTipMessage(MSG* pMsg);

		struct ToolTipHelper
		{
			CToolTipCtrl* m_pToolTip;
			BOOL	m_bEnableToolTip;
			UINT	m_nLastHit; // Last hitted control ID with tool tip
			CWndSuper* m_pLastHit; // Last hitted windows for tool tip
			
			TOOLINFO* m_pLastInfo;

			struct ToolTipHelper();

			~ToolTipHelper()
			{
				if (m_pToolTip)
				{
					delete m_pToolTip;
					m_pToolTip = NULL;
				}

				if (m_pLastInfo)
				{
					delete m_pLastInfo;
					m_pLastInfo = NULL;
				}
			}
		};
		static ToolTipHelper s_tth;

		BOOL m_bEnableToolTip;
		BOOL	m_bTracking; // It hints that Tool tip that have flag TTF_TRACK

	protected:
		HINSTANCE m_hInst; // The instance associated 
	};

}