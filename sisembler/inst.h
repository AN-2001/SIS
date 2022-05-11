#ifndef SIS_INST
#define SIS_INST

#define MAX_INST 126
typedef unsigned INST;
typedef enum{
	SUCCESS,
	BAD_ARGUMENT,
	UNKNOWN_OPCODE,
	NO_OPCODE,
	BAD_INST,
	NOT_AN_IMMEDIATE,
	UNKNOWN_ERROR
}Status;

Status decode(const char *opCode, const char *arg1, const char *arg2, const char *arg3, INST *inst);
#endif // SIS_INST
