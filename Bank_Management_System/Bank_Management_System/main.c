#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<crtdbg.h>

int main()
{

	// Provjera da li smo uspjesno oslobodili svu alociranu memoriju u programu
	_CrtDumpMemoryLeaks();
	return 0;
}