#!/usr/bin/env bash


git log > ChangeLog
touch NEWS
if ! test -f README ; then
        cp README.md README
fi

aclocal
automake --add-missing
automake -f
autoconf -f

# just skip this stepp when running from ebuild script
if ! [[ ${1} ]] ; then
	./configure
fi
