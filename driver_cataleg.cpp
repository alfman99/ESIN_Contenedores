#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include <esin/error>
#include <esin/gen_driver>
#include <esin/util>

#include "cataleg.hpp"

using util::nat;
using util::byte;
using namespace std;

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
  
  if (tid == "cataleg<int>") {
    if (dr.nargs()==3) {
      return static_cast<void*>(new cataleg<int>(util::toint(dr.args(3))));
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

template <> const char* TypeTraits<cataleg<int> >::name = "cataleg<int>";

/*------------------------------< CATALEG<T> >-----------------------*/

template <typename T>
void tracta_assig(gen_driver& dr) { 
  cataleg<T> *ct = dr.template object<cataleg<T> >();
  string k = dr.args(1);
  T *v = dr.object<T> (dr.args(2));
  ct->assig(k, *v);
}
 
template <>
void tracta_assig<int>(gen_driver& dr) { 
  cataleg<int> *ct = dr.object<cataleg<int> >();
  string k = dr.args(1);
  int v = util::toint(dr.args(2));
  ct->assig(k, v);
}

template <typename T>
void tracta_consulta(gen_driver& dr) { 
  cataleg<T> *ct = dr.template object<cataleg<T> >();
  string k = dr.args(1);
  dr.get_ostream() << (*ct) [k] << endl;
}

template <typename T>
void tracta_elimina(gen_driver& dr) { 
  cataleg<T> *ct = dr.template object<cataleg<T> >();
  string k = dr.args(1);
  ct->elimina(k);
}

template <typename T>
void tracta_existeix(gen_driver& dr) { 
  cataleg<T> *ct = dr.template object<cataleg<T> >();
  string k = dr.args(1);
  dr.get_ostream() <= ct->existeix(k);
  dr.get_ostream() << endl;
}

template <typename T>
void tracta_quants(gen_driver& dr) { 
  cataleg<T> *ct = dr.template object<cataleg<T> >();
  dr.get_ostream() << ct->quants() << endl;
}

/*------------------------------< MAIN >-----------------------*/

int main(){  
  gen_driver d("gestterm.err", false);

  //contenidor
  // d.add_call("longitud", tracta_longitud, "contenidor");
  // d.add_call("matricula", tracta_matricula, "contenidor");

  // cataleg<int>
  d.add_call("assig", tracta_assig<int>, "cataleg<int>", "string int");
  d.add_call("val", tracta_consulta<int>, "cataleg<int>", "string");
  d.add_call("existeix", tracta_existeix<int>, "cataleg<int>", "string");
  d.add_call("elimina", tracta_elimina<int>, "cataleg<int>", "string");
  d.add_call("quants", tracta_quants<int>, "cataleg<int>");
  
  // instal�lacio de tipus
  d.install_type<cataleg<int> >();

  d.go();
}
