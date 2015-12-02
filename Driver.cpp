#include "Driver.h"

#include "Campus.h"
#include "CampusSeguro.h"


namespace aed2 {

Driver::Driver()
{
	campus = NULL;
	campusSeguro = NULL;
}

Driver::~Driver() {
	delete campusSeguro;
	delete campus;
}


// Generadores del Campus

void Driver::crearCampus(Nat ancho, Nat alto)
{
	this->campus = new Campus(ancho, alto);
}

void Driver::agregarObstaculo(Posicion p)
{
	Posicion pNueva;
	pNueva.x = p.x-1;
	pNueva.y = p.y-1;
	this->campus->AgregarObstaculo(pNueva);
}


// Observadores del Campus

Nat Driver::filas() const
{
  return this->campus->Filas();
}

Nat Driver::columnas() const
{
  return this->campus->Columnas();
}

bool Driver::ocupada(Posicion p) const
{
	Posicion pNueva;
	pNueva.x = p.x-1;
	pNueva.y = p.y-1;
  return this->campus->Ocupada(pNueva);
}


// Generadores de CampusSeguro
void  Driver::comenzarRastrillaje(const Dicc<Agente,Posicion>& d) {
	Dicc<Agente, Posicion> dicc = d;
	Dicc<Agente, Posicion>::Iterador itDicc = dicc.CrearIt();
	while(itDicc.HaySiguiente()) {
		itDicc.Siguiente().significado.x = itDicc.Siguiente().significado.x - 1;
		itDicc.Siguiente().significado.y = itDicc.Siguiente().significado.y - 1;
		itDicc.Avanzar();
	}
	this->campusSeguro = new CampusSeguro(*(this->campus), dicc);
}

void Driver::ingresarEstudiante(Nombre n, Posicion p)
{
	Posicion pNueva;
	pNueva.x = p.x-1;
	pNueva.y = p.y-1;
  this->campusSeguro->IngresarEstudiante(n, pNueva);
}

void Driver::ingresarHippie(Nombre n, Posicion p)
{
	Posicion pNueva;
	pNueva.x = p.x-1;
	pNueva.y = p.y-1;
  this->campusSeguro->IngresarHippie(n,pNueva);
}

void Driver::moverEstudiante(Nombre n, Direccion d)
{
  this->campusSeguro->MoverEstudiante(n,d);
}

void Driver::moverHippie(Nombre n)
{
  this->campusSeguro->MoverHippie(n);
}

void Driver::moverAgente(Agente pl)
{
  this->campusSeguro->MoverAgente(pl);
}


// Observadores de CampusSeguro

Nombre Driver::iesimoEstudiante(Nat i) const
{
	//PRE: i < #estudiantes
	CampusSeguro cs = *(this->campusSeguro);
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
	CampusSeguro cs = *(this->campusSeguro);
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
	CampusSeguro cs = *(this->campusSeguro);
  Conj<Agente>::Iterador it = cs.Agentes();
	Nat k = 0;
	while(k < i && it.HaySiguiente()){
		it.Avanzar();
		k++;
	}
	return it.Siguiente();
}

Nat Driver::cantEstudiantes() const {
	CampusSeguro cs = *(this->campusSeguro);
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
	CampusSeguro cs = *(this->campusSeguro);
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
	CampusSeguro cs = *(this->campusSeguro);
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
	CampusSeguro cs = *(this->campusSeguro);
	Posicion pFinal = cs.PosEstudianteYHippie(n);
	pFinal.y +=1;
	pFinal.x +=1;
	return pFinal;

}

Posicion Driver::posAgente(Agente pl) const
{
	CampusSeguro cs = *(this->campusSeguro);
	return cs.PosAgente(pl);
}

Nat Driver::cantSanciones(Agente pl) const
{
	CampusSeguro cs = *(this->campusSeguro);
	return cs.CantSanciones(pl);
}

Nat Driver::cantHippiesAtrapados(Agente pl) const
{
	CampusSeguro cs = *(this->campusSeguro);
	return cs.CantHippiesAtrapados(pl);
}


// Otras operaciones de CampusSeguro

Agente Driver::masVigilante() const
{
	CampusSeguro cs = *(this->campusSeguro);
	return cs.MasVigilante();
}

const Conj<Agente> Driver::conMismasSanciones(Agente a) const
{
	CampusSeguro cs = *(this->campusSeguro);
	return cs.ConMismasSanciones(a);
}

const Conj<Agente> Driver::conKSanciones(Nat k)
{
	CampusSeguro cs = *(this->campusSeguro);
	return cs.ConKSanciones(k);
}


} // namespace aed2
