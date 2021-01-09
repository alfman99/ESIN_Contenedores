#include "terminal.hpp"
#include "terminal.rep"

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
}

void terminal::insereix_contenidor(const contenidor &c) throw(error){
   if (this->elementos->existeix(c.matricula())) {
      throw error(MatriculaDuplicada);
   }
   switch (this->estrategia_usada) {
   case FIRST_FIT: {
      insereix_firstfit(c);
      break;
   }
   case LLIURE: {
      insereix_lliure(c);
      break;
   }
   }
}

void terminal::retira_contenidor(const string &m) throw(error){
   if (!elementos->existeix(m)) {
      throw error(MatriculaInexistent);
   }
   std::pair<contenidor,ubicacio> cont = (*elementos)[m];
   // Zona de espera
   if (cont.second == ubicacio(-1, 0, 0)) {
      retira_contenedor_espera(cont.first);
   }
   // Zona principal
   else {
      retira_contenedor_principal(cont.first.matricula(), true);
      recolocar_espera_en_principal();
   }
}

/* Retorna la ubicació <i, j, k> del contenidor la matrícula del qual és
   igual a m si el contenidor està a l'àrea d'emmagatzematge de la terminal,
   la ubicació <-1, 0, 0> si el contenidor està a l'àrea d'espera, i la
   ubicació <-1, -1, -1> si no existeix cap contenidor que tingui una
   matrícula igual a m.
   Cal recordar que si un contenidor té més de 10 peus, la seva ubicació
   correspon a la plaça que tingui el número de plaça més petit. */
