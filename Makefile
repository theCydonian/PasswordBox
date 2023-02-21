LIBS = -lncurses -ltinfo
LDFLAGS  = ${LIBS}

box: box.c box.h
	cc -o box box.c ${LDFLAGS}
