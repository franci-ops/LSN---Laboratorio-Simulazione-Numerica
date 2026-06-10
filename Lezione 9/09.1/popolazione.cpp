#include "popolazione.h"

//costruttore
Popolazione::Popolazione() {}

//genero la popolazione iniziale
void Popolazione::Genera_popolazione(int individui, int citta, Random &gen){

    //pulisco il vector della vecchia popolazione (se è presente)
    m_popolazione.clear();

    //ciclo sugli individui
    for(int k=0; k<individui; k++){

        //creo un percorso in modo che le città siano inserite in modo ordinato, poi andrò a mescolarne l'ordine
        vector<int> percorso;

        for(int j=0; j<citta; j++){
            
            //la città 0 sarà in posizione 0 e via dicendo (così sono sicura che non inserisco la stessa città + volte)
            //l'ordine delle città lo modifico in seguito
            percorso.push_back(j);
        }

        //scelgo di fare 100 scambi per generare soluzioni sufficientemente diverse
        for(int i=0; i<100; i++){
            int a=int(gen.Rannyu(1,citta));  //estraggo partendo da 1 perchè la città 0 deve sempre essere al primo posto
            int b=int(gen.Rannyu(1,citta));

            int appo=percorso[a];
            percorso[a]=percorso[b];
            percorso[b]=appo;
        }

        //costruisco il mio individuo come un elemento della classe Percorso
        Percorso P;
        P.SetPercorso(percorso);

        //controllo che effettivamente il mio individuo rispetti i requisiti necessari
        if(P.Controlla_vincoli()==false){
            cout<<endl<<"ERRORE: VINCOLI NON RISPETTATI!"<<endl;
            exit(-1);
        }

        //aggiungo il percorso alla popolazione
        m_popolazione.push_back(P);
    }

}

//funzione che stampa tutta la popolazione (controllo che effettivamnete i percrosi stampati soddisfino i requisiti)
void Popolazione::Stampa() const {

    //stampo ogni individuo della popolazione
    for(int k=0; k<m_popolazione.size(); k++){

        cout<<"Percorso "<<k<<":    ";

        //stampo il percorso usando la funzione che ho implementato apposta
        m_popolazione[k].Stampa();
    }
}

//funzione per valutare la lunghezza L1 di un percorso
void Popolazione::Ordina(const vector<Citta> &citta){

    //ciclo for scorre tutti i percorsi
    for(int k=0; k<m_popolazione.size(); k++){
        //calcolo per ogni percorso la sua lunghezza
        m_popolazione[k].Calcola_L1(citta);
    }

    //uso la funzione sort della libreria algorithm per ordinare la mia popolazione in ordine crescente di lunghezza percorso
    //dato che ho implementato operator< in percorso, i percorsi verranno ordinati dal più breve al più lungo
    sort(m_popolazione.begin(),m_popolazione.end());
}


//implemento metodo che mi restituisce un determinato percorso di una popolazione
Percorso Popolazione::GetPercorso(int k) const{
    return m_popolazione[k];
}


//implemento funzione per iniizializzare una popolazione
void Popolazione::SetPopolazione(const vector<Percorso> &popolazione){
    m_popolazione=popolazione;
  }