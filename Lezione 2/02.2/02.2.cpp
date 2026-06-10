#include <iostream>
#include <fstream>
#include <cmath>
#include "../../RANDOM/random.h"

using namespace std;


int main(int argc, char *argv[]){

int M=10000;   //n di RWs
int N=100;     //n di blocchi
int L=M/N;     //RWs per ogni blocco
int step=100;      //passi per ciascuna RW

double pos[3]={0,0,0};   //vettore che contiene le coordinate x,y,z della RW
double distanza2[step]={0};
double somma[step]={0};
double somma2[step]={0};

double val;
double r2;
double costheta;
double phi;


ofstream fileout_1("Metodo1.dat");
fileout_1<<0<<"   "<<0<<"  "<<0<<endl;    //questa riga mi dice che all'inizio parto dall'origine
ofstream fileout_2("Metodo2.dat");
fileout_2<<0<<"   "<<0<<"  "<<0<<endl;


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

for(int k=0; k<N; k++){       //ciclo sul numero di blocchi
   
   for(int m=0; m<step; m++){  //azzero il contatore della distanza
   distanza2[m]=0;
   }

   for(int i=0; i<L; i++){    //questo blocco contiene L=100 RWs 

   //azzero le coordinate iniziali all'inizio di ciasucna RW
   pos[0]=0;
   pos[1]=0;
   pos[2]=0;


      for(int j=0; j<step; j++){    //ciascuna RW è fatta da step=100 passi
         
      val=gen.Rannyu()*6;     //in questo modo genero un numero uniformemnte distribuito tra 0 e 5

      if(int(val)==0){pos[0]--;};
      if(int(val)==1){pos[0]++;};
      if(int(val)==2){pos[1]--;};
      if(int(val)==3){pos[1]++;};
      if(int(val)==4){pos[2]--;};
      if(int(val)==5){pos[2]++;};

      r2=(pos[0])*(pos[0])+(pos[1])*(pos[1])+(pos[2])*(pos[2]); //calcolo il quadrato della distanza dall'origine
      distanza2[j]+=r2;   //per ogni passo accumulo la distanza (al quadrato) a cui mi trovo dall'origine per ad ogni blocco
      
      }
   


   }

   
   for(int s=0; s<step; s++){ //accumulo le quantità che mi servono per l metodo a blocchi
      distanza2[s]=sqrt(distanza2[s]/L);  //calcolo la radice del <||^2> della distanza dall'origine
      somma[s]+=distanza2[s];
      somma2[s]+=distanza2[s]*distanza2[s];
   }

}


//per ogni passo fatto, devo claoclare la distanza media (su tutti i blocchi) a cui mi trovo rispetto all'origine + la rispettiva incertezza
for(int k=0; k<step; k++){
   fileout_1<<(k+1)<<"  "<<somma[k]/N<<"  "<<sqrt((somma2[k]/(N)-(somma[k]/N)*(somma[k]/N))/(N-1))<<endl;
   somma[k]=0;
   somma2[k]=0;
}


/////////////////////////////////////
//METODO 2: passo in direzione casuale nello spazio continuo


for(int k=0; k<N; k++){       //ciclo sul numero di blocchi
   
   for(int m=0; m<step; m++){
   distanza2[m]=0;
   }

   for(int i=0; i<L; i++){    //questo blocco contiene L=100 RWs 


   pos[0]=0;
   pos[1]=0;
   pos[2]=0;


      for(int j=0; j<step; j++){    //ciascuna RW è fatta da step=100 passi
         
      costheta=gen.Rannyu(-1,1);
      phi=gen.Rannyu(0,2*M_PI);

      pos[0]+=sqrt(1-costheta*costheta)*cos(phi); //x
      pos[1]+=sqrt(1-costheta*costheta)*sin(phi); //y
      pos[2]+=costheta;                           //z

      r2=(pos[0])*(pos[0])+(pos[1])*(pos[1])+(pos[2])*(pos[2]);
      distanza2[j]+=r2;
      
      }
   


   }

   
   for(int s=0; s<step; s++){
      distanza2[s]=sqrt(distanza2[s]/L);
      somma[s]+=distanza2[s];
      somma2[s]+=distanza2[s]*distanza2[s];
   }

}


for(int k=0; k<step; k++){
   fileout_2<<(k+1)<<"  "<<somma[k]/N<<"  "<<sqrt((somma2[k]/(N)-(somma[k]/N)*(somma[k]/N))/(N-1))<<endl;
}




   return 0;
}
