#include "../include/tokenizer.h"
#include "../include/value.h"
#include "../include/talloc.h"
#include "../include/linkedlist.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

// Return TRUE is c is in a-zA-Z else FALSE
int isLetter(char c) {
    switch(c){
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            return TRUE;
        default:
            return FALSE;
    }
}

// Return TRUE is c is in 0-9 else FALSE
int isDigit(char c) {
    switch(c){
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return TRUE;
        default:
            return FALSE;
    }
}

// Return TRUE is c is in a-zA-Z or is in [!,$, %, &, /, *, :, <, >, =, ?, -, _, ^] else FALSE
int isInitial(char c){
    if(isLetter(c)){
        return TRUE;
    }
    switch(c){
        case '!':
        case '$':
        case '%':
        case '&':
        case '/':
        case '*':
        case ':':
        case '<':
        case '>':
        case '=':
        case '?':
        case '~':
        case '_':
        case '^':
            return TRUE;
        default:
            return FALSE;
    }
}

// Return TRUE if c is in a-zA-Z or is in [!,$, %, &, /, *, :, <, >, =, ?, -, _, ^] or is in 0-9 or is in [., +, -] else FALSE
int isSubsequent(char c){
    if (isInitial(c) || isDigit(c)){
        return TRUE;
    }
    switch(c){
        case '.':
        case '+':
        case '-':
            return TRUE;
        default:
            return FALSE;
    }
}

// Read all of the input from stdin, and return a linked list consisting of the
// tokens.
Value *tokenize(){
    char charRead;
    Value *list = makeNull();
    charRead = (char)fgetc(stdin);

    while (charRead != EOF) {
        // Check if char is a newline or space or if it is a comment
        if (charRead == '\n' || charRead == ' '){
            charRead = (char)fgetc(stdin);
            continue;
        } else if (charRead == ';'){
            char nextChar;
            do {
                nextChar = (char)fgetc(stdin);
            } while (nextChar != EOF && nextChar != '\n');
            if (nextChar == EOF) {
                break;
            } else {
                charRead = (char)fgetc(stdin);
                continue;
            }
        } 
        // initialize new token
        Value *token = talloc(sizeof(Value));
        // Figure out which token type and assign correct value
        if (charRead == '(') {
            token->type = OPEN_TYPE;
        } else if (charRead == ')') {
            token->type = CLOSE_TYPE;
        } else if (charRead == '#') {
            char nextChar = (char)fgetc(stdin);
            if (nextChar == 'f'){
                token->type = BOOL_TYPE;
                token->i = 0;
            } else if (nextChar == 't'){
                token->type = BOOL_TYPE;
                token->i = 1;
            } else {
                printf("Syntax error: got #%c\n", nextChar);
                texit(3);
            }
        } else if (charRead == '"') {
            char str[301];
            str[0] = charRead;
            int count = 1;
            char nextChar;
            do {
                nextChar = (char)fgetc(stdin);
                str[count++] = nextChar;
            } while(nextChar != EOF && nextChar != '"' && count < 300);
            if ((count >= 300 && nextChar != '"') || nextChar == EOF){
                printf("Syntax error: String too long or reach EOF\n");
                texit(4);
            } else {
                str[count++] = '\0';
                token->type = STR_TYPE;
                token->s = talloc(sizeof(char)*count);
                strncpy(token->s, str, count);
            }
        } else if (isInitial(charRead)){
            char identifier[301];
            identifier[0] = charRead;
            int count = 1;
            char nextChar = (char)fgetc(stdin);
            while(nextChar != EOF  && count < 300 && isSubsequent(nextChar)){
                identifier[count++] = nextChar;
                nextChar = (char)fgetc(stdin);
            }
            ungetc(nextChar, stdin);
            identifier[count++] = '\0';
            token->type = SYMBOL_TYPE;
            token->s = talloc(sizeof(char)*(count));
            strncpy(token->s, identifier, count);
        } 
        else if (charRead == '+' || charRead == '-'){
            char nextChar = (char)fgetc(stdin);
            if(isDigit(nextChar)){
                char number[301];
                bool isDecimal = FALSE;
                number[0] = charRead;
                number[1] = nextChar;
                int count = 2;
                nextChar = (char)fgetc(stdin);
                while(nextChar != EOF  && count < 300){
                    if (nextChar == '.'){
                        if (isDecimal){
                            printf("Syntax Error: Too many decimals in number\n");
                            texit(1);
                        } else {
                            isDecimal = TRUE;
                        }
                    } else if (!isDigit(nextChar)){
                        ungetc(nextChar, stdin);
                        break;
                    }
                    number[count++] = nextChar;
                    nextChar = (char)fgetc(stdin);
                }
                number[count] = '\0';
                char *nullptr;
                if (isDecimal) {
                    token->type = DOUBLE_TYPE;
                    token->d = strtod(number, &nullptr);
                } else {
                    token->type = INT_TYPE;
                    token->i = strtol(number, &nullptr, 0);
                }
            } else {
                ungetc(nextChar, stdin);
                token->type = SYMBOL_TYPE;
                token->s = talloc(sizeof(char)*2);
                token->s[0] = charRead;
                token->s[1] = '\0';
            }

        } else if(isDigit(charRead)){
            char number[301];
            bool isDecimal = FALSE;
            number[0] = charRead;
            int count = 1;
            char nextChar = (char)fgetc(stdin);
            while(nextChar != EOF  && count < 300){
                if (nextChar == '.'){
                    if (isDecimal){
                        printf("Syntax Error: Too many decimals in number\n");
                        texit(1);
                    } else {
                        isDecimal = TRUE;
                    }
                } else if (!isDigit(nextChar)){
                    ungetc(nextChar, stdin);
                    break;
                }
                number[count++] = nextChar;
                nextChar = (char)fgetc(stdin);
            }
            number[count] = '\0';
            char *nullptr;
            if (isDecimal) {
                token->type = DOUBLE_TYPE;
                token->d = strtod(number, &nullptr);
            } else {
                token->type = INT_TYPE;
                token->i = strtol(number, &nullptr, 0);
            }
        } else {
            printf("Syntax error: could not match %c\n", charRead);
            texit(2);
        }
        // Add token to list
        list = cons(token, list);
        charRead = (char)fgetc(stdin);
    }
    
    Value *revList = reverse(list);
    return revList; 
}



// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list){
    Value *head = list;
    Value *curr;
    while(head->type != NULL_TYPE){
        curr = car(head);
        switch(curr->type){
            case INT_TYPE:
                printf("%i:integer\n", curr->i);
                break;
            case DOUBLE_TYPE:
                printf("%f:double\n", curr->d);
                break;
            case STR_TYPE:
                printf("%s:string\n", curr->s);
                break;
            case OPEN_TYPE:
                printf("(:open\n");
                break;
            case CLOSE_TYPE:
                printf("):close\n");
                break;
            case BOOL_TYPE:
                if (curr->i){
                    printf("#t:boolean\n");
                } else {
                    printf("#f:boolean\n");
                }
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", curr->s);
                break;
            case PRIMITIVE_TYPE:
            case NULL_TYPE:
            case PTR_TYPE:
            case CONS_TYPE:
            case OPENBRACKET_TYPE:
            case CLOSEBRACKET_TYPE:
            case DOT_TYPE:
            case CLOSURE_TYPE:
            case SINGLEQUOTE_TYPE:
            case UNSPECIFIED_TYPE:
            case VOID_TYPE:
                printf("Error: Not implemented display type in token list\n");
                texit(5);
        }
        head = cdr(head);
    }
}