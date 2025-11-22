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

void checksum(int queue_filter_checksum, int queue_checksum_visual){

        int ret, i, j, checksum;
        message mess;

        for (j=0; j<NUM_MESSAGES; j++){

                printf("[checksum] Ricevo dal processo Filter...\n");

                /* TODO: ricevere il messaggio dal processo filter */
                ret = msgrcv(queue_filter_checksum, &mess, sizeof(message) - sizeof(long), MSG_TYPE, IPC_NOWAIT);
                // devo tenere in conto che non tutti i messaggi inviati al filer arriveranno ad essere inviati
                // quindi il numero di messaggi che potrà ottenere checksum non è detto che sia proprio 8.
                // sempre a meno di errori durante l'invio di un messaggio corretto
            
                if(ret<0) {
                        if (errno == ENOMSG){
                                printf("Non ci sono più messaggi da ricevere dal processo filter...exit!\n");
                                break;
                        }
                        else{
                                perror("ERROR!!!");
                                exit(-1);
                        }
                }
                /* TODO: Calcolare la checksum e inviarla al visualizzatore  */
                for (int i = 0; i < STRING_MAX_DIM; i++)
                {
                        mess.var += (int)mess.text[i];
                }
                for (int i = 0; i < INT_MAX_DIM; i++)
                {
                        mess.var += mess.interi[i];
                }

                printf("[checksum] Invio messaggio di CHECKSUM al Visualizzatore...\n");
                int ret_invio = msgsnd(queue_checksum_visual, &mess, sizeof(message)- sizeof(long), 0);
                if (ret_invio < 0)
                {
                        perror("msgsnd del messaggio on ds_queue_checksum_visual FALLITA!");
                        exit(-1);
                }
                
        }
        
        exit(0);
}

