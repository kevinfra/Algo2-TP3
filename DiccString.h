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
			
			Iterador();
			bool HaySiguiente();
			bool HayAnterior();
			alpha& SiguienteSignificado();
			alpha& AnteriorSignificado();
			void Retroceder();
			void Avanzar();


		private:
	};

	private:

	struct Nodo{
		Arreglo<Nodo*> arbolTrie;
		alpha info;
		bool infoValida;
		Lista<Nodo*> infoEnLista;
	};

	Arreglo<Nodo*> raiz;
	Lista<Nodo*> listaIterable;

	bool tieneHermanos(const Nodo* nodo){
		int i = 0;
		int l = nodo->arbolTrie.Tamanho();
		while(i<l && !nodo->arbolTrie.Definido(i)){
			i++;
		}
		return i < l;
	}


	bool tieneHermanosEInfo(const Nodo* nodo){
		return tieneHermanos(nodo) && nodo->infoValida;
	}

	Nat ord(char caracter){
		switch(caracter) {
			case 'a':
				return 0;
				break;
			case 'b':
				return 1;
				break;
			case 'c':
				return 2;
				break;
			case 'd':
				return 3;
				break;
			case 'e':
				return 4;
				break;
			case 'f':
				return 5;
				break;
			case 'g':
				return 6;
				break;
			case 'h':
				return 7;
				break;
			case 'i':
				return 8;
				break;
			case 'j':
				return 9;
				break;
			case 'k':
				return 10;
				break;
			case 'l':
				return 11;
				break;
			case 'm':
				return 12;
				break;
		}
	}

};

template <typename alpha>
DiccString<alpha>::DiccString(){
	raiz = Arreglo<Nodo*>(26);
	//listaIterable = new Lista<Nodo*>;
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
		//typename Lista<Nodo*>::Iterador it = listaIterable.AgregarAdelante(NULL);
		arr->infoValida = true;
		//arr->infoEnLista = it;
		//it.Siguiente() = arr;
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
	while(i<c.size()-1){
		i++;
		letra = ord(c[i]);
		arr = arr->arbolTrie[letra];
		pila.AgregarAdelante(arr);
	}
	if(tieneHermanos(arr)){
		arr->infoValida = false;
	}
	else
	{
		typename Lista<Nodo*>::Iterador itPila = pila.CrearIt();
		i = 1;
		itPila.EliminarSiguiente();
		//itPila.Avanzar();
		while(itPila.HaySiguiente() && !tieneHermanosEInfo(itPila.Siguiente())){
			itPila.EliminarSiguiente();
			//itPila.Avanzar();
			i++;
		}
		if(i == c.size()-1){
			raiz.Borrar(ord(c[0]));
		}
	}

}

template <typename alpha>
Conj<String>::Iterador DiccString<alpha>::CrearItClaves(){
}

#endif