ubicacio terminal::on(const string &m) const throw(){
   if ((*elementos).existeix(m)) {
      return (*elementos)[m].second;
   }
   else {
      return ubicacio(-1, -1, -1);
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
   if (u.filera() >= 0 && u.placa() >= 0 && u.pis() >= 0 && u.filera() < this->fileres && u.placa() < this->places && u.pis() < this->pisos) {
      m = principalStorage[u.filera()][u.placa()][u.pis()];
   }
   else {
      throw error(UbicacioNoMagatzem);
   }   
}  

// Bien
nat terminal::fragmentacio() const throw(){
   int fragmentos = 0;
   int i = 0;
   while(i < this->pisos) {
      int j = 0;
      while(j < this->fileres) {
         int k = 0;
         int contador = 0;
         while(k < this->places){
            if ((i == 0 || this->principalStorage[j][k][i-1] != "") && this->principalStorage[j][k][i] == "") {
               contador++;
            }
            else {
               if (contador == 1) {
                  fragmentos++;
                  contador = 0;
               }
            }
            k++;
         }
         if (contador == 1) {
            fragmentos++;
         }
         j++;
      }
      i++;
   }
   return fragmentos;
}

nat terminal::ops_grua() const throw(){
   return moviments_grua;
}

// Podemos implementar nosotros el sort
void terminal::area_espera(list<string> &l) const throw(){
   // Hacer
   l = this->waitingStorage;
   l.sort();
}

/* Retorna el número de fileres de la terminal. */
nat terminal::num_fileres() const throw(){
   return this->fileres;
}

/* Retorna el número de places per filera de la terminal. */
nat terminal::num_places() const throw(){
   return this->places;
}

/* Retorna l'alçada màxima d'apilament de la terminal. */
nat terminal::num_pisos() const throw(){
   return this->pisos;
}

/* Retorna l'estratègia d'inserció i retirada de contenidors de
   la terminal. */
terminal::estrategia terminal::quina_estrategia() const throw(){
   return estrategia_usada;
}

void terminal::insereix_firstfit(const contenidor &c) {
   ubicacio ub = encontrarPosicionTerminal_firstfit(c);
   if (ub != ubicacio(-1, -1, -1)) {
      insertar_contenedor_principal(c, ub);
   }
   else {
      insertar_contenedor_espera(c);
   }
}

void terminal::insereix_lliure(const contenidor &c) {
   return;
}

// Inserta el contenedor en el area principal
void terminal::insertar_contenedor_principal(const contenidor &c, const ubicacio &u) {
   if (this->elementos->existeix(c.matricula())) {
      this->elementos->elimina(c.matricula());
   }
   for (int i = u.placa(); i < (u.placa() + c.longitud()/10); i++) {
      this->principalStorage[u.filera()][i][u.pis()] = c.matricula();
   }
   this->elementos->assig(c.matricula(), std::pair<contenidor,ubicacio>(c, u));
   this->moviments_grua++;
}

// Eliminar el contenedor de la area principal para siempre
void terminal::eliminar_contenedor_principal(const contenidor &c) {
   if (this->elementos->existeix(c.matricula())) {
      ubicacio ub = (*elementos)[c.matricula()].second;
      for (int i = ub.placa(); i < ub.placa() + c.longitud()/10; i++) {
         this->principalStorage[ub.filera()][i][ub.pis()] = "";
      }
      this->elementos->elimina(c.matricula());
   }
}

// Inserta el contenedor en el area de espera
void terminal::insertar_contenedor_espera(const contenidor &c) {
   if (this->elementos->existeix(c.matricula())) {
      this->elementos->elimina(c.matricula());
   }
   this->waitingStorage.push_back(c.matricula());
   this->elementos->assig(c.matricula(), std::pair<contenidor,ubicacio>(c, ubicacio(-1, 0, 0)));
}

// Mueve un contenedor del area de espera al principal
void terminal::mover_espera_principal(const contenidor &c, const ubicacio &u) {
   // lo eliminamos del catalogo (está en espera)
   elementos->elimina(c.matricula());
   // Lo eliminamos de la waitingStorage
   this->waitingStorage.remove(c.matricula());
   // insertamos en contenedor principal
   insertar_contenedor_principal(c, u);
   this->moviments_grua++;
}

// Mueve un contenedor de la zona principal a la zona de espera
void terminal::mover_principal_espera(const contenidor &c) {
   if (this->elementos->existeix(c.matricula())) {
      ubicacio ub = (*elementos)[c.matricula()].second;
      for (int i = ub.placa(); i < ub.placa() + c.longitud()/10; i++) {
         this->principalStorage[ub.filera()][i][ub.pis()] = "";
      }
      this->elementos->elimina(c.matricula());
      this->elementos->assig(c.matricula(), std::pair<contenidor,ubicacio>(c, ubicacio(-1, 0, 0)));
   }
}

// Elimina el contenedor de la zona principal
void terminal::retira_contenedor_principal(const string &m, bool elimina) {
   std::pair<contenidor, ubicacio> cont = (*elementos)[m];

   int i = cont.second.placa();
   while(i < cont.second.placa() + cont.first.longitud()/10) {
      int j = this->pisos-1;
      while(j > cont.second.pis()) {
         if (this->principalStorage[cont.second.filera()][i][j] != "") {
            retira_contenedor_principal(this->principalStorage[cont.second.filera()][i][j], false);
         }
         j--;
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

// Elimina el contenedor de la zona de espera
void terminal::retira_contenedor_espera(const contenidor &c) {
   this->elementos->elimina(c.matricula());
   this->waitingStorage.remove(c.matricula());
}

// Recoloca los contenedores de la espera al principal
void terminal::recolocar_espera_en_principal() {
   for (list<string>::reverse_iterator it = this->waitingStorage.rbegin(); it != this->waitingStorage.rend(); --it) {
      if (elementos->existeix(*it)) {
         std::pair<contenidor,ubicacio> cont = (*elementos)[*it];

         ubicacio ub = encontrarPosicionTerminal_firstfit(cont.first);

         if (ub != ubicacio(-1, -1, -1)) {
            mover_espera_principal(cont.first, ub);
         }
      }      
   }
}

// Encuentra una posicion el el area principal para el contenedor
ubicacio terminal::encontrarPosicionTerminal_firstfit(const contenidor &c) {
   return ubicacio(-1, -1, -1);
}