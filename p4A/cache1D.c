////////////////////////////////////////////////////////////////////////////////
// Main File:        cache1D.c
// This File:        cache2Drows, cache2Dcols.c, cache2Dclash.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Fall 2022
// Instructor:       deppeler
//
// Author:           Ayush Mahapatra
// Email:            mahapatra3@wisc.edu
// CS Login:         amahapatra
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   including Peer Mentors, Instructors, and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define GLOBAL_N 100000
int array[GLOBAL_N];
void main(){
    for(int i = 0; i < GLOBAL_N; i++){
        array[i] = i;
    }
}