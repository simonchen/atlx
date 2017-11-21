#pragma once

namespace ATLX{

#if defined(_M_IX86)

#pragma pack(push,1)
	struct _stdcallthunk
	{
		DWORD   m_mov;          // 
		DWORD   m_this;         //
		BYTE    m_jmp;          // jmp WndProc
		DWORD   m_relproc;      // relative jmp
		BOOL Init(DWORD_PTR proc, void* pThis)
		{
			m_this = PtrToUlong(pThis);
			m_jmp = 0xe9;
			m_relproc = DWORD((INT_PTR)proc - ((INT_PTR)this + sizeof(_stdcallthunk)));
			// write block from data cache and
			//  flush from instruction cache
			FlushInstructionCache(GetCurrentProcess(), this, sizeof(_stdcallthunk));
			return TRUE;
		}
		//some thunks will dynamically allocate the memory for the code
		void* GetCodeAddress()
		{
			return this;
		}
	};
#pragma pack(pop)

#elif defined(_M_AMD64)

#pragma pack(push,2)
	struct _stdcallthunk
	{
		USHORT	m_mov;
		ULONG64 m_this;         // 
		USHORT  RaxMov;         // mov rax, target
		ULONG64 RaxImm;         //
		USHORT  RaxJmp;         // jmp target
		BOOL Init(DWORD_PTR proc, void *pThis)
		{
			m_this = (ULONG64)pThis;  // 
			RaxMov = 0xb848;          // mov rax, target
			RaxImm = (ULONG64)proc;   //
			RaxJmp = 0xe0ff;          // jmp rax
			FlushInstructionCache(GetCurrentProcess(), this, sizeof(_stdcallthunk));
			return TRUE;
		}
		//some thunks will dynamically allocate the memory for the code
		void* GetCodeAddress()
		{
			return this;
		}
	};
#pragma pack(pop)

#endif

	//////////////////////////////////////////////////////////////////////
	// CWndThunker - Base class for process messages for either window or dialog.
	class CWndThunker
	{
	public:
		CWndThunker(void);
	public:
		virtual ~CWndThunker(void);

		HWND m_hWnd;
		BOOL m_fDialog;
		DLGPROC m_oldDlgProc;
		WNDPROC m_oldWndProc;

		BOOL m_fMsgHandled;

		// A static private heap for room thunks.
		static HANDLE s_hPrivHeap;

		struct _stdcallthunk *m_thunk;
		struct _stdcallthunk *m_thunk2;

		// Delete thunks
		void Delete();

		// Destroy window
		BOOL Destroy();

		static INT_PTR CALLBACK WindowProc(HWND hwnd,  // handle to window
			UINT uMsg,     // message  WPARAM wParam, 
			WPARAM wParam, // first message parameter
			LPARAM lParam  // second message parameter
			);

		static INT_PTR CALLBACK StartWindowProc(HWND hwnd,  // handle to window
			UINT uMsg,     // message 
			WPARAM wParam, // first message parameter
			LPARAM lParam,  // second message parameter
			DWORD_PTR This
			);

		virtual LRESULT ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bMsgHandled);
	};

} // ATLX