#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define MAX_ACCOUNTS 100
// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char Name[30];    // account  name
    char city[30];    // city name
    char phone[13];   // account phone number
    char password[7]; // account password
    double balance;       // account balance
};                        // end structure clientData

// prototypes
void menu(FILE *fPtr,unsigned int accNum);
void textFile(FILE *readPtr);
void registerAccount(FILE *fPtr);
void login(FILE *fPtr);
void viewAccount(FILE *fPtr,unsigned int accNum);

void updateRecord(FILE *fPtr,unsigned int accNum);
void deleteRecord(FILE *fPtr,unsigned int accNum);
void depositMoney(FILE *fPtr,unsigned int accNum);
void withdrawMoney(FILE *fPtr,unsigned int accNum);
void changePassword(FILE *fPtr,unsigned int accNum);
void transferMoney(FILE *fPtr,unsigned int accNum);
void deleteRecord(FILE *fPtr,unsigned int accNum);
void exitProgram();
int main()
{
    FILE *fPtr;         
    int choice; // user's choice

    // fopen opens the file; exits if file cannot be opened
    if ((fPtr = fopen("bankdata.dat", "rb+")) == NULL)
    {  
        if((fPtr = fopen("bankdata.dat","wb+")) == NULL){
        printf(" File could not be opened.\n");
        exit(1);
    }}

    // enable user to specify action
    while (1) {
        printf("----------------------\n");
        printf("\n WELCOME TO MY BANK\n");
        printf("1. Register Account\n2. Login\n3. Save and view in accounts.txt\n4. Exit\n\n Enter Your Choice: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
        
        case 1:
            registerAccount(fPtr); // to register
            break;
       
        case 2:
           login(fPtr); // to login
            break;
        
        case 3:
            textFile(fPtr); // create text file from record file
            break;
        
        case 4:
            exitProgram();// to exit    
            break;
        // display if user does not select valid choice
        default:
            printf("Invalid  choice! Please try again.\n");
            break;
        } // end switch
    }     // end while

    fclose(fPtr); // fclose closes the file
    return 0;
} // end main

// create formatted text file for printing

void textFile(FILE *readPtr) {
    FILE *writePtr; // accounts.txt file pointer
    struct clientData client = {0, "", "", "", "", 0.0};

    // fopen opens the file; exits if file cannot be opened
    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        puts("File could not be opened.");
    } else {
        rewind(readPtr); // sets pointer to beginning of file
        fprintf(writePtr, "%-6s%-16s%-11s%13s%7s%10s\n", "Acct", "Name", "City", "Phone", "Password", "Balance");

        // copy all records from random-access file into text file
        while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
            // write single record to text file
            if (client.acctNum != 0) {
                fprintf(writePtr, "%-6d%-16s%-11s%13s%7s%10.2f\n", client.acctNum, client.Name, client.city, client.phone, client.password, client.balance);
            }
        }

        fclose(writePtr); // fclose closes the file
    }printf("\n---VIEW THE ACCOUNTS.TXT FILE ----\n");
}

void registerAccount(FILE *fPtr){
    struct clientData client = {0, "", "","","", 0.0};
    char phoneStr[13];
    char passwordStr[7];
    printf("\nEnter account number (4 digit):");
    scanf("%d", &client.acctNum);
    printf("\nEnter your Name:");
    scanf("%s", client.Name);
    printf("Eter your city:");
    scanf("%s", client.city);
    printf("Enter your phone number(10 digits):");
    scanf("%s", phoneStr);
    printf("Enter your password(6 digits):");
    scanf("%s", passwordStr);
    printf("Enter initial deposit amount:");
    scanf("%lf", &client.balance);
    strncpy(client.phone, phoneStr,sizeof(client.phone)-1);
    client.phone[sizeof(client.phone)-1] = '\0';
    strncpy(client.password, passwordStr,sizeof(client.password)-1);
    client.password[sizeof(client.password)-1] = '\0';

    fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);
    printf("\n-----Account successfully registered!----\n");
}

