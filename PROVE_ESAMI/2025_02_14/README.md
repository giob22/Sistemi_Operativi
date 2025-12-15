# ATTEZIONE

Ho riscontrato un errore:

- sono presenti due visualizzatori che devono scrivere a terminale 5 medie;
- il professore ha inserito nello scheletro della prova d'esame un for per ogni visualizzatore con valore finale `NUM_MESSAGGI_PER_SENSORE/2`.

`NUM_MESSAGGI_PER_SENSORE = 5` quindi la divisione tra due interi restituisce il valore della parte intera che in questo caso è `2`.

Quindi non saranno visualizzate tutte le medie, ma solamente `4` di queste.