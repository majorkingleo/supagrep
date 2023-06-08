#/usr/bin/bash

QTDIR=/cygdrive/c/Qt/6.4.3/mingw_64

cmake  -DCMAKE_TOOLCHAIN_FILE=TC-mingw.cmake .
for JSON in `find CMakeFiles -name "*.json"` ; do
	sed -si  "s|/cygdrive.*/moc.exe|`pwd`/mymoc.sh|" ${JSON}
done
sed -s "s|QTDIR|${QTDIR}|" mymoc.sh_ > mymoc.sh && chmod a+x mymoc.sh

make -j4
mkdir dist
cp -u supagrep.exe dist
cp -u ${QTDIR}/bin/*.dll dist
mkdir -p dist/plugins/platforms/ && cp -a ${QTDIR}/plugins/platforms/*.dll dist/plugins/platforms/
