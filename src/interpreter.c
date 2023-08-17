#include "../include/interpreter.h"
#include "../include/value.h"
#include "../include/talloc.h"
#include <stdio.h>
#include "../include/linkedlist.h"
#include <string.h>

// primitive function that binds to '+' and adds all args together
Value *primitiveAdd(Value *args) {
    Value *result = talloc(sizeof(Value));
    result->i = 0;
    result->type = INT_TYPE;
    Value *cur = args;
    while(!isNull(cur)){
        if(car(cur)->type == DOUBLE_TYPE){
            result->d = result->i + car(cur)->d;
            result->type = DOUBLE_TYPE;
            cur = cdr(cur);
            break;
        } else if(car(cur)->type == INT_TYPE) {
            result->i += car(cur)->i;
        } else {
            printf("Evaluation error: plus given non-valid type\n");
            texit(-1);
        }
        cur = cdr(cur);
    }
    while(!isNull(cur)){
            if(car(cur)->type == DOUBLE_TYPE){
            result->d += car(cur)->d;
            } else if(car(cur)->type == INT_TYPE) {
            result->d += car(cur)->i;
            } else {
            printf("Evaluation error: plus given non-valid type\n");
            texit(-1);
        }
        cur = cdr(cur);
    }
    return result;
}

Value *primitiveMultiply(Value *args) {
    Value *result = talloc(sizeof(Value));
    result->i = 1;
    result->type = INT_TYPE;
    Value *cur = args;
    while(!isNull(cur)){
        if(car(cur)->type == DOUBLE_TYPE){
            result->d = result->i * car(cur)->d;
            result->type = DOUBLE_TYPE;
            cur = cdr(cur);
            break;
        } else if(car(cur)->type == INT_TYPE) {
            result->i *= car(cur)->i;
        } else {
            printf("Evaluation error: plus given non-valid type\n");
            texit(-1);
        }
        cur = cdr(cur);
    }
    while(!isNull(cur)){
        if(car(cur)->type == DOUBLE_TYPE){
            result->d *= car(cur)->d;
        } else if(car(cur)->type == INT_TYPE) {
            result->d *= car(cur)->i;
        } else {
            printf("Evaluation error: plus given non-valid type\n");
            texit(-1);
        }
        cur = cdr(cur);
    }
    return result;
}

