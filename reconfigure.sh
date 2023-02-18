#!/usr/bin/env bash


git log > ChangeLog
touch NEWS

aclocal
automake --add-missing
automake -f
autoconf -f

./configure $*
