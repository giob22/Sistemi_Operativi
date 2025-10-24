### Esercizio

Implementare un'applicazione a linea di comando per creare, leggere, scrivere e cancellare un segmento di memoria condivisa. La memoria condivisa viene creata ogni volta durante le operazioni descritte precedentemente. Considerare il seguente esempio di utilizzo:

```console
# Scrittura su shared memory
$ shmtool  w   test
$ shmtool  w   "This is a test"

# Lettura su shared memory
$ shmtool  r   

# Cancellazione della shared memory
$ shmtool  d
```