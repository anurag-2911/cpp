#pragma once

class RegistryHelper
{
public:
	RegistryHelper(void);
	~RegistryHelper(void);
	
	std::string RegistryHelper::GetHello();
	std::wstring RegistryHelper::RegGetString(HKEY hKey,const std::wstring& subKey,const std::wstring& value);
	DWORD RegistryHelper::RegGetDword(HKEY hKey,const std::wstring& subKey,const std::wstring& value);
	std::string RegGetString1(HKEY hKey,const std::string& subKey,const std::string& value);
};

