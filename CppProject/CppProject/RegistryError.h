#include <iostream>
#include "stdafx.h"
#include <windows.h>
#pragma once
class RegistryError: public std::runtime_error
{
public:
	RegistryError(const char* message, LONG errorCode);
	~RegistryError(void);
	LONG ErrorCode();
	
private:
  
};

