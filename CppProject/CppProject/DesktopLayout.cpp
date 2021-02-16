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
#include <shlwapi.h>
#include<atlstr.h>
#include <string>
#include <tchar.h>
#include <locale>
#include <codecvt>

#define BUFFER 8192

#pragma comment(lib, "shlwapi.lib") 
#define DESKTOP_LINK_POSITION_KEY_NAME _T("Software\\Netware\\Nal\\1.0\\Links\\*POSITIONS")



void FindDesktopFolderView(REFIID riid, void **ppv);



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

	/*const wchar_t* filename=L"positions.txt";
	SavePositions(folderView,filename);*/
	
	for (CComHeapPtr<ITEMID_CHILD> item;idList->Next(1, &item, nullptr) == S_OK;
      item.Free()) 
	{
		STRRET str;
		STRRET filenamewithExtn;
		shellFolder->GetDisplayNameOf(item, SHGDN_NORMAL, &str);
		shellFolder->GetDisplayNameOf(item,SHGDN_FORPARSING,&filenamewithExtn);
		LPWSTR nameWithExtn=filenamewithExtn.pOleStr;
	    std::wstring fname=nameWithExtn;
		int index = fname.find(L".nal"); 

		
		CComHeapPtr<wchar_t> spszName;
		StrRetToStr(&str, item, &spszName);
		
		POINT point;
		folderView->GetItemPosition(item, &point);
	
		wprintf(L"At %4d,%4d is %ls \n", point.x, point.y, spszName);
		if(index!=-1)
		{
			HKEY hKey;
			
			if(RegOpenKeyEx(HKEY_CURRENT_USER, DESKTOP_LINK_POSITION_KEY_NAME, 0,
			KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
			{
				
				if(point.x > 0 && point.y > 0)
				{
					CString strPointPair;
					strPointPair.Format(_T("%d,%d"), point.x, point.y);
					
					RegSetValueEx(hKey, spszName, 0, REG_SZ,
						(LPBYTE)strPointPair.GetBuffer(strPointPair.GetLength()), 
						strPointPair.GetLength()*sizeof(TCHAR));
				}
			}
		}
		
	}
	
}


void DesktopLayout::ModifyLayout()
{
  IShellWindows *psw;
  HRESULT hr= CoInitialize(NULL);
  CComPtr<IFolderView> folderView;
  FindDesktopFolderView(IID_PPV_ARGS(&folderView));
  	CComPtr<IShellFolder> shellFolder;
	folderView->GetFolder(IID_PPV_ARGS(&shellFolder));

  CComPtr<IEnumIDList> spEnum;
  folderView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&spEnum));
   
  for (CComHeapPtr<ITEMID_CHILD> item;
      spEnum->Next(1, &item, nullptr) == S_OK;
      item.Free()) {
		STRRET str;
		STRRET filenamewithExtn;
		shellFolder->GetDisplayNameOf(item, SHGDN_NORMAL, &str);
		shellFolder->GetDisplayNameOf(item,SHGDN_FORPARSING,&filenamewithExtn);
		LPWSTR nameWithExtn=filenamewithExtn.pOleStr;
	    std::wstring fname=nameWithExtn;
		int index = fname.find(L".nal"); 
		POINT pt;
		if(index!=-1)
		{
			folderView->GetItemPosition(item, &pt);
			std::string pnt=RegGetString(HKEY_CURRENT_USER,wstringTostring(DESKTOP_LINK_POSITION_KEY_NAME),wstringTostring(str.pOleStr));
			std::string xcord=substrbeforecomma(pnt);
			std::string ycord=pnt.substr(pnt.find(",") + 1); 
			long xcrd=atol(xcord.c_str());
			long ycrd=atol(ycord.c_str());
			pt.x = xcrd;
			pt.y = ycrd;

			PCITEMID_CHILD apidl[1] = { item };
			folderView->SelectAndPositionItems(
				1, apidl, &pt, SVSI_POSITIONITEM);
		}
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

std::string DesktopLayout::RegGetString(HKEY hKey,const std::string& subKey,const std::string& value)
{
	std::wstring val;
	DWORD dataSize;
	std::wstring subkeyw=stringTowstring(subKey);
	
	std::wstring valuew = stringTowstring(value);
	
	LONG retCode = ::RegGetValueW(hKey,subkeyw.c_str(),valuew.c_str(),RRF_RT_REG_SZ,nullptr,nullptr,&dataSize);
	if (retCode == ERROR_SUCCESS)
	{
	  std::wstring data;
	  data.resize(dataSize / sizeof(wchar_t));
	  retCode = ::RegGetValueW(hKey,subkeyw.c_str(),valuew.c_str(),RRF_RT_REG_SZ,nullptr,&data[0],&dataSize);
	  if(retCode == ERROR_SUCCESS)
	  {
		  DWORD stringLengthInWchars = dataSize / sizeof(wchar_t);
		  stringLengthInWchars--; // Exclude the NUL written by the Win32 API
		  data.resize(stringLengthInWchars);
		  val=data;
	  }

	}

	return wstringTostring(val);
}
std::wstring DesktopLayout::stringTowstring(const std::string& str)
{
	//using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string DesktopLayout::wstringTostring(const std::wstring& wstr)
{
    //using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}
std::string DesktopLayout::substrbeforecomma(std::string const& s)
{
    std::string::size_type pos = s.find(',');
    if (pos != std::string::npos)
    {
        return s.substr(0, pos);
    }
    else
    {
        return s;
    }
}
DesktopLayout::~DesktopLayout(void)
{
}
