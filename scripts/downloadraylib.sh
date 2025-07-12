#!/bin/bash

URL=git@github.com:raysan5/raylib.git

DOWNLOAD_DIR=build/
RAYLIB_PATH=$DOWNLOAD_DIR/raylib

LIB_PATH=libs/desktop
TARGET_RAYLIB_A=libraylib.a
ORIGIN_RAYLIB_A=libraylib.a

if [ $PLATFORM = "PLATFORM_WEB" ]; then
    LIB_PATH=libs/web/
    ORIGIN_RAYLIB_A=libraylib.web.a
fi

if [ ! -d $DOWNLOAD_DIR ]; then
    mkdir $DOWNLOAD_DIR
fi

if [ ! -d $RAYLIB_PATH ]; then
    git clone $URL --depth=1 $RAYLIB_PATH
fi

cd $RAYLIB_PATH

cd src
make clean
make PLATFORM=$PLATFORM -B
cd ..

cd ../.. # back to the root dir

mv $RAYLIB_PATH/src/$ORIGIN_RAYLIB_A $LIB_PATH/$TARGET_RAYLIB_A
