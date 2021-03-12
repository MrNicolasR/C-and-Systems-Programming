//
//  mazesolver.c
//  MAZE
//
//  Created by Nicolas Rubert on 5/5/19.
//  Copyright © 2019 Nicolas Rubert. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Variables
int rows, columns; // Rows and Columns in file
int strtx, strty; // Starting Positions
int endx, endy; // End Positions
int currX, currY; // Current Positions

// Variables
char** maze; // Maze
char direction; // Direction
char fix; // Debugging

// Frees the memory that was allocated for the maze
void freeMaze(char** maze, int rows){
    // Goes through and frees the rows
    for(int i = 0; i < rows; i++){
        // Free maze characters by row
        free(maze[i]);
    }
    // Free the maze
    free(maze);
}

// Prints the maze
void printMaze(char** maze, int rows, int columns){
    // Goes through the rows
    for(int i = 0; i < rows; i++){
        // Goes through the columns
        for(int j = 0; j < columns; j++){
            // Prints the characters in the maze
            if(maze[i][j] == '+'){
                // If its the path print green
                printf("\033[22;31m%c\x1b[0m", maze[i][j]);
            }else{
                //prints normal
                printf("%c", maze[i][j]);
            }
        }
        // Prints a new line
        printf("\n");
    }
    
}

// Gets the file
void getFile(char* fileName){
    
    // File pointer
    FILE *file;
    
    // Opens the file in read mode
    file = fopen(fileName, "r");
    
    // Checks to see if there is errors
    if(file==NULL){
        // Prints error
        printf("ERROR: Cannot open the file\n");
        exit(1); // help check opening error
    }
    
    // Scan for dimensions, starting postions and ending positions
    fscanf(file," %d %d %d %d %d %d\n ", &rows, &columns, &strtx,
           &strty, &endx, &endy);
    
    
    printf("\n\nThe Maze has:\n Rows: %d, Columns: %d\n Starting X: = %d, Starting Y: %d\n Ending X: %d, Ending Y: %d\n\n",
           rows, columns, strtx, strty, endx, endy);
    
    // Allocates the memory for the maze
    maze = (char**)malloc(sizeof(char*) * rows);
    
    // If there is an error with the memory allocation
    if(!maze){
        // Prints error
        printf("ERROR: Memory allocation failed\n");
        exit(1);
    }
    
    // Mallocs the characters in the maze
    for (int i = 0; i < rows; i++){
        // Goes and allocates memory for each character
        maze[i] = (char*)malloc(sizeof(char) * columns);
    }
    
    // Goes through Rows
    for (int i = 0; i < rows; i++){
        // Goes through Columns
        for (int j = 0; j < columns; j++){
            // Scans the character and adds into maze
            fscanf(file,"%c",&maze[i][j]);
        }
        // Scans
        fscanf(file,"%c",&fix);
    }
    
    // Prints the maze
    printMaze(maze, rows, columns);
    printf("\n\n");
    
    printf("The idiots in the back are solving it....\n");
    
    printf("\n\n");
    // Closes the file
    fclose(file);
}


void navigate(){
    
    // Direction Definitions
    if (strtx  == 0)
    {
        direction = 'S';
    }
    else if (strtx == (rows - 1)){
        direction = 'N';
    }
    else if (strty == 0){
        direction = 'E';
    }
    else
    {
        direction = 'W';
    }
    
    
    // Direction Variable
    currX = strtx;
    currY = strty;
    
    // Starts navigating
    while(!((currX == endx) && (currY == endy))){
        // Head South
        if ( direction == 'S'){
            if ( (currY > 0) && (maze[currX][currY - 1]  == ' ' || maze[currX][currY - 1]  == '+')){
                maze[currX][currY] = '+';
                --currY;
                direction = 'W';
                
            }
            else if ( (currX < (rows - 1)) && (maze[currX + 1][currY] == ' ' || maze[currX + 1][currY] == '+')){
                maze[currX][currY] = '+';
                ++currX;
                direction = 'S';
            }
            else if ( (currY < (columns - 1)) && (maze[currX][currY + 1] == ' ' || maze[currX][currY + 1] == '+')){
                maze[currX][currY] = '+';
                ++currY;
                direction = 'E';
            }
            else {
                maze[currX][currY] = '+';
                --currX;
                direction = 'N';
            }
        }
        // Head North
        else if ( direction == 'N'){
            if ( (currY < (columns - 1)) && (maze[currX][currY + 1]  == ' ' || maze[currX][currY + 1]  == '+')){
                maze[currX][currY] = '+';
                ++currY;
                direction = 'E';
            }
            else if ( (currX > 0) && (maze[currX - 1][currY] == ' '|| maze[currX - 1][currY] == '+')){
                maze[currX][currY] = '+';
                --currX;
                direction = 'N';
            }
            else if ( (currY > 0) && (maze[currX][currY - 1] == ' ' || maze[currX][currY - 1] == '+')){
                maze[currX][currY] = '+';
                --currY;
                direction = 'W';
            }
            else {
                maze[currX][currY] = '+';
                currX++;
                direction = 'S';
            }
        }
        // Head West
        else if ( direction == 'W'){
            if ( (currX > 0) && (maze[currX - 1][currY]  == ' ' || maze[currX - 1][currY]  == '+')){
                maze[currX][currY] = '+';
                --currX;
                direction = 'N';
            }
            else if ( (currY > 0) && (maze[currX][currY - 1] == ' ' || maze[currX][currY - 1] =='+')){
                maze[currX][currY] = '+';
                --currY;
                direction = 'W';
            }
            else if ( (currX < (rows - 1)) && (maze[currX + 1][currY] == ' ' || maze[currX + 1][currY] =='+')){
                maze[currX][currY] = '+';
                ++currX;
                direction = 'S';
            }
            else {
                maze[currX][currY] = '#';
                currY++;
                direction = 'E';
            }
        }
        // Head East
        else {
            if ( (currX < (rows - 1)) && (maze[currX + 1][currY]  == ' ' || maze[currX + 1][currY]  =='+')){
                maze[currX][currY] = '+';
                ++currX;
                direction = 'S';
            }
            else if ( (currY < (columns - 1)) && (maze[currX][currY + 1] == ' ' || maze[currX][currY + 1] =='+')){
                maze[currX][currY] = '+';
                ++currY;
                direction = 'E';
            }
            else if ( (currX > 0) && (maze[currX - 1][currY] == ' ' || maze[currX - 1][currY] =='+')){
                maze[currX][currY] = '+';
                --currX;
                direction = 'N';
            }
            else {
                maze[currX][currY] = '+';
                currY--;
                direction = 'W';
            }
        }
        
    }
    maze[currX][currY] = '+';
    
    // Prints the maze
    printMaze(maze, rows, columns);
    
    // printf("test");
    printf("\n");
    
    // If navigated
    if((currX == endx) && (currY == endy)){
        // If done
        printf("The maze has been solved\n\n");
        
    }
    
    freeMaze(maze, rows);
    
    return;
    
}

int main(){
    
    char input[1000];
    
    printf("Enter the maze you want to solve (with the extension i.e. txt): \n");
    scanf("%s", input);
    
    getFile(input);
    navigate();
    
    
    
    
    return 0;
}

