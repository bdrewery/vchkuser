#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdarg.h>

void vdebug(const char *format, va_list ap);
void debug(const char *format, ...);

#endif
