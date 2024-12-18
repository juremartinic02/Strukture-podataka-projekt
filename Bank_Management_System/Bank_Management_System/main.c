#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<crtdbg.h>

struct bank_account;
typedef struct bank_account* Bank_Account_Position;
typedef struct bank_account {
	int personID;
	int balance;
} bank_account;

int main()
{

	// Provjera da li smo uspjesno oslobodili svu alociranu memoriju u programu
	_CrtDumpMemoryLeaks();
	return 0;
}