all: main seller_buyer report

main: main.o
	gcc -o main_exe main.o

main.o: main.c seller_buyer.h
	gcc -c main.c

seller_buyer: seller_buyer.o seller_buyer_procedure.o seller_buyer.h
	gcc -o seller_buyer seller_buyer.o seller_buyer_procedure.o -lpthread

seller_buyer_procedure.o: seller_buyer_procedure.c seller_buyer.h
	gcc -c seller_buyer_procedure.c

report: report.o seller_buyer.h
	gcc -o report report.o

report.o: report.c seller_buyer.h
	gcc -c report.c


clean:
	rm -f *.o 
	rm -f main_exe
	rm -f seller_buyer
	rm -f report


