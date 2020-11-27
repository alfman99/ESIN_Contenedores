#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include <esin/error>
#include <esin/gen_driver>
#include <esin/util>

#include "contenidor.hpp"

using util::nat;
using util::byte;
using namespace std;

// imprimir ubicacio i imprimir contenidor

ostream& operator<<(ostream& os, const contenidor& c) {
  os << "<" << c.matricula() << ", " << c.longitud() << ">\n";
  return os;
}

// funcions d'utilitat

// imprimeix un boole�
ostream& operator<=(ostream& os, const bool b) {
  os << (b ? "true" : "false");
  return os;
}

// imprimeix una llista de T's
template <typename T>
ostream& operator<<(ostream& os, const list<T>& L) {
  if (L.empty()) { 
    os << "[]";  
  }
  else {
    typename list<T>::const_iterator it = L.begin();
    T prev = *it;
    os << "[";
    ++it;
    while (it != L.end()) {
      os << prev << ", ";
      prev = *it;
      ++it;
    }
    os << prev << "]";
  }
  return os;
}

// user init
void* user_init(gen_driver& dr) {
  string id = dr.args(1);
  string tid = dr.args(2);
  
  if (tid == "contenidor") {
    if (dr.nargs()==4) {
      if (util::toint(dr.args(4)) < 0) {
	throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		    gen_driver::WrongTypeArgsMsg);
      }
      nat longi = static_cast<nat> (util::toint(dr.args(4)));
      return static_cast<void*>(new contenidor(dr.args(3), longi));
    }
    else {
      throw error(gen_driver::WrongNumArgs, gen_driver::nom_mod,
		  gen_driver::WrongNumArgsMsg);
    }
  }
    else {
      throw error(gen_driver::WrongNumArgs,gen_driver::nom_mod,
		  gen_driver::WrongNumArgsMsg);
    }
}

/* TypeTraits */

template <> const char* TypeTraits<contenidor>::name = "contenidor";

/*-------------------------< CONTENIDOR >--------------------------*/

void tracta_longitud(gen_driver& dr) {
  contenidor* c = dr.object<contenidor>(); 
  dr.get_ostream() << c -> longitud() << endl;
}

void tracta_matricula(gen_driver& dr) {
  contenidor* c = dr.object<contenidor>(); 
  dr.get_ostream() << c -> matricula() << endl;
}

void tracta_menor(gen_driver& dr) {
  if (dr.object_type()=="contenidor") {
    contenidor* c1 = dr.object<contenidor>();
    contenidor* c2 = dr.object<contenidor>(1);
    (dr.get_ostream() <= (*c1 < *c2)) << endl;
  }
  else {
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
  }
}

void tracta_major(gen_driver& dr) {
  if (dr.object_type()=="contenidor") {
    contenidor* c1 = dr.object<contenidor>();
    contenidor* c2 = dr.object<contenidor>(1);
    (dr.get_ostream() <= (*c1 > *c2)) << endl;
  }
  else {
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
  }
}

void tracta_igual(gen_driver& dr) {
  if (dr.object_type()=="contenidor") {
    contenidor* c1 = dr.object<contenidor>();
    contenidor* c2 = dr.object<contenidor>(1);
    (dr.get_ostream() <= (*c1 == *c2)) << endl;
  }
  else
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
}

void tracta_no_igual(gen_driver& dr) {
  if (dr.object_type()=="contenidor") {
    contenidor* c1 = dr.object<contenidor>();
    contenidor* c2 = dr.object<contenidor>(1);
    (dr.get_ostream() <= (*c1 != *c2)) << endl;
  }
  else
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
}

void tracta_menor_igual(gen_driver& dr) {
  if (dr.object_type()=="contenidor") {
    contenidor* c1 = dr.object<contenidor>();
    contenidor* c2 = dr.object<contenidor>(1);
    (dr.get_ostream() <= (*c1 <= *c2)) << endl;
  }
  else
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
}

void tracta_major_igual(gen_driver& dr) {
  if (dr.object_type()=="contenidor") {
    contenidor* c1 = dr.object<contenidor>();
    contenidor* c2 = dr.object<contenidor>(1);
    (dr.get_ostream() <= (*c1 >= *c2)) << endl;
  }
  else {
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
  }
}



/*------------------------------< MAIN >-----------------------*/

int main(){  
  gen_driver d("gestterm.err", false);

  //contenidor
  d.add_call("longitud", tracta_longitud, "contenidor");
  d.add_call("matricula", tracta_matricula, "contenidor");

  

  //operadors relacionals
  d.add_call("<", tracta_menor, "any", "any");
  d.add_call(">", tracta_major, "any", "any");
  d.add_call("==", tracta_igual, "any", "any");
  d.add_call("!=", tracta_no_igual, "any", "any");
  d.add_call("<=", tracta_menor_igual, "any", "any");
  d.add_call(">=", tracta_major_igual, "any", "any");

  // instal�lacio de tipus
  d.install_type<contenidor>();

  d.go();
}
