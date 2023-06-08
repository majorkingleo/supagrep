#!/usr/bin/env bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

DLLS="libbz2-1.dll  libFOX-1.7-0.dll  libgcc_s_seh-1.dll  libjpeg-8.dll  libpng16-16.dll  libstdc++-6.dll  libwinpthread-1.dll  libz.dll"

cd "$DIR"

if ! test -d dist ; then
	mkdir dist

	for DLL in ${DLLS} ; do
    	( cd dist && cp -u /usr/x86_64-w64-mingw32/sys-root/mingw/bin/${DLL} . )
	done
	( cd dist && ln -s ../supagrep.exe . )
	( cd dist && ln -s ../addpath.exe . )
	( cd dist && ln -s ../forward.exe qc.exe )
	( cd dist && ln -s ../forward.exe qh.exe )
	( cd dist && ln -s ../forward.exe qch.exe )
	( cd dist && ln -s ../forward.exe qrc.exe )
else
# update dll
	( cd dist && for i in ${DLLS} ; do cp -u /usr/x86_64-w64-mingw32/sys-root/mingw/bin/$i . ; done )
fi

strip supagrep.exe
ISCC="/home/martin/.wine/drive_c/Program Files (x86)/Inno Setup 6/ISCC.exe"
wine "${ISCC}" setup.iss
