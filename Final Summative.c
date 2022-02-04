//Taseen's ICS3U Summative
//January 6th

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef struct { //Structure to hold the data of all the users
    char fn[20];
    char ln[20];
    char email[30];
    char username[20];
    char password[20];
}userinfo;

typedef struct{ //Structure to hold the data of the groceries
    char item[40];
    char supplier[40];
    char department[40];
    char unitmeasure[40];
    float price;
    int quantity;
}groceryinfo;

typedef struct{ //Structure to hold users cart
    char username[50];
    char itemnames[300][300];
    float totalcost;
}buyercart;

int MngLogin(int, groceryinfo *); //Function will login the manager
int MngMenu(int, int, char[15][15], char[15][15], groceryinfo *); //The menu functions that managers (after logging in) can access
void DisplayInv(int, groceryinfo *); //Displays the inventory for a managers perspective
void DisplayInvBuyers(int, groceryinfo *); //Displays the inventory for a buyers perspective
int AddInv(int, groceryinfo *); //Function for managers only to add a product to inventory
void EditInv(int, groceryinfo *); //Function for managers only to edit a product in inventory
int Usercycle(userinfo *); // Reads in all the existing users.
int Groceryread(groceryinfo *); //Reads the groceries into the structure, returns the number of products
void GrocerySort(int, groceryinfo *); //Sorts groceries by department, then item AND REWRITES THE FILE so it's in the correct order
void BuyerLogin(int, int *, int, userinfo *, groceryinfo *, buyercart *); //Function that logs in existing users, or takes a new user to Usercreate();
void BuyerMnu(int, int, userinfo *, groceryinfo *, buyercart *); //The menu functions that users (after logging in) can access
void BuyerPurchases(int, int, userinfo *, groceryinfo *, buyercart *);
void Usercreate(int, int, userinfo *, groceryinfo *, buyercart *); //Will create a new account user and add it to the database

int main(void){
    userinfo *users[300]; //Declaration of the struct of users and the max amount of users its holding
    groceryinfo *groceries[960]; //Declaration of struct of groceries and its max capaciity
    buyercart *carts[900]; //Declaration of struct of carts and its max capacity
    int choice;
    int userloggedin=0; //Represents the user logged in
    int productcount=0; //Product counter (value of how many products there are)
    int userquant=0; //Usercounter (value of how many users there are

    productcount=Groceryread(groceries); //Reading in grocery list and counting the amount of products

    GrocerySort(productcount, groceries); //Sorting the products in correct alphabetical order

    printf("Welcome to Taseen's Grocery Haven\n\n");
    printf("Please select a user type for login:\n1. Manager\n2. Buyer\n\n-> ");
    scanf("%i",&choice);

    while(choice > 2 || choice <= 0){ //Filtering invalid entries
        printf("\nError, please re-select\n\n-> ");
        scanf("%i",&choice);
    }

    system("CLS");

    if(choice == 1){
        productcount=MngLogin(productcount, groceries); //User will be sent to the manager function, which is also returning a possibly new productcount value
    }

    if(choice == 2){
        userquant=Usercycle(users); //The program will obtain the number of existing users, and read them into the structure
        BuyerLogin(productcount,&userloggedin,userquant,users,groceries,carts); //The user will be sent to the buyer login
    }
}

int MngLogin(int productcount, groceryinfo groceries[]){

    int userchoice=0;
    int managerval=0;
    int i=0;
    int comparediff;
    char userentered[15];
    char savedmanagers[15][15];
    char savedpasswords[15][15];

    printf("Management Login\n");

    FILE *fp;
    fp=fopen("managerlogininfo.txt" , "r+");

    while(!feof(fp)){
         fscanf(fp,"%s",savedmanagers[i]); //Reading the manager names from the file into the variable
         fscanf(fp,"%s",savedpasswords[i]); //Reading the manager passwords from the file into the varible
         i++; //Counter representing the number of managers
    }

    printf("\nSelect User (1-3):\n1. %s\n2. %s\n3. %s\n\n-> ",savedmanagers[0],savedmanagers[1],savedmanagers[2]);
    scanf("%i",&userchoice);
    
    while(userchoice > 3 || userchoice < 1){
        printf("Invalid choice, please re-select\n-> ");
        scanf("%i",&userchoice);
    }

    system("CLS");

    printf("Enter password for %s:\n-> ",savedmanagers[userchoice-1]);

    scanf("%s",&userentered); //Taking the user entered username

    comparediff=strcmp(userentered,savedpasswords[userchoice-1]); //This will compare what the user entered, to the password of the chosen manager (-1 because arrays start at 0)

    managerval=userchoice-1; //Changing the user inputted manager to its array value

    while(comparediff != 0){ //If the user entered password is incorrect when compared to the actual one
        printf("\nIncorrect password, please try again\n-> ");
        scanf("%s",&userentered);
        comparediff=strcmp(userentered,savedpasswords[userchoice-1]);
    }

        if(comparediff == 0){ //If user entered password matches the correct one
            system("CLS");
            printf("*Password Correct*\n\n");
            system("PAUSE");
            system("CLS");
            productcount=MngMenu(productcount, managerval, savedmanagers, savedpasswords, groceries); //Sending the logged in manager number, and all the managers names, and passwords
        }

    fclose(fp);

    return productcount; //Sending back the value of productcount which is possibly changed throughout the function

}

