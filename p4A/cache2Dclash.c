////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dclash.c
// This File:        cache1D.c, cache2Drows.c, cache2Dcols.c
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

#define MAX 100
#define ROWS 128 
#define COLS 8

int arr2D[ROWS][COLS];
void main(){
    for(int iteration = 0; iteration < MAX; iteration++){
        for(int row = 0; row < ROWS; row += 64){
            for(int col = 0; col < COLS; col++){
                arr2D[row][col] = iteration + row + col;
            }
        }
    }
}

