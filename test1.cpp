#include "Driver.h"
#include "mini_test.h"

#include <string>
#include <iostream>

using namespace aed2;


// ---------------------------------------------------------------------

/**
 * Ejemplo de caso de test, con llamadas a las rutinas de aserción
 * definidas en mini_test.h
 */
void test_agregar_obstaculos() {
	Driver campus;
	campus.crearCampus(10,10);
	//Dicc<Agente,Posicion> agentes;
	//campus.comenzarRastrillaje(agentes); Para que comienza rastrillaje si solo hace cosas de campus?

	Posicion p;
	p.x = 2;
	p.y = 3;
	campus.agregarObstaculo(p);

	ASSERT(campus.ocupada(p) == true);

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	ASSERT(campus.ocupada(p2) == false);


}

void test_ingresa_estudiante(){
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	Posicion p0;
	p0.x = 5;
	p0.y = 5;
	agentes.Definir(5,p0);
	campus.comenzarRastrillaje(agentes);

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	Nombre s = "pepe";
	campus.ingresarEstudiante(s,p2);

	ASSERT(campus.ocupada(p2) == false); //cambiado por no tener sentido

	Posicion p3 = campus.posEstudianteYHippie(s);
	ASSERT(p3.x == p2.x && p3.y == p3.x);
}

// DIFICIL --- CORRIJO EL TEST YA QUE NECESITA ESTAR RODEADO DE 4 ESTUDIANTES, NO DE 3
void test_ingresa_hippie_y_convierte() {
	// Testea que cuando ingresa un hippie y esta rodeado de estudiantes se convierte  --> ESTO NO PASA. SOLO CUANDO SE RODEA DE 4 ESTUDIANTES.
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	// Posicion pAgente;
	// pAgente.x = 9;
	// pAgente.y = 9;
	// agentes.Definir(0,pAgente);
	campus.comenzarRastrillaje(agentes);

	//Las posiciones para entrar son en 0 y van para arriba
	Posicion p1;
	p1.x = 1;
	p1.y = 0;

	Posicion p2;
	p2.x = 2;
	p2.y = 0;


	Posicion p3;
	p3.x = 3;
	p3.y = 0;

	Nombre s1 = "pepe";
	Nombre s2 = "pepo";
	Nombre s3 = "pepa";
	Nombre s4 = "mclovin";

	// Ingreso 3 estudiantes uno al lado del otro
	campus.ingresarEstudiante(s2,p2);
	campus.moverEstudiante(s2,arriba);
	campus.moverEstudiante(s2,arriba);
	// Avanzo el estudiante del medio -- NO! Avanzo TODOS los estudiantes hacia arriba y una vez mas el del medio
	//Siendo que primero entra el del medio, luego avanza dos veces, luego entra el hippie y luego rodean al hippie los otros estudiantes
	// Ahora hago ingresar un hippie,se tiene que convertir a estudiante
	Nombre h1 = "wololohippie";
	campus.ingresarHippie(h1,p2);
	campus.moverHippie(h1);

	campus.ingresarEstudiante(s1,p1);
	campus.moverEstudiante(s1,arriba);
	campus.ingresarEstudiante(s3,p3);
	campus.moverEstudiante(s3,arriba);
	campus.ingresarEstudiante(s4,p2);
	campus.moverEstudiante(s4,arriba);
	ASSERT(campus.cantEstudiantes() == 5);
	ASSERT(campus.cantHippies() == 0);
}

// DIFICIL
void test_mueve_estudiante_y_convierte() {
	// Testea que cuando se mueve un estudiante y rodea a un hippie se convierte
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	campus.comenzarRastrillaje(agentes);

	Posicion p1;
	p1.x = 1;
	p1.y = 1;

	Posicion p2;
	p2.x = 2;
	p2.y = 1;


	Posicion p3;
	p3.x = 3;
	p3.y = 1;

	Nombre s1 = "pepe";
	Nombre s2 = "pepo";
	Nombre s3 = "pepa";

	// Ingreso 3 estudiantes uno al lado del otro
	campus.ingresarEstudiante(s1,p1);
	campus.ingresarEstudiante(s2,p2);
	campus.ingresarEstudiante(s3,p3);

	// Avanzo el estudiante del medio
	campus.moverEstudiante(s2,abajo);
	campus.moverEstudiante(s2,abajo);

	// Ahora hago ingresar un hippie Y NO SE TIENE QUE CONVERTIR
	Nombre h1 = "wololoHippie";
	campus.ingresarHippie(h1,p2);

	ASSERT(campus.cantEstudiantes() == 3);
	ASSERT(campus.cantHippies() == 1);

	// Muevo el estudiante hacia arriba y tiene que convertir talannnn
	campus.moverEstudiante(s2,arriba);

	ASSERT(campus.cantEstudiantes() == 4);
	ASSERT(campus.cantHippies() == 0);

}

