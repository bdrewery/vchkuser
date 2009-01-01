#include <unistd.h>
#include <stdlib.h>

#ifndef VPOPMAILDIR
#define VPOPMAILDIR "/var/vpopmail"
#endif

#ifndef VPOPMAILUID
#define VPOPMAILUID 89
#endif

#ifndef VPOPMAILGID
#define VPOPMAILGID 89
#endif

int main(int argc, char *argv[])
{
	const char *vchkuser = VPOPMAILDIR "/plugins/vchkuser.sh";
	setuid(VPOPMAILUID);
	execl(vchkuser, vchkuser, NULL);
	exit(EXIT_FAILURE);
}
