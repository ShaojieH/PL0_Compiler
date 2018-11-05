#include <stdio.h>
#include "lexical.h"
#include "syntax.h"

int main() {
    // char input_file_name[200];
    // printf("Enter source file location:\n");
    // gets(input_file_name);
    char* input_file_name = "C:\\Users\\hasee\\source\\repos\\PL0_Compiler\\PL0_Compiler\\test.txt";
	errno_t err = fopen_s(&file, input_file_name, "r");
	if(err != 0){
        printf("Open file failed\n");
		printf("Error code: %d\n", err);
		getchar();
        return 0;
    }
    init();
    /*while(!isEnd()){
        getNextSymbol();
    }*/
    getNextSymbol();
    Sub_program* sub_program = getSubProgram();
	printf("\n\nDone.\n");
	getchar();
    return 0;
}