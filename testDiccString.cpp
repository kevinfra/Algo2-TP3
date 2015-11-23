#include "DiccString.h"
#include <iostream>

using namespace std;
using namespace aed2;

int main(int argc, char **argv){
	DiccString<int> midic;
	midic.Definir("ab",2);
	midic.Definir("ab",1);
	midic.Definir("a",2);
	midic.Definir("ccc",3);
	DiccString<int>::Iterador it =  midic.CrearIt();

}