//
//  session.c
//  Case
//
//  Created by Nicolas Rubert on 4/17/19.
//  Copyright © 2019 Nicolas Rubert. All rights reserved.
//

#include "server.h"
#include "functions.h"

char cart[20][6];
float total[20];
char precio[100][20];
int cartNum = 0;
char balance[20][20];

// prototyping
void menu(SERVINFO serv, int sock, int index);

// Transfer prices
void pre(){
    
    for(int i = 0; i < 100; i++){
        // Copies prices
        strcpy(precio[i], price[i]);
        // Prints the price for debugging
        //printf("The price is %s\n", precio[i]);
    }
    return;
}


// Searches for the item
int search(char *message){
    
    // Variables
    int found;
    // Debugging
	//printf("search : got \"%s\"",message);
    
    for(int i = 0; i < 100; i++){
        // Debugging
        //printf("\"%s\" =? \"%s\"",message,serial[i]);
        // Checks for product
        if(strcmp(message, serial[i]) == 0){
            
            if(strcmp(message,"h-6800")== 0){
                
                found = 100;
                // Returns product
                return found;
  
            }else{
                
                found = i;
                // Returns product
                return found;
            }
        }
        
    }
    // If unable to find the product
    printf("Unable to find item\n");
    
    
    return -1;
}

// Splits the space from the product
void split(char * input, char * output, char sep){
    // Variables
	int end,i;
    // Gets size
	const int m_size = strlen(input);
	char a[m_size];
    // Checks the characters
	for(i = 0; i < m_size;i++){
        // If found the remove
		if(input[i] == sep){end = i; a[i] = '\0'; break;}
		a[i] = input[i];
	}
    // Makes a new string
	for(i = 0; i < m_size-end;i++)
		output[i] = input[i+end+1];
	strcpy(input,a);
}

// Debugging stuff
void printIntVersion(char * a){
	for(int i = 0; i < strlen(a);i++)
		printf("%i of %i = %s",i,strlen(a),a[i]);
}

// Registers new account if not full or duplicate
void regi(SERVINFO serv, int sock){
    
    // Variables
    int isfound = 0, lock_ret;
    int i = 0;
    
    // Allocate memory for client message
    char *client_message = malloc(100);
    
    // Get the account name
    if(recv(sock, client_message, 100, 0) > 0)
    {
        // Check if there is enough space for new account
        for(i = 0; i < SIZE && isfound == 0; i++)
        {
            // Check for duplicate
            if(strcmp(acc[i].accName, client_message) == 0)
            {
                // If duplicate is found
                isfound = -1;
                
                write(sock, "Account is already registered.\n", 60);
                return;
            }
            // Check for empty space
            else if(strlen(acc[i].accName) == 0)
                isfound = 1;
        }
    }
    
    // Trylock the mutex
    lock_ret = pthread_mutex_trylock(&nlock);
    if(!lock_ret && isfound == 1){
        
        // Make new account
        strcpy(acc[i-1].accName, client_message);
        acc[i].balance = 0.0;
        acc[i].inSession = 0;
        
        // Gets address line 1
        write(sock, "Enter your adress line 1:\n", 50);
        recv(sock, add1[i-1], 50, 0);
        
        // Gets address line 2
        write(sock, "Enter your address line 2:\n", 50);
        recv(sock, add2[i-1], 50, 0);
        
        // Gets the city
        write(sock, "Enter your city (i.e. camden):\n", 50);
        recv(sock, city[i-1], 50, 0);
        
        // Gets the state
        write(sock, "Enter your state (i.e nj):\n", 50);
        recv(sock, state[i-1], 50, 0);
        
        // Gets the zipcode
        write(sock, "Enter your zipcode (i.e. 08004):\n", 50);
        recv(sock, zip[i-1], 50, 0);
        
        // Gets the balance
        write(sock, "Enter your balance:\n", 50);
        recv(sock, balance[i-1], 50, 0);
        
        // Sends message
        write(sock, "Account created.\n", 50);
        
        write(sock, "\n Options: [shop, exit]", 100);
        
        // Unlock mutex
        lock_ret = pthread_mutex_unlock(&nlock);
    }
    else{
        //puts("\n \x1b[31m[Error]\x1b[0m Account creation failed.");
        write(sock, "Cannot open account at this time...\n", 50);
    }
    
    // Free memory
    free(client_message);
}

