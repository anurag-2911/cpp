#include "stdafx.h"
#include "RegistryError.h"


LONG m_errorCode;
RegistryError::RegistryError(const char* message, LONG errorCode): std::runtime_error(message)
	
{
	m_errorCode=errorCode;
}

LONG RegistryError::ErrorCode()
{
	return m_errorCode;
}



RegistryError::~RegistryError(void)
{

}

	

