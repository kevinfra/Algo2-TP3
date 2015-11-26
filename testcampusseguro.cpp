#include "Driver.h"
#include "mini_test.h"
#include "CampusSeguro.h"
#include "Campus.h"

#include <string>
#include <iostream>

using namespace aed2;
using namespace std;

// ---------------------------------------------------------------------

/**
 * Ejemplo de caso de test, con llamadas a las rutinas de aserción 
 * definidas en mini_test.h
 */
void test_simple(){
    ASSERT(true);
}

void test_constructor_campus_seguro(){
	Campus grilla1(10,10);
	Dicc<Agente,Posicion> dicc1;
	Agente a1;
	Posicion pos1;
	for(Nat i = 0; i < 5; i++){
		a1 = i;
		pos1.x = i;
		pos1.y = i;
		dicc1.Definir(a1,pos1);
	}
	CampusSeguro cs1(grilla1, dicc1);
	Campus griTest1 = cs1.campus();
	ASSERT(griTest1.Filas() == grilla1.Filas());
	ASSERT(griTest1.Columnas() == grilla1.Columnas());
	for(Nat i = 0; i < 10; i++){
		for(Nat j = 0; i < 10; i++){
			pos1.x = j;
			pos1.y = i;
			ASSERT(griTest1.Ocupada(pos1) == grilla1.Ocupada(pos1));
		}
	}
	Conj<Nombre>::Iterador itEst1 = cs1.Estudiantes();
	ASSERT(!itEst1.HaySiguiente());
	Conj<Nombre>::Iterador itHip1 = cs1.Hippies();
	ASSERT(!itHip1.HaySiguiente());
	ASSERT(cs1.MasVigilante() == 0);
	for(Nat i = 0; i < 5; i++){
		a1 = i;
		pos1.x = i;
		pos1.y = i;
		ASSERT(cs1.PosAgente(a1) == pos1);
		ASSERT(cs1.CantSanciones(a1) == 0);
		ASSERT(cs1.CantHippiesAtrapados(a1) == 0);
		Conj<Agente> conjMSan1 = cs1.ConMismasSanciones(a1);
		Conj<Agente> conjK1 = cs1.ConKSanciones(0);
		ASSERT(conjMSan1 == conjK1);
	}

}

void test_ingresar_estudiante(){
	Campus campus(10,10);
	Dicc<Agente,Posicion> dicc;

	Agente a;
	Posicion pos;

	for(Nat i = 1; i < 5; i++){
		a = i;
		pos.x = i;
		pos.y = i;
		dicc.Definir(a, pos);
	}

	CampusSeguro campusSeguro(campus, dicc);

	Nombre estudiante = "manuelmena";
	pos.x = 1;
	pos.y = 0;
	campusSeguro.IngresarEstudiante(estudiante, pos);

}

void test_ingresar_hippie(){
	//Test 1 hay 2 agentes y meto al hippie, el hippie no queda atrapado
	Campus campus(5,5);
	Posicion pos;
	pos.y = 0;
	pos.x = 0;
	campus.AgregarObstaculo(pos);
	pos.y = 4;
	pos.x = 0;
	campus.AgregarObstaculo(pos);
	pos.y = 3;
	pos.x = 2;
	campus.AgregarObstaculo(pos);
	pos.y = 1;
	pos.x = 3;
	campus.AgregarObstaculo(pos);
	pos.y = 1;
	pos.x = 4;
	campus.AgregarObstaculo(pos);	

	Dicc<Agente,Posicion> dicc;
	Agente a;
	a = 1;
	pos.y = 0;
	pos.x = 1;
	dicc.Definir(a,pos);
	a = 2;
	pos.y = 1;
	pos.x = 2;
	dicc.Definir(a,pos);
	CampusSeguro cs1(campus,dicc);

	Nombre h = "joel";
	pos.y = 0;
	pos.x = 2;
	cs1.IngresarHippie(h,pos);
	Conj<Nombre>::Iterador it1 = cs1.Hippies();
	ASSERT(it1.HaySiguiente());
	ASSERT(cs1.PosEstudianteYHippie(h) == pos);
	Conj<Agente> conj1 = cs1.ConKSanciones(0);
	ASSERT(conj1.Cardinal() == 2);
	ASSERT(conj1.Pertenece(1));
	ASSERT(conj1.Pertenece(2));
	ASSERT(0 == cs1.CantHippiesAtrapados(1));
	ASSERT(0 == cs1.CantHippiesAtrapados(2));
/*	
	//test2 hay tres agentes y meto al hippie en un lugar donde queda encerrado.
	a = 3;
	pos.y = 0;
	pos.x = 3;
	dicc.Definir(a,pos);
	CampusSeguro cs2(campus,dicc);
	pos.y = 0;
	pos.x = 2;
	//cs2.IngresarHippie(h,pos);
	//Conj<Nombre>::Iterador it2 = cs2.Hippies();
	//ASSERT(!it2.HaySiguiente());
	Conj<Agente> conj2 = cs2.ConKSanciones(0);
	ASSERT(conj2.Cardinal() == 3);
	ASSERT(conj2.Pertenece(1));
	ASSERT(conj2.Pertenece(2));
	ASSERT(conj2.Pertenece(3));
	ASSERT(0 == cs1.CantHippiesAtrapados(1));
	ASSERT(0 == cs1.CantHippiesAtrapados(2));
	ASSERT(0 == cs1.CantHippiesAtrapados(3));

*/
}

int main(int argc, char **argv)
{
    RUN_TEST(test_simple);
	RUN_TEST(test_constructor_campus_seguro);
	RUN_TEST(test_ingresar_estudiante);
	RUN_TEST(test_ingresar_hippie);

	/********************************************************************
	 * TODO: escribir casos de test exhaustivos para todas              *
	 * las funcionalidades de cada módulo.                              *
     * La interacción con el TAD principal se debe hacer exclusivamente *
	 * a través de la interfaz del driver.                              *
	 ********************************************************************/

	return 0;
}
