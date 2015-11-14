#include "Campus.h"
#include "Tipos.h"

using namespace aed2;

Campus::Campus() {}

//CrearCampus
Campus::Campus(Nat alto, Nat ancho){
}

void Campus::AgregarObstaculo(Posicion pos){
}

void Campus::Filas(){
}

void Campus::Columnas(){
}

bool Campus::Ocupada(Posicion pos){
	return false;
}

bool Campus::PosValida(Posicion pos){
	return false;
}

bool Campus::EsIngreso(Posicion pos){
	return false;
}

bool Campus::IngresoSuperior(Posicion pos){
	return false;
}

bool Campus::IngresoInferior(Posicion pos){
	return false;
}

Conj<Posicion> Campus::Vecinos(Posicion pos){
}

Nat Campus::Distancia(Posicion pos1, Posicion pos2){
	return 0;
}

Posicion Campus::MoverDir(Posicion pos, Direccion dir){
}

Conj<Posicion> Campus::IngresosMasCercanos(Posicion pos){
}