void test_mover_estudiante() {
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	campus.comenzarRastrillaje(agentes);

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	Nombre s = "pepe";
	campus.ingresarEstudiante(s,p2);
	campus.moverEstudiante(s, abajo);

	Posicion p3;
	p3.x = 1;
	p3.y = 0;

	ASSERT(campus.ocupada(p2) == false);
	ASSERT(campus.ocupada(p3) == false); //Este assert se cambió porque ocupada solo dice si hay un obstaculo

	Posicion p = campus.posEstudianteYHippie(s);
	ASSERT(p3.x == p.x && p3.y == p.y);

}

void test_mover_estudiante_fuera() {
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	campus.comenzarRastrillaje(agentes);

	Posicion p2;
	p2.x = 1;
	p2.y = 0; //CAMBIO ESTO PORQUE TIENE QUE ENTRAR POR 0 O POR MAX.FILA

	Nombre s = "pepe";
	campus.ingresarEstudiante(s,p2);
	campus.moverEstudiante(s, abajo); //CAMBIO ARRIBA POR ABAJO PORQUE EN EL NUESTRO SALE POR 0, NO POR 1 Y 0 ES ABAJO.


	ASSERT(campus.ocupada(p2) == false);
	ASSERT(campus.cantEstudiantes() == 0);
}

void test_ingresa_hippie() {
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	campus.comenzarRastrillaje(agentes);

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	Nombre s = "pepe";
	campus.ingresarHippie(s,p2);

	ASSERT(campus.ocupada(p2) == false); //OCUPADA ES DE CAMPUS, NO DE CAMPUSSEGURO

	Posicion p = campus.posEstudianteYHippie(s);
	ASSERT(p2.x == p.x && p2.y == p.y);

}

void test_ingresa_hippie_y_estudiante() {
	// Idea de este test es probar el funcionamiento del Trie
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	campus.comenzarRastrillaje(agentes);

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	Nombre s = "pepe";
	campus.ingresarHippie(s,p2);

	Posicion p3;
	p3.x = 1;
	p3.y = 2;

	Nombre t = "pepa";
	campus.ingresarEstudiante(t,p3);

	Posicion p;
	p.x = 1;
	p.y = 3;

	Nombre r = "pepen";
	campus.ingresarEstudiante(r,p);


	ASSERT(campus.iesimoHippie(0) == s);

	// No importa el orden, pero tienen que estar los dos
	ASSERT(campus.iesimoEstudiante(0) == t || campus.iesimoEstudiante(1) == t);
	ASSERT(campus.iesimoEstudiante(0) == r || campus.iesimoEstudiante(1) == r);

}


void test_mover_hippie_a_estudiante() {
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	campus.comenzarRastrillaje(agentes);

	Posicion p;
	p.x = 1;
	p.y = 0; //entra por 0, no por 1

	Nombre t = "pepa";
	campus.ingresarEstudiante(t,p);

	campus.moverEstudiante(t, arriba);
	campus.moverEstudiante(t, arriba);
	campus.moverEstudiante(t, arriba);
	campus.moverEstudiante(t, arriba);
	campus.moverEstudiante(t, arriba); //CAMBIE TODOS LOS ABAJOS POR ARRIBAS

	// Sube el estudiante
	Posicion p3 = campus.posEstudianteYHippie(t);
	ASSERT(p3.x == 1 && p3.y == 5);

	Posicion p2;
	p2.x = 1;
	p2.y = 0; //entra por 0, no por 1

	Nombre s = "pepe";
	campus.ingresarHippie(s,p2);
	campus.moverHippie(s);

	// El hippie se mueve hacia arriba
	Posicion p4 = campus.posEstudianteYHippie(s);
	ASSERT(p4.x == 1 && p4.y == 1);

	Posicion p5;
	p5.x = 3;
	p5.y = 0; //entra por 0, no por 1

	Nombre r = "pepo";
	campus.ingresarEstudiante(r,p5);
	// El hippie se mueve hacia el nuevo estudiante
	campus.moverHippie(s);
	p4 = campus.posEstudianteYHippie(s);
	ASSERT((p4.x == 1 && p4.y == 0) || (p4.x == 2 && p4.y == 1) );
	//MODIFIQUE LOS ASSERTS PARA QUE LOS POS.Y SEAN POS.Y-1
}


