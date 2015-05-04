
#include "my_malloc.h"

/* You *MUST* use this macro when calling my_sbrk to allocate the 
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code in your malloc library, then you will receive a grade
 * deduction. You have been warned!
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif

/* Make sure this always points to the beginning of your current
 * heap space! If it does not, then the grader will not be able
 * to run correctly and you will probably get a 0. Remember that
 * only the _first_ call to my_malloc() returns the beginning of
 * the heap. Sequential calls will return a pointer to the newly
 * added space!
 * Technically this should be declared static because we do not
 * want any program outside of this file to be able to access it.
 * However, DO NOT CHANGE the way this variable is declared or
 * it will break the autograder.
 */
void* heap;

/* Our freelist structure. This is where the current freelist of
 * blocks will be maintained. Failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist[8];
/* SIZES FOR THE FREE LIST:
 *   freelist[0] -> 16
 *   freelist[1] -> 32
 *   freelist[2] -> 64
 *   freelist[3] -> 128
 *   freelist[4] -> 256
 *   freelist[5] -> 512
 *   freelist[6] -> 1024
 *   freelist[7] -> 2048
 */

metadata_t* findPair(metadata_t* other);
int nearestPower(int size);
int getIndex(int size);
void removeFreeList(metadata_t* chunk, int idx);
void putInFreelist(metadata_t* chunk, int idx);
metadata_t* my_malloc_helper(int i);
void my_free_helper(metadata_t* chunk, int i);

void* my_malloc(size_t size)
{
	//add in metadata size
	size_t trueSize = size + sizeof(metadata_t);
	//error if too big;
	if (trueSize > 2048) {
		ERRNO = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}
	//where to start in the freelist
	int i = getIndex(trueSize);
	metadata_t* block = my_malloc_helper(i);
	if(!block) {
		return NULL;
	}
	//set the pointer past the metadata
	block = (metadata_t*) ((char*) block + sizeof(metadata_t));
	ERRNO = NO_ERROR;
	return block;
}

metadata_t* my_malloc_helper(int i)
{
	//we need more heap!
	if(i == 8)
	{
		metadata_t* chunk = (metadata_t*) my_sbrk(SBRK_SIZE);
		if(!chunk)
		{
			ERRNO = OUT_OF_MEMORY;
			return NULL;
		}
		//set the heap if not set
		if(!heap)
		{
			heap = (void*) chunk;
		}
		chunk->size = 2048;
		chunk->in_use = 1;
		return chunk;
	} else if(freelist[i]) {
		//grab the head of the freelist
		metadata_t* chunk = freelist[i];
		removeFreeList(chunk,i);
		return chunk;
	}
	//we need to recurse
	metadata_t* bigChunk = my_malloc_helper(i + 1);
	if(i == 7)
	{
		//dont break up fresh heap
		return bigChunk;
	}
	metadata_t* half = (metadata_t*)((char*)bigChunk + bigChunk->size/2);
	putInFreelist(half, i);
	//half the size
	bigChunk->size >>= 1;
	return bigChunk;
}

//puts a chunk back into the freelist
void putInFreelist(metadata_t* chunk, int idx)
{
	//set the size of the chunk
	chunk->size = 1 << (idx + 4);
	chunk->in_use = 0;
	//set as the head of the list
	chunk->next = freelist[idx];
	//insert into list
	if(freelist[idx])
	{
		chunk->next->prev = chunk;
	}
	chunk->prev = NULL;
	freelist[idx] = chunk;
}

//takes a chunk out of the freelist
void removeFreeList(metadata_t* chunk, int idx)
{
	//test if it is the head
	if(freelist[idx] == chunk)
	{
		freelist[idx] = chunk->next;
	} else {
		chunk->prev->next = chunk->next;
	}
	//remove from list
	if(chunk->next)
	{
		chunk->next->prev = chunk->prev;
	}
	chunk->in_use = 1;
}

void* my_calloc(size_t num, size_t size)
{
	//get a byte array
	char* array = (char*) my_malloc(num * size);
	//return null on my_malloc error
	if(ERRNO != NO_ERROR)
	{
		return NULL;
	}
	//set to zero
	for(int i = 0; i < num * size; i++)
	{
		array[i] = 0;
	}
	return array;
}

void my_free(void* ptr)
{
	//set the pointer to the metadata
	metadata_t* chunk = (metadata_t*)((char*)ptr - sizeof(metadata_t));
	//dont free twice
	if(!chunk->in_use)
	{
		ERRNO = DOUBLE_FREE_DETECTED;
		return;
	}
	//find the index for the freelist
	int i = getIndex(chunk->size);
	my_free_helper(chunk, i);
	ERRNO = NO_ERROR;
}

//recursively pairs chunks and puts them in freelist
void my_free_helper(metadata_t* chunk, int i)
{
	//dont recurse higher than 2048
	if(i == 7)
	{
		putInFreelist(chunk, i);
		return;
	}
	//find its buddy
	metadata_t* pair = findPair(chunk);
	//test if we can pair them
	if(!pair->in_use && pair->size == chunk->size)
	{
		removeFreeList(pair, i);
		//find the lower one and then recurse
		metadata_t* lower = ( chunk < pair) ? chunk : pair;
		lower->size <<= 1;
		my_free_helper(lower, i + 1);
	} else {
		putInFreelist(chunk, i);
	}
}

//moves memory in place
void* my_memmove(void* dest, const void* src, size_t num_bytes)
{
	//should we copy high to low or low to high
	if( dest <  src)
	{
		//copy
		for(int i = 0; i < num_bytes; i++)
		{
			((char*) dest)[i] = ((char*) src)[i];
		}
	} else {
		//copy
		for(int i = num_bytes - 1; i >= 0; i--)
		{
			((char*) dest)[i] = ((char*) src)[i];
		}
	}
	//set error
	ERRNO = NO_ERROR;
	return dest;
}

//get index finds the appropriate index of a size in the freelist
int getIndex(int size)
{
	int i = 0;
	//double until 
	while((1 << (i + 4)) < size) {
		i++;
	}
	return i;
}

//find the nearest power of two to the size
int nearestPower(int size)
{
	int i = 16;
	while(i < size) {
		i <<= 1;
	}
	return i;
}

//finds the address of the pair block
metadata_t* findPair(metadata_t* other)
{
	metadata_t* pair;
	
	//get relative difference from the heap start
	uintptr_t base = ((uintptr_t)((char*)other) - (uintptr_t)heap);

	//int base = (short)other - (short)heap;
	//test the size bit
	if(base & other->size) {
		pair = (metadata_t*)((char*)other - other->size);
	} else {
		pair = (metadata_t*)((char*)other + other->size);
	}
	return pair;
}






