all: main_exe

main_exe: monitor_hoare.o main.o procedure.o
	gcc -o main_exe monitor_hoare.o main.o procedure.o
procedure.o: procedure.c procedure.h
	gcc -c procedure.c
monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	gcc -c monitor_hoare.c

clear: 
	rm *.o
	rm *_exe