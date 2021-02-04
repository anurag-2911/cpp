// CppProject.cpp :Simple cpp functions
//

#include "stdafx.h"
#include <iostream>

std::string SayHello();

int _tmain(int argc, _TCHAR* argv[])
{
	std:: string val = SayHello();
	return 0;
}

std::string SayHello(){
	std::string var ="hello world";
	std::cout << "hello world";

	return var;
}

