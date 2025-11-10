#include "procedure-h"


int consuma(Buffer* pc){
	// entriamo dentro il monitor per iniziare la consumazione in mutua esclusione 
	enter_monitor(&(pc->m));
}
void produci(Buffer* pc, int valore_prodotto){

}	
