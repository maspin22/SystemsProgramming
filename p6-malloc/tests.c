#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "heaplib.h"
#include <pthread.h>

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
    /* 4  */ "check that the heap is initially 8-byte aligned, passes",
    /* 5  */ "after one alloc and resize the returned pointer should be 8-byte aligned, passes",
    /* 6  */ "Resizing a larger block to a smaller block works",
    /* 7  */ "check alloc returns pointer that's not within another allocated block, passes",
    /* 8  */ "alloc the heap five times, then release five times, then alloc five times again, passes",
    /* 9  */ "try to alloc 7/8 heap twice, passes",
    /* 10 */ "alloc 1/8 of heap two times, resize the first to 1/4, should be alloced after the second alloc while maintaining the data, passes",
    /* 11 */ "resize to an amount greater than the size of the heap and check that it fails, passes",
    /* 12 */ "Resize should behave like alloc when the pointer passed in is NULL, passes",
    /* 13 */ "Checks release frees the entire block of memory, passes",
    /* 14 */ "Checks release fails gracefully by testing if alloc runs after it releases a null pointer, passes",
    /* 15 */ " multiple threads simultaneously using the library should not interfere with each other or cause a deadlock, passes",
    /* STRESS tests */
    /* 16 */ "alloc & free, stay within heap limits",
    /* 17 */ "your description here",
    /* 18 */ "your description here",
    /* 19 */ "your description here",
    /* 20 */ "your description here",
    /* 21 */ "your description here",
    /* 22 */ "your description here",
    /* 23 */ "your description here",
};

/* ------------------ COMPLETED SPEC TESTS ------------------------- */

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init
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
 * FUNCTIONS BEING TESTED: init & alloc
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
 * FUNCTIONS BEING TESTED: init & alloc
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
 * FUNCTIONS BEING TESTED: init & alloc
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
 * FUNCTIONS BEING TESTED: hl_init
 * SPECIFICATION BEING TESTED: confirms the heap is 8-byte alligned when initialized
 *
 *
 * MANIFESTATION OF ERROR: Intialize a heap with an arguement that is not 8 byte alligned
 *
 */
