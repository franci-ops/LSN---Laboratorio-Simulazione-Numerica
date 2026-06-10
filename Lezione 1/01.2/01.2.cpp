#include <iostream>
#include <fstream>
#include <cmath>
#include "../../RANDOM/random.h"

using namespace std;

int main(int argc, char *argv[]){

int L=10000;
int N[4]={1,2,10,100};

double lambda=1.0;
double mu=0.0;
double gamma=1.0;

//vriabili per accumulare somme
double somma_dadi=0;
double somma_exp=0;
double somma_lor=0;

//file output
ofstream fileout_dadi("Dadi.dat");
ofstream fileout_exp("Esponenziale.dat");
ofstream fileout_lor("Lorentziana.dat");





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

//3 distribuzioni: dado standard (= distrib uniforme tra 1 e 6); esponenziale; lorentziana


   for(int k=0; k<L; k++){ //ciclo su L=10000 realizzazioni indipendenti

      for(int i=0; i<4; i++){  //plotto una riga per volta: i mi dice la colonna in cui plotto
      
      //azzero somme
      somma_dadi=0;
      somma_exp=0;
      somma_lor=0;
   

         //sommo N[i] variabili casuali
         for(int j=0; j<N[i]; j++){
            somma_dadi=somma_dadi+gen.Rannyu(1,6); //dado uniforme
            somma_exp+=gen.Exp(lambda);            //esponenziale
            somma_lor+=gen.Lorentz(mu, gamma);     //lorentziana
         }


      //salvo la media delle somme
      fileout_dadi<<(somma_dadi)/(N[i])<<"  ";
      fileout_exp<<(somma_exp)/(N[i])<<"  ";
      fileout_lor<<(somma_lor)/(N[i])<<" ";
      }

   fileout_dadi<<endl;
   fileout_exp<<endl;
   fileout_lor<<endl;

   }
   return 0;
}
