#!/bin/sh

LIB_VERSION="$*"


# TODO: Maybe validate the version number a little more thoroughly

if [ ${#LIB_VERSION} -lt 5 ]; then
    LIB_VERSION="0.0.1"
fi

clang shmbridge.c -O3 -shared -o shmbridge.so -I /usr/include/wine/windows/ -L /usr/lib64/wine-wow64/wine/x86_64-unix/ -l:ntdll.so -lrt
winegcc -shared -lrt -mno-cygwin -O3 -DVERSION_INFO="\"$LIB_VERSION\"" version.c winshim.c shmbridge.spec -o shmbridge.dll
