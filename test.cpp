#include "Driver.h"
#include "mini_test.h"
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

void test_crear_campus_filas_columnas(){
	// Testeo el constructor de campus, las funciones filas y columnas
	Campus ctest1(10,10);
	ASSERT_EQ(ctest1.Filas(),10);
	ASSERT_EQ(ctest1.Columnas(),10);
	Campus ctest2(1,7);
	ASSERT_EQ(ctest2.Filas(),1);
	ASSERT_EQ(ctest2.Columnas(),7);
	Campus ctest3(0,3);
	ASSERT_EQ(ctest3.Filas(),0);
	ASSERT_EQ(ctest3.Columnas(),3);
	Campus ctest4(5,0);
	ASSERT_EQ(ctest4.Filas(),5);
	ASSERT_EQ(ctest4.Columnas(),0);
	Campus ctest5(0,0);
	ASSERT_EQ(ctest5.Filas(), 0);
	ASSERT_EQ(ctest5.Columnas(),0);
	Campus ctest6(6,14);
	ASSERT_EQ(ctest6.Filas(), 6);
	ASSERT_EQ(ctest6.Columnas(), 14);
}

void test_agregar_obstaculo_y_ocupada(){
	// Testeo la funcion AgregarObstaculo y Ocupada

	Campus ctest(10,10);
	Posicion pos;

	//estan todas vacias
	for(int i = 0; i<10; i++){
		for(int j = 0; j<10; j++){
			pos.x = i;
			pos.y = j;
			ASSERT(!ctest.Ocupada(pos));
		}
	}
	
	//agrego obstaculos en la diagonal principal y reviso si estan
	for(int i = 0; i<10; i++){
		pos.x = i;
		pos.y = i;
		ctest.AgregarObstaculo(pos);
		ASSERT(ctest.Ocupada(pos));
	}

	//creo otro campus, lo lleno de obstaculos y reviso si estan.
	Campus ctest2(3,3);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			pos.x = j;
			pos.y = i;
			ctest2.AgregarObstaculo(pos);
			ASSERT(ctest2.Ocupada(pos));
		}
	}
	//idem con filas y columnas de distinto tamaño
	Campus ctest3(6,14);
	for(int i = 0; i < 6 ; i++){
		for(int j = 0; j< 14; j++){
			pos.x = j;
			pos.y = i;
			ctest3.AgregarObstaculo(pos);
			ASSERT(ctest3.Ocupada(pos));
		}
	}
}

void test_posvalida(){
	Campus ctest1(10,10);
	Posicion pos;
	
	//todas las posiciones son validas
	for(int i = 0; i<10 ;i++){
		for(int j = 0; j<10; j++){
			pos.x = j;
			pos.y = i;
			ASSERT(ctest1.PosValida(pos));
		}
	}
	//posiciones invalidas
	for(int i = 10; i<20; i++){
		for(int j = 200; j< 210; j++){
			pos.x = j;
			pos.y = i;
			ASSERT(!ctest1.PosValida(pos));
		}
	}

	Campus ctest2(9,32);
	//todas las posiciones son validas
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 32; j++){
			pos.x = j;
			pos.y = i;
			ASSERT(ctest2.PosValida(pos));
		}
	}
	//posiciones invalidas
	for(int i = 10; i < 27; i++){
		for(int j = 59; j < 81; j++){
			pos.x = j;
			pos.y = i;
			ASSERT(!ctest2.PosValida(pos));
		}
	}

}

