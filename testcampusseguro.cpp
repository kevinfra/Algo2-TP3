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
	ASSERT(griTest1.Filas() == 10);
	ASSERT(griTest1.Columnas() == 10);
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
	
	/*
	//Test 2 sin agentes
	Campus grilla2(3,3);
	Dicc<Agente,Posicion> dicc2;
	CampusSeguro cs2(grilla2, dicc2);
	*/
}

void test_ingresar_estudiante(){
	Campus campus(10,10);
	Posicion obs;

	obs.x = 0;
	obs.y = 0;
	campus.AgregarObstaculo(obs);

	obs.x = 1;
	obs.y = 1;
	campus.AgregarObstaculo(obs);

	Dicc<Agente,Posicion> dicc;

	Posicion pos;
	pos.x = 2;
	pos.y = 0;
	dicc.Definir(0, pos);

	CampusSeguro campusSeguro(campus, dicc);

	Nombre estudiante = "manuelmena";
	pos.x = 1;
	pos.y = 0;
	campusSeguro.IngresarEstudiante(estudiante, pos);

	Conj<Nombre>::Iterador it = campusSeguro.Estudiantes();
	ASSERT(it.Siguiente() == "manuelmena");

	ASSERT(campusSeguro.PosEstudianteYHippie("manuelmena") == pos);

	ASSERT_EQ(campusSeguro.CantSanciones(0), 1);
}

