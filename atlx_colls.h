#pragma once

#include "atlx_string.h"

#ifndef BEFORE_START_POSITION
#define BEFORE_START_POSITION ((ATLX::POSITION)-1L)
#endif

namespace ATLX{
	struct __POSITION {};
	typedef __POSITION* POSITION;

#pragma pack(push, 4)
	struct CPlex     // warning variable length structure
	{
		CPlex* pNext;
		// BYTE data[maxNum*elementSize];

		void* data() { return this + 1; }

		static CPlex* __stdcall Create(CPlex*& head, UINT_PTR nMax, UINT_PTR cbElement);
		// like 'calloc' but no zero fill
		// may throw memory exceptions

		void FreeDataChain();       // free this one and links
	};
#pragma pack(pop)


	/////////////////////////////////////////////////////////////////////////////
	// CMapStringToPtr
	class CMapStringToPtr
	{
	protected:
		// Association
		struct CAssoc
		{
			CAssoc* pNext;
			UINT nHashValue;  // needed for efficient iteration
			CString key;
			void* value;
		};

	public:

		// Construction
		explicit CMapStringToPtr(INT_PTR nBlockSize = 10);

		// Attributes
		// number of elements
		INT_PTR GetCount() const;
		INT_PTR GetSize() const;
		BOOL IsEmpty() const;

		// Lookup
		BOOL Lookup(LPCTSTR key, void*& rValue) const;
		BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const;

		// Operations
		// Lookup and add if not there
		void*& operator[](LPCTSTR key);

		// add a new (key, value) pair
		void SetAt(LPCTSTR key, void* newValue);

		// removing existing (key, ?) pair
		BOOL RemoveKey(LPCTSTR key);
		void RemoveAll();

		// iterating all (key, value) pairs
		POSITION GetStartPosition() const;
		void GetNextAssoc(POSITION& rNextPosition, CString& rKey, void*& rValue) const;

		// advanced features for derived classes
		UINT GetHashTableSize() const;
		void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

		// Overridables: special non-virtual (see map implementation for details)
		// Routine used to user-provided hash keys
		UINT HashKey(LPCTSTR key) const;

		// Implementation
	protected:
		CAssoc** m_pHashTable;
		UINT m_nHashTableSize;
		INT_PTR m_nCount;
		CAssoc* m_pFreeList;
		struct CPlex* m_pBlocks;
		INT_PTR m_nBlockSize;

		CAssoc* NewAssoc();
		void FreeAssoc(CAssoc*);
		CAssoc* GetAssocAt(LPCTSTR, UINT&, UINT&) const;

	public:
		~CMapStringToPtr();

	protected:
		// local typedefs for CTypedPtrMap class template
		typedef CString BASE_KEY;
		typedef LPCTSTR BASE_ARG_KEY;
		typedef void* BASE_VALUE;
		typedef void* BASE_ARG_VALUE;
	};

	inline void CMapStringToPtr::SetAt(LPCTSTR key, void* newValue)
	{
		(*this)[key] = newValue;
	}
}