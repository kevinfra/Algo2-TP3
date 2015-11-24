#ifndef DICCSTRING_H_
#define DICCSTRING_H_

#include <iostream>
#include "Tipos.h"
#include "aed2/Lista.h"
#include "aed2/Conj.h"
#include "aed2/Arreglo.h"

using namespace aed2;

template<typename alpha>
class DiccString{

	public:

	class Iterador;

	struct Nodo{
		Arreglo<Nodo*> arbolTrie;
		alpha info;
		bool infoValida;
		typename Conj<String>::Iterador clave;
		typename Lista<Nodo*>::Iterador infoEnLista;
	};

	//CrearDiccionario
	DiccString();

	bool Definido(String c);
	void Definir(String c,alpha significado);
	alpha& Obtener(String c);
	void Eliminar(String c);
	Conj<String>::Iterador CrearItClaves();

	Iterador CrearIt();

	class Iterador{

		public:

			Iterador(Lista<Nodo*> &lista);
			bool HaySiguiente();
			bool HayAnterior();
			alpha& SiguienteSignificado();
			alpha& AnteriorSignificado();
			void Retroceder();
			void Avanzar();


		private: typename Lista<Nodo*>::Iterador iteradorLista;
	};

	private:

	Arreglo<Nodo*> raiz;
	Lista<Nodo*> listaIterable;
	Conj<String> conjIterable;

	bool tieneHermanos(const Nodo* nodo){
		int i = 0;
		int l = nodo->arbolTrie.Tamanho();
		while(i<l && !nodo->arbolTrie.Definido(i)){
			i++;
		}
		return i < l;
	}


	bool tieneHermanosEInfo(const Nodo* nodo){
		return tieneHermanos(nodo) || nodo->infoValida;
	}

	Nat ord(char caracter){
		return int(caracter) - 97;
	}

};

template <typename alpha>
DiccString<alpha>::DiccString(){
	raiz = Arreglo<Nodo*>(26);
}

template <typename alpha>
bool DiccString<alpha>::Definido(String c){
	Nat i = 0;
	Nat letra = ord(c[0]);
	if(raiz.Definido(letra)){
		Nodo* arr = raiz[letra];
		i++;
		letra = ord(c[i]);
		while(i<c.size()-1 && arr->arbolTrie.Definido(letra)){
			arr = arr->arbolTrie[letra];
			i++;
			letra = ord(c[i]);
		}
		return i==c.size()-1 && arr->arbolTrie.Definido(letra) && arr->arbolTrie[letra]->infoValida;
	}
	else
	{
		return false;
	}
}

template <typename alpha>
void DiccString<alpha>::Definir(String c,alpha significado){
	Nat i = 0;
	Nat letra = ord(c[0]);
	if(!raiz.Definido(letra)){
		Nodo* nodo = new Nodo();
		nodo->arbolTrie = Arreglo<Nodo*>(26);
		nodo->infoValida = false;
		raiz.Definir(letra,nodo);
	}
	Nodo* arr = raiz[letra];
	while(i<c.size()-1){
		i++;
		letra = ord(c[i]);
		if(!arr->arbolTrie.Definido(letra)){
			Nodo* nuevoHijo = new Nodo();
			nuevoHijo->arbolTrie = Arreglo<Nodo*>(26);
			nuevoHijo->infoValida = false;
			arr->arbolTrie.Definir(letra,nuevoHijo);
		}
		arr = arr->arbolTrie[letra];
	}
	arr->info = significado;
	if(!arr->infoValida){
		arr->infoValida = true;
		listaIterable.AgregarAdelante(arr);
		arr->clave = conjIterable.Agregar(c);
		typename Lista<Nodo*>::Iterador it = listaIterable.CrearIt();
		arr->infoEnLista = it;
	}
}

template <typename alpha>
alpha& DiccString<alpha>::Obtener(String c){
	Nat i = 0;
	Nat letra = ord(c[0]);
	Nodo* arr = raiz[letra];
	while(i<c.size()-1){
		i++;
		letra = ord(c[i]);
		arr = arr->arbolTrie[letra];
	}
	return arr->info;
}

template <typename alpha>
void DiccString<alpha>::Eliminar(String c){
	Nat i = 0;
	Nat letra = ord(c[0]);
	Nodo* arr = raiz[letra];
	Lista<Nodo*> pila;
	pila.AgregarAdelante(arr);
	while(i<c.size()-1){
		i++;
		letra = ord(c[i]);
		arr = arr->arbolTrie[letra];
		pila.AgregarAdelante(arr);
	}
	arr->clave.EliminarSiguiente();
	if(tieneHermanos(arr)){
		arr->infoValida = false;
		arr->infoEnLista.EliminarSiguiente();
	}
	else
	{
		typename Lista<Nodo*>::Iterador itPila = pila.CrearIt();
		i = 1;
		itPila.Siguiente()->infoEnLista.EliminarSiguiente();
		itPila.EliminarSiguiente();
		while(itPila.HaySiguiente() && !tieneHermanosEInfo(itPila.Siguiente())){
			itPila.EliminarSiguiente();
			i++;
		}
	}

}

template <typename alpha>
Conj<String>::Iterador DiccString<alpha>::CrearItClaves(){
	return conjIterable.CrearIt();
}

template<typename alpha>
typename DiccString<alpha>::Iterador DiccString<alpha>::CrearIt(){
  return Iterador(this->listaIterable);
}

template<typename alpha>
DiccString<alpha>::Iterador::Iterador(Lista<Nodo*> &lista)
  :     iteradorLista(lista.CrearIt())
{}

template<typename alpha>
bool DiccString<alpha>::Iterador::HaySiguiente(){
	return this->iteradorLista.HaySiguiente();
}

template<typename alpha>
bool DiccString<alpha>::Iterador::HayAnterior(){
	return this->iteradorLista.HayAnterior();
}


template<typename alpha>
alpha& DiccString<alpha>::Iterador::SiguienteSignificado(){
	return (this->iteradorLista.Siguiente())->info;
}

template<typename alpha>
alpha& DiccString<alpha>::Iterador::AnteriorSignificado(){
	return (this->iteradorLista.Anterior())->info;
}

template<typename alpha>
void DiccString<alpha>::Iterador::Avanzar(){
	this->iteradorLista.Avanzar();
}

template<typename alpha>
void DiccString<alpha>::Iterador::Retroceder(){
	this->iteradorLista.Retroceder();
}

#endif
