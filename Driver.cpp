#include "Driver.h"

#include "Campus.h"
#include "CampusSeguro.h"


namespace aed2 {

Driver::Driver()
{}

Driver::~Driver()
{}


// Generadores del Campus

void Driver::crearCampus(Nat ancho, Nat alto)
{
	Campus caca(ancho, alto);
	this->campus = caca;
}

void Driver::agregarObstaculo(Posicion p)
{
	Posicion pNueva;
	pNueva.x = p.x-1;
	pNueva.y = p.y-1;
	this->campus.AgregarObstaculo(pNueva);
}


// Observadores del Campus

Nat Driver::filas() const
{
	Campus c = this->campus;
  return c.Filas();
}

Nat Driver::columnas() const
{
	Campus c = this->campus;
  return c.Columnas();
}

bool Driver::ocupada(Posicion p) const
{
	Campus c = this->campus;
	Posicion pNueva;
	pNueva.x = p.x-1;
	pNueva.y = p.y-1;
  return c.Ocupada(pNueva);
}


// Generadores de CampusSeguro
void  Driver::comenzarRastrillaje(const Dicc<Agente,Posicion>& d) {
	this->campusSeguro = CampusSeguro(this->campus,d);
}

void Driver::ingresarEstudiante(Nombre n, Posicion p)
{
	Posicion pNueva;
	pNueva.x = p.x-1;
	pNueva.y = p.y-1;
  this->campusSeguro.IngresarEstudiante(n, pNueva);
}

void Driver::ingresarHippie(Nombre n, Posicion p)
{
	Posicion pNueva;
	pNueva.x = p.x-1;
	pNueva.y = p.y-1;
  this->campusSeguro.IngresarHippie(n,pNueva);
}

void Driver::moverEstudiante(Nombre n, Direccion d)
{
	if(d == arriba){
		d = abajo;
	}else if(d == abajo){
		d = arriba;
	}
  this->campusSeguro.MoverEstudiante(n,d);
}

void Driver::moverHippie(Nombre n)
{
  this->campusSeguro.MoverHippie(n);
}

void Driver::moverAgente(Agente pl)
{
  this->campusSeguro.MoverAgente(pl);
}


// Observadores de CampusSeguro

Nombre Driver::iesimoEstudiante(Nat i) const
{
	//PRE: i < #estudiantes
	CampusSeguro cs = this->campusSeguro;
  Conj<Nombre>::Iterador it = cs.Estudiantes();
	Nat k = 0;
	while(k < i && it.HaySiguiente()){
		it.Avanzar();
		k++;
	}
	return it.Siguiente();
}

Nombre Driver::iesimoHippie(Nat i) const
{
	//PRE: i < #hippies
	CampusSeguro cs = this->campusSeguro;
  Conj<Nombre>::Iterador it = cs.Hippies();
	Nat k = 0;
	while(k < i && it.HaySiguiente()){
		it.Avanzar();
		k++;
	}
	return it.Siguiente();
}

Nat Driver::iesimoAgente(Nat i) const
{
	//PRE: i < #Agentes
	CampusSeguro cs = this->campusSeguro;
  Conj<Agente>::Iterador it = cs.Agentes();
	Nat k = 0;
	while(k < i && it.HaySiguiente()){
		it.Avanzar();
		k++;
	}
	return it.Siguiente();
}

Nat Driver::cantEstudiantes() const {
	CampusSeguro cs = this->campusSeguro;
  Conj<Nombre>::Iterador it = cs.Estudiantes();
	Nat k = 0;
	while(it.HaySiguiente()){
		it.Avanzar();
		k++;
	}
	return k;
}

Nat Driver::cantHippies() const
{
	CampusSeguro cs = this->campusSeguro;
  Conj<Nombre>::Iterador it = cs.Hippies();
	Nat k = 0;
	while(it.HaySiguiente()){
		it.Avanzar();
		k++;
	}
	return k;
}

Nat Driver::cantAgentes() const
{
	CampusSeguro cs = this->campusSeguro;
  Conj<Agente>::Iterador it = cs.Agentes();
	Nat k = 0;
	while(it.HaySiguiente()){
		it.Avanzar();
		k++;
	}
	return k;
}

Posicion Driver::posEstudianteYHippie(Nombre n) const
{
	CampusSeguro cs = this->campusSeguro;
	Posicion pFinal = cs.PosEstudianteYHippie(n);
	pFinal.y +=1;
	pFinal.x +=1;
	return pFinal;

}

Posicion Driver::posAgente(Agente pl) const
{
	CampusSeguro cs = this->campusSeguro;
	return cs.PosAgente(pl);
}

Nat Driver::cantSanciones(Agente pl) const
{
	CampusSeguro cs = this->campusSeguro;
	return cs.CantSanciones(pl);
}

Nat Driver::cantHippiesAtrapados(Agente pl) const
{
	CampusSeguro cs = this->campusSeguro;
	return cs.CantHippiesAtrapados(pl);
}


// Otras operaciones de CampusSeguro

Agente Driver::masVigilante() const
{
	CampusSeguro cs = this->campusSeguro;
	return cs.MasVigilante();
}

const Conj<Agente> Driver::conMismasSanciones(Agente a) const
{
	CampusSeguro cs = this->campusSeguro;
	return cs.ConMismasSanciones(a);
}

const Conj<Agente> Driver::conKSanciones(Nat k)
{
	CampusSeguro cs = this->campusSeguro;
	return cs.ConKSanciones(k);
}


} // namespace aed2
