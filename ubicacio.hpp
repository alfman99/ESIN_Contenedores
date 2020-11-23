#ifndef _UBICACIO_HPP
#define _UBICACIO_HPP 

#include <esin/error>
#include <esin/util>

class ubicacio {
public:

  /* Constructora. Crea la ubicació <i, j, k>. Genera un error si
    <i, j, k> no pertany a {<u, v, w> | u >= 0 ^ v >= 0 ^ w >= 0}
     o a {<-1, 0, 0>,  <-1,-1,-1>}. */
  ubicacio(int i, int j, int k) throw(error);

  /* Constructora per còpia, assignació i destructora. */
  ubicacio(const ubicacio& u) throw(error);
  ubicacio& operator=(const ubicacio& u) throw(error);
  ~ubicacio() throw();

  /* Consultors. Retornen respectivament el primer, segon i tercer
     component de la ubicació. */
  int filera() const throw();
  int placa() const throw();
  int pis() const throw();

  /* Operadors de comparació.
     L'operador d'igualtat retorna cert si i només si les dues ubicacions
     tenen la mateixa filera, plaça i pis. L'operador menor retorna cert si
     i només si la filera del paràmetre implícit és més petit que la
     d'u, o si les dues fileres són iguals i la plaça del paràmetre
     implícit és més petita que la d'u, o si les fileres i les places
     coincideixen i el pis del paràmetre implícit és més petit que el d'u.
     La resta d'operadors es defineixen consistentment respecte <. */
  bool operator==(const ubicacio &u) const throw();
  bool operator!=(const ubicacio &u) const throw();
  bool operator<(const ubicacio &u) const throw();
  bool operator<=(const ubicacio &u) const throw();
  bool operator>(const ubicacio &u) const throw();
  bool operator>=(const ubicacio &u) const throw();

  /* Gestió d'errors */
  static const int UbicacioIncorrecta = 10;

private:
  #include "ubicacio.rep"
};
#endif
