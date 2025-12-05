#include <stdio.h>
#include <stdlib.h>



// implementazione di una semplice somma tra due numeri inseriti a riga di comando

int main(int argc, char **argv){
	if (argc < 3){
		printf("errore, devi inserire inline due valori interii\n");
		exit(-1);	
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);

	printf("ho ricevuto: %d e %d\n", a , b);
	int result = 0;

	asm volatile(
		"add %2, %1\n\t" // %2 + %1 → %1
		"mov %1, %0\n\t" // %1 → %0
		: "=r" (result),  // output
		  "+r" (a) // sto dicendo che a sarà sia un input che potrà esser modificato
		: "r" (b) // input
);
// result → %0
// a → %1
// b → %2
printf("result: %d\n", result);

	return 0;
}
