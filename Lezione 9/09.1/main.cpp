#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "../../RANDOM/random.h"
#include "città.h"
#include "ga.h"

using namespace std;

int main(){
    
int citta=34;
int individui=100;
int generazioni=500;

int pos;
double theta;

ofstream best("best.dat");

/////////////////////////////////////
/////////////////////////////////////
int seed[4];
int p1, p2;

Random gen;

ifstream Primes("../../RANDOM/Primes");

   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();


   ifstream input("../../RANDOM/seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            gen.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

/////////////////////////////////////
/////////////////////////////////////


//CASO 1: CITTA' SULLA CIRCONFERENZA
vector<Citta> citta_circonferenza;

//inizializzo le 34 città
for(int k=0; k<citta; k++){
    theta=gen.Rannyu(0,2*M_PI);
    citta_circonferenza.push_back(Citta(cos(theta),sin(theta)));
}

//salvo le coordinate delle città per il plot del percorso
ofstream fileout1("coordinate_circonferenza.dat");

//stampo su file le coordinate di tutte le città
for(int k=0; k<citta; k++){
fileout1<<k<<"  "<<citta_circonferenza[k].GetX()<<"  "<<citta_circonferenza[k].GetY()<<endl;
}
fileout1.close();

//stampo generazione - lunghezza migliore - media sulla metà migliore
ofstream fileout2("evoluzione_cerchio.dat");

//creo e faccio girare l'Algoritmo Genetico
AlgoritmoGenetico algoritmo_circonferenza(individui,generazioni,gen);
algoritmo_circonferenza.Evoluzione(citta_circonferenza,fileout2);
fileout2.close();

//salvo il percorso migliore
ofstream fileout3("best_circonferenza.dat");
Percorso best_circonferenza=algoritmo_circonferenza.GetMigliore();
vector<int> p_cerchio=best_circonferenza.GetPercorso();

//salvo le coordinate del percorso migliore
for(int k=0; k<p_cerchio.size(); k++){
    pos=p_cerchio[k]; //indice della città visitata
    fileout3<<citta_circonferenza[pos].GetX()<<"  "<<citta_circonferenza[pos].GetY()<<endl; //coordinate della città visitata
}

//chiudo il percorso tornando alla città iniziale
int pos0=p_cerchio[0];
fileout3<<citta_circonferenza[pos0].GetX()<<"  "<<citta_circonferenza[pos0].GetY()<<endl;
fileout3.close();

///////////////////////////////////
///////////////////////////////////

//CASO 2: CITTA' NEL QUADRATO
vector<Citta> citta_quadrato;
for(int k=0; k<citta; k++){
    citta_quadrato.push_back(Citta(gen.Rannyu(),gen.Rannyu()));
}

//salvo le coordinate delle città
ofstream fileout4("coordinate_quadrato.dat");
for(int k=0; k<citta; k++){
    fileout4<<k<<"  "<<citta_quadrato[k].GetX()<<"  "<<citta_quadrato[k].GetY()<<endl;
}
fileout4.close();

//salvo generazione - lunghezza best - media sulla metà migliore
ofstream fileout5("evoluzione_quadrato.dat");
AlgoritmoGenetico algoritmo_quadrato(individui,generazioni,gen);
algoritmo_quadrato.Evoluzione(citta_quadrato,fileout5);
fileout5.close();

//salvo coordinate percorso migliore
ofstream fileout6("best_quadrato.dat");
Percorso best_quadrato=algoritmo_quadrato.GetMigliore();
vector<int> p_quadrato=best_quadrato.GetPercorso();
for(int k=0; k<p_quadrato.size(); k++){
    pos=p_quadrato[k];
    fileout6<<citta_quadrato[pos].GetX()<<"  "<<citta_quadrato[pos].GetY()<<endl;
}
int pos0q=p_quadrato[0];
fileout6<<citta_quadrato[pos0q].GetX()<<"  "<<citta_quadrato[pos0q].GetY()<<endl;
fileout6.close();

//stampo a video lunghezza migliore per quadrato e circonferenza
cout<<"Circonferenza - Lunghezza migliore: "<<best_circonferenza.GetL1()<<endl;
best<<best_circonferenza.GetL1()<<endl;
cout<<"Quadrato - Lunghezza migliore: "<<best_quadrato.GetL1()<<endl;
best<<best_quadrato.GetL1()<<endl;

return 0;
}