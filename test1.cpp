#include "CampusSeguro.h"
#include "mini_test.h"
#include "Campus.h"

#include <string>
#include <iostream>

using namespace aed2;
using namespace std;



/**
 * Ejemplo de caso de test, con llamadas a las rutinas de aserción
 * definidas en mini_test.h
 */
void test_agregar_obstaculos() {
	Campus campus(10,10);

	Posicion p;
	p.x = 1;
	p.y = 2;

	campus.AgregarObstaculo(p);

	ASSERT(campus.Ocupada(p) == true);

	Posicion p2;
	p2.x = 0;
	p2.y = 0;

	ASSERT(campus.Ocupada(p2) == false);


}

void test_ingresa_estudiante(){
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);

	Posicion p2;
	p2.x = 0;
	p2.y = 0;

	Nombre s = "pepe";
	cs.IngresarEstudiante(s,p2);


	Posicion p3 = cs.PosEstudianteYHippie(s);
	ASSERT(p3.x == p2.x && p3.y == p3.x);
}

// DIFICIL
void test_ingresa_hippie_y_convierte() {
	// Testea que cuando ingresa un hippie y esta rodeado de estudiantes se convierte
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);

	Posicion p1;
	p1.x = 0;
	p1.y = 0;

	Posicion p2;
	p2.x = 1;
	p2.y = 0;


	Posicion p3;
	p3.x = 2;
	p3.y = 0;

	Nombre s1 = "pepe";
	Nombre s2 = "pepo";
	Nombre s3 = "pepa";

	// Ingreso 3 estudiantes uno al lado del otro
	cs.IngresarEstudiante(s1,p1);
	cs.IngresarEstudiante(s2,p2);
	cs.IngresarEstudiante(s3,p3);

	// Avanzo el estudiante del medio
	cs.MoverEstudiante(s2,abajo);

	// Ahora hago ingresar un hippie,se tiene que convertir a estudiante
	Nombre h1 = "wololoHippie";
	cs.IngresarHippie(h1,p2);
	ASSERT(cs.cantEstudiantes() == 4);
	ASSERT(cs.cantHippies() == 0);
}

// DIFICIL
void test_mueve_estudiante_y_convierte() {
	// Testea que cuando se mueve un estudiante y rodea a un hippie se convierte
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);

	Posicion p1;
	p1.x = 0;
	p1.y = 0;

	Posicion p2;
	p2.x = 1;
	p2.y = 0;


	Posicion p3;
	p3.x = 2;
	p3.y = 0;

	Nombre s1 = "pepe";
	Nombre s2 = "pepo";
	Nombre s3 = "pepa";

	// Ingreso 3 estudiantes uno al lado del otro
	cs.IngresarEstudiante(s1,p1);
	cs.IngresarEstudiante(s2,p2);
	cs.IngresarEstudiante(s3,p3);

	// Avanzo el estudiante del medio
	cs.MoverEstudiante(s2,abajo);
	cs.MoverEstudiante(s2,abajo);

	// Ahora hago ingresar un hippie Y NO SE TIENE QUE CONVERTIR
	Nombre h1 = "wololoHippie";
	cs.IngresarHippie(h1,p2);

	ASSERT(cs.cantEstudiantes() == 3);
	ASSERT(cs.cantHippies() == 1);

	// Muevo el estudiante hacia arriba y tiene que convertir talannnn
	cs.MoverEstudiante(s2,arriba);

	ASSERT(cs.cantEstudiantes() == 4);
	ASSERT(cs.cantHippies() == 0);

}

void test_mover_estudiante() {
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);

	Posicion p2;
	p2.x = 0;
	p2.y = 0;

	Nombre s = "pepe";
	cs.IngresarEstudiante(s,p2);
	cs.MoverEstudiante(s, abajo);

	Posicion p3;
	p3.x = 0;
	p3.y = 1;


	Posicion p = cs.PosEstudianteYHippie(s);
	ASSERT(p3.x == p.x && p3.y == p.y);

}

void test_mover_estudiante_fuera() {
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);

	Posicion p2;
	p2.x = 0;
	p2.y = 0;

	Nombre s = "pepe";
	cs.IngresarEstudiante(s,p2);
	cs.MoverEstudiante(s, arriba);


	ASSERT(cs.cantEstudiantes() == 0);
}

void test_ingresa_hippie() {
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);

	Posicion p2;
	p2.x = 0;
	p2.y = 0;

	Nombre s = "pepe";
	cs.IngresarHippie(s,p2);


	Posicion p = cs.PosEstudianteYHippie(s);
	ASSERT(p2.x == p.x && p2.y == p.y);

}

