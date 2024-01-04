CC = gcc
CFLAGS = -Wall -O2 -static
LIBS = -lncurses -ltinfo -lsodium

box: box.c box.h
	$(CC) $(CFLAGS) -o box box.c $(LIBS)

install:
	rm -f /usr/local/bin/box
	cp ./box /usr/local/bin/box

clean:
	rm -f ./box

uninstall:
	rm -f /usr/local/bin/box
