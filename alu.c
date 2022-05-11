#include <stdlib.h>
#include "alu.h"
#define LOGIC_OFFSET 3
#define LOGIC_FLAG 0x20
#define OP_MASK 0x3
typedef unsigned (*operation)(unsigned in1, unsigned in2);
static unsigned add(unsigned in1, unsigned in2);
static unsigned mult(unsigned in1, unsigned in2);
static unsigned divide(unsigned in1, unsigned in2);
static unsigned and(unsigned in1, unsigned in2);
static unsigned or(unsigned in1, unsigned in2);
static unsigned xor(unsigned in1, unsigned in2);
// it's a unary operation but we have to conform with the 
// structure
static unsigned not(unsigned in1, unsigned in2);
static const operation operations[] = {add, divide, mult, and, or, xor, not};

static void performOperation(ALU alu);
struct alu_t{
	unsigned in1;
	unsigned in2;
	unsigned out;
	OP operation;
};

ALU ALUcreate(){
	ALU alu = malloc(sizeof(*alu));
	if(!alu)
		return NULL;
	return alu;
}
void ALUfree(ALU alu){
	free(alu);
}

void ALUclock(ALU alu){
	performOperation(alu);
}

unsigned ALUgetOut(ALU alu){
	if(!alu)
		return 0;
	return alu->out;
}


static void performOperation(ALU alu){
	if(!alu)
		return;
	unsigned opIndx = (alu->operation & OP_MASK) + LOGIC_OFFSET * !!(alu->operation & LOGIC_FLAG);
	alu->out = operations[opIndx](alu->in1, alu->in2);
}

void ALUset(ALU alu, OP op, unsigned in1, unsigned in2){
	if(!alu)
		return;
	alu->in1 = in1;
	alu->in2 = in2;
	alu->operation = op;
}
unsigned add(unsigned in1, unsigned in2){
	return in1 + in2;
}

unsigned mult(unsigned in1, unsigned in2){
	return in1 * in2;
}

unsigned divide(unsigned in1, unsigned in2){
	return in1 / in2;
}


unsigned and(unsigned in1, unsigned in2){
	return in1 & in2;
}

unsigned or(unsigned in1, unsigned in2){
	return in1 | in2;
}

unsigned xor(unsigned in1, unsigned in2){
	return in1 ^ in2;
}

unsigned not(unsigned in1, unsigned in2){
	return ~in1;
}