void test_ingresar_hippie(){
	Nombre h = "aba";
	Nombre e = "bab";

	/* Test 1 hay 2 agentes y meto al hippie, el hippie no queda atrapado
	 * los agentes tienen 0 sanciones y atrapados
	 */

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


	pos.y = 0;
	pos.x = 2;
	cs1.IngresarHippie(h,pos);
	Conj<Nombre>::Iterador it1 = cs1.Hippies();
	ASSERT(it1.HaySiguiente());
	ASSERT(cs1.PosEstudianteYHippie(h) == pos);
	Conj<Agente> conjK1 = cs1.ConKSanciones(0);
	ASSERT(conjK1.Cardinal() == 2);
	ASSERT(conjK1.Pertenece(1));
	ASSERT(conjK1.Pertenece(2));
	ASSERT(0 == cs1.CantHippiesAtrapados(1));
	ASSERT(0 == cs1.CantHippiesAtrapados(2));
	

	/*
	 * Test2 hay tres agentes y meto al hippie en un lugar donde queda encerrado.
	 * Esos 3 agentes tienen 0 sanciones y 1 cantidad de atrapados cada uno.
	 * El mas vigilante es uno de ellos.
	 */

	a = 3;
	pos.y = 0;
	pos.x = 3;
	dicc.Definir(a,pos);
	CampusSeguro cs2(campus,dicc);
	pos.y = 0;
	pos.x = 2;
	cs2.IngresarHippie(h,pos);
	Conj<Nombre>::Iterador it2 = cs2.Hippies();
	ASSERT(!it2.HaySiguiente());
	ASSERT(cs2.MasVigilante() == 1 || cs2.MasVigilante() == 2 || cs2.MasVigilante() == 3);
	Conj<Agente> conjK2 = cs2.ConKSanciones(0);
	ASSERT(conjK2.Cardinal() == 3);
	ASSERT(conjK2.Pertenece(1));
	ASSERT(conjK2.Pertenece(2));
	ASSERT(conjK2.Pertenece(3));
	Conj<Agente> conjMS2_a = cs2.ConMismasSanciones(1);
	Conj<Agente> conjMS2_b = cs2.ConMismasSanciones(2);
	Conj<Agente> conjMS2_c = cs2.ConMismasSanciones(3);
	ASSERT(conjK2 == conjMS2_a && conjK2 == conjMS2_b && conjK2 == conjMS2_c);
	ASSERT(1 == cs2.CantHippiesAtrapados(1));
	ASSERT(1 == cs2.CantHippiesAtrapados(2));
	ASSERT(1 == cs2.CantHippiesAtrapados(3));


	/*
	 * Test3 meto a un hippie y queda atrapado un estudiante.
	 * Hay dos agentes pero estan lejos asi que no se modifican.
	 * El estudiante deberia hacerse hippie.
	 */
	
	Dicc<Agente, Posicion> dicc3;
	a = 0;
	pos.y = 3;
	pos.x = 1;
	dicc3.Definir(a,pos);
	a = 1;
	pos.y = 3;
	pos.x = 3;
	dicc3.Definir(a,pos);
	CampusSeguro cs3(campus,dicc3);
	pos.y = 1;
	pos.x = 1;
	h = "lolo";
	cs3.PonerHippie(h,pos);
	pos.y = 2;
	pos.x = 2;
	h = "wololo";
	cs3.PonerHippie(h,pos);
	pos.y = 1;
	pos.x = 2;
	e = "luna";
	cs3.PonerEstudiante(e,pos);
	pos.y = 0;
	pos.x = 2;
	h = "hippie";
	cs3.IngresarHippie(h,pos);
	Conj<Nombre>::Iterador itHip3 = cs3.Hippies();
	ASSERT(itHip3.HaySiguiente());
	Nat i=0;
	while(itHip3.HaySiguiente()){
		ASSERT(itHip3.Siguiente() == "lolo" || itHip3.Siguiente() == "wololo" || itHip3.Siguiente() == "luna" || itHip3.Siguiente() == "hippie");
		i++;
		itHip3.Avanzar();	
	}
	//tienen que ser 4 los hippies ahora.
	ASSERT(i == 4);
	//no tiene que haber estudiantes
	Conj<Nombre>::Iterador itEst3 = cs3.Estudiantes();
	ASSERT(!itEst3.HaySiguiente());
	ASSERT(cs3.MasVigilante() == 0);
	Conj<Agente> conjK3 = cs3.ConKSanciones(0);
	ASSERT(conjK3.Cardinal() == 2);
	ASSERT(conjK3.Pertenece(1));
	ASSERT(conjK3.Pertenece(0));
	Conj<Agente> conjMS3_a = cs3.ConMismasSanciones(1);
	Conj<Agente> conjMS3_b = cs3.ConMismasSanciones(0);
	ASSERT(conjK3 == conjMS3_a && conjK3 == conjMS3_b);
	ASSERT(0 == cs3.CantHippiesAtrapados(1));
	ASSERT(0 == cs3.CantHippiesAtrapados(0));


	/*
	 * Test4 Meto un hippie y esto hace que otro hippie quede atrapado por dos
	 * agentes.
	 * El hippie atrapado desaparece y los agentes son premiados.
	 */

	Dicc<Agente, Posicion> dicc4;
	a = 0;
	pos.y = 2;
	pos.x = 3;
	dicc4.Definir(a,pos);
	a = 1;
	pos.y = 3;
	pos.x = 4;
	dicc4.Definir(a,pos);
	CampusSeguro cs4(campus,dicc4);
	pos.y = 3;
	pos.x = 3;
	h = "lolo";
	cs4.PonerHippie(h,pos);
	pos.y = 4;
	pos.x = 3;
	h = "wololo";
	cs4.IngresarHippie(h,pos);
	Conj<Nombre>::Iterador itHip4 = cs4.Hippies();
	ASSERT(itHip4.HaySiguiente());
	i=0;
	while(itHip4.HaySiguiente()){
		ASSERT(itHip4.Siguiente() == "wololo");
		i++;
		itHip4.Avanzar();	
	}
	//tienen que ser 1 los hippies ahora.
	ASSERT(i == 1);
	//no tiene que haber estudiantes
	Conj<Nombre>::Iterador itEst4 = cs4.Estudiantes();
	ASSERT(!itEst4.HaySiguiente());
	ASSERT(cs4.MasVigilante() == 0 || cs4.MasVigilante() == 1);
	Conj<Agente> conjK4 = cs4.ConKSanciones(0);
	ASSERT(conjK4.Cardinal() == 2);
	ASSERT(conjK4.Pertenece(1));
	ASSERT(conjK4.Pertenece(0));
	Conj<Agente> conjMS4_a = cs4.ConMismasSanciones(1);
	Conj<Agente> conjMS4_b = cs4.ConMismasSanciones(0);
	ASSERT(conjK4 == conjMS4_a && conjK4 == conjMS4_b);
	ASSERT(1 == cs4.CantHippiesAtrapados(1));
	ASSERT(1 == cs4.CantHippiesAtrapados(0));


	/*
	 * Test5 Meto un hippie y esto hace que un estudiante quede atrapado por dos
	 * agentes.
	 * Los agentes son sancionados.
	 */

	Dicc<Agente, Posicion> dicc5;
	a = 0;
	pos.y = 3;
	pos.x = 0;
	dicc5.Definir(a,pos);
	a = 1;
	pos.y = 2;
	pos.x = 1;
	dicc5.Definir(a,pos);
	CampusSeguro cs5(campus,dicc5);
	e = "cantar";
	pos.y = 3;
	pos.x = 1;
	cs5.PonerEstudiante(e,pos);
	h = "hippieohippy";
	pos.y = 4;
	pos.x = 1;
	cs5.IngresarHippie(h,pos);
	Conj<Nombre>::Iterador itHip5 = cs5.Hippies();
	ASSERT(itHip5.HaySiguiente());
	i=0;
	while(itHip5.HaySiguiente()){
		ASSERT(itHip5.Siguiente() == "wololo");
		i++;
		itHip5.Avanzar();	
	}
	//tienen que ser 1 los hippies ahora.
	ASSERT(i == 1);
	//no tiene que haber estudiantes
	Conj<Nombre>::Iterador itEst5 = cs5.Estudiantes();
	ASSERT(itEst5.HaySiguiente());
	i=0;
	while(itEst5.HaySiguiente()){
		ASSERT(itHip5.Siguiente() == "wololo");
		i++;
		itHip5.Avanzar();	
	}
	//la cantidad de estudiantes es 1
	ASSERT(i == 1);
	ASSERT(cs5.MasVigilante() == 0 || cs5.MasVigilante() == 1);
	Conj<Agente> conjK5 = cs5.ConKSanciones(1);
	ASSERT(conjK5.Cardinal() == 2);
	ASSERT(conjK5.Pertenece(1));
	ASSERT(conjK5.Pertenece(0));
	Conj<Agente> conjMS5_a = cs5.ConMismasSanciones(1);
	Conj<Agente> conjMS5_b = cs5.ConMismasSanciones(0);
	ASSERT(conjK5 == conjMS5_a && conjK5 == conjMS5_b);
	ASSERT(0 == cs5.CantHippiesAtrapados(1));
	ASSERT(0 == cs5.CantHippiesAtrapados(0));


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
