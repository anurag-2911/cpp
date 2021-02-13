#include "stdafx.h"
#include "Obfuscator.h"

int key[] = { 1, 85, 7, 105, 7, 9, 47, 9, 67, 9, 6, 73, 6, 3, 5, 164, 98, 6, 13, 0, 2 };
int keyLength = 21;


char* Obfuscator::obfuscate( char* data, size_t length )
{
	int keyIndex = 0;

	//XOR input string with the key
	for(int i=0; i<length; i++ )
	{
		data[i] = data[i] ^ key[keyIndex++];
		if(keyIndex >= keyLength)
			keyIndex = 0;
	}
	return data;
}

void Obfuscator::obfuscate(char* input)
{
	
	char* result=obfuscate( input, keyLength );
	
}


Obfuscator::Obfuscator(void)
{
}


Obfuscator::~Obfuscator(void)
{
}