int test04() {
    char heap[HEAP_SIZE * 3];
    hl_init(heap + 5, 3 * HEAP_SIZE - 5);
    char *pt = hl_alloc(heap + 5, HEAP_SIZE);
    return ((uintptr_t)pt % 8 == 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: Resize should return an 8-byte aligned pointer
 *
 * MANIFESTATION OF ERROR: Checks the pointer returned by resize is 8-byte aligned
 * after calling alloc and resizing the pointer returned one time each.
 *
 */
int test05() {

    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char *ptr = hl_alloc(heap, 1 * HEAP_SIZE / 3);
    hl_alloc(heap, 1 * HEAP_SIZE / 3);
    char *ptr0 = hl_resize(heap, ptr, 1 *HEAP_SIZE / 4);
    return ((uintptr_t)ptr0 % 8 == 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: Resizing a larger block to a smaller block works
 *
 *
 * MANIFESTATION OF ERROR: reducing the block size should work because there is enough space
 *
 */
int test06() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char *ptr1 = hl_alloc(heap, 7 * HEAP_SIZE / 8);
    char *ptr2 = hl_resize(heap, ptr1, 5 * HEAP_SIZE / 8);
    return ((uintptr_t)ptr2 % 8 == 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_alloc
 * SPECIFICATION BEING TESTED: alloc returns a pointer outside of previosuly allocated memory
 *
 *
 * MANIFESTATION OF ERROR: Test the distance between two pointers is correct
 *
 */
int test07() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char* ptr1 = hl_alloc(heap, HEAP_SIZE / 4);
    char* ptr2 = hl_alloc(heap, HEAP_SIZE / 8);
    return ptr2 >= ptr1 + (HEAP_SIZE / 4);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_release/hl_alloc
 * SPECIFICATION BEING TESTED: Alloc will allocate memory after release has freed the heap
 *
 *
 * MANIFESTATION OF ERROR: Test allocs 5 memory blocks, releases those blocks, then allocs 5 blocks.
 
 */
int test08() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char *arr[5];
    int num = 0;

    for (int i = 0; i < 5; i+=1) {
        char *ptr = hl_alloc(heap, HEAP_SIZE / 8);
        arr[i] = ptr;
        num += 1;
    }
    for (int i = 0; i < 5; i+=1) {
        hl_release(heap, arr[i]);
    }
    for (int i = 0; i < 5; i+=1){
        char *ptr = hl_alloc(heap, HEAP_SIZE / 8);
        arr[i] = ptr;
        num +=1;
    }
    
    return (num == 10);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_init/hl_alloc
 * SPECIFICATION BEING TESTED: check that alloc return null when there is not enough space in the heap
 *
 *
 * MANIFESTATION OF ERROR: request a second block of memory too large for alloc to fufill.
 *
 */
int test09() {
	
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char *ptr1 = hl_alloc(heap, 7 * HEAP_SIZE / 8);
    char *ptr2 = hl_alloc(heap, 7 * HEAP_SIZE / 8);
    return (ptr1 != NULL) && (ptr2 == NULL);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_alloc/hl_resize
 * SPECIFICATION BEING TESTED: Resizing preserves the data in memory 
 *
 *
 * MANIFESTATION OF ERROR: allocates two memory blocks, after resizing check the data in the first memory block is the same
 *
 */
int test10() {
    char heap[2 * HEAP_SIZE];
    hl_init(heap, 2 * HEAP_SIZE);
    char *ptr1 = hl_alloc(heap, 2 * HEAP_SIZE / 8);
    ptr1[4] = 'a';
    char *ptr2 = hl_alloc(heap, 2 * HEAP_SIZE / 8);
    char *ptr3 = hl_resize(heap, ptr1, 2 * HEAP_SIZE / 4);
    return (ptr3 != FAILURE) && (ptr3[4]=='a') && (ptr2 != FAILURE);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: resize fails when there isn't space
 *
 *
 * MANIFESTATION OF ERROR: resize to an amount greater than the size of the heap and check that it fails
 *
 */
int test11() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char *ptr1 = hl_alloc(heap, 7 * HEAP_SIZE / 8);
    char *ptr2 = hl_resize(heap, ptr1, 9 *HEAP_SIZE / 8);
    char *ptr3 = hl_alloc(heap, 7 * HEAP_SIZE / 8);
    return (ptr2 == FAILURE && ptr3 == FAILURE);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED: Resize should behave like alloc when the pointer passed in is NULL
 *
 *
 * MANIFESTATION OF ERROR: Breaks implementations where resize fails when the pointer passed in is NULL
 *
 */
int test12() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char *ptr1 = hl_alloc(heap, 9 * HEAP_SIZE / 8); // will be null because size exceeds bounds
    char *ptr2 = hl_resize(heap, ptr1, 3 *HEAP_SIZE / 8);
    return (ptr2 != FAILURE);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_alloc/hl_release
 * SPECIFICATION BEING TESTED: Checks release frees the entire block of memory 
 *
 *
 * MANIFESTATION OF ERROR: Breaks implementations of hl_release that don't free the whole block of memory, because if they don't then there isnt enought space for the second pointer implying it's NULL
 *
 */
int test13() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    char *ptr1 = hl_alloc(heap, 7 * HEAP_SIZE / 8);
    hl_release(heap, ptr1);
    char *ptr2 = hl_alloc(heap, 7 * HEAP_SIZE / 8);
    return (ptr2 != NULL);

}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_release
 * SPECIFICATION BEING TESTED: Release should do nothing if block == 0
 *
 *
 * MANIFESTATION OF ERROR: Checks release fails gracefully by testing if alloc runs after it releases a null pointer
 *
 */
int test14() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    // should make ptr1 NULL
    char *ptr1 = hl_alloc(heap, 4 * HEAP_SIZE);
    hl_release(heap, ptr1);
    char *ptr2 = hl_alloc(heap, HEAP_SIZE / 8);
    return (ptr2 != NULL);
}

typedef struct {
    int arg1;
    void* heap;
    pthread_barrier_t* barrier;
} arg_struct;

/* The signature for a function run as a thread is a single void* argument returning a void*.
 * If you want to pass in arguments, you need to define a struct of arguments and cast manually.
 */

void* thread_function(void* ptr){
    arg_struct* args = (arg_struct*) ptr;
    pthread_barrier_t* barrier = args->barrier;
    //wait for all threads to synchronize at a barrier so they will run concurrently
    pthread_barrier_wait(barrier);
    hl_alloc(args->heap, 16);
    //do stuff with your arguments here
    return NULL;
}


/* THIS TEST IS NOT FINISHED.  It is a scaffold you can use for a threaded test.
 *
 * FUNCTIONS BEING TESTED: alloc
 * SPECIFICATION BEING TESTED:
 * Malloc library must be thread-safe: multiple threads simultaneously using the library
 * should not interfere with each other or cause a deadlock
 *
 * MANIFESTATION OF ERROR:
 * Test runs forever on a deadlock, or integrity is violated on a race condition.
 */
int test15() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);

    int n_threads = 100;
    //int num_iter = 10000;
    pthread_t threads[n_threads];
    int irets[n_threads]; //return values of the threads

    //initialize a synchronization barrier
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, n_threads);

    arg_struct args[n_threads];
    for(int i = 0; i < n_threads; i++){
	args[i] = (arg_struct) {.arg1=1, .barrier = &barrier, .heap=&heap}; //inline initialization of a struct

    }
    

    
    //create threads to execute your function
    for(int i = 0; i < n_threads; i++){
	irets[i] = pthread_create(&threads[i], NULL, thread_function, (void*) &args[i]);
    }
    //wait for all of the threads to finish before moving on
    for(int i = 0; i < n_threads; i++){
	pthread_join(threads[i], NULL);
    }
    //cleanup the barrier
    pthread_barrier_destroy(&barrier);
    printf("Thread 0 returned: %d\n", irets[0]);
    
    return FAILURE;
}

/* ------------------ STRESS TESTS ------------------------- */

/* THIS TEST IS NOT FINISHED. It is a stress test, but it does not
 * actually test to see whether the library or the user writes
 * past the heap. You are encouraged to complete this test.
 *
 * FUNCTIONS BEING TESTED: alloc, free
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

    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);
        }
        else{
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }
    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test17() {

    return FAILURE;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test18() {

    return FAILURE;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test19() {

    return FAILURE;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test20() {

    return FAILURE;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test21() {

    return FAILURE;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test22() {

    return FAILURE;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED:
 * INTEGRITY OR DATA CORRUPTION?
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test23() {

    return FAILURE;
}
