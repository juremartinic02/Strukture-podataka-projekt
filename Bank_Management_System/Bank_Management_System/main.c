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

// Deklaracije funkcija
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

// Glavna funkcija
int main() {
    Menu();
    return 0;
}

// Update menu to include the new UpdateAccount function
void Menu() {
    int choice = 0;
    int consoleWidth = 120;
    int i;
    char buffer[32];
    int validInput;

    do {
        system("cls");

        // Upper border
        for (i = 0; i < consoleWidth; i++) printf("=");
        printf("\n");

        printf("%*s%-*s\n", (consoleWidth - 35) / 2, "", (consoleWidth - 35) / 2, "BANKING MANAGEMENT SYSTEM");
        printf("%*s%-*s\n", (consoleWidth + 25) / 2, "", (consoleWidth + 25) / 2, "Projekt by Jure Martinic");

        // Middle section
        for (i = 0; i < consoleWidth; i++) printf("=");
        printf("\n");

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

        for (i = 0; i < consoleWidth; i++) printf("=");
        printf("\n");

        do {
            printf("Enter your choice: ");
            validInput = 1;

            fflush(stdin);

            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                for (i = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++) {
                    if (!isdigit(buffer[i])) {
                        validInput = 0;
                        break;
                    }
                }

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

        system("pause");
    } while (choice != 8);
}

int isValidName(const char* name) {
    if (strlen(name) > MAX_NAME_LENGTH - 1 || strlen(name) == 0) return 0;

    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int isValidUserCredential(const char* str) {
    if (strlen(str) > MAX_NAME_LENGTH - 1 || strlen(str) == 0) return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i])) return 0;  // Only letters and numbers allowed
    }
    return 1;
}

int getValidatedInput(char* buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return 0; // Input failed
    }
    // Check if newline exists in the buffer
    if (strchr(buffer, '\n') == NULL) {
        clearInputBuffer(); // Clear excess characters
        return 0; // Input too long
    }
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
    return 1; // Input successful
}


