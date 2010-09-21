AUTOMAKE_OPTIONS = foreign

sppexecdir = $(qmaildir)/plugins

sppexec_PROGRAMS = vchkuser
vchkuser_SOURCES = debug.c debug.h util.c util.h vchkuser.c
vchkuser_CPPFLAGS = $(AM_CPPFLAGS) @VPOPMAIL_INCLUDES@
vchkuser_LDADD = $(LIBS) @VPOPMAIL_LIBS@

install-exec-hook:
	chown $(vpopuser):$(qmailgroup) $(DESTDIR)$(sppexecdir)/vchkuser
	chmod 4750 $(DESTDIR)$(sppexecdir)/vchkuser
