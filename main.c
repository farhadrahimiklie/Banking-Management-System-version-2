#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct credintials{
    char username[35];
    char email[40];
    int password;
}credintials;

typedef struct Accounts{
    int Account_ID;
    char fullName[35];
    long int phoneNumber;
    char AccountType[9]; // savings, current
    double balance;
    char status[10]; // active, blocked, close
    char lastlogin[40]; // who's last login to account
    char dateCreated[40];
}Accounts;

typedef struct Transactions{
    int transactionID;
    int accountID; // who's transfer the money account ID
    char type; // deposit, withdraw, transfer
    double Amount; // how much money you transfer to someone account
    char date_time[40];
    double previous_balance; // last account balance is
    double new_balance; // and new account balance is
}Transactions;

void trim_newline(char *str)
{
    str[strcspn(str, "\n")] = '\0';
}

void Registers(credintials* credintial){
    FILE* ptr = fopen("data.bin", "ab");
    if (ptr == NULL) {
        perror("Cannot Open while Writing to file.  ");
        return;
    }

    printf("username__ ");
    fgets(credintial->username, sizeof(credintial->username), stdin);
    trim_newline(credintial->username);
    
    printf("email__ ");
    fgets(credintial->email, sizeof(credintial->email), stdin);

    printf("password__ ");
    scanf("%d", &credintial->password);

    fwrite(credintial, sizeof(credintials), 1, ptr);
    printf("FIle OK Successfully. \n");

    fclose(ptr);
}

void Account_Creation(Accounts* accounts, credintials* username){                                            
    time_t t = time(NULL);
    char time_str[40];
    strcpy(time_str, ctime(&t));
    time_str[strlen(time_str) - 1] = '\0';
    
    strcpy(accounts->dateCreated, time_str);
    strcpy(accounts->lastlogin, username->username);

    FILE* ptr = fopen("accounts.bin", "ab");
    if (ptr == NULL) {
        perror("Cannot open file while add account data. ");
        return;
    }

    fwrite(accounts, sizeof(Accounts), 1, ptr);
    fclose(ptr);
}

void View_All_Accounts(Accounts* accounts, credintials* username){
    FILE* ptr = fopen("accounts.bin", "rb");
    if (ptr == NULL) {
        perror("Cannot open file for reading. ");
        return;
    }


    int found = 0;
    while (fread(accounts, sizeof(Accounts), 1, ptr) == 1) {

        if (strcmp(accounts->lastlogin, username->username) == 0) {
            printf("Account_ID : %d \n", accounts->Account_ID);
            printf("FullName : %s \n", accounts->fullName);
            printf("phoneNumber : %ld \n", accounts->phoneNumber);
            printf("AccountType : %s \n", accounts->AccountType);
            printf("balance : %.3lf \n", accounts->balance);
            printf("status : %s \n", accounts->status);
            printf("lastlogin : %s \n", accounts->lastlogin);
            printf("dateCreated : %s \n", accounts->dateCreated);
            found = 1;
        }
    }

       if (!found) printf("No accounts found.\n");
       fclose(ptr);
}

void Update_My_Account(Accounts* accounts, credintials* username){
      FILE* ptr = fopen("accounts.bin", "rb+");
      if (ptr == NULL) {
          perror("Cannot open file for reading. ");
          return;
      }
      int found = 0;
      int choice;

      while (fread(accounts, sizeof(Accounts), 1, ptr) == 1) {
  
          if (strcmp(accounts->lastlogin, username->username) == 0) {

              found = 1;
              printf("1. FullName \n");
              printf("2. AccountType \n");
              printf("3. balance \n");
              printf("4. status \n");
              printf("enter your choice ");
              scanf("%d", &choice);
              getchar(); // clear buffer
              switch (choice) {
                  case 1:
                      printf("enter your new account name ");
                      fgets(accounts->fullName, sizeof(accounts->fullName), stdin);
                      trim_newline(accounts->fullName);
                      break;
                  case 2:
                      printf("enter your new account type ");
                      fgets(accounts->AccountType, sizeof(accounts->AccountType), stdin);
                      trim_newline(accounts->AccountType);
                      break;
                  case 3:
                      printf("enter your new account balance ");
                      scanf("%lf", &accounts->balance);
                      getchar();
                      break;
                  case 4:
                      printf("enter your new account status ");
                      fgets(accounts->status, sizeof(accounts->status), stdin);
                      trim_newline(accounts->status);
                      break;
                  default:
                      printf("invalid choice. \n");
                      fclose(ptr);
                      return;
              }
              fseek(ptr, -sizeof(Accounts), SEEK_CUR);
              fwrite(accounts, sizeof(Accounts), 1, ptr);

              printf("Account updated Successfully \n");
              break;
          } 
      } 

      if (!found) printf("No accounts found.\n");
      fclose(ptr);

}

