CC = clang
CFLAGS = -c -Wall -Wextra -ggdb3
LFLAGS = -Wall -Wextra

all: rumours test
clean:
	rm -f rumours *.o

rumours: rumours.o
	${CC} ${LFLAGS} rumours.o -o rumours

test: thread1.o
	${CC} ${LFLAGS} thread1.o -o test -pthread