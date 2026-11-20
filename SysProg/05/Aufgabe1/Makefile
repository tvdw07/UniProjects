ifeq ($(OS),Windows_NT)
# Use the Linux compiler environment from WSL when make is launched in Windows.
.PHONY: all run clean
all run clean:
	wsl.exe --cd "$(CURDIR)" -- make OS=Linux $@
else
CC      = gcc
TARGET  := speicher-demo
RUN     := ./$(TARGET)

CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -O2
SOURCES := test.c mystdlib.c

.PHONY: all run clean
all: run

$(TARGET): $(SOURCES) mystdlib.h mystd-def.h
	$(CC) $(CFLAGS) $(SOURCES) -o $@

run: $(TARGET)
	$(RUN)

clean:
	$(RM) $(TARGET) *.o
endif
