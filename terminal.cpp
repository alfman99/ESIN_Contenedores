#include "terminal.hpp"
#include "terminal.rep"

const ubicacio ubicacioError(-1, -1, -1);
const ubicacio ubicacioEspera(-1, 0, 0);

terminal::terminal(nat n, nat m, nat h, estrategia st) throw(error) {
   if (n == 0) {
      throw error(NumFileresIncorr);
   } 
   else if (m == 0) {
      throw error(NumPlacesIncorr);
   } 
   else if (h == 0 || h > HMAX) {
      throw error(AlcadaMaxIncorr);
   }
   else if (st != FIRST_FIT && st != LLIURE) {
      throw error(EstrategiaIncorr);
   }
   else {
      this->fileres = n;
      this->places = m;
      this->pisos = h;
      estrategia_usada = st;
   }

   elementos = new cataleg< std::pair<contenidor,ubicacio> >(this->fileres * this->places * this->pisos + 50);

   // inicializamos principalStorage a strings vacias
   principalStorage = new string**[this->fileres];
   for (int i = 0; i < this->fileres; i++) {
      principalStorage[i] = new string*[this->places];
      for (int j = 0; j < this->places; j++) {
         principalStorage[i][j] = new string[this->pisos];
         for (int k = 0; k < this->pisos; k++) {
            principalStorage[i][j][k] = ""; 
         }
      }
   }

   // inicializamos estadoPrincipal a 0
   estadoPrincipal = new int*[this->fileres];
   for (int i = 0; i < this->fileres; i++) {
      estadoPrincipal[i] = new int[this->places];
      for (int j = 0; j < this->places; j++) {
         estadoPrincipal[i][j] = 0; 
      }
   }

}

terminal::terminal(const terminal& b) throw(error) {
   *this = b;
}

terminal& terminal::operator=(const terminal& b) throw(error){
   if (this != &b) {
      this->fileres = b.fileres;
      this->places = b.places;
      this->pisos = b.pisos;
      this->moviments_grua = b.moviments_grua;
      this->estrategia_usada = b.estrategia_usada;

      this->principalStorage = new string**[this->fileres];
      for (int i = 0; i < this->fileres; i++) {
         this->principalStorage[i] = new string*[this->places];
         for (int j = 0; j < this->places; j++) {
            this->principalStorage[i][j] = new string[this->pisos];
            for (int k = 0; k < this->pisos; k++) {
               this->principalStorage[i][j][k] = b.principalStorage[i][j][k];
            }
         }
      }

      this->estadoPrincipal = new int*[this->fileres];
      for (int i = 0; i < this->fileres; i++) {
         this->estadoPrincipal[i] = new int[this->places];
         for (int j = 0; i < this->places; j++) {
            this->estadoPrincipal[i][j] = b.estadoPrincipal[i][j];
         }
      }

      this->waitingStorage = b.waitingStorage;

      this->elementos = b.elementos;

   }
   return *this;
}

terminal::~terminal() throw() {

   delete this->elementos;

   for (int i = 0; i < this->fileres; i++) {
      for (int j = 0; j < this->places; j++) {
         delete [] principalStorage[i][j];
      }
      delete [] principalStorage[i];
   }
   delete principalStorage;

   for (int i = 0; i < this->fileres; i++) {
      delete [] estadoPrincipal[i];
   }
   delete estadoPrincipal;

}

void terminal::insereix_contenidor(const contenidor &c) throw(error){
   if (this->elementos->existeix(c.matricula())) {
      throw error(MatriculaDuplicada);
   }

   ubicacio ub = ubicacioError;

   // Buscamos la ubicacion en funcion de la estrategia utilizada
   switch (this->estrategia_usada) {
   case FIRST_FIT: {
      ub = encontrarPosicionTerminal_firstfit(c);
      break;
   }
   case LLIURE: {
      ub = encontrarPosicionTerminal_lliure(c);
      break;
   }
   }

   // Si no se ha encontrado ubicacion valida en la principalStorage
   // se guarda en la zona de espera
   if (ub != ubicacioError) {
      insertar_contenedor_principal(c, ub);
      recolocar_espera_en_principal();
   }
   else {
      insertar_contenedor_espera(c);
   }

}

void terminal::retira_contenidor(const string &m) throw(error){
   if (!elementos->existeix(m)) {
      throw error(MatriculaInexistent);
   }
   std::pair<contenidor,ubicacio> cont = (*elementos)[m];
   // Zona de espera
   if (cont.second == ubicacioEspera) {
      retira_contenedor_espera(cont.first);
   }
   // Zona principal
   else {
      retira_contenedor_principal(cont.first.matricula(), true);
      recolocar_espera_en_principal();
   }
}

