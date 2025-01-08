#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<crtdbg.h> // library koji nam omogucuje provjeru oslobadjanja alocirane memorije

#define MAX_NAME_LENGTH 32

// Definicija strukture klijent
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

/*
    Function Menu();
    -------------------------
        Displays the main menu of the banking management system.
        Function loops until the user chooses to exit the program.
        Function validates user input to ensure valid choices so it can perform functions that are dedicated to each number 
*/
void Menu();

/*
    Function isValidName(const char* name);
    ------------------------------
        Validates a given name string based on criteria:
            1. The length of the name must be between 0 and MAX_NAME_LENGTH
            2. The name can only contain alphabetic characters and spaces
        Returns 1 if true (name is valid) and 0 if false (name is invalid)
*/
int isValidName(const char* name);

/*
    Function isValidUserCredential(const char* str);
    -------------------------------------
        Validates a user credential based on criteria:
            1. The length of the name must be between 0 and MAX_NAME_LENGTH
            2. The name can only contain alphabetic characters and digits
        Returns 1 if true (name is valid) and 0 if false (name is invalid)
*/
int isValidUserCredential(const char* str);

/*
    Function getValidatedInput(char* buffer, size_t size);
    -------------------------------------
        Reads user input from the standard input (stdin) and validates it.
        Ensures the input fits within the provided buffer size and removes the newline character.
*/
int getValidatedInput(char* buffer, size_t size);

/*
    Function generateUniqueID();
    -------------------------------------
        Generates a unique random ID for a new client.
*/
int generateUniqueID();

/*
    Function clearInputBuffer();
    -------------------------------------
        Clears the input buffer.
*/
void clearInputBuffer();

/*
    Function CreateAccount();
    ----------------------------
        Handles the creation of a new client account. 
        Validates all user inputs such as name, username, password, and initial balance. 
        Ensures uniqueness of IDs and usernames, hashes passwords, and saves the account data to the linked list and file.
*/
void CreateAccount();

/*
    Function SaveClientsToFile();
    ----------------------------------
        Saves all client data from the linked list into a file named "clients.txt".
        If the file is empty, headers are written first.
*/
void SaveClientsToFile();

/*
    Function FreeClientList();
    --------------------------------------
        Free allocated memory
*/
void FreeClientList();

/*
    Function LoadClientsFromFile();
    ---------------------------------------
        Loads client data from the file "clients.txt" into the linked list.
        Clears the existing client list before loading new data.
*/
void LoadClientsFromFile();

/*
    Function PrintClients();
    --------------------------------
        Prints all client data stored in the linked list to the console.
*/
void PrintClients();

/*
    Function GetCurrentTime(char* buffer, size_t bufferSize);
    ----------------------------------------------------------------
        Retrieves the current system time and formats it as a string.
        The formatted time is stored in the provided buffer in the argument of the function.
*/
void GetCurrentTime(char* buffer, size_t bufferSize);

/*
    Function SaveClientsToFileAfterDeletion();
    -----------------------------------------------
        Writes the current list of clients to the "clients.txt" file after deletion operations.
        The file is overwritten with the updated list of clients.
*/
void SaveClientsToFileAfterDeletion();

/*
    Function DeleteAccount();
    ------------------------------
        This function deletes a client account from the linked list if the provided
        username and password match an existing account. 
        The updated client list is saved to the file after deletion.
*/
void DeleteAccount();

/*
    Function UpdateAccount();
    ------------------------------
        This function allows a client to update their account details such as 
        first name, last name, or password. 
        It ensures proper validation and saves the updated information to the file.
*/
void UpdateAccount();

/*
    Function Transactions();
    ---------------------------------
    Allows the user to perform various financial transactions after logging into their account.
*/
void Transactions();

/*
    Function Client* FindClientByUsername(const char* username, const char* password);
    --------------------------------------------------------------------------------------
        Find a client by username and password in a linked list of clients
*/
Client* FindClientByUsername(const char* username, const char* password);

/*
    Function SaveTransactionLog(const char* username, const char* action, int amount, const char* otherUsername);
    ------------------------------------------------------------------------------------------------------------------
        Function appends a new transaction log entry to a file named "transactions.txt".
        It records the username, action performed, transaction amount, and timestamp. 
*/
void SaveTransactionLog(const char* username, const char* action, int amount, const char* otherUsername);

/*
    Function PrintTransactionLog();
    ------------------------------------
        Prints the transaction log for a specific user
*/
void PrintTransactionLog();

/*
    Function SortAndPrintClients();
    ------------------------------------
    Function allows the user to choose a sorting parameter (balance, name, last name, or username)
    and the order (ascending or descending). 
    It then sorts the list of clients accordingly and prints the sorted list.
*/
void SortAndPrintClients();

/*
    Function Client* CopyClientList();
    --------------------------------------
        Function iterates through the original list of clients, dynamically allocates memory for each client, 
        and copies the client data to a new list.
*/
Client* CopyClientList();

/*
    Function SortClientsByBalance(Client** headRef, int descending);
    ---------------------------------------------------------------------
        Function sorts the list using selection sort, finding the client with the 
        maximum (or minimum) balance in each iteration and moving it to the sorted portion of the list.
*/
void SortClientsByBalance(Client** headRef, int descending);

/*
    Function SortClientsByStringField(Client** headRef, size_t offset, int descending);
    ---------------------------------------------------------------------------------------
        Function uses selection sort to find the node with the lexicographically maximum (or minimum) string 
        value in each iteration and moves it to the sorted portion of the list.
*/
void SortClientsByStringField(Client** headRef, size_t offset, int descending);

/*
    Function PrintClientList(Client* list);
    --------------------------------------------
        Function checks if the list is empty, and if not, it prints the client details such as ID, first name, 
        last name, username, balance, and creation time in a form of a table.
*/
void PrintClientList(Client* list);

/*
    Function hashPassword(const char* password, char* hashedPassword);
    ----------------------------------------------------------------------
        This function takes a password, applies the DJB2 hash function to it, 
        and stores the resulting hash as a string in the provided output buffer.
*/
void hashPassword(const char* password, char* hashedPassword);

#endif