void test_ingresa_hippie_y_estudiante() {
	// Idea de este test es probar el funcionamiento del Trie
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);

	Posicion p2;
	p2.x = 0;
	p2.y = 0;

	Nombre s = "pepe";
	cs.IngresarHippie(s,p2);

	Posicion p3;
	p3.x = 0;
	p3.y = 1;

	Nombre t = "pepa";
	cs.IngresarEstudiante(t,p3);

	Posicion p;
	p.x = 0;
	p.y = 2;

	Nombre r = "pepen";
	cs.IngresarEstudiante(r,p);


	ASSERT(cs.iesimoHippie(0) == s);

	// No importa el orden, pero tienen que estar los dos
	ASSERT(cs.iesimoEstudiante(0) == t || cs.iesimoEstudiante(1) == t);
	ASSERT(cs.iesimoEstudiante(0) == r || cs.iesimoEstudiante(1) == r);

}


void test_mover_hippie_a_estudiante() {
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);

	Posicion p;
	p.x = 0;
	p.y = 0;

	Nombre t = "pepa";
	cs.IngresarEstudiante(t,p);

	cs.MoverEstudiante(t, abajo);
	cs.MoverEstudiante(t, abajo);
	cs.MoverEstudiante(t, abajo);
	cs.MoverEstudiante(t, abajo);
	cs.MoverEstudiante(t, abajo);

	// Baja el estudiante
	Posicion p3 = cs.PosEstudianteYHippie(t);
	ASSERT(p3.x == 0 && p3.y == 5);

	Posicion p2;
	p2.x = 0;
	p2.y = 0;

	Nombre s = "pepe";
	cs.IngresarHippie(s,p2);
	cs.MoverHippie(s);

	// El hippie se mueve hacia abajo
	Posicion p4 = cs.PosEstudianteYHippie(s);
	ASSERT(p4.x == 0 && p4.y == 1);

	Posicion p5;
	p5.x = 2;
	p5.y = 0;

	Nombre r = "pepo";
	cs.IngresarEstudiante(r,p5);

	// El hippie se mueve hacia el nuevo estudiante
	cs.MoverHippie(s);
	p4 = cs.PosEstudianteYHippie(s);
	ASSERT((p4.x == 0 && p4.y == 0) || (p4.x == 1 && p4.y == 1) );

}


void test_mover_hippie_a_ingreso() {
	// Idea del test: el hippie se mueve hacia el estudiante
	// Luego el estudiante se borra
	// Luego el hippie se mueve al ingreso mas cercano

	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	CampusSeguro cs(campus, agentes);


	Posicion p;
	p.x = 0;
	p.y = 0;
	Nombre t = "pepa";
	cs.IngresarEstudiante(t,p);

	for (int i=0; i < 5; i++) {
		cs.MoverEstudiante(t, abajo);
	}

	// Baja el estudiante
	Posicion p3 = cs.PosEstudianteYHippie(t);
	ASSERT(p3.x == 0 && p3.y == 5);

	Posicion p2;
	p2.x = 0;
	p2.y = 0;

	Nombre s = "pepe";
	cs.IngresarHippie(s,p2);
	cs.MoverHippie(s);
	cs.MoverHippie(s);

	// El hippie se mueve hacia abajo
	Posicion p4 = cs.PosEstudianteYHippie(s);
	ASSERT(p4.x == 0 && p4.y == 2);

	// Hago salir al estudiante, lo muevo a la derecha para no pisar el hippie
	cs.MoverEstudiante(t, der);
	for (int i=0; i < 6; i++) {
		cs.MoverEstudiante(t, arriba);
	}

	ASSERT(cs.cantEstudiantes() == 0);

	// Muevo al hippie , tiene que ir al ingreso mas cercano
	cs.MoverHippie(s);

	p4 = cs.PosEstudianteYHippie(s);
	// FIXME: esta linea esta comentada porque el tad no pide que los hippies se muevan hacia las entradas sino los agentes
//	ASSERT(p4.x == 0 && p4.y == 1);

}


void test_comenzar_rastrillaje_simple() {
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	Agente a = 1;

	Posicion p;
	p.x = 0;
	p.y = 0;

	agentes.Definir(a,p);

	CampusSeguro cs(campus, agentes);
	ASSERT(cs.cantAgentes() == 1);
}


void test_rastrillaje_mover_hacia_hippie() {
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	Agente a = 1;

	Posicion p;
	p.x = 0;
	p.y = 0;

	agentes.Definir(a,p);

	CampusSeguro cs(campus, agentes);

	Posicion ph;
	ph.x = 4;
	ph.y = 0;

	Nombre h = "hippie";
	cs.IngresarHippie(h,ph);

	cs.MoverAgente(a);

	Posicion p4 = cs.PosAgente(a);

	ASSERT(p4.x == 1 && p4.y == 0);

}