void Delete_My_Account(credintials* username){
    FILE* ptr = fopen("accounts.bin", "rb");
    FILE* temp = fopen("temp.bin", "wb");
    if (!ptr || !temp) {
        perror("error wihle opening file. \n");
        return;
    }

    Accounts second;
    int found = 0;

    while(fread(&second, sizeof(Accounts), 1, ptr) == 1) {
        if (strcmp(second.lastlogin, username->username) == 0) {
            found = 1; // skip writing - delete it.
        }else {
            fwrite(&second, sizeof(Accounts), 1, temp);
        }
    }

    fclose(ptr);
    fclose(temp);

    remove("accounts.bin");
    rename("temp.bin", "accounts.bin");

    if (found) {
    printf("Account Deleted Successfully \n");
    }else {
    printf("No Account Found \n");
    }

}

void Check_Balance(Accounts* accounts, credintials* username){
    FILE* ptr = fopen("accounts.bin", "rb");
    if (ptr == NULL) {
        perror("Cannot open while reading the file. ");
        return;
    }

    int found = 0;

    while (fread(accounts, sizeof(Accounts), 1, ptr) == 1) {
        if (strcmp(accounts->lastlogin, username->username) == 0) {
            printf("Your Current Balance is %.3lf \n", accounts->balance);
            found = 1;
        }
    }

    if (!found) {
        printf("Account not found \n");
    }
    fclose(ptr);
}

static int count = 1;

void Transaction_Logs(Transactions* transaction ,Accounts* id, char type, double Add_balance, char time_logs[], double kuhna_balance, Accounts* balance){
    FILE* ptr = fopen("transaction_logs.bin", "ab");
    if (ptr == NULL) {
        perror("Cannot open file for writing in logs \n");
        return;
    }
    
    transaction->transactionID  = count++;
    transaction->accountID = id->Account_ID;
    transaction->type = type;
    transaction->Amount = Add_balance;
    strcpy(transaction->date_time, time_logs);
    transaction->previous_balance = kuhna_balance;
    transaction->new_balance = balance->balance;

    fwrite(transaction, sizeof(Transactions), 1, ptr);

    fclose(ptr);
}

void Show_Transaction_Logs(Transactions* transaction){
    FILE* ptr = fopen("transaction_logs.bin", "rb");
    if (ptr == NULL) {
    perror("cannot open file for reading \n");
    return;
    }
    int found = 0;
    while (fread(transaction, sizeof(Transactions), 1, ptr) == 1) {
        found = 1;
        printf("Transaction ID %d \n", transaction->transactionID);
        printf("Account_ID %d \n", transaction->accountID);
        printf("Type %c \n", transaction->type);
        printf("Amount %lf \n", transaction->Amount);
        printf("date created %s \n", transaction->date_time);
        printf("Previous Amount %lf \n", transaction->previous_balance);
        printf("New Amount %lf \n", transaction->new_balance);
    }
    fclose(ptr);
}

void Deposit_Money(Accounts* accounts, credintials* username, Transactions* transaction){
    FILE* ptr = fopen("accounts.bin", "rb+");
    if (ptr == NULL) {
        perror("cannot open while writing the file ");
        return;
    }

    time_t t = time(NULL);
    char time_logs[40];
    strcpy(time_logs, ctime(&t));
    time_logs[strlen(time_logs) - 1] = '\0';



    int found = 0;
    double n_balance;
    while (fread(accounts, sizeof(Accounts), 1, ptr) == 1) {
        if (strcmp(accounts->lastlogin, username->username) == 0) {
            found = 1;
            double old_balance = accounts->balance;
            printf("enter your deposit balance ");
            scanf("%lf", &n_balance);
            getchar(); // for clear buffer
            accounts->balance += n_balance;
            fseek(ptr, -sizeof(Accounts), SEEK_CUR);
            fwrite(accounts, sizeof(Accounts), 1, ptr);
            Transaction_Logs(transaction, accounts, 'W', n_balance ,time_logs, old_balance, accounts);
            printf("You Successfully Deposit your Money to your Account \n");
            break;
        }
    }
    if (!found) printf("Account not found \n");
    fclose(ptr);
}


