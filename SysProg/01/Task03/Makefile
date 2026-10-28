CC := clang
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
CFLAGS := -g -Wall -Wextra -fsanitize=address,undefined
LDFLAGS := -g -Wall -Wextra -fsanitize=address,undefined

.PHONY: all clean

all: demo

demo: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Header-Aenderungen bauen die jeweils betroffenen Quelldateien neu.
demo.o: mystring.h mystddef.h
mystring.o: mystring.h mystddef.h

clean:
	$(RM) $(OBJECTS) demo
