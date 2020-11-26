#include "cataleg.hpp"

#include <esin/util>

/* Constructora. Crea un catàleg buit on numelems és el nombre
    aproximat d'elements que com a màxim s'inseriran al catàleg. */
template<typename Valor>
cataleg<Valor>::cataleg(nat numelems) throw(error) {
    this->_M = numelems;

}

/* Constructora per còpia, assignació i destructora. */
template<typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error) {

}

template<typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg& c) throw(error) {
    
}

template<typename Valor>
cataleg<Valor>::~cataleg() throw() {

}

/* Mètode modificador. Insereix el parell <clau, valor> indicat.
    En cas que la clau k ja existeixi en el catàleg actualitza el valor
    associat. Genera un error en cas que la clau sigui l'string buit. */
template<typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) throw(error) {
    if (k.empty()) {
        throw error(ClauStringBuit);
    }
    else {
        util::Hash<string> hashFunction;
        unsigned long i = hashFunction(k);

        node_hash *p = this->_taula[i];

        bool trobat = false;

        while (p != NULL && !trobat) {
            if (p->_k == k) {
                trobat = true;
            }
            else {
                p = p->_seg;
            }
        }

        if (trobat) {
            p->_v = v;
        }
        else {
            node_hash* aux = new node_hash;
            aux->_k = k;
            aux->_v = v;
            aux->_seg = this->_taula[i];
            this->_taula[i] = aux;
            this->_quants;
        }
    }
}

/* Elimina del catàleg el parell que té com a clau k.
    En cas que la clau k no existeixi en el catàleg genera un error. */
template<typename Valor>
void cataleg<Valor>::elimina(const string &k) throw(error) {

}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
    en cas contrari. */
template<typename Valor>
bool cataleg<Valor>::existeix(const string &k) const throw() {

}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
    clau k llavors genera un error. Exemple:
    cataleg<int> ct;
    ...
    int n = ct["dia"]; */
template<typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const throw(error) {

    Valor returnVal;

    util::Hash<string> hashFunction;
    unsigned long i = hashFunction(k);
    node_hash* p = _taula[i];
    bool trobat = false;
    while (p != NULL && !trobat) {
        if (p->_k == k) {
            trobat = true;
            returnVal = p->_v;
        }
        else {
            p = p->_seg;
        }
    }

    if (trobat) {
        return returnVal;
    }
    else {
        throw error(ClauInexistent);
    }  
    
}

/* Retorna el nombre d'elements que s'han inserit en el catàleg
    fins aquest moment. */
template<typename Valor>
nat cataleg<Valor>::quants() const throw() {

}