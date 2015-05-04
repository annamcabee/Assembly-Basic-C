#include "list.h"
#include <stdio.h>
#include <stdlib.h>
/**
* This function will create a new list and return a pointer to that list.
*/
LIST* create_list()
{
	LIST* newList=malloc(sizeof(newList));
	newList->head=NULL;
	newList->tail=NULL;
	newList->size=0;
	return newList;
}
/**
* This function will add the given data to the linked list (llist)
*/
void list_add(LIST* llist, int data)
{
	NODE* newNode=create_node(data);
	if(llist->head==NULL){ //Checks Empty List
		llist->head=newNode;
		llist->tail=newNode;
		return;
	}
	else if(data<=(llist->head->data)){ //Checks if < Head
		newNode->next=llist->head;
		llist->head=newNode;
	}
	else if(data>(llist->tail->data)){ //Checks if > Tail
		llist->tail->next=newNode;
		llist->tail=newNode;
	}
	else{ //Data is between Nodes
		NODE* currNode=llist->head;
		NODE* nextNode=llist->head->next;
		while(nextNode!=NULL){
			if(data>=nextNode->data){
				currNode=currNode->next;
				nextNode=nextNode->next;
			}
			else{
				newNode->next=nextNode;
				currNode->next=newNode;
				llist->size++;
				return;
			}
		}
	}
	llist->size++;
	return;
}
/**
* This function will return the data located at the given index in the list (llist)
* i.e. for a list of 0,4,8,9,12 list_get(llist, 2) would give me the value 8
*/
int list_get(LIST* llist, int index)
{
	NODE *currNode=llist->head;
	if(index>llist->size-1){
		return;
	} else{
		int i=0;
		for(i=0;i<index;i++){
			currNode=currNode->next;
		}
	}
	return currNode->data;
}
/**
* This function will remove the data located at the given index in the list (llist)
* just like the above function, but removes the node instead
* remember to free the node and re-arrange your pointers as needed!
*/
void list_del(LIST* llist, int index)
{
	NODE *currNode=llist->head;
	if(index==0){
		llist->head=currNode->next;
		free(currNode);
	}
	else{
		int i;
		for(i=0;i<index-1;i++){
			currNode=currNode->next;
		}
		if(currNode->next==llist->tail){
			llist->tail=currNode;
			free(currNode->next);
		} else {
			NODE* tempNode=currNode->next->next;
			free(currNode->next);
			currNode->next=tempNode;
		}
	}
	llist->size--;
	return;
}
/**
* This function will create a node from the given data and return a pointer to that node.
*/
NODE* create_node(int data)
{
	NODE* newNode=malloc(sizeof(newNode));
	newNode->next=NULL;
	newNode->data=data;
	return newNode;
}
/**
* This function will print a node out in a nicely formatted way. Do whatever you want, just try to make it easy to read.
* Note you will be using this to traverse over the list and print the nodes.
*/
void print_node(NODE* node)
{
	printf("The Current Node's Data is: ");
	printf("%d\n", node->data );
	return;
}
