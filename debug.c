#ifdef DEBUG
#include <stdarg.h>
#include <syslog.h>
#include "util.h"
#endif

#include "debug.h"

void vdebug(const char *format, va_list ap)
{
#ifdef DEBUG
	char *msg = vxprintf(format, ap);

	openlog("vchkuser", LOG_PID, LOG_MAIL);
	syslog(LOG_NOTICE, msg);
	closelog();
#endif
}

void debug(const char *format, ...)
{
#ifdef DEBUG
	va_list ap;
	va_start(ap, format);
	vdebug(format, ap);
	va_end(ap);
#endif
}
