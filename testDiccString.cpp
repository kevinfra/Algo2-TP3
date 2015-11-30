#include "DiccString.h"
#include <iostream>
#include "Driver.h"
#include "mini_test.h"
#include "aed2.h"
#include "Tipos.h"

using namespace std;
using namespace aed2;


void test_DefinidoVacio(){
	DiccString<int> midic;
	ASSERT_EQ(midic.Definido(""),false);
}

void test_DefinidoValido(){
	DiccString<int> midic;
	midic.Definir("ab",3);
	ASSERT_EQ(midic.Definido("ab"),true);
}


void test_ObtenerDefinido(){
	DiccString<int> midic;
	midic.Definir("ab",3);
	ASSERT_EQ(midic.Obtener("ab"),3);
}

void test_Eliminar(){
	DiccString<int> midic;
	midic.Definir("abc",45);
	midic.Definir("ab",2);
	midic.Definir("a",1);
	midic.Definir("abd",4);
	ASSERT_EQ(midic.Definido("abc"),true);
	midic.Eliminar("abc");
	ASSERT_EQ(midic.Definido("abc"),false);
}

void test_iteradorClaves(){
	DiccString<int> midic;
	midic.Definir("ab",4);
	midic.Definir("rec",3);
	midic.Definir("jaja",2);
	midic.Definir("pajaro",1);
	DiccString<int>::Iterador it =  midic.CrearIt();
	int i = 0;
	while(it.HaySiguiente()){
		i++;
		it.Avanzar();
	}
	ASSERT_EQ(i,4);
}

void test_itDiccString(){
	DiccString<int> midic;
	midic.Definir("ab",4);
	midic.Definir("rec",3);
	midic.Definir("jaja",2);
	Conj<String>::Iterador itClave = midic.CrearItClaves();
	int i = 0;
		while(itClave.HaySiguiente()){
		i++;
		itClave.Avanzar();
	}
	ASSERT_EQ(i,3);
}

void test_definirYRedefinir(){
	DiccString<int> midic;
	midic.Definir("holz",5);
	assert(midic.Obtener("holz") == 5);
	midic.Definir("holz",9);
	assert(midic.Obtener("holz") != 5);
	assert(midic.Obtener("holz") == 9);
}

void test_MayusculasNumeros(){
	DiccString<int> midic;
	midic.Definir("Hola",2);
	assert(midic.Obtener("Hola") == 2);
	midic.Definir("Jaja123",9);
	assert(midic.Obtener("Jaja123") == 9);
	midic.Definir("Jaja123",1);
	assert(midic.Obtener("Jaja123") == 1);
}

void test_EliminarDePosicion(){
	DiccString<Posicion> midic;
	Posicion mipos;
	mipos.x = 2;
	mipos.y = 3;
	midic.Definir("huno",mipos);
	assert(midic.Obtener("huno").x == mipos.x);
	assert(midic.Definido("huno"));
	midic.Eliminar("huno");
	assert(!midic.Definido("huno"));
}

void test_SimbolosRaros(){
	DiccString<Posicion> *midic = new DiccString<Posicion>();
	Posicion mipos;
	mipos.x = 0;
	mipos.y = 1;
	midic->Definir("?':-",mipos);
	midic->Definir("v",mipos);
	midic->Definir(";;;+++*}{",mipos);
	assert(midic->Definido(";;;+++*}{"));
	assert(midic->Obtener("?':-").x == mipos.x);
	assert(midic->Definido("?':-"));
	midic->Eliminar("?':-");
	assert(!midic->Definido("?':-"));
	delete midic;
}


int main(int argc, char **argv)
{
  RUN_TEST(test_DefinidoVacio);
  RUN_TEST(test_DefinidoValido);
  RUN_TEST(test_ObtenerDefinido);
  RUN_TEST(test_Eliminar);
  RUN_TEST(test_iteradorClaves);
  RUN_TEST(test_itDiccString);
  RUN_TEST(test_definirYRedefinir);
  RUN_TEST(test_MayusculasNumeros);
  RUN_TEST(test_EliminarDePosicion);
  RUN_TEST(test_SimbolosRaros);
  return 0;
}
