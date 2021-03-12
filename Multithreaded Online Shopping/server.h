//
//  server.h
//  Case
//
//  Created by Nicolas Rubert on 4/17/19.
//  Copyright © 2019 Nicolas Rubert. All rights reserved.
//

#ifndef server_h
#define server_h

// Bunch of includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

// Max accounts
#define SIZE 20

// struct to hold accounts
struct account
{
    char accName[40]; // Account name
    float balance; // User balance
    int inSession; // InSesssion flag
    
    // Mutex locker
    pthread_mutex_t lock;
};

// Creates an array of structs
struct account acc[SIZE];


// Product stuff
char serial[100][20]; // Serial number as a string
char price[100][20]; // Price of the product
char description[100][20]; // A brief discription of the prodcut

// Client info stuff
char add1[20][20]; // Address line 1
char add2[20][20]; // Address lien 2
char city[20][20]; // City
char state[20][20]; // State
char zip[20][20]; // Zip Code

// Struct to hold server info
struct servinfo
{
    int *new_sock; // sock number
    char *ip_addr; // ip address
    int portno; // port number
};

typedef struct servinfo *SERVINFO;

// Mutex locker
pthread_mutex_t nlock;

#endif
