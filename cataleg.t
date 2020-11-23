#include "cataleg.hpp"

/* Constructora. Crea un catàleg buit on numelems és el nombre
    aproximat d'elements que com a màxim s'inseriran al catàleg. */
explicit cataleg<Valor>::cataleg(nat numelems) throw(error) {
    this->size = numelems;
    this->_root = NULL;
}

/* Constructora per còpia, assignació i destructora. */
cataleg<Valor>::cataleg(const cataleg& c) throw(error) {
    this->_size = c._size;
    this->_root = copy_nodes(c._root);
}

cataleg& cataleg<Valor>::operator=(const cataleg& c) throw(error) {
    if (this != &a) {
        NodeBST* aux;
        aux = copia_nodes(a._root);
        esborra_nodes(this->_root);
        this->_root = aux;
    }
    this->_size = c._size;
    return *this;
}

cataleg<Valor>::~cataleg() throw() {
    delete_nodes(this->_root);
}

/* Mètode modificador. Insereix el parell <clau, valor> indicat.
    En cas que la clau k ja existeixi en el catàleg actualitza el valor
    associat. Genera un error en cas que la clau sigui l'string buit. */
void cataleg<Valor>::assig(const string &k, const Valor &v) throw(error) {

}

/* Elimina del catàleg el parell que té com a clau k.
    En cas que la clau k no existeixi en el catàleg genera un error. */
void cataleg<Valor>::elimina(const string &k) throw(error) {

}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
    en cas contrari. */
bool cataleg<Valor>::existeix(const string &k) const throw() {

}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
    clau k llavors genera un error. Exemple:
    cataleg<int> ct;
    ...
    int n = ct["dia"]; */
Valor cataleg<Valor>::operator[](const string &k) const throw(error) {

}

/* Retorna el nombre d'elements que s'han inserit en el catàleg
    fins aquest moment. */
nat cataleg<Valor>::quants() const throw() {

}

cataleg<Valor>* cataleg<Valor>::copy_nodes(NodeBST* node) {
    NodeBST* aux;
    if (node == NULL) {
        aux = NULL;
    } 
    else {
        aux = new NodeBST;
        try {
            aux->key = node->key;
            aux->value = node->value;
            aux->left = copy_nodes(node->left);
            aux->right = copy_nodes(node->right);
        }
        catch(...) {
            delete aux;
            throw;
        }
    }
    return aux;
}

void cataleg<Valor>::delete_nodes(NodeBST* node) {
    if (node != NULL) {
        delete_nodes(node->left);
        delete_nodes(node->right);
        delete node;
    }
}