// Login to existing account and make transactions
void curr_acc(SERVINFO serv, int sock){
    
    // Variables
    int isfound = 0, lock_ret, i;
    
    // Allocate memory for client message
    char *client_message = malloc(256);
    
    // Wait for the account name
    if((recv(sock, client_message, 256, 0)) > 0)
    {
        fflush(stdout);
    }
    
    // See if account name exists
    for(i = 0; i < SIZE && isfound != 1; i++)
    {
        // Checks if found
        if((strcmp(acc[i].accName, client_message)) == 0){
            // If found
            isfound = 1;
        }
    }
    
    // Trylock the thread
    lock_ret = pthread_mutex_trylock(&acc[i-1].lock);
    if(!lock_ret && isfound == 1)
    {
        // Free memory and tell client
        free(client_message);
        
        // Show customer menu
        write(sock, "\n Welcome! Options: [add, remove, show, empty, placeorder, catalog, done]\n", 100);
        // Menus can be used
        menu(serv, sock, i-1);
        
        // Unlock the thread
        lock_ret = pthread_mutex_unlock(&acc[i-1].lock);
    }
    // If the user is already in session
    else{
        
        write(sock, "Cannot login. Try again or wait...\n", 50);
        
        // Flush and free memory
        fflush(stdout);
        free(client_message);
    }
}

// Checks for empty cart space
int is_empty(int index){
    
    for(int i = 0; i < 6; i++){
        // Checks to see if cart has an empty space
        if(cart[index][i] == 0){
            // Debugging
            //printf("This is the place---->%d", i);
            
            return i;
            
        }
    }
    // Debugging if not found
    //printf("unable to find space\n");
    
    return 0;
    
}

// Add item to cart
void add(SERVINFO serv, int sock, int index)
{
    // Allocate space for memory
    char *client_message = malloc(256);
    
    if((recv(sock, client_message, 256, 0)) > 0)
    {
        // Calls to see if there is open space
        int basio = is_empty(index);
        
        pre();
        // Debugging
        //printf("\n\"%s\"\n", client_message);
    // Client message output after split
	char output[strlen(client_message)];
    // Calls Split function
	split(client_message,output,' ');
    // Debugging
//	if(strcmp(clientMessage,"") == 0)printIntVersion(output);
    // Debugging
	//printf("a = \"%s\" b = \"%s\"\n",client_message,output);
        // If the product was not found
        if(search(output) == -1){
            write(sock,"Unable to add item.\n",100);
        // If product found
        }else if(search(output) == 100){
            // Adds product to cart
            cart[index][basio] = 100;
            // Adds the product to the total
            total[index] += atof(precio[search(output)]);
            //printf("added  %f\n", total[index]);
            write(sock, "The item was added to the cart.\n", 100);
            
        }else{
            // Adds product to cart
            cart[index][basio] = search(output);
            // Adds the price to the total
            total[index] += atof(precio[search(output)]);
            // Debugging
            //printf("The totla is%f\n",total[index]);
            write(sock,"The item was added to the cart.\n", 100);
            
        }

    }
    
    // Resets the client message
    free(client_message);

return;

}

// Withdraw amount from account (needs work)
void Remove(SERVINFO serv, int sock, int index){
    // Allocates memory for the client response
    char *client_message = malloc(256);
    
    if((recv(sock, client_message, 256, 0)) > 0){
        // Debugging
        //printf("This is the length -->%lu",strlen(client_message));
        // Client message after split
        char output[strlen(client_message)];
        // Splits the client message
        split(client_message,output,' ');
        // Sees if the product exits
        int busca = search(output);
        
        if(busca != -1){
            
            for(int i = 0; i < 6; i ++){
                // See if the product is in the cart
                if(cart[index][i] == busca){
                    // Sets the cart space free
                    cart[index][i] = 0;
                    // Subtracts the price from the balance
                    total[index] -= atof(precio[search(output)]);
                    // Debugging
                    //printf("The total is%f\n",total[index]);
                    write(sock, "The item was removed from your cart.\n", 100);
                    
                    return;
                }
                
            }
            
        }
        
        // If item cant be removed
        write(sock, "Item was not removed.\n", 60);
    }
    
    // Clears the client's message
    free(client_message);
    return;

}

