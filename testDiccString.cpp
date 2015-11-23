#include "DiccString.h"
#include <iostream>

using namespace std;
using namespace aed2;

int main(int argc, char **argv){
	DiccString<int> midic;
	midic.Definir("ab",2);
	if(midic.Definido("ab")){
		cout << "Esta definido!!" << endl;
	}
	else{
		cout << "No esta definido" << endl;
	}
	cout << midic.Obtener("ab") << endl;
	midic.Definir("ab",3);
	cout << midic.Obtener("ab") << endl;
	midic.Eliminar("ab");
	if(midic.Definido("ab")){
		cout << "Esta definido!!" << endl;
	}
	else{
		cout << "No esta definido" << endl;
	}
}