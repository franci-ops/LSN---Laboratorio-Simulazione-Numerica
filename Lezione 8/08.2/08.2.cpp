#include <iostream>
#include <fstream>
#include <cmath>
#include "../../RANDOM/random.h"

using namespace std;

double psi(double x, double mu, double sigma);
double V(double x);
double psi_seconda(double x, double mu, double sigma);
double energia_locale(double x, double mu, double sigma);
bool Metropolis_VMC(double &xold, double mu, double sigma, double delta, Random &gen);
double errore(double media, double media2, int N);
double H(double mu, double sigma, Random &gen, int N, int L, double &errore, double &x, double delta); //implemento una funzione per stimare il valor medio di H, senza stare a dover fare il data blocking nel main
double calcolo_delta(double mu, double sigma, Random &gen, double &x);

int main(int argc, char *argv[]){

// double mu=1.1;
// double sigma=0.5;
double mu=2.2;
double sigma=1.0;
double mu_new, sigma_new;

double T=1.0; //scelgo la temperatura iniziale
double coeff=0.95; //scelgo il tasso di raffreddamento 0.95
double delta_mu=0.1;  //scelgo i passi di campionamento per sigma e mu
double delta_sigma=0.1;

int M=100000;  //passi
int N=100;   //blocchi
int L=int(M/N); //lanci per blocco

double energia_old, energia_new;
double err_old, err_new;
double delta_E;
double x=0;
double delta;

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
ofstream fileout_ott("ottimizzazione.dat");
ofstream fileout_parametri("parametri_ottimali.dat");

//PRIMA PARTE: OTTIMIZZAZIONE DEI PARAMETRI MU E SIGMA
int conta=0;

//ciclo esterno: passo del simulated annealing
for(int k=0; k<150; k++){ 

   //SELEZIONE NUMERO PASSI IN FUNZIONE TEMPERATURA
   int appo=(int)(1.0/T);
   appo=max(5, min(30, appo));  //scelgo il numero di passi di SA in base alla temperatura
   //faccio in modo che il numero di passi non sia esagerato, imponendo come limite massimo 30 step per ciascuna temperatura

   cout << "Temperatura corrente: " << T << " | Numero passi SA: " << appo << endl;

   for(int t=0; t<appo; t++){ 
   //calcolo il delta ottimale
   delta=calcolo_delta(mu,sigma,gen,x);

   //calcolo energia_old coi parametri old
   energia_old=H(mu,sigma,gen,N,L,err_old,x,delta);

   //propongo dei nuovi sigma e mu
   mu_new=mu+gen.Rannyu(-delta_mu,delta_mu);

   //evito sigma negative (non fisiche)
   do{
   sigma_new=sigma+gen.Rannyu(-delta_sigma,delta_sigma);
   }while(sigma_new<0);

   //calcolo la nuova energia usando i nuovi sigma e mu
   energia_new=H(mu_new,sigma_new,gen,N,L,err_new,x,delta);

   delta_E=energia_new-energia_old;

   if(energia_new<energia_old){
         mu=mu_new;
         sigma=sigma_new;
         energia_old=energia_new;
         err_old=err_new;
   }else{
   double A=exp(-delta_E/T);

      if(gen.Rannyu()<A){
         mu=mu_new;
         sigma=sigma_new;
         energia_old=energia_new;
         err_old=err_new;
      }
   }

   conta++;
   //stampo i risultati
   fileout_ott<<conta<<" "<<T<<" "<< mu<<" "<<sigma<<" "<<energia_old<<" "<<err_old<<endl; //k+1
   }
   T=T*coeff; //raffreddo il sistema
}

//SECONDA PARTE: DATA BLOCKING FATTO SUI PARAMETRI OTTIMALI DI MU E SIGMA

//eseguo la stima finale coi parametri ottimali che ho trovato nell'ultima simulazione
cout<<"Parametri ottimali: mu="<<mu<<" sigma="<<sigma<<endl;
fileout_parametri<<mu<<"   "<<sigma;

ofstream fileout_finale("energia_finale.dat");   
ofstream fileout_psi("psi2.dat");             

double media=0;
double media2=0;
double somma_prog=0;
double somma2_prog=0;
double errore_prog=0;

delta=calcolo_delta(mu,sigma,gen,x);  //ha già la riequilibrazione di 1000 passi almeno
cout<<endl<<"Delta finale:"<<delta<<endl;


for(int k=0; k<N; k++){ //ciclo sui blocchi
    double somma=0;

    for(int j=0; j<L; j++){ //ciclo sui passi per blocco
        Metropolis_VMC(x,mu,sigma,delta,gen);
        somma+=energia_locale(x,mu,sigma);
        fileout_psi<<x<<endl;   // salva ogni posizione campionata per costruire l'istogramma finale
    }

    media+=somma/L;
    media2+=somma*somma/(L*L);

    somma_prog=media/(k+1);
    somma2_prog=media2/(k+1);

    errore_prog=errore(somma_prog,somma2_prog,k);

    fileout_finale<<k+1<<" "<<somma_prog<<" "<<errore_prog<<endl;
}

   return 0;
}



