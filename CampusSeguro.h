#ifndef CAMPUSSEGURO_H_
#define CAMPUSSEGURO_H_

#include "diccNat.h"
#include "Campus.h"
#include "DiccString.h"
#include "Tipos.h"
#include "aed2/Vector.h"
#include "aed2/Lista.h"
#include "aed2/Conj.h"
#include <iostream>

using namespace aed2;

class CampusSeguro{

	struct kSanc{
		Nat sanc;
		Conj<Agente> agentes;
	};
	
	struct datosAgente{
		Posicion posicion;
		Nat cantSanc;
		Nat cantAtrapados;
		typename Lista<kSanc>::Iterador itMismasSanc;
		typename Conj<Agente>::Iterador itConjMismasSanc;
	};

	struct As{
		Agente agente;
		typename diccNat<datosAgente>::Iterador datos;
	};

	public:
	CampusSeguro();
	//ComenzarRastrillaje
	CampusSeguro(Campus c, diccNat<datosAgente> d);

	void IngresarEstudiante(Nombre e, Posicion p);
	void IngresarHippie(Nombre h, Posicion p);
	void MoverEstudiante(Nombre e, Direccion d);
	void MoverHippie(Nombre h);
	void MoverAgente(Agente a);
	Campus campus();
	Conj<Nombre>::Iterador Estudiantes();
	Conj<Nombre>::Iterador Hippies();
	Conj<Agente>::Iterador Agentes();
	Posicion PosEstudianteYHippie(Nombre id);
	Posicion PosAgente(Agente a);
	Nat CantSanciones(Agente a);
	Nat CantHippiesAtrapados(Agente a);
	Agente MasVigilante();
	Conj<Agente> ConMismasSanciones(Agente a);
	Conj<Agente> ConKSanciones(Nat k);

	private:
	diccNat<datosAgente> personalAS;
	Vector<As> agentesOrdenados;
	Vector<As> posicionesAgente;
	As masVigilante;
	Lista<kSanc> listaMismasSanc;
	Lista<kSanc>::Iterador arregloMismasSanc[];
	bool mismasSancModificado;
	DiccString<Posicion> hippies;
	DiccString<Posicion> estudiantes;
	Vector<Nombre> posicionesHippies;
	Vector<Nombre> posicionesEstudiantes;
};


#endif
