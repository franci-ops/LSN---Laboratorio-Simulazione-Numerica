#include "ga.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

//costruttore
AlgoritmoGenetico::AlgoritmoGenetico(int individui, int generazioni, Random &gen){
    m_individui=individui;
    m_generazioni=generazioni;
    m_gen=gen;

    //iniizializzo le probabilità di crossover e delle varie mutazioni
    m_pcross=0.8;
    m_pscambio=0.1;
    m_pshift=0.1;
    m_pperm=0.1;
    m_pinv=0.1;

    m_pelitismo=0.8;
}

//selezione basata sul rango
int AlgoritmoGenetico::Selezione_rango(){
    double r=m_gen.Rannyu(); //genero numero uniformemente distribuito tra 0 e 1
    double p=2.; //preferirò la selezione di percorsi brevi

    int M=m_individui; //numero di individui
    int j=int(M*pow(r,p)); //non ho aggiunto +1 perchè gli indici degli individui partono da 0

    //eseguo un controllo sull'indice affinchè l'inidce selezionato non sia oltre il limite
    if(j>=M){
        j=M-1; //M-1 è l'ultimo indice possibile
    }

return j;  //resituisce l'indice dell'individuo scelto per la riproduzione
}

//implemento il crossover
vector<Percorso> AlgoritmoGenetico::Crossover(const Percorso &g1, const Percorso &g2){
    vector<Percorso> figli; //creo vector che conterrà i figli

    //il vettore di interi contiene la sequenza di città (ovvero il percorso) di padre e madre
    vector<int> padre=g1.GetPercorso(); 
    vector<int> madre=g2.GetPercorso();
    int N=madre.size();

    //creo i vector per i fligli: dimensione N, riempiti di -1 (il -1 lo scelgo perchè sicuramente non indica una città e quindi non dà problemi per i vincoli)
    vector<int> f1(N,-1); 
    vector<int> f2(N,-1);

    //genero il punto di taglio
    int taglio=int(m_gen.Rannyu(1,N-1));

    //mi servono i contatori per andare a riempire i vector dei figli
    int pos1=taglio; 
    int pos2=taglio;

    //genero i figli
    //copio la parte iniziale fino all'indice di taglio (escluso)
    for(int k=0; k<taglio; k++){
        f1[k]=padre[k];
        f2[k]=madre[k];
    }
    
    //genero f1: completo con le città della madre
    //scorro (con k) le città della madre e per ciascuna controllo eventuale presenza nel figlio. Se manca, la aggiungo
    for(int k=1; k<N; k++){
        int citta1=madre[k];
        bool check_citta1=false;

        for(int j=1; j<taglio; j++){
            if(f1[j]==citta1){ //se nel figlio è già presente la città
                check_citta1=true;
            }
        }

        //se nel figlio non è presente la città, la aggiungo
        if(check_citta1==false and pos1<N){
            f1[pos1]=citta1;
            pos1++;
        }

        
    }

    //genero f2: completo con le città del padre
    for(int k=1; k<N; k++){
        int citta2=padre[k];
        bool check_citta2=false;

        for(int j=1; j<taglio; j++){
            if(f2[j]==citta2){ //se nel figlio è già presente la città
                check_citta2=true;
            }
        }
        if(check_citta2==false and pos2<N){
            f2[pos2]=citta2;
            pos2++;
        }
        

    }
    

Percorso p1, p2;
p1.SetPercorso(f1);
p2.SetPercorso(f2);

if(p1.Controlla_vincoli()==true){
figli.push_back(p1);
}else{
    cout<<endl<<"ERRORE: figlio non rispetta vincoli!"<<endl;
    exit(-1);
}

if(p2.Controlla_vincoli()==true){
figli.push_back(p2);
}else{
    cout<<endl<<"ERRORE: figlio non rispetta vincoli!"<<endl;
    exit(-1);
}
    return figli;
}



//MUTAZIONE 1: SCAMBIO DI DUE CITTà QUALSIASI
void AlgoritmoGenetico::Mutazione_scambio(Percorso &p){

    vector<int> percorso=p.GetPercorso();
    int N=percorso.size();
    
    //genero due indici tra 1 e N-1 (così non tocco la città 0 di partenza)
    int i1=int(m_gen.Rannyu(1,N));
    int i2;

    do{
    i2=int(m_gen.Rannyu(1,N));
    }while(i1==i2);
    swap(percorso[i1],percorso[i2]);
    
    p.SetPercorso(percorso);
    
    if(p.Controlla_vincoli()==false){
        cout<<endl<<"ERRORE: percorso non rispetta vincoli!"<<endl;
        exit(-1);
    }
}


