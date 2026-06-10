#ifndef __Percorso__
#define __Percorso__

#include "../../RANDOM/random.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "città.h"

using namespace std;

class Percorso {

private:
  vector<int> m_percorso; //vector che contiene gli indici delle città (ciascuna città è rappresentata da un indice)
  double m_L1; //lunghezza del percorso

public:
  Percorso(); //costruttore
  void SetPercorso(const vector<int> &percorso); //assegna un percorso: passo la reference e uso const così evito di fare copie inutili del percorso e allo stesso tempo sono sicura che non andrò a modificare il percorso
  vector<int> GetPercorso() const; //restituisce il percorso  
  double GetL1() const; //restituisce la norma 1, ovvero la lunghezza del percorso 
  bool Controlla_vincoli() const; //esegue un check dei vincoli
  double Calcola_L1(const vector<Citta> &luoghi); //prende in ingresso un vettore di città, legge il percorso e calcola la lunghezza totale
  void Stampa() const;
  bool operator<(const Percorso &p) const;
};

#endif // __Percorso__

