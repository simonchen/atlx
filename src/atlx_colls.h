#pragma once

#include <stdlib.h>
#include "atlx_macro.h"
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

	// Association
	template<class TKey, class TValue>
	struct CAssoc
	{
		CAssoc<TKey, TValue>* pNext;
		UINT nHashValue;  // needed for efficient iteration
		TKey key;
		TValue value;
	};

	/////////////////////////////////////////////////////////////////////////////
	// CMap
	template<class TKey, class TValue>
	class CMap
	{
	public:

		// Construction
		explicit CMap(INT_PTR nBlockSize = 10);

		// Attributes
		// number of elements
		INT_PTR GetCount() const;
		INT_PTR GetSize() const;
		BOOL IsEmpty() const;

		// Lookup
		BOOL Lookup(TKey key, TValue& rValue) const;
		BOOL LookupKey(TKey key, TKey& rKey) const;

		// Operations
		// Lookup and add if not there
		void*& operator[](TKey key);

		// add a new (key, value) pair
		void SetAt(TKey key, TValue newValue);

		// removing existing (key, ?) pair
		BOOL RemoveKey(TKey key);
		virtual void RemoveAll();

		// iterating all (key, value) pairs
		POSITION GetStartPosition() const;
		void GetNextAssoc(POSITION& rNextPosition, TKey& rKey, TValue& rValue) const;
		//void GetNextAssoc<CString, TValue>(POSITION& rNextPosition, CString& rKey, TValue& rValue) const;

		// advanced features for derived classes
		UINT GetHashTableSize() const;
		void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

		// Overridables: special non-virtual (see map implementation for details)
		// Routine used to user-provided hash keys
		UINT HashKey(TKey key) const;

		// Implementation
	protected:
		CAssoc<TKey, TValue>** m_pHashTable;
		UINT m_nHashTableSize;
		INT_PTR m_nCount;
		CAssoc<TKey, TValue>* m_pFreeList;
		struct CPlex* m_pBlocks;
		INT_PTR m_nBlockSize;

		CAssoc<TKey, TValue>* NewAssoc();
		void FreeAssoc(CAssoc<TKey, TValue>*);
		CAssoc<TKey, TValue>* Test();
		CAssoc<TKey, TValue>* GetAssocAt(TKey key, UINT& nHashBucket, UINT& nHashValue) const;

	public:
		~CMap();

	protected:
		// local typedefs for CTypedPtrMap class template
		typedef CString BASE_KEY;
		typedef LPCTSTR BASE_ARG_KEY;
		typedef void* BASE_VALUE;
		typedef void* BASE_ARG_VALUE;
	};

	template<class TKey, class TValue>
	inline INT_PTR CMap<TKey, TValue>::GetCount() const
	{
		return m_nCount;
	}
	template<class TKey, class TValue>
	inline INT_PTR CMap<TKey, TValue>::GetSize() const
	{
		return m_nCount;
	}
	template<class TKey, class TValue>
	inline BOOL CMap<TKey, TValue>::IsEmpty() const
	{
		return m_nCount == 0;
	}

	template<class TKey, class TValue>
	inline void CMap<TKey, TValue>::SetAt(TKey key, TValue newValue)
	{
		(*this)[key] = newValue;
	}
	template<class TKey, class TValue>
	inline POSITION CMap<TKey, TValue>::GetStartPosition() const
	{
		return (m_nCount == 0) ? NULL : BEFORE_START_POSITION;
	}
	template<class TKey, class TValue>
	inline UINT CMap<TKey, TValue>::GetHashTableSize() const
	{
		return m_nHashTableSize;
	}

	/////////////////////////////////////////////////////////////////////////////
	// Special implementations for CStrings
	// it is faster to bit-wise copy a CString than to call an official
	//   constructor - since an empty CString can be bit-wise copied

	// Deprecated
	static inline void ConstructElement(ATLX::CString* pNewData)
	{
		ATLX_ASSERT(pNewData != NULL);
		new(pNewData)ATLX::CString;
	}

	// Deprecated
	template<class ARG_DATA>
	static inline void DestructElement(ARG_DATA* pOldData)
	{
		ATLX_ASSERT(pOldData != NULL);
		pOldData->~ARG_DATA();
	}

	template<> static inline void DestructElement<ATLX::CString>(ATLX::CString* pOldData)
	{
		ATLX_ASSERT(pOldData != NULL);
		pOldData->~basic_string();
	}

	// Deprecated
	static inline void CopyElement(ATLX::CString* pSrc, ATLX::CString* pDest)
	{
		ATLX_ASSERT(pSrc != NULL && pDest != NULL);
		*pSrc = *pDest;
	}

	/////////////////////////////////////////////////////////////////////////////
	// HashKey functions
	template<class ARG_KEY>
	inline UINT __stdcall HashKey(ARG_KEY key)
	{
		// (algorithm copied from STL hash in xfunctional)
#pragma warning(suppress: 4302) // 'type cast' : truncation
		ldiv_t HashVal = ldiv((long)(ARG_KEY)key, 127773);
		HashVal.rem = 16807 * HashVal.rem - 2836 * HashVal.quot;
		if (HashVal.rem < 0)
			HashVal.rem += 2147483647;
		return ((UINT)HashVal.rem);
	}

	template<> inline UINT __stdcall HashKey<__int64>(__int64 key)
	{
		// (algorithm copied from STL hash in xfunctional)
		return (HashKey<DWORD>((DWORD)(key & 0xffffffffUL)) ^ HashKey<DWORD>((DWORD)(key >> 32)));
	}

	template<> inline UINT HashKey<LPCTSTR>(LPCTSTR key)
	{
		if (key == NULL)
		{
			throw "Invalid argument key!";
		}

		// hash key to UINT value by pseudorandomizing transform
		// (algorithm copied from STL string hash in xfunctional)
		UINT uHashVal = 2166136261U;
		UINT uFirst = 0;
		UINT uLast = (UINT)_tcslen(key);
		UINT uStride = 1 + uLast / 10;

		for (; uFirst < uLast; uFirst += uStride)
		{
			uHashVal = 16777619U * uHashVal ^ (UINT)key[uFirst];
		}

		return(uHashVal);
	}

	template<> inline UINT HashKey<ATLX::CString>(const ATLX::CString key)
	{
		return HashKey<LPCTSTR>((LPCTSTR)key);
	}

	/////////////////////////////////////////////////////////////////////////////
	template<class TKey, class TValue>
	ATLX::CMap<TKey, TValue>::CMap(INT_PTR nBlockSize)
	{
		ATLX_ASSERT(nBlockSize > 0);
		if (nBlockSize <= 0)
			nBlockSize = 10;	// default size

		m_pHashTable = NULL;
		m_nHashTableSize = 17;  // default size
		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks = NULL;
		m_nBlockSize = nBlockSize;
	}

	template<class TKey, class TValue>
	inline UINT ATLX::CMap<TKey, TValue>::HashKey(TKey key) const
	{
		return ATLX::HashKey(key);
	}

	template<class TKey, class TValue>
	void ATLX::CMap<TKey, TValue>::InitHashTable(
		UINT nHashSize, BOOL bAllocNow)
		//
		// Used to force allocation of a hash table or to override the default
		//   hash table size of (which is fairly small)
	{
		ATLX_ASSERT(m_nCount == 0);
		ATLX_ASSERT(nHashSize > 0);
		if (nHashSize == 0)
			nHashSize = 17;	// default value

		if (m_pHashTable != NULL)
		{
			// free hash table
			delete[] m_pHashTable;
			m_pHashTable = NULL;
		}

		if (bAllocNow)
		{
			m_pHashTable = new CAssoc<TKey, TValue>*[nHashSize];
			memset(m_pHashTable, 0, sizeof(CAssoc<TKey, TValue>*) * nHashSize);
		}
		m_nHashTableSize = nHashSize;
	}

	template<class TKey, class TValue>
	void ATLX::CMap<TKey, TValue>::RemoveAll()
	{
		if (m_pHashTable != NULL)
		{
			// destroy elements
			for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
			{
				CAssoc<TKey, TValue>* pAssoc;
				for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
					pAssoc = pAssoc->pNext)
				{
					DestructElement<TKey>(&pAssoc->key);  // free up string data

				}
			}

			// free hash table
			delete[] m_pHashTable;
			m_pHashTable = NULL;
		}

		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks->FreeDataChain();
		m_pBlocks = NULL;
	}

	template<class TKey, class TValue>
	ATLX::CMap<TKey, TValue>::~CMap()
	{
		RemoveAll();
		ATLX_ASSERT(m_nCount == 0);
	}

	/////////////////////////////////////////////////////////////////////////////
	// Assoc helpers
	// same as CList implementation except we store CAssoc's not CNode's
	//    and CAssoc's are singly linked all the time

	template<class TKey, class TValue>
	ATLX::CAssoc<TKey, TValue>*
		ATLX::CMap<TKey, TValue>::NewAssoc()
	{
		if (m_pFreeList == NULL)
		{
			// add another block
			CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
				sizeof(ATLX::CAssoc<TKey, TValue>));
			// chain them into free list
			CAssoc<TKey, TValue>* pAssoc =
				(CAssoc<TKey, TValue>*) newBlock->data();
			// free in reverse order to make it easier to debug
			pAssoc += m_nBlockSize - 1;
			for (INT_PTR i = m_nBlockSize - 1; i >= 0; i--, pAssoc--)
			{
				pAssoc->pNext = m_pFreeList;
				m_pFreeList = pAssoc;
			}
		}
		ATLX_ASSERT(m_pFreeList != NULL);  // we must have something

		CAssoc<TKey, TValue>* pAssoc = m_pFreeList;
		m_pFreeList = m_pFreeList->pNext;
		m_nCount++;
		ATLX_ASSERT(m_nCount > 0);  // make sure we don't overflow

		//	AssocConstructKey(pAssoc);

		//#pragma push_macro("new")
		//#undef new
		//#pragma pop_macro("new")
		pAssoc->key = NULL;

		pAssoc->value = 0;

		return pAssoc;
	}

	template<class TKey, class TValue>
	void ATLX::CMap<TKey, TValue>::FreeAssoc(CAssoc<TKey, TValue>* pAssoc)
	{
		DestructElement<TKey>(&pAssoc->key);  // free up string data

		pAssoc->pNext = m_pFreeList;
		m_pFreeList = pAssoc;
		m_nCount--;
		ATLX_ASSERT(m_nCount >= 0);  // make sure we don't underflow

		// if no more elements, cleanup completely
		if (m_nCount == 0)
			RemoveAll();
	}

	template<class TKey, class TValue>
	inline ATLX::CAssoc<TKey, TValue>* ATLX::CMap<TKey, TValue>::Test()
	{
		return NULL;
	}

	template<class TKey, class TValue>
	ATLX::CAssoc<TKey, TValue>* ATLX::CMap<TKey, TValue>::GetAssocAt(TKey key, UINT& nHashBucket, UINT& nHashValue) const
		// find association (or return NULL)
	{
		nHashValue = HashKey(key);
		nHashBucket = nHashValue % m_nHashTableSize;

		if (m_pHashTable == NULL)
			return NULL;

		// see if it exists
		CAssoc<TKey, TValue>* pAssoc;
		for (pAssoc = m_pHashTable[nHashBucket]; pAssoc != NULL; pAssoc = pAssoc->pNext)
		{
			if (pAssoc->nHashValue == nHashValue && pAssoc->key == key)
				return pAssoc;
		}
		return NULL;
	}

	/////////////////////////////////////////////////////////////////////////////

	template<class TKey, class TValue>
	BOOL ATLX::CMap<TKey, TValue>::Lookup(TKey key, TValue& rValue) const
	{
		UINT nHashBucket, nHashValue;
		CAssoc<TKey, TValue>* pAssoc = GetAssocAt(key, nHashBucket, nHashValue);
		if (pAssoc == NULL)
			return FALSE;  // not in map

		rValue = pAssoc->value;
		return TRUE;
	}

	template<class TKey, class TValue>
	BOOL ATLX::CMap<TKey, TValue>::LookupKey(TKey key, TKey& rKey) const
	{
		UINT nHashBucket, nHashValue;
		CAssoc<TKey, TValue>* pAssoc = GetAssocAt(key, nHashBucket, nHashValue);
		if (pAssoc == NULL)
			return FALSE;  // not in map

		rKey = pAssoc->key;
		return TRUE;
	}

	template<class TKey, class TValue>
	void*& ATLX::CMap<TKey, TValue>::operator[](TKey key)
	{
		UINT nHashBucket, nHashValue;
		CAssoc<TKey, TValue>* pAssoc;
		if ((pAssoc = GetAssocAt(key, nHashBucket, nHashValue)) == NULL)
		{
			if (m_pHashTable == NULL)
				InitHashTable(m_nHashTableSize);

			// it doesn't exist, add a new Association
			pAssoc = NewAssoc();
			pAssoc->nHashValue = nHashValue;
			pAssoc->key = key;
			// 'pAssoc->value' is a constructed object, nothing more

			// put into hash table
			pAssoc->pNext = m_pHashTable[nHashBucket];
			m_pHashTable[nHashBucket] = pAssoc;
		}
		return pAssoc->value;  // return new reference
	}

	template<class TKey, class TValue>
	BOOL ATLX::CMap<TKey, TValue>::RemoveKey(TKey key)
		// remove key - return TRUE if removed
	{
		if (m_pHashTable == NULL)
			return FALSE;  // nothing in the table

		CAssoc<TKey, TValue>** ppAssocPrev;
		UINT nHashValue;
		nHashValue = HashKey(key);
		ppAssocPrev = &m_pHashTable[nHashValue%m_nHashTableSize];

		CAssoc<TKey, TValue>* pAssoc;
		for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
		{
			if ((pAssoc->nHashValue == nHashValue) && (pAssoc->key == key))
			{
				// remove it
				*ppAssocPrev = pAssoc->pNext;  // remove from list
				FreeAssoc(pAssoc);
				return TRUE;
			}
			ppAssocPrev = &pAssoc->pNext;
		}
		return FALSE;  // not found
	}

	/////////////////////////////////////////////////////////////////////////////
	// Iterating
	template<class TKey, class TValue>
	void ATLX::CMap<TKey, TValue>::GetNextAssoc(POSITION& rNextPosition,
		TKey& rKey, TValue& rValue) const
	{
		ATLX_ASSERT(m_pHashTable != NULL);  // never call on empty map

		CAssoc<TKey, TValue>* pAssocRet = (CAssoc<TKey, TValue>*)rNextPosition;
		ATLX_ASSERT(pAssocRet != NULL);
		if (pAssocRet == NULL)
			return;

		if (pAssocRet == (CAssoc<TKey, TValue>*)BEFORE_START_POSITION)
		{
			// find the first association
			for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			{
				if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				{
					break;
				}
			}
			ATLX_ASSERT(pAssocRet != NULL);  // must find something
		}

		// find next association
		CAssoc<TKey, TValue>* pAssocNext;
		if ((pAssocNext = pAssocRet->pNext) == NULL)
		{
			// go to next bucket
			for (UINT nBucket = (pAssocRet->nHashValue % m_nHashTableSize) + 1;
				nBucket < m_nHashTableSize; nBucket++)
				if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
					break;
		}

		rNextPosition = (POSITION)pAssocNext;

		// fill in return data
		rKey = pAssocRet->key;
		rValue = pAssocRet->value;
	}

	// Parameter types defined.
	typedef CMap<LPCTSTR, void*> CMapStringToPtr;
	typedef CMap<HWND, void*> CMapHwndToPtr;
}