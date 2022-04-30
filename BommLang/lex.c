#include "lex.h"

TOKEN* lex(CODE* code) {
	int err = 0;

	int nTokens = 0;
	Token u;

	TOKEN* result = (TOKEN*) malloc(sizeof(TOKEN));

	for (size_t i = 0; i < code->length; i++) {
		while ((code->content[i] == ' ' || code->content[i] == '\t' || code->content[i] == '\n' || code->content[i] == '\r') && i < code->length)
			i++;

		if (i >= code->length)
			return;

		TOKEN t;
		t.value = code->content + i;
		t.valLength = 1;

		// TODO offload to function [TOKEN getFirstToken(char* str)]
		switch (code->content[i])
		{
		// single-char
		case '+':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_ADDEQU;
				t.valLength = 2;
				break;
			}
			if (code->content[i + 1] == '+') {
				i++;
				t.type = TT_INC;
				t.valLength = 2;
				break;
			}
			t.type = TT_ADD;
			break;
		case '-':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_SUBEQU;
				t.valLength = 2;
				break;
			}
			if (code->content[i + 1] == '-') {
				i++;
				t.type = TT_DEC;
				t.valLength = 2;
				break;
			}
			t.type = TT_SUB;
			break;
		case '*':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_MULEQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_MUL;
			break;
		case '/':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_DIVEQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_DIV;
			break;
		case '=':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_BOOLEQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_EQU;
			break;

		case '|':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_OREQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_OR;
			break;
		case '&':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_ANDEQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_AND;
			break;
		case '^':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_XOREQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_XOR;
			break;
		case '>':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_GRTEQU;
				t.valLength = 2;
				break;
			}
			if (code->content[i + 1] == '<') {
				i++;
				t.type = TT_NOTEQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_GRT;
			break;
		case '<':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_LSSEQU;
				t.valLength = 2;
				break;
			}
			if (code->content[i + 1] == '>') {
				i++;
				t.type = TT_NOTEQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_LSS;
			break;
		case '!':
			if (code->content[i + 1] == '=') {
				i++;
				t.type = TT_NOTEQU;
				t.valLength = 2;
				break;
			}
			t.type = TT_NOT;
			break;

		case '(':
			t.type = TT_PARENTH_OP;
			break;
		case ')':
			t.type = TT_PARENTH_CL;
			break;
		case '[':
			t.type = TT_BRACKET_OP;
			break;
		case ']':
			t.type = TT_BRACKET_CL;
			break;
		case '{':
			t.type = TT_BRACE_OP;
			break;
		case '}':
			t.type = TT_BRACE_CL;
			break;
		case ';':
			t.type = TT_EOL;
			break;
		case '@':// code point
			if ((code->content[i - 1] != '\n') && (code->content[i - 1] != '\r')) {
				//	TODO: Error message
				// the code point is not at the start of the line
				return 1;
			}

			int begin = i;

			while ((code->content[++i] != '\r') && (code->content[i] != '\n') && (code->content[i] != EOF)) {
				printf("%d ", code->content[i]);
			}
			t.type = TT_CODE_POINT;
			t.valLength = i - begin;

			break;
		default:
			// if keyword
			if (isKeyword("if", 2, code->content + i)) {
				i += 1;
				t.type = TT_IF;
				t.valLength = 2;
				break;
			}

			// jump keyword
			if (isKeyword("jump", 4, code->content + i)) {
				i += 3;
				t.type = TT_JUMP;
				t.valLength = 4;
				break;
			}

			// return keyword
			if (isKeyword("return", 6, code->content + i)) {
				i += 5;
				t.type = TT_RET;
				t.valLength = 6;
				break;
			}

			// call keyword
			if (isKeyword("call", 4, code->content + i)) {
				i += 3;
				t.type = TT_CALL;
				t.valLength = 4;
				break;
			}

			// param keyword
			if (isKeyword("param", 5, code->content + i)) {
				i += 4;
				t.type = TT_PARAM;
				t.valLength = 5;
				break;
			}

			// numbers (TODO: other systems than decimal)
			if (((code->content[i] >= '0') && code->content[i] <= '9')) {

				t.type = TT_NUM;
				t.valLength = 1;

				while ((code->content[++i] >= '0') && code->content[i] <= '9') {
					t.valLength++;
				}
				i--;

				break;
			}

			//words (cant begin with 1-9, numbers are fine after this)
			if (((code->content[i] >= 'a') && code->content[i] <= 'z')
				|| ((code->content[i] >= 'A') && code->content[i] <= 'Z')
				|| (code->content[i] == '_')
				) {

				t.type = TT_WORD;
				t.valLength = 0;

				while (((code->content[i] >= 'a') && code->content[i] <= 'z')
					|| ((code->content[i] >= 'A') && code->content[i] <= 'Z')
					|| ((code->content[i] >= '0') && code->content[i] <= '9')
					|| (code->content[i] == '_') || (code->content[i] == '-')
					) {

					t.valLength++;
					i++;

				}

				i--;

				break;
			}

			// No type
			t.type = 0;
			t.valLength = 0;
			break;
		}
	
		
		TOKEN* realloced = realloc(result, sizeof(TOKEN) * (nTokens + 1));
		if (!realloced) {
			free(result);
			return NULL;
		}
		result = (TOKEN*)realloced;

		result[nTokens++] = t;
		u = result[0];


	}

	Token t;
	t.type = TT_EOF;
	t.valLength = 0;
	t.value = 0;

	TOKEN* realloced = realloc(result, sizeof(TOKEN) * (nTokens + 1));
	if (!realloced) {
		free(result);
		return NULL;
	}

	result = (TOKEN*)realloced;

	result[nTokens++] = t;

	printf("\n%d Tokens evaluated\n", nTokens);
	
	return result;

}

