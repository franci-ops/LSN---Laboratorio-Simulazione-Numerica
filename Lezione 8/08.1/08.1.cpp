#include <iostream>
#include <fstream>
#include <cmath>
#include "../../RANDOM/random.h"

using namespace std;

double psi(double x, double mu, double sigma);
double V(double x);
double psi_seconda(double x, double mu, double sigma);
double energia_locale(double x, double mu, double sigma);
bool Metropolis(double &xold, double mu, double sigma, double delta, Random &gen);
double errore(double media, double media2, int N);

int main(int argc, char *argv[]){

double x=0.;
double mu=1.1;
double sigma=0.5;
double delta=1.02; //da variare all'inizio per trovare acceptance
// double delta=2.5;
int M=10000;  //passi
int N=100;   //blocchi
int L=int(M/N); //lanci per blocco
double somma=0;
double media=0, media2=0;
double somma_prog=0;
double somma2_prog=0;
double errore_prog=0;
double E; //energia
double acc=0;
bool val=false;

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

ofstream fileout("energia.dat");
ofstream fileout_acc("acceptance.dat");

//ciclo per calcolare l'acceptance
for(int l=0; l<1000; l++){
val=Metropolis(x,mu,sigma,delta,gen);

if(val==true){
   acc++;
}
}

cout<<endl<<"ACCEPTANCE: "<<acc/1000<<endl;

//eseguo un ciclo di riequilibrazione del sistema
for(int m=0; m<2000; m++){
Metropolis(x,mu,sigma,delta,gen); //passi a vuoto
}


//ciclo sul numero di blocchi
for(int k=0; k<N; k++){
somma=0;
acc=0;

   for(int i=0; i<L; i++){
   val=Metropolis(x,mu,sigma,delta,gen);
   if(val==true){
      acc++;
   }
   E=energia_locale(x,mu,sigma);
   somma+=E;
   }

fileout_acc<<1.*acc/L<<endl; //stampo valor medio dell'acceptance


media+=somma/L;
media2+=somma*somma/(L*L);

somma_prog=media/(k+1);
somma2_prog=media2/(k+1);
errore_prog=errore(somma_prog,somma2_prog,k);

fileout<<somma_prog<<"  "<<errore_prog<<endl;

}

   return 0;
}



//implemento la psi trial
double psi(double x, double mu, double sigma) {
    return exp(-pow(x-mu,2)/(2*sigma*sigma))+exp(-pow(x+mu,2)/(2*sigma*sigma));
}


//potenziale
double V(double x) {
    return pow(x,4)-2.5*pow(x,2);
}

//implemento derivata seconda di psi (mi serve per l'energia)
double psi_seconda(double x, double mu, double sigma) {

    double g1=exp(-pow(x-mu,2)/(2*sigma*sigma));
    double g2=exp(-pow(x+mu,2)/(2*sigma*sigma));

    double g1_seconda=g1*( pow(x-mu,2)/(sigma*sigma*sigma*sigma)-1.0/(sigma*sigma));
    double g2_seconda=g2*( pow(x+mu,2)/(sigma*sigma*sigma*sigma)-1.0/(sigma*sigma));
   return g1_seconda+g2_seconda;
}


double energia_locale(double x, double mu, double sigma) {
    return -0.5*psi_seconda(x,mu,sigma)/psi(x,mu,sigma)+V(x);
}


//implemento Metropolis per campionare il modulo quadro della funzione d'onda
bool Metropolis(double &xold, double mu, double sigma, double delta, Random &gen){    //metropolis restituisce un valore booleano: true se accetta la mossa, false se la rifiuta
                                 // passo la reference a xold così quando nell'algoritmo pongo xold=xnew effettivamente modifico xold
double xnew, pold, pnew, A;

//propongo una mossa (ovvero uno spostamento a sx o a dx di un valore compreso tra -Delta e Delta, campionato uniformemente)
xnew=xold+gen.Rannyu(-delta,delta); //in questo modo lo spostamento può essere in una direzione o nell'altra

//calcolo la probabilità associata a xold e a xnew
pold=psi(xold,mu,sigma)*psi(xold,mu,sigma);
pnew=psi(xnew,mu,sigma)*psi(xnew,mu,sigma);

//calcolo il rapporto di accettanza
A=min(1.,1.*(pnew)/(pold));

//implemento la regola di accettazione: estraggo un numero distribuito uniformemente tra 0 e 1: se è <A, accetto la mossa, altrimenti continuo ad estrarre finchè la mossa non è accettata
if(gen.Rannyu()<A){
   xold=xnew;
   return true;
}else{
   return false;
}

}


double errore(double media, double media2, int N){
   if(N==0){return 0;}
   else{
   return sqrt((media2-media*media)/(N));  
   }
}
