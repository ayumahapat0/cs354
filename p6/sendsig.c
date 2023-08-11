////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c division.c
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

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define SIGINT 2
#define SIGUSR1 10

/* Sends SIGINT and SIGUSR1 signals to the mySigHandler.c
 *
 * argc: the number of command line args (CLAs)
 * argv: the CLA strings, includes the program name
 */
int main(int argc, char* argv[]){
    
    // Checks if correct number of CLAs were used
    if (argc != 3){
	printf("Usage: <signal type> <pid>\n");
	exit(1);
    }

    // Checks is the second CLA was a SIGUSR1 or SIGINT 
    if(!((strcmp(argv[1], "-u") == 0) || (strcmp(argv[1], "-i") == 0))){
	printf("Usage: <signal type> <pid>\n");
	exit(1);
    }

    // Checks if the PID given is valid
    if(atoi(argv[2]) <= 0){
	printf("Error: invalid pid\n");
	exit(1);
    }

    pid_t pid = atoi(argv[2]);

    if(strcmp(argv[1], "-u") == 0){
	
	// Checks if kill command was successful
	if(kill(pid, SIGUSR1) != 0){
	    printf("Error!\n");
	    exit(1);
	}
    }

    if(strcmp(argv[1], "-i") == 0){

	// Checks if kill command was successful
	if(kill(pid, SIGINT) != 0){
	    printf("Error!\n");
	    exit(1); 
	}
    }


}
