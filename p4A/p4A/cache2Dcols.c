////////////////////////////////////////////////////////////////////////////////
// Main File:        cache1D.c, cache2Drows.c, cache2Dcols.c, cache2Dclash.c
// This File:        cache2Dcols.c
// Other Files:      cache1D.c, cache2Drows.c, cache2Dclash.c
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

#define ROWS 3000
#define COLS 500

int arr2D[ROWS][COLS];
int main(){
    for(int col = 0; col < COLS; col++){
        for(int row = 0; row < ROWS; row++){
            arr2D[row][col] = row + col;
        }
    }
    return 0;
}
