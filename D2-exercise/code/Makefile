CC=gcc
CFLAGS=-O2 -Wall 
LFLAGS=-lrt

SRC= OptMatTransp.c #ex01.c #ex02.c
EXE= $(SRC:%.c=%.x)

all: $(EXE)

verbose: CFLAGS += -DVERBOSE
verbose: $(EXE)

clean:
	rm -f *.x *.o *~

.SUFFIX:
.SUFFIX: .x .c

%.x: %.c
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

