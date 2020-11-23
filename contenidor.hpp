#ifndef _CONTENIDOR_HPP
#define _CONTENIDOR_HPP 

#include <string>
#include <esin/error>
#include <esin/util>

using std::string;
using util::nat;

class contenidor {
public:
  /* Constructora. Crea un contenidor amb matrícula m i longitud l.
     Es produeix un error si m no és una seqüència de un o més caràcters,
     formada exclusivament per lletres majúscules i dígits,
     o si l no pertany a {10, 20, 30} */
  contenidor(const string &m, nat l) throw(error);

  /* Constructora per còpia, assignació i destructora. */
  contenidor(const contenidor &u) throw(error);
  contenidor& operator=(const contenidor &u) throw(error);
  ~contenidor() throw();

  /* Consultors. Retornen respectivament la longitud i la matrícula del
     contenidor. */
  nat longitud() const throw();
  string matricula() const throw();

  /* Operadors de comparació. L'operador d'igualtat retorna cert si i
     només si els dos contenidors contenen la mateixa matrícula i longitud.
     L'operador menor retorna cert si i només si la matrícula del
     paràmetre implícit es més petit en ordre alfabètic que la c o si les
     dues matrícules són iguals i la longitud del paràmetre implícit és més
     petita que la de c. La resta d'operadors es defineixen consistentment
     respecte a <. */
  bool operator==(const contenidor &c) const throw();
  bool operator!=(const contenidor &c) const throw();
  bool operator<(const contenidor &c) const throw();
  bool operator<=(const contenidor &c) const throw();
  bool operator>(const contenidor &c) const throw();
  bool operator>=(const contenidor &c) const throw();

  /* Gestió d'errors */
  static const int MatriculaIncorrecta = 20;
  static const int LongitudIncorrecta = 21;

private:
  #include "contenidor.rep"
};
#endif
