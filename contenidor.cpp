#include "contenidor.hpp"
#include "contenidor.rep"

/* Constructora. Crea un contenidor amb matrícula m i longitud l.
    Es produeix un error si m no és una seqüència de un o més caràcters,
    formada exclusivament per lletres majúscules i dígits,
    o si l no pertany a {10, 20, 30} */
contenidor::contenidor(const string &m, nat l) throw(error){
    if (l != 10 && l != 20 && l != 30) {
        throw error(LongitudIncorrecta);
    }
    else if (errorMatricula(m)) {
        throw error(MatriculaIncorrecta);
    }
    else{
        this->matri = m;
        this->longi = l;
    }
    
}

/* Constructora per còpia, assignació i destructora. */
contenidor::contenidor(const contenidor &u) throw(error){
    this->matri = u.matri;
    this->longi = u.longi;
}

contenidor& contenidor::operator=(const contenidor &u) throw(error){
    if(this != &u){
        this->matri = u.matri;
        this->longi = u.longi;
    }
    return *this;
}

contenidor::~contenidor() throw() {}

/* Consultors. Retornen respectivament la longitud i la matrícula del
    contenidor. */
nat contenidor::longitud() const throw(){
    return this->longi;
}

string contenidor::matricula() const throw(){
    return this->matri;
}

/* Operadors de comparació. L'operador d'igualtat retorna cert si i
    només si els dos contenidors contenen la mateixa matrícula i longitud.
    L'operador menor retorna cert si i només si la matrícula del
    paràmetre implícit es més petit en ordre alfabètic que la c o si les
    dues matrícules són iguals i la longitud del paràmetre implícit és més
    petita que la de c. La resta d'operadors es defineixen consistentment
    respecte a <. */

bool contenidor::operator==(const contenidor &c) const throw(){
    return this->matri == c.matri && this->longi == c.longi;
}

bool contenidor::operator!=(const contenidor &c) const throw(){
    return this->matri != c.matri || this->longi != c.longi;
}

bool contenidor::operator<(const contenidor &c) const throw(){
    if (this->matri < c.matri) {
        return true;
    } 
    else if (this->matri == c.matri) {
        if (this->longi < c.longi) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool contenidor::operator<=(const contenidor &c) const throw(){
    return *this < c || *this == c;
}

bool contenidor::operator>(const contenidor &c) const throw(){
    if (this->matri > c.matri) {
        return true;
    } 
    else if (this->matri == c.matri) {
        if (this->longi > c.longi) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool contenidor::operator>=(const contenidor &c) const throw(){
    return *this > c || *this == c;
}

bool contenidor::errorMatricula (const string &matricula) {
    if (matricula.size() <= 0) {
        return true;
    }
    for(int i = 0; i < matricula.size(); i++) {
        char letra = matricula[i];
        if (!((48 <= letra && letra <= 57) || (65 <= letra && letra <= 90))) {
            return true;
        }
    }
    return false;
}