void print_tokens(TOKEN* tokens) {
	for (size_t i = 0; tokens[i].type != TT_EOF; i++)
	{
		if (!tokens[i].type)
			continue;

		if (tokens[i].type == TT_EOL) {
			printf("\n");
			continue;
		}

		char* val = tokens[i].value;

		switch (tokens[i].type)
		{
		case TT_ADD:
			printf("+");
			break;
		case TT_SUB:
			printf("-");
			break;
		case TT_MUL:
			printf("*");
			break;
		case TT_DIV:
			printf("/");
			break;
		case TT_AND:
			printf("&");
			break;
		case TT_OR:
			printf("|");
			break;
		case TT_XOR:
			printf("^");
			break;
		case TT_NOT:
			printf("!");
			break;
		case TT_GRT:
			printf(">");
			break;
		case TT_LSS:
			printf("<");
			break;
		case TT_EQU:
			printf("=");
			break;
		case TT_PARENTH_OP:
			printf("(");
			break;
		case TT_PARENTH_CL:
			printf(")");
			break;
		case TT_BRACKET_OP:
			printf("[");
			break;
		case TT_BRACKET_CL:
			printf("]");
			break;
		case TT_BRACE_OP:
			printf("{");
			break;
		case TT_BRACE_CL:
			printf("}");
			break;
		case TT_IF:
			printf("if");
			break;
		case TT_JUMP:
			printf("jump");
			break;
		case TT_RET:
			printf("return");
			break;
		case TT_CALL:
			printf("call");
			break;
		case TT_PARAM:
			printf("param");
			break;
		case TT_INC:
			printf("++");
			break;
		case TT_DEC:
			printf("--");
			break;
		case TT_ADDEQU:
			printf("+=");
			break;
		case TT_SUBEQU:
			printf("-=");
			break;
		case TT_MULEQU:
			printf("*=");
			break;
		case TT_DIVEQU:
			printf("/=");
			break;
		case TT_ANDEQU:
			printf("&=");
			break;
		case TT_OREQU:
			printf("|=");
			break;
		case TT_XOREQU:
			printf("^=");
			break;
		case TT_NOTEQU:
			printf("!=");
			break;
		case TT_GRTEQU:
			printf(">=");
			break;
		case TT_LSSEQU:
			printf("<=");
			break;
		case TT_BOOLEQU:
			printf("==");
			break;

		case TT_EOL:
			printf("\n");
			break;
		case TT_WORD:
			printf("$");
		default:

			if (tokens[i].type == TT_NUM)
				printf("#");

			if (tokens[i].type != TT_PARENTH_OP && tokens[i].type != TT_PARENTH_CL && tokens[i].type != TT_CODE_POINT)
				printf("(");

			for (size_t j = 0; j < tokens[i].valLength; j++)
			{
				char c = val[j];

				printf("%c", val[j]);
			}
			if (tokens[i].type == TT_CODE_POINT)
				printf("\n");
			else if (tokens[i].type != TT_PARENTH_OP && tokens[i].type != TT_PARENTH_CL)
				printf(")");
			break;
		}

		if(tokens[i].type != TT_CODE_POINT)
			printf(" ");

		
	}
	printf("\nEOF\n");
}