// login
void login(FILE *fPtr){
    struct clientData client ={0, "", "","","", 0.0};
    unsigned int accNum;
    char password[7];
    printf("\nEnter your account number : ");
    scanf("%d", &accNum);
    printf("Enter your  password: ");
    scanf("%s", password);
    fseek(fPtr, (accNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);
    if(strcmp(client.password, password) == 0 && client.acctNum !=0){
        printf("===Login was  successful Welcome ===%s.\n",client.Name);
        menu(fPtr, accNum);
        }
    else{
        printf("Invalid account number or password.\n");
    }
}
// view account// view account


void viewAccount(FILE *fPtr, unsigned int accNum){
    struct clientData client ={0, "", "","","", 0.0};
    fseek(fPtr,(accNum -1)* sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);
    if(client.acctNum !=0){
        printf("\n====Account Information====\n");
        printf("Account Number: %d\n",client.acctNum);
        printf("Name: %s\n",client.Name);
        printf("City: %s\n",client.city);
        printf("Phone: %s\n",client.phone);
        printf("Balance: %.2f\n",client.balance);
    }else{
        printf("\n ====Account not found!===\n");
    }
}
// update balance in record
void updateRecord(FILE *fPtr, unsigned int accNum)
{ struct clientData client ={0, "", "","","", 0.0};
    // move file pointer to correct record in file
    fseek(fPtr, (accNum - 1) * sizeof(struct clientData), SEEK_SET);
    // read record from file
    fread(&client, sizeof(struct clientData), 1, fPtr);
    // display error if account does not exist
    if (client.acctNum != 0)
    {
        printf("=== UPDATING ACCOUNT ===\n");
        printf("Enter new Name: ");
        scanf("%s",client.Name);
        printf("Enter new city: ");
        scanf("%s",client.city);
        printf("Enter new phone number(10 digits): ");
        scanf("%s",client.phone);
        fseek(fPtr, (accNum - 1) * sizeof(struct clientData), SEEK_SET);
        fread(&client, sizeof(struct clientData), 1, fPtr);
        printf("=======Record updated Successfully!=======\n");
    }
    else
    { printf("Account not found.\n"); 
    } // end else
} // end function updateRecord

// delete an existing record
void deleteRecord(FILE *fPtr,unsigned int accNum)
{
    struct clientData client ={0,"","","","",0.0};         
    fseek(fPtr, (accNum - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client,sizeof(struct clientData),1,fPtr);
    printf("\n-----Account successfully deleted!----\n");
} 
void depositeMoney(FILE *fPtr,unsigned int accNum){
    struct clientData client ={0, "", "","","", 0.0};
    double amount;
    fseek(fPtr,(accNum -1)* sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);
    if(client.acctNum !=0){
        printf("\nEnter amount to deposit: ");
        scanf("%lf", &amount);
        client.balance += amount;
        fseek(fPtr,(accNum -1)* sizeof(struct clientData),SEEK_SET);
        fwrite(&client,sizeof(struct clientData),1,fPtr);
        printf("===Deposit successful! New Balance : %.2f===\n",client.balance);
    }else{
        printf("Account not found.\n");
    }
}
void withdrawMoney(FILE *fPtr,unsigned int accNum){
    struct clientData client ={0, "", "","","", 0.0};
    double amount;
    fseek(fPtr,(accNum -1)* sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);
    if(client.acctNum !=0){
        printf("\nEnter amount to withdraw: ");
        scanf("%lf",&amount);
        if (amount >client.balance){
            printf("Insufficient balance.\n");
        }
        else{
            client.balance -= amount;
            fseek(fPtr,(accNum -1)* sizeof(struct clientData),SEEK_SET);
            fwrite(&client,sizeof(struct clientData),1,fPtr);
            printf("===Withdraw successful! New Balance : %.2f===\n",client.balance);
        }
    }else{
        printf("Account not found.\n");
    }
}
void changePassword(FILE *fPtr,unsigned int accNum){
    struct clientData client ={0, "", "","","", 0.0};
    char newPassword[7];
    fseek(fPtr,(accNum-1)* sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);
    if(client.acctNum !=0){
        printf("\nEnter new password(6 digits): ");
        scanf("%s",newPassword);
        strncpy(client.password,newPassword,sizeof(client.password)-1);
        client.password[sizeof(client.password)-1] = '\0';
        fseek(fPtr,(accNum-1)* sizeof(struct clientData),SEEK_SET);
        fwrite(&client,sizeof(struct clientData),1,fPtr);
        printf("===Password successfully changed!===\n");
    }else{
        printf("Account not found.\n");
    }
}
void transferMoney(FILE *fPtr,unsigned int accNum){
    struct clientData clientSender ={0, "", "","","", 0.0};
    struct clientData clientReceiver ={0, "", "","","", 0.0};
    unsigned int receiverAccNum;
    double amount;
    fseek(fPtr,(accNum-1)* sizeof(struct clientData),SEEK_SET);
    fread(&clientSender,sizeof(struct clientData),1,fPtr);
    if(clientSender.acctNum !=0){
        printf("\nEnter receiver account number: ");
        scanf("%d",&receiverAccNum);
        fseek(fPtr,(receiverAccNum-1)* sizeof(struct clientData),SEEK_SET);
        fread(&clientReceiver,sizeof(struct clientData),1,fPtr);
        if(clientReceiver.acctNum !=0){
            printf("\nEnter amount to transfer: ");
            scanf("%lf",&amount);
            if(amount > clientSender.balance){
                printf("Insufficient balance.\n");
            }else{
                clientSender.balance -= amount;
                clientReceiver.balance += amount;
                fseek(fPtr,(accNum-1)* sizeof(struct clientData),SEEK_SET);
                fwrite(&clientSender,sizeof(struct clientData),1,fPtr);
                fseek(fPtr,(receiverAccNum-1)* sizeof(struct clientData),SEEK_SET);
                fwrite(&clientReceiver,sizeof(struct clientData),1,fPtr);
                printf("===Transfer successful!===\n");
            }
        }else{
            printf("Receiver account not found.\n");
        }
    }else{
        printf("Sender account not found.\n");
    }
}
//menu
void menu(FILE *fPtr, unsigned int accNum){
    int choice;
    do{
        printf("\n===MENU===\n");
        printf("1. View account\n");
        printf("2. Update account\n");
        printf("3. Delete account\n");
        printf("4. Deposit money\n");
        printf("5. Withdraw money\n");
        printf("6. Change password\n");
        printf("7. Transfer money\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                viewAccount(fPtr, accNum);
                break;
            case 2:
                updateRecord(fPtr, accNum);
                break;
            case 3:
                deleteRecord(fPtr, accNum);
                break;
            case 4:
                depositeMoney(fPtr, accNum);
                break;
            case 5:
                withdrawMoney(fPtr, accNum);
                break;
            case 6:
                changePassword(fPtr, accNum);
                break;
            case 7:
                transferMoney(fPtr, accNum);
                break;
            case 8:
                textFile(fPtr);
                break;
            default:
                printf("Invalid choice! Please try again.\n");    
               break;
            
        }
    }  while(choice != 8);
    
}
void exitProgram(){
    printf("\nThank you for using our banking system!\n");
    exit(0);
}