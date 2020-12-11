#include "cataleg.hpp"

#include <esin/util>

/* Constructora. Crea un catàleg buit on numelems és el nombre
    aproximat d'elements que com a màxim s'inseriran al catàleg. */
template<typename Valor>
cataleg<Valor>::cataleg(nat numelems) throw(error) : _quants(0) {
    this->_M = nextPrime(numelems);
    this->_taula = new node_hash*[this->_M];
    this->_quants = 0;
    for (unsigned int i = 0; i < this->_M; i++){
        this->_taula[i] = NULL;
    }
    std::cout << this->_M << std::endl;
}

/* Constructora per còpia, assignació i destructora. */
template<typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error) {

    this->_taula = new node_hash*[c._M];

    for(unsigned int i = 0; i < _M; i++) {
        if (c._taula[i] != NULL) {
            node_hash* aux = c._taula[i];
            while (aux != NULL) {
                try {
                    node_hash* newNode = new node_hash(aux->_k, aux->_v, this->_taula[i]);
                    this->_taula[i] = newNode;
                    aux = aux->_seg;
                }
                catch(error) {
                    delete this;
                }
                
            }
        }
        else {
            this->_taula[i] = NULL;
        }
    }

    this->_quants = c._quants;
    this->_M = c._M;

}

template<typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg& c) throw(error) {
    if(this != &c){
        cataleg<Valor> aux(c); 
    }
    return *this;
}

template<typename Valor>
cataleg<Valor>::~cataleg() throw() {
    node_hash* actual;
    node_hash* seguent;
    this->_quants = 0;
    for(unsigned int i = 0; i < this->_M ; i++ ){
        actual = _taula[i];
        while(actual != NULL){
            seguent = actual->_seg;
            delete actual;
            actual = seguent;
        }
    }
    delete[] this->_taula;
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
        util::Hash<string> h;
        int i = h(k)%this->_M;

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
            this->_taula[i] = new node_hash(k, v, this->_taula[i]);
            this->_quants++;
        }
    }
}

/* Elimina del catàleg el parell que té com a clau k.
    En cas que la clau k no existeixi en el catàleg genera un error. */
template<typename Valor>
void cataleg<Valor>::elimina(const string &k) throw(error) {
    if (k.empty()) {
        throw error(ClauStringBuit);
    }
    if (this->_quants == 0) {
        throw error(ClauInexistent);
    }
    else {
        
        util::Hash<string> h;
        int i = h(k)%this->_M;

        node_hash *p = this->_taula[i], *ant = NULL;

        bool trobat = false;

        while (p != NULL && !trobat) {
            if (p->_k == k) {
                trobat = true;
            }
            else {
                ant = p;
                p = p->_seg;
            }
        }

        if (trobat) {
            if (ant == NULL) {
                this->_taula[i] = p->_seg;
            }
            else {
                ant->_seg = p->_seg;
            }
            delete p;
            this->_quants--;
        }
    }
}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
    en cas contrari. */
template<typename Valor>
bool cataleg<Valor>::existeix(const string &k) const throw() {
    if (k.empty()) {
        throw error(ClauStringBuit);
    }
    else if (this->_quants == 0) {
        return false;
    }
    else {
        
        util::Hash<string> h;
        int i = h(k)%this->_M;

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

        return trobat;
    }
    return false;
}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
    clau k llavors genera un error. Exemple:
    cataleg<int> ct;
    ...
    int n = ct["dia"]; */
template<typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const throw(error) {

    if (this->_quants == 0) {
        throw error(ClauInexistent);
    }

    Valor returnVal;

    util::Hash<string> h;
    int i = h(k)%this->_M;

    node_hash* p = this->_taula[i];
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
    return this->_quants;
}

template<typename Valor>
bool cataleg<Valor>::esPrimo(unsigned int numero) throw() {
    if (numero == 0 || numero == 1) {
        return false;
    }
    else {
        for(unsigned int i = 2; i <= numero / 2; i++) {
            if (numero % i == 0) {
                return false;
            }
        }
    }
    return true;
}

template<typename Valor>
nat cataleg<Valor>::nextPrime(nat num) throw() {
    nat aux = num;
    while (!esPrimo(aux)) {
        if (aux % 2 == 0) {
            aux++;
        }
        else {
            aux += 2;
        }
    }
    return aux;
}