int generateUniqueID() {
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }

    int newID;
    int isUnique;
    do {
        isUnique = 1;
        newID = rand() % 1001;  // Generate ID between 0 and 1000

        // Check if ID already exists
        Client* current = head;
        while (current != NULL) {
            if (current->ID == newID) {
                isUnique = 0;
                break;
            }
            current = current->next;
        }
    } while (!isUnique);

    return newID;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void CreateAccount() {
    Client* newClient = (Client*)malloc(sizeof(Client));
    if (!newClient) {
        perror("Memory allocation failed");
        return;
    }

    char buffer[MAX_NAME_LENGTH];

    // Generate random unique ID
    newClient->ID = generateUniqueID();
    printf("\nGenerated ID for new client: %d\n", newClient->ID);

    // First Name validation
    do {
        printf("Enter first name (letters only, max %d characters): ", MAX_NAME_LENGTH - 1);
        if (getValidatedInput(buffer, sizeof(buffer)) && isValidName(buffer)) {
            strcpy(newClient->firstName, buffer);
            break;
        }
        printf("Invalid first name or input too long! Use only letters.\n");
    } while (1);

    // Last Name validation
    do {
        printf("Enter last name (letters only, max %d characters): ", MAX_NAME_LENGTH - 1);
        if (getValidatedInput(buffer, sizeof(buffer)) && isValidName(buffer)) {
            strcpy(newClient->lastName, buffer);
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
    char passwordConfirm[MAX_NAME_LENGTH];
    do {
        printf("Enter password (letters and numbers only, max %d characters): ", MAX_NAME_LENGTH - 1);
        if (getValidatedInput(buffer, sizeof(buffer)) && isValidUserCredential(buffer)) {
            printf("Confirm password: ");
            if (getValidatedInput(passwordConfirm, sizeof(passwordConfirm)) && strcmp(buffer, passwordConfirm) == 0) {
                strcpy(newClient->password, buffer);
                break;
            }
            printf("Passwords do not match! Please try again.\n");
        }
        else {
            printf("Invalid password or input too long! Use only letters and numbers.\n");
        }
    } while (1);

    // Balance validation (user cant enter a negative number)
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
    newClient->next = head;
    head = newClient;

    printf("Account successfully created!\n");

    SaveClientsToFile();
}


void SaveClientsToFile() {
    FILE* file = fopen("clients.txt", "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Check if the file is empty to write headers
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) { // If file size is 0, write headers
        fprintf(file, "LIST OF ACCOUNTS IN THE BANK\n");
        fprintf(file, "-------------------------------------------------------------------------------\n");
        fprintf(file, "ID\tFirst Name\tLast Name\tUsername\tPassword\tBalance\tCreation Time\n");
        fprintf(file, "-------------------------------------------------------------------------------\n");
    }

    Client* temp = head;
    while (temp) {
        fprintf(file, "%d\t%s\t%s\t%s\t%s\t%d\t%s\n",
            temp->ID, temp->firstName, temp->lastName,
            temp->username, temp->password, temp->balance, temp->creationTime);
        temp = temp->next;
    }

    fclose(file);
    printf("All clients appended to file successfully.\n");
}

void FreeClientList() {
    Client* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void LoadClientsFromFile() {
    FreeClientList(); // Clear the existing list before loading

    FILE* file = fopen("clients.txt", "r");
    if (!file) {
        printf("No existing data found. Starting fresh.\n");
        return;
    }

    char line[256];
    for (int i = 0; i < 3; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fclose(file);
            return;
        }
    }

    while (fgets(line, sizeof(line), file)) {
        Client* newClient = (Client*)malloc(sizeof(Client));
        if (!newClient) {
            perror("Memory allocation failed");
            fclose(file);
            return;
        }

        if (sscanf(line, "%d\t%s\t%s\t%s\t%s\t%d\t%[^\n]", &newClient->ID, newClient->firstName,
            newClient->lastName, newClient->username, newClient->password,
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
    if (!head) {
        printf("No clients found.\n");
        return;
    }

    Client* temp = head;

    // Print headers with proper alignment
    printf("LIST OF ACCOUNTS IN THE BANK\n");
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("%-5s %-20s %-20s %-20s %-20s %-10s %-20s\n",
        "ID", "First Name", "Last Name", "Username", "Password", "Balance", "Creation Time");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (temp) {
        // Print each clients details with proper alignment
        printf("%-5d %-20s %-20s %-20s %-20s %-10d %-20s\n",
            temp->ID, temp->firstName, temp->lastName,
            temp->username, temp->password, temp->balance, temp->creationTime);
        temp = temp->next;
    }
}

void GetCurrentTime(char* buffer, size_t bufferSize) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", tm_info);
}

void SaveClientsToFileAfterDeletion() {
    FILE* file = fopen("clients.txt", "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "LIST OF ACCOUNTS IN THE BANK\n");
    fprintf(file, "-------------------------------------------------------------------------------\n");
    fprintf(file, "ID\tFirst Name\tLast Name\tUsername\tPassword\tBalance\tCreation Time\n");
    fprintf(file, "-------------------------------------------------------------------------------\n");

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

void DeleteAccount() {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char confirmation;

    printf("Enter username of the account to delete: ");
    getValidatedInput(username, sizeof(username));

    printf("Enter password for the account: ");
    getValidatedInput(password, sizeof(password));

    Client* temp = head;
    Client* prev = NULL;

    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0) {
            printf("Account found! Are you sure you want to delete this account? (y/n): ");
            confirmation = getchar();
            clearInputBuffer();

            if (tolower(confirmation) == 'y') {
                if (prev == NULL) {
                    head = temp->next;
                }
                else {
                    prev->next = temp->next;
                }

                free(temp);
                SaveClientsToFileAfterDeletion();
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
    printf("Enter your username: ");
    getValidatedInput(username, sizeof(username));

    printf("Enter your password: ");
    getValidatedInput(password, sizeof(password));

    // Search for the client
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0) {
            printf("Account found!\n");

            // Iterate through each field for potential updates
            char choice;

            printf("Current first name: %s\nDo you want to update it? (y/n): ", temp->firstName);
            choice = getchar();
            clearInputBuffer();
            if (tolower(choice) == 'y') {
                char newFirstName[MAX_NAME_LENGTH];
                printf("Enter new first name: ");
                getValidatedInput(newFirstName, sizeof(newFirstName));
                if (isValidName(newFirstName)) {
                    strcpy(temp->firstName, newFirstName);
                    printf("First name updated successfully.\n");
                }
                else {
                    printf("Invalid first name. Update skipped.\n");
                }
            }

            printf("Current last name: %s\nDo you want to update it? (y/n): ", temp->lastName);
            choice = getchar();
            clearInputBuffer();
            if (tolower(choice) == 'y') {
                char newLastName[MAX_NAME_LENGTH];
                printf("Enter new last name: ");
                getValidatedInput(newLastName, sizeof(newLastName));
                if (isValidName(newLastName)) {
                    strcpy(temp->lastName, newLastName);
                    printf("Last name updated successfully.\n");
                }
                else {
                    printf("Invalid last name. Update skipped.\n");
                }
            }

            printf("Current username: %s\nDo you want to update it? (y/n): ", temp->username);
            choice = getchar();
            clearInputBuffer();
            if (tolower(choice) == 'y') {
                char newUsername[MAX_NAME_LENGTH];
                printf("Enter new username: ");
                getValidatedInput(newUsername, sizeof(newUsername));
                if (isValidUserCredential(newUsername)) {
                    // Ensure the username is unique
                    Client* current = head;
                    int usernameExists = 0;
                    while (current != NULL) {
                        if (strcmp(current->username, newUsername) == 0) {
                            usernameExists = 1;
                            break;
                        }
                        current = current->next;
                    }
                    if (!usernameExists) {
                        strcpy(temp->username, newUsername);
                        printf("Username updated successfully.\n");
                    }
                    else {
                        printf("Username already exists. Update skipped.\n");
                    }
                }
                else {
                    printf("Invalid username. Update skipped.\n");
                }
            }

            printf("Do you want to update the password? (y/n): ");
            choice = getchar();
            clearInputBuffer();
            if (tolower(choice) == 'y') {
                char newPassword[MAX_NAME_LENGTH], passwordConfirm[MAX_NAME_LENGTH];
                printf("Enter new password: ");
                getValidatedInput(newPassword, sizeof(newPassword));
                printf("Confirm new password: ");
                getValidatedInput(passwordConfirm, sizeof(passwordConfirm));
                if (strcmp(newPassword, passwordConfirm) == 0 && isValidUserCredential(newPassword)) {
                    strcpy(temp->password, newPassword);
                    printf("Password updated successfully.\n");
                }
                else {
                    printf("Password update failed. Ensure they match and meet requirements.\n");
                }
            }

            SaveClientsToFileAfterDeletion(); // Save updated data to file
            printf("Account updated successfully.\n");
            return;
        }
        temp = temp->next;
    }

    printf("Account not found or incorrect credentials.\n");
}

void Transactions() {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    Client* user;

    printf("Enter your username: ");
    getValidatedInput(username, sizeof(username));

    printf("Enter your password: ");
    getValidatedInput(password, sizeof(password));

    user = FindClientByUsername(username, password);
    if (!user) {
        printf("Invalid username or password!\n");
        return;
    }

    int choice, amount;
    char targetUsername[MAX_NAME_LENGTH];
    Client* targetUser;

    do {
        printf("\n1. Deposit money\n");
        printf("2. Withdraw money\n");
        printf("3. Transfer money\n");
        printf("4. Exit transactions\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1: // Deposit money
            printf("Your current balance is: %d\n", user->balance);
            printf("Enter amount to deposit: ");
            scanf("%d", &amount);
            clearInputBuffer();

            if (amount <= 0) {
                printf("Invalid amount!\n");
            }
            else {
                user->balance += amount;
                SaveTransactionLog(user->username, "Deposit", amount, NULL);
                SaveClientsToFileAfterDeletion();
                printf("Deposit successful! New balance: %d\n", user->balance);
            }
            break;

        case 2: // Withdraw money
            printf("Your current balance is: %d\n", user->balance);
            printf("Enter amount to withdraw: ");
            scanf("%d", &amount);
            clearInputBuffer();

            if (amount <= 0 || amount > user->balance) {
                printf("Invalid amount!\n");
            }
            else {
                user->balance -= amount;
                SaveTransactionLog(user->username, "Withdraw", amount, NULL);
                SaveClientsToFileAfterDeletion();
                printf("Withdrawal successful! New balance: %d\n", user->balance);
            }
            break;

        case 3: // Transfer money
            printf("Enter the username of the recipient: ");
            getValidatedInput(targetUsername, sizeof(targetUsername));
            targetUser = FindClientByUsername(targetUsername, NULL);

            if (!targetUser) {
                printf("Recipient not found!\n");
                break;
            }

            printf("Your current balance is: %d\n", user->balance);
            printf("Enter amount to transfer: ");
            scanf("%d", &amount);
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

        default:
            printf("Invalid option! Try again.\n");
        }
    } while (choice != 4);
}

// Find a client by username and password
Client* FindClientByUsername(const char* username, const char* password) {
    Client* temp = head;
    while (temp) {
        if (strcmp(temp->username, username) == 0 && (!password || strcmp(temp->password, password) == 0)) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Save transaction log
void SaveTransactionLog(const char* username, const char* action, int amount, const char* otherUsername) {
    FILE* file = fopen("transactions.txt", "a");
    if (!file) {
        perror("Failed to open transaction log file");
        return;
    }

    char timestamp[20];
    GetCurrentTime(timestamp, sizeof(timestamp));

    if (otherUsername) {
        fprintf(file, "%s | %s | %d | To: %s | %s\n", username, action, amount, otherUsername, timestamp);
    }
    else {
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

    // Validate credentials
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

    char line[256];
    int found = 0;

    printf("\nTransaction Log for %s:\n", username);
    printf("-------------------------------------------------------------\n");

    // Read and filter transaction logs
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, username)) {
            printf("%s", line);
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf("No transactions found for this account.\n");
    }

    printf("-------------------------------------------------------------\n");
}

void SortAndPrintClients() {
    int choice, order;
    Client* sortedList = NULL;

    // Prompt user for sorting parameter
    printf("\nChoose sorting parameter:\n");
    printf("1. Sort by Balance\n");
    printf("2. Sort by Name\n");
    printf("3. Sort by Last Name\n");
    printf("4. Sort by Username\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

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
    scanf("%d", &order);
    clearInputBuffer();

    if (order < 1 || order > 2) {
        printf("Invalid choice! Returning to main menu.\n");
        return;
    }

    // Create a copy of the client list for sorting
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

// Function to create a copy of the client list
Client* CopyClientList() {
    Client* copyHead = NULL, * current = head, * newClient, * last = NULL;

    while (current) {
        newClient = (Client*)malloc(sizeof(Client));
        if (!newClient) {
            perror("Memory allocation failed");
            FreeClientList(copyHead);
            return NULL;
        }

        *newClient = *current; // Copy the structure
        newClient->next = NULL;

        if (!copyHead) {
            copyHead = newClient;
        }
        else {
            last->next = newClient;
        }
        last = newClient;
        current = current->next;
    }
    return copyHead;
}

// Function to sort clients by balance
void SortClientsByBalance(Client** headRef, int descending) {
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
            maxPrev->next = maxNode->next;
        }
        else {
            current = maxNode->next;
        }

        // Add the max/min node to the sorted list
        maxNode->next = sorted;
        sorted = maxNode;
    }
    *headRef = sorted;
}

// Function to sort clients by string fields (e.g., name, last name, username)
void SortClientsByStringField(Client** headRef, size_t offset, int descending) {
    if (!*headRef || !(*headRef)->next) return;

    Client* sorted = NULL, * current = *headRef, * prev, * maxPrev, * maxNode;

    while (current) {
        maxPrev = NULL;
        maxNode = current;
        prev = current;

        // Find the node with the max/min string value
        while (prev->next) {
            const char* field1 = (const char*)((char*)maxNode + offset);
            const char* field2 = (const char*)((char*)prev->next + offset);

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
        sorted = maxNode;
    }
    *headRef = sorted;
}

// Function to print the list of clients
void PrintClientList(Client* list) {
    if (!list) {
        printf("No clients to display.\n");
        return;
    }

    printf("\nSorted List of Clients:\n");
    printf("---------------------------------------------------------------------------------------------------------------\n");
    printf("%-5s %-20s %-20s %-20s %-10s %-20s\n",
        "ID", "First Name", "Last Name", "Username", "Balance", "Creation Time");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (list) {
        printf("%-5d %-20s %-20s %-20s %-10d %-20s\n",
            list->ID, list->firstName, list->lastName, list->username, list->balance, list->creationTime);
        list = list->next;
    }
}
