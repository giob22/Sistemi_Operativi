#include "monitor_hoare.h"
#include "semafori.h"

#define DIM 6
#define TOT_GIOCATTOLI 20

/*
    TODO: definire quanto necessario per la gestione del MonitorProdCons con vettore di stato 
*/

#define CV_ASS_1 0
#define CV_ASS_2 2
#define CV_FAB 1

/*
ho bisogno di 3 condition variables:
- un produttore deve sospendersi se non ci sono posti liberi
- un asseblatore_1 deve sospendersi se non ci sono giocattoli di tipo 1
- un asseblatore_2 deve sospendersi se non ci sono giocattoli di tipo 2

*/

// CATEGORIE REGALI
#define TIPO1 1
#define TIPO2 2

#define LIBERO 0
#define OCCUPATO_GIOCO_1 1
#define IN_USO 2
#define OCCUPATO_GIOCO_2 3


/*
TODO: definire quanto necessario per la gestione del buffer mediante utilizzo di semafori
*/
// semafori per gestire la cooperazione tra babbo natale e gli elfi assemblatori
#define BABBO 0 // semaforo per indicare per BABBO NATALE
#define ELFI 1

typedef struct {
    /*
        TODO: completare struttura
    */
    int buffer[DIM];

    int stato[DIM];

    int num_liberi;
    int num_occupati_tipo_1;
    int num_occupati_tipo_2;

    Monitor m;
    
} MonitorProdCons;

typedef struct {
    /*
        TODO: completare struttura
    */
    int buffer;


    int semid;
} BufferBabbo;

void inizializza_vettore(MonitorProdCons *pc);
void rimuovi_vettore(MonitorProdCons *pc);
void inizializza_buffer(BufferBabbo * bb);
void rimuovi_buffer(BufferBabbo * bb);

void Produttore(MonitorProdCons *pc);
void produci(MonitorProdCons *pc, int tipo);
void Consumatore1(MonitorProdCons *pc, BufferBabbo *bb);
void Consumatore2(MonitorProdCons *pc, BufferBabbo *bb);
void BabboNatale(BufferBabbo *bb);