#!/bin/bash

set -ex

for pkg in libarchive libsndfile cppunit portaudio libwinpthread-git;
do
    C:/msys64/usr/bin/pacman --noconfirm -S ${BITS}-${pkg}
done

export CMAKE_PREFIX_PATH=${QTDIR}
export PKG_CONFIG_PATH="C:\msys64\mingw64\lib\pkgconfig"

mv "C:\Program Files\Git\usr\bin\sh.exe" "sh2.exe" 

mkdir build
cd build
cmake -G ${GENERATOR} ..
