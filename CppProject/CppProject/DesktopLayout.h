#include <shlobj.h>
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
};

