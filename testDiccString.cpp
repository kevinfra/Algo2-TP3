#include "DiccString.h"
#include <iostream>

using namespace std;
using namespace aed2;

int main(int argc, char **argv){
	DiccString<int> midic;
	midic.Definir("ab",2);
	midic.Definir("ab",1);
	midic.Definir("ab",3);
	//midic.Eliminar("ab");
	//midic.Eliminar("a");
	//midic.Eliminar("ccc");
	DiccString<int>::Iterador it =  midic.CrearIt();
	while(it.HaySiguiente()){
		cout << it.SiguienteSignificado() << endl;
		it.Avanzar();
	}
}
