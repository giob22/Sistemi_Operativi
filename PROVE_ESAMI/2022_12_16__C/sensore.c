#include "header.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>

int main() {

    printf("Avvio processo sensore...\n");
    
    int key_coda = ftok(FTOK_PATH, FTOK_CHAR);
    int id_coda_sensore = msgget(key_coda, IPC_CREAT | 0664);

    if (id_coda_sensore < 0){
        // oppure id_coda_sensore == -1
        // errno = EEXIST se la coda già esiste
        if (errno == EEXIST){
            printf("\n\n\n SUCCESO \n\n");
            id_coda_sensore = msgget(key_coda, 0);
        }else{
            // qualsiasi altro errore che ha portato in fallimento l'operazione
            perror("errore");
            exit(1);
        }
    }else{
        // errore perché la message id_coda_sensore è stata appena creata
        // errore se ci si aspetta che lo sia già
        msgctl(id_coda_sensore, IPC_RMID, NULL);
        perror("errore");
        exit(1);
    }
    

    srand(time(NULL));

    for(int i=0; i<10; i++) {

        // TODO: inviare interi casuali sulla coda   
        Messaggio msg;
        msg.type = TIPO;
        msg.value = rand()%11;  
        
        int ret = msgsnd(id_coda_sensore, &msg, sizeof(Messaggio) - sizeof(long), 0);
        if (ret < 0) {
            perror("Errore nell'invio del messaggio");
            exit(2);
        }
        printf("[%d]Messaggio inviato: %d\n",getpid(),msg.value);
        
        sleep(1);
    }

    return 0;

}
