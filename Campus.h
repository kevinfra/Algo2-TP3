#ifndef CAMPUS_H_
#define CAMPUS_H_

#include "aed2/Vector.h"
#include "aed2/Conj.h"
#include "Tipos.h"
#include <iostream>

using namespace aed2;

class Campus{
	
	public:

	Campus();
	//CrearCampus
	//ancho = fila, alto = columna
	Campus(Nat ancho, Nat alto);

	void AgregarObstaculo(Posicion pos);
	Nat Filas();
	Nat Columnas();
	bool Ocupada(Posicion pos);
	bool PosValida(Posicion pos);
	bool EsIngreso(Posicion pos);
	bool IngresoSuperior(Posicion pos);
	bool IngresoInferior(Posicion pos);
	Conj<Posicion> Vecinos(Posicion pos);
	Nat Distancia(Posicion pos1, Posicion pos2);
	Posicion MoverDir(Posicion pos, Direccion dir);
	Conj<Posicion> IngresosMasCercanos(Posicion pos);

	private:

	Nat filas;
	Nat columnas;
	Vector<bool> obstaculos;
};

#endif
