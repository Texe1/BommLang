#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* content;
	size_t length;
} Code, CODE;

CODE* readFile(char* path);

int isKeyword(char* key, int keyLength, char* text);