#include <iostream>
#include <fstream>
#include <cmath>
#include "../../RANDOM/random.h"

using namespace std;

int main(int argc, char *argv[]){

int lanci=10000;
int blocchi=100;
int n=lanci/blocchi;

double num;
double medie=0;
double medie2=0;
double somma;

double var_sum=0;
double var_m=0;
double var_m2=0;

double somma_prog=0;
double somma2_prog=0;
double errore_prog;

double somma_prog_var=0;
double somma2_prog_var=0;
double errore_prog_var;

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

//per stampare i dati su file e poi graficarli con Python
ofstream fileout_med("Media.dat");
ofstream fileout_var("Errore.dat");

ofstream fileout_var_med("Media_var.dat");
ofstream fileout_var_var("Errore_var.dat");


for(int k=0; k<blocchi; k++){
somma=0;
var_sum=0;


   for(int i=0; i<n; i++){
      num=gen.Rannyu();
      somma=somma+num;
      var_sum=var_sum+(num-0.5)*(num-0.5);
   }

medie=medie+(somma/n);
medie2=medie2+(somma*somma)/(n*n);

somma_prog=medie/(k+1);
somma2_prog=medie2/(k+1);

var_m=var_m+(var_sum/n);  //sommo progressivamente le medie
var_m2=var_m2+(var_sum*var_sum)/(n*n);

somma_prog_var=var_m/(k+1); //riempio array medie progressive
somma2_prog_var=var_m2/(k+1);


if(k==0){
errore_prog=0;
errore_prog_var=0;
}else{
errore_prog=sqrt((somma2_prog-somma_prog*somma_prog)/(k));
errore_prog_var=sqrt((somma2_prog_var-somma_prog_var*somma_prog_var)/(k));
}

fileout_med<<somma_prog<<endl;
fileout_var<<errore_prog<<endl;

fileout_var_med<<somma_prog_var<<endl;
fileout_var_var<<errore_prog_var<<endl;


}


//////////////////////////////////////////////
///////////////////////////////////////////// PUNTO 3

int intervalli=100;
int n_lanci=10000;
int ripe=100;

int contatore[intervalli]={0};
int point;

int E=n_lanci/intervalli; //frequenza attesa in ciascun bin
double X;
double sum;

ofstream fileout_chi("X.dat");

//ciclo esterno per ripetere
for(int k=0; k<ripe; k++){ //ciclo sul numero di esecuzioni del test del chi2
sum=0;
   for(int l=0; l<intervalli; l++){ //azzero conteggi bin all'inizio di ogni esecuzione
      contatore[l]=0;
   }
//faccio i lanci e riempio il contatore
   for(int i=0; i<n_lanci; i++){
      num=gen.Rannyu();
      point=int(num*intervalli); //identifico il bin
      contatore[point]++;
   }

//calcolo il chi quadro
   for(int j=0; j<intervalli; j++){
      sum=sum+pow((contatore[j]-E),2);
   }
   X=sum/E;
   fileout_chi<<X<<endl;

}

   return 0;
}
