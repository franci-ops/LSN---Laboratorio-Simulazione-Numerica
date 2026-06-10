#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "../../RANDOM/random.h"

using namespace std;

double N(double x){

   return 0.5*(1+erf((x)/(sqrt(2.))));
}


double Call(double S0, double K, double T, double r, double sigma){
double d1;
double d2;

d1=(1./(sigma*sqrt(T)))*(log((S0)/(K))+(r+(sigma*sigma*T)/(2.)));
d2=d1-sigma*sqrt(T);

return S0*(N(d1))-K*exp(-r*T)*(N(d2));
}

double Put(double S0, double K, double T, double r, double sigma){
double d1;
double d2;

d1=(1./(sigma*sqrt(T)))*(log((S0)/(K))+(r+(sigma*sigma*T)/(2.)));
d2=d1-sigma*sqrt(T);

return S0*(N(d1)-1.)-K*exp(-r*T)*(N(d2)-1.);
}

double S(double gen, double r, double sigma, double t, double t0, double S){
double W=gen;

return S*exp((r-0.5*sigma*sigma)*(t-t0)+sigma*W*sqrt(t-t0));
}

int main(int argc, char *argv[]){

double t=0.;
double S0=100.;
double T=1.;
double K=100.;
double r=0.1;
double sigma=0.25;
double W;

int M=10000;
int N=100;
int L=int(M/N);

double somma1_C=0;
double somma1_P=0;
double media1_C=0;
double media1_P=0;
double media2_C=0;
double media2_P=0;
double somma_prog_C=0;
double somma2_prog_C=0;
double errore_prog_C=0;
double somma_prog_P=0;
double somma2_prog_P=0;
double errore_prog_P=0;

double S1;
double S_appo;

ofstream fileout_1("Call1.dat");
ofstream fileout_2("Put1.dat");
ofstream fileout_3("Call2.dat");
ofstream fileout_4("Put2.dat");

ofstream fileout_attesi("Attesi.dat");

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


//METODO 1

for(int k=0; k<N; k++){    //ciclo sui blocchi
somma1_C=0;
somma1_P=0;


   for(int i=0; i<L; i++){    //ciclo sui lanci per blocco
      W=gen.Gauss(0,1);
      S1=S(W,r,sigma,T,t,S0);
      somma1_C+=exp(-r*T)*max(0.,S1-K);
      somma1_P+=exp(-r*T)*max(0.,K-S1);
   }

//accumulo medie
media1_C+=somma1_C/L;
media1_P+=somma1_P/L;

//accumulo medie al quadrato
media2_C+=somma1_C*somma1_C/(L*L);
media2_P+=somma1_P*somma1_P/(L*L);

//somma progressiva
somma_prog_C=media1_C/(k+1);
somma2_prog_C=media2_C/(k+1);

somma_prog_P=media1_P/(k+1);
somma2_prog_P=media2_P/(k+1);


//errore progressivo
if(k==0){
errore_prog_C=0;
errore_prog_P=0;
}
else{
errore_prog_C=sqrt((somma2_prog_C-somma_prog_C*somma_prog_C)/(k));
errore_prog_P=sqrt((somma2_prog_P-somma_prog_P*somma_prog_P)/(k));
}


fileout_1<<(k+1)<<" "<<somma_prog_C<<"  "<<errore_prog_C<<endl;
fileout_2<<(k+1)<<" "<<somma_prog_P<<" "<<errore_prog_P<<endl;

}

///////////////////////////////////////////////////
//////////////////////////////////////////////////////

//METODO 2

media1_C=0;
media1_P=0;
media2_C=0;
media2_P=0;

for(int k=0; k<N; k++){ //ciclo sui blocchi
somma1_C=0;
somma1_P=0;



   for(int i=0; i<L; i++){ //ciclo sui lanci per blocco
      S_appo=S0;
      for(int j=0; j<100; j++){ //calcolo la funzione S ad ogni istante t_i
      W=gen.Gauss(0,1);
      S1=S(W,r,sigma,(j+1)*T/100.,j*T/100.,S_appo);             //j*T/100 mi dà il ti
      S_appo=S1;
      }

   somma1_C+=exp(-r*T)*max(0.,S1-K);
   somma1_P+=exp(-r*T)*max(0.,K-S1);

   }

//incremento medie e medie al quadrato
media1_C+=somma1_C/L;
media1_P+=somma1_P/L;
media2_C+=somma1_C*somma1_C/(L*L);
media2_P+=somma1_P*somma1_P/(L*L);


//somma progressiva
somma_prog_C=media1_C/(k+1);
somma2_prog_C=media2_C/(k+1);

somma_prog_P=media1_P/(k+1);
somma2_prog_P=media2_P/(k+1);


//errore progressivo
if(k==0){
errore_prog_C=0;
errore_prog_P=0;
}
else{
errore_prog_C=sqrt((somma2_prog_C-somma_prog_C*somma_prog_C)/(k));
errore_prog_P=sqrt((somma2_prog_P-somma_prog_P*somma_prog_P)/(k));
}


fileout_3<<(k+1)<<" "<<somma_prog_C<<"  "<<errore_prog_C<<endl;
fileout_4<<(k+1)<<" "<<somma_prog_P<<" "<<errore_prog_P<<endl;

}




fileout_attesi<<Call(S0,K,T,r,sigma)<<"   "<<Put(S0,K,T,r,sigma);
   return 0;
}
