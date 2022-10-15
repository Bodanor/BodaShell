CC = gcc
CFLAGS = -Wall -W -g

C_SOURCES := $(wildcard *.c)
C_OBJECTS := $(C_SOURCES:.c=.o)
C_HEADERS := $(wildcard *.h)

shell: $(C_SOURCES) $(C_HEADERS) $(C_OBJECTS)
	@echo "Building final exec" $@
	$(CC) $(CFLAGS) -o $@ $(C_OBJECTS)

%.o: %.c
	@echo "Making" $@
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo "Removing Objects files and exec"
	rm -rf shell
	rm -rf *.o