ubicacio terminal::on(const string &m) const throw(){
   if ((*elementos).existeix(m)) {
      return (*elementos)[m].second;
   }
   else {
      return ubicacioError;
   }
}

nat terminal::longitud(const string &m) const throw(error){
   if ((*elementos).existeix(m)) {
      return (*elementos)[m].first.longitud();
   }
   else {
      throw error(MatriculaInexistent);
   }
}

void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error){
   // Comprobamos que el contendedor esté en la zona principal (principalStorage)
   if (u.filera() >= 0 && u.placa() >= 0 && u.pis() >= 0 && u.filera() < this->fileres && u.placa() < this->places && u.pis() < this->pisos) {
      m = principalStorage[u.filera()][u.placa()][u.pis()];
   }
   else {
      throw error(UbicacioNoMagatzem);
   }   
}  

nat terminal::fragmentacio() const throw() {
   int contador = 0;
   int i = 0;

   // Si el numero de plazas es igual a 1
   // solo tendremos que contar la cantidad de plazas que no ocupan todos los pisos
   // de la plaza y la fila
   if (this->places == 1) {
      for (int i = 0; i < this->fileres; i++) {
         if (this->estadoPrincipal[i][0] != this->pisos) {
            contador++;
         }
      }
   }
   else {
      while (i < this->fileres) {
         int j = 0;
         while (j < this->places) {
            if (this->estadoPrincipal[i][j] != this->pisos) {
               if (j == 0) {// si está en el principio
                  if (this->estadoPrincipal[i][j] != this->estadoPrincipal[i][j+1]) {
                     contador++;
                  }
               }
               else if (j == this->places-1) {// si está en el final
                  if (this->estadoPrincipal[i][j] != this->estadoPrincipal[i][j-1]) {
                     contador++;
                  }
               }
               else {// si está entre medio
                  if (this->estadoPrincipal[i][j] != this->estadoPrincipal[i][j-1] && this->estadoPrincipal[i][j] != this->estadoPrincipal[i][j+1]) {
                     contador++;
                  }
               }
            }
            j++;
         }
         i++;
      }
   }
   
   return contador;
}

nat terminal::ops_grua() const throw(){
   return moviments_grua;
}

void terminal::area_espera(list<string> &l) const throw(){
   l = this->waitingStorage;
   l.sort();
}

nat terminal::num_fileres() const throw(){
   return this->fileres;
}

nat terminal::num_places() const throw(){
   return this->places;
}

nat terminal::num_pisos() const throw(){
   return this->pisos;
}

terminal::estrategia terminal::quina_estrategia() const throw(){
   return estrategia_usada;
}

void terminal::insertar_contenedor_principal(const contenidor &c, const ubicacio &u) {
   if (this->elementos->existeix(c.matricula())) {
      this->elementos->elimina(c.matricula());
   }
   for (int i = u.placa(); i < (u.placa() + c.longitud()/10); i++) { // pone todos las posiciones donde va el contenedor al valor de la matricula
      this->principalStorage[u.filera()][i][u.pis()] = c.matricula();
   }
   for (int i = u.placa(); i < (u.placa() + c.longitud()/10); i++) { // aumenta el piso de las posiciones en estadoPrincipal
      this->estadoPrincipal[u.filera()][i] += 1;
   }
   this->elementos->assig(c.matricula(), std::pair<contenidor,ubicacio>(c, u)); // guarda el contenedor y ubicaion en el catalogo
   this->moviments_grua++;
}

void terminal::eliminar_contenedor_principal(const contenidor &c) {
   if (this->elementos->existeix(c.matricula())) {
      ubicacio ub = (*elementos)[c.matricula()].second;
      for (int i = ub.placa(); i < ub.placa() + c.longitud()/10; i++) { // pone todos las posiciones donde va el contenedor a ""
         this->principalStorage[ub.filera()][i][ub.pis()] = "";
      }
      for (int i = ub.placa(); i < ub.placa() + c.longitud()/10; i++) { // disminuye el piso de las posiciones en estadoPrincipal
         this->estadoPrincipal[ub.filera()][i] -= 1;
      }
      this->elementos->elimina(c.matricula()); // elimina el contenedor del catalogo
      this->moviments_grua++;
   }
}

