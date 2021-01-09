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
   switch (this->estrategia_usada) {
   case FIRST_FIT: {
      insereix_firstfit(c);
      // printMatrix();
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
   if (cont.second == ubicacioEspera) {
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
               if (j == 0) {
                  if (this->estadoPrincipal[i][j] != this->estadoPrincipal[i][j+1]) {
                     contador++;
                  }
               }
               else if (j == this->places-1) {
                  if (this->estadoPrincipal[i][j] != this->estadoPrincipal[i][j-1]) {
                     contador++;
                  }
               }
               else {
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

   /*std::cout << "this->estadoPrincipal: " << std::endl;
   for(int i = 0; i < this->fileres; i++) {
      for(int j = 0; j < this->places; j++) {
         std::cout << " " << this->estadoPrincipal[i][j];
      }
      std::cout << std::endl;
   }*/
   
   return contador;
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
   if (ub != ubicacioError) {
      insertar_contenedor_principal(c, ub);
      recolocar_espera_en_principal();
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
   for (int i = u.placa(); i < (u.placa() + c.longitud()/10); i++) {
      this->estadoPrincipal[u.filera()][i] += 1;
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
      for (int i = ub.placa(); i < ub.placa() + c.longitud()/10; i++) {
         this->estadoPrincipal[ub.filera()][i] -= 1;
         // std::cout << c.matricula() << " " << ub.filera() << " " << i << std::endl;
      }
      this->elementos->elimina(c.matricula());
      this->moviments_grua++;
      // std::cout << "ESTOY EN eliminar_contenedor_principal" <<  std::endl;
   }
}

// Inserta el contenedor en el area de espera
void terminal::insertar_contenedor_espera(const contenidor &c) {
   if (this->elementos->existeix(c.matricula())) {
      this->elementos->elimina(c.matricula());
   }
   this->waitingStorage.push_front(c.matricula());
   this->elementos->assig(c.matricula(), std::pair<contenidor,ubicacio>(c, ubicacioEspera));
   // std::cout << "ESTOY EN insertar_contenedor_espera" <<  std::endl;
}

// Mueve un contenedor del area de espera al principal
void terminal::mover_espera_principal(const contenidor &c, list<string>::iterator& it) {

   ubicacio ub = encontrarPosicionTerminal_firstfit(c);
   if (ub != ubicacioError) {
      // lo eliminamos del catalogo (está en espera)
      elementos->elimina(c.matricula());
      // Lo eliminamos de la waitingStorage
      it = this->waitingStorage.erase(it);
      // insertamos en contenedor principal
      insertar_contenedor_principal(c, ub);
   }
   else {
      ++it;
   }
   
}

// Mueve un contenedor de la zona principal a la zona de espera
void terminal::mover_principal_espera(const contenidor &c) {
   if (this->elementos->existeix(c.matricula())) {
      eliminar_contenedor_principal(c);
      insertar_contenedor_espera(c);
   }
}

// Elimina el contenedor de la zona principal
void terminal::retira_contenedor_principal(const string &m, bool elimina) {
   std::pair<contenidor, ubicacio> cont = (*elementos)[m];

   int i = cont.second.placa();
   while(i < cont.second.placa() + cont.first.longitud()/10) {
      int j = cont.second.pis()+1;
      while(j < this->pisos) {
         if (this->principalStorage[cont.second.filera()][i][j] != "") {
            retira_contenedor_principal(this->principalStorage[cont.second.filera()][i][j], false);
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

// Elimina el contenedor de la zona de espera
void terminal::retira_contenedor_espera(const contenidor &c) {
   this->elementos->elimina(c.matricula());
   this->waitingStorage.remove(c.matricula());
}

// Recoloca los contenedores de la espera al principal
void terminal::recolocar_espera_en_principal() {
   list<string>::iterator it = this->waitingStorage.begin();
   while (it != this->waitingStorage.end()) {
      if (elementos->existeix(*it)) {
         std::pair<contenidor,ubicacio> cont = (*elementos)[*it];
         mover_espera_principal(cont.first, it);
      }
      else {
         ++it;
      }
   }
}

// Encuentra una posicion el el area principal para el contenedor
ubicacio terminal::encontrarPosicionTerminal_firstfit(const contenidor &c) {
   for (int i = 0; i < this->fileres; i++) {
      for (int j = 0; j < this->places; j++) {
         bool cabe = cabeContenedorUbi(c.longitud()/10, i, j);
         if (cabe) {
            return ubicacio(i, j, this->estadoPrincipal[i][j]);
         }
      }
   }
   return ubicacioError;
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

void terminal::printMatrix() {
   std::cout << "this->estadoPrincipal: " << std::endl;
   for(int i = 0; i < this->fileres; i++) {
      for(int j = 0; j < this->places; j++) {
         std::cout << " " << this->estadoPrincipal[i][j];
      }
      std::cout << std::endl;
   }
}