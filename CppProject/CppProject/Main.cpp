#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "RegistryHelper.h"
#define UNICODE
#define _UNICODE
#include <shlobj.h>
#include <exdisp.h>
#include <shlwapi.h>
#include <atlbase.h>
#include <atlalloc.h>
#include <stdio.h>
#include "exdisp.h"


std::string SayHello();
void FindDesktopFolderView(REFIID riid, void **ppv);


int _tmain(int argc, _TCHAR* argv[])
{
	IShellWindows *psw;
	HRESULT hr= CoInitialize(NULL);
	//if (SUCCEEDED(hr))
	//{
	//	hr = CoCreateInstance(
	//		CLSID_ShellWindows,
	//		NULL,
	//		CLSCTX_ALL,
	//		IID_IShellWindows,
	//		(void**)&psw
	//	);
 //   
	//	if (SUCCEEDED(hr))
	//	{
	//		// Use the IShellWindows instance...
 //       
	//		psw->Release();
	//	}
	//}
	CComPtr<IFolderView> spView;
	FindDesktopFolderView(IID_PPV_ARGS(&spView));
	CComPtr<IShellFolder> spFolder;
	spView->GetFolder(IID_PPV_ARGS(&spFolder));
	CComPtr<IEnumIDList> spEnum;
	spView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&spEnum));
	for (CComHeapPtr<ITEMID_CHILD> spidl;
      spEnum->Next(1, &spidl, nullptr) == S_OK;
      spidl.Free()) 
	{
		STRRET str;
		spFolder->GetDisplayNameOf(spidl, SHGDN_NORMAL, &str);
		CComHeapPtr<wchar_t> spszName;
		StrRetToStr(&str, spidl, &spszName);

		POINT pt;
		spView->GetItemPosition(spidl, &pt);
  
		wprintf(L"At %4d,%4d is %ls\n", pt.x, pt.y, spszName);
	}


	
	
	std:: string val = SayHello();
	RegistryHelper registryHelper;	
	std::string returnVal=registryHelper.GetHello();
	std::wstring s = registryHelper.RegGetString(HKEY_CURRENT_USER, L"Software\\Microfocus\\zapp", L"hashKey");
	std::string ret1= registryHelper.RegGetString1(HKEY_CURRENT_USER, "Software\\Microfocus\\zapp", "hashKey");
	DWORD data = registryHelper.RegGetDword(HKEY_CURRENT_USER, L"Console", L"ColorTable01");
	return 0;
}

std::string SayHello(){
	std::string var ="hello world";
	std::cout << "hello world";	
	return var;
}

void FindDesktopFolderView(REFIID riid, void **ppv)
{
 CComPtr<IShellWindows> spShellWindows;
 spShellWindows.CoCreateInstance(CLSID_ShellWindows);

 CComVariant vtLoc(CSIDL_DESKTOP);
 CComVariant vtEmpty;
 long lhwnd;
 CComPtr<IDispatch> spdisp;
 spShellWindows->FindWindowSW(&vtLoc, &vtEmpty,SWC_DESKTOP, &lhwnd, SWFO_NEEDDISPATCH, &spdisp);

 CComPtr<IShellBrowser> spBrowser;
 CComQIPtr<IServiceProvider>(spdisp)->
	 QueryService(SID_STopLevelBrowser,
	 IID_PPV_ARGS(&spBrowser));

 CComPtr<IShellView> spView;
 spBrowser->QueryActiveShellView(&spView);

 spView->QueryInterface(riid, ppv);
}





