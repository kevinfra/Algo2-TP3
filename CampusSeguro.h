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
	struct datosAgente;
	struct kSanc;
	struct As;

	public:
	CampusSeguro();
	//ComenzarRastrillaje
	CampusSeguro(const class Campus& c, const Dicc<Agente, Posicion>& dicc);

	void IngresarEstudiante(Nombre e, Posicion p);
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
	struct kSanc{
		Nat sanc;
		Conj<Agente> agentes;
	};

	struct As{
		Agente agente;
		typename diccNat<datosAgente>::itDiccNat datos;
		/*	bool operator==(const As& a) const{
			return (agente == a.agente && datos == a.datos);
		}*/
	};

	struct datosAgente{
		Posicion posicion;
		Nat cantSanc;
		Nat cantAtrapados;
		typename Lista<kSanc>::Iterador itMismasSanc;
		typename Conj<Agente>::Iterador itConjMismasSanc;
	};

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
	void SancionarAgentes(Conj<As> c);
	void PremiarAgentes(Conj<As> c);
	Vector<As> vectorizarPos(diccNat<datosAgente>& d, Nat f, Nat c);
	As menorPlaca(diccNat<datosAgente>& d);
	Lista<kSanc> generarListaMismasSanc(diccNat<datosAgente>& d);
	Conj<Posicion> EstudiantesRodeadosAs(Conj<Posicion> c);
	Conj<NombrePosicion> EstudiantesRodeadosHippies(Conj<Posicion> c);
	bool HippiesAtrapando(Conj<Posicion> c);
	Conj<NombrePosicion> HippiesRodeadosAs(Conj<Posicion> c);
	Conj<As> AgParaPremSanc(Conj<Posicion> c);
	Nat CantHippiesVecinos(Conj<Posicion> c);
	bool TodosEstudiantes(Conj<Posicion> c);
	bool TodasOcupadas(Conj<Posicion> c);
	Conj<NombrePosicion> HippiesRodeadosEstudiantes(Conj<Posicion> c);
	bool AlMenosUnAgente(Conj<Posicion> c);
	typename diccNat<datosAgente>::itDiccNat busqBinPorPlaca(Agente a, Vector<As> v);
	Posicion proxPos(Posicion pos, DiccString<Posicion> dicc);
	Nat distanciaMasCorta(Posicion pos, DiccString<Posicion> dicc);
	Nat distancia(Posicion p1, Posicion p2);
	Nat modulo(int val);
	Conj<Posicion> lugaresPosibles(Posicion pos, Conj<Posicion> posiciones);
	bool hayAlgoEnPos(Posicion pos);
	void actualizarAgente(Posicion pos, Agente a, typename diccNat<datosAgente>::itDiccNat);
	bool atrapado(Posicion pos);
};

#endif
