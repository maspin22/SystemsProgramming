#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "heaplib.h"

#define HEAP_SIZE 1024
#define NUM_TESTS 24
#define NPOINTERS 100

// TODO: Add test descriptions as you add more tests...
const char* test_descriptions[] = {
    /* our SPEC tests */
    /* 0 */ "single init, should return without error",
    /* 1 */ "single init then single alloc, should pass",
    /* 2 */ "single alloc which should fail b/c heap is not big enough",
    /* 3 */ "multiple allocs, verifying no hard-coded block limit",
    /* your SPEC tests */
    /* 4  */ "single large alloc, single release and same large alloc, makes sure that release works",
    /* 5  */ "alloc 3/4 heap and try to alloc 3/4 heap again, should fail b/c heap is not big enough",
    /* 6  */ "alloc does not return a pointer within another allocated block",
    /* 7  */ "alloc 1/4 heap three times and release three times, and same multiple alloc, should work",
    /* 8  */ "single alloc 2 * heapsize, release that should act as NOP, then alloc 3/4 heap should work",
    /* 9  */ "check if it is 8-byte aligned, single alloc, check if the pointer is divided by 8, ***work",
    /* 10 */ "alloc half of the heap, then resize it to 5/4 heapsize, should failure b/c not enough space, and the original block should still valid",
    /* 11 */ "alloc 1/4 of the heap twice, then resize the first one to 1/3, should be alloced in the other place of the heap, and keep the data",
    /* 12 */ "alloc 5/4 of the heap, this should return FAILURE, when resize it to 3/4 of the heap, it should behave like alloc",
    /* 13 */ "single alloc and resize, pointer returned should be 8-byte aligned",
    /* 14 */ "alloc 1/4 of the heap twice, then resize the first one to 1/3, should be alloced in the other place of the heap, and 8-byte aligned",
    /* 15 */ "check that heap is 8-byte aligned initially",
    /* STRESS tests */
    /* 16 */ "alloc & free, stay within heap limits",
    /* 17 */ "do multiple alloc, and free some of them, then alloc again, check if the alloc will assign the user a block that overlap with another unfreed block",
    /* 18 */ "do multiple alloc, and resize them to block_size+1, the content should be copied to the new block",
    /* 19 */ "do ten alloc with block_size 2,and store hi in it, and resize these blocks to size 3, check if hi still exist",
    /* 20 */ "simply alloc resize and free",
    /* 21 */ "alloc a block of size 1 firstly, and resize it to 2,3,4... multiple times, everytime with a new character adding in the end, the content should reserve",
    /* 22 */ "alloc a large block, resize it down by 1-byte smaller each time until size of zero, and resize it to 10-byte larger block untill the heap is full",
    /* 23 */ "alloc a large block, resize it down by 3-byte smaller each time until size of zero, and resize it to 6-byte larger block untill the heap is full",
};

/* ------------------ COMPLETED SPEC TESTS ------------------------- */

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: hl_init
 * SPECIFICATION BEING TESTED:
 * hl_init should successfully complete (without producing a seg
 * fault) for a HEAP_SIZE of 1024 or more.
 *
 * MANIFESTATION OF ERROR:
 * A basic test of hl_init.  If hl_init has an eggregious programming
 * error, this simple call would detect the problem for you by
 * crashing.
 *
 * Note: this shows you how to create a test that should succeed.
 */
int test00() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    return SUCCESS;
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: hl_init & hl_alloc
 * SPECIFICATION BEING TESTED:
 * If there is room in the heap for a request for a block, hl_alloc
 * should sucessfully return a pointer to the requested block.
 *
 * MANIFESTATION OF ERROR:
 * The call to hl_alloc will return NULL if the library cannot find a
 * block for the user (even though the test is set up such that the
 * library should be able to succeed).
 */
int test01() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns null, test01 returns FAILURE (==0)
    return (hl_alloc(heap, HEAP_SIZE/2) != NULL);
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: hl_init & hl_alloc
 * SPECIFICATION BEING TESTED:
 * If there is not enough room in the heap for a request for a block,
 * hl_alloc should return NULL.
 *
 * MANIFESTATION OF ERROR:
 * This test is designed to request a block that is definitely too big
 * for the library to find. If hl_alloc returns a pointer, the library is flawed.
 *
 * Notice that heaplame's hl_alloc does NOT return NULL. (This is one
 * of many bugs in heaplame.)
 *
 * Note: this shows you how to create a test that should fail.
 *
 * Surely it would be a good idea to test this SPEC with more than
 * just 1 call to alloc, no?
 */
