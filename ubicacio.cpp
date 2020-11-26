#include "ubicacio.hpp"
#include "ubicacio.rep"

ubicacio::ubicacio(int i, int j, int k) throw(error) {
    if (es_error(i, j, k)) {
        throw error(UbicacioIncorrecta);
    }
    else{//si no hay error guardamos 
    this->fila = i;
    this->plaza = j;
    this->piso = k;        
    }
}

/* Constructora per còpia, assignació i destructora. */
ubicacio::ubicacio(const ubicacio& u) throw(error) {
    this->fila = u.fila;
    this->plaza = u.plaza;
    this->piso = u.piso;
}

ubicacio& ubicacio::operator=(const ubicacio& u) throw(error) {
    if (this != &u) {
        this->fila = u.fila;
        this->plaza = u.plaza;
        this->piso = u.piso;
    }
    return *this;
}

ubicacio::~ubicacio() throw() {}

/* Consultors. Retornen respectivament el primer, segon i tercer
   component de la ubicació. */
int ubicacio::filera() const throw() {
    return this->fila;
}

int ubicacio::placa() const throw() {
    return this->plaza;
}

int ubicacio::pis() const throw() {
    return this->piso;
}

/* Operadors de comparació.
   L'operador d'igualtat retorna cert si i només si les dues ubicacions
   tenen la mateixa filera, plaça i pis. L'operador menor retorna cert si
   i només si la filera del paràmetre implícit és més petit que la
   d'u, o si les dues fileres són iguals i la plaça del paràmetre
   implícit és més petita que la d'u, o si les fileres i les places
   coincideixen i el pis del paràmetre implícit és més petit que el d'u.
   La resta d'operadors es defineixen consistentment respecte <. */
bool ubicacio::operator == (const ubicacio & u) const throw() {
    return this->fila == u.fila && this->plaza == u.plaza && this->piso == u.piso;
}

bool ubicacio::operator != (const ubicacio & u) const throw() {
    return this->fila != u.fila || this->plaza != u.plaza || this->piso != u.piso;
}

bool ubicacio::operator < (const ubicacio & u) const throw() {
    if (this->fila < u.fila) return true;
    else if (this->fila == u.fila) {
        if (this->plaza < u.plaza) return true;
        else if (this->plaza == u.plaza) {
            if (this->piso < u.piso) return true;
            else return false;
        }
        else return false;
    }
    else return false;
}

bool ubicacio::operator <= (const ubicacio & u) const throw() {
    return *this < u || *this == u;
}

bool ubicacio::operator > (const ubicacio & u) const throw() {
    if (this->fila > u.fila) return true;
    else if (this->fila == u.fila) {
        if (this->plaza > u.plaza) return true;
        else if (this->plaza == u.plaza) {
            if (this->piso > u.piso) return true;
            else return false;
        }
        else return false;
    }
    else return false;
}

bool ubicacio::operator >= (const ubicacio & u) const throw() {
    return *this > u || *this == u;
}

bool ubicacio::es_error(int i, int j, int k) {
    bool b;
    if ((i >= 0 && j >= 0 && k >= 0) || (i == -1 && j == 0 && k == 0) || (i == -1 && j == -1 && k == -1)) {
        b = false;
    }
    else {
        b = true;
    }
    return b;
}
