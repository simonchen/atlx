#include "atlx.h"
#include "atlx_colls.h"

/////////////////////////////////////////////////////////////////////////////
// CPlex

ATLX::CPlex* __stdcall ATLX::CPlex::Create(CPlex*& pHead, UINT_PTR nMax, UINT_PTR cbElement)
{
	ATLX_ASSERT(nMax > 0 && cbElement > 0);
	if (nMax == 0 || cbElement == 0)
	{
		throw "Invalid arguments!";
	}

	ATLX::CPlex* p = (ATLX::CPlex*) new BYTE[sizeof(ATLX::CPlex) + nMax * cbElement];
	// may throw exception
	p->pNext = pHead;
	pHead = p;  // change head (adds in reverse order for simplicity)
	return p;
}

void ATLX::CPlex::FreeDataChain()     // free this one and links
{
	ATLX::CPlex* p = this;
	while (p != NULL)
	{
		BYTE* bytes = (BYTE*)p;
		ATLX::CPlex* pNext = p->pNext;
		delete[] bytes;
		p = pNext;
	}
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
static inline void DestructElement(ATLX::CString* pOldData)
{
	ATLX_ASSERT(pOldData != NULL);
	pOldData->~basic_string();
}

static inline void CopyElement(ATLX::CString* pSrc, ATLX::CString* pDest)
{
	ATLX_ASSERT(pSrc != NULL && pDest != NULL);
	*pSrc = *pDest;
}

/////////////////////////////////////////////////////////////////////////////

ATLX::CMapStringToPtr::CMapStringToPtr(INT_PTR nBlockSize)
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

inline UINT ATLX::CMapStringToPtr::HashKey(LPCTSTR key) const
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

void ATLX::CMapStringToPtr::InitHashTable(
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
		m_pHashTable = new CAssoc*[nHashSize];
		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

void ATLX::CMapStringToPtr::RemoveAll()
{
	if (m_pHashTable != NULL)
	{
		// destroy elements
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
				pAssoc = pAssoc->pNext)
			{
				DestructElement(&pAssoc->key);  // free up string data

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

ATLX::CMapStringToPtr::~CMapStringToPtr()
{
	RemoveAll();
	ATLX_ASSERT(m_nCount == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Assoc helpers
// same as CList implementation except we store CAssoc's not CNode's
//    and CAssoc's are singly linked all the time

ATLX::CMapStringToPtr::CAssoc*
ATLX::CMapStringToPtr::NewAssoc()
{
	if (m_pFreeList == NULL)
	{
		// add another block
		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
			sizeof(CMapStringToPtr::CAssoc));
		// chain them into free list
		CMapStringToPtr::CAssoc* pAssoc =
			(CMapStringToPtr::CAssoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (INT_PTR i = m_nBlockSize - 1; i >= 0; i--, pAssoc--)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	ATLX_ASSERT(m_pFreeList != NULL);  // we must have something

	CMapStringToPtr::CAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	ATLX_ASSERT(m_nCount > 0);  // make sure we don't overflow
#pragma push_macro("new")
#undef new
	new(&pAssoc->key) CString;
#pragma pop_macro("new")


	pAssoc->value = 0;

	return pAssoc;
}

void ATLX::CMapStringToPtr::FreeAssoc(CMapStringToPtr::CAssoc* pAssoc)
{
	DestructElement(&pAssoc->key);  // free up string data

	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
	ATLX_ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

ATLX::CMapStringToPtr::CAssoc*
ATLX::CMapStringToPtr::GetAssocAt(LPCTSTR key, UINT& nHashBucket, UINT& nHashValue) const
// find association (or return NULL)
{
	nHashValue = HashKey(key);
	nHashBucket = nHashValue % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	CAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHashBucket]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (pAssoc->nHashValue == nHashValue && pAssoc->key == key)
			return pAssoc;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

BOOL ATLX::CMapStringToPtr::Lookup(LPCTSTR key, void*& rValue) const
{
	UINT nHashBucket, nHashValue;
	CAssoc* pAssoc = GetAssocAt(key, nHashBucket, nHashValue);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

BOOL ATLX::CMapStringToPtr::LookupKey(LPCTSTR key, LPCTSTR& rKey) const
{
	UINT nHashBucket, nHashValue;
	CAssoc* pAssoc = GetAssocAt(key, nHashBucket, nHashValue);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rKey = pAssoc->key;
	return TRUE;
}

void*& ATLX::CMapStringToPtr::operator[](LPCTSTR key)
{
	UINT nHashBucket, nHashValue;
	CAssoc* pAssoc;
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


BOOL ATLX::CMapStringToPtr::RemoveKey(LPCTSTR key)
// remove key - return TRUE if removed
{
	if (m_pHashTable == NULL)
		return FALSE;  // nothing in the table

	CAssoc** ppAssocPrev;
	UINT nHashValue;
	nHashValue = HashKey(key);
	ppAssocPrev = &m_pHashTable[nHashValue%m_nHashTableSize];

	CAssoc* pAssoc;
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

void ATLX::CMapStringToPtr::GetNextAssoc(POSITION& rNextPosition,
	CString& rKey, void*& rValue) const
{
	ATLX_ASSERT(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)rNextPosition;
	ATLX_ASSERT(pAssocRet != NULL);
	if (pAssocRet == NULL)
		return;

	if (pAssocRet == (CAssoc*)BEFORE_START_POSITION)
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
	CAssoc* pAssocNext;
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
