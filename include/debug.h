#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

void cfprintf(char *color, FILE *stream, char *fmt, ...);
void info_fprintf(FILE *stream, char *fmt, ...);
void warning_fprintf(FILE *stream, char *fmt, ...);
void error_fprintf(FILE *stream, char *fmt, ...);
void success_fprintf(FILE *stream, char *fmt, ...);
void fatal_fprintf(FILE *stream, char *fmt, ...);

#endif /* DEBUG_H */