//MUTAZIONE 2: SHIFT DI M CITTA' CONTIGUE
void AlgoritmoGenetico::Mutazione_shift(Percorso &p){

    vector<int> percorso=p.GetPercorso();
    int N=percorso.size();

    //estraggo quante città spostare
    int num=int(m_gen.Rannyu(1,N-1)); //bloccco di lunghezza almeno 1 e massimo N-2

    //estraggo punto di partenza del blocco evitando l'indice 0, associato alla prima città
    int start=int(m_gen.Rannyu(1,N-num));

    //ricorstruisco il blocco di città da shiftare
    vector<int> blocco;

    //copio nel blocco le città consecutive selezionate
    for(int k=0; k<num; k++){
        blocco.push_back(percorso[start+k]);
    }

    //rimuovo il blocco dal percorso, tenendo solo le città fuori dall'intervallo [start,start+num]
    vector<int> resto;
    for(int k=0; k<N; k++){
        if(k<start or k>=start+num){
            resto.push_back(percorso[k]);
        }
    }

    //estraggo posizione in cui inserire il blocco (evito indice 0)
    int pos=int(m_gen.Rannyu(1,resto.size()));

    //ricostriusco il percorso inserendo il blocco in pos
    vector<int> percorso_finale;

    //inserisco fino alla posizione estratta le città "esterne" al blocco
    for(int k=0; k<pos; k++){
        percorso_finale.push_back(resto[k]);
    }

    //inserisco il blocco shiftato
    for(int k=0; k<num; k++){
        percorso_finale.push_back(blocco[k]);
    }

    //inserisco le città mancanti
    for(int k=pos; k<int(resto.size()); k++){
        percorso_finale.push_back(resto[k]);
    }

    p.SetPercorso(percorso_finale);

    if(p.Controlla_vincoli()==false){
        cout<<endl<<"ERRORE: percorso non rispetta vincoli!"<<endl;
        exit(-1);
    }

}

//MUTAZIONE 3: PERMUTAZIONE TRA GRUPPI
void AlgoritmoGenetico::Mutazione_permutazione(Percorso &p){

    vector<int> percorso=p.GetPercorso();
    int N=percorso.size();

    //estraggo dimensione dei blocchi, ovvero il numero totale di città da permutare, assicurandomi che sia <N/2
    int dim=int(m_gen.Rannyu(1,N/2));

    //scelgo gli indici di partenza dei due blocchi in modo che non siano sovrapposti
    //il primo indice deve trovarsi nella prima metà del percorso
    int start1=int(m_gen.Rannyu(1,N/2-dim+1));
    int start2=int(m_gen.Rannyu(N/2,N-dim));

    //scambio uno ad uno i vari elementi dei due blocchi
    for(int k=0; k<dim; k++){
        swap(percorso[start1+k],percorso[start2+k]);
    }

    p.SetPercorso(percorso);

    //aggiorno percorso
    if(p.Controlla_vincoli()==false){
    cout<<endl<<"ERRORE: percorso non rispetta vincoli!"<<endl;
    exit(-1);
    }
}


//MUTAZIONE 4:INVERSIONE
void AlgoritmoGenetico::Mutazione_inversione(Percorso &p){

    vector<int> percorso=p.GetPercorso();
    int N=percorso.size();

    //estraggo lunghezza del pezzo da invertire (al minimo 2 città, al massimo N-1 città così la città 0 resta in posizione iniziale)
    int dim=int(m_gen.Rannyu(2,N));

    //estraggo posizione di partenza
    int pos=int(m_gen.Rannyu(1,N-dim+1));

    int sx=pos;
    int dx=pos+dim-1;

    //inverto l'ordine
    while(sx<dx){
        swap(percorso[sx],percorso[dx]);
        sx++;
        dx--;
    }

    p.SetPercorso(percorso);


    if(p.Controlla_vincoli()==false){
        cout<<endl<<"ERRORE: percorso non rispetta vincoli!"<<endl;
        exit(-1);
    }
}


