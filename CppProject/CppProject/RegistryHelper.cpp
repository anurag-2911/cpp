#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "RegistryHelper.h"
#include "RegistryError.h"
#include <locale>
#include <codecvt>
#include <string>


std::string RegistryHelper::GetHello()
{
	std::string var="hello";
	return var;
}

std::wstring stringTowstring(const std::string& str)
{
	//using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string wstringTostring(const std::wstring& wstr)
{
    //using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}
std::string RegistryHelper::RegGetString1(HKEY hKey,const std::string& subKey,const std::string& value)
{
	std::wstring val;
	DWORD dataSize;
	std::wstring subkeyw=stringTowstring(subKey);
	
	std::wstring valuew = stringTowstring(value);
	
	LONG retCode = ::RegGetValue(hKey,subkeyw.c_str(),valuew.c_str(),RRF_RT_REG_SZ,nullptr,nullptr,&dataSize);
	if (retCode == ERROR_SUCCESS)
	{
	  std::wstring data;
	  data.resize(dataSize / sizeof(wchar_t));
	  retCode = ::RegGetValue(hKey,subkeyw.c_str(),valuew.c_str(),RRF_RT_REG_SZ,nullptr,&data[0],&dataSize);
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