int test02() {

    // simulated heap is just an array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns NULL, test02 returns SUCCESS (==1)
    return !hl_alloc(heap, HEAP_SIZE*2);

}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: hl_init & hl_alloc
 * SPECIFICATION BEING TESTED:
 * There should be no hard-coded limit to the number of blocks heaplib
 * can support. So if the heap gets larger, so should the number of
 * supported blocks.
 *
 * MANIFESTATION OF ERROR:
 * See how many blocks are supported with heap size N. This number should
 * increase with heap size 2N. Otherwise fail!
 *
 * Note: unless it is fundamentally changed, heaplame will always fail
 * this test. That is fine. The test will have more meaning when run on
 * your heaplib.c
 */
int test03() {

    // now we simulate 2 heaps, once 2x size of the first
    char heap[HEAP_SIZE], heap2[HEAP_SIZE*2];
    int num_blocks = 0;
    int num_blocks2 = 0;

    hl_init(heap, HEAP_SIZE);

    while(true) {
        int *array = hl_alloc(heap, 8);
        if (array)
            num_blocks++;
        else
            break;
    }

    hl_init(heap2, HEAP_SIZE*2);

    while(true) {
        int *array = hl_alloc(heap2, 8);
        if (array)
            num_blocks2++;
        else
            break;
    }
#ifdef PRINT_DEBUG
    printf("%d blocks (n), then %d blocks (2n) allocated\n", num_blocks, num_blocks2);
#endif

    // hoping to return SUCCESS (==1)
    return (num_blocks2 > num_blocks);
}

/* ------------------ YOUR SPEC TESTS ------------------------- */

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_alloc and hl_release
 * SPECIFICATION BEING TESTED: Release makes frees up the whole block the pointer pointed to.
 *
 * MANIFESTATION OF ERROR: This test is designed to catch an implementation of release
 * that only partially frees up a block and does not make the whole block available
 * for a malloc on that block.
 *
 */
int test04() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    char *ptr1 = hl_alloc(heap, 3 * HEAP_SIZE / 4);
    hl_release(heap, ptr1);

    char *ptr2 = hl_alloc(heap, 3 * HEAP_SIZE / 4);
    // should return a pointer successfully
    return (ptr2 != NULL);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_init & hl_alloc
 * SPECIFICATION BEING TESTED:
 * If there is not enough room in the heap for a request for a block,
 * i.e., part of the heap has been allocated already and there is not
 * enough free space for a second allocation, hl_alloc should return NULL.
 *
 * MANIFESTATION OF ERROR:
 * This test is designed to request a block that is too big
 * for the library to find in a partly allocated heap.
 * If hl_alloc returns a pointer, the library is flawed.
 *
 */
