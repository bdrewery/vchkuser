VPOPMAILDIR ?= /var/vpopmail
QMAILDIR ?= /var/qmail

CC = gcc
CFLAGS = $(OPTCFLAGS) -Wall -std=c99 -pedantic -I${VPOPMAILDIR}/include

OBJS = vchkuser.o util.o debug.o
LIBS = $(VPOPMAILDIR)/lib/libvpopmail.a -lcrypt -lpcre

vchkuser: $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

install: vchkuser
	install -o vpopmail -g nofiles -m 4750 -T $^ $(QMAILDIR)/plugins/$^

clean:
	rm -f vchkuser ${OBJS}
