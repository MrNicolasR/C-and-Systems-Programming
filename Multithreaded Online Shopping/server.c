//
//  server.c
//  Case
//
//  Created by Nicolas Rubert on 4/17/19.
//  Copyright © 2019 Nicolas Rubert. All rights reserved.
//

#include "server.h"
#include "session.c"

// Protypes
void server_setup(void);
void *serverListen(void *);
void *clientBridge(void *);
void *alarma(void *);
void printInfo(int);

// Gets product catalogs
int getCat(){
    
    FILE *fp;
    
    fp = fopen("catalog.txt", "r");
    
    // If file is empty
    if(fp == NULL){
        
        printf("error");
        return -1;
    }
    
    // Scans the file
    while(fgetc(fp) != EOF){
        
        for(int i = 0; i < 100; i++){
            
            fscanf(fp, "%s %s %[^\n]\n", serial[i], price[i], description[i]);
            
        }
        
    }
    //printf("starting to add data to pointer\n");
    
    // Prints the whole catalog (for debugging)
    //for(int j = 0; j < 100; j++){

        //printf("%s\t", serial[j]);
        //printf("%f\t", atof(price[j]));
        //printf("%s\n", description[j]);
        
        
        
    //}
    
    return 0;
}


// Sets up Server (needs work)
void server_setup()
{
    // Declare socket variables
    int sd;
    int portnum = 4005;
    struct sockaddr_in server;
    pthread_t serv_listen, info;
    
    // Create socket and check if able to create
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1)
    {
        puts(" \x1b[31m[Error]\x1b[0m Could not create socket."); // Error message in red
        exit(0);
    }
    
    // Prepare sockaddr structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portnum);
    
    // Bind the port
    if(bind(sd, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        // Print error message
        perror("\n \x1b[31m[Error]\x1b[0m Bind");
        printf("\n");
        exit(0);
    }
    
    // Listen
    printf("\n Listening on port %d...", portnum);
    fflush(stdout);
    listen(sd, 3);
    
    // Create server acceptor thread
    if(pthread_create(&serv_listen, NULL, serverListen, (void *) &sd) < 0)
    {
        perror(" \x1b[31m[Error]\x1b[0m Listen thread creation");
        exit(0);
    }
    
    // Create server info thread
    if(pthread_create(&info, NULL, alarma, NULL) < 0)
    {
        perror(" \x1b[31m[Error]\x1b[0m Info thread creation");
        exit(0);
    }
    
    // Wait for acceptor thread
    pthread_join(serv_listen, NULL);

    
    
    
}
// Server listen for incoming connections
void *serverListen(void *sd)
{
    // Struct to hold server info
    SERVINFO serv;
    
    // Set up to listen
    int cs; //client sock
    int sock = *((int *) sd); // socket to write and recieve
    struct sockaddr_in client;
    int c = sizeof(struct sockaddr_in);
    
    // Accept connection from an incoming cient (changed this)
    while((cs = accept(sock, (struct sockaddr *) &client, (socklen_t *) &c)) != -1)
    {
        // Connection accepted, create struct to hold info
        serv = malloc(sizeof(struct servinfo));
        serv->ip_addr = inet_ntoa(client.sin_addr); // gets the ip addr
        serv->portno = (int) ntohs(client.sin_port); // gets the port num
        printf("\n \x1b[32m[Connected]\x1b[0m %s:%d\n", serv->ip_addr, serv->portno);
        
        // Create new client thread
        pthread_t client_thread;
        serv->new_sock = malloc(1);
        *(serv->new_sock) = cs;
        
        // Check the thread
        if(pthread_create(&client_thread, NULL, clientBridge, (void *) serv) < 0)
            perror(" \x1b[31m[Error]\x1b[0m Client thread creation\n");
    }
    
    if(cs < 0){
        perror(" \x1b[31m[Error]\x1b[0m Accept");
    }
    
    return 0;
}

// Accept incoming commands
void *clientBridge(void *servinfo)
{
    // Convert void struct to a server info struct
    SERVINFO serv = (SERVINFO) servinfo;
    
    // Prepare to receive and send
    int sock = *(serv->new_sock);
    int read_size, index;
    char *client_message = malloc(25);
    
    // Receive message from client
    while((read_size = recv(sock, client_message, 25, 0)) > 0)
    {
        
        // Print info about input from client
        fflush(stdout);
        
        // "register" Create new account
        if(strcmp(client_message, "register") == 0)
            regi(serv, sock);
        // "shop" Login to account
        else if(strcmp(client_message, "shop") == 0)
            curr_acc(serv, sock);
        
        // "exit" Client wants to disconnect
        else if(strcmp(client_message, "exit") == 0)
        {
            write(sock, "Disconnected from server.", 50);
            break;
        }
        // INVALID INPUTS
        else
        {
            write(sock, "Invalid input!", 50);
        }
        
        // Clear the client message to get new message
        memset(client_message, 0, sizeof(client_message));
    }
    
    // Disconnect if no message can come through from that client
    if(read_size >= 0)
    {
        printf(" \x1b[31m[Disconnected]\x1b[0m %s:%d\n", serv->ip_addr, serv->portno);
        fflush(stdout);
    }
    else if(read_size == -1)
        perror(" \x1b[31m[Error]\x1b[0m Receive");
    
    // Free the socket pointer
    free(servinfo);
    free(client_message);
    
    // Exit the thread
    pthread_exit(0);
}

// Setup alarm and signal
void *alarma(void *arg)
{
    // Create signal
    signal(SIGALRM, printInfo);
    
    // Set alarm
    alarm(20);
    
    // Keep going
    while(&free)
        pause();
    
    // Exit thread
    pthread_exit(0);
}

// Print client info every 25 seconds (Gets annoying)
void printInfo(int signum)
{
    int index, isfound = 0;
    printf("\n Server Info");
    printf("\n ------------");
    
    // Print info
    for(index = 0; index < SIZE; index++)
    {
        // Print
        if(strlen(acc[index].accName) != 0)
        {
            printf("\n Account:%s\n %s: ",
                   acc[index].accName, "Session");
            
            // Check inSession
            if(acc[index].inSession == 1)
                printf("IN SERVICE\n");
            else
                printf("NOT IN SERVICE\n");
            
            isfound++;
        }
    }
    
    if(isfound == 0)
        printf("\n No accounts found");
    
    printf("\n");
    
    // Reset alarm for 25 seconds
    alarm(25);
}


int main()
{
    int i;    // Loop counter variable
    
    // Variable that runs the get catalog function
    int a = getCat();
    
    // Checks to see if there is an error
    if(a == -1){
        printf("ERROR\n");
        exit(0);
    }else{
        printf("The file has been loaded.\n");
    }

    // Initialize mutex for new & current customers
    pthread_mutex_init(&nlock, NULL);
    for(i = 0; i < SIZE; i++)
        pthread_mutex_init(&acc[i].lock, NULL);
    
    // Setup and listen for connections
    server_setup();
    
    // End mutex
    for(i = 0; i < SIZE; i++)
        pthread_mutex_destroy(&acc[i].lock);
    
    return 0;
}