void BuyerLogin(int productcount, int *loginnum, int buyercnt, userinfo users[], groceryinfo groceries[], buyercart carts[]){

    int accnum=0; //Represents the user the program is looking at the possible user attempted to be logged into
    int loginchoice;
    char entereduser[25];
    char enteredpass[25];
    int usercomp;
    int passcomp;

    printf("Buyer Login\n");
    printf("\nSelect from the follow:\n\n1. Login - Existing User\n2. Signup - Create New User\n\n-> ");
    scanf("%i",&loginchoice);

    while(loginchoice > 2 || loginchoice <= 0){
        printf("\nInvalid choice, please re-select\n-> ");
        scanf("%i",&loginchoice);
    }

    if(loginchoice == 2){
        system("CLS");
        Usercreate(productcount, buyercnt, users, groceries,carts); //User will be sent to create a non existing user in the function Usercreate
        accnum=buyercnt; //After a new user is created and subsenquently logged into, the accnum value becomes the value of the buyer count (so the last user), which represents which user number is logged in
        }

    if(loginchoice ==1){

        system("CLS");
        printf("Enter your username: ");
        scanf("%s",&entereduser);

        usercomp=strcmp(entereduser,users[accnum].username); //Comparing the userentered name to the actual username

        while(usercomp != 0){ //If the user entered username doesn't match that of the specified actual one
            accnum++; //This will then check the next users username to see if the user entered username mathces
            usercomp=strcmp(entereduser,users[accnum].username); //Comparing again the user entered username with the next actual username
            if(accnum >= buyercnt){ //If the array value of users surpasses the actual amount of accounts
                printf("\nUsername not recognized, try again: ");
                scanf("%s",&entereduser); //Rescanning user inputted username when it doesn't match any usernames in the file
                accnum=0; //Restarting the user value to check again from the first user in the file
                usercomp=strcmp(entereduser,users[accnum].username);
            }
        }

        if (usercomp == 0){ //If the username entered matches that of which is inside the user info file
            printf("\nEnter your password: ");
            scanf("%s",&enteredpass);

            passcomp=strcmp(enteredpass,users[accnum].password); //Comparing the user entered password with the password in the file that matches the entered username

            while(passcomp != 0){ //If the comparison shows the user entered password is incorrect
                printf("\nIncorrect, re-enter password: ");
                scanf("%s",&enteredpass);
                passcomp=strcmp(enteredpass,users[accnum].password);
            }

            if(passcomp == 0){ //If the comparison shows the user entered the correct password
                system("CLS");
                printf("Welcome back %s! (Account #%i)\n\n",users[accnum].fn,accnum+1); //Displaying the users first name, and displaying account number (accnum+1 because it starts at 0)
                *loginnum=accnum; //Will send the user number thats logged in back to the main via passing by reference
                BuyerMnu(productcount,accnum,users,groceries,carts);
            }
        }
    }
}