//funzione trial
double psi(double x, double mu, double sigma) {
    return exp(-pow(x-mu,2)/(2*sigma*sigma))+exp(-pow(x+mu,2)/(2*sigma*sigma));
}


//potenziale
double V(double x) {
    return pow(x,4)-2.5*pow(x,2);
}

//derivata seconda
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
bool Metropolis_VMC(double &xold, double mu, double sigma, double delta, Random &gen){    //metropolis restituisce un valore booleano: true se accetta la mossa, false se la rifiuta
// passo la reference a xold così quando nell'algoritmo pongo xold=xnew effettivamente modifico xold

double xnew, pold, pnew, A;

//propongo una mossa (ovvero uno spostamento a sx o a dx di un valore compreso tra -Delta e Delta, campionato uniformemente)
xnew=xold+gen.Rannyu(-delta,delta); //in questo modo lo spostamento può essere in una direzione o nell'altra in quanto estraggo lo spostamento tra -0.5 e 0.5

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


//questa funzione restituisce il valor medio dell'energia, calcolato tramite metodo a blocchi. In particolare, restituisce tramite reference anche errore e posizione x
double H(double mu, double sigma, Random &gen, int N, int L, double &err, double &x, double delta){
double media=0, media2=0;
double somma_prog, somma2_prog;
bool val;

//implemento il data blocking
for(int k=0; k<N; k++){
   double somma=0;

   for(int l=0; l<L; l++){
      val=Metropolis_VMC(x,mu,sigma,delta,gen);
      somma+=energia_locale(x,mu,sigma);
   }

media+=somma/L;
media2+=somma*somma/(L*L);

somma_prog=media/(k+1);
somma2_prog=media2/(k+1);
err=errore(somma_prog,somma2_prog,k); //errore viene restituito tramite reference

}

return somma_prog;
}


double calcolo_delta(double mu, double sigma, Random &gen, double &x){  //funge anche da riequilibrazione
   //x viene aggiornata: la catena continua da dove era
   double delta=1.0;
   double acc=0;
   int n_acc=0;
   int N=0;


do{    
      //ripeto più volte la misura dell'acceptance per stabilizzarne il valore e avere una statistica sufficiente di dati
      n_acc=0;
        for(int i=0; i<1000; i++){   //eseguo 1000 mosse e incremento il contatore se la accetto
            if(Metropolis_VMC(x,mu,sigma,delta,gen)==true){n_acc++;}
        }
        acc=double(n_acc/1000.);

        if(acc>0.5)delta*= 1.1;
        else delta*= 0.9;
      N++;

   }while((acc<0.4 or acc>0.6) and N<100); //al + 100000 steps per evitare bug

    return delta;
}