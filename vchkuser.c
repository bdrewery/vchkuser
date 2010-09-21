#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <vauth.h>
#include <vpopmail.h>

#include "debug.h"
#include "util.h"

/* reject the smtp session with a permanent error */
void nack(const char *format, ...)
{
#ifdef DEBUG
	va_list ap;
	va_start(ap, format);
	vdebug(format, ap);
	va_end(ap);
#endif
	printf("E511 Sorry, no mailbox here by that name (#5.1.1)\n");
	exit(1);
}

/* accept the smtp session without further checks */
void ack(const char *format, ...)
{
#ifdef DEBUG
	va_list ap;
	va_start(ap, format);
	vdebug(format, ap);
	va_end(ap);
#endif
	exit(0);
}

int main(int argc, char *argv[])
{
	// if the recipient is empty it does not exist
	char *SMTPRCPTTO = getenv("SMTPRCPTTO");
	if (!SMTPRCPTTO)
		nack("invalid empty recipient");

	debug("running checks for %s", SMTPRCPTTO);

	// check if the box name contains invalid characters
	debug("check mailbox name for invalid characters");
	if (!match(SMTPRCPTTO, "^[-0-9a-z\\.@_=]+$", 0))
		nack("invalid characters in recipient name: %s", SMTPRCPTTO);

	// get the domain name and extension of the recipient
	char *ext = str_dup(SMTPRCPTTO);
	char *domain = strchr(ext, '@');

	debug("ensure both domain and extension have been supplied");
	if (!domain || strlen(domain) <= 1)
		nack("invalid empty domain name");
	*domain++ = '\0';

	if (strlen(ext) < 1)
		nack("invalid empty extension");

	// check relay users
	if (getenv("RELAYCLIENT") || getenv("SMTPAUTHUSER"))
		ack("relaying email to %s", SMTPRCPTTO);

	// get domain info
	debug("loading domain entry if available");
	domain_entry *de = get_domain_entries(domain);

	// if domain does not exist ...
	if (!de) {
		if (verrori) {
			debug("failed to lookup domain entries: %s", verror(verrori));
		}

		nack("no such domain: %s", domain);
	} else {
		// ... otherwise sanitize alias domains and continue
		domain = de->realdomain;
		SMTPRCPTTO = xprintf("%s@%s", ext, domain);
	}

	// check existance of the user
	debug("checking vauth database");
	struct vqpasswd *ue = vauth_getpw(ext, domain);
	if (ue)
		ack("%s exists (vuserinfo)", SMTPRCPTTO);

	// check for aliases
	debug("checking valias database");
	if (valias_select(ext, domain))
		ack("%s exists (valias)", SMTPRCPTTO);

	// if a .qmail-ext file exists, delivery is possible
	char *dotqmail = xprintf("%s/.qmail-%s", de->path, ext);
	debug("checking '%s' access", dotqmail);
	if (isreadable(dotqmail))
		ack("%s exists (.qmail-%s)", SMTPRCPTTO, ext);
	free(dotqmail);

	// check dotqmail -default patterns
	char *p, *ext2 = str_dup(ext);

	do {
		if ((p = strrchr(ext2, '-')) == NULL)
			break;

		*p = '\0';

		char *dotdefault = xprintf("%s/.qmail-%s-default", de->path, ext2);
		debug("checking '%s' access", dotdefault);
		if (isreadable(dotdefault))
			ack("%s exists (.qmail-%s-default)", SMTPRCPTTO, ext2);
		free(dotdefault);
	} while (true);

	// if a .qmail-default file exists, delivery is possible
	// catchall is very bad, but possible
	char *dotdefault = xprintf("%s/.qmail-default", de->path);
	debug("checking '%s' access", dotdefault);
	if (isreadable(dotdefault)) {
		debug("checking catchall");
		if (!grep(dotdefault, "vdelivermail(.*)(bounce-no-mailbox|delete)"))
			ack("%s exists (.qmail-default)", SMTPRCPTTO);
	}
	free(dotdefault);

	nack("no such recipient: %s", SMTPRCPTTO);
}
