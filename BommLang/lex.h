#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

// Token Types

#define TT_NULL			0x00

//single-char

// math
#define TT_ADD			TT_NULL			+1
#define TT_SUB			TT_ADD			+1
#define TT_MUL			TT_SUB			+1
#define TT_DIV			TT_MUL			+1

#define TT_AND			TT_DIV			+1
#define TT_OR			TT_AND			+1
#define TT_XOR			TT_OR			+1
#define TT_NOT			TT_XOR			+1

// comparisons
#define TT_GRT			TT_NOT			+1
#define TT_LSS			TT_GRT			+1

#define TT_EQU			TT_LSS			+1

// parentheses
#define TT_PARENTH_OP	TT_EQU			+1
#define TT_PARENTH_CL	TT_PARENTH_OP	+1
#define TT_BRACKET_OP	TT_PARENTH_CL	+1
#define TT_BRACKET_CL	TT_BRACKET_OP	+1
#define TT_BRACE_OP		TT_BRACKET_CL	+1
#define TT_BRACE_CL		TT_BRACE_OP		+1

// Keywords
#define TT_IF			TT_BRACE_CL		+1
#define TT_JUMP			TT_IF			+1
#define TT_RET			TT_JUMP			+1
#define TT_CALL			TT_RET			+1
#define TT_PARAM		TT_CALL			+1

#define TT_WORD			TT_PARAM		+1
#define TT_CODE_POINT	TT_WORD			+1
#define TT_NUM			TT_CODE_POINT	+1

// double-Chars
#define TT_INC			TT_NUM			+1
#define TT_DEC			TT_INC			+1

#define TT_ADDEQU		TT_DEC			+1
#define TT_SUBEQU		TT_ADDEQU		+1
#define TT_MULEQU		TT_SUBEQU		+1
#define TT_DIVEQU		TT_MULEQU		+1

#define TT_ANDEQU		TT_DIVEQU		+1
#define TT_OREQU		TT_ANDEQU		+1
#define TT_XOREQU		TT_OREQU		+1
#define TT_NOTEQU		TT_XOREQU		+1

#define TT_GRTEQU		TT_NOTEQU		+1
#define TT_LSSEQU		TT_GRTEQU		+1
#define TT_BOOLEQU		TT_LSSEQU		+1

// specials
#define TT_EOL			TT_BOOLEQU		+1
#define TT_EOF			TT_EOL			+1


typedef struct
{
	uint16_t type;
	size_t valLength;
	void* value;
} Token, TOKEN;

TOKEN* lex(CODE* code);

void print_tokens(TOKEN* tokens);