#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#include "header.h"

void inizializza_vettore(MonitorProdCons *pc) {
    /*
        TODO: completare
    */
    init_monitor(&(pc->m), 3);

    for(int i = 0; i < DIM; i++)
        pc->stato[i] = LIBERO;
    pc->num_liberi = DIM;
    pc->num_occupati_tipo_1 = 0;
    pc->num_occupati_tipo_2 = 0;

}

void rimuovi_vettore(MonitorProdCons *pc) {
    /*
        TODO: completare
    */
    
    remove_monitor(&(pc->m));
}

void inizializza_buffer(BufferBabbo * bb) {

	/*
        TODO: inizializzazione array di semafori necessari e inizializzazione struttura BufferBabbo
    */

    bb->buffer = 0;
    // ho ottenuto il descrittore del semaforo per gestire la cooperazione tra elfi e Babbo Natale

    bb->semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0664);

    semctl(bb->semid, ELFI, SETVAL, 1); // produttore
    semctl(bb->semid, BABBO, SETVAL, 0); // consumatore

    printf("buffer dei regali inizializzato a %d\n", bb->buffer);

}


void rimuovi_buffer(BufferBabbo * bb) {
    /*
        TODO: completare
    */
    semctl(bb->semid, 0, IPC_RMID);
    
}

void Produttore(MonitorProdCons *pc) { // SINGOLO PROCESSO FABBRICATORE

    for (int i=0; i<TOT_GIOCATTOLI; i++) {
        if (i%2==0) {
            /*
        		TODO: completare
    		*/
            produci(pc, TIPO1);
        } else {
            /*
            TODO: completare
    		*/
            produci(pc, TIPO2);
        }

		sleep(2);
    }

}

void produci(MonitorProdCons *pc, int tipo) {

    /*
        TODO: produzione con monitor e vettore di stato
    */

    enter_monitor(&(pc->m));

    // verifica della condizione di sincronizzazione

    if (pc->num_liberi == 0)
        wait_condition(&(pc->m), CV_FAB);

    // se la condizione di sincronizzazione è verificata, si ricerca il buffer libero
    int i = 0;
    while (i < DIM && pc->stato[i] != LIBERO) i++;

    // appena trovato il primo buffer libero, si modifica lo stato di questo in USO
    pc->stato[i] = IN_USO;

    // decremento del numero di posti liberi
    pc->num_liberi--;

    // uscita dal monitor
    leave_monitor(&(pc->m));

    // inizia la produzione



	sleep(1);

	/*
        TODO: completare
    */

    pc->buffer[i] = tipo;

	printf("Produzione - posizione %d, giocattolo %d\n", i, tipo);

    // rientra all'interno del monitor

    enter_monitor(&(pc->m));

    // modifica delle variabili di stato a supporto della cooperazione 
    
    
    // il fabbricatore deve segnalare i processi assemblatori, ma non uno qualsiasi, quello che opera sulla tipologia del regalo che ha creato
    
    
    if (tipo == 1) {
        /*
        TODO: completare
    	*/


        pc->stato[i] = OCCUPATO_GIOCO_1;
        pc->num_occupati_tipo_1++;
        signal_condition(&(pc->m), CV_ASS_1);


    } else if (tipo == 2) {
        /*
        TODO: completare
    	*/


        pc->stato[i] = OCCUPATO_GIOCO_2;
        pc->num_occupati_tipo_2++;
        signal_condition(&(pc->m), CV_ASS_2);


    }

    // uscita dal monitor
    leave_monitor(&(pc->m));
}

void Consumatore1(MonitorProdCons *pc, BufferBabbo *bb) {

    int gioco;

	/*
        TODO: consumo con monitor e vettore di stato
    */

    // entrata nel monitor
    enter_monitor(&(pc->m));

    // verifico la condizione di sicronizzazione di sincronizzazione
    if (pc->num_occupati_tipo_1 == 0)
        wait_condition(&(pc->m), CV_ASS_1);
    
    // la condizione di sincronizzazione è verificata

    // si cerca il primo buffer occupato da un giocattolo 1
    int i = 0;
    while (i < DIM && pc->stato[i] != OCCUPATO_GIOCO_1) i++;

    // una volta ottenuto il buffer lo si setta come in uso
    pc->stato[i] = IN_USO;
    // decremento del numero di giocattoli di tipo 1
    pc->num_occupati_tipo_1--;

    // uscita dal monitor

    leave_monitor(&(pc->m));

    // inizia l'assemblazione

    gioco = pc->buffer[i];
    
	printf("Consumo - posizione %d, gioco %d\n", i, gioco);
    
	/*
    TODO: consumo con monitor e vettore di stato
    */
   
   // termino la consumazione entrando nel monitor e aggiornando le variabili di stato
   
   enter_monitor(&(pc->m));
   
   pc->num_liberi++;
   pc->stato[i] = LIBERO;
   
   // segnala degli eventuali fabbricatori in attesa di fabbricare
   signal_condition(&(pc->m), CV_FAB);
   
   leave_monitor(&(pc->m));
   
   /*
   TODO: produzione di un aggiornamento del buffer in mutua esclusione
   */
  
  // inserisco il gioco dentro il buffer per babbo natale il cui accesso è gestito da semafori
  
  Wait_Sem(bb->semid, ELFI);
  bb->buffer++;
  
  printf("Elfo Assemblatore 1, incremento il buffer %d  %d\n",bb->buffer, gioco);
  Signal_Sem(bb->semid, BABBO);
  
  
  
  
  
}

void Consumatore2(MonitorProdCons *pc, BufferBabbo *bb) {
    int gioco;
    
	/*
    TODO: consumo con monitor e vettore di stato
    */
   
   enter_monitor(&(pc->m));
   
   // condizione di sincronizzazione
    if(pc->num_occupati_tipo_2 == 0)
        wait_condition(&(pc->m), CV_ASS_2);
        
        int i = 0;
        while (i < DIM && pc->stato[i] != OCCUPATO_GIOCO_2) i++;
        
    pc->stato[i] = IN_USO;

    pc->num_occupati_tipo_2--;

    leave_monitor(&(pc->m));
    
    // inizia la comsumazione
    
    gioco = pc->buffer[i];

	printf("Consumo - posizione %d, gioco %d\n", i, gioco);
    
	/*
    TODO: consumo con monitor e vettore di stato
    */

    enter_monitor(&(pc->m));
    
    pc->num_liberi++;
    pc->stato[i] = LIBERO;
    
    signal_condition(&(pc->m), CV_FAB);
    
    leave_monitor(&(pc->m));
    
    
    /*
    TODO: produzione di un aggiornamento del buffer in mutua esclusione
    */
   
    Wait_Sem(bb->semid, ELFI);
   
    bb->buffer++;
    
    printf("Elfo Assemblatore 2, incremento il buffer %d  %d\n",bb->buffer, gioco);
    Signal_Sem(bb->semid, BABBO);
    
    
}

void BabboNatale(BufferBabbo *bb) {
    
    /*
    TODO: consumo di un aggiornamento del buffer in mutua esclusione
    */
   for(int i = 0; i < TOT_GIOCATTOLI; i++){
       
        Wait_Sem(bb->semid, BABBO);
        
        printf("Sono Babbo Natale, ho consegnato un totale di %d regali\n",bb->buffer);

        Signal_Sem(bb->semid, ELFI);   
    
    }
}