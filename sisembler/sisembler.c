#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inst.h"
#include "parser.h"
#define MEM_SIZE 256

#define MAX_LEN 256
int main(int argc, const char *argv[]){
	if(argc < 2){
		fprintf(stderr, "format: %s <in_file> [out_file]\n", argv[0]);	
		return 1;
	}

	FILE *file = fopen(argv[1], "r");
	FILE *out = stdout;
	unsigned bytesWritten = 0;

	if(argv[2])
		out = fopen(argv[2], "w");

	char *current = malloc(MAX_LEN + 1);	
	if(!current){
		fclose(file);
		return 1;
	}

	while(fgets(current, MAX_INST, file)){
		// remove trailing new-line
		current[strlen(current) - 1] = '\0';
		// ignore empty lines
		if(!current[0])
			continue;

		char *ptr = current;
		unsigned code_len = 0;		
		char *opCode = getOpCode(current, &code_len);
		ptr += code_len;
		removeSpaces(ptr);
		char **args = getArgs(ptr);
		INST inst;
		Status st = decode(opCode, args[0], args[1], args[2], &inst);

		for(int i = 0; i < 4;i++){
			putc(inst >> (i * 8), out);
		}

		bytesWritten += 4;
		freeArgs(args);	
		free(opCode);
	}
	for(;bytesWritten < MEM_SIZE; bytesWritten++)
		putc(0, out);

	free(current);
	fclose(file);
	fclose(out);
	return 0;
}

