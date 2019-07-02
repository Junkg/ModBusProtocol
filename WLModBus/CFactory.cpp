#include "pch.h"
#include "CFactory.h"


ULONG    g_LockNumber = 0;

CFactory::CFactory()
{
	m_Ref = 0; 
}

HRESULT __stdcall CFactory::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown)
	{
		*ppv = (IUnknown*)this;
		((IUnknown*)(*ppv))->AddRef();
	}else if (riid == IID_IClassFactory)
	{
		*ppv = (IClassFactory*)this;
		((IClassFactory*)(*ppv))->AddRef();
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG __stdcall CFactory::AddRef(void)
{
	m_Ref++;
	return m_Ref;
}

ULONG __stdcall CFactory::Release(void)
{
	m_Ref--;
	if (m_Ref == 0 )
	{
		delete this;
	}
	return m_Ref;
}

HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	if ((pUnkOuter != NULL) && (riid != IID_IUnknown))
		return CLASS_E_NOAGGREGATION;


	CMyComObject* pObj = new CMyComObject(pUnkOuter);
	if (pObj == NULL)
		return E_OUTOFMEMORY;

	HRESULT hr = pObj->NodelegationQueryInyterface(riid, ppv);
	if (hr != S_OK)
	{
		g_ObjNumber--; // Reference count g_cDictionary be added in constructor
		delete pObj;
		return hr;
	}
	return S_OK;
}

HRESULT __stdcall CFactory::LockServer(BOOL fLock)
{
	if (fLock)
		g_LockNumber++;
	else
		g_LockNumber--;
	return NOERROR;
}
