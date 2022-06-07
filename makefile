CC = gcc
CFLAGS = -Wall -O3

shell: main.c env.c env.h shell.c shell.h commands.h
	$(CC) $(CFLAGS) main.c env.c shell.c commands.c -o $@

install: shell
	cp shell /usr/bin/

install-vm:	shell
	rm -rf /usr/bin/shell
	cp shell /usr/bin/
	chmod 755 /usr/bin/shell
	
clean:
	rm -rf *.out
	rm -f shell