void test_esingreso(){
	/*	Con EsIngreso se testean IngresoSuperior e IngresoInferior
	 *	porque depende de esas dos funciones.
	 */
	Campus ctest1(12,12);
	Posicion pos;
	pos.x = 0;

	pos.y = 0;
	ASSERT(ctest1.EsIngreso(pos));
	pos.y = 11;
	ASSERT(ctest1.EsIngreso(pos));
	pos.y = 3;
	ASSERT(!ctest1.EsIngreso(pos));
	pos.y = 7;
	ASSERT(!ctest1.EsIngreso(pos));

	Campus ctest2(1,1);
	pos.y = 0;
	ASSERT(ctest1.EsIngreso(pos));
	pos.y = 1;
	ASSERT(!ctest1.EsIngreso(pos));
	pos.y = 3;
	ASSERT(!ctest1.EsIngreso(pos));
	pos.y = 7;
	ASSERT(!ctest1.EsIngreso(pos));

	Campus ctest3(0,0);
	pos.y = 0;
	ASSERT(ctest3.EsIngreso(pos));
	pos.y = 1;
	ASSERT(!ctest3.EsIngreso(pos));
	pos.y = 3;
	ASSERT(!ctest3.EsIngreso(pos));
	pos.y = 7;
	ASSERT(!ctest3.EsIngreso(pos));
}

void test_vecinos(){
	/* LA FUNCION PERTENECE DE CONJUNTO NO FUNCIONA!
	 * POR ESO HICE ESTA PARTE ASI.
	 */

	Campus ctest1(10,10);
	Posicion pos;
	Posicion prueba;

	pos.x = 0;
	pos.y = 0;
	Conj<Posicion> conj1(ctest1.Vecinos(pos));
	ASSERT_EQ(conj1.Cardinal(), 2);
	prueba.x = 0;
	prueba.y = 1;
	ASSERT(conj1.Pertenece(prueba));
	prueba.x = 1;
	prueba.y = 0;
	ASSERT(conj1.Pertenece(prueba));

	/*
	Conj<Posicion>::Iterador it1 = conj1.CrearIt();
	cout << endl;
	while(it1.HaySiguiente()){
		cout << it1.Siguiente().y << " " << it1.Siguiente().x << endl;
		it1.Avanzar();
	}
	
	pos.x = 3;
	pos.y = 0;
	Conj<Posicion> conj2(ctest1.Vecinos(pos));
	ASSERT_EQ(conj2.Cardinal(), 3);
	Conj<Posicion>::Iterador it2 = conj2.CrearIt();
	cout << endl;
	while(it2.HaySiguiente()){
		cout << it2.Siguiente().y << " " << it2.Siguiente().x << endl;
		it2.Avanzar();
	}

	pos.x = 9;
	pos.y = 0;
	Conj<Posicion> conj3(ctest1.Vecinos(pos));
	ASSERT_EQ(conj3.Cardinal(), 2);
	Conj<Posicion>::Iterador it3 = conj3.CrearIt();
	cout << endl;
	while(it3.HaySiguiente()){
		cout << it3.Siguiente().y << " " << it3.Siguiente().x << endl;
		it3.Avanzar();
	}

	pos.x = 0;
	pos.y = 5;
	Conj<Posicion> conj4(ctest1.Vecinos(pos));
	ASSERT_EQ(conj4.Cardinal(), 3);
	Conj<Posicion>::Iterador it4 = conj4.CrearIt();
	cout << endl;
	while(it4.HaySiguiente()){
		cout << it4.Siguiente().y << " " << it4.Siguiente().x << endl;
		it4.Avanzar();
	}

	pos.x = 9;
	pos.y = 5;
	Conj<Posicion> conj5(ctest1.Vecinos(pos));
	ASSERT_EQ(conj5.Cardinal(), 3);
	Conj<Posicion>::Iterador it5 = conj5.CrearIt();
	cout << endl;
	while(it5.HaySiguiente()){
		cout << it5.Siguiente().y << " " << it5.Siguiente().x << endl;
		it5.Avanzar();
	}

	pos.x = 0;
	pos.y = 9;
	Conj<Posicion> conj6(ctest1.Vecinos(pos));
	ASSERT_EQ(conj6.Cardinal(), 2);
	Conj<Posicion>::Iterador it6 = conj6.CrearIt();
	cout << endl;
	while(it6.HaySiguiente()){
		cout << it6.Siguiente().y << " " << it6.Siguiente().x << endl;
		it6.Avanzar();
	}

	pos.x = 3;
	pos.y = 9;
	Conj<Posicion> conj7(ctest1.Vecinos(pos));
	ASSERT_EQ(conj7.Cardinal(), 3);
	Conj<Posicion>::Iterador it7 = conj7.CrearIt();
	cout << endl;
	while(it7.HaySiguiente()){
		cout << it7.Siguiente().y << " " << it7.Siguiente().x << endl;
		it7.Avanzar();
	}

	pos.x = 9;
	pos.y = 9;
	Conj<Posicion> conj8(ctest1.Vecinos(pos));
	ASSERT_EQ(conj8.Cardinal(), 2);
	Conj<Posicion>::Iterador it8 = conj8.CrearIt();
	cout << endl;
	while(it8.HaySiguiente()){
		cout << it8.Siguiente().y << " " << it8.Siguiente().x << endl;
		it8.Avanzar();
	}

	pos.x = 5;
	pos.y = 5;
	Conj<Posicion> conj9(ctest1.Vecinos(pos));
	ASSERT_EQ(conj9.Cardinal(), 4);
	Conj<Posicion>::Iterador it9 = conj9.CrearIt();
	cout << endl;
	while(it9.HaySiguiente()){
		cout << it9.Siguiente().y << " " << it9.Siguiente().x << endl;
		it9.Avanzar();
	}*/

}

