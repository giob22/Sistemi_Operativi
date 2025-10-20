# Operazioni sul semaforo

Il `semaforo` è un **tipo di dato astratto** su cui sono definite le seguenti operazioni:

- **inizializzazione** della variabile ad un valore **NON negativo**;
- `wait()`: ha l'effetto di **decrementare** il valore del semaforo. Se il valore del semaforo diventa negativo, il processo che chiama la procedura `wait()` si auto blocca poiché un valore negativo indentifica la mancanza della disponibilità della risorsa;
- `signal()`: ha l'effetto di **incrementare** il valore del semaforo. Se il valore del semaforo è minore o uguale a zero, l'effetto della chiamata `signal()` produce il risveglio un processo che si era sospeso durante la chiamata della `wait()`.

**IMPORTANTE:** le procedure `wait()` e `signal()` vengono eseguite in maniera **mutuamente esclusiva**. Ovvero nel momento in cui un processo esegue una delle due procedure, questo non può esser prerilasciato a favore di un altro processo che potrebbe a sua volta chiamare la stessa procedura.

---
*ESEMPIO:*

Supponiamo di avere due processi *P1* e *P2* e che due procedure non siano eseguite in maniera mutuamente esclusiva:

- *P1* chiama la `wait()`, legge quindi il valore del semaforo `s.value = 1`.
- prima che *P1* decrementi il `s.value` questo viene prerilasciato dalla CPU a favore di *P2*.
- *P2* esegue `wait()` e legge anche lui lo stesso valore `s.value = 1`, decrementa il valore ed entra nella **sezione critica**.
- *P1* una volra che ottiene nuovamente la CPU decrementa nuovamente il valore del semaforo poiché ha verificato precedentemente che questo è pari ad $1$, ma in realtà `s.value = -1`. Quindi entra anche esso in **sezione critica**.

risultato: due processi entrano nella **sezione critica** -> mutua escluzione violata.