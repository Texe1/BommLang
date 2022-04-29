#include "read.h"

char* readFile(char* path) {
	FILE* f = fopen(path, "r");

	int fileSize = 0;

	while (fgetc(f) != EOF)
		fileSize++;

	fclose(f);
}