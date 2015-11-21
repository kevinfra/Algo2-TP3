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
		
		Nodo(Nodo* &ab, const alpha& a, bool valida, typename Lista<Nodo*>::Iterador it){
		};
		
		Nodo* arbolTrie[26];
		alpha& info;
		bool infoValida;
		Lista<Nodo*> infoEnLista;
	};

	Nodo* raiz[26];
	Lista<Nodo*> listaIterable;

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
	raiz = new Arreglo<Nodo*>(26);
	listaIterable = new Lista<Nodo*>;
}

template <typename alpha>
bool DiccString<alpha>::Definido(String c){
	Nat i = 0;
	Nat letra = ord(c[0]);
	Nodo* arr = raiz[letra];
	while(i<c.size() && !arr == NULL){
		i++;
		letra = ord(c[i]);
		arr = arr->arbolTrie[letra];
	}
	if(i=c.size()){
		return arr->infoValida;
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
	if(raiz[letra] == NULL){
		Nodo nodo = Nodo();
		nodo.arbolTrie = new Arreglo<Nodo*>(26);
		nodo.infoValida = false;
		raiz[letra] = *nodo;
	}
	Nodo* arr = raiz[letra];
	while(i<c.size()){
		i++;
		letra = ord(c[i]);
		if(arr->arbolTrie[letra] == NULL){
			Nodo nuevoHijo = Nodo();
			nuevoHijo.arbolTrie = new Arreglo<Nodo*>(26);
			nuevoHijo.infoValida = false;
			arr->arbolTrie[letra] = *nuevoHijo;
		}
		arr = arr->arbolTrie[letra];
	}
	arr->info = significado;
	if(!arr->infoValida){
		//Lista<Nodo*>::Iterador it = listaIterable.AgregarAdelante(NULL);
		arr->infoValida = true;
	}

}

template <typename alpha>
alpha& DiccString<alpha>::Obtener(String c){
	Nat i = 0;
	Nat letra = ord(c[0]);
	Nodo* arr = raiz[letra];
	while(i<c.size()){
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
	//Pila<Nodo*> pil = new Pila<Nodo*>();

}

template <typename alpha>
Conj<String>::Iterador DiccString<alpha>::CrearItClaves(){
}

#endif
