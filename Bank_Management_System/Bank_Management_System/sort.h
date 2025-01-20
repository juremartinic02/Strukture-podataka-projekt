#ifndef SORT_H
#define SORT_H

#include "functions.h"
#include "client_management.h"
#include "parameter_validation.h"

void SortAndPrintClients() {
    int choice, order; // Variables to store the user's sorting parameter and order choice
    Client* sortedList = NULL;

    // Prompt user for sorting parameter
    printf("\nChoose sorting parameter:\n");
    printf("1. Sort by Balance\n");
    printf("2. Sort by Name\n");
    printf("3. Sort by Last Name\n");
    printf("4. Sort by Username\n");
    printf("Enter your choice: ");
    (void)scanf("%d", &choice);
    clearInputBuffer();

    // Check if the input choice is valid
    if (choice < 1 || choice > 4) {
        printf("Invalid choice! Returning to main menu.\n");
        return;
    }

    // Prompt user for sorting order
    if (choice == 1) { // Balance
        printf("\nChoose sorting order:\n");
        printf("1. Ascending (Low to High)\n");
        printf("2. Descending (High to Low)\n");
    }
    else { // Name, Last Name, Username
        printf("\nChoose sorting order:\n");
        printf("1. Alphabetical (A-Z)\n");
        printf("2. Reverse Alphabetical (Z-A)\n");
    }

    printf("Enter your choice: ");
    (void)scanf("%d", &order);
    clearInputBuffer();

    if (order < 1 || order > 2) {
        printf("Invalid choice! Returning to main menu.\n");
        return;
    }

    // Create a copy of the client list to sort (so original list is not modified)
    sortedList = CopyClientList();

    // Sort the list based on the user's choice
    switch (choice) {
    case 1: // Balance
        SortClientsByBalance(&sortedList, order == 2);
        break;
    case 2: // Name
        SortClientsByStringField(&sortedList, offsetof(Client, firstName), order == 2);
        break;
    case 3: // Last Name
        SortClientsByStringField(&sortedList, offsetof(Client, lastName), order == 2);
        break;
    case 4: // Username
        SortClientsByStringField(&sortedList, offsetof(Client, username), order == 2);
        break;
    }

    // Print the sorted list
    PrintClientList(sortedList);

    // Free the sorted list
    FreeClientList(sortedList);
}

Client* CopyClientList() {
    Client* copyHead = NULL, * current = head, * newClient, * last = NULL;

    // Iterate through the original client list
    while (current) {
        newClient = (Client*)malloc(sizeof(Client));
        if (!newClient) {
            perror("Memory allocation failed");
            FreeClientList(copyHead);
            return NULL;
        }

        *newClient = *current; // Copy the data from the current client to the new client
        newClient->next = NULL; // Set the next pointer of the new client to NULL

        // If this is the first client in the new list, set it as the head
        if (!copyHead) {
            copyHead = newClient;
        }
        else {
            // Otherwise, link the new client to the last client in the new list
            last->next = newClient;
        }
        last = newClient;
        current = current->next;
    }
    return copyHead; // Return the head of the newly created list
}

void SortClientsByBalance(Client** headRef, int descending) {
    // Return if the list is empty or has only one element
    if (!*headRef || !(*headRef)->next) return;

    Client* sorted = NULL, * current = *headRef, * prev, * maxPrev, * maxNode;

    while (current) {
        maxPrev = NULL;
        maxNode = current;
        prev = current;

        // Find the node with the max/min balance
        while (prev->next) {
            if ((descending && prev->next->balance < maxNode->balance) ||
                (!descending && prev->next->balance > maxNode->balance)) {
                maxPrev = prev;
                maxNode = prev->next;
            }
            prev = prev->next;
        }

        // Remove the max/min node from the original list
        if (maxPrev) {
            maxPrev->next = maxNode->next; // Link the previous node to the next node of maxNode
        }
        else {
            current = maxNode->next; // Move the current pointer if the maxNode is the first node
        }

        // Add the max/min node to the sorted list
        maxNode->next = sorted;
        sorted = maxNode;
    }
    *headRef = sorted;
}

void SortClientsByStringField(Client** headRef, size_t offset, int descending) {
    if (!*headRef || !(*headRef)->next) return;

    Client* sorted = NULL, * current = *headRef, * prev, * maxPrev, * maxNode;

    while (current) {
        maxPrev = NULL;
        maxNode = current;
        prev = current;

        // Find the node with the maximum or minimum string value based on the sorting order
        while (prev->next) {
            // Get the string field of the current and next nodes using the provided offset
            const char* field1 = (const char*)((char*)maxNode + offset);
            const char* field2 = (const char*)((char*)prev->next + offset);

            // Compare the two strings and determine if we need to update maxPrev and maxNode
            if ((descending && strcmp(field2, field1) < 0) ||
                (!descending && strcmp(field2, field1) > 0)) {
                maxPrev = prev;
                maxNode = prev->next;
            }
            prev = prev->next;
        }

        // Remove the max/min node from the original list
        if (maxPrev) {
            maxPrev->next = maxNode->next;
        }
        else {
            current = maxNode->next;
        }

        // Add the max/min node to the sorted list
        maxNode->next = sorted;
        sorted = maxNode; // Update the sorted list to include the max/min node at the front
    }
    *headRef = sorted; // Set the head of the list to the sorted list
}

#endif