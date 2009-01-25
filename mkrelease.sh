#!/bin/bash

source /etc/init.d/functions.sh

PROJECT=$(sed -n '2{p;q;}' README.rst)
VERSION=$(sed 's/^:Version: \(.*\)/\1/;t;d' README.rst)

HTDOCS=~/public_html/projects/${PROJECT}
DISTTAR=${HTDOCS}/dist/${PROJECT}-${VERSION}.tar.bz2

mkdir -p ${HTDOCS}/dist

ebegin "Generating project page"
rst2html.py < README.rst > ${HTDOCS}/index.html
eend $?

if [[ -e ${DISTTAR} ]]; then
	echo "!!! ${DISTTAR} exists."
else
	ebegin "Creating release tarball"
	git archive --format=tar --prefix=${PROJECT}-${VERSION}/ HEAD | \
	bzip2 > ${DISTTAR}
	eend $?
fi
