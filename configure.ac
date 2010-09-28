AC_PREREQ([2.63])
AC_INIT([vchkuser], [0.4], [bb@xnull.de])
AM_INIT_AUTOMAKE([vchkuser], [0.4])

AC_CONFIG_SRCDIR([vchkuser.c])
AC_CONFIG_HEADER([config.h])
AC_CANONICAL_HOST

AC_ARG_ENABLE(debug,
	AS_HELP_STRING([--enable-debug], [enable dugging code]),
	[], [enable_debug=no])
AS_IF([test "x$enable_debug" != xno], [CFLAGS="$CFLAGS -DDEBUG"])

AC_ARG_WITH(qmaildir,
	AS_HELP_STRING([--with-qmaildir=dir],[directory where qmail is installed.]),
	[qmaildir="$withval"],[qmaildir="/var/qmail"])
AS_IF([test ! -d "$qmaildir"],
	AC_MSG_ERROR([Unable to find your qmail directory, specify --with-qmaildir.]))
AC_SUBST([qmaildir])

AC_ARG_WITH(vpopmaildir,
	AS_HELP_STRING([--with-vpopmaildir=dir],[directory where qmail is installed.]),
	[vpopmaildir="$withval"],[vpopmaildir="/var/vpopmail"])
AS_IF([test ! -d "$vpopmaildir"],
	AC_MSG_ERROR([Unable to find your vpopmaildir directory, specify --with-vpopmaildir.]))
AC_SUBST([vpopmaildir])

AC_ARG_WITH(vpopuser,
	AS_HELP_STRING([--with-vpopuser=vpopmail],[vpopmail username]),
	[vpopuser="$withval"],[vpopuser="vpopmail"])
AC_SUBST([vpopuser])

AC_ARG_WITH(qmailgroup,
	AS_HELP_STRING([--with-qmailgroup=nofiles],[qmail group]),
	[qmailgroup="$withval"],[qmailgroup="nofiles"])
AC_SUBST([qmailgroup])

# Checks for programs.
AC_PROG_CC
AC_PATH_PROG([CHMOD], [chmod])

if test "$GCC" = yes; then
	CFLAGS="$CFLAGS -std=c99 -Wall -Wextra -Wno-unused-parameter -pedantic"
fi

# Checks for libraries.
AC_SEARCH_LIBS([pcre_exec], [pcre])

AS_IF([test ! -f $vpopmaildir/etc/lib_deps],
	AC_MSG_ERROR([No $vpopmaildir/etc/lib_deps file. Upgrade to vpopmail-4.9.8 or above]))
VPOPMAIL_LIBS="`cat $vpopmaildir/etc/lib_deps`"
AC_SUBST([VPOPMAIL_LIBS])

AS_IF([test ! -f $vpopmaildir/etc/inc_deps],
	AC_MSG_ERROR([No $vpopmaildiretc/inc_deps file. Upgrade to vpopmail-4.9.8 or above]))
VPOPMAIL_INCLUDES="`cat $vpopmaildir/etc/inc_deps`"
AC_SUBST([VPOPMAIL_INCLUDES])

AC_OUTPUT([Makefile])
