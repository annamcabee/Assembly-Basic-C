#include <errno.h>
#include <stdlib.h>
//Anna McAbee
/* Emulates a call to the system call sbrk(2) */

/* We emulate for ease of debugging; if the allocator used the system
 * call sbrk, then we would need to redefine the system call malloc to 
 * be ours instead to avoid conflicts. If our library did not work correctly,
 * then printf could not even perform simple tasks since it internally
 * dynamically allocates memory when formatting strings!
 */

/* 0x2000 = 2^13 = 8 KB */
#define HEAP_SIZE 0x2000

void *my_sbrk(int increment) {

	static char *fake_heap = NULL;
	static int current_top_of_heap = 0;
	void *ret_val;

	if(!fake_heap) {
		if(!(fake_heap = calloc(HEAP_SIZE, 1))) {
			return (void*) -1;
		}
	}
	ret_val = current_top_of_heap + fake_heap;

	if ((current_top_of_heap + increment > HEAP_SIZE) || (current_top_of_heap+increment < 0)) {
		errno = ENOMEM;
		return (void*) -1;
	}

	current_top_of_heap += increment;
	return ret_val;
}

