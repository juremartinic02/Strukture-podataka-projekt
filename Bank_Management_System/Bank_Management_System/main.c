#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<crtdbg.h>

#define MAX_NAME_LENGTH 32

struct account;
typedef struct account* AccountPosition;
typedef struct account {
	int personID;
	int balance;
} Account;

struct client;
typedef struct client* ClientPosition;
typedef struct client {
	int ID;
	char firstName[MAX_NAME_LENGTH];
	char lastName[MAX_NAME_LENGTH];
} Client;

int main()
{

	// Provjera da li smo uspjesno oslobodili svu alociranu memoriju u programu
	_CrtDumpMemoryLeaks();
	return 0;
}