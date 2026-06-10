#include <iostream>
#include <fstream>
#include <cmath>
#include "../../RANDOM/random.h"

using namespace std;


int main(int argc, char *argv[]){

//lanci, blocchi, lanci per blocco
int M=10000;
int N=100;
int L=M/N;

int e1=0;
int e2=1;
double I;
double J;
double x;
double x_s;
double somma=0;
double sommaJ=0;
double media=0;
double media2=0;
double mediaJ=0;
double media2J=0;
double somma_prog=0;
double somma2_prog=0;
double errore_prog=0;
double sommaJ_prog=0;
double somma2J_prog=0;
double erroreJ_prog=0;


ofstream fileout_I1("Metodo1.dat");
ofstream fileout_I2("Metodo2.dat");


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


for(int j=0; j<N; j++){ //ciclo sui bloccih
somma=0;  //distribuzione uniforme
sommaJ=0;   //importance sampling


   
      for(int k=0; k<L; k++){   //ciclo sui lanci per ciascun blocco
         x=gen.Rannyu();
         x_s=1-sqrt(1-x);
         I=(e2-e1)*M_PI*0.5*cos(0.5*M_PI*x);                //distribuzione uniforme
         J=(e2-e1)*M_PI*0.5*cos(0.5*M_PI*x_s)/(-2*x_s+2);   //importnace sampling
         somma+=I;
         sommaJ+=J;
      }

media+=somma/(L); //incremento le medie
media2+=somma*somma/(L*L);  //incremento i quadrati delle medie

mediaJ+=sommaJ/(L);
media2J+=sommaJ*sommaJ/(L*L);



somma_prog=media/(j+1);  //somme progressive
somma2_prog=media2/(j+1);

//errore porgressivo
if(j==0){
errore_prog=0;
}else{
errore_prog=sqrt((somma2_prog-somma_prog*somma_prog)/(j));
}

sommaJ_prog=mediaJ/(j+1);
somma2J_prog=media2J/(j+1);

if(j==0){
erroreJ_prog=0;
}else{
erroreJ_prog=sqrt((somma2J_prog-sommaJ_prog*sommaJ_prog)/(j));
}

//stampo su file media ed errore progressivo
fileout_I1<<(j+1)<<" "<<somma_prog<<"  "<<errore_prog<<endl;
fileout_I2<<(j+1)<<" "<<sommaJ_prog<<" "<<erroreJ_prog<<endl;
}


   return 0;
}
