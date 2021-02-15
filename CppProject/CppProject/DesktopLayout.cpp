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
#include <KnownFolders.h>
#include "stdafx.h"
#include <shlwapi.h>
#include<atlstr.h>
#include <string>

#pragma comment(lib, "shlwapi.lib") 




void FindDesktopFolderView(REFIID riid, void **ppv);
char* GetDesktopPath();

DesktopLayout::DesktopLayout(void)
{
}

void DesktopLayout::RestoreLayout()
{
	IShellWindows *psw;
	HRESULT hr= CoInitialize(NULL);
	
	CComPtr<IFolderView> folderView;
	FindDesktopFolderView(IID_PPV_ARGS(&folderView));
	CComPtr<IShellFolder> shellFolder;
	folderView->GetFolder(IID_PPV_ARGS(&shellFolder));
	CComPtr<IEnumIDList> idList;
	folderView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&idList));
	const wchar_t* filename=L"positions.txt";
	RestorePositions(folderView,filename);
}

void DesktopLayout::ReadLayout()
{
	IShellWindows *psw;
	HRESULT hr= CoInitialize(NULL);
	
	CComPtr<IFolderView> folderView;
	FindDesktopFolderView(IID_PPV_ARGS(&folderView));
	CComPtr<IShellFolder> shellFolder;
	folderView->GetFolder(IID_PPV_ARGS(&shellFolder));
	CComPtr<IEnumIDList> idList;
	folderView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&idList));
	const wchar_t* filename=L"positions.txt";
	SavePositions(folderView,filename);
	
	for (CComHeapPtr<ITEMID_CHILD> item;idList->Next(1, &item, nullptr) == S_OK;
      item.Free()) 
	{
		STRRET str;
		shellFolder->GetDisplayNameOf(item, SHGDN_NORMAL, &str);
		LPWSTR nm=str.pOleStr;
	    std::wstring ws=nm;
		std::wstring sSearchPattern = ws + L".nal";
		/*wprintf(L"%s",sSearchPattern);
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		static char path[MAX_PATH+1];
		char* dsktopPath = GetDesktopPath();
		hFind = FindFirstFile(sSearchPattern.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) 
	   {
		  printf ("FindFirstFile failed (%d)\n", GetLastError());
		  return;
	   } 
	   else 
	   {
		  _tprintf (TEXT("The first file found is %s\n"), 
					FindFileData.cFileName);
		  FindClose(hFind);
	   }*/
		CComHeapPtr<wchar_t> spszName;
		StrRetToStr(&str, item, &spszName);
		
		POINT point;
		folderView->GetItemPosition(item, &point);
	
		wprintf(L"At %4d,%4d is %ls and %ls", point.x, point.y, spszName,nm);

		WIN32_FIND_DATA findData;
		LPCWSTR lpFileName=spszName;
		
		CString strPath;
		HRESULT hResult = E_OUTOFMEMORY;
		LPTSTR szPath = strPath.GetBufferSetLength(MAX_PATH+1);

	}


}
char* GetDesktopPath()
{
	char* path="";

	if (SHGetSpecialFolderPathA(HWND_DESKTOP, path, CSIDL_DESKTOP, FALSE))
        return path;
		else
        return "ERROR";
}

void DesktopLayout::ModifyLayout()
{
  IShellWindows *psw;
  HRESULT hr= CoInitialize(NULL);
  CComPtr<IFolderView> spView;
  FindDesktopFolderView(IID_PPV_ARGS(&spView));

  CComPtr<IEnumIDList> spEnum;
  spView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&spEnum));
  //spView->Set­Current­Folder­Flags(FWF_AUTO­ARRANGE | FWF_SNAP­TO­GRID, 0);
  
  for (CComHeapPtr<ITEMID_CHILD> spidl;
      spEnum->Next(1, &spidl, nullptr) == S_OK;
      spidl.Free()) {
  POINT pt;
  spView->GetItemPosition(spidl, &pt);
  pt.x += 150;
  pt.y += 150;

 PCITEMID_CHILD apidl[1] = { spidl };
 spView->SelectAndPositionItems(
     1, apidl, &pt, SVSI_POSITIONITEM);
 }
 
 
}
void DesktopLayout::FindDesktopFolderView(REFIID riid, void **ppv)
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

void DesktopLayout::SavePositions(IFolderView *pView, PCWSTR pszFile)
{
 CComPtr<IStream> spStream;
 SHCreateStreamOnFileEx(pszFile, STGM_CREATE | STGM_WRITE,
     FILE_ATTRIBUTE_NORMAL, TRUE, nullptr, &spStream);
 CComPtr<IEnumIDList> spEnum;
 pView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&spEnum));
 for (CComHeapPtr<ITEMID_CHILD> spidl;
      spEnum->Next(1, &spidl, nullptr) == S_OK;
      spidl.Free()) {
  IStream_WritePidl(spStream, spidl);
  POINT pt;
  pView->GetItemPosition(spidl, &pt);
  IStream_Write(spStream, &pt, sizeof(pt));
 }
}

void DesktopLayout::RestorePositions(IFolderView *pView, PCWSTR pszFile)
{
 CComPtr<IStream> spStream;
 SHCreateStreamOnFileEx(pszFile, STGM_READ,
     FILE_ATTRIBUTE_NORMAL, FALSE, nullptr, &spStream);
 POINT pt;
 for (CComHeapPtr<ITEMID_CHILD> spidl;
      SUCCEEDED(IStream_ReadPidl(spStream, &spidl)) &&
      SUCCEEDED(IStream_Read(spStream, &pt, sizeof(pt)));
      spidl.Free()) {
  PCITEMID_CHILD apidl[1] = { spidl };
  pView->SelectAndPositionItems(1, apidl, &pt, SVSI_POSITIONITEM);
 }
}


DesktopLayout::~DesktopLayout(void)
{
}