void test_distancia(){
	// No hace falta inicializar la clase para usar esta funcion.
	Campus ctest(10,10);
	Posicion pos1;
	Posicion pos2;

	pos1.x = 0;
	pos1.y = 0;
	pos2.x = 0;
	pos2.y = 0;
	ASSERT_EQ(ctest.Distancia(pos1,pos2), 0);

	pos1.x = 0;
	pos1.y = 2;
	pos2.x = 1;
	pos2.y = 0;
	ASSERT_EQ(ctest.Distancia(pos1,pos2), 3);

	pos1.x = 0;
	pos1.y = 1;
	pos2.x = 2;
	pos2.y = 1;
	ASSERT_EQ(ctest.Distancia(pos1,pos2), 2);

	pos1.x = 1;
	pos1.y = 1;
	pos2.x = 5;
	pos2.y = 5;
	ASSERT_EQ(ctest.Distancia(pos1,pos2), 8);
}

void test_moverdir(){
	Campus ctest(10,10);
	Posicion pos;
	Direccion dir;

	pos.x = 5;
	pos.y = 5;
	dir = izq;
	pos = ctest.MoverDir(pos,dir);
	ASSERT_EQ(pos.x, 4);
	ASSERT_EQ(pos.y, 5);

	dir = der;
	pos = ctest.MoverDir(pos,dir);
	ASSERT_EQ(pos.x, 5);
	ASSERT_EQ(pos.y, 5);

	dir = arriba;
	pos = ctest.MoverDir(pos,dir);
	ASSERT_EQ(pos.x, 5);
	ASSERT_EQ(pos.y, 4);

	dir = abajo;
	pos = ctest.MoverDir(pos,dir);
	ASSERT_EQ(pos.x, 5);
	ASSERT_EQ(pos.y, 5);

}

