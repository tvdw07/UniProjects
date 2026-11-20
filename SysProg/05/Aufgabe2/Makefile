ifeq ($(OS),Windows_NT)
# NASM elf64 outputs Linux objects, so run the Linux build through WSL.
.PHONY: all run clean
all run clean:
	wsl.exe --cd "$(CURDIR)" -- make OS=Linux $@
else
NASM    ?= nasm
CC      ?= gcc
TARGET  := uppercase
RUN     := ./$(TARGET)

.PHONY: all run clean
all: run

uppercase.o: uppercase.asm
	$(NASM) -f elf64 $< -o $@

$(TARGET): uppercase.o
	$(CC) -no-pie $< -o $@

run: $(TARGET)
	$(RUN)

clean:
	$(RM) $(TARGET) uppercase.o
endif

