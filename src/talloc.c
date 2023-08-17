#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/linkedlist.h"
#include "../include/talloc.h"

//Root node for the garbage collection linked list
Node *headNode = NULL;

// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in some kind of list; a linked list would do fine, but insert
// here whatever code you'll need to do so; don't call functions in the
// pre-existing linkedlist.h. Otherwise you'll end up with circular
// dependencies, since you're going to modify the linked list to use talloc.
void *talloc(size_t size){
    void *new = malloc(size);
    Node *newNode = malloc(sizeof(Node));
    newNode->pointer = new;
    newNode->next = headNode;
    headNode = newNode;
    return new;
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree(){
    Node *curr = headNode;
    Node *next = headNode;
    while(curr != NULL){
        next = curr->next;
        free(curr->pointer);
        free(curr);
        curr = next;
    }
    headNode = NULL;
}

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status){
    tfree();
    exit(status);
}