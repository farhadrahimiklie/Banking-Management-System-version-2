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
    int phoneNumber;
    char AccountType[9];
    double balance;
    char status[10];
    char lastlogin[40];
    char dateCreated[40];
}Accounts;

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
        
            printf("Account_ID : %d \n", accounts->Account_ID);
            printf("FullName : %s \n", accounts->fullName);
            printf("phoneNumber : %d \n", accounts->phoneNumber);
            printf("AccountType : %s \n", accounts->AccountType);
            printf("balance : %.3lf \n", accounts->balance);
            printf("status : %s \n", accounts->status);
            printf("lastlogin : %s \n", accounts->lastlogin);
            printf("dateCreated : %s \n", accounts->dateCreated);
            found = 1;

    }

       if (!found) printf("No accounts found.\n");
       fclose(ptr);
}

void Account_Menu(Accounts* accounts, credintials* username){
    int choice;

   do {
    printf("\n=================================================\n");
    printf("User Account Details \n");
    printf("\n===========================================\n");

    printf("1. Create New Account\n");
    printf("2. View All Accounts\n");
    printf("3. Search Accounts\n");
    printf("4. Update Accounts\n");
    printf("5. Delete Accounts\n");
    printf("0. Exits\n");
    printf("\n===========================================\n");
    printf("enter your choice ");
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
        scanf("%d", &accounts->phoneNumber);
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
        printf("View All Accounts \n");
        View_All_Accounts(accounts, username);
        break;

     default:
        printf("Invalid Choice! \n");
        break;
    }

  }while(choice != 0);
}

void login(credintials* credintial, char username[], int password, Accounts* accounts){
   FILE* ptr = fopen("data.bin", "rb");
   if (ptr == NULL) {
       perror("Cannot open file for reading.  ");
       return;
   }
   int found = 0;
   while (fread(credintial, sizeof(credintials), 1, ptr) == 1) {
       if (strcmp(credintial->username, username) == 0 && credintial->password == password) {
           found = 1;
           Account_Menu(accounts, credintial);    
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
                login(&credintial, username, password, &accounts);
                break;

          default:
                printf("Invalid Choice! ");
                break;
       }

    return 0;
}
