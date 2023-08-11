////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      sendsig.c, mySigHandler.c
// Semester:         CS 354 Lecture 001 Fall 2022
// Instructor:       deppeler
// 
// Author:           Ayush Mahapatra
// Email:            mahapatra3@wisc.edu
// CS Login:         amahapatra
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          None
//                   
//
// Online sources:   None
//                    
//                   
//////////////////////////// 80 columns wide ///////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission for Fall 2022
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

// Max size of input 
#define BUFFER 100

// Global variable keeping track of successful divison operations
int div_count = 0;

/*
 * Handler that ends program 
 * When the user inputs 0 as
 * The second integer
 */
void handler_SIGFPE(){
   
    // Prints out number of successful division operations
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", div_count);
    printf("The program will be terminated\n");
    exit(0);
}
/*
 * Handler that ends program when user inputs "ctrl-c"
 */
void handler_SIGINT(){
    
    // Prints out number of successful division operations
    printf("\nTotal number of operations completed successfully: %d\n", div_count);
    printf("The program will be terminated\n");
    exit(0);
}

/*
 * prompts the user to ask for 2 integers to divide 
 */
int main(){

    // Create struct for sigaction SIGFPE
    struct sigaction divide_by_zero;
    memset(&divide_by_zero, 0, sizeof(divide_by_zero));

    //Function pointer to handler
    divide_by_zero.sa_handler = handler_SIGFPE;

    // Check if sigaction was successful or not
    if(sigaction(SIGFPE, &divide_by_zero, NULL) != 0){
        printf("Error: binding SIGFPE Handler\n");
        exit(1);
    }
    
    // Create struct for sigaction SIGFPE
    struct sigaction int_ctrl_c;
    memset(&int_ctrl_c, 0, sizeof(int_ctrl_c));

    //Function pointer to handler
    int_ctrl_c.sa_handler = handler_SIGINT;

    // Check if sigaction was successful or not
    if(sigaction(SIGINT, &int_ctrl_c, NULL) != 0){
        printf("Error: binding SIGINT Handler\n");
        exit(1);
    }
    



    while(1){
        char num[BUFFER];
        char denom[BUFFER];
	
	// Ask the user for the first integer and check 
	// If it is an integer
        printf("Enter first integer: ");

        if(fgets(num, BUFFER, stdin) == NULL){
	    printf("Error!\n");
	    exit(1);
        }
	
	// Ask the user for the second integer and check
	// If it is an integer
	printf("Enter second integer: ");

        if(fgets(denom, BUFFER, stdin) == NULL){
	    printf("Errr!\n");
	    exit(1);
        }

	int firstInt = atoi(num);
	int secInt = atoi(denom);
	
	
	printf("%d / %d is %d with a remainder of %d\n", firstInt, secInt, firstInt/secInt, firstInt % secInt);

	// Divisiom operation successful
	div_count++;

    }


}
