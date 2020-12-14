// HelloWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
void UserInput();
void SizeOfVariables();
void PointerExample();
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "hello world! ";
	UserInput();
	SizeOfVariables();
	PointerExample();
	return 0;
}
void PointerExample()
{
   int  var = 20;   // actual variable declaration.
   int  *ip;        // pointer variable 

   ip = &var;       // store address of var in pointer variable

   cout << "Value of var variable: ";
   cout << var << endl;

   // print the address stored in ip pointer variable
   cout << "Address stored in ip variable: ";
   cout << ip << endl;

   // access the value at the address available in pointer
   cout << "Value of *ip variable: ";
   cout << *ip << endl;

}
void UserInput()
{
	int number;
	std::cout<< "enter a number ";
	std::cin>> number;
	std::cout<<"you entered"<<number;
	
}
void SizeOfVariables()
{
   cout << "Size of char : " << sizeof(char) << endl;
   cout << "Size of int : " << sizeof(int) << endl;
   cout << "Size of short int : " << sizeof(short int) << endl;
   cout << "Size of long int : " << sizeof(long int) << endl;
   cout << "Size of float : " << sizeof(float) << endl;
   cout << "Size of double : " << sizeof(double) << endl;
   cout << "Size of wchar_t : " << sizeof(wchar_t) << endl;
}

