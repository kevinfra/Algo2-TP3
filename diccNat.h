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
  crearDiccionario(Vector(tupla(Nat clave, alpha significado)));

  //Operaciones Basicas
  void redefinir(diccNat<alpha> d, Nat n, alpha a);
  alpha obtener(Nat n, diccNat<alpha> d);
  bool definido(Nat n, diccNat<alpha> d);
  Nat cantClaves(diccNat<alpha> d);
  Conj::Iterador crearItClaves(diccNat<alpha> d);

  //Operaciones del Iterador
  itDiccNat crearIt(diccNat<alpha> d);

  //Clase iterador de Diccionario
  class itDiccNat{
  public:
    bool haySiguiente(itDiccNat<alpha> it);
    tupla(const Nat, alpha) siguiente(itDiccNat<alpha> it);
    const alpha siguienteSignificado(itDiccNat<alpha> it);
    void avanzar(itDiccNat<alpha> it);

  private:
    Lista::Iterador _iteradorLista;
  };

private:
  Vector(Lista(tupla(Nat clave, alpha significado))) _tabla;
  Lista( *(tupla(Nat clave, alpha significado)) ) _listaIterable;
};

//Implementacion