//implemento il CICLO EVOLUTIVO
void AlgoritmoGenetico::Evoluzione(const vector<Citta> &citta, ofstream &fileout, int rank, int size, bool cond){

    //definisco dopo quante generazioni eseguire la migrazione
    int Nmigr=100;

    //genero la popolazione iniziale, in modo che sia formata da m_individui
    m_popolazione.Genera_popolazione(m_individui,citta.size(),m_gen);

    //controllo che tutti gli individui soddisfino i vincoli
    for(int k=0; k<m_individui; k++){
        if(m_popolazione.GetPercorso(k).Controlla_vincoli()==false){
            cout<<endl<<"ERRORE: individuo "<<k<<"non rispetta i vincoli! ESCO dal programma."<<endl;
            exit(-1);
        }
    }


    //eseguo un ciclo sul numero di generazioni
    for(int gen=0; gen<m_generazioni; gen++){

        //prima di tutto devo ordinare la popolazione
        m_popolazione.Ordina(citta);

        //implemento codice per migrazione MPI: solo se cond==true
        //faccio migrazione ogni volta che il numero della generazione corrente è pari a un multiplo Nmigr, tranne alla prima generazione
         if(cond==true and gen>0 and gen%Nmigr==0){

            Migrazione_MPI(rank,size);

            m_popolazione.Ordina(citta);

            cout<<"Rank "<<rank<<" ha effettuato migrazione alla generazione "<<gen<<endl;
        }

        //creo un vector per la nuova popolazione, ovvero per la popolazione dei figli + eventualmente individuo migliore
        vector<Percorso> nuova_popolazione;

        //ELITISMO= Il migliore passa con una probabilità dell'0.8 (NON SO SE METTERLO O NO)
        if(m_gen.Rannyu()<m_pelitismo){
        nuova_popolazione.push_back(m_popolazione.GetPercorso(0));
        }

        //genero i figli finchè la popolazione è piena
        while(int(nuova_popolazione.size())<m_individui){

            //seleziono i due genitori
            int g1=Selezione_rango();
            int g2=Selezione_rango();

            //risalgo ai percorsi di padre e madre e li salvo in variabili apposite
            Percorso madre=m_popolazione.GetPercorso(g1);
            Percorso padre=m_popolazione.GetPercorso(g2);

            Percorso figlio1, figlio2;

            //eseguo il crossover con probabilità del 80%
            if(m_gen.Rannyu()<m_pcross){
                vector<Percorso> figli=Crossover(padre,madre);

                figlio1=figli[0];
                figlio2=figli[1];}
            else{ //se non c'è crossover, i figli sono identici ai genitori
                figlio1=madre;
                figlio2=padre;

            }             

            //applico eventuali mutazioni (estratte con la rispettiva probabilità)
            //mutazione scambio


            if(m_gen.Rannyu()<m_pscambio){
                Mutazione_scambio(figlio1);
            }
            if(m_gen.Rannyu()<m_pscambio){
                Mutazione_scambio(figlio2);
            }            


            //mutazione shift
            if(m_gen.Rannyu()<m_pshift){
                Mutazione_shift(figlio1);
            }
            if(m_gen.Rannyu()<m_pshift){
                Mutazione_shift(figlio2);
            }


            //mutazione permutaizone
            if(m_gen.Rannyu()<m_pperm){
                Mutazione_permutazione(figlio1);
            }
            if(m_gen.Rannyu()<m_pperm){
                Mutazione_permutazione(figlio2);
            }


            //mutazione inversione
            if(m_gen.Rannyu()<m_pinv){
                Mutazione_inversione(figlio1);
            }
            if(m_gen.Rannyu()<m_pinv){
                Mutazione_inversione(figlio2);
            }


            //controllo i vincoli e se i figli li rispettano li aggiungo alla nuova popolazione
            if(figlio1.Controlla_vincoli()==true and int(nuova_popolazione.size())<m_individui){
                nuova_popolazione.push_back(figlio1);
            }
            if(figlio2.Controlla_vincoli()==true and int(nuova_popolazione.size())<m_individui){
                nuova_popolazione.push_back(figlio2);
            }

        }

        //implemento la media della metà migliore della popolazione
        double somma=0.;
        double media;
        for(int w=0; w<m_individui/2; w++){
            somma+=m_popolazione.GetPercorso(w).GetL1();
        }

        media=somma/(m_individui/2);

        fileout<<gen+1<<"   "<<m_popolazione.GetPercorso(0).GetL1()<<"    "<<media<<endl;


    m_popolazione.SetPopolazione(nuova_popolazione);

    }

//ordino la popolazione finale
m_popolazione.Ordina(citta);

}


