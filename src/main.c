#include <stdio.h>
#include "../include/tokenizer.h"
#include "../include/value.h"
#include "../include/linkedlist.h"
#include "../include/parser.h"
#include "../include/talloc.h"
#include "../include/interpreter.h"

int main() {

    Value *list = tokenize();
    Value *tree = parse(list);
    interpret(tree);

    tfree();
    return 0;
}
