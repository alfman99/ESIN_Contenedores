#ifndef _CATALEG_HPP
#define _CATALEG_HPP 

#include <string>
#include <esin/error>
#include <esin/util>

using std::string;
using util::nat;

template <typename Valor>
class cataleg {
public:

  /* Constructora. Crea un catàleg buit on numelems és el nombre
     aproximat d'elements que com a màxim s'inseriran al catàleg. */
  explicit cataleg(nat numelems) throw(error);

  /* Constructora per còpia, assignació i destructora. */
  cataleg(const cataleg& c) throw(error);
  cataleg& operator=(const cataleg& c) throw(error);
  ~cataleg() throw();

  /* Mètode modificador. Insereix el parell <clau, valor> indicat.
     En cas que la clau k ja existeixi en el catàleg actualitza el valor
     associat. Genera un error en cas que la clau sigui l'string buit. */
  void assig(const string &k, const Valor &v) throw(error);

  /* Elimina del catàleg el parell que té com a clau k.
     En cas que la clau k no existeixi en el catàleg genera un error. */
  void elimina(const string &k) throw(error);

  /* Retorna true si i només si la clau k existeix dins del catàleg; false
     en cas contrari. */
  bool existeix(const string &k) const throw();

  /* Retorna el valor associat a la clau k; si no existeix cap parell amb
     clau k llavors genera un error. Exemple:
       cataleg<int> ct;
       ...
       int n = ct["dia"]; */
  Valor operator[](const string &k) const throw(error);

  /* Retorna el nombre d'elements que s'han inserit en el catàleg
     fins aquest moment. */
  nat quants() const throw();

  /* Gestió d'errors */
  static const int ClauStringBuit = 30;
  static const int ClauInexistent = 31;

private:
  #include "cataleg.rep"
};
#include "cataleg.t"
#endif