void Withdraw_Money(Accounts* accounts, credintials* username, Transactions* transaction){
    FILE* ptr = fopen("accounts.bin", "rb+");
    if (ptr == NULL) {
        perror("cannot open while writing the file ");
        return;
    }

    time_t t = time(NULL);
    char time_logs[40];
    strcpy(time_logs, ctime(&t));
    time_logs[strlen(time_logs) - 1] = '\0';



    int found = 0;
    double n_balance;
    while (fread(accounts, sizeof(Accounts), 1, ptr) == 1) {
        if (strcmp(accounts->lastlogin, username->username) == 0) {
            found = 1;
            double old_balance = accounts->balance;
            printf("enter your Withdraw Money ");
            scanf("%lf", &n_balance);
            getchar(); // for clear buffer
            accounts->balance -= n_balance;
            fseek(ptr, -sizeof(Accounts), SEEK_CUR);
            fwrite(accounts, sizeof(Accounts), 1, ptr);
            Transaction_Logs(transaction, accounts, 'W', n_balance ,time_logs, old_balance, accounts);
            printf("You Successfully Withdraw your Money from your Account \n");
            break;
        }
    }
    if (!found) printf("Account not found \n");
    fclose(ptr);
}


void Account_Menu(Accounts* accounts, credintials* username, Transactions* transaction){
    int choice;

   do {
    printf("\n=================================================\n");
    printf("User Account Details \n");
    printf("\n===========================================\n");

    printf("1. Create Your Account\n");
    printf("2. View Your Account\n");
    printf("3. Update Your Account\n");
    printf("4. Delete Your Account\n");
    printf("5. Check Your Balance \n");
    printf("6. Deposit Money \n");
    printf("7. Withdraw Money \n");
    printf("8. Show Transactions Logs \n");
    printf("0. logout \n");
    printf("\n===========================================\n");
    printf("Enter your choice ");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
        printf("Create new account \n");
        printf("Account_ID__ ");
        scanf("%d", &accounts->Account_ID);
        getchar();

        printf("fullName__ ");
        fgets(accounts->fullName, sizeof(accounts->fullName), stdin);

        printf("phoneNumber__ ");
        scanf("%ld", &accounts->phoneNumber);
        getchar();

        printf("AccountType__ ");
        fgets(accounts->AccountType, sizeof(accounts->AccountType), stdin);

        printf("balance__ ");
        scanf("%lf", &accounts->balance);
        getchar();

        printf("status__ ");
        fgets(accounts->status, sizeof(accounts->status), stdin);
        Account_Creation(accounts, username);
        break;
     case 2:
        printf("View Your Account \n");
        View_All_Accounts(accounts, username);
        break;
    case 3:
        printf("Update Your Account \n");
        Update_My_Account(accounts, username);
        break;
    case 4:
        printf("Delete Your Account \n");
        Delete_My_Account(username);
        break;
    case 5:
        printf("check balance \n");
        Check_Balance(accounts, username);
        break;
    case 6:
        printf("Deposit Your Money \n");
        Deposit_Money(accounts, username, transaction);
        break;
    case 7:
        printf("Withdraw Your Money \n");
        Withdraw_Money(accounts, username, transaction);
        break;
    case 8:
        printf("Transaction logs \n");
        Show_Transaction_Logs(transaction);
        break;
     default:
        printf("Invalid Choice! \n");
        break;
    }

  }while(choice != 0);
}

void login(credintials* credintial, char username[], int password, Accounts* accounts, Transactions* transaction){
   FILE* ptr = fopen("data.bin", "rb");
   if (ptr == NULL) {
       perror("Cannot open file for reading.  ");
       return;
   }
   int found = 0;
   while (fread(credintial, sizeof(credintials), 1, ptr) == 1) {
       if (strcmp(credintial->username, username) == 0 && credintial->password == password) {
           found = 1;
           Account_Menu(accounts, credintial, transaction);    
           break;
       }
   }
   if (!found) {
       printf("user with [%d]  password is not found \n", password);
   }else {
       printf("user Successfully found \n");
   }

   fclose(ptr);
}

void Menu()
{
    printf("\n=================================================\n");
    printf("LOGIN SYSTEM \n");
    printf("\n===========================================\n");

    printf("1. Register\n");
    printf("2. login\n");
    printf("0. Exit\n");

}

int main()
{
    credintials credintial;
    Accounts accounts;
    Transactions transaction;

    int choice, password;
    char username[35];
    
        Menu();
        printf("enter your choice__ ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        switch (choice) {
            case 1:
                printf("First Register Yourself \n");
                Registers(&credintial);
                break;
           case 2:
                printf("login to your account \n");
                printf("enter your username__ ");
                fgets(username, sizeof(username), stdin);
                trim_newline(username);
                printf("enter your password__ ");
                scanf("%d", &password);
                login(&credintial, username, password, &accounts, &transaction);
                break;

          default:
                printf("Invalid Choice! ");
                break;
       }

    return 0;
}
