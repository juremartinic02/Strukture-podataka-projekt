// client_management.h
#ifndef CLIENT_MANAGEMENT_H
#define CLIENT_MANAGEMENT_H

#include "functions.h"
#include "parameter_validation.h"

void CreateAccount() {
    Client* newClient = NULL;
    newClient = (Client*)malloc(sizeof(Client));
    if (!newClient) {
        perror("Memory allocation failed");
        return;
    }

    char buffer[MAX_NAME_LENGTH]; // Buffer for input validation
    char passwordConfirm[MAX_NAME_LENGTH]; // Buffer for password confirmation

    // Generate random unique ID
    do {
        newClient->ID = rand() % 10000; // Generate a larger range of random IDs
        Client* current = head;
        int idExists = 0;
        while (current != NULL) {
            if (current->ID == newClient->ID) { // Check if there is already account with that number
                idExists = 1;
                break;
            }
            current = current->next;
        }
        if (!idExists) break;
    } while (1);

    printf("\nGenerated ID for new client: %d\n", newClient->ID);

    // First Name validation
    do {
        printf("Enter first name (letters only, max %d characters): ", MAX_NAME_LENGTH - 1);
        if (getValidatedInput(buffer, sizeof(buffer)) && isValidName(buffer)) {
            strcpy(newClient->firstName, buffer); // Store validated first name
            break;
        }
        printf("Invalid first name or input too long! Use only letters.\n");
    } while (1);

    // Last Name validation
    do {
        printf("Enter last name (letters only, max %d characters): ", MAX_NAME_LENGTH - 1);
        if (getValidatedInput(buffer, sizeof(buffer)) && isValidName(buffer)) {
            strcpy(newClient->lastName, buffer); // Store validated last name
            break;
        }
        printf("Invalid last name or input too long! Use only letters.\n");
    } while (1);

    // Username validation
    do {
        printf("Enter username (letters and numbers only, max %d characters): ", MAX_NAME_LENGTH - 1);
        if (getValidatedInput(buffer, sizeof(buffer)) && isValidUserCredential(buffer)) {
            // Check if username already exists
            Client* current = head;
            int usernameExists = 0;
            while (current != NULL) {
                if (strcmp(current->username, buffer) == 0) {
                    usernameExists = 1;
                    break;
                }
                current = current->next;
            }
            if (!usernameExists) {
                strcpy(newClient->username, buffer);
                break;
            }
            printf("Username already exists! Please choose another.\n");
        }
        else {
            printf("Invalid username or input too long! Use only letters and numbers.\n");
        }
    } while (1);

    // Password validation
    do {
        printf("Enter password (letters and numbers only, max %d characters): ", MAX_NAME_LENGTH - 1);
        if (getValidatedInput(buffer, sizeof(buffer)) && isValidUserCredential(buffer)) {
            printf("Confirm password: ");
            if (getValidatedInput(passwordConfirm, sizeof(passwordConfirm)) && strcmp(buffer, passwordConfirm) == 0) {
                hashPassword(buffer, newClient->password); // Hash and store the password
                break;
            }
            printf("Passwords do not match! Please try again.\n");
        }
        else {
            printf("Invalid password or input too long! Use only letters and numbers.\n");
        }
    } while (1);

    // Balance validation (user cannot enter a negative number)
    do {
        printf("Enter initial balance: ");
        if (scanf("%d", &newClient->balance) == 1 && newClient->balance >= 0) {
            clearInputBuffer();
            break;
        }
        printf("Invalid balance! Please enter a non-negative number.\n");
        clearInputBuffer();
    } while (1);

    // Set creation time and add to linked list
    GetCurrentTime(newClient->creationTime, sizeof(newClient->creationTime));
    newClient->next = head; // Insert new client at the head of the linked list
    head = newClient;

    printf("Account successfully created!\n");

    // Save all clients to the clients.txt file
    SaveClientsToFile();
}

