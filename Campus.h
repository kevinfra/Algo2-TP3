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
	Campus(const Nat ancho, const Nat alto);

	void AgregarObstaculo(const Posicion pos);
	Nat Filas();
	Nat Columnas();
	bool Ocupada(const Posicion pos);
	bool PosValida(const Posicion pos);
	bool EsIngreso(const Posicion pos);
	bool IngresoSuperior(const Posicion pos);
	bool IngresoInferior(const Posicion pos);
	Conj<Posicion> Vecinos(const Posicion pos);
	Nat Distancia(const Posicion pos1, const Posicion pos2);
	Posicion MoverDir(const Posicion pos, const Direccion dir);
	Conj<Posicion> IngresosMasCercanos(const Posicion pos);

	private:

	Nat filas;
	Nat columnas;
	Vector<bool> obstaculos;
};

#endif
