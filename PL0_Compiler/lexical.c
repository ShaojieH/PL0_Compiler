//
// Created by Tom on 2018/10/9.
//
#include <stdlib.h>
#include "lexical.h"
#include "helper.h"
const int ReservedSize = 12;
char *Reserved[12] = {
        "const", "var", "begin", "end",
        "procedure", "if", "then", "while",
        "do", "call", "read", "write"};
const char PlusOrMinus[] = {'+', '-'};
const char MulOrDiv[] = {'*', '/'};
const int CmpSize = 6;
char *Cmp[6] = {"<", ">", "<=", ">=", "=", "<>"};
const char Parentheses[] = {'(', ')'};



void getNextSymbol() {
    clearSpaceAndReadChar();

    if (getLetter() != -1) {    //id
        getNextID();
        int reserve = getReserve();
        if (reserve != -1) {  //reserve
            currentSymbol = (Symbol) {.type=RESERVED, .value =  malloc(sizeof(int))};
            currentSymbol.value->number_or_index = reserve;
        } else {    //identifier
            currentSymbol = (Symbol) {.type=ID, .value = malloc(sizeof(Value))};
            currentSymbol.value->identifier = malloc(sizeof(char) * (strlen(currentID)+1));
			my_strcpy(currentSymbol.value->identifier, currentID);
        }
    } else if (getDigit() != -1) {   //number_or_index
        int number = 0;
        while (getDigit() != -1) {
            number = 10 * number + getDigit();
            getNextChar();
        }
        if (isDelimiter()) {
            currentSymbol = (Symbol) {.type=NUMBER, .value = malloc(sizeof(int))};
            currentSymbol.value->number_or_index = number;
        } else {
            error("get next symbol");
        }
    } else if (getPlusOrMinus() != -1) { //+-
        currentSymbol = (Symbol) {.type=PLUS_OR_MINUS, .value = malloc(sizeof(int))};
        currentSymbol.value->number_or_index = getPlusOrMinus();
        getNextChar();
    } else if (getMulOrDiv() != -1) {   //*/
        currentSymbol = (Symbol) {.type=MUL_OR_DIV, .value= malloc(sizeof(int))};
        currentSymbol.value->number_or_index = getMulOrDiv();
        getNextChar();
    } else if (isSemi()) {    //;
        currentSymbol = (Symbol) {.type=SEMI, .value = 0};
        getNextChar();
    } else if (isComma()) {   //,
        currentSymbol = (Symbol) {.type=COMMA, .value = 0};
        getNextChar();
    } else if (isLParen()) {   //(
        currentSymbol = (Symbol) {.type=LPAREN, .value = 0};
        getNextChar();
    } else if (isRParen()) {   //)
        currentSymbol = (Symbol) {.type=RPAREN, .value = 0};
        getNextChar();
    } else if (isCmp()) {    //<>
        currentSymbol = (Symbol) {.type=CMP, .value = malloc(sizeof(int))};
        currentSymbol.value->number_or_index = getCmp();
    } else if (isAssign()) {  //:=
        currentSymbol = (Symbol) {.type=ASSIGN, .value=0};
        getNextChar();
    }
    // printCurrentSymbol(currentSymbol);
    // return currentSymbol;
}

void printCurrentSymbol(Symbol symbol) {
    switch (symbol.type) {
        case ID:
            printf("identifier: %s\n", symbol.value->identifier);
            break;
        case RESERVED:
            printf("reserved word: %s\n", Reserved[symbol.value->number_or_index]);
            break;
        case NUMBER:
            printf("number_or_index: %d\n", symbol.value->number_or_index);
            break;
        case PLUS_OR_MINUS:
            printf("plus or minus: %c\n", PlusOrMinus[symbol.value->number_or_index]);
            break;
        case MUL_OR_DIV:
            printf("mul or div: %c\n", MulOrDiv[symbol.value->number_or_index]);
            break;
        case SEMI:
            printf("semi\n");
            break;
        case COMMA:
            printf("comma\n");
            break;
        case CMP:
            printf("relation: %s\n", Cmp[symbol.value->number_or_index]);
            break;
        case LPAREN:
            printf("l paren\n");
            break;
        case RPAREN:
            printf("r paren\n");
            break;
        case ASSIGN:
            printf("assign\n");
            break;
        case INVALID:
            printf("Invalid symbol\n");
            break;
        default:
            printf("default symbol, error maybe\n");
    }
}

