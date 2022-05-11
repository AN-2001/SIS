#include <stdio.h>
#include <string.h>
#define INST_COUNT 19
static const char * inst[INST_COUNT] = {"add", "mult", "div",
							            "addi", "multi", "divi",
							            "and", "or", "xor",
							            "andi", "ori", "xori", "not",
								        "jmp", "jle", "jge", "jz", "jnz","nop"};

int main(){
	int hashes[INST_COUNT] = {0};
	int max_hash = 0;
	for(int i = 0; i < INST_COUNT; i++){
		for(int j = 0; inst[i][j] != '\0'; j++){
			hashes[i] ^= inst[i][j];
			if(hashes[i] > max_hash)
				max_hash = hashes[i];
		}
	}

	for(int i = 0; i < INST_COUNT; i++){
		for(int j = i + 1; j < INST_COUNT; j++){
			if(hashes[i] == hashes[j]){
				fprintf(stderr, "FUCK at %s and %s\n", inst[i], inst[j]);
				return 1;
			}
		}
	}

	printf("enum generated:\n\n");
	printf("enum {\n");
	for(int i = 0; i < INST_COUNT ;i++){
		char upper[strlen(inst[i])];
		int j = 0;
		for(j = 0; inst[i][j]; j++){
			upper[j] = inst[i][j] + ('A' - 'a');
		}
		upper[j] = '\0';
		
		printf("\t%s = %u,\n", upper, hashes[i]); 
	}
	printf("};\n");

	printf("map generated:\n\n");
	printf("Decoder decoders[] = {");
	for(int i = 0; i < max_hash; i++){
		int found = 0;
		for(int j = 0; j < INST_COUNT; j++){
			if(hashes[j] == i){
				printf("_%s,\n", inst[j]);
				found = 1;
				break;
			}
		}
		if(!found)
			printf("NULL,");
	}
	printf("};\n");


	return 0;
}
