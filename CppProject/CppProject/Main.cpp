#include "stdafx.h"
#include <iostream>
#include <windows.h>

std::string SayHello();
std::wstring RegGetString(HKEY hKey,const std::wstring& subKey,const std::wstring& value);

int _tmain(int argc, _TCHAR* argv[])
{
	std:: string val = SayHello();
	std::wstring s = RegGetString(HKEY_CURRENT_USER, L"Software\\Microfocus\\zapp", L"hashKey");
	return 0;
}

std::string SayHello(){
	std::string var ="hello world";
	std::cout << "hello world";	
	return var;
}

std::wstring RegGetString(HKEY hKey,const std::wstring& subKey,const std::wstring& value)
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