int MngMenu(int productcount, int managernum, char managernames[15][15], char managerpasses[15][15], groceryinfo groceries[]){
    int mngchoice=0;
    int choice=1;

    printf("Welcome back Manager %s\n",managernames[managernum]);

    while(choice == 1){
        printf("\nWhat would you like to do?\n\n1. Display Inventory (Sorted)\n2. Add Inventory\n3. Modify Inventory\n\n-> ");
        scanf("%i",&mngchoice);

        while(mngchoice > 3 || mngchoice <=0 ){
            printf("\nInvalid choice, try again\n-> ");
            scanf("%i",&mngchoice);
        }

        switch (mngchoice){ //These will make sure that the user is given the according code/function to what they selected as their choice
            case 1:
            DisplayInv(productcount, groceries);
            break;

            case 2:
            productcount=AddInv(productcount, groceries);
            break;

            case 3:
            EditInv(productcount, groceries);
            
            }

        system("PAUSE");
        system("CLS");

        printf("Would you like to perform any other managetorial actions?\n\n1. Yes\n2. No\n\n-> ");
        scanf("%i",&choice);

        if(choice == 2){
            system("PAUSE");
            system("CLS");
            printf("Goodbye!\n\n\n");
        }

        while(choice > 2 || choice <= 0){
            printf("\nInvalid input, try again\n-> ");
            scanf("%i",&choice);
        }
        system("CLS");
    }

    return productcount;
}


void DisplayInv(int productcount, groceryinfo groceries[]){
    int p=0;

    system("CLS");

    printf("Current Inventory:\n\n");
    for(p=0; p<productcount; p++){ //Will loop through until p reaches the value of how many products there are
        printf("Item #%i - ",p+1);
        printf("%s\n",groceries[p].item);
        printf("Supplier: %s\n",groceries[p].supplier);
        printf("Department: %s\n",groceries[p].department);
        printf("U. Measure: %s\n",groceries[p].unitmeasure);
        printf("Price: $%.2f\n",groceries[p].price);
        printf("Quantity: %i\n\n",groceries[p].quantity);
    }


}

void DisplayInvBuyers(int productcount, groceryinfo groceries[]){
    int p=0;

    system("CLS");
    printf("Here are our currently purchasable items:\n\n");
    system("PAUSE");
    system("CLS");

    for(p=0; p<productcount; p++){ //Will loop through until p reaches the value of how many products there are
        printf("Item: %s - ",groceries[p].item);
        printf("%s\n",groceries[p].supplier);
        printf("Department: %s\n",groceries[p].department);
        printf("Amount: %s\n",groceries[p].unitmeasure);
        printf("$%.2f ea\n",groceries[p].price);
        printf("In Stock: %i\n\n",groceries[p].quantity);
    }


}

int AddInv(int productcount, groceryinfo groceries[]){ //ISSUE
    int quant=0;

    system("CLS");

    printf("Adding New Item\n");

    printf("\nFill in the following\n\nItem Name: ");
    scanf("%s",&groceries[productcount].item);
    printf("\nSupplier: ");
    scanf("%s",&groceries[productcount].supplier);
    printf("\nDepartment: ");
    scanf("%s",&groceries[productcount].department);
    printf("\nUnit Measurement: ");
    scanf("%s",&groceries[productcount].unitmeasure);
    printf("\nExact Price: ");
    scanf("%.2f",&groceries[productcount].price);
    printf("\nQuantity Avaliable: ");
    scanf("%i",&quant);

    groceries[productcount].quantity=quant; //ISSUE

    printf("\n\nItem successfully added!\n\n");

    productcount=productcount+1; //The new number of existing products

    return productcount;

}

void EditInv(int productcount, groceryinfo groceries[]){


}

void BuyerMnu(int productcount, int loggedusernum, userinfo users[], groceryinfo groceries[], buyercart carts[]){
    int answer;

    system("PAUSE");
    system("CLS");
    printf("Buyer Menu:\t\t\t\t\t\t\t\t%s\n\n",users[loggedusernum].username);
    DisplayInvBuyers(productcount,groceries); //Will display the buyer version of the inventory so users can see what they can buy
    printf("\n");
    system("PAUSE");
    system("CLS");
    printf("Would you like to open a cart?\n\n1. Yes\n2. No\n\n-> ");
    scanf("%i",&answer);

    while(answer > 2 || answer <= 0){ //Error checking for possible invalid answer
        printf("\nInvalid answer, try again\n\n-> ");
        scanf("%i",&answer);
    }

    if(answer == 1){
        system("CLS");
        BuyerPurchases(productcount, loggedusernum, users, groceries, carts);
    }

    if(answer == 2){
        printf("Alright, thank you for using Taseen's Grocery Haven, goodbye!"); //There is no other option, program will be closed
        return 0;
    }

}

