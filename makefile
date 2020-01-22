#!/usr/bin/env make -f

CC       := cc
AR       := ar
LD       := ld
CFLAGS   := -g -O0 -Wpedantic
ARFLAGS  := -rcs
LDFLAGS  := -arch x86_64 -dynamic -pie
LIBS     := $(addprefix -l, System)

DBG      := lldb
DBGFLAGS := -S .dbg-before -K .dbg-after -o run

MODULES  := list gc
PROJECT  := mark-n-sweep-gc
LIBRARY  := $(PROJECT).a
HEADERS  := $(addsuffix .h, $(MODULES))
SOURCES  := $(addsuffix .c, $(MODULES))
OBJECTS  := $(addsuffix .o, $(MODULES))
TARGET   := $(PROJECT).exe
ARGS     := param1 param2

all: $(TARGET)

test: $(TARGET)
	$(DBG) $(DBGFLAGS) -- ./$< $(ARGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(TARGET): $(LIBRARY) main.o
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
