////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c, division.c 
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
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Global variable representing the number of seconds between every alarm 
const int seconds = 4;

// Global variabel keeping track of how many times SIGUSR1 signal was handled
int countUser1 = 0;

/*
 * Handler that deals with SIGARLM signals
 */
void handler_SIGALRM(){
    
    // Get the pid
    pid_t pid = getpid();
    
    // Find the current time and date
    time_t currentTime;

    // Check if current time is valid
    if(time(&currentTime) == -1){
	printf("Error: invalid time\n");
	exit(1);
    }
    
    // Convert the time (seconds) into the current date
    char* date = ctime(&currentTime);

    // Check if the date is valid 
    if(date == NULL){
	printf("Error: invalid data\n");
	exit(1);
    }
    
    // Print out the pid and the current date
    printf("PID: %d CURRENT TIME:%s", pid, ctime(&currentTime));

    // Trigger another alarm
    alarm(seconds); 
}

/*
 * Handles SIGUSR1 signals
 */
void handler_SIGUSR1(){

    // SIGUSR1 signal handled 
    countUser1++;
    printf("SIGUSR1 handled and counted!\n");
}


/*
 * Handles SIGINT signal
 * When user does ctrl-c command 
 */
void handler_SIGINT(){
    // Handles SIGINT signal and print out
    // how many times SIGUSR1 signals were handled
    printf("\nSIGINT handled.\n");
    printf("SIGUSR1 was handled %d times. Exiting now.\n", countUser1);
    exit(0);
}

/*
 * prints out the PID and current date every 4 seconds
 */
int main(){
    printf("PID and time print every 4 seconds.\n");
    printf("Type Ctrl-C to end the program.\n");
	
    // Trigger alarm
    alarm(seconds);

    // Create sigaction struct for SIGALRM
    struct sigaction alarm;
    memset(&alarm, 0, sizeof(alarm));

    // Function pointer to handler 
    alarm.sa_handler = handler_SIGALRM;
    
    // Check if sigaction is successful or not
    if(sigaction(SIGALRM, &alarm, NULL) != 0){
	printf("Error: binding SIGALRM Handler\n");
	exit(1);
    }

    // Create sigaction struct for SIGUSR1
    struct sigaction user1;
    memset(&user1, 0, sizeof(user1));

    // Function pointer to handler 
    user1.sa_handler = handler_SIGUSR1;

    // Check if sigaction is successful or not
    if(sigaction(SIGUSR1, &user1, NULL) != 0){
      printf("Error: binding SIGUSR1 Handler\n");
      exit(1);
    }

    // Create sigaction struct for SIGINT
    struct sigaction int_ctrl_c;
    memset(&int_ctrl_c, 0, sizeof(int_ctrl_c));

    // Function pointer to handler
    int_ctrl_c.sa_handler = handler_SIGINT;

    // Check if sigaction is successful or not
    if(sigaction(SIGINT, &int_ctrl_c, NULL) != 0){
	printf("Error: binding SIGINT Handler\n");
	exit(1);
    }

    // infinite loop
    while(1){
    }

    return 0;
}
