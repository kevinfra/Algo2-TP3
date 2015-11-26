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

	Conj<Nombre>::Iterador it = campusSeguro.Estudiantes();
	ASSERT_EQ(it.Siguiente(), "manuelmena");

}

int main(int argc, char **argv)
{
    RUN_TEST(test_simple);
	RUN_TEST(test_constructor_campus_seguro);
	RUN_TEST(test_ingresar_estudiante);

	/********************************************************************
	 * TODO: escribir casos de test exhaustivos para todas              *
	 * las funcionalidades de cada módulo.                              *
     * La interacción con el TAD principal se debe hacer exclusivamente *
	 * a través de la interfaz del driver.                              *
	 ********************************************************************/

	return 0;
}
