#ifndef __AlgoritmoGenetico__
#define __AlgoritmoGenetico__

#include "../../RANDOM/random.h"
#include "popolazione.h"
using namespace std;

//questa classe mi serve in particolare per implementare mutazioni e crossover

class AlgoritmoGenetico {

private:
    Popolazione m_popolazione;
    Random m_gen;

    int m_individui; //numero individui popolazione
    int m_generazioni;  //quante generazioni faccio evolvere
    
    double m_pcross;
    double m_pscambio;
    double m_pshift;
    double m_pperm;
    double m_pinv;

    double m_pelitismo;

public:
    AlgoritmoGenetico(int individui, int generazioni, Random &gen); //costruttore
    int Selezione_rango(); //implemento la selezione basata sul rango, e lavorerò su una popolazione già ordinata grazie all'apposita funzione che ho creato
    vector<Percorso> Crossover(const Percorso &g1, const Percorso &g2); //prende in ingresso due genitori (g1 e g2) e restituisce un vector di due Percorsi (i due figli)
    void Mutazione_scambio(Percorso &p);
    void Mutazione_shift(Percorso &p);
    void Mutazione_permutazione(Percorso &p);
    void Mutazione_inversione(Percorso &p);
    void Evoluzione(const vector<Citta> &citta, ofstream &fileout); //ciclo evolutivo
    Percorso GetMigliore() const;
};

#endif // __AlgoritmoGenetico__

