#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "../../RANDOM/random.h"
#include "città.h"
#include "ga.h"
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]){

int size, rank;

MPI_Init(&argc, &argv); //avvia MPI

MPI_Comm_size(MPI_COMM_WORLD, &size); //dice quanti siamo
MPI_Comm_rank(MPI_COMM_WORLD, &rank);  //dice chi sono io

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
            seed[0]+=rank; //in questo modo ogni rank avrà seed diversi e quindi evoluzioni diverse
            seed[1]+=rank;
            seed[2]+=rank;
            seed[3]+=rank;
            gen.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
/////////////////////////////////////
/////////////////////////////////////

//GA CON MIGRAZIONE
//leggo dati capoluoghi provincia
vector<Citta> citta;
ifstream filein("cap_prov_ita.dat");

double x,y;
while(filein>>x>>y){
   citta.push_back(Citta(x, y));
}
filein.close();

if(rank==0)
   cout<<"Numero citta: "<<citta.size()<<endl;


//setto parametri evolutivi
int N_individui=200;
int N_generazioni=2000;


//algoritmo genetico
AlgoritmoGenetico ga(N_individui,N_generazioni,gen);

//apro un fileout diverso per ciascun rank: per ciascun rank salvo: generazione - lunghezza best - media metà migliore
ofstream fileout;
fileout.open("fileout_rank_"+to_string(rank)+".dat");

//ciclo evolutivo
ga.Evoluzione(citta,fileout,rank,size,true); //true=esegui migrazione

fileout.close();

//raccolgo risultati
Percorso best=ga.GetMigliore();
double L1=best.GetL1();

vector<double> completo(size);

//ogni rank invia il proprio best al rank 0, che inserisce tutti gli L1 in un vettore ordinato per rank
MPI_Gather(&L1,               
         1,                   //ogni processo invia un elemento
         MPI_DOUBLE,          //tipo del dato inviato
         completo.data(),     //buffer di ricezione
         1,                   //ogni processo contribuisce con 1 elemento
         MPI_DOUBLE,          //tipo del dato ricevuto
         0,                   //rank del proceso che raccoglie tutti i dati
         MPI_COMM_WORLD);     //comunicatore che include tutti i processi

//analisi finale
int best_rank=0;

if(rank==0){ //solo il rank 0 possiede il vettore completo con tutti i valori di L1 ricevuti
   best_rank=min_element(completo.begin(),completo.end())-completo.begin(); //funzione della libreria standard che restituisce la posizione del valore minimo  nell'intervallo

   //stampa il rank migliore e il relativo valore di L1
   cout<<"Best rank = "<<best_rank<<" L1 = "<<completo[best_rank]<<endl;
}

//il rank 0 comunica a tutti gli altri rank quale è il migliore
MPI_Bcast(&best_rank,
         1,                   //numero di elementi da trasmettere
         MPI_INT,             //tipo del dato trasmesso
         0,                   //rank del processo sorgente
         MPI_COMM_WORLD);     //comunnicatore che include tutti i processi

//salvo coordinate del percorso migliore per ciascun rank
ofstream out("best_path_rank_"+to_string(rank)+".dat");

vector<int> p=best.GetPercorso();

for(int i:p){ //scorro per ogni indice i contenuto in p
    out<<citta[i].GetX()<<" "<<citta[i].GetY()<<"\n";
}

//chiusura ciclo
out<<citta[p[0]].GetX()<<" "<<citta[p[0]].GetY()<<"\n";

out.close();


/////////////////////////////////////////////
/////////////////////////////////////////////
//ESEGUE GA SENZA MIGRAZIONE, LAVORANDO COUNQUE IN PARALLELO

//arpo file output diverso per ogni rank
ofstream fileout_solo;
fileout_solo.open("fileout_solo_rank_"+to_string(rank)+".dat");

//creo GA indipendente per ogni rank
AlgoritmoGenetico ga_solo(N_individui,N_generazioni,gen);

//eseguo evolunzione senza migrazione (false)
ga_solo.Evoluzione(citta,fileout_solo,rank,size,false);
fileout_solo.close();

//salvo coordinate percorso migliore di ogni rank senza migrazione
Percorso best_solo=ga_solo.GetMigliore();
ofstream out_solo("best_solo_"+to_string(rank)+".dat");
vector<int> p_solo=best_solo.GetPercorso();
for(int i:p_solo){
    out_solo<<citta[i].GetX()<<" "<<citta[i].GetY()<<"\n";
}

out_solo<<citta[p_solo[0]].GetX()<<" "<<citta[p_solo[0]].GetY()<<"\n";
out_solo.close();

//chiudo MPI (tutti i processi lo chiamano)
MPI_Finalize();

return 0;
}