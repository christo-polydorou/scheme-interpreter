#include <stdio.h>
#include <assert.h>
#include "../include/parser.h"
#include "../include/linkedlist.h"
#include "../include/value.h"
#include "../include/talloc.h"

// My addToParseTree function takes in a pre-existing tree, a token to add 
// to it, and a pointer to an integer depth. depth is updated to represent 
// the number of unclosed open parentheses in the parse tree.
Value *addToParseTree(Value *tree, int *depth, Value *token){
    if(token->type == CLOSE_TYPE){
        Value *cur = tree;
        Value *subtree = makeNull();
        while(cur->type != NULL_TYPE && car(cur)->type != OPEN_TYPE){
            subtree = cons(car(cur), subtree);
            cur = cdr(cur);
        }
        if(cur->type == NULL_TYPE){
            printf("Syntax error: too many close parentheses\n");
            texit(1);
        }
        if(isNull(subtree)){
            Value *void_val = talloc(sizeof(Value));
            void_val->type = VOID_TYPE;
            tree = cons(cons(void_val, subtree), cdr(cur));
        } else {
            tree = cons(subtree, cdr(cur));
        }
        (*depth)--;
    }
    else {
        if (token->type == OPEN_TYPE){
            (*depth)++;
        }
        tree = cons(token, tree);
    }
    return tree;
}

// Takes a list of tokens from a Scheme program, and returns a pointer to a
// parse tree representing that program.
Value *parse(Value *tokens) {
    Value *tree = makeNull();
    int depth = 0;

    Value *current = tokens;
    assert(current != NULL && "Error (parse): null pointer");
    while (current->type != NULL_TYPE) {
        Value *token = car(current);
        tree = addToParseTree(tree, &depth, token);
        current = cdr(current);
    }
    if (depth != 0) {
        printf("Syntax error: not enough close parentheses");
    }
    current = tree;
    tree = makeNull();
    while(current->type != NULL_TYPE){
        tree = cons(car(current), tree);
        current = cdr(current);
    }
    // printTree(tree);
    return tree;
}


// Prints the tree to the screen in a readable fashion. It should look just like
// Scheme code; use parentheses to indicate subtrees.
void printTree(Value *tree){
    display(tree);
    printf("\n");
}

