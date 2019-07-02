#pragma once
#include <Unknwnbase.h>

#include "CMyComObject.h"

extern "C" ULONG    g_LockNumber; 

class CFactory : public IClassFactory
{
public:
	CFactory();

	// IUnKnown  
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_opt_  REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppv);
	virtual ULONG   STDMETHODCALLTYPE AddRef(void);
	virtual ULONG   STDMETHODCALLTYPE Release(void);

	// IClassFactory
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(_In_opt_  IUnknown* pUnkOuter, _In_  REFIID riid, _COM_Outptr_  void** ppv);
	virtual HRESULT STDMETHODCALLTYPE LockServer(/* [in] */ BOOL fLock);

private:
	ULONG  m_Ref;
};

