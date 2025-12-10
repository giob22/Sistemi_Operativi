
CC := gcc
CFLAGS ?=
LDFLAGS ?=
OUTPUT := start

all: $(OUTPUT)

$(OUTPUT): main.o procedure.o monitor_signal_continue.o
	$(CC) $(LDFLAGS) -o $(OUTPUT) main.o procedure.o monitor_signal_continue.o

run: all
	stdbuf -oL -eL ./$(OUTPUT) 2>&1 | ts > output.txt

main.o: main.c procedure.o monitor_signal_continue.o
	$(CC) $(CFLAGS) -c main.c

procedure.o: procedure.c header.h monitor_signal_continue.o
	$(CC) $(CFLAGS) -c procedure.c

monitor_signal_continue.o: monitor_signal_continue.c monitor_signal_continue.h
	$(CC) $(CFLAGS) -c monitor_signal_continue.c

# debug target: compile with DEBUG defined and no optimizations
debug: CFLAGS += -DDEBUG -g
debug: clean $(OUTPUT)

clean:
	rm -f *.o
	rm -f $(OUTPUT)
	rm -f output.txt
