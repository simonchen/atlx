#include "StdAfx.h"
#include "WndThunker.h"

HANDLE ATLX::CWndThunker::s_hPrivHeap = NULL;

ATLX::CWndThunker::CWndThunker(void) : m_fDialog(FALSE), m_oldDlgProc(NULL), m_oldWndProc(NULL), m_fMsgHandled(FALSE)
{
	// Uses Heap to construct the thunk(s) for avoiding DEP (Data Execution Prevention)
	if (!s_hPrivHeap)
	{
		// dwMaximumSize is zero that means it specifies that the private heap is growable. 
		// The heap's size is limited only by available memory. 
		s_hPrivHeap = ::HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
		if (!s_hPrivHeap) throw "error: failed to create private heap!";
	}

	m_thunk = (_stdcallthunk*)::HeapAlloc(s_hPrivHeap, HEAP_ZERO_MEMORY, sizeof(_stdcallthunk));//(_stdcallthunk*)VirtualAlloc(NULL, sizeof(_stdcallthunk), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!m_thunk) throw "error: m_thunk cannot be allocated by HeapAlloc";
#if defined(_M_IX86) 
	// mov dword ptr [esp+0x14], pThis (esp + 0x14 is the custom 5th param- pThis)
	m_thunk->m_mov = 0x142444C7;
#elif defined (_M_AMD64)
	// mov r9, pThis (r9 is the 64 bit register to store the 4th parameter - lParam
	m_thunk->m_mov = 0xb949; // mov r9, pThis
#endif
	
	m_thunk2 = (_stdcallthunk*)::HeapAlloc(s_hPrivHeap, HEAP_ZERO_MEMORY, sizeof(_stdcallthunk));//(_stdcallthunk*)VirtualAlloc(NULL, sizeof(_stdcallthunk), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!m_thunk2) throw "error: m_thunk2 cannot be allocated by HeapAlloc";
#if defined(_M_IX86)
	// mov dword ptr [esp+0x4], pThis (esp+0x04 is hWnd, pThis is assigned to hWnd)
	m_thunk2->m_mov = 0x042444C7; 
#elif defined (_M_AMD64)
	// mov rcx, pThis (rcx is the 64 bit register to store the 1st parameter - hWnd
	m_thunk2->m_mov = 0xb948; 
#endif
}

ATLX::CWndThunker::~CWndThunker(void)
{
	if (m_thunk)
	{
		HeapFree(s_hPrivHeap, 0, m_thunk);//MEM_RELEASE);
		m_thunk = NULL;
	}
	if (m_thunk2)
	{
		HeapFree(s_hPrivHeap, 0, m_thunk2);//MEM_RELEASE);
		m_thunk2 = NULL;
	}
}

INT_PTR CALLBACK ATLX::CWndThunker::StartWindowProc(HWND hwnd,  // handle to window
							UINT uMsg,     // message  WPARAM wParam, 
							WPARAM wParam, // first message parameter
							LPARAM lParam,// second message parameter
							DWORD_PTR This
							)
{
#if defined(_M_IX86)
	CWndThunker* pThis = (CWndThunker*)This;
#elif defined(_M_AMD64)
	CTestDlg* pThis = (CTestDlg*)lParam;
#endif
	pThis->m_hWnd = hwnd;

	// Initalize next thunk ..
	pThis->m_thunk2->Init((DWORD_PTR)pThis->WindowProc, pThis);
	if (pThis->m_fDialog)
	{
		DLGPROC pProc = (DLGPROC)pThis->m_thunk2->GetCodeAddress();
		pThis->m_oldDlgProc = (DLGPROC)::SetWindowLongPtr(hwnd, DWLP_DLGPROC, (LONG_PTR)pProc);
		return pProc(hwnd, uMsg, wParam, lParam);
	}
	else
	{
		WNDPROC pProc = (WNDPROC)pThis->m_thunk2->GetCodeAddress();
		pThis->m_oldWndProc = (WNDPROC)::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)pProc);
		return pProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

INT_PTR CALLBACK ATLX::CWndThunker::WindowProc(HWND hwnd,  // handle to window
							UINT uMsg,     // message  WPARAM wParam, 
							WPARAM wParam, // first message parameter
							LPARAM lParam  // second message parameter
							)
{
	CWndThunker* pThis = (CWndThunker*)hwnd; // We take out pThis from hwnd
	hwnd = pThis->m_hWnd; // Re-assign HWND back to hwndDlg .

	pThis->m_fMsgHandled = FALSE;
	LRESULT ret = pThis->ProcessMessage(uMsg, wParam, lParam, pThis->m_fMsgHandled);
	if (pThis->m_fMsgHandled)
		return ret;

	if (!pThis->m_fDialog && pThis->m_oldDlgProc)
		return ::CallWindowProc(pThis->m_oldWndProc, hwnd, uMsg, wParam, lParam);

	return 0;
}

LRESULT ATLX::CWndThunker::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled)
{
	// Overridden by sub-class
	return 0;
}