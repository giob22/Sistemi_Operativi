#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "header.h"

void filtro(int ds_queue_gen_filter, int ds_queue_filter_checksum){

        int ret, i;
        message mess;

        for (i=0; i<NUM_MESSAGES; i++){

                /* TODO: ricevere il messaggio dai processi generatori consumatori */
                ret = msgrcv(ds_queue_gen_filter, &mess, sizeof(message) - sizeof(long), MSG_TYPE, 0);
                // è un receive bloccante → sono sicuro che saranno presenti tutti e 8 i messaggi all'interno della message queue
                // a meno di errori di invio da parte dei gen consumatori

                if(ret<0) {
                        perror("msgrcv on ds_queue_gen_filter FALLITA!");
                        exit(-1);
                }

                printf("[filtro] Ricevuto #%d messaggio dal generatore...\n", i);
                //ricerca del carattere 'x'
                printf("[filtro] Ricerca carattere 'x' sulla stringa: %s...\n", mess.text);
            
                /* TODO: effettuare la ricerca del caratter 'x' e inviare il messaggio al processo checksum nel caso di carattere non trovato */
                int ret_invio;
                if (strchr(mess.text, 'x') == NULL) 
                // il messaggio deve essere inviato
                        ret_invio = msgsnd(ds_queue_filter_checksum, &mess, sizeof(message) - sizeof(long), 0);
                // altrimenti il messaggio viene scartato
                // verifichiamo che non ci siano stati problemi nell'invio
                if (ret_invio < 0){
                        perror("msgsnd del messaggio on ds_queue_filter_checksum FALLITA!");
                        exit(-1);
                }
        
                
                
        }
    
        exit(0);
}