void BuyerPurchases(int productcount, int loggedusernum, userinfo users[], groceryinfo groceries[], buyercart carts[]){
    int m=0;
    int j=0;
    int t=0;
    int x=0;
    int v=0;
    int p=0;
    int entry=0;
    float price[50];
    int last=0;
    int amount[50];
    int addanother=1;
    float singlecost=0;
    float totalprice[50];
    FILE *fp;

    while(addanother == 1){ //This will loop until the user inputs a decision indicating they do not want to add any more products to the cart
        for(m=0; m<productcount; m++){ //Quick display of all the products
            printf("Item #%i: %s - %s ($%.2f/unit)\n",m+1,groceries[m].item,groceries[m].supplier,groceries[m].price);
        }

        printf("\nWhich item would you like to add to your cart (enter item #)\n-> ");
        scanf("%i",&entry);

        while(entry > productcount+1 || entry <= 0){ //Error checking so the user can't select a non existant item number
            printf("\nInvalid entry, try again\n-> ");
            scanf("%i",entry);
        }

        strcpy(carts[loggedusernum].username,users[loggedusernum].username); //Setting the accounts cart username the same as there account username
        strcpy(carts[loggedusernum].itemnames[j],groceries[entry-1].item);

        printf("\nQuantity Desired? (Max %i)\n-> ",groceries[entry-1].quantity);
        scanf("%i",&amount[j]);

        while(amount[j] > groceries[entry-1].quantity || amount[j] <= 0){
            printf("\nInvalid quantity, please try again\n-> ");
            scanf("%i",&amount[j]);
        }

        printf("\n%i %s successfully added to cart!\n\nWould you like to add another item?\n1. Yes\n2. No\n\n-> ",amount[j],carts[loggedusernum].itemnames[j]);
        scanf("%i",&addanother);

        while(addanother > 2 || addanother <=0){
            printf("\nInvalid entry, try again\n-> ");
            scanf("%i",addanother);
        }

        groceries[entry-1].quantity=groceries[entry-1].quantity-amount[j]; //This will take out the amount bought from the original inventory

        singlecost=groceries[entry-1].price;

        price[j]=amount[j]*singlecost;

        j++; //Incrementing j so all the uses of j for array value, will go up by one therefore filling the next value each time

        system("CLS");

    }

    printf("Your current cart holds:\n\n");
    for(t=0; t<j; t++){
        printf("%s (%i)\n",carts[loggedusernum].itemnames[t],amount[t]); //Display of selected items/amount
    }

    printf("\n\n");
    system("PAUSE");
    printf("\n\nWhat would you like to do next?\n\n1. Save to file\n2. Final Checkout\n\n-> ");
    scanf("%i",&last);

    while(last > 2 || last <=0 ){ //Error checking for invalid input
        printf("Invalid entry, please try again\n-> ");
        scanf("%i",&last);
    }

    for(x=0; x<j; x++){
        totalprice[loggedusernum]=totalprice[loggedusernum]+price[x]; //Obtaining the total price by continously adding up the prices of the selected products until all the prices have been added (x<j)
    }

    x=0; //Resetting x to 0

    if(last == 1){
        system("CLS");

        fp=fopen("carts.txt" , "w+");

        while(!feof(fp) && p<1){

            fprintf(fp,"%s",carts[loggedusernum].username);

            for(v=0; v<=j; v++){
                fprintf(fp,"\n%s",carts[loggedusernum].itemnames[v]);
            }

            fprintf(fp,"%.2f",totalprice[loggedusernum]);

            p++; //Stopping it
        }
        printf("File successfully updated!\n\nProceeding to checkout now...\n\n\n");
        system("PAUSE");

        last=2;

        fclose(fp);
    }

    if (last == 2){
        system("CLS");
        printf("Checkout Completed\n\n");
        system("PAUSE");
        system("CLS");
        printf("Taseen's Grocery Haven\n\nReceipt [%s %s]:\n\n",users[loggedusernum].fn,users[loggedusernum].ln);

        for(x=0; x<j; x++){
            printf("\t%s (%i) - $%.2f\n",carts[loggedusernum].itemnames[x],amount[x],price[x]);
        }

        printf("\n        Total Price - $%.2f\n\n\n",totalprice[loggedusernum]);
    }

    system("PAUSE");
    system("CLS");
    printf("Thank you for shopping at Taseen's Grocery Haven!\n\n\nGoodbye %s\n\n\n",users[loggedusernum].fn);

    return 0;

}

