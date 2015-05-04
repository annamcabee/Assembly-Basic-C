/**
 * CS 2110 - Spring 2015 - Homework #10
 * Edited by: Brandon Whitehead, Andrew Wilder
 *
 * list.c: Complete the functions!
 *
 * Anna McAbee 
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "my_malloc.h"

/* The node struct.  Has a prev pointer, next pointer, and data. */
/* DO NOT DEFINE ANYTHING OTHER THAN WHAT'S GIVEN OR YOU WILL GET A ZERO*/
/* Design consideration only this file should know about nodes */
/* Only this file should be manipulating nodes */
/* DO NOT TOUCH THIS DECLARATION DO NOT PUT IT IN OTHER FILES */
typedef struct lnode
{
  struct lnode* prev; /* Pointer to previous node */
  struct lnode* next; /* Pointer to next node */
  void* data; /* User data */
} node;


/* Do not create any global variables here. Your linked list library should obviously work for multiple linked lists */
// This function is declared as static since you should only be calling this inside this file.
static node* create_node(void* data);
/** create_node
  *
  * Helper function that creates a node by allocating memory for it on the heap.
  * Be sure to set its pointers to NULL.
  *
  * @param data a void pointer to data the user wants to store in the list
  * @return a node
  */
static node* create_node(void* data)
{
    	node* new = (node*) my_malloc(sizeof(node));
	new->data = data;
	new->next = NULL;
	new->prev = NULL;
	return new;
}

/** create_list
  *
  * Creates a list by allocating memory for it on the heap.
  * Be sure to initialize size to zero and head/tail to NULL.
  *
  * @return an empty linked list
  */
list* create_list(void)
{
    	list* new = (list*) malloc(sizeof(list));
	new->size = 0;
	new->head = NULL;
	new->tail = NULL;
	return new;
}

/** push_front
  *
  * Adds the data to the front of the linked list
  *
  * @param llist a pointer to the list.
  * @param data pointer to data the user wants to store in the list.
  */
void push_front(list* llist, void* data)
{
    	node* newNode = create_node(data);
	llist->size++;
	if(llist->tail == NULL) {
		llist->tail = newNode;
	} else {
		newNode->next = llist->head;
		llist->head->prev = newNode;
	}
	llist->head = newNode;
}

/** push_back
  *
  * Adds the data to the back/end of the linked list
  *
  * @param llist a pointer to the list.
  * @param data pointer to data the user wants to store in the list.
  */
void push_back(list* llist, void* data)
{
	node* newNode = create_node(data);
	llist->size++;
	if(llist->tail == NULL) {
		llist->head = newNode;
	} else {
	llist->tail->next = newNode;
		newNode->prev = llist->tail;
	}
	llist->tail = newNode;
}

/** front
  *
  * Gets the data at the front of the linked list
  * If the list is empty return NULL.
  *
  * @param llist a pointer to the list
  * @return The data at the first node in the linked list or NULL.
  */
void* front(list* llist)
{
	if(llist->head != NULL) {
		return llist->head;
	}
	return llist->head;
}

/** back
  *
  * Gets the data at the "end" of the linked list
  * If the list is empty return NULL.
  *
  * @param llist a pointer to the list
  * @return The data at the last node in the linked list or NULL.
  */
void* back(list* llist)
{
    	if(llist->tail != NULL) {
		return llist->tail;
	}
	return llist->tail;
}

/** remove_front
  *
  * Removes the node at the front of the linked list
  *
  * @warning Note the data the node is pointing to is also freed. If you have any pointers to this node's data it will be freed!
  *
  * @param llist a pointer to the list.
  * @param free_func pointer to a function that is responsible for freeing the node's data.
  * @return -1 if the remove failed (which is only there are no elements) 0 if the remove succeeded.
  */
