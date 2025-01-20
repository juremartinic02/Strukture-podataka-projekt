// parameter_validation.h
#ifndef PARAMETER_VALIDATION_H
#define PARAMETER_VALIDATION_H

#include "functions.h"
#include "client_management.h"

int isValidName(const char* name) {
    if (strlen(name) > MAX_NAME_LENGTH - 1 || strlen(name) == 0) return 0;

    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0; // Allow spaces in names
    }
    return 1;
}


int isValidUserCredential(const char* str) {
    // Check if the string is empty or exceeds the maximum allowed length
    if (strlen(str) > MAX_NAME_LENGTH - 1 || strlen(str) == 0) return 0;

    // Return false if a character is not alphabetical character or digit
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i])) return 0;
    }
    return 1;
}

// buffer - a pointer to the character array
// size - maximum size of the buffer
int getValidatedInput(char* buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return 0; // Input failed
    }

    // Remove trailing newline character if it exists
    char* newlinePos = strchr(buffer, '\n');
    if (newlinePos) {
        *newlinePos = '\0';
    }
    else {
        clearInputBuffer(); // Clear excess characters
        printf("Input too long. Try again.\n");
        return 0;
    }

    // Ensure input is not empty
    if (strlen(buffer) == 0) {
        printf("Input cannot be empty. Try again.\n");
        return 0;
    }

    return 1; // Input successful
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void GetCurrentTime(char* buffer, size_t bufferSize) {
    // Get the current time in seconds
    time_t t = time(NULL);

    // Convert the time to a structure representing local time
    struct tm* tm_info = localtime(&t);

    // Format the time as "YYYY-MM-DD HH:MM:SS" and store it in the buffer
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", tm_info);
}

Client* FindClientByUsername(const char* username, const char* password) {
    Client* temp = head; // Initialize a temporary pointer to iterate through the list of clients

    char hashedInput[MAX_NAME_LENGTH];
    if (password) {
        hashPassword(password, hashedInput); // Hash the input password if provided
    }

    while (temp) {
        //Check if the current client's username matches the input username and if the password matches
        if (strcmp(temp->username, username) == 0 &&
            (!password || strcmp(temp->password, hashedInput) == 0)) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

#endif