//restituisce l'individuo migliore (mi serve per l'elitismo)
Percorso AlgoritmoGenetico::GetMigliore() const {
    return m_popolazione.GetPercorso(0);
}


//implemento la migrazione: rank=chi sono io; size=quanti rank siamo
void AlgoritmoGenetico::Migrazione_MPI(int rank, int size){
    
    //prendo il milgior individuo del continente locale (si troverà in posizione 0 eprchè popolazione è ordinata)
    vector<int> migliore=m_popolazione.GetPercorso(0).GetPercorso();
    
    int N=migliore.size();

    //scelgo casualmente un continente (diverso da me) a cui passare l'individuo
    int continente; //il continente sarà un intero che corrisponde al rank a cui mando individuo

    //uso un do while estraendo fino a quando ottengo un indice diverso da me stesso
    do{
        continente=int(m_gen.Rannyu(0,size)); //estraggo casualmente il rank con cui scambiare
    }while(continente==rank);


    //il rank 0 costruisce la permutazione
    vector<int> ordine(size);

    if(rank==0){

        //genero il vettore con i continenti ordinati, poi li mescolo
        for(int i=0; i<size; i++){ //size è il numero di rank
            ordine[i]=i;
        }

        //mescolo casualmente l'ordine dei continenti
        for(int i=0; i<size; i++){
            int j=int(m_gen.Rannyu(0,size)); //estraggo casualmente il rank
            swap(ordine[i],ordine[j]);
        }
    }


    //comunico permutazione a tutti i continenti
    MPI_Bcast(
        ordine.data(),   //indirizzo della comunicazione da inviare (ordine continenti)
        size,            //numero di interi contenuti in ordine
        MPI_INT,         //tipo degli elementi
        0,               //rank del processo sorgente
        MPI_COMM_WORLD   //comunicatore che contiene tutti i processi
    );

    //trovo posizione del mio rank nel vettore "mescolato"
    int pos;

    for(int i=0; i<size; i++){

        if(ordine[i]==rank){
            pos=i;
        }
    }

    //scelgo il continente a cui invio: sarà quello posizionato subito dopo di me
    int dest;
    if(pos==size-1){ //caso particolare se sono l'ultimo nel vector
        dest=ordine[0];   //chiudo il cerchio
    }else{
        dest=ordine[pos+1];
    }

    //scelgo continente da cui ricevo: quello posizionato prima di me
    int sorg;
    if(pos==0){ //caso particolare se sono il primo del vecotr
        sorg=ordine[size-1]; //chiudo il cerciho
    }else{
        sorg=ordine[pos-1];
    }

    //creo vettore che contenga percorso ricevuto
    vector<int> percorso(N);

    //variabile utilizzata da MPI_Sendrecv per memorizzare lo stato della ricezione.
    MPI_Status stat;

    //ivio il miglior individuo al continente "dest" e, contemporaneamente, ricevo un individuo dal continente "sorg"
    //MPI_Sendrecv evita possibili deadlock eseguendo invio e ricezione in un'unica operazione.
    MPI_Sendrecv(
        migliore.data(),    //dati da inviare: invio il percorso imgliore nel mio contnente
        N,                  //numero di elementi inviati
        MPI_INT,            //tipo degli elementi inviati
        dest,               //rank destinatario
        1,                  //tag del messaggio inviato (1 perchè nel programma c'è un solo tipo di comunicazione: la migrazione degli individui)

        percorso.data(),    //area di memoria per la ricezione: salvo il percorso che mi arriva in percorso
        N,                  //numero massimo di elementi ricevuti
        MPI_INT,            //tipo degli elementi ricevuti
        sorg,               //rank sorgente
        1,                  //tag del messaggio atteso

        MPI_COMM_WORLD,     //comunicatore
        &stat               //stato della ricezione
    );

    //aggiungo individuo ricevuto
    Percorso migr;

    migr.SetPercorso(percorso);

    //sostituisco individuo ricevuto al posto del peggiore della popolazione
    m_popolazione.SetPercorso(m_individui-1,migr);
}