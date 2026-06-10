#include "percorso.h"

//costruttore
Percorso::Percorso() {
    m_L1=0.;
}

//costruttore: assegno il percorso
void Percorso::SetPercorso(const vector<int> &percorso){
    m_percorso=percorso;
}

//restituisco il percorso
vector<int> Percorso::GetPercorso() const {
    return m_percorso;
}

//restituisco la lunghezza del percorso
double Percorso::GetL1() const {
    return m_L1;
}

//effettuo il check delle diverse condizioni
bool Percorso::Controlla_vincoli() const {
    
    //constrollo che la città iniziale sia fissata (ho scelto di iniziare dalla 0)
    if(m_percorso[0]!=0){
        cout<<endl<<"ERRORE: città iniziale NON corretta!"<<endl;
        return false;
    }

    //controllo che nessuna città compaia più di una volta
    for(int k=0; k<m_percorso.size(); k++){
        for(int j=k+1; j<m_percorso.size(); j++)
            if(m_percorso[k]==m_percorso[j]){
                return false;
            }
    }

    return true;
}

//ora devo calcolare la lunghezza del percorso: prendo la prima città e quella la successiva, calcolo la distanza e procedo "spostandomi in là di una città"
double Percorso::Calcola_L1(const vector<Citta> &luoghi){

    double L1=0;
    int c1,c2;
    double lx,ly; //distanza in x e distanza in y

    for(int k=0; k<m_percorso.size()-1; k++){ //scorro tutte le città per calcolare le distanze tra una città e quella successiva
        c1=m_percorso[k]; //prendo l'indice della prima città
        c2=m_percorso[k+1]; //prendo l'indice della seconda città

        lx=luoghi[c1].GetX()-luoghi[c2].GetX(); //seleziono le città c1 e c2 dal vector che contiene tutte le città del percorso
        ly=luoghi[c1].GetY()-luoghi[c2].GetY();

        L1+=sqrt(lx*lx+ly*ly);
    }

    //calcolo a parte l'ultimo tratto del percorso, ovvero dall'ultima città alla città di partenza (città 0)
    int ultimo=m_percorso.size()-1;

    c1=m_percorso[ultimo];
    c2=m_percorso[0];

    lx=luoghi[c1].GetX()-luoghi[c2].GetX();
    ly=luoghi[c1].GetY()-luoghi[c2].GetY();

    L1+=sqrt(lx*lx+ly*ly);

    m_L1=L1;

return L1;
}


//creo una funzione per la stampa del percorso
void Percorso::Stampa() const {

    for(int k=0; k<m_percorso.size(); k++){
        cout<<m_percorso[k]<<"  ";
    }

    
    cout<<endl;

}

//devo definire l'operatore < per l'oggetto percorso perchè mi servirà poi per ordinare la popolazione
//devo "istruirlo" a capire come valutare se un percorso è < di un altro
bool Percorso::operator<(const Percorso &p) const{
    //un percroso è minore di un altro se la sua lunghezza è minore di quella dell'altro percorso
    return m_L1<p.m_L1; 
}