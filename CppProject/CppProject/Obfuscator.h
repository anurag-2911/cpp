#pragma once
class Obfuscator
{
public:
	Obfuscator(void);
	~Obfuscator(void);
	char* Obfuscator::obfuscate( char* data, size_t length );
	void Obfuscator::obfuscate(char* input);
};

