#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================
// STRUCTURE DEFINITION
// ============================================================

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// ============================================================
// FUNCTION PROTOTYPES
// ============================================================

unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

void initializeFile(void);
void displayHeader(void);
void displayFooter(void);
void pauseScreen(void);
void displayDateTime(void);

// ============================================================
// MAIN FUNCTION
// ============================================================

int main(int argc, char *argv[])
{
    FILE *cfPtr;
    unsigned int choice;

    initializeFile();

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    displayHeader();

    while ((choice = enterChoice()) != 5)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        default:
            puts("Incorrect choice");
            break;
        }

        pauseScreen();
    }

    fclose(cfPtr);

    displayFooter();

    return 0;
}

// ============================================================
// INITIALIZE FILE
// ============================================================

void initializeFile(void)
{
    FILE *filePtr;
    struct clientData blankClient = {0, "", "", 0.0};

    filePtr = fopen("credit.dat", "rb");

    if (filePtr == NULL)
    {
        filePtr = fopen("credit.dat", "wb");

        if (filePtr == NULL)
        {
            printf("Unable to create file.\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blankClient,
                   sizeof(struct clientData),
                   1,
                   filePtr);
        }
    }

    fclose(filePtr);
}

// ============================================================
// DISPLAY HEADER
// ============================================================

void displayHeader(void)
{
    printf("\n");
    printf("============================================\n");
    printf("      BANK ACCOUNT MANAGEMENT SYSTEM        \n");
    printf("============================================\n");

    displayDateTime();
}

// ============================================================
// DISPLAY FOOTER
// ============================================================

void displayFooter(void)
{
    printf("\n");
    printf("============================================\n");
    printf("         PROGRAM TERMINATED SUCCESSFULLY    \n");
    printf("============================================\n");
}

// ============================================================
// DISPLAY DATE AND TIME
// ============================================================

void displayDateTime(void)
{
    time_t t;
    time(&t);

    printf("Current Date and Time: %s\n", ctime(&t));
}

// ============================================================
// PAUSE SCREEN
// ============================================================

void pauseScreen(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("\nPress Enter to continue...");
    getchar();
}

// ============================================================
// CREATE TEXT FILE
// ============================================================

void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-6s%-16s%-11s%10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "Balance");

        while (fread(&client,
                     sizeof(struct clientData),
                     1,
                     readPtr) == 1)
        {
            if (client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-6u%-16s%-11s%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);

        printf("accounts.txt created successfully.\n");
    }
}

// ============================================================
// UPDATE RECORD
// ============================================================

void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account to update (1 - 100): ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account #%u has no information.\n", account);
    }
    else
    {
        printf("\nAccount Details\n");

        printf("%-6u%-16s%-11s%10.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        printf("\nEnter charge (+) or payment (-): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        printf("Updated Balance: %.2f\n",
               client.balance);

        fseek(fPtr,
              -((long)sizeof(struct clientData)),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);
    }
}

// ============================================================
// DELETE RECORD
// ============================================================

void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete (1 - 100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account %u does not exist.\n",
               accountNum);
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// ============================================================
// CREATE NEW RECORD
// ============================================================

void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account #%u already contains information.\n",
               client.acctNum);
    }
    else
    {
        printf("Enter lastname firstname balance:\n");

        scanf("%14s %9s %lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (client.acctNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("New account added successfully.\n");
    }
}

// ============================================================
// MENU FUNCTION
// ============================================================

unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\n");
    printf("1 - Store formatted text file\n");
    printf("2 - Update an account\n");
    printf("3 - Add a new account\n");
    printf("4 - Delete an account\n");
    printf("5 - End program\n");
    printf("Enter your choice: ");

    scanf("%u", &menuChoice);

    return menuChoice;
}

// ============================================================
// EXTRA FUNCTIONS
// ============================================================

void extraFunction1()
{
    printf("");
}

void extraFunction2()
{
    printf("");
}

void extraFunction3()
{
    printf("");
}

void extraFunction4()
{
    printf("");
}

void extraFunction5()
{
    printf("");
}

//
// ============================================================