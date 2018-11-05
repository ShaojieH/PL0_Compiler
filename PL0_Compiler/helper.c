#include "helper.h"

void my_strcpy(char* dst, char* src) {
	strcpy_s(dst, strlen(src) + 1, src);
}