#include <unistd.h>
#include <stdlib.h>

#ifndef QMAILDIR
#define QMAILDIR "/var/qmail"
#endif

int main(int argc, char *argv[])
{
	const char *vchkuser = QMAILDIR "/plugins/vchkuser.sh";
	setresuid(0, 0, 0);
	setresgid(0, 0, 0);
	execl(vchkuser, vchkuser, NULL);
	exit(EXIT_FAILURE);
}
