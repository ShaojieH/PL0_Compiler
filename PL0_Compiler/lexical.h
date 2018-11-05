//
// Created by Tom on 2018/10/9.
//

#ifndef PL0_LEXPARSER_H
#define PL0_LEXPARSER_H

#include <stdio.h>
#include <string.h>

#define bool int
#define true 1
#define false 0
#define MAX_TOKEN_LENGTH 50


enum SYMBOL_TYPE {ID = 0, PLUS_OR_MINUS = 1, MUL_OR_DIV = 2,
        NUMBER = 3, SEMI = 4, COMMA = 5, CMP = 6,
        LPAREN = 7, RPAREN = 8, RESERVED = 9,
        ASSIGN = 10, END = 11, INVALID = 12 };

extern const int ReservedSize;
extern const int CmpSize;
extern char* Reserved[12];
extern const char PlusOrMinus[];
extern const char MulOrDiv[];
extern char* Cmp[6];
extern const char Parentheses[];

typedef union {
    int number_or_index;
    char* identifier;
} Value;

typedef struct {
    enum SYMBOL_TYPE type;
    Value* value;
} Symbol;


FILE* file;
char* currentID;
char currentChar;
Symbol currentSymbol;

void init();
void retract();
void getNextID();
void getNextChar();
void printCurrentSymbol(Symbol symbol);
bool isSpace();
bool isEnd();

int getReserve();
void clearSpaceAndReadChar();
int getPlusOrMinus();
int getMulOrDiv();
int getDigit();
char getLetter();
bool isSemi();
bool isComma();
bool isLParen();
bool isRParen();
bool isAssign();
bool isCmp();
int getCmp();
void getNextSymbol();
void* error(char* info);
bool isDelimiter();
bool isIDCharacter();


#endif //PL0_LEXPARSER_H