int remove_front(list* llist, list_op free_func)
{
   	if(llist->size == 0) {
		return -1;
	}
	if(llist->size == 1) {
		llist->tail = NULL;
	}
	llist->size--;
	node* head = llist->head;
	if(head->next) {
		head->next->prev = NULL;
	}
	llist->head = head->next;
	free_func(head->data);
	free(head);
	return 0;
}

/** remove_back
  *
  * Removes the node at the back of the linked list
  *
  * @warning Note the data the node is pointing to is also freed. If you have any pointers to this node's data it will be freed!
  *
  * @param llist a pointer to the list.
  * @param free_func pointer to a function that is responsible for freeing the node's data.
  * @return -1 if the remove failed 0 if the remove succeeded.
  */
int remove_back(list* llist, list_op free_func)
{
   	if(llist->size == 0) {
		return -1;
	}
	if(llist->size == 1) {
		llist->head = NULL;
	}
	llist->size--;
	node* tail = llist->tail;
	if(tail->prev) {
		tail->prev->next = NULL;
	}
	llist->tail = tail->prev;
	free_func(tail->data);
	free(tail);
	return 0;
}

/** copy_list
  *
  * Create a new list structure, new nodes, and new copies of the data by using
  * the copy function. Its implementation for any test structure must copy
  * EVERYTHING!
  *
  * @param llist A pointer to the linked list to make a copy of
  * @param copy_func A function pointer to a function that makes a copy of the
  *        data that's being used in this linked list, allocating space for
  *        every part of that data on the heap. This is some function you must
  *        write yourself for testing, tailored specifically to whatever context
  *        you're using the linked list for in your test.
  * @return The linked list created by copying the old one
  */
list* copy_list(list* llist, list_cpy copy_func)
{
	list* new = create_list();
	node* iter = llist->tail;
	for (unsigned int i = 0; i < llist->size; i++) {
		push_front(new, copy_func(iter->data));
		iter = iter->prev;
	}
	return new;
}

/** size
  *
  * Gets the size of the linked list
  *
  * @param llist a pointer to the list
  * @return The size of the linked list
  */
int size(list* llist)
{
        return llist->size;
}

/** remove_if
  *
  * Removes all nodes whose data when passed into the predicate function returns true
  *
  * @param llist a pointer to the list
  * @param pred_func a pointer to a function that when it returns true it will remove the element from the list and do nothing otherwise @see list_pred.
  * @param free_func a pointer to a function that is responsible for freeing the node's data
  * @return the number of nodes that were removed.
  */
int remove_if(list* llist, list_pred pred_func, list_op free_func)
{
    	int removed = 0;
	node* iter = llist->head;	
	while(iter != NULL)
	{
		if(pred_func(iter->data))
		{
			if(iter == llist->head)
			{
				remove_front(llist, free_func);
			} else if(iter == llist->tail) {
				remove_back(llist, free_func);
			} else {
				iter->next->prev = iter->prev;
				iter->prev->next = iter->next;
				free_func(iter->data);
				free(iter);
				llist->size--;
			}
			removed++;
		}
		iter = iter->next;
	}
	return removed;
}

/** is_empty
  *
  * Checks to see if the list is empty.
  *
  * @param llist a pointer to the list
  * @return 1 if the list is indeed empty 0 otherwise.
  */
int is_empty(list* llist)
{
        return !llist->size;
}

/** empty_list
  *
  * Empties the list after this is called the list should be empty.
  *
  * @param llist a pointer to a linked list.
  * @param free_func function used to free the node's data.
  */
void empty_list(list* llist, list_op free_func)
{
   	while(llist->size)
	{
		remove_front(llist, free_func);
	}
}

/** traverse
  *
  * Traverses the linked list calling a function on each node's data.
  *
  * @param llist a pointer to a linked list.
  * @param do_func a function that does something to each node's data.
  */
void traverse(list* llist, list_op do_func)
{
    	node* iter = llist->head;
	for (unsigned int i = 0; i < llist->size; i++) {
		do_func(iter->data);
		iter = iter->next;
	}
}