// show items in cart (needs work)
void show(SERVINFO serv, int sock, int index){
    
    for(int i = 0; i < 6; i++){
        // If cart is empty
        if(cart[index][0] == 0 && cart[index][1] == 0 && cart[index][2] == 0 && cart[index][3] == 0 && cart[index][4] == 0 && cart[index][5] ==0){
            // Prints if the cart is empty
            write(sock, "Your cart is empty.\n", 60);
        
        }else if(cart[index][i] == 100){
            // Prints the first item and the price
            printf("\nProduct number-->%s Product Price --->%s\n", serial[0], price[0]);

                
        }else if(cart[index][i] > 0){
            // Prints the prodcut and the price
            printf("\nProduct number-->%s Product Price --->%s\n", serial[cart[index][i]], price[cart[index][i]]);

        }
    
        
    }
    
    return;
}

// The user places order (needs work)
void placeorder(SERVINFO serv, int sock, int index){
    
        int new_b = atof(balance[index]) - total[index];
        
        // If the total is greater than the balance
        if( new_b < 0){
            
            write(sock, "Insufficient funds.\n", 50);
            
        }else{
            char *header = "---------ORDER---------";
            
            // prints header
            write(sock, header, 50);
            
            show(serv, sock, index);
            
            //sprintf(totally, "\nThe total is: %f\n", total[index]);
            
            printf("\nThe total is: %f\n", total[index]);
            //prints total
            //write(sock, totally, 55);
            
            total[index] = 0;
            
            printf("\nYour new balance is: %d\n", new_b);
            // things
            write(sock, "\nYour order will be shipped to: \n", 60);
            
            // prints name
            write(sock, acc[index].accName, 42);
            write(sock, "\n", 5);            // prints add 1
            write(sock, add1[index], 42);
            write(sock, "\n", 5);            // prints add 2
            write(sock, add2[index], 42);
            write(sock, "\n", 5);            // prints the city
            write(sock, city[index], 42);
            write(sock, "\n", 5);            // prints state
            write(sock, state[index], 3);
            write(sock, "\n", 5);            // prints zip
            write(sock, zip[index], 6);
            
            // prints the ending
            write(sock, "----------------------", 60);
            
            write(sock, "\n Goodbye! Options: [register, shop, exit]\n", 150);
        }
    
    
}

// Empties the user cart
void empty(SERVINFO serv, int sock, int index){
    
    for(int i = 0; i < 6; i++){
        // Checks to see what cart space is taken
        if(cart[index][i] != 0){
            // Sets the cart space to be free
            cart[index][i] = 0;
        }

    }
    
    write(sock, "Your cart has been emptied.\n", 45);
    
    write(sock, "\n Goodbye! Options: [register, shop, exit]\n", 150);

    return;
    
}

// Prints the catalog
void catalog(SERVINFO serv, int sock){
    
    printf("starting to print to the catalog\n");
    
    // Prints the whole catalog (for debugging)
    for(int j = 0; j < 100; j++){
        
        printf("%s\t", serial[j]);
        printf("%s\t", price[j]);
        printf("%s\n", description[j]);
        
    }
    
    return;
}

// Customer menu after successful login
void menu(SERVINFO serv, int sock, int index)
{
    char *client_message = malloc(20);
    
    // Account is in session
    acc[index].inSession = 1;
    
    // Wait for commands for the customer menu
    while(recv(sock, client_message, 30, 0) > 0)
    {
        // Adds an item
        if(strcmp(client_message, "add") == 0)
            add(serv, sock, index);
        // Removes an item
        else if(strcmp(client_message, "remove") == 0){
            Remove(serv, sock, index);
        // Shows the user's cart
        }else if(strcmp(client_message, "show") == 0){
            show(serv, sock, index);
        // Empties' user cart
        }else if(strcmp(client_message, "empty") == 0){
            // Done with the session
            empty(serv, sock, index);
            write(sock, "F", 1);
        // Places order
        }else if(strcmp(client_message, "placeorder") == 0) {
            // Done with the session
            placeorder(serv, sock, index);
            write(sock, "F", 1);
        // Shows the catalog of products
        }else if(strcmp(client_message, "catalog") == 0){
            catalog(serv, sock);
        // Done with session
        }else if(strcmp(client_message, "done") == 0)
        {
            // Done with the session
            write(sock, "F", 1);
            break;
        // Invalid input
        }else{
            write(sock, "Invalid input!\n", 50);
        
        // Clear the client message to get new message
        free(client_message);
        }
    }
    
    // Free memory
    free(client_message);
    // client session is returned to 0
    acc[index].inSession = 0;
}
