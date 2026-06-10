# LSN---Laboratorio-Simulazione-Numerica

Questo repository raccoglie tutte le esercitazioni svolte durante il corso di Laboratorio di Simulazione Numerica. Ogni cartella è dedicata a una specifica lezione e include i codici in C++ (dove richiesti), i notebook Jupyter per l'analisi dei dati ed eventuali file input.

## Struttura del Repository
```text
Laboratorio-di-Simulazione-Numerica-LSN-
├── GenRandom/
├── Lezione01/
├── Lezione02/
├── Lezione03/
├── Lezione04/
├── Lezione06/
├── Lezione07/
├── Lezione08/
├── Lezione09/
├── Lezione10/
├── Lezione11/
├── Lezione12/
└── README.md

- GenRandom/: Contiene il generatore di numeri casuali (necessario per il funzionamento delle lezioni 1, 2, 3, 8, 9 e 10).
- LezioneXX/: Cartelle dedicate alle singole lezioni. Ciascuna contiene le sottocartelle dei relativi esercizi (EsercizioXX.Y/) e il Jupyter Notebook con l'analisi dei risultati.

## Argomenti Trattati

| Lezione | Argomenti |
| --- | --- |
| 01 | Metodi Monte Carlo e tecnica del data blocking |
| 02 | Importance sampling e random walk |
| 03 | Integrazione stocastica e valutazione di opzioni finanziarie |
| 04 | Dinamica molecolare |
| 06 | Modello di Ising |
| 07 | Dinamica molecolare e tail corrections |
| 08 | Monte Carlo Variazionale e Simulated Annealing |
| 09 | Problema del commesso viaggiatore (TSP) e algoritmi genetici |
| 10 | Algoritmi genetici e tecniche di parallelizzazione |
| 11 | Introduzione alle reti neurali |
| 12 | Reti neurali profonde (DNN) e convoluzionali (CNN) |


## Compilazione ed Esecuzione

Per compilare ed eseguire i programmi in C++, seguire questi passaggi:

1. Entrare all'interno della cartella.
2. Selezionare la sottocartella con l'esercizio di interesse.
3. Compilare il codice sorgente tramite il comando:
make
4. Eseguire l'eseguibile generato:
./EsercizioXX_Y.exe

### Note Particolari e Comandi Aggiuntivi
* Lezioni 11 e 12: Queste unità non contengono codice C++. Il lavoro si concentra esclusivamente sull'analisi dei dati e sulla costruzione dei modelli tramite i notebook Jupyter.
