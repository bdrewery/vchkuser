#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pcre.h>

#include "util.h"
#include "debug.h"

char *vxprintf(const char *format, va_list ap)
{
	va_list aq;
	va_copy(aq, ap);
	int len = vsnprintf(NULL, 0, format, aq) + 1;
	va_end(aq);

	char *buf = malloc(len);
	memset(buf, 0, len);

	vsnprintf(buf, len, format, ap);
	return buf;
}

char *xprintf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	char *buf = vxprintf(format, ap);
	va_end(ap);
	return buf;
}

#define CHUNKSIZE 128

char *readline(int fd)
{
	int chunks = 1, len = 0;
	char *buf = malloc(chunks * CHUNKSIZE + 1);
	char c;

	while (1) {
		switch(read(fd, &c, 1)) {
		case -1:
			free(buf);
			return NULL;

		case 0:
			if (len == 0)
				return NULL;
			goto out;

		default:
			if (c == '\n' || c == '\r')
				goto out;

			if (len >= chunks * CHUNKSIZE) {
				chunks++;
				buf = realloc(buf, chunks * CHUNKSIZE + 1);
			}

			buf[len++] = c;
			break;
		}
	}

out:
	buf[len] = '\0';
	return buf;
}

char *match(const char *str, const char *pattern, int nmatch)
{
	const char *errstr = NULL;
	int erroff = 0;

	pcre *re_bin = pcre_compile(pattern, PCRE_CASELESS,
			&errstr, &erroff, NULL);
	if (!re_bin) {
		debug("failed to compile pattern: %s (near '%s')",
				errstr, pattern + erroff);
		return NULL;
	}

	errstr = NULL;
	pcre_extra *re_extra = pcre_study(re_bin, 0, &errstr);

	if (errstr) {
		debug("failed to study pattern: %s", errstr);
		return NULL;
	}

	int oveclen = (nmatch + 1) * 3;
	int ovec[oveclen];
	int matched = pcre_exec(re_bin, re_extra, str, strlen(str), 0, 0, ovec, oveclen);

	debug("str: '%s', pattern: '%s', nmatch: %d, matched: %d",
			str, pattern, nmatch, matched);

	if (matched < -1) {
		debug("failed to execute pattern: %d", matched);
		return NULL;
	}

	if (matched == -1) {
		if (nmatch > 0)
			return strdup(str);
		else
			return NULL;
	}

	if (nmatch == 0)
		return strdup(str);

	const char *buf = NULL;
	if (pcre_get_substring(str, ovec, matched, nmatch, &buf) < 0) {
		debug("failed to extract substring");
		return NULL;
	}

	return strdup(buf);
}

bool grep(const char *path, const char *pattern)
{
	int fd = open(path, O_RDONLY);
	bool ret = false;
	char *line = NULL;

	while (!ret && (line = readline(fd)) != NULL) {
		if (match(line, pattern, 0))
			ret = true;
		free(line);
	}

	close(fd);
	return ret;
}
