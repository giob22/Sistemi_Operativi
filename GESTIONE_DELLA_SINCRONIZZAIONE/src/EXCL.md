<script src="../javascript.js"></script>

# UTILIZZO DI EXCL

## CASO CREAZIONE DI UNA MESSAGE QUEUE

Come verificare se una coda è stata già creata o meno. Per farlo abbiamo diverse alternative

-  ```c
    key_t key = ...;
    int queue = msgget(key, IPC_CREAT | IPC_EXCL | 0664);
    if (queue < 0){
        // oppure queue == -1
        // errno = EEXIST se la coda già esiste
        if (errno == EEXIST){
            queue = msgget(key, 0);
        }else{
            // qualsiasi altro errore che ha portato in fallimento l'operazione
            perror("errore");
            exit(1);
        }
    }else{
        // errore perché la message queue è stata appena creata
        // errore se ci si aspetta che lo sia già
        msgctl(queue, IPC_RMID, NULL);
        perror("errore");
        exit(1);
    }
    ... // codice
   ```

-   ```c
    key_t key = ...;
    int queue = mssget(key, 0); // il kernel cerca un'associazione tra key e msqid_ds
    if (queue < 0){
        if (errno == ENOENT){ 
            // ovvero ENOENT indica che non c'è corrispondeza tra chiave e descrittore di code di messaggi
            perror("errore");
            exit(1);
        }else{
            // caso di un qualsiasi altro errore differente
            perror("errore");
            exit(1);
        }
    }
    ... // codice
    ```