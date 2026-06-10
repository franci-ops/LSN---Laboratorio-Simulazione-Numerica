#include <iostream>
#include <fstream>
#include <cmath>
#include "../../RANDOM/random.h"


using namespace std;


double errore(double media[], double media2[], int n){
   if(n==0)
      return 0.0;
   else
      return sqrt((media2[n]-media[n]*media[n])/n);
}




int main(int argc, char *argv[]){

int M=10000;
int N=100;
int lanci=M/N;

double L=0.7;
double d=1.0;

double sintheta,r,x,y;
double pos;
int contatore=0;
double PI;

double medie=0;
double medie2=0;
double somma_prog[N];
double somma2_prog[N];
double errore_prog[N];

ofstream fileout_PI("PI.dat");

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


//gli aghi atterreranno a una certa distanza da una riga e con un certo angolo, quindi dovrò estrarre due valori per determinare come cade l'ago
//devo capire se la porzione dell'ago in direzione perpendicolare alle righe interseca una riga
//per farlo uso le funzioni seno/coseno e l'angolo dell'inclinazione  ---> cerco di non usare pigreco, quindi di calcolare direttamente sentheta
//visto che la griglia è fatta da righe equispaziate, estraggo sempre la posizione dell'ago tra due stesse righe, ovvero tra [0 e d) (per comodità ho posto d=1 così uso Rannyu())


//ciclo sul nuemro di blocchi
for(int k=0; k<N; k++){
contatore=0;

   for(int i=0; i<lanci; i++){

   pos=gen.Rannyu();    //estraggo posizione estremo sx ago

   do{                  //genero x e y finchè la distanza punto-origine non è all'interno del quarto di circonferenza
   x=gen.Rannyu();      //in questo modo, estraendo x e y tra 0 e 1, sto estraendo dei punti in un quadrato
   y=gen.Rannyu();
   }while(x*x+y*y>1.0);

   r=sqrt(x*x+y*y);     //calcolo la distanza punto-origine
   sintheta=y/r;        //calcolo sen theta (in questo modo non ho usato M_PI per ricavare theta)

   if(pos+L*sintheta>=d){     
      contatore++;
   }

   }

PI=double((2.0*L*lanci)/(contatore*d));


medie+=PI;
medie2+=PI*PI;

somma_prog[k]=medie/(k+1);
somma2_prog[k]=medie2/(k+1);
errore_prog[k]=errore(somma_prog,somma2_prog,k);

fileout_PI<<somma_prog[k]<<" "<<errore_prog[k]<<"  "<<k+1<<endl;
}

   return 0;
}
