#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<crtdbg.h>

#define MAX_NAME_LENGTH 32

// Struktura klijenta
typedef struct client {
    int ID;
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    int balance;
    char creationTime[20];
    struct client* next;
} Client;

Client* head = NULL;

void Menu();
int isValidName(const char* name);
int isValidUserCredential(const char* str);
int getValidatedInput(char* buffer, size_t size);
int generateUniqueID();
void clearInputBuffer();
void CreateAccount();
void SaveClientsToFile();
void FreeClientList();
void LoadClientsFromFile();
void PrintClients();
void GetCurrentTime(char* buffer, size_t bufferSize);
void SaveClientsToFileAfterDeletion();
void DeleteAccount();
void UpdateAccount();
void Transactions();
Client* FindClientByUsername(const char* username, const char* password);
void SaveTransactionLog(const char* username, const char* action, int amount, const char* otherUsername);
void PrintTransactionLog();
void SortAndPrintClients();
Client* CopyClientList();
void SortClientsByBalance(Client** headRef, int descending);
void SortClientsByStringField(Client** headRef, size_t offset, int descending);
void PrintClientList(Client* list);

#endif