void test_mover_hippie_a_ingreso() {
	// Idea del test: el hippie se mueve hacia el estudiante
	// Luego el estudiante se borra
	// Luego el hippie se mueve al ingreso mas cercano

	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	campus.comenzarRastrillaje(agentes);


	Posicion p;
	p.x = 1;
	p.y = 1;

	Nombre t = "pepa";
	campus.ingresarEstudiante(t,p);

	for (int i=0; i < 5; i++) {
		campus.moverEstudiante(t, arriba); //EL ESTUDIANTE VA PAR ARRIBA! NO PARA ABAJO!
	}

	// Baja el estudiante
	Posicion p3 = campus.posEstudianteYHippie(t);
	ASSERT(p3.x == 1 && p3.y == 6);

	Posicion p2;
	p2.x = 1;
	p2.y = 1;

	Nombre s = "pepe";
	campus.ingresarHippie(s,p2);
	campus.moverHippie(s);
	campus.moverHippie(s);
	// El hippie se mueve hacia abajo
	Posicion p4 = campus.posEstudianteYHippie(s);
	ASSERT(p4.x == 1 && p4.y == 3);

	// Hago salir al estudiante, lo muevo a la derecha para no pisar el hippie
	campus.moverEstudiante(t, der);
	for (int i=0; i < 7; i++) { //COMO ARRANCA EN UNA POSICION MAS DE LA QUE DEBERIA ENTRAR, TIENE QUE MOVERSE 7 VECES EN LUGAR DE 6 PARA LLEGAR A LA SALIDA
		campus.moverEstudiante(t, abajo); //SE MUEVE HACIA ABAJO, NO HACIA ARRIBA!
	}

	ASSERT(campus.cantEstudiantes() == 0);

	// Muevo al hippie , tiene que ir al ingreso mas cercano -- NO! SE TIENE QUE QUEDAR DONDE ESTA!
	campus.moverHippie(s);

	p4 = campus.posEstudianteYHippie(s);

	ASSERT(p4.x == 1 && p4.y == 3); //SE QUEDA DONDE ESTA!

}


void test_comenzar_rastrillaje_simple() {
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	Agente a = 1;

	Posicion p;
	p.x = 1;
	p.y = 1;

	agentes.Definir(a,p);

	campus.comenzarRastrillaje(agentes);
	ASSERT(campus.cantAgentes() == 1);
}


void test_rastrillaje_mover_hacia_hippie() {
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	Agente a = 1;
	Agente b = 2;

	Posicion p;
	p.x = 1;
	p.y = 1;

	Posicion pdos;
	pdos.x = 4;
	pdos.y = 4;

	Nombre h = "aaaaaa";
	agentes.Definir(a,p);
	agentes.Definir(b,pdos);

	campus.comenzarRastrillaje(agentes);

	Posicion ph;
	ph.x = 5;
	ph.y = 1;

	//std::cout << campus.masVigilante() << std::endl;
	campus.ingresarHippie(h,ph);
	campus.moverAgente(a);

	std::cout << "rompe en posAgente" << std::endl;
	Posicion p4 = campus.posAgente(a);

	ASSERT(p4.x == 2 && p4.y == 1);

}


