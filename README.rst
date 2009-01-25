========
vchkuser
========

:Author: `Benedikt Böhm <bb@xnull.de>`_
:Version: 0.2
:Web: http://bb.xnull.de/projects/vchkuser/
:Source: http://git.xnull.de/gitweb/?p=vchkuser.git (also on `github <http://github.com/hollow/vchkuser>`_)
:Download: http://bb.xnull.de/projects/vchkuser/dist/

The vchkuser qmail-spp plugin checks for recipient existence with vpopmail. It
is based on a script by Werner Maier (http://www.maiers.de/qmail/art38,71.html)
but has been rewritten in C to get rid of all the suid hacks and to enhance
performance.


Installation
============

to install vchkuser edit the Makefile, verify ``VPOPMAILDIR`` and ``QMAILDIR``
are set correctly and call make:
::

  make && sudo make install

to enable logging to the system log, set the debug flag during make:
::

  make OPTCFLAGS="-DDEBUG" && sudo make install


Configuration
=============

In the ``[rcpt]`` section of ``/var/qmail/control/smtpplugins`` add the
following:
::

  plugins/vchkuser
