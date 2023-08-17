#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/linkedlist.h"
#include "../include/value.h"
#include "../include/talloc.h"


// Create a new NULL_TYPE value node.
Value *makeNull(){
    Value *null_value = talloc(sizeof(Value));
    null_value->type = NULL_TYPE;
    return null_value;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr){
    Value *cons_value = talloc(sizeof(Value));
    cons_value->type = CONS_TYPE;
    cons_value->c.car = newCar;
    cons_value->c.cdr = newCdr;
    return cons_value;
}

// Display the contents of the linked list to the screen in some kind of
// readable formata

void display(Value *list){
    switch (list->type){
        case INT_TYPE:
            printf("%i", list->i);
            break;
        case DOUBLE_TYPE:
            printf("%f", list->d);
            break;
        case STR_TYPE:
        case SYMBOL_TYPE:
            printf("%s", list->s);
            break;
        case BOOL_TYPE:
            if(list->i){
                printf("#t");
            }else {
                printf("#f");
            }
            break;
        case CONS_TYPE: {
            Value *cur = list;
            printf("(");
            while(cur->type != NULL_TYPE){
                display(car(cur));
                
                if(cdr(cur)->type != CONS_TYPE && cdr(cur)->type != NULL_TYPE && car(cur)->type != VOID_TYPE){
                    printf(" . ");
                    display(cdr(cur));
                    break;
                } else if(cdr(cur)->type != NULL_TYPE && car(cdr(cur))->type != VOID_TYPE){
                    printf(" ");
                }

                cur = cdr(cur);
            }
        
            
            printf(")");
            break;
        }
        case CLOSURE_TYPE:
            printf("#<procedure>");
            break;
        case PRIMITIVE_TYPE:
        case VOID_TYPE:
        case NULL_TYPE:
        case PTR_TYPE:
        case OPEN_TYPE:
        case CLOSE_TYPE:
        case OPENBRACKET_TYPE:
        case CLOSEBRACKET_TYPE:
        case DOT_TYPE:
        case SINGLEQUOTE_TYPE:
        case UNSPECIFIED_TYPE:
            break;
    }
}

// Prints Value Type for debugging
void printDEBUG(Value *list){
    switch (list->type){
        case INT_TYPE:
            printf("INT_TYPE\n");
            break;
        case DOUBLE_TYPE:
            printf("DOUBLE_TYPE\n");
            break;
        case STR_TYPE:
            printf("STR_TYPE\n");
            break;
        case SYMBOL_TYPE:
            printf("SYMBOL_TYPE\n");
            break;
        case BOOL_TYPE:
            printf("BOOL_TYPE\n");
            break;
        case CONS_TYPE:
            printf("CONS_TYPE\n");
            break;
        case NULL_TYPE:
            printf("NULL_TYPE\n");
            break;
        case PTR_TYPE:
            printf("PTR_TYPE\n");
            break;
        case OPEN_TYPE:
            printf("OPEN_TYPE\n");
            break;
        case CLOSE_TYPE:
            printf("CLOSE_TYPE\n");
            break;
        case CLOSURE_TYPE:
            printf("CLOSURE_TYPE\n");
            break;
        case OPENBRACKET_TYPE:
            printf("OPENBRACKET_TYPE\n");
            break;
        case CLOSEBRACKET_TYPE:
            printf("CLOSEBRAcKET_TYPE\n");
            break;
        case DOT_TYPE:
            printf("DOT_TYPE\n");
            break;
        case SINGLEQUOTE_TYPE:
            printf("SINGLEQUOTE_TYPE\n");
            break;
        case VOID_TYPE:
            printf("VOID_TYPE\n");
            break;
        case PRIMITIVE_TYPE:
            printf("PRIMITIVE_TYPE\n");
            break;
        case UNSPECIFIED_TYPE:
            printf("UNSPECIFIED_TYPE\n");
            break;
        default:
            printf("ERROR\n");
    }
    
}

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory whatsoever between the original list and the new one.
//
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
Value *reverse(Value *list){
    Value *prev = makeNull();
    Value *curr = list;
    while(!(isNull(curr))){
        Value *copy = talloc(sizeof(Value));
        copy->type = car(curr)->type;
        switch (car(curr)->type){
        case INT_TYPE:
        case BOOL_TYPE:
            copy->i = car(curr)->i;
            break;
        case DOUBLE_TYPE:
            copy->d = car(curr)->d;
            break;
        case STR_TYPE:
        case SYMBOL_TYPE:
            copy->s = car(curr)->s;
            break;
        case CONS_TYPE:
            copy->c = car(curr)->c;
        case PRIMITIVE_TYPE:
        case OPEN_TYPE:
        case CLOSE_TYPE:
        case CLOSURE_TYPE:
        case NULL_TYPE:
        case PTR_TYPE:
        case OPENBRACKET_TYPE:
        case CLOSEBRACKET_TYPE:
        case DOT_TYPE:
        case UNSPECIFIED_TYPE:
        case SINGLEQUOTE_TYPE:
        case VOID_TYPE:
            break;
        }
        Value *cons_cell = cons(copy, prev);
        prev = cons_cell;
        curr = cdr(curr);
    }
    return prev;
}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list){
    assert(list != NULL);
    assert(list->type == CONS_TYPE);
    return list->c.car;
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list){
    assert(list != NULL);
    assert(list->type == CONS_TYPE);
    return list->c.cdr;
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value){
    assert(value != NULL);
    return value->type == NULL_TYPE;
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value){
    assert(value != NULL);
    assert(value->type == CONS_TYPE || value->type == NULL_TYPE);
    int len = 0;
    Value *curr = value;
    while(!(isNull(curr))){
        len++;
        curr = cdr(curr);
    }
    return len;
}