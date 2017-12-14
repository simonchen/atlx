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
