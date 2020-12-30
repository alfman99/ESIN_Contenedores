#include "terminal.hpp"
#include "terminal.rep"

/* Constructora. Crea una terminal buida amb n fileres de m places
   cadascuna, i una alçada màxima d'apilament h; a més fixa l'estratègia
   d'inserció i retirada dels contenidors respecte el paràmetre st.
   Genera un error si n=0, m=0, h=0, h > HMAX o
   st no pertany a {FIRST_FIT, LLIURE}. */
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

   elementos = new cataleg< std::pair<contenidor,ubicacio> >(this->fileres * this->places * this->pisos);

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

/* Constructora per còpia, assignació i destructora. */
terminal::terminal(const terminal& b) throw(error) {
   
}

terminal& terminal::operator=(const terminal& b) throw(error){
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

/* Col·loca el contenidor c en l'àrea d'emmagatzematge de la terminal o
   en l'àrea d'espera si no troba lloc en l'àrea d'emmagatzematge usant
   l'estratègia prefixada en el moment de crear la terminal. Si el
   contenidor c es col·loca en l'àrea d'emmagatzematge pot succeir que
   un o més contenidors de l'àrea d'espera puguin ser moguts a l'àrea
   d'emmagatzematge.
   En aquest cas es mouran els contenidors de l'àrea d'espera a l'àrea
   d'emmagatzematge seguint l'ordre que indiqui l'estratègia que s'està
   usant. Finalment, genera un error si ja existís a la terminal un
   contenidor amb una matrícula idèntica que la del contenidor c. */
void terminal::insereix_contenidor(const contenidor &c) throw(error){
   std::cout << "insereix... hacer" << std::endl;
   (*elementos).assig(c.matricula(), std::pair<contenidor,ubicacio>(c, ubicacio(-1,-1,-1)));
}

/* Retira de la terminal el contenidor c la matrícula del qual és igual
   a m. Aquest contenidor pot estar a l'àrea d'emmagatzematge o a l'àrea
   d'espera. Si el contenidor estigués a l'àrea d'emmagatzematge llavors
   s'hauran de moure a l'àrea d'espera tots els contenidors que siguin
   necessaris per netejar la part superior de c, s'hauran de retirar
   possiblement diversos contenidors, començant pel contenidor sense cap
   altre a sobre amb el número de plaça més baix (més a l'esquerra) i així
   successivament (veure exemple amb detall a la subsecció Estratègia
   FIRST_FIT). Un cop s'hagi eliminat el contenidor indicat, s'intenta
   moure contenidors de l'àrea d'espera a l'àrea d'emmagatzematge, seguint
   l'ordre que indiqui l'estratègia que s'està usant. Genera un error si a
   la terminal no hi ha cap contenidor la matrícula del qual sigui igual a m. */
void terminal::retira_contenidor(const string &m) throw(error){
   std::cout << "retira... hacer" << std::endl;
}

/* Retorna la ubicació <i, j, k> del contenidor la matrícula del qual és
   igual a m si el contenidor està a l'àrea d'emmagatzematge de la terminal,
   la ubicació <-1, 0, 0> si el contenidor està a l'àrea d'espera, i la
   ubicació <-1, -1, -1> si no existeix cap contenidor que tingui una
   matrícula igual a m.
   Cal recordar que si un contenidor té més de 10 peus, la seva ubicació
   correspon a la plaça que tingui el número de plaça més petit. */
ubicacio terminal::on(const string &m) const throw(){
   return (*elementos)[m].second;
}

/* Retorna la longitud del contenidor la matrícula del qual és igual
   a m. Genera un error si no existeix un contenidor a la terminal
   la matrícula del qual sigui igual a m. */
nat terminal::longitud(const string &m) const throw(error){
   return (*elementos)[m].first.longitud();
}

/* Retorna la matrícula del contenidor que ocupa la ubicació u = <i, j, k>
   o la cadena buida si la ubicació està buida. Genera un error si
   i < 0, i >= n, j < 0, j >= m, k < 0 o k >= h, o sigui si <i, j, k> no
   identifica una ubicació vàlida de l'àrea d'emmagatzematge. Cal observar
   que si m, obtinguda amb t.contenidor_ocupa(u, m), és una matrícula (no
   la cadena buida) pot succeir que u != t.on(m), ja que un contenidor pot 
   ocupar diverses places i la seva ubicació es correspon amb la de la
   plaça ocupada amb número de plaça més baix. */
void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error){
   if (u.filera() < 0 || u.placa() < 0 || u.pis() < 0 || u.filera() > this->fileres || u.placa() > this->places || u.pis() > this->pisos) {
      throw error(UbicacioNoMagatzem);
   }

   m = principalStorage[u.filera()][u.placa()][u.pis()];
}  

/* Retorna el nombre de places de la terminal que en aquest instant
   només hi cabrien un contenidor de 10 peus, però no un de més llarg.
   Per exemple, la filera de la figura 1 de l'enunciat contribuirà amb
   7 unitats a la fragmentació total (corresponen a les ubicacions
   <f, 0, 1>, <f, 1, 2>, <f, 2, 1>, <f, 7, 1>, <f, 8, 0>, <f, 9, 1> i
   <f, 10, 0>). */
nat terminal::fragmentacio() const throw(){
   // Hacer
   return 3;
}

/* Retorna el número d'operacions de grua realitzades des del moment
   de creació de la terminal.
   Es requereix d'una operació de grua per moure un contenidor
   des de l'àrea d'espera a l'àrea d'emmagatzematge o viceversa.
   També es requereix d'una operació de grua per inserir o
   retirar directament un contenidor de l'àrea d'emmagatzematge.
   En canvi no requereix cap operació de grua inserir o
   retirar directament un contenidor de l'àrea d'espera. */
nat terminal::ops_grua() const throw(){
   return moviments_grua;
}

/* Retorna la llista de les matrícules de tots els contenidors
   de l'àrea d'espera de la terminal, en ordre alfabètic creixent. */
void terminal::area_espera(list<string> &l) const throw(){
   // Hacer
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