#include <iostream>
#include "system.h"

using namespace std;

int main (int argc, char *argv[]){

  int nconf = 1;
  System SYS;



for(int k=0; k<16; k++){ //ciclo su 16 temperature diverse
  SYS.initialize(); //Inizializzo il sistema (posizioni, velocità, variabili interne)
  SYS.initialize_properties();  //Inizializzo proprietà termodinamiche/statistiche (energia,...)
  SYS.block_reset(0);  //resetto i dati del primo blocco
  SYS.settemperature(0.5+0.1*k); //imposto la temperatura da 0.5 a 1.9 con incremento 0.1

  for(int l=0; l<20000; l++){   //aggiungo passi "a vuoto" per riequilibrare il sistema ogni volta che riapre il file
  SYS.step();
  }


  for(int i=0; i < SYS.get_nbl(); i++){ //loop over blocks
    for(int j=0; j < SYS.get_nsteps(); j++){ //loop over steps in a block
      SYS.step();
      SYS.measure();
//      if(j%50 == 0){
//      SYS.write_XYZ(nconf); //Write actual configuration in XYZ format //Commented to avoid "filesystem full"! 
        //nconf++;
//      }
    }
    SYS.averages(i+1);
    SYS.block_reset(i+1);
  }

SYS.finalize();

}


  return 0;
}
