# srt_sync makefile

CC = gcc
CFLAGS = -O2 -Wall
OBJS = srt_sync.o
EXES = srt_sync

all: ${EXES}

${EXES}: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(RM) $(OBJS)

%.o: %.c
	${CC} -o $@ -c $< $(CFLAGS)

clean:
	$(RM) *.o $(EXES)
