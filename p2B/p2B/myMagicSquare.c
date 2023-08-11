///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2022, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////
   
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      (name of all other files if any)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
    int size = 0;
    //prompt the user to enter the size 
    printf("Enter Magic squares size (odd integer >= 3)\n");
    scanf("%d", &size);

    // check if the size is valid 
    if(size < 3){
	printf("Magic square size must be >= 3\n");
	exit(1);
    }
    if(size % 2 == 0){
       printf("Magic square size must be odd\n");
       exit(1);
    }
    return size;   
} 
   
/* TODO:
 * Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {
    // dynamically allocate 2D board 
    int **square = malloc(sizeof(int*) * n);
    // check if square is NULL 
    if(square == NULL){
	printf("Error: not enought space to dynamically allocate\n");
	exit(1);
    }
    
    //dynamically allocating for each row 
    for(int i = 0; i < n; i++){
        *(square + i) = malloc(sizeof(int) * n);
	// check if each row is NULL
         if(*(square + i) == NULL){
         printf("Error: not enough space to dynamically allocate\n");
         exit(1);
        }
    }
    // initialize each value to be 0
    for(int a = 0; a < n; a++){
	for(int b = 0; b < n; b++){
	    *(*(square + a) + b) = 0;
	}
    }


    int counter = 0;
    int max = n*n;
    // starting positions of the first element of the square 
    int row = (n/2)+1;
    int col = n/2;
    // create the Magic Square 
    while(counter < max){
 	*(*(square + row) + col) = counter + 1;
	
	// calculate next position of the element
	// if next position is filled, move 2 spots down 
	// from original position
	int nextRow = (row + 1) % n;
	int nextCol = (col + 1) % n;
	if(*(*(square+nextRow) + nextCol) != 0){
	   nextRow = (row + 2) % n;
	   nextCol = col;
	}

	row = nextRow;
	col = nextCol;
	counter++;
	
    }
    // create the magic square 
    MagicSquare *magic = malloc(sizeof(MagicSquare));
    if(magic == NULL){
	printf("Error: not enough space to dynamically allocate\n");
    }
    magic->size = n;
    magic->magic_square = square;

    return magic;    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square_ptr the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square_ptr, char *filename) {
    // create file pointer to write to filename
    FILE *fptr = fopen(filename, "w");
    int size = magic_square_ptr->size;
    // check if fptr is NULL 
    if(fptr == NULL){
       printf("Error: file doesn't exist\n");
       exit(1); 
    }
    // write the size to the file 
    if(fprintf(fptr, "%d\n", size) < 0){
	 printf("Error: no characters\n");
	 exit(1);
    }
    
    // add each element to the file to create the magic square output
    int **square_ptr = magic_square_ptr->magic_square; 
    int num = 0;
    for(int i = 0; i < size; i++){
	num = *(*(square_ptr + i));
	// add first element in the row 
	if(fprintf(fptr, "%d", num) < 0){
         	printf("Error: no characters\n");
         	exit(1);
    	}
	// add the rest of the elements in the row 
	for(int j = 1; j < size; j++){
	    num = *(*(square_ptr + i) + j);

	     if(fprintf(fptr, ",%d", num) < 0){
                printf("Error: no characters\n");
                exit(1);
        	}
	}
	// go to the next row 
	fprintf(fptr, "\n");
    }

       // close the file 
       if(fclose(fptr) != 0){
           printf("Error: file didn't close properly\n");
           exit(1);
        }

    }

/* TODO:
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 *
 * argc: the number of command line args (CLAs)
 * argv: the CLA strings, includes the program name
 */
int main(int argc, char **argv) {
    // TODO: Check input arguments to get output filename
    if(argc != 2){
	printf("Usage: ./myMagicSquare <output_filename>\n");
	exit(1); 
    }
    char *file = *(argv+1);
    // TODO: Get magic square's size from user
    int size = getSize();
    // TODO: Generate the magic square
    MagicSquare *square_pt = generateMagicSquare(size);
    // TODO: Output the magic square
    fileOutputMagicSquare(square_pt, file);
    
    // free all the dynamically allocated memory
    int **matrix = square_pt -> magic_square; 
  	 for(int i = 0; i < size; i++){
	     free(*(matrix + i));
	     *(matrix + i) = NULL;
	 }

    free(matrix); 
    matrix = NULL; 
    free(square_pt);
    square_pt = NULL;

    return 0;
} 


//     F22 deppeler myMagicSquare.c      
