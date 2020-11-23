#include "cataleg.hpp"

/* Constructora. Crea un catàleg buit on numelems és el nombre
    aproximat d'elements que com a màxim s'inseriran al catàleg. */
explicit cataleg::cataleg(nat numelems) throw(error) {

}

/* Constructora per còpia, assignació i destructora. */
cataleg::cataleg(const cataleg& c) throw(error) {

}

cataleg& cataleg::operator=(const cataleg& c) throw(error) {

}

cataleg::~cataleg() throw() {}

/* Mètode modificador. Insereix el parell <clau, valor> indicat.
    En cas que la clau k ja existeixi en el catàleg actualitza el valor
    associat. Genera un error en cas que la clau sigui l'string buit. */
void cataleg::assig(const string &k, const Valor &v) throw(error) {

}

/* Elimina del catàleg el parell que té com a clau k.
    En cas que la clau k no existeixi en el catàleg genera un error. */
void cataleg::elimina(const string &k) throw(error) {

}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
    en cas contrari. */
bool cataleg::existeix(const string &k) const throw() {

}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
    clau k llavors genera un error. Exemple:
    cataleg<int> ct;
    ...
    int n = ct["dia"]; */
Valor cataleg::operator[](const string &k) const throw(error) {

}

/* Retorna el nombre d'elements que s'han inserit en el catàleg
    fins aquest moment. */
nat cataleg::quants() const throw() {

}
