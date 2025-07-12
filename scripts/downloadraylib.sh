#!/bin/bash

URL=git@github.com:raysan5/raylib.git

DOWNLOAD_DIR=build/
RAYLIB_PATH=$DOWNLOAD_DIR/raylib
LIB_PATH=libs/desktop/
RAYLIB_A=libraylib.a

if [ ! -d $DOWNLOAD_DIR ]; then
    mkdir $DOWNLOAD_DIR
fi

if [ ! -d $RAYLIB_PATH ]; then
    git clone $URL --depth=1 $RAYLIB_PATH
fi

cd $RAYLIB_PATH

cd src
make
cd ..

cd ../.. # back to the root dir

mv $RAYLIB_PATH/src/$RAYLIB_A $LIB_PATH/$RAYLIB_A
