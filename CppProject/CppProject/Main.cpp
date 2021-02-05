#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "RegistryHelper.h"


std::string SayHello();


int _tmain(int argc, _TCHAR* argv[])
{
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





