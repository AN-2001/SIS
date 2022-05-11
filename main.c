#include <stdio.h>
#include "SIS.h"

// start with a RAM image for now
// I'll implement program loading later (never)

int main(int argc, const char *argv[]){
	const char *path = "prog.sis";

	if(argc > 1){
		path = argv[1];
	}

	SIS cpu = SIScreate(path);

	if(!cpu)
		return 1;
	char in;
	do{
		in = fgetc(stdin);
		switch(in){
			case 'C':
			case 'c':
				SISclock(cpu);
				break;
			case 'M':
			case 'm':
				SISdumpMemory(cpu);
				break;
			case 'R':
			case 'r':
				SISdumpCPU(cpu);
				break;
		}
	}while(in != 's');

	SISfree(cpu);
	return 0;
}
