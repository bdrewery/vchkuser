AC_PREREQ([2.63])
AC_INIT([vchkuser], [0.1], [bb@xnull.de])
AM_INIT_AUTOMAKE([vchkuser], [0.1])

AC_CONFIG_SRCDIR([vchkuser.c])
AC_CONFIG_HEADER([config.h])
AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CC
AC_PATH_PROG([CHMOD], [chmod])

if test "$GCC" = yes; then
	CFLAGS="$CFLAGS -std=c99 -Wall -Wextra -Wno-unused-parameter -pedantic"
fi

qmaildir=""
for f in /var/qmail; do
	if test -d $f; then
		qmaildir=$f
	fi
done

AC_ARG_WITH(qmaildir,
	AS_HELP_STRING([--with-qmaildir=dir],[directory where qmail is installed.]),
	qmaildir="$enableval",
	[
		if test "$qmaildir" = ""; then
			AC_MSG_ERROR([Unable to find your qmail directory, specify --with-qmaildir.])
		fi
	])
AC_SUBST([qmaildir])

vpopmaildir=""
for f in /var/vpopmail; do
	if test -d $f; then
		vpopmaildir=$f
	fi
done

AC_ARG_WITH(vpopmaildir,
	AS_HELP_STRING([--with-vpopmaildir=dir],[directory where qmail is installed.]),
	vpopmaildir="$enableval",
	[
		if test "$vpopmaildir" = ""; then
			AC_MSG_ERROR([Unable to find your vpopmaildir directory, specify --with-vpopmaildir.])
		fi
	])
AC_SUBST([vpopmaildir])

vpopuser="vpopmail"
AC_ARG_WITH(vpopuser,
	AS_HELP_STRING([--with-vpopuser=vpopmail],[vpopmail username]),
	vpopuser="$enableval",
	[
		if test "$vpopuser" = ""; then
			AC_MSG_ERROR([Unable to find your vpopuser user, specify --with-vpopuser.])
		fi
	])
AC_SUBST([vpopuser])

qmailgroup="nofiles"
AC_ARG_WITH(qmailgroup,
	AS_HELP_STRING([--with-qmailgroup=nofiles],[qmail group]),
	qmailgroup="$enableval",
	[
		if test "$qmailgroup" = ""; then
			AC_MSG_ERROR([Unable to find your qmail group, specify --with-qmailgroup.])
		fi
	])
AC_SUBST([qmailgroup])

# Checks for libraries.
AC_SEARCH_LIBS([pcre_exec], [pcre])

if test ! -f $vpopmaildir/etc/lib_deps; then
	AC_MSG_ERROR([No $vpopmaildir/etc/lib_deps file. Upgrade to vpopmail-4.9.8 or above])
fi
VPOPMAIL_LIBS="`cat $vpopmaildir/etc/lib_deps`"
AC_SUBST([VPOPMAIL_LIBS])

if test ! -f $vpopmaildir/etc/inc_deps; then
	AC_MSG_ERROR([No $vpopmaildiretc/inc_deps file. Upgrade to vpopmail-4.9.8 or above])
fi
VPOPMAIL_INCLUDES="`cat $vpopmaildir/etc/inc_deps`"
AC_SUBST([VPOPMAIL_INCLUDES])

AC_OUTPUT([Makefile])
