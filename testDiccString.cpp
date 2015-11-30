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

void test_NombreSuperLargo(){
	DiccString<char> midic;
	midic.Definir("SuperNombreDeHippieEstudianteTodoJunto",'a');
	ASSERT_EQ(midic.Obtener("SuperNombreDeHippieEstudianteTodoJunto"),'a');
	midic.Eliminar("SuperNombreDeHippieEstudianteTodoJunto");
	ASSERT_EQ(midic.Definido("SuperNombreDeHippieEstudianteTodoJunto"),false);
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
	Posicion mipos2;
	mipos2.x = 1;
	mipos2.y = 1;
	Posicion mipos3;
	mipos3.x = 2;
	mipos3.y = 2;
	midic->Definir("?':-",mipos);
	midic->Definir("v",mipos);
	midic->Definir(";;;+++*}{",mipos);
	midic->Definir("?':",mipos2);
	midic->Definir("?':2",mipos3);
	midic->Definir("1",mipos);
	midic->Definir("2",mipos3);
	assert(midic->Definido(";;;+++*}{"));
	assert(midic->Obtener("?':-").x == mipos.x);
	assert(midic->Definido("?':-"));
	midic->Eliminar("?':-");
	midic->Eliminar("1");
	assert(midic->Obtener("?':2").y == 2);
	assert(!midic->Definido("?':-"));
	assert(midic->Definido("?':"));
	assert(midic->Definido("?':2"));
	delete midic;
}

void test_IteradorAFondo(){
	DiccString<Posicion> *midicfull = new DiccString<Posicion>();
	Posicion mipos;
	mipos.x = 0;
	mipos.y = 1;
	Posicion mipos2;
	mipos2.x = 1;
	mipos2.y = 1;
	Posicion mipos3;
	mipos3.x = 2;
	mipos3.y = 2;
	midicfull->Definir(".",mipos);
	midicfull->Definir("++",mipos2);
	midicfull->Definir("+.",mipos3);
	DiccString<Posicion>::Iterador it =  midicfull->CrearIt();
	int i = 0;
	assert(it.HaySiguiente());
	assert(it.SiguienteSignificado() == mipos3);
	while(it.HaySiguiente()){
		i++;
		it.Avanzar();
	}
	assert(it.HayAnterior());
	assert(i==3);
	assert(it.AnteriorSignificado() == mipos);
	it.Retroceder();
	assert(it.SiguienteSignificado() == mipos);
	assert(it.AnteriorSignificado() == mipos2);
	midicfull->Eliminar(".");
	DiccString<Posicion>::Iterador itNuevo =  midicfull->CrearIt();
	i = 0;
	assert(itNuevo.HaySiguiente());
	assert(itNuevo.SiguienteSignificado() == mipos3);
	while(itNuevo.HaySiguiente()){
		i++;
		itNuevo.Avanzar();
	}
	assert(i==2);
	it.Retroceder();
	delete midicfull;
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
  RUN_TEST(test_IteradorAFondo);
  RUN_TEST(test_NombreSuperLargo);
  return 0;
}
