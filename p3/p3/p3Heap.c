///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2022 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2022, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Main File:        p3Heap.c
// This File:        p3Heap.c
// Other Files:      p3Heap.h
// Semester:         CS 354 Lecture 001 Fall 2022
// Instructor:       deppeler
// 
// Author:           Ayush Mahapatra
// Email:            mahapatra3@wisc.edu
// CS Login:         amahapatra
//
/////////////////////////// OTHER SOURCES OF HELP /////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          None
//                   
//
// Online sources:   None
//                    
//                   
//////////////////////////// 80 columns wide //////////////////////////////////

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "p3Heap.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

    int size_status;

    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int alloc_size;

/*
 * Additional global variables may be added as needed below
 */

 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 * - If a BEST-FIT block found is NOT found, return NULL
 *   Return NULL unable to find and allocate block for desired size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* balloc(int size) {
   
    // check if the size is valid
    if(size < 0 || size > alloc_size) return NULL;
    
    // add the size of the header and necessary padding
    int totalSize = size + sizeof(blockHeader);

    if(totalSize % 8 != 0){
	    int padding = 8 - (totalSize % 8);
	    totalSize += padding;
    }
    
    // pointer to the start of the heap
    blockHeader *start = heap_start;

    // pointer to the block that is larger than 
    // the size needed to allocated
    blockHeader *closestFitptr = start;

    // size_status of the current block
    int hdrSize = start -> size_status;

    // size of the current block (a multiple of 8)
    int currBlockSize = hdrSize & ~(3);

    // size of the closest fit block
    int closestFit = alloc_size + 1;
    
    // traverse through the heap until we reach the end
    while(start -> size_status != 1){
        hdrSize = start -> size_status;
        currBlockSize = hdrSize & ~(3);
	
	// check if block is free
        if(!(hdrSize & 1)){
	    
	    // if current block size is exact, then allocate that block
            if(currBlockSize == totalSize){
                start -> size_status += 1;
		
		// change the p-bit of the next block
		// if the next block isn't the end block
		if((start + (currBlockSize / sizeof(blockHeader))) -> size_status != 1){
                    (start + (currBlockSize / sizeof(blockHeader))) -> size_status += 2;
		}

		// return the payload address
                return start + 1;
	      
	      // check for the closest fit block
            } else if((currBlockSize > totalSize) && (currBlockSize < closestFit)){
                closestFitptr = start;
                closestFit = currBlockSize;
            }
	    // move to the next block
            start += (currBlockSize / sizeof(blockHeader));
        } else {
	    // move to the next block since current block
	    // is already allocated 
            start += (currBlockSize / sizeof(blockHeader));
        }
    }
    
    // no free block has been found to split
    if(closestFit == alloc_size + 1){
        return NULL;
    }
    
    // split the free block into
    // one allocated and one free block

    // size of the new free block
    int newSizeStatus = closestFit - totalSize;
    
    // change the size of the free block and allocate it 
    closestFitptr -> size_status = totalSize + (closestFitptr -> size_status %8);
    closestFitptr -> size_status += 1;

    // change the p-bit of the next block
    blockHeader *newFreeBlock = closestFitptr + (totalSize / sizeof(blockHeader));
    newFreeBlock -> size_status = newSizeStatus + 2;

    // return the payload
    return closestFitptr + 1;
} 
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int bfree(void *ptr) {    
    
    // get the address of the header 
    blockHeader *newPtr = (blockHeader*)((char*) ptr) - 1;
    
    // get the size of the block
    int newPtrSize = (newPtr -> size_status) & ~(3);
    
    // check if the pointer is NULL
    if(newPtr == NULL){
        return -1;
    }
    
    // check if the ptr is within the bounds of the heap
    if(((int)ptr < (int)heap_start) || ((int)ptr > ((int)heap_start + alloc_size))){
        return -1;
    }
    
    // check if the ptr is a multiple of 8 
    if(((int)ptr - (int)heap_start - sizeof(blockHeader)) % 8 != 0){
        return -1;
    }
    
    // check if the ptr block has already been freed
    if(((newPtr -> size_status) & 1) == 0){
        return -1; 
    }
    
    // allocate the ptr block 
    newPtr -> size_status -= 1;

    // change the p-bit of the next block if the next block
    // isn't the end block
    blockHeader *nextPtr = newPtr + ((newPtrSize)/sizeof(blockHeader));
    if(nextPtr -> size_status != 1){
        nextPtr -> size_status -= 2;
    }
    return 0;
} 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for delayed coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
    // pointer to the start of the heap
    blockHeader *start = heap_start; 
    int currentBlockSize = (start -> size_status & ~(3));

    // pointer to the next block
    blockHeader *nextBlock = start + (currentBlockSize/sizeof(blockHeader));
    int nextBlockSize = (nextBlock -> size_status & ~(3));

    // keeps track of whether coalesce happened
    int coalesceHappened = 0; 
    
    // traverse through the heap 
    while(start -> size_status != 1){
	
	// move on if block is allocated
        if((start -> size_status) & 1){
            start += ((currentBlockSize)/sizeof(blockHeader));
            nextBlock += ((nextBlockSize)/sizeof(blockHeader)); 
            currentBlockSize = (start -> size_status & ~(3));
            nextBlockSize = (nextBlock -> size_status & ~(3));
        }else{
	    // if next block is allocated move
	    // to the next block
            if((nextBlock -> size_status) & 1) {
                start += ((currentBlockSize)/sizeof(blockHeader));
                nextBlock += ((nextBlockSize)/sizeof(blockHeader)); 
                currentBlockSize = (start -> size_status & ~(3));
                nextBlockSize = (nextBlock -> size_status & ~(3));
            }else{
		// coalesce the two blocks
                start -> size_status += nextBlockSize;
		currentBlockSize += nextBlockSize;

		// coalesce happened
                coalesceHappened = 1;

		// move the pointer of the next block
		// and update the size
                nextBlock += (nextBlockSize/sizeof(blockHeader));
		nextBlockSize = (nextBlock -> size_status & ~(3));
            }

        }
    }
	return coalesceHappened;
}

 
/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int init_heap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int pagesize;   // page size
    int padsize;    // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* end_mark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heap_start = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    end_mark = (blockHeader*)((void*)heap_start + alloc_size);
    end_mark->size_status = 1;

    // Set size in header
    heap_start->size_status = alloc_size;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heap_start->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heap_start + alloc_size - 4);
    footer->size_status = alloc_size;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void disp_heap() {     
 
    int counter;
    char status[6];
    char p_status[6];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heap_start;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, 
	"*********************************** Block List **********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
	"*********************************************************************************\n");
    fflush(stdout);

    return;  
} 


// end of myHeap.c (Spring 2022)                                         