void test_captura_hippie_entre_agentes()
{
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	Agente a = 1;
	Agente a2 = 2;
	Agente a3 = 3;

	Posicion p;
	p.x = 0;
	p.y = 0;

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	Posicion p3;
	p3.x = 2;
	p3.y = 0;

	agentes.Definir(a,p);
	agentes.Definir(a2,p2);
	agentes.Definir(a3,p3);

	CampusSeguro cs(campus, agentes);

	Posicion ph;
	ph.x = 1;
	ph.y = 0;

	Nombre h = "hippie";
	cs.IngresarHippie(h,ph);

	ASSERT(cs.CantHippiesAtrapados(a) == 1);
	ASSERT(cs.CantHippiesAtrapados(a2) == 1);
	ASSERT(cs.CantHippiesAtrapados(a3) == 1);

//Esto es agregado
	for(Nat i = 0; i < 7; i++){
		cs.IngresarHippie(h,ph);
		ASSERT(cs.CantHippiesAtrapados(a) == i+2);
	}

	ASSERT(cs.MasVigilante() == a || cs.MasVigilante() == a2 || cs.MasVigilante() == a3);

}

void test_captura_estudiante()
{
	Campus campus(10,10);

	Dicc<Agente,Posicion> agentes;
	Agente a = 1;
	Agente a2 = 2;
	Agente a3 = 3;

	Posicion p;
	p.x = 0;
	p.y = 0;

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	Posicion p3;
	p3.x = 2;
	p3.y = 0;

	agentes.Definir(a,p);
	agentes.Definir(a2,p2);
	agentes.Definir(a3,p3);

	CampusSeguro cs(campus, agentes);

	Posicion pe;
	pe.x = 1;
	pe.y = 0;

	Nombre e = "pepe";
	std::cout << e << std::endl;
	cs.IngresarEstudiante(e,pe);

	ASSERT(cs.ConMismasSanciones(a).Cardinal() == 3 );
	ASSERT(cs.ConMismasSanciones(a2).Cardinal() == 3 );
	ASSERT(cs.ConMismasSanciones(a3).Cardinal() == 3 );
	Nat c = cs.ConKSanciones(1).Cardinal();
	ASSERT(c == 3);

	Nat c2 = cs.ConKSanciones(0).Cardinal();
	ASSERT(c2 == 0 );


}

void test_mas_vigilante()
{
// 3 primeras lineas antes de que ingresen los estudiantes
//     1   2   3   4   5
// 1 | A |   | A |   | O
// 2 |   | A |   | O |
// 3 ...
// Los hippies ingresan en (2,1) y (4,1) los huecos. El Agente en (3,1) se transforma en el mas vigilante
	Campus campus(10,10);

	Posicion o;
	o.x = 3;
	o.y = 1;

	campus.AgregarObstaculo(o);

	Posicion o2;
	o2.x = 4;
	o2.y = 0;

	campus.AgregarObstaculo(o2);


	Dicc<Agente,Posicion> agentes;
	Agente a = 1;
	Agente a2 = 2;
	Agente a3 = 3;

	Posicion p;
	p.x = 0;
	p.y = 0;

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	Posicion p3;
	p3.x = 2;
	p3.y = 0;

	agentes.Definir(a,p);
	agentes.Definir(a2,p2);
	agentes.Definir(a3,p3);

	CampusSeguro cs(campus, agentes);

	Posicion ph;
	ph.x = 1;
	ph.y = 0;

	Nombre h = "hippie";
	cs.IngresarHippie(h,ph);

	Posicion ph2;
	ph2.x = 3;
	ph2.y = 0;

	Nombre h2 = "hippie2";
	cs.IngresarHippie(h2,ph2);

	ASSERT(cs.CantHippiesAtrapados(a) == 1);
	ASSERT(cs.CantHippiesAtrapados(a2) == 1);
	std::cout << cs.CantHippiesAtrapados(a3) << std::endl;
	ASSERT(cs.CantHippiesAtrapados(a3) == 2);

	ASSERT(cs.MasVigilante() == a3);

}


int main(int argc, char **argv)
{
  RUN_TEST(test_agregar_obstaculos);
	RUN_TEST(test_ingresa_estudiante);
	RUN_TEST(test_mover_estudiante);
	RUN_TEST(test_mover_estudiante_fuera);
	RUN_TEST(test_ingresa_hippie);
	RUN_TEST(test_ingresa_hippie_y_estudiante);
	RUN_TEST(test_mover_hippie_a_estudiante);
	RUN_TEST(test_mover_hippie_a_ingreso);
//	RUN_TEST(test_ingresa_hippie_y_convierte);
//	RUN_TEST(test_mueve_estudiante_y_convierte);
	RUN_TEST(test_comenzar_rastrillaje_simple);
	RUN_TEST(test_rastrillaje_mover_hacia_hippie);
	RUN_TEST(test_captura_hippie_entre_agentes);
//	RUN_TEST(test_captura_estudiante);
	RUN_TEST(test_mas_vigilante);

	/********************************************************************
	 * TODO: escribir casos de test exhaustivos para todas              *
	 * las funcionalidades de cada módulo.                              *
   * La interacción con el TAD principal se debe hacer exclusivamente *
	 * a través de la interfaz del driver.                              *
	 ********************************************************************/

	return 0;
}
