#ifndef __UTIL_H
#define __UTIL_H

char *str_dup(const char *str);
char *vxprintf(const char *format, va_list ap);
char *xprintf(const char *format, ...);
char *readline(int fd);
char *match(const char *str, const char *pattern, int nmatch);
bool grep(const char *path, const char *pattern);

#endif
