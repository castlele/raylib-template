# Support: PLATFORM_DESKTOP and PLATFORM_WEB
APP_NAME ?= ENTER_HERE
PLATFORM ?= PLATFORM_DESKTOP
PLATFORM_OS ?= Linux

BUILD_WEB_ASYNCIFY    ?= FALSE
BUILD_WEB_SHELL       ?= minshell.html
BUILD_WEB_HEAP_SIZE   ?= 128MB
BUILD_WEB_STACK_SIZE  ?= 1MB
BUILD_WEB_ASYNCIFY_STACK_SIZE ?= 1048576
BUILD_WEB_RESOURCES   ?= FALSE
BUILD_WEB_RESOURCES_PATH  ?= resources

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

ifeq ($(PLATFORM),PLATFORM_WEB)
	CC=emcc

    LDFLAGS += -sUSE_GLFW=3 -sTOTAL_MEMORY=$(BUILD_WEB_HEAP_SIZE) -sSTACK_SIZE=$(BUILD_WEB_STACK_SIZE) -sFORCE_FILESYSTEM=1 -sMINIFY_HTML=0

    ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
        LDFLAGS += -sASYNCIFY -sASYNCIFY_STACK_SIZE=$(BUILD_WEB_ASYNCIFY_STACK_SIZE)
    endif

    ifeq ($(BUILD_WEB_RESOURCES),TRUE)
        LDFLAGS += --preload-file $(BUILD_WEB_RESOURCES_PATH)
    endif

    LDFLAGS += --shell-file $(BUILD_WEB_SHELL)
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
		LIB=-L/usr/local/lib/ -lraylib -lm -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
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

APP=./build/$(APP_NAME)

.PHONY: run build bear_build clean

run: clean bear_build
	$(APP)

bear_build:
	bear -- make build

build: $(RAYLIB_PATH) $(OBJECTS)
	$(CC) -o $(APP)$(EXT) $^ $(CFLAGS) $(INC) $(LIB)

$(RAYLIB_PATH):
	sh ./scripts/downloadraylib.sh

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $< -g -MMD

clean:
	-rm $(SRC)/*.o
	-rm $(SRC)/*.d
	-rm $(APP)

