#ifndef DICCSTRING_H_
#define DICCSTRING_H_

#include <iostream>
#include "Tipos.h"
#include "aed2/Lista.h"
#include "aed2/Conj.h"

using namespace aed2;

template<typename alpha>
class DiccString{
	
	public:
	
	//CrearDiccionario
	DiccString();
	
	bool Definido(String c);
	bool Definir(String c);
	alpha& Obtener(String c);
	void Eliminar(String c);
	Conj<String>::Iterador CrearItClaves();

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

};

template <typename alpha>
DiccString<alpha>::DiccString(){
}

template <typename alpha>
bool DiccString<alpha>::Definido(String c){
	return false;
}

template <typename alpha>
bool DiccString<alpha>::Definir(String c){
	return false;
}

template <typename alpha>
alpha& DiccString<alpha>::Obtener(String c){
}

template <typename alpha>
void DiccString<alpha>::Eliminar(String c){
}

template <typename alpha>
Conj<String>::Iterador DiccString<alpha>::CrearItClaves(){
}

#endif
