#include "parser.h"
#include <stdlib.h>
#define MAX_OPCODE_LEN 32
char *getOpCode(const char *inst, unsigned *codeLen){
	if(!inst || !codeLen)
		return NULL;
	char *code = malloc(MAX_OPCODE_LEN + 1);
	if(!code)
		return NULL;
	*codeLen = 0;
	for(;inst[*codeLen] != ' ' && inst[*codeLen] != '\0'; (*codeLen)++){
			code[*codeLen] = inst[*codeLen];	
	}
	code[*codeLen] = '\0';
	return code;
}

void removeSpaces(char *str){
	char * iter = str;			
	while(*iter && *str){
		while(*iter == ' ')
			iter++;
		*str = *iter;
		str++;
		iter++;
	}
	*str = '\0';
}

char **getArgs(const char *fullArgs){
	char **args = malloc(sizeof(*args) * 3);	
	if(!args)
		return NULL;

	args[0] = args[1] = args[2] = NULL;

	if(fullArgs[0] != '\0')
		args[0] = malloc(MAX_OPCODE_LEN + 1);

	int currentArg = 0;
	int j = 0;
	for(int i = 0; fullArgs[i] != '\0'; i++){
		if(fullArgs[i] == ','){
			args[currentArg][j] = '\0';
			currentArg++;
			j = 0;
			args[currentArg] = malloc(MAX_OPCODE_LEN + 1);
			continue;
		}
		args[currentArg][j] = fullArgs[i];
		j++;
	}
	return args;
}

void freeArgs(char **args){
	for(int i = 0; i < 3; i++)
		free(args[i]);
	free(args);
}
