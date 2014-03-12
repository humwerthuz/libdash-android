#!/bin/bash


#COMMENTED -> USING STANDALONE TOOLCHAIN @ /OPT/ANDROID-8/

#export PATH=/home/humberto/playground/android/android-ndk-r9c/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86_64/bin:$PATH

export CC=arm-linux-androideabi-gcc
export CPP=arm-linux-androideabi-cpp
export CXX=arm-linux-androideabi-g++
export LD=arm-linux-androideabi-ld

export LDFLAGS="-L../lib"

export LIBS="-lstdc++ -lxml2 -lcurl -liconv -lz"

export CFLAGS="-Iinclude/ -I../libs/curl/include -I../libs/libiconv/include -I../libs/libxml2/include"

export CPPFLAGS="-Iinclude/"

make clean && make && make install
