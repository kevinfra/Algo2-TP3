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

  //Constructor
  crearDiccionario(Vector(tupla(Nat clave, alpha significado)) v);

  //Operaciones Basicas
  void redefinir(Nat n, alpha a);
  alpha obtener(Nat n);
  bool definido(Nat n);
  Nat cantClaves();
  Conj::Iterador crearItClaves();

  //Operaciones del Iterador
  itDiccNat crearIt();

  //Clase iterador de Diccionario
  class itDiccNat{
  public:
    bool haySiguiente();
    tupla(const Nat, alpha) siguiente();
    const alpha siguienteSignificado();
    void avanzar();

  private:
    Lista<tupla(Nat clave, alpha significado)>::Iterador _iteradorLista;
  };

private:
  Vector(Lista(tupla(Nat clave, alpha significado))) _tabla;
  Lista( *(tupla(Nat clave, alpha significado)) ) _listaIterable;
};

//Implementacion
template<typename alpha>
diccNat<alpha>::crearDiccionario(Vector(tupla(Nat clave, alpha significado)) v){
  Nat i = 0;
  while(i < v.Longitud()){
    this->_tabla.AgregarAtras(Vector());
    i++;
  }
  i = 0;
  while(i < v.Longitud()){
    Nat k = (v[i].clave % v.Longitud()); //REVISAR TIPO TUPLA
    this->_tabla[k].AgregarAtras(v[i]);
    Nat q = this->_tabla[k].Longitud();
    this->_listaIterable.AgregarAtras(*(this->_tabla[k][q-1]));
    i++;
  }
}

template<typename alpha>
void diccNat<alpha>::redefinir(Nat n, alpha a){
  Nat k = (n % this->_tabla.Longitud());
  Lista<*(tupla(Nat clave, alpha significado))>::Iterador it = this->_tabla[k].CrearIt();
  while (it.HaySiguiente()){
  	if((*(it.Siguiente())).clave == n){
  		(*(it.Siguiente())).significado = a;
  	}
  	it.Avanzar();
  }
}

template<typename alpha>
*alpha diccNat<alpha>::obtener(Nat n){
	Nat k = (n % this->_tabla.Longitud());
	Lista<*(tupla(Nat clave, alpha significado))>::Iterador it = this->_tabla[k].CrearIt();
	*alpha res = NULL;
	while (it.HaySiguiente()){
  		if((*(it.Siguiente())).clave == n){
  			res = (*(it.Siguiente())).significado;
  		}
  	it.Avanzar();
  }
  return res;
}

template<typename alpha>
bool diccNat<alpha>::definido(Nat n){
	Nat k = (n % this->_tabla.Longitud());
	Lista<*(tupla(Nat clave, alpha significado))>::Iterador it = this->_tabla[k].CrearIt();
	while (it.HaySiguiente() && (*(it.Siguiente())).clave != n){
	  	it.Avanzar();
  	}
  	return (it.HaySiguiente() && (*(it.Siguiente())).clave == n);
}

template<typename alpha>
Nat diccNat<alpha>::cantClaves(){
	return this->_tabla.Longitud();
}

template<typename alpha>
typename diccNat<alpha>::itDiccNat diccNat<alpha>::crearIt(){
	_iteradorLista = this->_tabla.CrearIt();
	return _iteradorLista;
}

template<typename alpha>
bool diccNat<alpha>::itDiccNat::haySiguiente(){
	return _iteradorLista.HaySiguiente();
}

template<typename alpha>
tupla(const Nat, alpha) diccNat<alpha>::itDiccNat::siguiente(){
	return *(_iteradorLista.Siguiente());
}

template<typename alpha>
const alpha diccNat<alpha>::itDiccNat::siguienteSignificado(){
	return *(_iteradorLista.Siguiente().significado);
}


template<typename alpha>
void diccNat<alpha>::itDiccNat::avanzar(){
	_iteradorLista.Avanzar();
}