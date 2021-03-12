//
//  functions.h
//  Case
//
//  Created by Nicolas Rubert on 4/17/19.
//  Copyright © 2019 Nicolas Rubert. All rights reserved.
//

#include "client.h"

// Make a string lowercase
void lower(Client client)
{
    int index = 0;
    
    // Loop through each character
    while(client->command[index])
    {
        // Makes it lowercase
        client->command[index] = tolower(client->command[index]);
        index++;
    }
    
    index = 0;
    while(client->arg[index])
    {
        client->arg[index] = tolower(client->arg[index]);
        index++;
    }
}

// Slows the command by seconds
void throttle(int sec)
{
    // If interrupted, go back to sleep
    if(sleep(sec) != 0)
        throttle(sec);
}