void init() {
    currentChar = 0;
    currentID = NULL;
    getNextChar();
    currentSymbol = (Symbol) {.type=INVALID, .value=0};
}

void getNextChar() {
    currentChar = (char) fgetc(file);
}

void retract() {
    fseek(file, -1, SEEK_CUR);
}

void getNextID() {
    // printf("Getting id");
    char *tmp = malloc(sizeof(char) * MAX_TOKEN_LENGTH);
    int i = 0;
    for (; isIDCharacter(); i++) {
        tmp[i] = currentChar;
        getNextChar();
    }
    tmp[i] = '\0';
    currentID = malloc(sizeof(char) * (i + 1));
	my_strcpy(currentID, tmp);
    free(tmp);
    // printf("%s\n", currentID);
}

void clearSpaceAndReadChar() {
    while (isSpace()) {
        getNextChar();
    }
}

void* error(char* info) {
    printf("error occured, info:%s\n", info);
    printf("current symbol:\n");
    printCurrentSymbol(currentSymbol);

    exit(-1);
}

bool isIDCharacter() {
    if ((currentChar >= '0' && currentChar <= '9')
        ||(currentChar >= 'A' && currentChar <= 'Z')
        ||(currentChar >= 'a' && currentChar <= 'z'))
        return true;
    return false;
}

int getReserve() {
    int i = 0;
    for (i = 0; i < ReservedSize; i++) {
        if (strcmp(currentID, Reserved[i]) == 0) {
            return i;
        }
    }
    return -1;
}

char getLetter() {
    if (currentChar >= 'A' && currentChar <= 'Z') return (char) (currentChar - 'A');
    if (currentChar >= 'a' && currentChar <= 'z') return (char) (currentChar - 'a');
    return -1;
}

int getDigit() {
    if (currentChar >= '0' && currentChar <= '9') return currentChar - '0';
    return -1;
}

int getPlusOrMinus() {
    if (currentChar == '+') return 0;
    if (currentChar == '-') return 1;
    return -1;
}

int getMulOrDiv() {
    if (currentChar == '*') return 0;
    if (currentChar == '/') return 1;
    return -1;
}

bool isSemi() {
    return currentChar == ';';
}

bool isCmp(){
    return currentChar == '=' || currentChar == '<' || currentChar =='>';
}

bool isComma() {
    return currentChar == ',';
}

bool isLParen() {
    return currentChar == '(';
}

bool isRParen() {
    return currentChar == ')';
}

bool isSpace() {
    return (currentChar == ' ' || currentChar == '\t' || currentChar == '\n');
}

bool isEnd() {
    return currentChar == '.' || currentChar == EOF;
}

bool isDelimiter() {
    return !isIDCharacter();
}

int getCmp() {
    char *cmp = malloc(sizeof(char) * 3);
    int i = 0;
    while((currentChar == '=' || currentChar == '<' || currentChar == '>') && i<2){
        cmp[i++] = currentChar;
        getNextChar();
    }
    cmp[i] = '\0';
    for (int j = 0; j < CmpSize; ++j) {
        if (strcmp(Cmp[j], cmp) == 0) {
            return j;
        }
    }
    return -1;
}

bool isAssign() {
    if (currentChar == ':') {
        getNextChar();
        if (currentChar == '=') {
            return true;
        } else {
            error("is assign");
            //retract();
            return false;
        }
    }
    return false;
}