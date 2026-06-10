#ifndef __Citta__
#define __Citta__

using namespace std;

class Citta {

private:
  double m_x; //coordinate della cità
  double m_y;

public:
  Citta();  //costruttore
  Citta(double x, double y);  //costruttore
  
  double GetX() const;
  double GetY() const;

};

#endif // __Citta__

