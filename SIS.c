#include <stdlib.h>
#include <stdio.h>
#include "SIS.h"
#include "alu.h"
#define INSTRUCTION_MASK 0x3f
#define REGISTER_MASK 0x3
#define IMMEDIATE_FLAG 0x10
#define JUMP_FLAG 0x08
#define RAM_SIZE 256
// r0 is always 0, no point in defining it
struct sis_t {
	unsigned pc;
	unsigned registers[3];
	unsigned *currentDst;
	INST currentInst;
	ALU alu;
	BYTE RAM[RAM_SIZE];
	int ramReadWrite;
};
static int decodeInsturction(SIS cpu);

void SISdumpMemory(SIS cpu){
	if(!cpu)
		return;
	printf("START OF RAM DUMP\n");
	for(int i = 0; i < RAM_SIZE / 4;i++){
		
		printf("%04d%s %02x %02x %02x %02x\n", i*4, i*4 == cpu->pc ? "(*)" : "", cpu->RAM[i*4], cpu->RAM[i*4 + 1],
										   cpu->RAM[i*4 + 2], cpu->RAM[i*4 + 3]);
	}
	printf("END OF RAM DUMP\n\n");
}
void SISclock(SIS cpu){
	if(!cpu)
		return;

	// update register from last operation
	if(cpu->currentDst)
		*cpu->currentDst = ALUgetOut(cpu->alu);

	// perform fetch, we'll simualte it with a simple add for now
	// addi r1 r1 3
	cpu->currentInst = *(int*)(cpu->RAM + cpu->pc);
	printf("inst: %x\n", cpu->currentInst);
	// increment program counter
	unsigned isJump = decodeInsturction(cpu);

	if(isJump)
		cpu->pc += isJump;
	else
		cpu->pc += 4;

	ALUclock(cpu->alu);
}

int decodeInsturction(SIS cpu){
	if(!cpu)
		return 0;
	// check for nop!
	if(!cpu->currentInst)
		return 0;

	OP operation = cpu->currentInst & INSTRUCTION_MASK;
	unsigned dst = (cpu->currentInst >> 6) & REGISTER_MASK;

	if(operation & JUMP_FLAG){
		unsigned imm = cpu->currentInst >> 8;	
		printf("IMM %x, DST %x\n", imm, dst);
		if(dst)
			dst = cpu->registers[dst - 1];
		unsigned jumpAmount = dst + imm;
		// normalize so we're on a 4 byte bound
		jumpAmount = (jumpAmount & (~0x3)); // TODO: make more readable?
		return jumpAmount;
	}

	unsigned val1 = (cpu->currentInst >> 8) & REGISTER_MASK;
	unsigned val2 = cpu->currentInst >> 10;
	cpu->currentDst = cpu->registers + dst - 1;	
	if(val1 != 0)
		val1 = cpu->registers[val1 - 1];

	if(!(operation & IMMEDIATE_FLAG)){
		val2 = val2 & REGISTER_MASK;
		val2 = cpu->registers[(val2 ? val2 - 1 : 0)];
	}

	ALUset(cpu->alu, operation, val1, val2);
	return 0;
}

SIS SIScreate(const char *path){
	SIS cpu = malloc(sizeof(*cpu));
	if(!cpu)
		return NULL;
	if(!path)
		return NULL;
	cpu->pc = 0;
	cpu->alu = ALUcreate();
	FILE *program = fopen(path, "r");	
	if(!program)
		return NULL;
	fseek(program, 0, SEEK_SET);
	fread(cpu->RAM, RAM_SIZE, 1, program);
	fclose(program);
	return cpu;
}

void SISdumpCPU(SIS cpu){
	printf("START OF CPU DUMP\n");
	printf("PC = %u\n", cpu->pc);
	printf("r1 = %u\n", cpu->registers[0]);
	printf("r2 = %u\n", cpu->registers[1]);
	printf("r3 = %u\n", cpu->registers[2]);
	printf("END OF CPU DUMP\n\n");
}

void SISfree(SIS cpu){
	ALUfree(cpu->alu);
	free(cpu);
}
