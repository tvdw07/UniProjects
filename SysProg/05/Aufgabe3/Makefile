ifeq ($(OS),Windows_NT)
# NASM elf64 outputs Linux objects, so run the Linux build through WSL.
.PHONY: all run clean
all run clean:
	wsl.exe --cd "$(CURDIR)" -- make OS=Linux $@
else
NASM    ?= nasm
CC      ?= gcc
TARGET  := fibonacci
RUN     := ./$(TARGET)

.PHONY: all run clean
all: run

fibonacci.o: fibonacci.asm
	$(NASM) -f elf64 $< -o $@

$(TARGET): fibonacci.o
	$(CC) -no-pie $< -o $@

run: $(TARGET)
	$(RUN)

clean:
	$(RM) $(TARGET) fibonacci.o
endif

