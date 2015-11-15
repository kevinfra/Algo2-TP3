#include "Campus.h"
#include "Tipos.h"

using namespace aed2;

Campus::Campus() {}

//CrearCampus
Campus::Campus(Nat ancho, Nat alto){
	this->filas = ancho;
	this->columnas = alto;
	
	for(int i = 0; i< alto*ancho; i++){
		this->obstaculos.AgregarAtras(false);
	}
}

void Campus::AgregarObstaculo(Posicion pos){
	this->obstaculos.Agregar(pos.y*this->columnas + pos.x, true);
}

Nat Campus::Filas(){
	return this->filas;
}

Nat Campus::Columnas(){
	return this->columnas;
}

bool Campus::Ocupada(Posicion pos){
	return this->obstaculos[pos.y*this->columnas + pos.x];
}

bool Campus::PosValida(Posicion pos){
	return (pos.x >= 0) && (pos.x < this->columnas) && (pos.y >= 0) && (pos.y < this->filas);
}

bool Campus::EsIngreso(Posicion pos){
	return (IngresoSuperior(pos)) || (IngresoInferior(pos));
}

//En la interfaz dice IngresoSuperior pero en los algoritmos dice EsIngresoSuperior
//Lo dejo como en la interfaz
bool Campus::IngresoSuperior(Posicion pos){
	return (pos.y == 0);
}

//En la interfaz dice IngresoInferior pero en los algoritmos dice EsIngresoInferior
//Lo dejo como en la interfaz
bool Campus::IngresoInferior(Posicion pos){
	return (pos.y == this->filas-1);
}

Conj<Posicion> Campus::Vecinos(Posicion pos){
	Conj<Posicion> res;

	Posicion vecino;
	vecino.x = pos.x+1;
	vecino.y = pos.y;
	if(PosValida(vecino)) res.AgregarRapido(vecino);

	vecino.x = pos.x-1;
	vecino.y = pos.y;
	if(PosValida(vecino)) res.AgregarRapido(vecino);

	vecino.x = pos.x;
	vecino.y = pos.y +1;
	if(PosValida(vecino)) res.AgregarRapido(vecino);

	vecino.x = pos.x;
	vecino.y = pos.y-1;
	if(PosValida(vecino)) res.AgregarRapido(vecino);
	
	return res;
}

Nat Campus::Distancia(Posicion pos1, Posicion pos2){
	/*	Si lo hago con Nat no entra al if y no me cambia el signo
	 *	cuando es negativo.
	 */
	int res1, res2;

	res1 = pos1.x - pos2.x;
	res2 = pos1.y - pos2.y;
	if(res1 < 0){
		res1 = -res1;
	}
	if(res2 < 0){
		res2 = -res2;
	}
	
	return (res1 + res2);
}

Posicion Campus::MoverDir(Posicion pos, Direccion dir){
	Posicion posNueva;
	
	if(dir == izq){
		posNueva.x = pos.x - 1;
		posNueva.y = pos.y;
	} else if(dir == der){
		posNueva.x = pos.x + 1;
		posNueva.y = pos.y;
	} else if(dir == arriba){
		posNueva.x = pos.x;
		posNueva.y = pos.y - 1;
	} else{
		posNueva.x = pos.x;
		posNueva.y = pos.y + 1;
	}
	return posNueva;
}

Conj<Posicion> Campus::IngresosMasCercanos(Posicion pos){
	Conj<Posicion> res;
	Posicion filaCero;
	Posicion filaMUno;

	filaCero.x = pos.x;
	filaCero.y = 0;
	filaMUno.x = pos.x;
	filaMUno.y = this->filas -1;

	if(Distancia(pos, filaCero) < Distancia(pos, filaMUno)){
		res.AgregarRapido(filaCero);
	} else if(Distancia(pos, filaCero) > Distancia(pos, filaMUno)){
			res.AgregarRapido(filaMUno);
	} else {
		if(this->filas > 1){
			res.AgregarRapido(filaCero);
			res.AgregarRapido(filaMUno);
		} else {
			res.AgregarRapido(filaCero);
		}

	}

	return res;
}
