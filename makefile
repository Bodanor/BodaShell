CC = gcc
CFLAGS = -Wall -O3

shell: main.c env.c env.h shell.c shell.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.out
	rm -f shell