// primitive function that binds to 'null?' and returns true if args is null else false
Value *primitiveIsNull(Value *args) {
    if(isNull(args)){
        printf("Evaluation error: null? has no arguments\n");
        texit(-1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    if(isNull(car(args)) || (car(args)->type == CONS_TYPE && car(car(args))->type == VOID_TYPE)){
        result->i = 1;
    } else {
        result->i = 0;
    }
    if(!isNull(cdr(args))){
        printf("Evaluation error: null? has more than one argument\n");
        texit(-1);
    }
    return result;
}

// primitive function that binds to 'car' returns car of a con-cell
Value *primitiveCar(Value *args) {
    if(isNull(args) || car(args)->type != CONS_TYPE){
        printf("Evaluation error: car has no arguments\n");
        texit(-1);
    } else if(!isNull(cdr(args))){
        printf("Evaluation error: car has more than one argument\n");
        texit(-1);
    }
    
    return car(car(args));
}

// primitive function that binds to 'cdr' returns cdr of a con-cell
Value *primitiveCdr(Value *args) {
    if(isNull(args) || car(args)->type != CONS_TYPE){
        printf("Evaluation error: cdr has no arguments\n");
        texit(-1);
    } else if(!isNull(cdr(args))){
        printf("Evaluation error: cdr has more than one argument\n");
        texit(-1);
    }
    return cdr(car(args));
}

// primitive function that binds to 'cons' returns a con-cell
// where the car is the first arg and cdr is the second arg in args
Value *primitiveCons(Value *args){
    if(isNull(args)){
        printf("Evaluation error: cons has no arguments\n");
        texit(-1);
    } else if(isNull(cdr(args))){
        printf("Evaluation error: cons has only one argument\n");
        texit(-1);
    } else if(!isNull(cdr(cdr(args)))){
        printf("Evaluation error: cons has more than two arguments\n");
        texit(-1);
    }
    return cons(car(args), car(cdr(args)));
}

// performs subtraction
Value *primitiveMinus(Value *args){
    if(isNull(args)){
        printf("Evaluation error: '-' has no arguments\n");
        texit(-1);
    } else if(isNull(cdr(args))){
        printf("Evaluation error: '-' has only one argument\n");
        texit(-1);
    } else if(!isNull(cdr(cdr(args)))){
        printf("Evaluation error: '-' has more than two arguments\n");
        texit(-1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = DOUBLE_TYPE;
    if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        result->d = car(args)->d - car(cdr(args))->d;
    } else if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        result->d = car(args)->d - car(cdr(args))->i;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        result->d = car(args)->i - car(cdr(args))->d;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        result->type = INT_TYPE;
        result->i = car(args)->i - car(cdr(args))->i;
    } else {
        printf("Evaluation error: '-' has a non (integer/double) arg\n");
        texit(-5);
    }
    return result;
}

// performs greater than comparison
Value *primitiveGreaterThan(Value *args){
    if(isNull(args)){
        printf("Evaluation error: '>' has no arguments\n");
        texit(-1);
    } else if(isNull(cdr(args))){
        printf("Evaluation error: '>' has only one argument\n");
        texit(-1);
    } else if(!isNull(cdr(cdr(args)))){
        printf("Evaluation error: '>' has more than two arguments\n");
        texit(-1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    int comparison;
    if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        comparison = car(args)->d > car(cdr(args))->d;
    } else if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        comparison = car(args)->d > car(cdr(args))->i;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        comparison = car(args)->i > car(cdr(args))->d;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        comparison = car(args)->i > car(cdr(args))->i;
    } else {
        printf("Evaluation error: '>' has a non (integer/double) arg\n");
        texit(-5);
    }
    result->i = comparison;
    return result;
}

Value *primitiveLessThan(Value *args){
    if(isNull(args)){
        printf("Evaluation error: '<' has no arguments\n");
        texit(-1);
    } else if(isNull(cdr(args))){
        printf("Evaluation error: '<' has only one argument\n");
        texit(-1);
    } else if(!isNull(cdr(cdr(args)))){
        printf("Evaluation error: '<' has more than two arguments\n");
        texit(-1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    int comparison;
    if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        comparison = car(args)->d < car(cdr(args))->d;
    } else if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        comparison = car(args)->d < car(cdr(args))->i;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        comparison = car(args)->i < car(cdr(args))->d;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        comparison = car(args)->i < car(cdr(args))->i;
    } else {
        printf("Evaluation error: '<' has a non (integer/double) arg\n");
        texit(-5);
    }
    result->i = comparison;
    return result;
}

Value *primitiveEqual(Value *args){
    if(isNull(args)){
        printf("Evaluation error: '=' has no arguments\n");
        texit(-1);
    } else if(isNull(cdr(args))){
        printf("Evaluation error: '=' has only one argument\n");
        texit(-1);
    } else if(!isNull(cdr(cdr(args)))){
        printf("Evaluation error: '=' has more than two arguments\n");
        texit(-1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    int comparison;
    if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        comparison = car(args)->d == car(cdr(args))->d;
    } else if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        comparison = car(args)->d == car(cdr(args))->i;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        comparison = car(args)->i == car(cdr(args))->d;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        comparison = car(args)->i == car(cdr(args))->i;
    } else {
        printf("Evaluation error: '=' has a non (integer/double) arg\n");
        texit(-5);
    }
    result->i = comparison;
    return result;
}

Value *primitiveModulo(Value *args){
    if(isNull(args) || car(args)->type != INT_TYPE){
        printf("Evaluation error: modulo has no arguments or first argument not int type\n");
        texit(-1);
    } else if(isNull(cdr(args)) || car(cdr(args))->type != INT_TYPE){
        printf("Evaluation error: modulo has only one argument or second argument not int type\n");
        texit(-1);
    } else if(!isNull(cdr(cdr(args)))){
        printf("Evaluation error: modulo has more than two arguments\n");
        texit(-1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = INT_TYPE;
    result->i = car(args)->i % car(cdr(args))->i;
    return result;
}

Value *primitiveDivide(Value *args){
    if(isNull(args)){
        printf("Evaluation error: / has no arguments\n");
        texit(-1);
    } else if(isNull(cdr(args))){
        printf("Evaluation error: / has only one argument\n");
        texit(-1);
    } else if(!isNull(cdr(cdr(args)))){
        printf("Evaluation error: / has more than two arguments\n");
        texit(-1);
    }
    Value *result = talloc(sizeof(Value));
    result->type = DOUBLE_TYPE;
    if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        result->d = car(args)->d / car(cdr(args))->d;
    } else if(car(args)->type == DOUBLE_TYPE && car(cdr(args))->type == INT_TYPE){
        result->d = car(args)->d / car(cdr(args))->i;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == DOUBLE_TYPE){
        result->d = car(args)->i / car(cdr(args))->d;
    } else if(car(args)->type == INT_TYPE && car(cdr(args))->type == INT_TYPE){
        if(car(args)->i % car(cdr(args))->i != 0){
            result->d = car(args)->i;
            result->d /= car(cdr(args))->i;
        } else {
            result->type = INT_TYPE;
            result->i = car(args)->i / car(cdr(args))->i;
        }
    } else {
        printf("Evaluation error: / has a non (integer/double) arg\n");
        texit(-5);
    }
    return result;
}


// Add primitive functions to top-level bindings list
void bind(char *name, Value *(*function)(struct Value *), Frame *frame) {
    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;
    Value *nameCell = talloc(sizeof(Value));
    nameCell->type = SYMBOL_TYPE;
    nameCell->s = name;
    frame->bindings = cons(cons(nameCell, value), frame->bindings);
}

// Top level go through each S-expression
void interpret(Value *tree){
    Frame *frame = talloc(sizeof(Frame));
    frame->parent = NULL;
    frame->bindings = makeNull();

    bind("+", primitiveAdd, frame);
    bind("null?", primitiveIsNull, frame);
    bind("car", primitiveCar, frame);
    bind("cdr", primitiveCdr, frame);
    bind("cons", primitiveCons, frame);
    bind("-", primitiveMinus, frame);
    bind("<", primitiveLessThan, frame);
    bind(">", primitiveGreaterThan, frame);
    bind("=", primitiveEqual, frame);
    bind("*", primitiveMultiply, frame);
    bind("/", primitiveDivide, frame);
    bind("modulo", primitiveModulo, frame);    
   
    Value *temp;
    while(tree->type != NULL_TYPE){
        temp = eval(car(tree), frame);
        if(temp->type != VOID_TYPE){
            display(temp);
            printf("\n");
        }
        tree = cdr(tree);

    }
    printf("\n");
}

// looks for symbol in current frame before moving up a frame to find symbol
Value *lookUpSymbol(Value *tree, Frame *frame){
    char *symbol = tree->s;
    Frame *cur_frame = frame;
    Value *cur_binding;
    while(cur_frame != NULL){
        cur_binding = cur_frame->bindings;
        while(cur_binding->type != NULL_TYPE){
            if(!strcmp(car(car(cur_binding))->s, symbol)){
                Value *symbol_value = cdr(car(cur_binding));
                if(symbol_value->type == SYMBOL_TYPE){
                    symbol = symbol_value->s;
                    break;
                } else if(symbol_value->type == CONS_TYPE){
                    return symbol_value;
                }
                return symbol_value;
            }
            cur_binding = cdr(cur_binding);
        }
        cur_frame = cur_frame->parent;
    }
    printf("Evaluation error: symbol '%s' not found.\n", symbol);
    texit(-2);
    return NULL;
}

void set_var(Value *var, Value *val, Frame *frame){
    char *symbol = var->s;
    Frame *cur_frame = frame;
    Value *cur_binding;
    while(cur_frame != NULL){
        cur_binding = cur_frame->bindings;
        while(cur_binding->type != NULL_TYPE){
            if(!strcmp(car(car(cur_binding))->s, symbol)){
                car(cur_binding)->c.cdr = eval(val, frame);
                return;
            }
            cur_binding = cdr(cur_binding);
        }
        cur_frame = cur_frame->parent;
    }
    printf("Evaluation error: symbol '%s' not found.\n", symbol);
    texit(-2);
}

// Creates new frame with variable bindings
Frame *bindVariables(Value *list_of_bindings, Frame *frame){
    if(list_of_bindings->type != CONS_TYPE && list_of_bindings->type != NULL_TYPE){
        printf("Evaluation error:  bad form in let\n");
        texit(-2);
    }
    Frame *new_frame = talloc(sizeof(Frame));
    new_frame->parent = frame;
    new_frame->bindings = makeNull();
    Value *cur = list_of_bindings;
    while(cur->type != NULL_TYPE){
        if(isNull(car(cur))){
            printf("Evaluation error: null binding in let\n");
            texit(-2);
        }
        if(car(cur)->type != CONS_TYPE){
            printf("Evaluation error:  bad form in let\n");
            texit(-2);
        }
        Value *var_name = car(car(cur));
        if(var_name->type != SYMBOL_TYPE){
            printf("Evaluation error: left side of a let pair doesn't have a variable\n");
            texit(-2);
        }
        // Check for duplicate variable in let
        Value *cur2 = new_frame->bindings;
        while(cur2->type != NULL_TYPE){
            if(!strcmp(car(car(cur2))->s, var_name->s)){
                printf("Evaluation error: duplicate variable in let\n");
                texit(-2);
            }
            cur2 = cdr(cur2);
        }

        Value *var_value = car(cdr(car(cur)));
        new_frame->bindings = cons(cons(var_name, eval(var_value, frame)), new_frame->bindings);
        cur = cdr(cur);
    }
    return new_frame;
}

// Creates new frame for each variable with variable value
Frame *bindVariablesLetStar(Value *list_of_bindings, Frame *frame){
    if(list_of_bindings->type != CONS_TYPE && list_of_bindings->type != NULL_TYPE){
        printf("Evaluation error:  bad form in let\n");
        texit(-2);
    }
    Value *cur = list_of_bindings;
    if(cur->type == NULL_TYPE) {
        return frame;
    }
    Frame *temp_parent = frame;
    Frame *new_frame;
    while(cur->type != NULL_TYPE){
        new_frame = talloc(sizeof(Frame));
        new_frame->parent = temp_parent;
        new_frame->bindings = makeNull();
        if(isNull(car(cur))){
            printf("Evaluation error: null binding in let\n");
            texit(-2);
        }
        if(car(cur)->type != CONS_TYPE){
            printf("Evaluation error:  bad form in let\n");
            texit(-2);
        }
        Value *var_name = car(car(cur));
        if(var_name->type != SYMBOL_TYPE){
            printf("Evaluation error: left side of a let pair doesn't have a variable\n");
            texit(-2);
        }
        
        Value *var_value = car(cdr(car(cur)));
        new_frame->bindings = cons(cons(var_name, eval(var_value, temp_parent)), new_frame->bindings);
        temp_parent = new_frame;
        cur = cdr(cur);
    }
    return new_frame;
}

// Creates new frame with variable bindings as defined in letrec
Frame *bindLetrec(Value *list_of_bindings, Frame *frame){
    if(list_of_bindings->type != CONS_TYPE && list_of_bindings->type != NULL_TYPE){
        printf("Evaluation error: bad form in letrec\n");
        texit(-2);
    }
    Value *unspec = talloc(sizeof(Value));
    unspec->type = UNSPECIFIED_TYPE;
    Frame *new_frame = talloc(sizeof(Frame));
    new_frame->parent = frame;
    new_frame->bindings = makeNull();
    Value *cur = list_of_bindings;
    Value *temp_value = makeNull();
    while(cur->type != NULL_TYPE){
        if(isNull(car(cur))){
            printf("Evaluation error: null binding in letrec\n");
            texit(-2);
        }
        if(car(cur)->type != CONS_TYPE){
            printf("Evaluation error:  bad form in letrec\n");
            texit(-2);
        }
        Value *var_name = car(car(cur));
        if(var_name->type != SYMBOL_TYPE){
            printf("Evaluation error: left side of a let pair doesn't have a variable\n");
            texit(-2);
        }
        // Check for duplicate variable in let
        Value *cur2 = new_frame->bindings;
        while(cur2->type != NULL_TYPE){
            if(!strcmp(car(car(cur2))->s, var_name->s)){
                printf("Evaluation error: duplicate variable in letrec\n");
                texit(-2);
            }
            cur2 = cdr(cur2);
        }

        Value *var_value = car(cdr(car(cur)));
        temp_value = cons(var_value, temp_value);
        new_frame->bindings = cons(cons(var_name, unspec), new_frame->bindings);
        cur = cdr(cur);
    }
    cur = temp_value;
    while(cur->type != NULL_TYPE){
        cur->c.car = eval(car(cur), new_frame);
        if(car(cur)->type == UNSPECIFIED_TYPE){
            printf("Evaluation error: Letrec arg evaluated to UNSPCECIFIED_TYPE\n");
            texit(-3);
        }
        cur = cdr(cur);
    }
    Value *cur_var = new_frame->bindings;
    Value *cur_value = temp_value;
    while(cur_var->type != NULL_TYPE){
        car(cur_var)->c.cdr = car(cur_value);
        cur_value = cdr(cur_value);
        cur_var = cdr(cur_var);
    }
    return new_frame;
}

//Takes a closure, parameters and frame.
//Evaluates parameters in frame then adds it to new frame used to evaluate closure.
Value *evalClosure(Value *closure, Value *params, Frame *frame){
    Frame *param_frame = talloc(sizeof(Frame));
    param_frame->parent = closure->cl.frame;
    param_frame->bindings = makeNull();
    Value *cur_binding = closure->cl.paramNames;
    if (car(cur_binding)->type != VOID_TYPE){
        Value *cur_arg = params;
        while(cur_binding->type != NULL_TYPE){
            if(cur_arg->type == NULL_TYPE){
                printf("Evaluation error: param has no corresponding value\n");
                texit(-11);
            }
            param_frame->bindings = cons(cons(car(cur_binding), car(cur_arg)), param_frame->bindings);
            cur_binding = cdr(cur_binding);
            cur_arg = cdr(cur_arg);
        }
    }
    return eval(closure->cl.functionCode, param_frame);
}

Value *evalAnd(Value *args, Frame *frame){
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    result->i = 0;
    if(isNull(args)){
        return result;
    }
    result->i = 1;
    Value *cur = args;
    while(!isNull(cur)){
        if(!eval(car(cur), frame)->i){
            result->i = 0;
            return result;
        }
        cur = cdr(cur);
    }
    return result;
}

Value *evalOr(Value *args, Frame *frame){
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    result->i = 0;
    Value *cur = args;
    while(!isNull(cur)){
        if(eval(car(cur), frame)->i){
            result->i = 1;
            return result;
        }
        cur = cdr(cur);
    }
    return result;
}

// Evaluate if expression and return evaluated branch.
Value *evalIf(Value *args, Frame *frame){
    Value *bool_expr = car(args);
    if(isNull(bool_expr)){
        printf("Evaluation error: no bool expression\n");
        texit(-3);
    }
    Value *result = eval(bool_expr, frame);
    if(result->type != BOOL_TYPE){
        printf("Evaluation error: bool expr not evaluate to bool\n");
        texit(-3);
    }
    if(isNull(cdr(args))){
        printf("Evaluation error: no cdr\n");
        texit(-3);
    }
    Value *if_true = car(cdr(args));
    if(isNull(if_true)){
        printf("Evaluation error: no if_true expression\n");
        texit(-3);
    }
    if(isNull(cdr(cdr(args)))){
        printf("Evaluation error: no cdr\n");
        texit(-3);
    }
    Value *if_false = car(cdr(cdr(args)));
    if(isNull(if_false)){
        printf("Evaluation error: no if_true expression\n");
        texit(-3);
    }
    if(!isNull(cdr(cdr(cdr(args))))){
        printf("Evaluation error: if statement has too many args\n");
        texit(-3);
    }
    if(result->i){
        return eval(if_true, frame);
    } else {
        Value* temp = eval(if_false, frame);
        return eval(if_false, frame);
    }
    
}

Value *evalCond(Value *args, Frame *frame){
    Value *cur = args;
    while(!isNull(cur)){
        Value *temp = car(car(cur));
        if(temp->type == SYMBOL_TYPE && !strcmp(temp->s,"else")){
            return eval(car(cdr(car(cur))), frame);
        } else {
            temp = eval(temp, frame);
        }
        if(temp->type == BOOL_TYPE && temp->i){
            return eval(car(cdr(car(cur))), frame);
        }
        cur = cdr(cur);
    }
    Value *void_val = talloc(sizeof(Value));
    void_val->type = VOID_TYPE;
    return void_val;
}

// evaluates an S-expression
Value *eval(Value *tree, Frame *frame) {
    switch (tree->type)  {
        case BOOL_TYPE:
        case INT_TYPE:
        case DOUBLE_TYPE:
        case STR_TYPE:
        case VOID_TYPE:
            return tree;
            break;
        case SYMBOL_TYPE:
            return lookUpSymbol(tree, frame);
            break;
        case CONS_TYPE: {
            Value *first = car(tree);
            Value *args = cdr(tree);
            // Sanity and error checking on first...
            if (first->type == SYMBOL_TYPE){
                if (!strcmp(first->s,"if")) {
                    return evalIf(args,frame);
                }else if (!strcmp(first->s,"let*")) {
                    Value *list_of_bindings = car(args);
                    Value *body = cdr(args);
                    if(isNull(body)){
                        printf("Evaluation error: no args following the bindings in let\n");
                        texit(-12);
                    }
                    
                    
                    Frame *new_frame = bindVariablesLetStar(list_of_bindings, frame);
                    Value *result;
                    while (body->type != NULL_TYPE)
                    {
                        
                        result = eval(car(body), new_frame);
                        body = cdr(body);
                    }
                    return result;
                } else if (!strcmp(first->s,"let")) {
                    Value *list_of_bindings = car(cdr(tree));
                    if(isNull(cdr(cdr(tree)))){
                        printf("Evaluation error: no args following the bindings in let\n");
                        texit(-12);
                    }
                    Value *body = cdr(cdr(tree));
                    
                    Frame *new_frame = bindVariables(list_of_bindings, frame);
                    Value *result;
                    while (body->type != NULL_TYPE)
                    {
                        result = eval(car(body), new_frame);
                        body = cdr(body);
                    }

                    return result;
                } else if (!strcmp(first->s,"letrec")) {
                    Value *list_of_bindings = car(cdr(tree));
                    if(isNull(cdr(cdr(tree)))){
                        printf("Evaluation error: no args following the bindings in let\n");
                        texit(-12);
                    }
                    Value *body = cdr(cdr(tree));
                    
                    Frame *new_frame = bindLetrec(list_of_bindings, frame);
                    Value *result;
                    while (body->type != NULL_TYPE)
                    {
                        result = eval(car(body), new_frame);
                        body = cdr(body);
                    }

                    return result;
                } else if (!strcmp(first->s,"set!")) {
                    if(isNull(args) || isNull(car(args))){
                        printf("Evaluation error: no set data\n");
                        texit(-4);
                    }
                    set_var(car(args), car(cdr(args)), frame);
                    Value *void_val = talloc(sizeof(Value));
                    void_val->type = VOID_TYPE;
                    return void_val;
                } else if (!strcmp(first->s,"begin")) {
                    if(isNull(args)){
                        Value *void_val = talloc(sizeof(Value));
                        void_val->type = VOID_TYPE;
                        return void_val;
                    }
                    Value *cur = args;
                    Value *result;
                    while (cur->type != NULL_TYPE)
                    {
                        result = eval(car(cur), frame);
                        cur = cdr(cur);
                    }

                    return result;
                } else if (!strcmp(first->s,"cond")) {
                    return evalCond(args, frame);
                } else if (!strcmp(first->s,"and")) {
                    return evalAnd(args, frame);
                } else if (!strcmp(first->s,"or")) {
                    return evalOr(args, frame);
                } else if (!strcmp(first->s,"quote")) {
                    if(isNull(args) || isNull(car(args)) || !isNull(cdr(args))){
                        printf("Evaluation error: no quote data\n");
                        texit(-4);
                    }
                    return car(args);
                } else if (!strcmp(first->s, "define")){
                    if(isNull(args) || isNull(car(args)) || isNull(cdr(args))){
                        printf("Evaluation error: no args following the bindings in define\n");
                        texit(-13);
                    }
                    if(car(args)->type != SYMBOL_TYPE){
                        printf("Evaluation error: define var name not symbol type\n");
                        texit(-13);
                    }
                    frame->bindings = cons((cons(car(args), eval(car(cdr(args)), frame))), frame->bindings);
                    Value *void_val = talloc(sizeof(Value));
                    void_val->type = VOID_TYPE;
                    return void_val;
                } else if (!strcmp(first->s, "lambda")){
                    if(isNull(args) || isNull(car(args)) || isNull(cdr(args))){
                        printf("Evaluation error: no args following the bindings in define\n");
                        texit(-14);
                    }
                    Value *closure = talloc(sizeof(Value));
                    Value *cur_param = car(args);
                    if (car(cur_param)->type != VOID_TYPE){
                        while(cur_param->type != NULL_TYPE){
                            if(car(cur_param)->type != SYMBOL_TYPE){
                                printf("Evaluation error: param name is not a symbol\n");
                                texit(-1);
                            }
                            cur_param = cdr(cur_param);
                        }
                        cur_param = car(args);
                        Value *temp;
                        while(cur_param->type != NULL_TYPE){
                            temp = cdr(cur_param);
                            while(temp->type != NULL_TYPE){
                                if(!strcmp(car(cur_param)->s, car(temp)->s)){
                                    printf("Evaluation error: duplicate param name\n");
                                    texit(-1);
                                }
                                temp = cdr(temp);
                            }
                            cur_param = cdr(cur_param);
                        }
                    }
                    closure->cl.paramNames = car(args);
                    closure->cl.functionCode = car(cdr(args));
                    closure->cl.frame = frame;
                    closure->type = CLOSURE_TYPE;
                    return closure;
                }
            }
            first = eval(first, frame);
            Value *evalledArgs = makeNull();
            Value *curArg = args;
            while(!isNull(curArg)){
                Value *temp = eval(car(curArg), frame);
                evalledArgs = cons(temp, evalledArgs);
                curArg = cdr(curArg);
            }
            evalledArgs = reverse(evalledArgs);
            if(first->type == SYMBOL_TYPE){
                first = lookUpSymbol(first, frame);
            }
            if(first->type == CLOSURE_TYPE){
                return evalClosure(first, evalledArgs, frame);
            } else {                    
                return first->pf(evalledArgs);
            }
            break;
        }
        default:
            printf("Evaluation error: default case\n");
            texit(-16);
    }
    printf("Evaluation error: ERROR\n");
    texit(-17);
    return NULL;
}