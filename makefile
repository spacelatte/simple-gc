#!/usr/bin/env make -f

MODULES  := list gc
PROJECT  := simple-gc
LIBRARY  := $(PROJECT).a
HEADERS  := $(addsuffix .h, $(MODULES))
SOURCES  := $(addsuffix .c, $(MODULES))
OBJECTS  := $(addsuffix .o, $(MODULES))
TARGET   := $(PROJECT).exe
ARGS     := param1 param2

CC       := cc
AR       := ar
LD       := ld
CFLAGS   := -g -O0 -Wpedantic
ARFLAGS  := -rcs
LDFLAGS  := -pie -L.
LIBS     := $(addprefix -l, c)

DBG      := lldb
DBGFLAGS := -b -S .dbg-before -K .dbg-after -o run
SYSTEM   := $(shell uname)

ifeq ($(SYSTEM),Linux)
LD       := gcc
LIBS     += $(addprefix -l, :$(LIBRARY))
endif

ifeq ($(SYSTEM),Darwin)
LIBS     += $(addprefix -l, System)
endif

all: $(TARGET)

test: $(TARGET)
	$(DBG) $(DBGFLAGS) -- ./$< $(ARGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(TARGET): main.o $(LIBRARY)
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $^

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f \
		$(OBJECTS) \
		$(LIBRARY) \
		$(TARGET) \
		main.o

headers: $(SOURCES)
	cproto -deqx -f 2 -O /dev/null $^

watch:
	watchexec -v -n -c -i '*.a.*' $(MAKE) clean all test