int test05() {

    // simulated heap is just an array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);
    char *ptr1 = hl_alloc(heap, 3 * HEAP_SIZE / 4);
    char *ptr2 = hl_alloc(heap, 3 * HEAP_SIZE / 4);
    // if this returns NULL, test05 returns SUCCESS (==1)
    return (ptr1 != NULL) && (ptr2 == NULL);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_alloc
 * SPECIFICATION BEING TESTED: alloc does not return a pointer within an
 * already allocated block of memory
 *
 *
 * MANIFESTATION OF ERROR: This test is designed to request 2 blocks of memory
 * and check whether they are atleast block_size apart from each other
 *
 */
int test06() {

    // simulated heap is just an array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);
    char* ptr1 = hl_alloc(heap, HEAP_SIZE / 2);
    char* ptr2 = hl_alloc(heap, HEAP_SIZE / 4);

    return ptr2 >= ptr1 + (HEAP_SIZE / 2);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_alloc and hl_release
 * SPECIFICATION BEING TESTED: release actually releases contiguous blocks and alloc is 
 * able to allocated memory after freeing a whole heap *
 *
 * MANIFESTATION OF ERROR: This test is designed to request 3 blocks of memory, release
 * all of them, and allocate the same three blocks again. A broken malloc will fail on this
 * test if release does not actually free up the heap, and alloc is not able to work on
 * a heap that has been released.
 *
 */
int test07() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char *ptrarray[3];
    int num_alloc = 0;

    for (int i = 0; i < 3; i++) {
        char *ptr = hl_alloc(heap, HEAP_SIZE / 4);
        ptrarray[i] = ptr;
        num_alloc += 1;
    }

    for (int i = 0; i < 3; i++) {
        hl_release(heap, ptrarray[i]);
    }

    for (int i = 0; i < 3; i++){
        char *ptr = hl_alloc(heap, HEAP_SIZE / 4);
        ptrarray[i] = ptr;
        num_alloc++;
    }
    // if this returns NULL, test05 returns SUCCESS (==1)
    return (num_alloc == 6);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_release
 * SPECIFICATION BEING TESTED: Release acts as a NOP if block == 0
 *
 * MANIFESTATION OF ERROR: This test tests whether calling release on a NULL pointer
 * crashes the program, or exits gracefully according to the spec. Any alloc after this 
 * NOP should work.
 *
 */
int test08() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    char *ptr1 = hl_alloc(heap, 2 * HEAP_SIZE);
    hl_release(heap, ptr1);

    char *ptr2 = hl_alloc(heap, 3 * HEAP_SIZE / 4);
    // should return a pointer successfully
    return (ptr2 != NULL);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_alloc
 * SPECIFICATION BEING TESTED: POinter returned by alloc is 8-byte aligned
 *
 * MANIFESTATION OF ERROR: This test is designed to break on implementations
 * that do not align their blocks
 *
 */
int test09() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    char *ptr1 = hl_alloc(heap, 3 * HEAP_SIZE / 4);

    return ((uintptr_t)ptr1 % 8 == 0);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: Resize gracefully fails when there is not enough space in
 * the heap, no blocks are moved or freed.
 *
 * MANIFESTATION OF ERROR: This test first allocates half the heap, tries to resize it to 5/4 heapsize
 * which should return a failure. An alloc of 3/4 of the heap should fail after this due to lack of
 * space. This means that the existing allocated memory is not freed when resize fails.
 *
 */
int test10() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    char *ptr1 = hl_alloc(heap, 3 * HEAP_SIZE / 4);
    char *ptr2 = hl_resize(heap, ptr1, 5 *HEAP_SIZE / 4);
    char *ptr3 = hl_alloc(heap, 3 * HEAP_SIZE / 4);
    return (ptr2 == FAILURE && ptr3 == FAILURE);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: Resizing to a larger block size preserves the data in the block
 *
 * MANIFESTATION OF ERROR: This test first allocates two blocks and resizes the first block to a larger
 * size. This is designed to check that the data in the first block remains the same when
 * the reassigned block is likely different from the first block. We check this by
 * simply assigning the first character and checking it after the realloc
 *
 */
int test11() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    char *ptr1 = hl_alloc(heap, HEAP_SIZE / 4);
    ptr1[2] = 'y';
    char *ptr2 = hl_alloc(heap, HEAP_SIZE / 4);
    char *ptr3 = hl_resize(heap, ptr1, HEAP_SIZE / 3);

    return (ptr3 != FAILURE) && (ptr3[2]=='y') && (ptr2 != FAILURE);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: Resize should behave like alloc when block == 0
 *
 * MANIFESTATION OF ERROR: This test is designed to catch implementations of resize
 * that simply fail when the pointer passed in is NULL, according to the spec resize 
 * should just behave like alloc in this case
 *
 */
int test12() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    char *ptr1 = hl_alloc(heap, 5 * HEAP_SIZE / 4);
    char *ptr2 = hl_resize(heap, ptr1, 3 *HEAP_SIZE / 4);

    return (ptr2 != FAILURE);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: Resize returns a 8-byte aligned pointer
 *
 * MANIFESTATION OF ERROR: This test checks that the pointer returned by resize is 8-byte aligned
 * by calling alloc once and resizing the pointer returned once.
 *
 */
int test13() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    char *ptr1 = hl_alloc(heap, 1 * HEAP_SIZE / 4);
    hl_alloc(heap, 1 * HEAP_SIZE / 4);
    char *ptr3 = hl_resize(heap, ptr1, 1 *HEAP_SIZE / 3);

    return ((uintptr_t)ptr3 % 8 == 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: Resizing a large block to a smaller size works
 *
 * MANIFESTATION OF ERROR: This test is designed to catch implementations of resize that 
 * reassign the pointer of a larger block to a different part of the heap, instead of 
 * reducing the block size (this should not fail because there is clearly enough space in
 * the heap to go from a bigger to a smaller block size)
 *
 */
int test14() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    char *ptr1 = hl_alloc(heap, 3 * HEAP_SIZE / 4);
    char *ptr2 = hl_resize(heap, ptr1, HEAP_SIZE / 2);

    return ((uintptr_t)ptr2 % 8 == 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: Init
 * SPECIFICATION BEING TESTED: Checks that the heap is initialized as 8-byte aligned
 *
 * MANIFESTATION OF ERROR: This test initializes the heap with a heap pointer that is not
 * divisible by 8 and checks that the first block address is 8-byte aligned inspite of
 * the argument not being 8-byte aligned
 *
 */
int test15() {
    char heap[HEAP_SIZE * 2];
    hl_init(heap + 3, 2 * HEAP_SIZE - 3);
    char *pt = hl_alloc(heap + 3, HEAP_SIZE);

    return ((uintptr_t)pt % 8 == 0);
}

/* ------------------ STRESS TESTS ------------------------- */

/* THIS TEST IS NOT FINISHED. It is a stress test, but it does not
 * actually test to see whether the library or the user writes
 * past the heap. You are encouraged to complete this test.
 *
 * FUNCTIONS BEING TESTED: hl_alloc, hl_release
 * 
 * SPECIFICATION BEING TESTED:
 * The library should not give user "permission" to write off the end
 * of the heap. Nor should the library ever write off the end of the heap.
 *
 * MANIFESTATION OF ERROR:
 * If we track the data on each end of the heap, it should never be
 * written to by the library or a good user.
 *
 */
int test16() {
    int n_tries    = 10000;
    int block_size = 16;

    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];

    char oldmem[HEAP_SIZE * 2];
    for(int i = 0; i < HEAP_SIZE; i++) {
        oldmem[i] = memarea[i];
    }
    for(int i = 0; i < HEAP_SIZE; i++) {
        oldmem[HEAP_SIZE + i] = memarea[2*HEAP_SIZE + i];
    }

    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);

            if(pointers[index] != 0){
                // check that pointers[index] is not out of bounds
                if(pointers[index] < heap_start || pointers[index] + block_size >= &memarea[2048]) {
                    return FAILURE;
                }

                // assign some data
                for(int i = 0; i < block_size; i++) {
                    pointers[index][i] = (char)('a' + i);
                }
            }
        }
        else{
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }

    for(int i = 0; i < NPOINTERS; i++){
        // check that data is still 'p'
        char *arr = pointers[i];
        if(arr != 0) {
            for(int j = 0; j < block_size; j++){
                if(arr[j] != (char)('a' + j)) {
                    return FAILURE;
                }
            }
        }
    }

    for(int i = 0; i < HEAP_SIZE; i++) {
        if(oldmem[i] != memarea[i]) {
            return FAILURE;
        }
    }
    for(int i = 0; i < HEAP_SIZE; i++) {
        if(oldmem[HEAP_SIZE + i] != memarea[2*HEAP_SIZE + i]) {
            return FAILURE;
        }

    }
    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: hl_alloc, hl_release, and hl_resize
 * 
 * INTEGRITY OR DATA CORRUPTION?
 * Check the structural integrity and alignment. The field in the heap thought to
 * contain a pointer should always contain a valid address, rather than being
 * overwritten by other data or pointing to a wrong place;
 * 
 * MANIFESTATION OF ERROR:
 * By allocing new blocks from size 1, and increment the block_size each time
 * the alloc should always work until there is not enough space, and the while
 * loop stops, otherwise this should return to a failure. At the same time,
 * we are also testing if it is properly aligned, if not, return failure.
 * Next, we release the last block and realloc a block with smaller size, and resized
 * the first block to a different size, these two actions should work, because
 * there is enough space for these two blocks, otherwise return failure
 *
 */
int test17() {

    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    int block_size = 2;

    char *array;
    char *last_array;
    char *first_array = hl_alloc(heap, 1);
    while(true) {
        array = hl_alloc(heap, block_size);
        if (array) {
            block_size += 1;
            if((uintptr_t)array % 8 != 0) {
                return FAILURE;
            }
            last_array = array;
        }
        else {
            break;
        }
    }

    hl_release(heap, last_array);
    char *ptr1 = hl_alloc(heap, block_size / 3);
    char *ptr2 = hl_resize(heap, first_array, block_size / 3);

    return (ptr1 != FAILURE && ptr2 != FAILURE);
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: hl_alloc and hl_resize
 * 
 * INTEGRITY OR DATA CORRUPTION?
 * structural and data integrity. The field in the heap thought to
 * contain a pointer should always contain a valid address, rather than being
 * overwritten by other data or pointing to a wrong place; A block can't go pass
 * the edge of the heap,and cannot have some overlapping region that has already
 * been given out in a previous request to alloc that has not yet been freed
 * The library itself cannot write something into the block portion of the heap,
 * which it should never do unless that block has been freed by the user.
 * 
 * MANIFESTATION OF ERROR:
 * By alloc blocks from size 2 and increment the block_size until the heap is full,
 * fill in data in each new block and resize those blocks to a 1-byte larger block,
 * The resized block should contain the original data if it is successfully resized,
 * Otherwise return failure.
 */
int test18() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    int block_size = 2;

    char *array;
    char *arraypointer[NPOINTERS];
    int first_alloc_index = 0;

    while(first_alloc_index<10) {
        array = hl_alloc(heap, block_size);
        if (array) {

            for (int i = 0; i < block_size; i++){
                array[i] = (char)'a' + i;
            }
            arraypointer[first_alloc_index] = array;
            first_alloc_index += 1;
            block_size += 1;
            if((uintptr_t)array % 8 != 0) {
                return FAILURE;
            }
        }
        else {
            break;
        }
    }
    unsigned int new_size = 3;
    int resize_index = 0;
    char* sec_array;
    char* arraypointer_sec[NPOINTERS];
    while (resize_index<first_alloc_index) {
        sec_array = hl_resize(heap, arraypointer[resize_index], new_size);
        arraypointer_sec[resize_index] = sec_array;
        resize_index += 1;
        new_size += 1;
    }
    char* check_array;
    int i = 0;
    int checking_size = 2;
    while (i<resize_index) {
        check_array = arraypointer_sec[i];

        for (int j = 0; j < checking_size; j++) {
            if (check_array[j] != (char)'a' + j) {
                return FAILURE;
            }
        }
        i += 1;
       checking_size += 1;
    }

    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: hl_alloc and hl_resize
 * 
 * INTEGRITY OR DATA CORRUPTION?
 * structural and data integrity. The field in the heap thought to
 * contain a pointer should always contain a valid address, rather than being
 * overwritten by other data or pointing to a wrong place; A block can't go pass
 * the edge of the heap,and cannot have some overlapping region that has already
 * been given out in a previous request to alloc that has not yet been freed
 * The library itself cannot write something into the block portion of the heap,
 * which it should never do unless that block has been freed by the user.
 * 
 * MANIFESTATION OF ERROR:
 * By allocing multiple blocks with size of 2 until the heap is full, and fill
 * in with two characters, and resize those blocks to size of 3. These process should
 * not cause segfault. Meanwhile checking the data in the resized block, if there is
 * any difference, return failure.
 */
int test19() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    int block_size = 2;

    char *array;
    char *arraypointer[NPOINTERS*100];
    int first_alloc_index = 0;

    while(true) {
        array = hl_alloc(heap, block_size);
        if (array) {
            array[0] = 'h';
            array[1] = 'i';
            arraypointer[first_alloc_index] = array;
            first_alloc_index += 1;
            if((uintptr_t)array % 8 != 0) {
                return FAILURE;
            }
        }
        else {
            break;
        }
    }
    unsigned int new_size = 3;
    int resize_index = 0;
    char* arraypointer_sec[NPOINTERS * 100];
    while (resize_index < first_alloc_index){
        arraypointer_sec[resize_index] = hl_resize(heap, arraypointer[resize_index], new_size);
        resize_index += 1;
    }
    char* check_array;
    int i = 0;
    while (i < resize_index) {
        check_array = arraypointer_sec[i];

        if(check_array[0] != 'h' || check_array[1] != 'i') {
            return FAILURE;
        }
        i += 1;
    }

    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: hl_alloc, hl_release, and hl_resize
 * 
 * INTEGRITY OR DATA CORRUPTION?
 * Check the structural integrity and alignment. The field in the heap thought to
 * contain a pointer should always contain a valid address, rather than being
 * overwritten by other data or pointing to a wrong place;
 * 
 * MANIFESTATION OF ERROR:
 * By allocing new blocks from size 1, and increment the block_size each time
 * the alloc should always work until there is not enough space, and the while
 * loop stops, otherwise this should return to a failure. At the same time,
 * we are also testing if it is properly aligned, if not, return failure.
 * Then we resize those blocks to 1-byte larger block, this will catch a segfault
 * if the structural integrity is violented. Next, we release all of the resized
 * blocks, the heap should become all free again, and do multiple alloc again,
 * the amount of blocks the third alloced should be greater or equal to the number
 * of just released blocks, otherwise return failure.
 *
 */
int test20() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    int block_size = 1;

    char *array;
    char *arraypointer[NPOINTERS];
    int first_alloc_index = 0;
    while(true) {
        array = hl_alloc(heap, block_size);
        if (array) {
            block_size += 1;
            arraypointer[first_alloc_index] = array;
            first_alloc_index += 1;
            if((uintptr_t)array % 8 != 0) {
                return FAILURE;
            }
        }
        else {
            break;
        }
    }

    unsigned int new_size = 2;
    int resize_index = 0;
    char* sec_array;
    char* arraypointer_sec[NPOINTERS];
    while (resize_index < first_alloc_index){
        sec_array = hl_resize(heap, arraypointer[resize_index], new_size);
        if((uintptr_t)sec_array % 8 != 0) {
            return FAILURE;
        }
        arraypointer_sec[resize_index] = sec_array;
        new_size += 1;
        resize_index += 1;
    }

    int release_index = 0;
    while (release_index < resize_index) {
        hl_release(heap, arraypointer_sec[release_index]);
        release_index += 1;
    }

    char* third_array;
    int block_size_third = 1;
    int third_alloc_index = 0;

    while(true){
        third_array = hl_alloc(heap, block_size_third);
        if (third_array) {
            block_size_third += 1;

            third_alloc_index += 1;
            if((uintptr_t)third_array % 8 != 0) {
                return FAILURE;
            }
        }
        else {
            break;
        }
    }


    return (resize_index == release_index);
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: hl_alloc and hl_resize
 * 
 * INTEGRITY OR DATA CORRUPTION?
 * structural and data integrity. The field in the heap thought to
 * contain a pointer should always contain a valid address, rather than being
 * overwritten by other data or pointing to a wrong place; A block can't go pass
 * the edge of the heap,and cannot have some overlapping region that has already
 * been given out in a previous request to alloc that has not yet been freed
 * The library itself cannot write something into the block portion of the heap,
 * which it should never do unless that block has been freed by the user.
 *
 * MANIFESTATION OF ERROR:
 * By allocing a block of size 1 and resize the block to 1-byte larger, after filling
 * up the heap, resize the block down to size of zero, and resize it to a larger
 * block again until the heap is full. This process will catch segfault if the
 * structural integrity is violented, and will return failure if the data is not
 * properly passed to the new block.
 */
int test21() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    int block_size = 1;
    char *array = hl_alloc(heap, block_size);
    array[0] = 'a';
    char *resize_array = array;
    while(true) {
        if (resize_array) {
            block_size += 1;
            resize_array = hl_resize(heap, resize_array, block_size);

            if((uintptr_t)resize_array % 8 != 0) {
                return FAILURE;
            }

            if(resize_array == 0) {
                break;
            }

            for(int i = 0; i < block_size - 1; i++) {
                if(resize_array[i] != (char)('a' + i)) {
                    return FAILURE;
                }
            }

            resize_array[block_size - 1] = (char)('a' + block_size - 1);

        }
        else {
            break;
        }
    }

    while(block_size > 0) {
        if (resize_array) {
            block_size -= 7;
            resize_array = hl_resize(heap, resize_array, block_size);

            if((uintptr_t)resize_array % 8 != 0) {
                return FAILURE;
            }

            if(resize_array == 0) {
                break;
            }

            for(int i = 0; i < block_size; i++) {
                if(resize_array[i] != (char)('a' + i)) {
                    return FAILURE;
                }
            }

        }
        else {
            break;
        }
    }
    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: hl_alloc and hl_resize
 * 
 * INTEGRITY OR DATA CORRUPTION?
 * structural and data integrity. The field in the heap thought to
 * contain a pointer should always contain a valid address, rather than being
 * overwritten by other data or pointing to a wrong place; A block can't go pass
 * the edge of the heap,and cannot have some overlapping region that has already
 * been given out in a previous request to alloc that has not yet been freed
 * The library itself cannot write something into the block portion of the heap,
 * which it should never do unless that block has been freed by the user.
 *
 * MANIFESTATION OF ERROR:
 * By allocing a block of size 7/8 of the heap_size, and resize it down to size of zero,
 * and then resize it to a 10-byte larger block again until the heap is full.
 * This process will catch segfault if the structural integrity is violented,
 * and will return failure if the data is not properly passed to the new block.
 */
int test22() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    int block_size = 7 * HEAP_SIZE / 8;
    char *array = hl_alloc(heap, block_size);

    for (int i = 0; i < block_size ; i++){
        array[i] = (char)('a'+i);
    }
    char *resize_array = array;
    while(block_size > 0) {
        if (resize_array) {
            block_size -= 1;
            resize_array = hl_resize(heap, resize_array, block_size);

            if((uintptr_t)resize_array % 8 != 0) {
                return FAILURE;
            }

            if(resize_array == 0) {
                break;
            }

            for(int i = 0; i < block_size; i++) {
                if(resize_array[i] != (char)('a' + i)) {
                    return FAILURE;
                }
            }

        }
        else {
            break;
        }
    }

    block_size += 10;
    resize_array = hl_resize(heap, resize_array, block_size);
    for (int i = 0; i < block_size; i++) {
      resize_array[i] = (char)'a' + i;
    }
    while(true) {
        if (resize_array) {
            block_size += 10;
            resize_array = hl_resize(heap, resize_array, block_size);

            if((uintptr_t)resize_array % 8 != 0) {
                return FAILURE;
            }

            if(resize_array == 0) {
                break;
            }

            for(int i = 0; i < block_size - 10; i++) {
                if(resize_array[i] != (char)('a' + i)) {
                    return FAILURE;
                }
            }

            for (int i = block_size - 10; i < block_size; i++){
               resize_array[i] = (char)'a' + i;
            }

        }
        else {
            break;
        }
    }
    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
*
* FUNCTIONS BEING TESTED: hl_alloc and hl_resize

* INTEGRITY OR DATA CORRUPTION?
* structural and data integrity. The field in the heap thought to
* contain a pointer should always contain a valid address, rather than being
* overwritten by other data or pointing to a wrong place; A block can't go pass
* the edge of the heap,and cannot have some overlapping region that has already
* been given out in a previous request to alloc that has not yet been freed
* The library itself cannot write something into the block portion of the heap,
* which it should never do unless that block has been freed by the user.
*
* MANIFESTATION OF ERROR:
* By allocing a block of size 3 and resize the block to 1-byte larger, after filling
* up the heap, resize the block down to size of zero, and resize it to a 6-byte larger
* block again until the heap is full. This process will catch segfault if the
* structural integrity is violented, and will return failure if the data is not
* properly passed to the new block.
*
*/
int test23() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    int block_size = 7 * HEAP_SIZE / 8;
    char *array = hl_alloc(heap, block_size);

    for (int i = 0; i < block_size; i++) {
        array[i] = (char)('a' + i);
    }
    char *resize_array = array;
    while(block_size > 0) {
        if (resize_array) {
            block_size -= 3;
            resize_array = hl_resize(heap, resize_array, block_size);

            if((uintptr_t)resize_array % 8 != 0) {
                return FAILURE;
            }

            if(resize_array == 0) {
                break;
            }

            for(int i = 0; i < block_size; i++) {
                if(resize_array[i] != (char)('a' + i)) {
                    return FAILURE;
                }
            }

        }
        else {
            break;
        }
    }

    block_size += 6;
    resize_array = hl_resize(heap, resize_array, block_size);
    for (int i = 0; i < block_size; i++) {
      resize_array[i] = (char)'a' + i;
    }
    while(true) {
        if (resize_array) {
            block_size += 6;
            resize_array = hl_resize(heap, resize_array, block_size);

            if((uintptr_t)resize_array % 8 != 0) {
                return FAILURE;
            }

            if(resize_array == 0) {
                break;
            }

            for(int i = 0; i < block_size - 6; i++) {
                if(resize_array[i] != (char)('a' + i)) {
                    return FAILURE;
                }
            }

            for (int i = block_size - 6; i < block_size; i++) {
               resize_array[i] = (char)'a' + i;
            }

        }
        else {
            break;
        }
    }
    return SUCCESS;
}
