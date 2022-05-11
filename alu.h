#ifndef ALU_H
#define ALU_H
typedef unsigned char OP;
typedef struct alu_t *ALU;

ALU ALUcreate();

void ALUclock(ALU alu);
void ALUset(ALU alu, OP op, unsigned in1, unsigned in2);
unsigned ALUgetOut(ALU alu);

void ALUfree(ALU alu);

#endif // ALU_H
