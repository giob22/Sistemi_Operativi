#include "header.h"

static int queue1;
static int queue2;

void init_synch(){
	queue1 = msgget(IPC_PRIVATE, IPC_CREAT | 0664);
	queue2 = msgget(IPC_PRIVATE, IPC_CREAT | 0664);
}
void leave_synch(){
	msgctl(queue1, IPC_RMID, 0);
	msgctl(queue2, IPC_RMID, 0);
}

// implementate seguendo il protocollo RENDEZVOUS
void send_synch(data_msg* message, int msq_id){
	ack_msg ack;
	ack.type = RTS;
	// invio un RTS
	#ifdef DEBUG
		prod("Sto inviando il primo messaggio di sincronizzazione: RTS (Request to send)");
	#endif
	if ( msgsnd(queue1, &ack, sizeof(ack_msg) - sizeof(long), 0) < 0){
		perror("Errore nell'invio di un RTS");
		exit(-1);
	}
	#ifdef DEBUG
		prod("sono in attesa di ricevere un OTS (OK to send)");
	#endif
	// ascolto un OTS
	if ( msgrcv(queue2, &ack, sizeof(ack_msg) - sizeof(long), OTS, 0) < 0){
		perror("Errore nella ricezione di un OTS");
		exit(-1);
	}
	#ifdef DEBUG
		prod("ho ricevuto l'OTS quindi invio il messaggio effettivo");
	#endif
	// invio del messaggio effettivo perché il processo destinatario è pronto per riceverlo
	if ( msgsnd(msq_id, message, sizeof(data_msg) - sizeof(long), 0) < 0){
		perror("Errore nel'invio del messaggio effettivo");
		exit(-1);
	}

}
void receive_synch(data_msg* message, int msq_id, long type){
	ack_msg ack;
	// ascolta un RTS
	#ifdef DEBUG
		cons("sono in attesa di un messaggio di sincronizzazione: RTS");
	#endif
	if ( msgrcv(queue1, &ack, sizeof(ack_msg) - sizeof(long), RTS, 0) < 0){
		perror("Errore nella ricezione del RTS");
		exit(-2);
	}
	// una volta ricevuto
	// invia un OTS
	#ifdef DEBUG
		cons("ho ottenuto il messaggio di sincronizzazione, invio un messaggio di OTS");
	#endif
	ack.type = OTS;
	if ( msgsnd(queue2, &ack, sizeof(ack_msg) - sizeof(long), 0) < 0){
		perror("Errore nell'invio del OTS");
		exit(-2);
	}
	// a questo punto il destinatario ascolta quello che viene inserito nella cosa dei messaggi
	#ifdef DEBUG
		cons("sono in attesa del messaggio dal processo mittente");
	#endif
	if ( msgrcv(msq_id, message, sizeof(data_msg) - sizeof(long), type, 0) < 0){
		perror("Errore nella ricezione del messaggio");
		exit(-2);
	}
}

void produci(int queue_id, char* text){
	data_msg message;
	message.type = MESSAGGIO;
	strcpy(message.text, text);
	send_synch(&message, queue_id);
	printf("[P] Messaggio inviato\n");
}
void consuma(int queue_id){
	data_msg message;
	receive_synch(&message, queue_id, MESSAGGIO);
	printf("[C] messaggio ricevuto\ntype: %ld\ttext: %s\n", message.type, message.text);
}

