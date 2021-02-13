#include "stdafx.h"
#include "DesktopLayout.h"
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
void FindDesktopFolderView(REFIID riid, void **ppv);

DesktopLayout::DesktopLayout(void)
{
}

void DesktopLayout::ReadLayout()
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


DesktopLayout::~DesktopLayout(void)
{
}