void test_captura_hippie_entre_agentes()
{
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	Agente a = 1;
	Agente a2 = 2;
	Agente a3 = 3;

	Posicion p;
	p.x = 1;
	p.y = 1;

	Posicion p2;
	p2.x = 2;
	p2.y = 2;

	Posicion p3;
	p3.x = 3;
	p3.y = 1;

	agentes.Definir(a,p);
	agentes.Definir(a2,p2);
	agentes.Definir(a3,p3);

	campus.comenzarRastrillaje(agentes);

	Posicion ph;
	ph.x = 2;
	ph.y = 1;

	Nombre h = "hippie";
	campus.ingresarHippie(h,ph);

	ASSERT(campus.cantHippiesAtrapados(a) == 1);
	ASSERT(campus.cantHippiesAtrapados(a2) == 1);
	ASSERT(campus.cantHippiesAtrapados(a3) == 1);

	ASSERT(campus.masVigilante() == a || campus.masVigilante() == a2 || campus.masVigilante() == a3);

}

void test_captura_estudiante()
{
	Driver campus;
	campus.crearCampus(10,10);

	Dicc<Agente,Posicion> agentes;
	Agente a = 1;
	Agente a2 = 2;
	Agente a3 = 3;

	Posicion p;
	p.x = 1;
	p.y = 1;

	Posicion p2;
	p2.x = 2;
	p2.y = 2;

	Posicion p3;
	p3.x = 3;
	p3.y = 1;

	agentes.Definir(a,p);
	agentes.Definir(a2,p2);
	agentes.Definir(a3,p3);

	campus.comenzarRastrillaje(agentes);

	Posicion pe;
	pe.x = 2;
	pe.y = 1;

	Nombre e = "pepe";
	campus.ingresarEstudiante(e,pe);

	ASSERT(campus.conMismasSanciones(a).Cardinal() == 3 );
	ASSERT(campus.conMismasSanciones(a2).Cardinal() == 3 );
	ASSERT(campus.conMismasSanciones(a3).Cardinal() == 3 );
	Nat c = campus.conKSanciones(1).Cardinal();
	ASSERT(c == 3);

	Nat c2 = campus.conKSanciones(0).Cardinal();
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
	Driver campus;
	campus.crearCampus(10,10);

	Posicion o;
	o.x = 4;
	o.y = 2;

	campus.agregarObstaculo(o);

	Posicion o2;
	o2.x = 5;
	o2.y = 1;

	campus.agregarObstaculo(o2);


	Dicc<Agente,Posicion> agentes;
	Agente a = 1;
	Agente a2 = 2;
	Agente a3 = 3;

	Posicion p;
	p.x = 1;
	p.y = 1;

	Posicion p2;
	p2.x = 2;
	p2.y = 2;

	Posicion p3;
	p3.x = 3;
	p3.y = 1;

	agentes.Definir(a,p);
	agentes.Definir(a2,p2);
	agentes.Definir(a3,p3);

	campus.comenzarRastrillaje(agentes);

	Posicion ph;
	ph.x = 2;
	ph.y = 1;

	Nombre h = "hippie";
	campus.ingresarHippie(h,ph);

	Posicion ph2;
	ph2.x = 4;
	ph2.y = 1;

	Nombre h2 = "hippie2";
	campus.ingresarHippie(h2,ph2);

	ASSERT(campus.cantHippiesAtrapados(a) == 1);
	ASSERT(campus.cantHippiesAtrapados(a2) == 1);
	ASSERT(campus.cantHippiesAtrapados(a3) == 2);

	ASSERT(campus.masVigilante() == a3);

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
	RUN_TEST(test_ingresa_hippie_y_convierte);
	//RUN_TEST(test_mueve_estudiante_y_convierte); -->Probado en ingresa hippie y convierte
	RUN_TEST(test_comenzar_rastrillaje_simple);
	RUN_TEST(test_rastrillaje_mover_hacia_hippie);
	RUN_TEST(test_captura_hippie_entre_agentes);
	RUN_TEST(test_captura_estudiante);
	RUN_TEST(test_mas_vigilante);
	/********************************************************************
	 * TODO: escribir casos de test exhaustivos para todas              *
	 * las funcionalidades de cada módulo.                              *
     * La interacción con el TAD principal se debe hacer exclusivamente *
	 * a través de la interfaz del driver.                              *
	 ********************************************************************/

	return 0;
}
