#include "DiccString.h"
#include <iostream>

using namespace std;
using namespace aed2;

int main(int argc, char **argv){
	DiccString<int> *midic = new DiccString<int>();
	midic->Definir("ab",2);
	midic->Definir("ab",1);
	midic->Definir("ab",3);
	midic->Definir("r",4);
	midic->Definir("hola",450);
	midic->Definir("jajajajajaja",70000);
	midic->Definir("asdsdsddsdsd",0);
	midic->Definir("bbbbbbbbbbbbbbbb",7);
	midic->Definir("bebe",2);
	midic->Eliminar("r");
	//midic.Eliminar("abc");
	//midic.Eliminar("abc");
	midic->Eliminar("ab");
	//midic.Eliminar("r");
	midic->Eliminar("hola");
	midic->Eliminar("jajajajajaja");
	//midic.Eliminar("ccc");
	DiccString<int>::Iterador it =  midic->CrearIt();
	while(it.HaySiguiente()){
		cout << it.SiguienteSignificado() << endl;
		it.Avanzar();
	}
	Conj<String>::Iterador itClave = midic->CrearItClaves();
		while(itClave.HaySiguiente()){
		cout << itClave.Siguiente() << endl;
		itClave.Avanzar();
	}
	delete midic;
}
