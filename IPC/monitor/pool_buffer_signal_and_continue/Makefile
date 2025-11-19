all: main_exe

main_exe: monitor_signal_continue.o main.o procedure.o
	gcc -o main_exe monitor_signal_continue.o main.o procedure.o
procedure.o: procedure.c procedure.h
	gcc -c procedure.c
monitor_signal_continue.o: monitor_signal_continue.c monitor_signal_continue.h
	gcc -c  monitor_signal_continue.c
# aggiungi -DDEBUG_ per visualizzare la modalità debug implementata nella libreria
clear: 
	rm *.o
	rm *_exe