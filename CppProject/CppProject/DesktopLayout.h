#include <shlobj.h>
#include<iostream>
#pragma once
class DesktopLayout
{
public:
	DesktopLayout(void);
	~DesktopLayout(void);
	void DesktopLayout::ReadLayout();
	void DesktopLayout::ModifyLayout();
	void SavePositions(IFolderView *pView, PCWSTR pszFile);
	void RestorePositions(IFolderView *pView, PCWSTR pszFile);
	void FindDesktopFolderView(REFIID riid, void **ppv);
	void DesktopLayout::RestoreLayout();
	std::string DesktopLayout::wstringTostring(const std::wstring& wstr);
	std::wstring DesktopLayout::stringTowstring(const std::string& str);
	std::string DesktopLayout::RegGetString(HKEY hKey,const std::string& subKey,const std::string& value);
	std::string substrbeforecomma(std::string const& s);
	
};

