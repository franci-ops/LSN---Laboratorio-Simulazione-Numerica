#ifndef __Popolazione__
#define __Popolazione__

#include "../../RANDOM/random.h"
#include <vector>
#include "percorso.h"
#include <algorithm>

using namespace std;

class Popolazione {

private:
  vector<Percorso> m_popolazione; //vector di Percorsi che contiene tutti gli individui

public:
  Popolazione(); //costruttore
  void Genera_popolazione(int individui, int citta, Random &gen);
  void Stampa() const;
  void Ordina(const vector<Citta> &citta);
  Percorso GetPercorso(int k) const;
  void SetPopolazione(const vector<Percorso> &popolazione);
  void SetPercorso(int i, const Percorso &p);
};

#endif // __Popolazione__

