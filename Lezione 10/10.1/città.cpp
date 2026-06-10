
#include <iostream>
#include <math.h>
#include "città.h"

using namespace std;

//costruttori
Citta::Citta() {
   m_x=0.;
   m_y=0.;
}

Citta::Citta(double x, double y){
   m_x=x;
   m_y=y;
}

//leggi coordinata x
double Citta::GetX() const {
   return m_x;
}

//leggi coordinata y
double Citta::GetY() const {
   return m_y;
}