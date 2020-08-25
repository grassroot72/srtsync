# srtsync makefile

CC = gcc
CFLAGS = -O2 -Wall
OBJS = srtsync.o
EXES = srtsync

all: ${EXES}

${EXES}: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(RM) $(OBJS)

%.o: %.c
	${CC} -o $@ -c $< $(CFLAGS)

clean:
	$(RM) *.o $(EXES)
