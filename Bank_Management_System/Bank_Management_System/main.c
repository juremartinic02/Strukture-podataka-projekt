#include "functions.h"
#include "client_management.h"
#include "parameter_validation.h"
#include "sort.h"

int main() {
    srand((unsigned int)time(NULL));
    Menu();

    _CrtDumpMemoryLeaks();
    return 0;
}

void Menu() {
    int choice = 0; // Variable to store user menu choice
    int consoleWidth = 120;
    int i;
    char buffer[32];
    int validInput;

    do {
        system("cls"); // Clears the console screen

        // Upper border
        for (i = 0; i < consoleWidth; i++) printf("=");
        printf("\n");

        // Display the title of the bank management system
        printf("%*s%-*s\n", (consoleWidth - 35) / 2, "", (consoleWidth - 35) / 2, "BANKING MANAGEMENT SYSTEM");
        printf("%*s%-*s\n", (consoleWidth + 25) / 2, "", (consoleWidth + 25) / 2, "Projekt by Jure Martinic");

        // Middle border
        for (i = 0; i < consoleWidth; i++) printf("=");
        printf("\n");

        // Function that loads the clients from clients.txt file
        LoadClientsFromFile();
        printf("\n");

        printf(" 1. Create a new account\n");
        printf(" 2. View all clients\n");
        printf(" 3. Delete an existing account\n");
        printf(" 4. Update account\n");
        printf(" 5. Transactions\n");
        printf(" 6. Print transaction log\n");
        printf(" 7. Sort client accounts by:\n");
        printf(" 8. Exit\n");

        // Lower border
        for (i = 0; i < consoleWidth; i++) printf("=");
        printf("\n");

        do {
            printf("Enter your choice: ");
            validInput = 1; // We assume that input is valid initially

            // Clear the input buffer (removing waiting characters)
            // This is not the optimal way to do it
            fflush(stdin);

            // Reads input and checks if it is a number
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                for (i = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++) {
                    if (!isdigit(buffer[i])) {
                        validInput = 0;
                        break;
                    }
                }

                // If input is valid, atoi function coverts a string stored in buffer to actual integer value and check range
                if (validInput) {
                    choice = atoi(buffer);
                    if (choice < 1 || choice > 8) {
                        printf("Invalid choice! Please enter a number between 1 and 8.\n");
                        validInput = 0;
                    }
                }
                else {
                    printf("Invalid input! Please enter a number.\n");
                }
            }
        } while (!validInput);

        switch (choice) {
        case 1:
            CreateAccount();
            break;
        case 2:
            PrintClients();
            break;
        case 3:
            DeleteAccount();
            break;
        case 4:
            UpdateAccount();
            break;
        case 5:
            Transactions();
            break;
        case 6:
            PrintTransactionLog();
            break;
        case 7:
            SortAndPrintClients();
            break;
        case 8:
            printf("Exiting the program. Goodbye!\n");
            break;
        }

        system("pause"); // Pause the console before redisplaying the menu
    } while (choice != 8);
}

void SaveClientsToFile() {
    FILE* file = NULL;
    file = fopen("clients.txt", "w"); // Open the file in write mode
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write headers
    fprintf(file, "LIST OF ACCOUNTS IN THE BANK\n");
    fprintf(file, "-------------------------------------------------------------------------------\n");
    fprintf(file, "ID\tFirst Name(s)\tLast Name(s)\tUsername\tPassword\tBalance\tCreation Time\n");
    fprintf(file, "-------------------------------------------------------------------------------\n");

    // Traverse the linked list and write each client's data
    Client* temp = head;
    while (temp) {
        fprintf(file, "%d\t\"%s\"\t\"%s\"\t%s\t%s\t%d\t%s\n",
            temp->ID, temp->firstName, temp->lastName,
            temp->username, temp->password, temp->balance, temp->creationTime);
        temp = temp->next;
    }

    fclose(file);
    printf("All clients saved to file successfully.\n");
}

void FreeClientList() {
    Client* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;

        // Ensure all dynamically allocated strings are freed if any
        free(temp); // Free the client node
    }

    printf("Client list successfully freed.\n");
}

void LoadClientsFromFile() {
    FreeClientList(); // Clear the existing list before loading

    FILE* file = NULL;
    file = fopen("clients.txt", "r");
    if (!file) {
        printf("No existing data found. Starting fresh.\n");
        return;
    }

    char line[256];
    for (int i = 0; i < 4; i++) { // Skip headers
        if (fgets(line, sizeof(line), file) == NULL) {
            fclose(file);
            return;
        }
    }

    while (fgets(line, sizeof(line), file)) {
        Client* newClient = NULL;
        newClient = (Client*)malloc(sizeof(Client));
        if (!newClient) {
            perror("Memory allocation failed");
            fclose(file);
            return;
        }

        // Parse the data from the line into the Client structure
        if (sscanf(line, "%d\t\"%[^\"]\"\t\"%[^\"]\"\t%s\t%s\t%d\t%[^\n]",
            &newClient->ID, newClient->firstName, newClient->lastName,
            newClient->username, newClient->password,
            &newClient->balance, newClient->creationTime) != 7) {
            free(newClient);
            continue;
        }


        newClient->next = head;
        head = newClient;
    }

    fclose(file);
    printf("Clients loaded from file successfully.\n");
}

