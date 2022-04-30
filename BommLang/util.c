#include "util.h"

CODE* readFile(char* path) {
	FILE** file_ptr = malloc(sizeof(FILE**));

	if (!file_ptr)
		return NULL;

	fopen_s(file_ptr, path, "r");

	FILE* file = *file_ptr;

	if (!file) {
		return NULL;
	}

	fseek(file, 0L, SEEK_END);
	long fileSize = ftell(file);
	rewind(file);

	char* code = (char*)malloc(((size_t)fileSize) + 1);

	if (!code)
		return NULL;


	for (size_t i = 0; i < fileSize; i++)
	{
		char c = fgetc(file);
		code[i] = c;
	}

	fclose(file);

	code[fileSize] = 0;

	CODE* code_obj = (CODE*)malloc(sizeof(CODE));
	
	if (!code_obj)
		return NULL;

	code_obj->content = code;
	code_obj->length = fileSize;

	return code_obj;
}



int isKeyword(char* key, int keyLength, char* text) {
	int isUpper = 0;

	if (text[0] != key[0]) {
		if (text[0] == (key[0] & ~(1 << 5))) {
			isUpper = 1;
		}
	}

	for (size_t i = 0; i < keyLength; i++)
	{
		if (text[i] != (key[i] & (isUpper ? ~(1 << 5) : ~0))) {
			return 0;
		}
	}
	return 1;
}
