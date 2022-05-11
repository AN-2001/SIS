#include "inst.h"
#include <stdlib.h>
#include <stdio.h>
#define MT 0
#define NOT_IMM -1
#define OPCODE_OFFSET 0
#define DST_OFFSET 6
#define SRC1_OFFSET 8
#define SRC2_OFFSET 8
#define IMM_OFFSET 10
enum {
	ADD = 97,
	MULT = 0,
	DIV = 123,
	ADDI = 8,
	MULTI = 105,
	DIVI = 18,
	AND = 107,
	OR = 29,
	XOR = 101,
	ANDI = 2,
	ORI = 116,
	XORI = 12,
	NOT = 117,
	JMP = 119,
	JLE = 99,
	JGE = 104,
	JZ = 16,
	JNZ = 126,
	NOP = 113,
};

typedef enum {
	R0 = 0,
	R1, 
	R2, 
	R3,
	RBAD
} Register;


typedef unsigned Hash;

static int isInstr(Hash hash);
static Hash hashStr(const char *str);
static unsigned strToImm(const char *str);
static Register strToReg(const char *str);


typedef INST (*Decoder)(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status);
static INST binaryAType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status);
static INST unaryAType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status);
static INST binaryAIType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status);
static INST nop(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status);
static INST binaryBType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status);
static INST unaryBType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status);


INST opcodes[] =     {/*mult*/ 0x2,
					  /*andi*/ MT, 0x30,
					  /*addi*/ MT,MT,MT,MT,MT, 0x10,
					  /*xori*/ MT,MT,MT, 0x32,
					  /*jz*/   MT,MT,MT, 0,
					  /*divi*/ MT, 0x11,
					  /*or*/   MT,MT,MT,MT,MT,MT,MT,MT,MT,MT, 0x21,
					  /*add*/  MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT, 0x00,
					  /*jle*/  MT, 0,
					  /*xor*/  MT, 0x21,
					  /*jge*/  MT,MT, 0,
					  /*multi*/ 0x12,
					  /*and*/  MT, 0x20,
					  /*nop*/  MT,MT,MT,MT,MT, 0,
					  /*ori*/  MT,MT, 0x31,
					  /*not*/   0x23,
					  /*jmp*/  MT, 0,
					  /*div*/  MT,MT,MT, 0x01,
					  /*MT*/   MT,MT};

Decoder decoders[] = {/*mult*/ binaryAType,
					  /*andi*/ MT, binaryAIType,
					  /*addi*/ MT,MT,MT,MT,MT, binaryAIType,
					  /*xori*/ MT,MT,MT, binaryAIType,
					  /*jz*/   MT,MT,MT, binaryBType,
					  /*divi*/ MT, binaryAIType,
					  /*or*/   MT,MT,MT,MT,MT,MT,MT,MT,MT,MT, binaryAType,
					  /*add*/  MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT,MT, binaryAType,
					  /*jle*/  MT, binaryBType,
					  /*xor*/  MT, binaryAType,
					  /*jge*/  MT,MT, binaryBType,
					  /*multi*/ binaryAIType,
					  /*and*/  MT, binaryAType,
					  /*nop*/  MT,MT,MT,MT,MT, nop,
					  /*ori*/  MT,MT, binaryAIType,
					  /*not*/   unaryAType,
					  /*jmp*/  MT, unaryBType,
					  /*div*/  MT,MT,MT, binaryAType,
					  /*MT*/   MT,MT};


Hash hashStr(const char *str){
	Hash res = 0;
	while(*str){
		res ^= *str;
		str++;
	}
	return res;
}

int isInstr(Hash hash){
	return decoders[hash] != NULL;
}

unsigned strToImm(const char *str){
	if(!str)
		return NOT_IMM;
	unsigned res = 0;
	while(*str){
		if(!(*str >= '0' && *str <= '9'))
			return NOT_IMM;
		res = res * 10 + (*str - '0');
		str++;
	}
	return res;
}

Register strToReg(const char *str){
	if(!str)
		return RBAD;
	if(!((str[0] == 'r' || str[0] == 'R') && str[2] == '\0'))
		return RBAD;
	switch(str[1]){
		case '0':
			return R0;
		case '1':
			return R1;
		case '2':
			return R2;
		case '3':
			return R3;
		default:
			return RBAD;
	}
}

Status decode(const char *opCode, const char *arg1, const char *arg2, const char *arg3, INST *inst){
	if(!inst)
		return BAD_ARGUMENT;

	if(!opCode)
		return NO_OPCODE;

	Hash hash = hashStr(opCode);	
	if(!isInstr(hash))
		return UNKNOWN_OPCODE;
	
	Status st = SUCCESS;
	*inst = decoders[hash](hash, arg1, arg2, arg3, &st);
	if(st != SUCCESS)
		return st;

	return SUCCESS;
}



INST binaryAType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status){
	if(!status)
		return 0;

	INST inst = 0x0;
	inst |= opcodes[hash]  << OPCODE_OFFSET;
	inst |= strToReg(arg1) << DST_OFFSET;
	inst |= strToReg(arg2) << SRC1_OFFSET;
	inst |= strToReg(arg3) << SRC2_OFFSET;
	*status = SUCCESS;
	return inst;
}

INST unaryAType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status){
	if(!status)
		return 0;

	INST inst = 0x0;
	inst |= opcodes[hash]  << OPCODE_OFFSET;
	inst |= strToReg(arg1) << DST_OFFSET;
	*status = SUCCESS;
	return inst;
}

INST binaryAIType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status){
	if(!status)
		return 0;
	INST inst = 0x0;
	inst |= opcodes[hash]  << OPCODE_OFFSET;
	inst |= strToReg(arg1) << DST_OFFSET;
	inst |= strToReg(arg2) << SRC1_OFFSET;
	inst |= strToImm(arg3) << IMM_OFFSET;
	*status = SUCCESS;
	return inst;
}

INST nop(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status){
	if(!status)
		return 0;
	*status = SUCCESS;
	return opcodes[hash];
}

INST binaryBType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status){
	if(!status)
		return 0;
	*status = SUCCESS;
	return opcodes[hash];
}

INST unaryBType(Hash hash, const char *arg1, const char *arg2, const char *arg3, Status *status){
	if(!status)
		return 0;
	*status = SUCCESS;
	return opcodes[hash];
}
