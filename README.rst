========
vchkuser
========

:Author: `Benedikt Böhm <bb@xnull.de>`_
:Version: 0.4
:Web: http://github.com/hollow/vchkuser
:Git: ``git clone https://github.com/hollow/vchkuser.git``
:Download: http://github.com/hollow/vchkuser/downloads

The vchkuser qmail-spp plugin checks for recipient existence with vpopmail. It
is based on a script by Werner Maier (http://www.maiers.de/qmail/art38,71.html)
but has been rewritten in C to get rid of all the suid hacks and to enhance
performance.


Installation
============

vchkuser is based on autoconf/automake and can be installed using the usual
procedure. you need to run ``autoreconf -i`` since the release tarball does not
include a configure script:
::

  autoreconf -i
  ./configure
  make && sudo make install

to enable logging to the system log, enable debugging during configure:
::

  ./configure --enable-debug

if your qmail and/or vpopmail installation cannot be found in the default paths
(``/var/qmail`` resp. ``/var/vpopmail``) you have to pass the correct paths to
configure as well:
::

  ./configure --with-qmaildir=/path/to/qmail \
              --with-vpopmaildir=/path/to/vpopmail

since vchkuser is a suid program, you need to tell configure the vpopmail user
(default: vpopmail) and the qmaild group (default: nofiles):
::

  ./configure --with-vpopuser=vmail \
              --with-qmailgroup=qmail


Configuration
=============

In the ``[rcpt]`` section of ``/var/qmail/control/smtpplugins`` add the
following:
::

  plugins/vchkuser