void DeleteAccount() {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char confirmation[3]; // Allow for null terminator

    // Prompt for username and password
    do {
        printf("Enter username of the account to delete: ");
        if (getValidatedInput(username, sizeof(username)) && strlen(username) > 0) {
            break;
        }
        printf("Username cannot be empty. Try again.\n");
    } while (1);

    do {
        printf("Enter password for the account: ");
        if (getValidatedInput(password, sizeof(password)) && strlen(password) > 0) {
            break;
        }
        printf("Password cannot be empty. Try again.\n");
    } while (1);

    // Hash the input password for comparison
    char hashedInput[MAX_NAME_LENGTH];
    hashPassword(password, hashedInput);

    Client* temp = head; // Pointer to traverse the list
    Client* prev = NULL; // Pointer to keep track of the previous node

    // Search for the matching account
    while (temp != NULL) {
        // Check if username and hashed password match
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, hashedInput) == 0) {
            // Prompt for confirmation before deletion
            do {
                printf("Account found! Are you sure you want to delete this account? (y/n): ");
                getValidatedInput(confirmation, sizeof(confirmation));
                if (strcmp(confirmation, "y") == 0 || strcmp(confirmation, "n") == 0) {
                    break;
                }
                printf("Invalid input! Please enter 'y' or 'n'.\n");
            } while (1);

            if (strcmp(confirmation, "y") == 0) {
                // Remove the account from the linked list
                if (prev == NULL) {
                    head = temp->next; // Update head if deleting the first node
                }
                else {
                    prev->next = temp->next; // Bypass the node to delete
                }

                free(temp); // Free memory for the deleted client
                SaveClientsToFileAfterDeletion(); // Save updated list to file
                printf("Account deleted successfully.\n");
                return;
            }
            else {
                printf("Account deletion canceled.\n");
                return;
            }
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Account not found or credentials incorrect.\n");
}

void UpdateAccount() {
    char username[MAX_NAME_LENGTH], password[MAX_NAME_LENGTH];
    Client* temp = head;

    // Prompt user for username and password
    do {
        printf("Enter username: ");
        if (getValidatedInput(username, sizeof(username)) && strlen(username) > 0) {
            break;
        }
        printf("Username cannot be empty. Try again.\n");
    } while (1);

    do {
        printf("Enter your password: ");
        if (getValidatedInput(password, sizeof(password)) && strlen(password) > 0) {
            break;
        }
        printf("Password cannot be empty. Try again.\n");
    } while (1);

    temp = FindClientByUsername(username, password);
    if (!temp) {
        printf("Account not found or incorrect credentials.\n");
        return;
    }

    printf("Account found!\n");

    // Update first name
    char choice[3];
    do {
        printf("Current first name: %s\nDo you want to update it? (y/n): ", temp->firstName);
        getValidatedInput(choice, sizeof(choice));
        if (strcmp(choice, "y") == 0 || strcmp(choice, "n") == 0) {
            break;
        }
        printf("Invalid input! Please enter 'y' or 'n'.\n");
    } while (1);

    if (strcmp(choice, "y") == 0) {
        char newFirstName[MAX_NAME_LENGTH];
        do {
            printf("Enter new first name: ");
            if (getValidatedInput(newFirstName, sizeof(newFirstName)) && isValidName(newFirstName)) {
                strcpy(temp->firstName, newFirstName);
                printf("First name updated successfully.\n");
                break;
            }
            printf("Invalid first name. Try again.\n");
        } while (1);
    }

    // Update last name
    do {
        printf("Current last name: %s\nDo you want to update it? (y/n): ", temp->lastName);
        getValidatedInput(choice, sizeof(choice));
        if (strcmp(choice, "y") == 0 || strcmp(choice, "n") == 0) {
            break;
        }
        printf("Invalid input! Please enter 'y' or 'n'.\n");
    } while (1);

    if (strcmp(choice, "y") == 0) {
        char newLastName[MAX_NAME_LENGTH];
        do {
            printf("Enter new last name: ");
            if (getValidatedInput(newLastName, sizeof(newLastName)) && isValidName(newLastName)) {
                strcpy(temp->lastName, newLastName);
                printf("Last name updated successfully.\n");
                break;
            }
            printf("Invalid last name. Try again.\n");
        } while (1);
    }

    // Update password
    do {
        printf("Do you want to update the password? (y/n): ");
        getValidatedInput(choice, sizeof(choice));
        if (strcmp(choice, "y") == 0 || strcmp(choice, "n") == 0) {
            break;
        }
        printf("Invalid input! Please enter 'y' or 'n'.\n");
    } while (1);

    if (strcmp(choice, "y") == 0) {
        char newPassword[MAX_NAME_LENGTH], passwordConfirm[MAX_NAME_LENGTH];
        do {
            printf("Enter new password: ");
            if (getValidatedInput(newPassword, sizeof(newPassword)) && isValidUserCredential(newPassword)) {
                printf("Confirm new password: ");
                getValidatedInput(passwordConfirm, sizeof(passwordConfirm));
                if (strcmp(newPassword, passwordConfirm) == 0) {
                    hashPassword(newPassword, temp->password);
                    printf("Password updated successfully.\n");

                    // Save the updated client data immediately
                    SaveClientsToFile();
                    printf("Changes saved to file.\n");
                    break;
                }
                else {
                    printf("Passwords do not match. Try again.\n");
                }
            }
            else {
                printf("Invalid password. Try again.\n");
            }
        } while (1);
    }

    // Save any remaining changes to the file
    SaveClientsToFile();
    printf("Account updated successfully.\n");
}

