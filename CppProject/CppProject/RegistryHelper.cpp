#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "RegistryHelper.h"
#include "RegistryError.h"




std::string RegistryHelper::GetHello()
{
	std::string var="hello";
	return var;
}

std::wstring RegistryHelper::RegGetString(HKEY hKey,const std::wstring& subKey,const std::wstring& value)
{
	std::wstring val;
	DWORD dataSize;
	LONG retCode = ::RegGetValue(hKey,subKey.c_str(),value.c_str(),RRF_RT_REG_SZ,nullptr,nullptr,&dataSize);
	if (retCode == ERROR_SUCCESS)
	{
	  std::wstring data;
	  data.resize(dataSize / sizeof(wchar_t));
	  retCode = ::RegGetValue(hKey,subKey.c_str(),value.c_str(),RRF_RT_REG_SZ,nullptr,&data[0],&dataSize);
	  if(retCode == ERROR_SUCCESS)
	  {
		  DWORD stringLengthInWchars = dataSize / sizeof(wchar_t);
		  stringLengthInWchars--; // Exclude the NUL written by the Win32 API
		  data.resize(stringLengthInWchars);
		  val=data;
	  }

	}

	return val;
}
DWORD RegistryHelper::RegGetDword(HKEY hKey,const std::wstring& subKey,const std::wstring& value)
{
	DWORD data;
	DWORD dataSize = sizeof(data);
	LONG retCode = ::RegGetValue(hKey,subKey.c_str(),value.c_str(),RRF_RT_REG_DWORD,nullptr,&data,&dataSize);
	if (retCode != ERROR_SUCCESS)
	{
		throw RegistryError::RegistryError("Cannot read DWORD from registry.", retCode);
	}
	return data;

}

RegistryHelper::RegistryHelper(void)
{
}


RegistryHelper::~RegistryHelper(void)
{
}
