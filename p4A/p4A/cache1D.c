////////////////////////////////////////////////////////////////////////////////
// Main File:        cache1D.c, cache2Drows.c, cache2Dcols.c, cache2clash.c
// This File:        cache1D.c
// Other Files:      cache2Drows.c, cache2Dcols.c, cache2Dclash.c
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
// Persons:          None
//                   
//
// Online sources:   None
//                   
//                
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define GLOBAL_N 100000
int array[GLOBAL_N];
int main(){
    for(int i = 0; i < GLOBAL_N; i++){
        array[i] = i;
    }
    return 0;
}
