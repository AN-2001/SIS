#ifndef PARSER_H
#define PARSER_H

char *getOpCode(const char *inst, unsigned *codeLen);
char **getArgs(const char *fullArgs);
void removeSpaces(char *str);
void freeArgs(char **args);

#endif // PARSER_H
