//
//  logtime.c
//  pa1
//
//  Created by Nicolas Rubert on 1/31/19.
//  Copyright © 2019 Nicolas Rubert. All rights reserved.
//

#include <stdio.h>

int main() {
    
    //Opens the txt file
    FILE *file = fopen("logtimes.txt", "r");
    
    //Varaibles
    int chr = fgetc(file); //Reads characters from the file
    int hour, min, num1, num2, lines, max_h, max_min; // Variables to store
    int sum_hours, sum_min, avg_min; // Variables to be used for math
    
    // Initializing all the variables
    hour = min = num1 = num2 = lines = max_h = max_min = 0;
    sum_hours = sum_min = avg_min = 0;
    
    //Loop to gather info from file
    while(chr != EOF)
    {
        chr =fgetc(file); // Reads each character
        fscanf(file, "%d:%d", &num1, &num2); // Scans the times
        
        // Finds the greatest hour and minute in the file
        if(num1 > max_h)
        {
            max_h = num1;
            if(num2 >= max_min)
            {
                max_min = num2;
            }
        }
        
        // Continues to add hour and minutes from each line
        hour += num1;
        min += num2;
        
        // Counts the amount of times logged to be used in the average
        if (chr == '\n')
        {
            lines ++;
        }
        chr = fgetc(file); // Reads each character
        
    }
    
    // Math operations to calculate the sum of all times
    sum_hours = ((min-100) + (hour * 60)) / 60;
    sum_min = ((min-100) + (hour *60)) % 60;
    
    // Math operation to calculate the average time
    avg_min = ((min - 100) + (hour * 60)) / lines;
    
    // Outputs in hhh:mm or hh:mm format
    printf("The sum is: %d:%d\n", sum_hours, sum_min); // sum
    printf("The avg is: 0%d:%d\n",(avg_min / 60), (avg_min % 60)); // avg
    printf("The longest time is: 0%d:0%d\n", max_h, max_min); // longest time
    
    // Closes file
    fclose(file);

    return 0;
}