void PrintClients() {
    // Check if the linked list is empty
    if (!head) {
        printf("No clients found.\n");
        return;
    }

    // Pointer to traverse the linked list
    Client* temp = head;

    // Print headers with proper alignment
    printf("LIST OF ACCOUNTS IN THE BANK\n");
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("%-5s %-25s %-25s %-25s %-10s %-20s\n",
        "ID", "First Name(s)", "Last Name(s)", "Username", "Balance", "Creation Time");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (temp) {
        // Print each client's details with proper alignment
        printf("%-5d %-25s %-25s %-25s %-10d %-20s\n",
            temp->ID, temp->firstName, temp->lastName,
            temp->username, temp->balance, temp->creationTime);
        temp = temp->next;
    }
}

void SaveClientsToFileAfterDeletion() {
    FILE* file = NULL;
    file = fopen("clients.txt", "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "LIST OF ACCOUNTS IN THE BANK\n");
    fprintf(file, "-------------------------------------------------------------------------------\n");
    fprintf(file, "ID\tFirst Name\tLast Name\tUsername\tPassword\tBalance\tCreation Time\n");
    fprintf(file, "-------------------------------------------------------------------------------\n");

    // Start from the head of the linked list
    Client* temp = head;

    while (temp) {
        fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\n",
            temp->ID, temp->firstName, temp->lastName,
            temp->username, temp->password, temp->balance, temp->creationTime);
        temp = temp->next;
    }

    fclose(file);
    printf("Client list updated successfully.\n");
}

void SaveTransactionLog(const char* username, const char* action, int amount, const char* otherUsername) {
    FILE* file = fopen("transactions.txt", "a");
    if (!file) {
        perror("Failed to open transaction log file");
        return;
    }

    // Declare a buffer to store the current timestamp
    char timestamp[20];
    GetCurrentTime(timestamp, sizeof(timestamp)); // Get the current time and store it in the timestamp buffer

    if (otherUsername) {
        // Log the transaction with the recipient's username
        fprintf(file, "%s | %s | %d | To: %s | %s\n", username, action, amount, otherUsername, timestamp);
    }
    else {
        // Log the transaction without a recipient username
        fprintf(file, "%s | %s | %d | %s\n", username, action, amount, timestamp);
    }

    fclose(file);
}

void PrintTransactionLog() {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];

    // Prompt user for username and password
    printf("Enter your username: ");
    getValidatedInput(username, sizeof(username));

    printf("Enter your password: ");
    getValidatedInput(password, sizeof(password));

    // Validate credentials by searching for the username and password
    Client* user = FindClientByUsername(username, password);
    if (!user) {
        printf("Invalid username or password!\n");
        return;
    }

    // Open the transaction log file
    FILE* file = fopen("transactions.txt", "r");
    if (!file) {
        perror("Failed to open transaction log file");
        return;
    }

    char line[256]; // Buffer to store each line of the transaction log
    int found = 0; // Flag to check if any transactions were found for the user

    printf("\nTransaction Log for %s:\n", username);
    printf("-------------------------------------------------------------\n");

    // Read and filter transaction logs
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, username)) { // Check if the username appears in the log line
            printf("%s", line); // Print the transaction log line for the user
            found = 1; // Mark that at least one transaction was found
        }
    }

    fclose(file);

    if (!found) {
        printf("No transactions found for this account.\n");
    }

    printf("-------------------------------------------------------------\n");
}

void PrintClientList(Client* list) {
    // If the list is empty, print a message and return
    if (!list) {
        printf("No clients to display.\n");
        return;
    }

    printf("\nSorted List of Clients:\n");
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("%-5s %-20s %-20s %-20s %-10s %-20s\n",
        "ID", "First Name", "Last Name", "Username", "Balance", "Creation Time");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    // Print each client’s details in the list
    while (list) {
        printf("%-5d %-20s %-20s %-20s %-10d %-20s\n",
            list->ID, list->firstName, list->lastName, list->username, list->balance, list->creationTime);
        list = list->next;
    }
}

void hashPassword(const char* password, char* hashedPassword) {
    unsigned long hash = 5381; // Initialize the hash value with a prime number
    int c;
    while ((c = *password++)) {
        hash = ((hash << 5) + hash) + c; // hash = hash * 33 + c (DJB2 algorithm)
    }

    // Convert the hash value to a string and store it in the output buffer
    snprintf(hashedPassword, MAX_NAME_LENGTH, "%lu", hash); // Store the hash as a string
}
