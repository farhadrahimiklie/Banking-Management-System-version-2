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

void Account_Menu(Accounts* accounts, credintials* username){
    int choice;

   do {
    printf("\n=================================================\n");
    printf("User Account Details \n");
    printf("\n===========================================\n");

    printf("1. Create Your Account\n");
    printf("2. View Your Account\n");
    printf("3. Update Your Account\n");
    printf("4. Delete Your Account\n");
    printf("0. Exits\n");
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