void Transactions() {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    Client* user;

    do {
        printf("Enter username: ");
        if (getValidatedInput(username, sizeof(username)) && strlen(username) > 0) {
            break;
        }
        printf("Username cannot be empty. Try again.\n");
    } while (1);


    do {
        printf("Enter your password: ");
        if (getValidatedInput(password, sizeof(password)) && strlen(password) > 0) {
            break;
        }
        printf("Password cannot be empty. Try again.\n");
    } while (1);

    user = FindClientByUsername(username, password);
    if (!user) {
        printf("Invalid username or password!\n");
        return;
    }

    // Variables for user choice, transaction amounts, and target user (for transfers)
    int choice, amount;
    char targetUsername[MAX_NAME_LENGTH];
    Client* targetUser;

    do {
        printf("\n1. Deposit money\n");
        printf("2. Withdraw money\n");
        printf("3. Transfer money\n");
        printf("4. Exit transactions\n");
        printf("Choose an option: ");
        (void)scanf("%d", &choice);
        clearInputBuffer();

        // Handle the user's menu choice
        switch (choice) {
        case 1: // Deposit money
            printf("Your current balance is: %d\n", user->balance);
            printf("Enter amount to deposit: ");
            (void)scanf("%d", &amount);
            clearInputBuffer();

            // Ensure the deposit amount is valid
            if (amount <= 0) {
                printf("Invalid amount!\n");
            }
            else {
                // Update the balance and log the transaction
                user->balance += amount;
                SaveTransactionLog(user->username, "Deposit", amount, NULL);
                SaveClientsToFileAfterDeletion();
                printf("Deposit successful! New balance: %d\n", user->balance);
            }
            break;

        case 2: // Withdraw money
            printf("Your current balance is: %d\n", user->balance);
            printf("Enter amount to withdraw: ");
            (void)scanf("%d", &amount);
            clearInputBuffer();

            // Check if the withdrawal amount is valid and does not exceed balance
            if (amount <= 0 || amount > user->balance) {
                printf("Invalid amount!\n");
            }
            else {
                // Update the balance and log the transaction
                user->balance -= amount;
                SaveTransactionLog(user->username, "Withdraw", amount, NULL);
                SaveClientsToFileAfterDeletion();
                printf("Withdrawal successful! New balance: %d\n", user->balance);
            }
            break;

        case 3: // Transfer money
            // Get the username of the transfer recipient
            printf("Enter the username of the recipient: ");
            getValidatedInput(targetUsername, sizeof(targetUsername));

            // Check if the user is trying to transfer to themselves
            if (strcmp(username, targetUsername) == 0) {
                printf("You cannot transfer money to your own account!\n");
                break;
            }

            // Find the recipient client
            targetUser = FindClientByUsername(targetUsername, NULL);

            // Check if the recipient exists
            if (!targetUser) {
                printf("Recipient not found!\n");
                break;
            }

            printf("Your current balance is: %d\n", user->balance);
            printf("Enter amount to transfer: ");
            (void)scanf("%d", &amount);
            clearInputBuffer();

            if (amount <= 0 || amount > user->balance) {
                printf("Invalid amount!\n");
            }
            else {
                user->balance -= amount;
                targetUser->balance += amount;
                SaveTransactionLog(user->username, "Transfer", amount, targetUser->username);
                SaveClientsToFileAfterDeletion();
                printf("Transfer successful! New balance: %d\n", user->balance);
            }
            break;

        case 4:
            printf("Exiting transactions.\n");
            break;

        default: // Handle invalid menu options
            printf("Invalid option! Try again.\n");
        }
    } while (choice != 4);
}

#endif