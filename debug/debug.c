#include <stdarg.h>

#include "../include/debug.h"

void cfprintf(char *color, FILE *stream, char *fmt, ...) {
	fprintf(stream, "%s", color);

	va_list args;
	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	fprintf(stream, "%s", RESET);
}

void info_fprintf(FILE *stream, char *fmt, ...) {
	fprintf(stream, "%s[INFO] ", KBLU);

	va_list args;
	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	fprintf(stream, "%s", RESET);
}

void warning_fprintf(FILE *stream, char *fmt, ...) {
	fprintf(stream, "%s[WARNING] ", KYEL);

	va_list args;
	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	fprintf(stream, "%s", RESET);
}

void error_fprintf(FILE *stream, char *fmt, ...) {
	fprintf(stream, "%s[ERROR] ", KRED);

	va_list args;
	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	fprintf(stream, "%s", RESET);
}

void success_fprintf(FILE *stream, char *fmt, ...) {
	fprintf(stream, "%s[SUCCESS] ", KGRN);

	va_list args;
	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	fprintf(stream, "%s", RESET);
}

void fatal_fprintf(FILE *stream, char *fmt, ...) {
	fprintf(stream, "%s[FATAL] ", KRED);

	va_list args;
	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	fprintf(stream, "%s", RESET);
}
