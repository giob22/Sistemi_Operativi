#include "procedure.h"


int consuma(Buffer* buff){
    enter_monitor(&(buff->m));
    // questa sarà l'unica differenza tra l'implementazione con signal-and-wait e signal-and-continue
    while (buff->num_occupati == 0) // significa che non ci sono dati da consumare
        wait_condition(&(buff->m), VARCOND_CONSUMATORI);
    // devo implementare il controllo con un while perché nel momento in cui il processo consumatore
    // viene sbloccato non si ha la certezza che la condizione di sincronizzazione sia ancora valida
    // come quando lo era nel momento in cui è stato sbloccato

    // se la condizione di sincronizzazione è valida allora esiste un buffer nel poll occupato
    int i = 0;
    while (i < DIM_BUFFER && buff->stato[i] != OCCUPATO)
        i++;
    
    // una volta trovato il buffer occupato lo setta come USO
    buff->stato[i] = USO;
    // e decrementa il numero di buffer occupati
    buff->num_occupati--;
    // ha questo punto ha un accesso esclusivo al buffer i-esimo del pool
    // quindi può operare al di fuori del monitor
    leave_monitor(&(buff->m));


    // consumazione
    sleep(getpid() % 4); // simulo una consumazione pesante
    int result = buff->buffer[i];
    printf("[CONSUMATORE] il valore consumato è: %d\n", buff->buffer[i]);

    // una colta terminata la consumazione il consumatore deve settare lo stato del buffer ad vuoto 
    // e incrementare il valore dei buffer vuoti
    // per farlo deve accedere alla variabile num_liberi e lo deve fare in mutua esclusione
    enter_monitor(&(buff->m));
    buff->stato[i] = VUOTO;
    buff->num_liberi++;

    // segnala un eventuale produttore in attesa sulla condition variable VARCOND_PRODUTTORI
    signal_condition(&(buff->m),VARCOND_PRODUTTORI);
    leave_monitor(&(buff->m));
    return result;
}
void produci(Buffer* buff, int valore_da_produrre){
    enter_monitor(&(buff->m));

    // verifica se la condizione di sincronizzazione è valida
    while (buff->num_liberi == 0)
        wait_condition(&(buff->m),VARCOND_PRODUTTORI);
    // uso sempre while perché nel momento in cui il produttore viene sboccato non è detto che entrarà direttamente nel monitor
    // quindi la condizione di sincronizzazione potrebbe non più essere valida nel momento in cui rientrerà nel monitor

    // se valida trova la posizione del buffer con stato vuoto
    int i = 0;
    while (i < DIM_BUFFER && buff->stato[i] != VUOTO)
        i++;
    // trovato la posizione setta lo stato del buffer i-esimo in USO in modo da avere un accesso esclusivo al buffer del pool
    // così da poter operare anche al di fuori del monitor
    buff->stato[i] = USO;
    // decrementa il valore dei buffer vuoti
    buff->num_liberi--;
    leave_monitor(&(buff->m));

    // simulo una produzione pesante
    sleep(getpid() % 10);
    
    printf("[PRODUTTORE] valore prodotto (%d)\n", valore_da_produrre);
    buff->buffer[i] = valore_da_produrre;

    // terminata la produzione il produttore deve modificare delle variabilli di stato del monitor
    // che determinano la veridicità delle condizioni di sincronizzazione
    // per farlo deve accedere in modo esclusivo a queste variabili, quindi entrare nel monitor

    enter_monitor(&(buff->m));
    buff->stato[i] = OCCUPATO;
    buff->num_occupati++;

    // segnala un eventuale processo consumatore in attesa sulla condition variable VARCOND_CONSUMATORI
    signal_condition(&(buff->m),VARCOND_CONSUMATORI);
    leave_monitor(&(buff->m));
}