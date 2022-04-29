#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct
{
	uint16_t type;
	void* value;
} Token, TOKEN;

Token* lex(FILE* file);