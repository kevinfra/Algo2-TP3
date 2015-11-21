#ifndef DICCIONARIO_NAT_FIJO
#define DICCIONARIO_NAT_FIJO

#include "aed2.h"
#include "Tipos.h"

using namespace aed2;

template<typename alpha>
class diccNat{
public:
  //forward declarations
  class itDiccNat;

  //Tupla clave-significado
  struct tupla{
    Nat clave;
    alpha significado;
  };

  //Constructor
  diccNat(const Vector<tupla> &v); //CrearDiccionario

  //Operaciones Basicas
  void redefinir(Nat n, const alpha &a);
  alpha* obtener(Nat n);
  bool definido(Nat n);
  Nat cantClaves();
  Conj<Nat>::Iterador crearItClaves();

  //Operaciones del Iterador
  itDiccNat crearIt();

  //Clase iterador de Diccionario
  class itDiccNat{
  public:
    bool haySiguiente();
    tupla siguiente();
    alpha siguienteSignificado();
    void avanzar();

  private:
    typename Lista<tupla*>::Iterador _iteradorLista;
  };


private:
  Vector< Lista<tupla> > _tabla;
  Lista<tupla*> _listaIterable;
  Conj<Nat> _clavesIterables; //SOLUCION A LA COMPLEJIDAD PEDIDA PARA ITERADOR DE CLAVES
};

//Implementacion
template<typename alpha>
diccNat<alpha>::diccNat(const Vector<tupla> &v){
  Nat i = 0;
  while(i < v.Longitud()){
    this->_tabla.AgregarAtras(Lista<tupla>());
    i++;
  }
  i = 0;
  while(i < v.Longitud()){
    Nat k = (v[i].clave % v.Longitud());
    this->_tabla[k].AgregarAtras(v[i]);
    Nat q = this->_tabla[k].Longitud();
    this->_listaIterable.AgregarAtras(&(this->_tabla[k][q-1]));

    //Creo _clavesIterables
    this->_clavesIterables.Agregar(v[i].clave);
    i++;
  }
}

template<typename alpha>
void diccNat<alpha>::redefinir(Nat n, const alpha &a){
  Nat k = (n % this->_tabla.Longitud());
  typename Lista<tupla>::Iterador it = this->_tabla[k].CrearIt();
  while(it.HaySiguiente()){
  	if(it.Siguiente().clave == n){
  		it.Siguiente().significado = a;
  	}
  	it.Avanzar();
  }
}

template<typename alpha>
alpha* diccNat<alpha>::obtener(Nat n){
	Nat k = (n % this->_tabla.Longitud());
	typename Lista<tupla>::Iterador it = this->_tabla[k].CrearIt();
	alpha* res = NULL;
	while (it.HaySiguiente()){
  		if(it.Siguiente().clave == n){
  			res = &(it.Siguiente().significado);
  		}
  	it.Avanzar();
  }
  return res;
}

template<typename alpha>
bool diccNat<alpha>::definido(Nat n){
	Nat k = (n % this->_tabla.Longitud());
	typename Lista<tupla>::Iterador it = this->_tabla[k].CrearIt();
	while (it.HaySiguiente() && (it.Siguiente().clave != n)){
	  it.Avanzar();
  }
  return (it.HaySiguiente() && (it.Siguiente().clave == n));
}

template<typename alpha>
Nat diccNat<alpha>::cantClaves(){
	return this->_tabla.Longitud();
}

template<typename alpha>
Conj<Nat>::Iterador diccNat<alpha>::crearItClaves(){
  return this->_clavesIterables.CrearIt();
}


template<typename alpha>
typename diccNat<alpha>::itDiccNat diccNat<alpha>::crearIt(){
	return this->_listaIterable.CrearIt();
}

template<typename alpha>
bool diccNat<alpha>::itDiccNat::haySiguiente(){
	return this->_iteradorLista.HaySiguiente();
}

template<typename alpha>
typename diccNat<alpha>::tupla diccNat<alpha>::itDiccNat::siguiente(){
	return this->_iteradorLista.Siguiente();
}

template<typename alpha>
alpha diccNat<alpha>::itDiccNat::siguienteSignificado(){
	return &(this->_iteradorLista.Siguiente()).significado;
}


template<typename alpha>
void diccNat<alpha>::itDiccNat::avanzar(){
	_iteradorLista.Avanzar();
}

#endif //DICCIONARIO_NAT_FIJO
