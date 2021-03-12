//
//  client.c
//  Case
//
//  Created by Nicolas Rubert on 4/17/19.
//  Copyright © 2019 Nicolas Rubert. All rights reserved.
//

#include "client.h"
#include "functions.h"

// Global boolean
static char istrue = ' ';

//protoype
void serv_connect(Client client, char *ip_addr, int portno);


// Input method (Send input to server)
void *input_handler(void *client_sock)
{
    Client client = (Client) client_sock;
    char *temp;
    
    // Communicate with server
    while(1)
    {
        int i = 0;
        // Make enough space for command input
        client->command = malloc(10);
        client->arg = malloc(100);
        
        // Get a command input, split it, and make it lowercase
        printf(" :");
        scanf("%s%99[^\n]", client->command, client->arg);
        lower(client);
        
        // "register account name" Creates a new account
        if(strcmp(client->command, "register") == 0 || strcmp(client->command, "shop") == 0)
        {
            write(client->sock, client->command, 10);
            throttle(1);
            
            write(client->sock, client->arg, 100);
        }
        // CUSTOMER MENU
        else if(istrue == 'T')
        {
            // "shows" Shows user cart
            if(strcmp(client->command, "show") == 0){
                write(client->sock, client->command, 10);
                
            // "placeorder" Places the order on the items in the user's cart
            }else if(strcmp(client->command, "placeorder") == 0){
                write(client->sock, client->command, 12);
                istrue = 'F';
                
            // "empty" Log out of account and go back to main menu
            }else if(strcmp(client->command, "empty") == 0)
            {
                write(client->sock, client->command, 10);
                throttle(1);
                break;
            }
            // "add" Add an item to cart "Remove" removes item from cart
            else if(strcmp(client->command, "add") == 0 || strcmp(client->command, "remove") == 0)
            {
                write(client->sock, client->command, 10);
                write(client->sock, client->arg, 100);
                
                throttle(1);
                
            // "catalog" shows the list of products
            }else if(strcmp(client->command, "catalog") == 0){
                
                write(client->sock, client->command, 20);
            
            }else
                write(client->sock, client->command, 7);
        }
        // "exit" Disconnect from server and close client
        else if(strcmp(client->command, "exit") == 0)
        {
            write(client->sock, client->command, 10);
            istrue = 'F';
            throttle(1);
            break;
        }
        else
            write(client->sock, client->command, 10);
        
        // Free the string
        free(client->command);
        free(client->arg);
        
        // Throttle the speed
        throttle(2);
    }
    
    // Exit the thread
    pthread_exit(0);
}

// Message handler (Receive message from server)
void *message_handler(void *client_sock){
    // Server stuff
    const int responseSize = 256;
    Client client = (Client) client_sock;
    client->server_reply = malloc(sizeof(char) * responseSize);
    float read_size;
    int counter = 0;
    // Actively listen for messages from server
    while((read_size = recv(client->sock, client->server_reply, responseSize, 0)) > 0)
    {
        if(client->server_reply[0] == '\n')
        {
            istrue = 'T';
            printf(" %s\n", client->server_reply);
        }
        // User uses menu if not in session
        else if(*(client->server_reply) == 'F')
        {
            istrue = 'F';
            printf("\n Goodbye! Options: [register, shop, exit]\n");
        }
        else{
            char * serv = client->server_reply;
            serv+='\0';
            printf(" %s\n", serv);
        }
        counter++;
        // Erase the contents(debugging)
        //printf("\nsize of response = %lu\n",sizeof(client->server_reply));
        memset(client->server_reply, '\0', responseSize);
    }
    // Debugging
    //printf("counter = %i, final result = %d",counter,counter/3);
    free(client->server_reply);
    // If server has shutdown
    if(read_size == 0)
    {
        pthread_cancel(client->input_thread);
        puts("\n\n \x1b[31m[Diconnected]\x1b[0m Server has shutdown!\n");
    }
    
    // Exit the thread
    pthread_exit(0);
}

// Tries to connect
void serv_connect(Client client, char *ip_addr, int portno){
    
    // Socket Stuff
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    // Create socket
    client->sock = socket(AF_INET, SOCK_STREAM,0);
    
    // Checks to make sure no errors
    if(client->sock == -1){
        printf("Unable to create socket.\n");
        exit(0);
    }
    
    // Get ip address/domain
    server = gethostbyname(ip_addr);
    // If server(host) not found error message will appear
    if(server == NULL){
        printf("ERROR! There is no server with that name.\n");
        exit(0);
    }
    
    // Socket address stuff
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    // Tries to connect to server
    printf("Attempting to connect to server...\n");
    
    while(1){
        
        // Checks connection
        if(connect(client->sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
            
            printf("Trying...\n");
        
        // Prints menu
        }else{
            
            printf("\nConnection established to clamshell.rutgers.edu on port %d!\n",portno);
            printf("\nMenu:\n-----------");
            printf("\nregister    <account name>\t- Register new account.");
            printf("\nshop        <account name>\t- Start shopping seession.");
            //printf("\ncatalog                   \t- Prints the catalog of products.");
            //printf("\nadd         <item name>   \t- Add item to cart.");
            //printf("\nremove      <item name>   \t- Remove item from cart.");
            //printf("\nshow                      \t- Shows items in cart.");
            //printf("\nempty                     \t- Empties cart and ends customer session.");
            //printf("\nplaceorder                \t- Places order on items in cart.");
            //printf("\ndone                      \t- Ends your session.");
            printf("\nexit                      \t- Disconnect from server.\n\n\n");
            
            break;
            
        }
        
        // Tries to connect after 3 seconds
        throttle(3);
        
        
    }
    
    // Listen for messages
    if(pthread_create(&client->message_thread, NULL, message_handler, (void*)client) < 0){
        
        perror("ERROR! Unable to create thread");
        exit(0);
    }
    
    // Create input thread
    if(pthread_create(&client->input_thread, NULL, input_handler, (void *)client) < 0){
        
        perror("ERROR! Unable to create thread");
        exit(0);
    }
    
    // Join the client input thread
    pthread_join(client->input_thread, NULL);
    
    
}

// run program
int main(int argc, char *argv[])
{
    // Get port number and initialize client struct
    int portno = 4005;
    Client client = malloc(sizeof(struct client));
    
    // Try to connect
    serv_connect(client, "localhost", portno);
    
    // Close socket and free the memory
    close(client->sock);
    free(client->message);
    free(client->server_reply);
    free(client);
    
    return 0;
}
