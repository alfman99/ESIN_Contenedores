#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include <esin/error>
#include <esin/gen_driver>
#include <esin/util>

#include "ubicacio.hpp"

using util::nat;
using util::byte;
using namespace std;

// imprimir ubicacio i imprimir contenidor
ostream& operator<<(ostream& os, const ubicacio& u) {
  os << "<" << u.filera() << ", " << u.placa() << ", " << u.pis() << ">";
  return os;
}

// funcions d'utilitat

// imprimeix un booleà
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
  
  if (tid == "ubicacio") {
    if (dr.nargs()==5) {
      return static_cast<void*>(new ubicacio(util::toint(dr.args(3)),
					     util::toint(dr.args(4)), 
					     util::toint(dr.args(5)))); 
    }
    else {
      throw error(gen_driver::WrongNumArgs, gen_driver::nom_mod,
		  gen_driver::WrongNumArgsMsg);
    }
  }

  throw error(gen_driver::WrongTypeArgs,gen_driver::nom_mod,
	      gen_driver::WrongTypeArgsMsg);
}

/* TypeTraits */

template <> const char* TypeTraits<ubicacio>::name = "ubicacio";

/*-------------------------< UBICACIO >---------------------------*/

void tracta_filera(gen_driver& dr) {
  ubicacio* u = dr.object<ubicacio>(); 
  dr.get_ostream() << u -> filera() << endl;
}

void tracta_placa(gen_driver& dr) {
  ubicacio* u = dr.object<ubicacio>(); 
  dr.get_ostream() << u -> placa() << endl;
}

void tracta_pis(gen_driver& dr) {
  ubicacio* u = dr.object<ubicacio>(); 
  dr.get_ostream() << u -> pis() << endl;
}

/*--------------------< OPERADORES RELACIONALES >-------------------*/

void tracta_menor(gen_driver& dr) {
  if (dr.object_type()=="ubicacio") {
    ubicacio* u1 = dr.object<ubicacio>();
    ubicacio* u2 = dr.object<ubicacio>(1);
    (dr.get_ostream() <= (*u1 < *u2)) << endl;
  }
  else {
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
  }
}

void tracta_major(gen_driver& dr) {
  if (dr.object_type()=="ubicacio") {
    ubicacio* u1 = dr.object<ubicacio>();
    ubicacio* u2 = dr.object<ubicacio>(1);
    (dr.get_ostream() <= (*u1 > *u2)) << endl;
  }
  else {
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
  }
}

void tracta_igual(gen_driver& dr) {
  if (dr.object_type()=="ubicacio") {
    ubicacio* u1 = dr.object<ubicacio>();
    ubicacio* u2 = dr.object<ubicacio>(1);
    (dr.get_ostream() <= (*u1 == *u2)) << endl;
  }
  else
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
}

void tracta_no_igual(gen_driver& dr) {
  if (dr.object_type()=="ubicacio") {
    ubicacio* u1 = dr.object<ubicacio>();
    ubicacio* u2 = dr.object<ubicacio>(1);
    (dr.get_ostream() <= (*u1 != *u2)) << endl;
  }
  else
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
}

void tracta_menor_igual(gen_driver& dr) {
  if (dr.object_type()=="ubicacio") {
    ubicacio* u1 = dr.object<ubicacio>();
    ubicacio* u2 = dr.object<ubicacio>(1);
    (dr.get_ostream() <= (*u1 <= *u2)) << endl;
  }
  else
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
}

void tracta_major_igual(gen_driver& dr) {
  if (dr.object_type()=="ubicacio") {
    ubicacio* u1 = dr.object<ubicacio>();
    ubicacio* u2 = dr.object<ubicacio>(1);
    (dr.get_ostream() <= (*u1 >= *u2)) << endl;
  }
  else {
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
		gen_driver::WrongTypeArgsMsg);
  }
}

/*------------------------------< MAIN >-----------------------*/

int main(){  
  gen_driver d("gestterm.err", false);

  //ubicacio
  d.add_call("filera", tracta_filera, "ubicacio");
  d.add_call("placa", tracta_placa, "ubicacio");
  d.add_call("pis", tracta_pis, "ubicacio");

  d.go();
}
