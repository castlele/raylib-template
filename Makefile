# Support: PLATFORM_DESKTOP and PLATFORM_WEB
APP_NAME=ENTER_HERE

PLATFORM ?= PLATFORM_DESKTOP

PLATFORM_OS ?= Linux

UNAMEOS = $(shell uname)

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(UNAMEOS),Linux)
		PLATFORM_OS = LINUX
	endif

	ifeq ($(UNAMEOS),FreeBSD)
		PLATFORM_OS = BSD
	endif
	ifeq ($(UNAMEOS),OpenBSD)
		PLATFORM_OS = BSD
	endif
	ifeq ($(UNAMEOS),NetBSD)
		PLATFORM_OS = BSD
	endif
	ifeq ($(UNAMEOS),DragonFly)
		PLATFORM_OS = BSD
	endif
	ifeq ($(UNAMEOS),Darwin)
		PLATFORM_OS = OSX
	endif
	ifndef PLATFORM_SHELL
		PLATFORM_SHELL = sh
	endif
endif

CC=gcc
CFLAGS=-Wall -std=c99 -Werror -D_DEFAULT_SOURCE

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	CFLAGS += -DPLATFORM_DESKTOP
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	CFLAGS += -DPLATFORM_WEB
endif

INC=-I/usr/local/include/ -I/src/ -I./libs/

ifeq ($(PLATFORM_OS), OSX)
	LIB=-L/usr/local/lib/ -lraylib -lm  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
	LIB=-L/usr/local/lib/ -lraylib -lm
endif

SRC=./src

FILES= \
	$(wildcard src/*.c)

OBJECTS=$(FILES:.c=.o)

APP=./build/$(APP_NAME)

.PHONY: run build clean

run: clean bear_build
	$(APP)

bear_build:
	bear -- make build

build: $(OBJECTS)
	$(CC) -o $(APP) $^ $(CFLAGS) $(INC) $(LIB)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< -g -MMD

clean:
	-rm $(SRC)/*.o
	-rm $(SRC)/*.d
	-rm $(APP)