int Usercycle(userinfo users[]){

    int count=0;
    FILE *fp;


    fp=fopen("buyerlogin.txt" , "r+");
    while(!feof(fp)){
            fscanf(fp,"%s",users[count].fn);
            fscanf(fp,"%s",users[count].ln);
            fscanf(fp,"%s",users[count].email);
            fscanf(fp,"%s",users[count].username);
            fscanf(fp,"%s",users[count].password);
            count++;
        }
    fclose(fp);

    return count;
}

int Groceryread(groceryinfo groceries[]){

    int counter=0;
    char cost[50]; //This is where the value will be stored directly from the file, later which I'll switch into an integer.
    char quant[50]; //This is where the value will be stored directly from the file, later which I'll switch into an integer.
    float costt=0; //The scanned "cost" variable will be turned into an integer and the value will be put in this variable, then this variable shall go into the struct
    int quantt=0; //The scanned "cost" variable will be turned into an integer and the value will be put in this variable, then this variable shall go into the struct
    FILE *fp;

    fp=fopen("grocery.txt" , "r+");



    while(!feof(fp)){
        fscanf(fp,"%4095[^\n]\n",groceries[counter].item);
        fscanf(fp,"%4095[^\n]\n",groceries[counter].supplier);
        fscanf(fp,"%4095[^\n]\n",groceries[counter].department);
        fscanf(fp,"%4095[^\n]\n",groceries[counter].unitmeasure);
        fscanf(fp,"%4095[^\n]\n",cost);
        fscanf(fp,"%4095[^\n]\n",quant);

        quantt=atoi(quant); //Turning string into int
        groceries[counter].quantity=quantt;

        costt=atof(cost); //Turning string into float
        groceries[counter].price=costt;

        counter++;
        }

    fclose(fp);

    return counter;

}

void GrocerySort(int productcount, groceryinfo groceries[]){
    int x=0;
    int q=0;
    int i=0;
    int compareddep;
    int compareditem;
    groceryinfo temp;

    for(q=0; q<50; q++){
        for(x=0; x<productcount-1; x++){

            compareddep=strcmp(groceries[x].department,groceries[x+1].department);

            if(compareddep > 0){ //If department is out of order
                temp=groceries[x+1];
                groceries[x+1]=groceries[x];
                groceries[x]=temp;
            }

            if(compareddep == 0){ //If the department is the same
                compareditem=strcmp(groceries[x].item,groceries[x+1].item);

                if(compareditem > 0){ //If the first item should be placed after (alphabetically)
                temp=groceries[x+1];
                groceries[x+1]=groceries[x];
                groceries[x]=temp;
                }
            }
        }
    }
}

void Usercreate(int productcount, int usercount, userinfo users[], groceryinfo groceries[], buyercart carts[]){
    int choice;
    int t=0;
    FILE *fp;

    //Getting all the new user information and storing it in the latest array value of the struct users
    printf("New account info (Account #%i)\n\n",usercount+1);
    printf("First Name: ");
    scanf("%s",&users[usercount].fn);
    printf("\nLast Name: ");
    scanf("%s",&users[usercount].ln);
    printf("\nEmail Address: ");
    scanf("%s",&users[usercount].email);
    printf("\nEnter desired username: ");
    scanf("%s",&users[usercount].username);
    printf("\nEnter desired password: ");
    scanf("%s",&users[usercount].password);
    system("CLS");
    printf("* Account Successfully Created *\n");
    printf("\n\nYour created account will be uploaded to the database!\n\n\n\n");


    fp=fopen("buyerlogin.txt" , "w+"); //Rewriting the file to add the new user, and reprint the existing ones
    while(!feof(fp) && t <=usercount){
        for(t=0; t<=usercount; t++){
            fprintf(fp,"%s\n",users[t].fn);
            fprintf(fp,"%s\n",users[t].ln);
            fprintf(fp,"%s\n",users[t].email);
            fprintf(fp,"%s\n",users[t].username);
            fprintf(fp,"%s\n",users[t].password);
        }

    }

    system("PAUSE");
    system("CLS");

    printf("You will now be directed to the buyer menu.\n\nWelcome %s\n\n",users[usercount].fn);
    system("PAUSE");
    system("CLS");

    fclose(fp);

    BuyerMnu(productcount,usercount,users,groceries,carts); //After being logged into their next account, users will be sent to the Buyer Menu
}
