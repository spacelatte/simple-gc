#!/usr/bin/env make -f

CC      := cc
AR      := ar
LD      := ld
CFLAGS  := -g
ARFLAGS := -rcs
LDFLAGS := -arch x86_64 -dynamic -pie
LIBS    := $(addprefix -l, System c m)

PROJECT := mark-n-sweep-gc
LIBRARY := $(PROJECT).a
OBJECTS := $(addsuffix .o, list alloc release collect)
TARGET  := $(PROJECT).exe

all: test

test: $(TARGET)
	./$< $(FLAGS)

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
