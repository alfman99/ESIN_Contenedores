#include "cataleg.hpp"
#include "cataleg.rep"

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
        aux = copy_nodes(a._root);
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
    /* falta el error si string esta vacio*/
    insert (this->_root , k, v);
}

/* Elimina del catàleg el parell que té com a clau k.
    En cas que la clau k no existeixi en el catàleg genera un error. */
void cataleg<Valor>::elimina(const string &k) throw(error) {

}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
    en cas contrari. */
bool cataleg<Valor>::existeix(const string &k) const throw() {
    return exists(this->_root,k);
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
    return _size;
}

template<typename Valor>
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

template<typename Valor>
void cataleg<Valor>::delete_nodes(NodeBST<Valor>* node) {
    if (node != NULL) {
        delete_nodes(node->left);
        delete_nodes(node->right);
        delete node;
    }
}

template<typename Valor>
bool cataleg<Valor>::exists (NodeBST<Valor>* node, const string &k) {
    bool b;
    if (node = NULL){
        b = false;
    }
    if (node->key == k){
        b = true;
    }
    if (node->key < k){
        b = exists(node->left, k);
    }
    if (node->key > k){
        b = exists(node->right, k);
    }
    return b;
}

template <typename Valor>
void cataleg<Valor>::insert (NodeBST<Valor>* node, const string &k, const Valor &v){
    NodeBST* father = NULL;
    NodeBST* actual = node;
    if (node == NULL){
        actual = new NodeBST;
        actual->value = v;
        actual->key = k;
    }
    else{ /*avancem fins a la fulla corresponent amb un punter indicant el antecesor*/
        while(node != NULL && node->key != K){
            father = node;
            if(k < node->key){
                node = node->left;
            }
            else{
                node = node->right;
            }
        }
    }
    /*si hem arriibat a una fulla añadirem fill dret o esquerre al antecessor*/
    if(node == NULL){
        if(k < father->key){
            NodeBST* aux = new NodeBST;
            aux->key = k;
            aux->value = v;
            father->left = aux;
        }
        else{
            NodeBST* aux = new NodeBST;
            aux->key = k;
            aux->value = v;
            father->right = aux;
        }
    }
    else{/*si ja existia un node amb aquesta clau actualitzem */
        node->value = v;
    }
}
