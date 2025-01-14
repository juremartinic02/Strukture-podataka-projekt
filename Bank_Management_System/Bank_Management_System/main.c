#include "functions.h"

int main() {
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

int isValidName(const char* name) {
    // Check if the name is empty or exceeds the maximum allowed length
    if (strlen(name) > MAX_NAME_LENGTH - 1 || strlen(name) == 0) return 0;

    // Check every character of the name and return false if a character is neither alpabetic or space
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
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

void SaveClientsToFile() {
    FILE* file = NULL;
    file = fopen("clients.txt", "w"); // Open the file in write mode so we can save clients to the clients.txt file
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Check if the file is empty to write headers
    fseek(file, 0, SEEK_END); // Move the file pointer to the end of the file
    if (ftell(file) == 0) { // If file size is 0, write headers
        fprintf(file, "LIST OF ACCOUNTS IN THE BANK\n");
        fprintf(file, "-------------------------------------------------------------------------------\n");
        fprintf(file, "ID\tFirst Name\tLast Name\tUsername\tPassword\tBalance\tCreation Time\n");
        fprintf(file, "-------------------------------------------------------------------------------\n");
    }

    // Traverse the linked list and write each client's data
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
    for (int i = 0; i < 3; i++) { // Skip headers
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
        if (sscanf(line, "%d\t%s\t%s\t%s\t%s\t%d\t%[^\n]", &newClient->ID, newClient->firstName,
            newClient->lastName, newClient->username, newClient->password,
            &newClient->balance, newClient->creationTime) != 7) {
            free(newClient);
            continue;
        }

        // Verify if passwords are hashed
        if (strspn(newClient->password, "0123456789") != strlen(newClient->password)) {
            
            // If the password is not numeric, hash it and store the hashed version
            char hashedPassword[MAX_NAME_LENGTH];
            hashPassword(newClient->password, hashedPassword);
            strcpy(newClient->password, hashedPassword);
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
    printf("%-5s %-20s %-20s %-20s %-10s %-20s\n",
        "ID", "First Name", "Last Name", "Username", "Balance", "Creation Time");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    while (temp) {
        // Print each clients details with proper alignment
        printf("%-5d %-20s %-20s %-20s %-10d %-20s\n",
            temp->ID, temp->firstName, temp->lastName,
            temp->username, temp->balance, temp->creationTime);
        temp = temp->next;
    }
}

void GetCurrentTime(char* buffer, size_t bufferSize) {
    // Get the current time in seconds
    time_t t = time(NULL);

    // Convert the time to a structure representing local time
    struct tm* tm_info = localtime(&t);

    // Format the time as "YYYY-MM-DD HH:MM:SS" and store it in the buffer
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", tm_info);
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

void DeleteAccount() {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char confirmation[2];

    // Prompt for username and password
    printf("Enter username of the account to delete: ");
    getValidatedInput(username, sizeof(username));

    printf("Enter password for the account: ");
    getValidatedInput(password, sizeof(password));

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
                if (strlen(confirmation) == 1 && (confirmation[0] == 'y' || confirmation[0] == 'n')) {
                    break;
                }
                printf("Invalid input! Please enter 'y' or 'n'.\n");
            } while (1);

            if (confirmation[0] == 'y') {
                // Remove the account from the linked list
                if (prev == NULL) {
                    head = temp->next;
                }
                else {
                    prev->next = temp->next;
                }

                free(temp); // Free memory
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
    printf("Enter your username: ");
    getValidatedInput(username, sizeof(username));

    printf("Enter your password: ");
    getValidatedInput(password, sizeof(password));

    temp = FindClientByUsername(username, password);
    if (!temp) {
        printf("Account not found or incorrect credentials.\n");
        return;
    }

    printf("Account found!\n");

    // Update first name
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

    // Update last name
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

    // Update password
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
            hashPassword(newPassword, temp->password);
            printf("Password updated successfully.\n");

            // Save the updated client data immediately
            SaveClientsToFile();
            printf("Changes saved to file.\n");
        }
        else {
            printf("Password update failed. Ensure they match and meet requirements.\n");
        }
    }

    // Save any remaining changes to the file
    SaveClientsToFile();
    printf("Account updated successfully.\n");
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