void terminal::insertar_contenedor_espera(const contenidor &c) {
   if (this->elementos->existeix(c.matricula())) {
      this->elementos->elimina(c.matricula());
   }
   switch (this->estrategia_usada) {
      case FIRST_FIT: {
         this->waitingStorage.push_front(c.matricula()); // si es first_fit guarda los elementos al principio de la lista para hacer un sistema LIFO
         break;
      }
      case LLIURE: {
         this->waitingStorage.push_back(c.matricula()); // si es lliure guarda los elemento al final de la lista para hacer un sistema FIFO
         break;
      }
   }
   this->elementos->assig(c.matricula(), std::pair<contenidor,ubicacio>(c, ubicacioEspera));
}

bool terminal::mover_espera_principal(const contenidor &c, list<string>::iterator it) {
   ubicacio ub = ubicacioError;

   switch (this->estrategia_usada) {
      case FIRST_FIT: {
         ub = encontrarPosicionTerminal_firstfit(c);
         break;
      }
      case LLIURE: {
         ub = encontrarPosicionTerminal_lliure(c);
         break;
      }
   }

   if (ub != ubicacioError) {
      // lo eliminamos del catalogo (está en espera)
      elementos->elimina(c.matricula());
      // Lo eliminamos de la waitingStorage
      this->waitingStorage.erase(it);
      // insertamos en contenedor principal
      insertar_contenedor_principal(c, ub);
      return true;
   }
   else {
      return false;
   }
   
}

void terminal::mover_principal_espera(const contenidor &c) {
   if (this->elementos->existeix(c.matricula())) {
      eliminar_contenedor_principal(c); // eliminamos del contenedor principal
      insertar_contenedor_espera(c); // insertamos en la zona de espera
   }
}

void terminal::retira_contenedor_principal(const string &m, bool elimina) {
   std::pair<contenidor, ubicacio> cont = (*elementos)[m];

   int i = cont.second.placa();
   while(i < cont.second.placa() + cont.first.longitud()/10) {
      int j = cont.second.pis()+1;
      while(j < this->pisos) {
         if (this->principalStorage[cont.second.filera()][i][j] != "") { // si encuentra un contenedor encima del que queremos sacar
            retira_contenedor_principal(this->principalStorage[cont.second.filera()][i][j], false); // saca ese contenedor tambien
         }
         j++;
      }
      i++;
   }

   if (!elimina) {
      mover_principal_espera(cont.first);
   }
   else {
      eliminar_contenedor_principal(cont.first);
   }
}

void terminal::retira_contenedor_espera(const contenidor &c) {
   this->elementos->elimina(c.matricula());
   this->waitingStorage.remove(c.matricula());
}

void terminal::recolocar_espera_en_principal() {
   list<string>::iterator it = this->waitingStorage.begin();
   while (it != this->waitingStorage.end()) {
      if (elementos->existeix(*it)) {
         std::pair<contenidor,ubicacio> cont = (*elementos)[*it];
         if (mover_espera_principal(cont.first, it)) { // si consigue mover el contenedor de la zona de espera a la zona principal
            it = this->waitingStorage.begin(); // reinicia el iterador al principio de la lista
         }
         else {
            ++it; // sino aumenta la posicion del iterador
         }
      }
      else {
         ++it; // si no existe el elemento aumenta la posicion del iterador (esto no deberia pasar)
      }
   }
}

ubicacio terminal::encontrarPosicionTerminal_firstfit(const contenidor &c) {
   for (int i = 0; i < this->fileres; i++) {
      for (int j = 0; j < this->places; j++) {
         bool cabe = cabeContenedorUbi(c.longitud()/10, i, j); // si cabe en la posicion
         if (cabe) {
            return ubicacio(i, j, this->estadoPrincipal[i][j]); // devuelve la posicion
         }
      }
   }
   return ubicacioError; // si llega aqui es que no ha encontrado una ubicacion valida para el contenedor
}

ubicacio terminal::encontrarPosicionTerminal_lliure(const contenidor &c) {
   for(int i = 0; i < this->pisos; i++) {
      for(int j = 0; j < this->fileres; j++) {
         for(int k = 0; k < this->places; k++) {
            if (this->estadoPrincipal[j][k] == i && cabeContenedorUbi(c.longitud()/10, j, k)) {
               return ubicacio(j, k, i);
            }
         }
      }
   }
   return ubicacioError; // si llega aqui es que no ha encontrado una ubicacion valida para el contenedor
}

bool terminal::cabeContenedorUbi(const int& size, const int& i, const int& j) {
   int posIni = this->estadoPrincipal[i][j];
   int contador = 0;
   for(int k = j; (k < j + size && k < this->places); k++) {
      if (this->estadoPrincipal[i][k] != posIni || this->estadoPrincipal[i][k] >= this->pisos) {
         return false;
      }
      contador++;
   }
   if (contador == size) {
      return true;
   }
   else {
      return false;
   }
}