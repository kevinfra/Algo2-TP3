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

struct NombrePosicion{
	Nombre nombre;
	Posicion pos;
}

class CampusSeguro{

	public:
	CampusSeguro();
	//ComenzarRastrillaje
	CampusSeguro(const class Campus& c, const diccNat<datosAgente>& d);

	void IngresarEstudiante(const Nombre e, const Posicion p);
	void IngresarHippie(Nombre h, Posicion p);
	void MoverEstudiante(Nombre e, Direccion d);
	void MoverHippie(Nombre h);
	void MoverAgente(Agente a);
	class Campus campus() const;
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
	class Campus grilla;
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

	//Funciones Auxiliares
	Vector<As>& vectorizarPos(const diccNat<datosAgente>& d, const Nat f, const Nat c);
	As menorPlaca(const diccNat<datosAgente>& d);
	Lista<kSanc>& generarListaMismasSanc(diccNat<datosAgente>& d);
	Conj<Posicion>& EstudiantesRodeadosAs(const Conj<Posicion>& c);
	Conj<NombrePosicion>& EstudiantesRodeadosHippies(const Conj<Posicion>& c);
	bool HippiesAtrapando(const Conj<Posicion>& c);
	void SancionarAgentes(const Conj<As>& c);
	Conj<NombrePosicion>& HippiesRodeadosAs(const Conj<Posicion>& c);
	Conj<As>& AgParaPremSanc(const Conj<Posicion>& c);
	void PremiarAgentes(const Conj<As>& c);
	Nat CantHippiesVecinos(const Conj<Posicion>& c);
	bool TodosEstudiantes(const Conj<Posicion>& c);
	bool TodasOcupadas(const Conj<Posicion>& c);
	Conj<NombrePosicion>& HippiesRodeadosEstudiantes(const Conj<Posicion>& c);
	bool AlMenosUnAgente(const Conj<Posicion>& c);



};


#endif
