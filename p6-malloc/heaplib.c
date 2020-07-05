#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include "spinlock.h"


/* 
 * Global lock object.  You should use this global lock for any locking you need to do.
 */
#ifdef __riscv
volatile lock_t malloc_lock = {.riscv_lock = 0};
#else
volatile lock_t malloc_lock = {.pthread_lock = PTHREAD_MUTEX_INITIALIZER};
#endif

//Types
typedef struct _block_info {
    unsigned int block_size;
	bool use;
} block_info;

typedef struct _heap_header {
	unsigned int heap_size;
} heap_header ;



// Pointer to char before adding
#define BYTE_ADDER(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))


/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * Add padding at the beginning of the heap. We wish to align the
 * the first 8-byte block. Heap initialized to a single free block of 
 * (size heap_size - size_heap_header - size_block_header - size_padding)
 */
int hl_init(void *heap, unsigned int heap_size){
    mutex_lock(&malloc_lock);
    if(heap_size<MIN_HEAP_SIZE){
        mutex_unlock(&malloc_lock);
        return FAILURE;
    }
    char *beg = BYTE_ADDER(heap, sizeof(heap_header) + sizeof(block_info));
    
    int heap_pad = (ALIGNMENT - ((uintptr_t)beg % ALIGNMENT)) % ALIGNMENT; 
    heap_header *header = (heap_header *)(BYTE_ADDER(heap, heap_pad));
    header->heap_size = heap_size;
    
	block_info *first_block = (block_info *) BYTE_ADDER(header, sizeof(heap_header)); 
    first_block->block_size = heap_size - sizeof(heap_header) - sizeof(block_info) - heap_pad;
    first_block->use = false;
    mutex_unlock(&malloc_lock);
    return SUCCESS;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 * 
 * Iterate completely through the heap, return the first block which is free. 
 * Then allocate new blocks, making sure the new block is big enough. 
 * Allocate requested space, new block from the remainder of the space. 
 * No space for header --> entire block assigned.  
 * Align, pad at each step
 */
void *hl_alloc(void *heap, unsigned int block_size) {
    mutex_lock(&malloc_lock);
    char *beg = BYTE_ADDER(heap, sizeof(heap_header) + sizeof(block_info));
    
    int heap_pad = (ALIGNMENT - ((uintptr_t)beg % ALIGNMENT)) % ALIGNMENT;

    heap_header *header = (heap_header *)(BYTE_ADDER(heap, heap_pad));

    // Block One
    block_info *block_in_use = (block_info *) BYTE_ADDER(header, sizeof(heap_header));
    int offset; 
    for (offset = heap_pad+sizeof(heap_header); offset < header->heap_size;) {
	    unsigned int this_size = block_in_use->block_size;
        int total_block_size = block_size + sizeof(block_info); 
        int pad = (ALIGNMENT - (total_block_size % ALIGNMENT)) % ALIGNMENT;
        total_block_size = total_block_size + pad;
        
        if (block_in_use->use != true && total_block_size <= sizeof(block_info) + this_size) {
            
            block_in_use->block_size = block_size + pad;
            block_in_use->use = true;
            
            int remainder = this_size - total_block_size + sizeof(block_info); 
            if(sizeof(block_info)>remainder) {
                block_in_use->block_size += remainder;
            }
            else{
                block_info *new_blk = (block_info *) BYTE_ADDER(block_in_use, total_block_size); 
                new_blk->block_size = this_size - total_block_size;
                new_blk->use = false;
            }
            mutex_unlock(&malloc_lock);
            return BYTE_ADDER(block_in_use, sizeof(block_info));
           
        }
        else {
            block_in_use = (block_info *) BYTE_ADDER(block_in_use, sizeof(block_info) + this_size);
            offset = offset + sizeof(block_info) + this_size;
        }
    }
    mutex_unlock(&malloc_lock);
    return FAILURE;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * set "use" bool for thr block.
 */
void hl_release(void *heap, void *block) {
    mutex_lock(&malloc_lock);
    if(block!=FAILURE){
    block_info *tar_block = (block_info *) BYTE_ADDER(block, -sizeof(block_info));
    tar_block->use = false;
    }
    mutex_unlock(&malloc_lock);
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * Free requested block and allocate new size. 
 * Failure --> unrelease and returns "FAILURE"
 * 
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {
    mutex_lock(&malloc_lock);
    if(block == FAILURE){
        mutex_unlock(&malloc_lock);
        return hl_alloc(heap, new_size);
    }
    block_info *tar_block = (block_info *) BYTE_ADDER(block, -sizeof(block_info));
    int old_size = tar_block->block_size;

    if(block!=FAILURE){
    block_info *tar_block = (block_info *) BYTE_ADDER(block, -sizeof(block_info));
    tar_block->use = false;
    }

	mutex_unlock(&malloc_lock);
    void *ptr = hl_alloc(heap, new_size);
	mutex_lock(&malloc_lock);
    if(ptr == FAILURE){
        tar_block = (block_info *) BYTE_ADDER(block, -sizeof(block_info)); // block is unreleased
        tar_block->use = true;
        mutex_unlock(&malloc_lock);
        return FAILURE;
    }
    // Move memory 
    if(old_size > new_size) {
        memmove(ptr, block, new_size);
    } else {
        memmove(ptr, block, old_size);
    }
    mutex_unlock(&malloc_lock);
    return ptr;
}
