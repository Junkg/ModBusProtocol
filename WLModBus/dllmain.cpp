// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "CFactory.h"
#include <tchar.h>
extern "C" HMODULE  g_hdllModule = NULL;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
extern "C" LONG DeleteKey(HKEY hKeyParent, LPTSTR lpszKeyChild);
extern "C" HRESULT  __stdcall DllRegisterServer();
extern "C" HRESULT __stdcall DllUnregisterServer();
extern "C" HRESULT __stdcall DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv);
extern "C" HRESULT __stdcall DllCanUnloadNow();

//注册到注册表
extern "C" HRESULT  __stdcall DllRegisterServer()
{
	//获取dll 路径

	if (g_hdllModule == NULL)
		return S_FALSE;

	TCHAR  szPath[MAX_PATH] = { 0 };
	GetModuleFileName(g_hdllModule, szPath, MAX_PATH);


	LPOLESTR  oszCLSID = NULL;
	StringFromCLSID(CLSID_MODBUS, &oszCLSID);

	TCHAR szGUID[64];
	_tcscpy_s(szGUID, 64, oszCLSID);
	CoTaskMemFree(oszCLSID);

	TCHAR KeyPath[64];
	_tcscpy_s(KeyPath, _T("CLSID\\"));
	_tcscat_s(KeyPath, 64, szGUID);

	//创建Key 
	HKEY  hRoot;
	LSTATUS  stRet = ::RegCreateKey(HKEY_CLASSES_ROOT, KeyPath, &hRoot);
	if (stRet != ERROR_SUCCESS)
		return S_FALSE;

	TCHAR szData[] = { _T("Contain Component") };
	::RegSetValueEx(hRoot, NULL, 0, REG_SZ, (BYTE*)szData, _tcslen(szData) * 2 + 1);

	//创建 ProgID
	HKEY  hProgID = 0;
	stRet = ::RegCreateKey(hRoot, _T("ProgID"), &hProgID);
	if (stRet != ERROR_SUCCESS)
		return S_FALSE;

	TCHAR szProgID[] = { _T("Contain.object") };



	::RegSetValueEx(hProgID, NULL, 0, REG_SZ, (BYTE*)szProgID, _tcslen(szProgID) * 2 + 1);

	//InprocServer32
	HKEY hInproc = 0;
	stRet = ::RegCreateKey(hRoot, _T("InprocServer32"), &hInproc);
	if (stRet != ERROR_SUCCESS)
		return S_FALSE;
	::RegSetValueEx(hInproc, NULL, 0, REG_SZ, (BYTE*)szPath, _tcslen(szPath) * 2 + 1);

	::RegCloseKey(hProgID);
	::RegCloseKey(hInproc);
	::RegCloseKey(hRoot);
	return S_OK;
}

extern "C" HRESULT __stdcall DllUnregisterServer()
{
	//GUID
	LPOLESTR  oszCLSID = NULL;
	StringFromCLSID(CLSID_MODBUS, &oszCLSID);

	TCHAR szGUID[64];
	_tcscpy_s(szGUID, 64, oszCLSID);
	CoTaskMemFree(oszCLSID);

	TCHAR KeyPath[64];
	_tcscpy_s(KeyPath, _T("CLSID\\"));
	_tcscat_s(KeyPath, 64, szGUID);



	LONG  lRet = DeleteKey(HKEY_CLASSES_ROOT, KeyPath);

	return lRet;
}

extern "C" HRESULT __stdcall DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{
	if (rclsid != CLSID_MODBUS)
		return CLASS_E_CLASSNOTAVAILABLE;

	CFactory* pFactory = new CFactory;
	if (pFactory == NULL)
		return E_OUTOFMEMORY;

	HRESULT hr = pFactory->QueryInterface(riid, ppv);
	return hr;
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
	if ((g_LockNumber == 0) && (g_ObjNumber == 0))
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


extern "C" LONG DeleteKey(HKEY hKeyParent, LPTSTR lpszKeyChild)
{

	HKEY hKeyChild;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0,
		KEY_ALL_ACCESS, &hKeyChild);
	if (lRes != ERROR_SUCCESS)
		return lRes;

	// Enumerate all of the decendents of this child.
	FILETIME time;
	TCHAR szBuffer[256];
	DWORD dwSize = 256;
	while (RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, NULL,
		NULL, NULL, &time) == S_OK)
	{
		// Delete the decendents of this child.
		lRes = DeleteKey(hKeyChild, szBuffer);
		if (lRes != ERROR_SUCCESS)
		{
			// Cleanup before exiting.
			RegCloseKey(hKeyChild);
			return lRes;
		}
		dwSize = 256;
	}

	// Close the child.
	RegCloseKey(hKeyChild);

	// Delete this child.
	return RegDeleteKey(hKeyParent, lpszKeyChild);
}

