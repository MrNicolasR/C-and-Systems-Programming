//
//  client.h
//  Case
//
//  Created by Nicolas Rubert on 4/17/19.
//  Copyright © 2019 Nicolas Rubert. All rights reserved.
//

#ifndef client_h
#define client_h

// Bunch of includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

// Struct to hold client info
struct client
{
    int sock; // Sock number
    char *message, // Message
    *command, // Command
    *arg; // Arguments
    char *server_reply; // Server Reply
    
    // Input and message threads
    pthread_t input_thread;
    pthread_t message_thread;
};

typedef struct client *Client;

#endif /* client_h */
