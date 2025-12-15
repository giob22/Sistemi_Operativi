#CFLAGS dovrebbe essere pari a _DDEBUG se DEBUG è pari ad 1
#posso fare make DEBUG=1 per impostare un valore a DEBUG
ifeq ($(DEBUG), 1)
	CFLAGS = -DDEBUG
else
	CFLAGS =
endif


all: start

start: main.o procedure.o
	gcc -o start main.o procedure.o
main.o: main.c header.h
	gcc $(CFLAGS) -c main.c
procedure.o: procedure.c header.h
	gcc $(CFLAGS) -c procedure.c

clean:
	rm *.o
	rm start
