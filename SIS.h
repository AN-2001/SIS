#ifndef SIS_H
#define SIS_H
typedef unsigned INST;
typedef char BYTE;
typedef unsigned WORD;
typedef short HALF_WORD;

// the cpu only has 3 general purpose 32-bit registers
// numbered 1-3
typedef struct sis_t *SIS;
SIS SIScreate(const char *path);
void SISdumpMemory(SIS cpu);
void SISdumpCPU(SIS cpu);
void SISclock(SIS cpu);

void SISfree(SIS cpu);

#endif // SIS_H
