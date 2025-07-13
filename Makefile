# Support: PLATFORM_DESKTOP and PLATFORM_WEB
APP_NAME ?= ENTER_HERE
PLATFORM ?= PLATFORM_DESKTOP
PLATFORM_OS ?= Linux

EMSDK_PATH         ?= ~/dev/open_source/emsdk
EMSCRIPTEN_PATH    ?= $(EMSDK_PATH)/upstream/emscripten
CLANG_PATH          = $(EMSDK_PATH)/upstream/bin
PYTHON_PATH         = /usr/bin/python3
NODE_PATH           = $(EMSDK_PATH)/node/12.9.1_64bit/bin
PATH = $(shell printenv PATH):$(EMSDK_PATH):$(EMSCRIPTEN_PATH):$(CLANG_PATH):$(NODE_PATH):$(PYTHON_PATH)

BUILD_WEB_ASYNCIFY            ?= TRUE
BUILD_WEB_SHELL               ?= libs/minshell.html
BUILD_WEB_HEAP_SIZE           ?= 128MB
BUILD_WEB_STACK_SIZE          ?= 1MB
BUILD_WEB_ASYNCIFY_STACK_SIZE ?= 1048576
BUILD_WEB_RESOURCES           ?= FALSE
BUILD_WEB_RESOURCES_PATH      ?= resources

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
CFLAGS=-Wall -std=c99 -D_DEFAULT_SOURCE

ifeq ($(PLATFORM),PLATFORM_WEB)
	CC=emcc

    CFLAGS += -s USE_GLFW=3 -s TOTAL_MEMORY=$(BUILD_WEB_HEAP_SIZE) -s STACK_SIZE=$(BUILD_WEB_STACK_SIZE) -s FORCE_FILESYSTEM=1 -s MINIFY_HTML=0

    ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
        CFLAGS += -s ASYNCIFY -s ASYNCIFY_STACK_SIZE=$(BUILD_WEB_ASYNCIFY_STACK_SIZE)
    endif

    ifeq ($(BUILD_WEB_RESOURCES),TRUE)
        CFLAGS += --preload-file $(BUILD_WEB_RESOURCES_PATH)
    endif

    CFLAGS += --shell-file $(BUILD_WEB_SHELL)
    EXT = .html
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	CFLAGS += -DPLATFORM_DESKTOP
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
	CFLAGS += -DPLATFORM_WEB
endif

INC=-I/usr/local/include/ -I/src/ -I./libs/include/

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
	ifeq ($(PLATFORM_OS), OSX)
		LIB=-L./libs/desktop/ -lraylib -lm -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
		RAYLIB_PATH=./libs/desktop/libraylib.a
	else
		# Linux here
		LIB=-L./libs/desktop/ -lraylib -lm
		RAYLIB_PATH=./libs/desktop/libraylib.a
	endif
else
	# WEB here
	LIB=-L./libs/web/ -lraylib -lm
	RAYLIB_PATH=./libs/web/libraylib.a
endif

SRC=./src

FILES= \
	$(wildcard src/*.c)

OBJECTS=$(FILES:.c=.o)

APP=./build/$(APP_NAME)$(EXT)

.PHONY: run server build bear_build clean

run: clean bear_build
	$(APP)

serve: clean bear_build
	open http://localhost:8080/ENTER_HERE.html
	cd build && python3 -m http.server 8080

bear_build:
	bear -- make build

build: $(RAYLIB_PATH) $(OBJECTS)
	$(CC) -o $(APP) $^ $(CFLAGS) $(INC) $(LIB)

$(RAYLIB_PATH):
	export PLATFORM=$(PLATFORM)
	sh ./scripts/downloadraylib.sh

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< -g -MMD

clean:
	-rm $(SRC)/*.o
	-rm $(SRC)/*.d
	-rm build/*

