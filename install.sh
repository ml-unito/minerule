#!/bin/sh

INSTALL=install
INSTOPT="-o mluser -g wwwsrv -m 0770"

if test $# -lt 1; then
    echo Wrong number of parameters given!
    echo Usage $0 "install-path"
    exit 1
fi

echo Deducing version number from current path...
version="ienco"-`tla pristines | tail -1 | perl -e '$r=<>; $r =~ /(.*)(\d+\.\d+\.\d+.*)/; print $2;'`

./configure


echo "================================================================================"
echo "=========================== Recompiling Executables ============================"
echo "================================================================================"
pushd Kernel
touch kernel.cpp
make
popd

pushd TmpProjects/PrintRules
touch PrintRules.cpp
make
popd

pushd TmpProjects/MRCatalogue
touch mrcatalogue.cpp
make
popd

pushd TmpProjects/MRDefaultOptions/
touch MRDefaultOptions.cpp
make
popd

pushd TmpProjects/MRShowLog
touch mrshowlog.cpp
make
popd

echo "================================================================================"
echo "============================ Installing Executables ============================"
echo "================================================================================"

echo Detected version: $version
echo Installing kernel...
$INSTALL $INSTOPT Kernel/kernel.exe $1/kernel_$version

echo Installing mrprintrules
$INSTALL $INSTOPT TmpProjects/PrintRules/mrprintrules $1/mrprintrules_$version

echo Installing mrcatalogue
$INSTALL $INSTOPT TmpProjects/MRCatalogue/mrcatalogue $1/mrcatalogue_$version

echo Installing mrdefaults
$INSTALL $INSTOPT TmpProjects/MRDefaultOptions/mrdefaults $1/mrdefaults_$version

echo Installing mrshowlog
$INSTALL $INSTOPT TmpProjects/MRShowLog/mrshowlog $1/mrshowlog_$version
