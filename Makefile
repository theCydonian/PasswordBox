LIBS = -lncurses -ltinfo
LDFLAGS  = ${LIBS}

box: box.c box.h
	cc -static -o box box.c ${LDFLAGS}

install:
	rm /usr/local/bin/box
	cp ./box /usr/local/bin/box
