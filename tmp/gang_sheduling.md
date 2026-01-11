# gang sheduling

l'obiettivo principale é quello di schedulare contemporaneamente un set di thread correlati tra loro, ovvero aventi un gradi di sincronizzazione fine.

Il motivo é per velocizzare la sincronizzazione tra questi, facendoli eseguire in parallelo sui processori disponibili evitando cosí disallineamenti nella loro esecuzione.

ad ogni processo che rappresenta un gang di thread correlati viene assegnato \\(1/n\\) di tempo su \\(M\\) processori utilizzando un politica time sharing.

se non si tiene conto per la scelta del time slice della quantitá di thread contenuti in ogni singolo processo allora questa potrebbe essere una metrica inefficiente in termini di utilizzo dei processori.

Si utilizza quindi una assegnazione pesata, in cui il time slice che viene assegnato ad un processo é pesato per i thread contenuti in esso rispetto ai thread di tutti i processi che sono pronti per essser schedulati.

Questo approccio aumenta la fairness, inoltre evita che per lunghi periodi di tempo alcuni processori sono inattivi ovvero in idle loop.

