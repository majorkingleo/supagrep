#/usr/bin/bash

QTDIR=/cygdrive/c/Qt/6.4.3/mingw_64

cmake  -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=TC-mingw.cmake .
#cmake  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=TC-mingw.cmake .
for JSON in `find CMakeFiles -name "*.json"` ; do
	sed -si  "s|/cygdrive.*/moc.exe|`pwd`/mymoc.sh|" ${JSON}
done
sed -s "s|QTDIR|${QTDIR}|" mymoc.sh_ > mymoc.sh && chmod a+x mymoc.sh

make -j8
mkdir dist
cp -u supagrep.exe dist

DLLS="Qt6Widgets.dll Qt6Gui.dll Qt6Core.dll"

for DLL in ${DLLS} ; do 
	cp -u "${QTDIR}/bin/${DLL}" dist
done

mkdir -p dist/plugins/platforms/ && cp -a ${QTDIR}/plugins/platforms/*.dll dist/plugins/platforms/