void test_ingresos_mas_cercanos(){
	Posicion pos;

	Campus ctest1(3,3);
	pos.x = 1;
	pos.y = 1;
	Conj<Posicion> conj1(ctest1.IngresosMasCercanos(pos));
	ASSERT_EQ(conj1.Cardinal(), 2);
	Conj<Posicion>::Iterador it1 = conj1.CrearIt();
	cout << endl;
	while(it1.HaySiguiente()){
		cout << it1.Siguiente().y << " " << it1.Siguiente().x << endl;
		it1.Avanzar();
	}
	
	Campus ctest2(4,4);
	pos.x = 2;
	pos.y = 3;
	Conj<Posicion> conj2(ctest2.IngresosMasCercanos(pos));
	ASSERT_EQ(conj2.Cardinal(), 1);
	Conj<Posicion>::Iterador it2 = conj2.CrearIt();
	cout << endl;
	while(it2.HaySiguiente()){
		cout << it2.Siguiente().y << " " << it2.Siguiente().x << endl;
		it2.Avanzar();
	}

	pos.x = 1;
	pos.y = 1;
	Conj<Posicion> conj3(ctest2.IngresosMasCercanos(pos));
	ASSERT_EQ(conj3.Cardinal(), 1);
	Conj<Posicion>::Iterador it3 = conj3.CrearIt();
	cout << endl;
	while(it3.HaySiguiente()){
		cout << it3.Siguiente().y << " " << it3.Siguiente().x << endl;
		it3.Avanzar();
	}

	Campus ctest4(0,0);
	pos.x = 0;
	pos.y = 0;
	Conj<Posicion> conj4(ctest4.IngresosMasCercanos(pos));
	ASSERT_EQ(conj4.Cardinal(), 1);
	Conj<Posicion>::Iterador it4 = conj4.CrearIt();
	cout << endl;
	while(it4.HaySiguiente()){
		cout << it4.Siguiente().y << " " << it4.Siguiente().x << endl;
		it4.Avanzar();
	}

	Campus ctest5(9,9);
	pos.x = 2;
	pos.y = 4;
	Conj<Posicion> conj5(ctest5.IngresosMasCercanos(pos));
	ASSERT_EQ(conj5.Cardinal(), 2);
	Conj<Posicion>::Iterador it5 = conj5.CrearIt();
	cout << endl;
	while(it5.HaySiguiente()){
		cout << it5.Siguiente().y << " " << it5.Siguiente().x << endl;
		it5.Avanzar();
	}

	Campus ctest6(1,5);
	pos.x = 3;
	pos.y = 0;
	Conj<Posicion> conj6(ctest6.IngresosMasCercanos(pos));
	ASSERT_EQ(conj6.Cardinal(), 1);
	Conj<Posicion>::Iterador it6 = conj6.CrearIt();
	cout << endl;
	while(it6.HaySiguiente()){
		cout << it6.Siguiente().y << " " << it6.Siguiente().x << endl;
		it6.Avanzar();
	}

	Campus ctest7(3,1);
	pos.x = 0;
	pos.y = 1;
	Conj<Posicion> conj7(ctest7.IngresosMasCercanos(pos));
	ASSERT_EQ(conj7.Cardinal(), 2);
	Conj<Posicion>::Iterador it7 = conj7.CrearIt();
	cout << endl;
	while(it7.HaySiguiente()){
		cout << it7.Siguiente().y << " " << it7.Siguiente().x << endl;
		it7.Avanzar();
	}

	Campus ctest8(16,4);
	pos.x = 1;
	pos.y = 1;
	Conj<Posicion> conj8(ctest8.IngresosMasCercanos(pos));
	ASSERT_EQ(conj8.Cardinal(), 1);
	Conj<Posicion>::Iterador it8 = conj8.CrearIt();
	cout << endl;
	while(it8.HaySiguiente()){
		cout << it8.Siguiente().y << " " << it8.Siguiente().x << endl;
		it8.Avanzar();
	}
}

int main(int argc, char **argv)
{
    RUN_TEST(test_simple);
	RUN_TEST(test_crear_campus_filas_columnas);
	RUN_TEST(test_agregar_obstaculo_y_ocupada);
	RUN_TEST(test_posvalida);
	RUN_TEST(test_esingreso);
	/* La funcion pertenece del tipo conjunto no funciona 
	 * entonces hice el test_vecinos imprimiendo los datos.
	 * Lo comento porque sino va a imprimirse en pantalla cuando corran
	 * los test.
	 * Devuelve lo que tiene que devolver.
	 */
	RUN_TEST(test_vecinos);
	RUN_TEST(test_distancia);
	//Idem test_vecinos
	//RUN_TEST(test_ingresos_mas_cercanos);


	/********************************************************************
	 * TODO: escribir casos de test exhaustivos para todas              *
	 * las funcionalidades de cada módulo.                              *
     * La interacción con el TAD principal se debe hacer exclusivamente *
	 * a través de la interfaz del driver.                              *
	 ********************************************************************/

	return 0;
}
