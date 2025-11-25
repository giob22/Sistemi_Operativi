#include "header.h"




void send_sincrona(message* messaggio, int msq_id){
	
	int ret = 0; // conterrà il valore di ritorno delle varie primitive
		     // mi servirà per capire se ci sono stati degli errori
	synch_msg ack;
	ack.type = RTS;
	ret = msgsnd(msq_id, &ack, sizeof(synch_msg) - sizeof(long), 0);
	
	if (ret < 0)
	perror("errore nell'invio della RTS");
	
	// dopo l'invio di un request to send message aspetto l'eventuale risposta RTR
	ret = msgrcv(msq_id, &ack, sizeof(synch_msg) - sizeof(long), RTR, 0);
	if (ret < 0)
	perror("errore nella ricezione della RTR");
	// attendo la ricezione di un RTR
	// nel momento in cui questo arriva allora sono sicur{o che c'è un processo pronto ad ascoltare 
	// quindi posso inviare in maniera asincrona il messaggio
	ret = msgsnd(msq_id, messaggio, sizeof(message) - sizeof(long), 0); 
	if (ret < 0) 
		perror("errore nell' invio del messaggio!!");
	LOGP("messaggio inviato");
}


void receive_bloccante(message* messaggio, int msq_id, int type){
	
	int ret = 0;
	synch_msg ack;

	// attendo un eventuale messaggio di richiesta di invio
	ret = msgrcv(msq_id, &ack, sizeof(synch_msg) - sizeof(long), RTS, 0);
	

	if (ret < 0)
		perror("errore nell'invio di RTR");
	// una volta ottenuta una RTS
		
	// invio una RTR
	ack.type = RTR;
	ret = msgsnd(msq_id, &ack, sizeof(synch_msg) - sizeof(long), 0);
	if (ret < 0)
		perror("errore nell'invio di RTR");
	// il processo si mette in attesa del messaggio effettivo
	ret = msgrcv(msq_id, messaggio, sizeof(message) - sizeof(long), type, 0); 
	if (ret < 0)
		perror("errore nella ricezione del messaggio");
}

void produci(int msq_id, char* text){
	message messaggio;
	messaggio.type = TYPE;
	strcpy(messaggio.text, text);
	LOGP("avvio la produzione");
	send_sincrona(&messaggio, msq_id);
}
void consuma(int msq_id){
	message messaggio;
	LOGC("pronto per ricevere il messaggio");
	receive_bloccante(&messaggio, msq_id, TYPE);
	LOGC("messaggio ricevuto \ntype: %ld\ttext: %s", messaggio.type, messaggio.text);
}

int init_msq(key_t key){
	int res = msgget(key, IPC_CREAT | 0664);
	return res;
}


