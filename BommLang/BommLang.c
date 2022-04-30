#include <stdio.h>
#include "lex.h"
#include "util.h"

int main()
{
	CODE*  code_raw = readFile("main.bl");
	Token* code_tok = lex(code_raw);
	printf("Tokenized Code:\n\n");
	print_tokens(code_tok);

}