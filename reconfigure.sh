#!/usr/bin/env bash
aclocal
automake --add-missing
automake -f
autoconf -f

# just skip this stepp when running from ebuild script
if ! [[ ${1} ]] ; then
	./configure
fi
