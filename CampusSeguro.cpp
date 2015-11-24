#include "CampusSeguro.h"
#include "Tipos.h"

using namespace aed2;

//ComenzarRastrillaje
CampusSeguro::CampusSeguro(const class Campus& c, const Dicc<Agente, Posicion>& dicc) : grilla(c) {

	//creo el hash
	Vector< diccNat<datosAgente>::tupla > v;
	diccNat<datosAgente>::tupla tuplaParaHash;
	datosAgente datosA;
	Dicc<Agente, Posicion>::const_Iterador itd = dicc.CrearIt();
	
	datosA.cantSanc = 0;
	datosA.cantAtrapados = 0;

	while(itd.HaySiguiente()){
		tuplaParaHash.clave = itd.SiguienteClave();
		datosA.posicion = itd.SiguienteSignificado();
		tuplaParaHash.significado = datosA;
		v.AgregarAtras(tuplaParaHash);
		itd.Avanzar();
	}

	diccNat<datosAgente> diccHash(v);
	this->personalAS = diccHash;

	this->listaMismasSanc = generarListaMismasSanc(diccHash);
	this->posicionesAgente = vectorizarPos(diccHash,campus().Filas(), campus().Columnas());
	this->masVigilante = menorPlaca(diccHash);
	this->mismasSancModificado = true;
	//los diccionarios con hippies y estudiantes deben iniciar vacios

	Nat i = 0;
	while(i < campus().Filas()*campus().Columnas()){
		this->posicionesHippies.AgregarAtras(" ");
		this->posicionesEstudiantes.AgregarAtras(" ");
		i++;
	}

	typename diccNat<datosAgente>::itDiccNat it = diccHash.crearIt();
	bool ordenado;
	while(it.haySiguiente()){
		i = 0;
		ordenado = true;
		while(i < this->agentesOrdenados.Longitud() && ordenado){
			if (this->agentesOrdenados[i].agente > it.siguiente().clave){
				ordenado = false;
			}
			i++;
		}

		As tupla;
		tupla.agente = it.siguiente().clave;
		tupla.datos = it;
		this->agentesOrdenados.AgregarAtras(tupla);
		it.avanzar();
	}
}

Vector<CampusSeguro::As> CampusSeguro::vectorizarPos(diccNat<datosAgente>& d, Nat f, Nat c){
	Vector<As> res;
	Nat i = 0;
	typename diccNat<datosAgente>::itDiccNat it = d.crearIt();

	/*	Pruebo creando un iterador vacio de esta forma.
	 *	Hay que testearlo bien esto.
	 */
	
	Vector< diccNat<datosAgente>::tupla > v;
	diccNat<datosAgente> dVacio(v);

	typename diccNat<datosAgente>::itDiccNat itVacio = dVacio.crearIt();

	As tuplaVacia;
	tuplaVacia.agente = 0;

	while(i < f*c){
		res.AgregarAtras(tuplaVacia);
		i++;
	}
	
	As tuplaAAgregar;
	while(it.haySiguiente()){
		typename diccNat<datosAgente>::itDiccNat itPos = it;
		tuplaAAgregar.agente = it.siguiente().clave;
		tuplaAAgregar.datos = itPos;
		res[it.siguiente().significado.posicion.y * c + it.siguiente().significado.posicion.x] = tuplaAAgregar;
		it.avanzar();
	}
	
	return res;
}

CampusSeguro::As CampusSeguro::menorPlaca(diccNat<datosAgente>& d){
	typename diccNat<datosAgente>::itDiccNat it = d.crearIt();
	typename diccNat<datosAgente>::itDiccNat itMenor;
	//idem que en funcion anterior.
	Nat placaMenor = it.siguiente().clave;

	while(it.haySiguiente()){
		if (it.siguiente().clave < placaMenor){
			placaMenor = it.siguiente().clave;
			itMenor = it;
		}
		it.avanzar();
	}
	As res;
	res.agente = placaMenor;
	res.datos = itMenor;
	return res;
}

Lista<CampusSeguro::kSanc> CampusSeguro::generarListaMismasSanc(diccNat<datosAgente>& d){
	typename diccNat<datosAgente>::itDiccNat itDic = d.crearIt();
	Lista<kSanc> res;
	typename Lista<kSanc>::Iterador itL = res.CrearIt();

	Conj<Agente> conj;
	kSanc tupla;
	tupla.sanc = 0;
	tupla.agentes = conj;
	res.AgregarAdelante(tupla);

	while(itDic.haySiguiente()){
		typename Conj<Agente>::Iterador itC = itL.Siguiente().agentes.AgregarRapido(itDic.siguiente().clave);
		itDic.siguiente().significado.itConjMismasSanc = itC;
		itDic.siguiente().significado.itMismasSanc = itL;
		
		itDic.avanzar();
	}

	return res;
}

void CampusSeguro::IngresarHippie(Nombre h, Posicion pos){
}

void CampusSeguro::IngresarEstudiante(Nombre e, Posicion pos){
	if(TodasOcupadas(campus().Vecinos(pos)) && AlMenosUnAgente(campus().Vecinos(pos))){
		Conj<As> conjAgParaSanc = AgParaPremSanc(campus().Vecinos(pos));
		SancionarAgentes(conjAgParaSanc);
	}

	if(CantHippiesVecinos(campus().Vecinos(pos)) < 2){
		this->estudiantes.Definir(e,pos);
		this->posicionesEstudiantes[pos.y * campus().Columnas() + pos.x] = e;
	} else{
		this->hippies.Definir(e,pos);
		this->posicionesHippies[pos.y * campus().Columnas() + pos.x] = e;
	}


	Conj<NombrePosicion> conjHippiesRodEst = HippiesRodeadosEstudiantes(campus().Vecinos(pos));

	if (conjHippiesRodEst.Cardinal() > 0) {
		typename Conj<NombrePosicion>::Iterador itHEst = conjHippiesRodEst.CrearIt();
		while(itHEst.HaySiguiente()){
			this->estudiantes.Definir(itHEst.Siguiente().nombre, itHEst.Siguiente().pos);
			this->hippies.Eliminar(itHEst.Siguiente().nombre);

			this->posicionesEstudiantes[itHEst.Siguiente().pos.y * campus().Columnas() + itHEst.Siguiente().pos.x] = this->posicionesHippies[itHEst.Siguiente().pos.y * campus().Columnas() + itHEst.Siguiente().pos.x];

			this->posicionesHippies[itHEst.Siguiente().pos.y * campus().Columnas() + itHEst.Siguiente().pos.x] = " ";

			itHEst.Avanzar();
		}
	}


	//Las Capturas se actualizan en HippiesRodeadosAs
	Conj<NombrePosicion> conjHippiesRodAs = HippiesRodeadosAs(campus().Vecinos(pos));

	if(conjHippiesRodAs.Cardinal() > 0){
		typename Conj<NombrePosicion>::Iterador itHAs = conjHippiesRodAs.CrearIt();
		while(itHAs.HaySiguiente()){
			this->hippies.Eliminar(itHAs.Siguiente().nombre);
			this->posicionesHippies[itHAs.Siguiente().pos.y * campus().Columnas() + itHAs.Siguiente().pos.x];

			itHAs.Avanzar();
		}
	}


	Conj<NombrePosicion> conjEstRodHip = EstudiantesRodeadosHippies(campus().Vecinos(pos));

	if(conjEstRodHip.Cardinal() > 0){
		typename Conj<NombrePosicion>::Iterador itEstH = conjEstRodHip.CrearIt();
		while(itEstH.HaySiguiente()){
			this->estudiantes.Eliminar(itEstH.Siguiente().nombre);
			this->posicionesEstudiantes[itEstH.Siguiente().pos.y * campus().Columnas() + itEstH.Siguiente().pos.x] = " ";

			this->hippies.Definir(itEstH.Siguiente().nombre, itEstH.Siguiente().pos);
			this->posicionesHippies[itEstH.Siguiente().pos.y * campus().Columnas() + itEstH.Siguiente().pos.x] = itEstH.Siguiente().nombre;

			itEstH.Avanzar();
		}
	}

	
	Conj<Posicion> conjEstRodAs = EstudiantesRodeadosAs(campus().Vecinos(pos));

	if(conjEstRodAs.Cardinal() > 0){
		typename Conj<Posicion>::Iterador itEstAs = conjEstRodAs.CrearIt();

		while(itEstAs.HaySiguiente()){
			if(TodasOcupadas(campus().Vecinos(itEstAs.Siguiente())) && AlMenosUnAgente(campus().Vecinos(itEstAs.Siguiente()))){
				Conj<As> conjAgParaSanc = AgParaPremSanc(campus().Vecinos(itEstAs.Siguiente()));
				SancionarAgentes(conjAgParaSanc);
			}
		}
	}

}

Conj<Posicion> CampusSeguro::EstudiantesRodeadosAs(Conj<Posicion> c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<Posicion> res;

	while(itC.HaySiguiente()){
		if(TodasOcupadas(campus().Vecinos(itC.Siguiente())) && AlMenosUnAgente(campus().Vecinos(itC.Siguiente()))){
			res.AgregarRapido(itC.Siguiente());
		}
		itC.Avanzar();
	}
	
	return res;
}

Conj<NombrePosicion> CampusSeguro::EstudiantesRodeadosHippies(Conj<Posicion> c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<NombrePosicion> res;
	NombrePosicion np;

	while(itC.HaySiguiente()){
		if(this->posicionesEstudiantes[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] != " " && TodasOcupadas(campus().Vecinos(itC.Siguiente())) && HippiesAtrapando(campus().Vecinos(itC.Siguiente()))){
			np.nombre = this->posicionesEstudiantes[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x];
			np.pos = itC.Siguiente();
			res.AgregarRapido(np);
		}
		itC.Avanzar();
	}

	return res;
}

bool CampusSeguro::HippiesAtrapando(Conj<Posicion> c){
	Nat i = 0;
	typename Conj<Posicion>::Iterador itC = c.CrearIt();

	while(itC.HaySiguiente()){
		if(this->posicionesHippies[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] != " "){
			i++;
		}
		itC.Avanzar();
	}
	
	return i >= 2;
}


void CampusSeguro::SancionarAgentes(Conj<As> c){
	typename Conj<As>::Iterador itC = c.CrearIt();

	if(c.Cardinal() > 0){
		this->mismasSancModificado = true;
	}

	kSanc tuplaksanc;
	Conj<Agente> conjVacio;

	while(itC.HaySiguiente()){
		itC.Siguiente().datos.siguiente().significado.cantSanc++;

		typename Lista<kSanc>::Iterador itLis = itC.Siguiente().datos.siguiente().significado.itMismasSanc;
		
		if(itC.Siguiente().datos.siguiente().significado.itMismasSanc.HaySiguiente()){
			itC.Siguiente().datos.siguiente().significado.itMismasSanc.Avanzar();
			
			if(itC.Siguiente().datos.siguiente().significado.itMismasSanc.Siguiente().sanc != itC.Siguiente().datos.siguiente().significado.cantSanc){
				tuplaksanc.agentes = conjVacio;
				tuplaksanc.sanc = itC.Siguiente().datos.siguiente().significado.cantSanc;
				itC.Siguiente().datos.siguiente().significado.itMismasSanc.AgregarComoAnterior(tuplaksanc);

				itC.Siguiente().datos.siguiente().significado.itMismasSanc.Retroceder();
			}

		} else {
			tuplaksanc.agentes = conjVacio;
			tuplaksanc.sanc = itC.Siguiente().datos.siguiente().significado.cantSanc;
			itC.Siguiente().datos.siguiente().significado.itMismasSanc.AgregarComoSiguiente(tuplaksanc);

			itC.Siguiente().datos.siguiente().significado.itMismasSanc.Avanzar();
		}
	
		itC.Siguiente().datos.siguiente().significado.itConjMismasSanc.EliminarSiguiente();
		itC.Siguiente().datos.siguiente().significado.itConjMismasSanc = itC.Siguiente().datos.siguiente().significado.itMismasSanc.Siguiente().agentes.AgregarRapido(itC.Siguiente().agente);
		itC.Avanzar();
	}

}

Conj<NombrePosicion> CampusSeguro::HippiesRodeadosAs(Conj<Posicion> c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<NombrePosicion> res;
	NombrePosicion np;

	while(itC.HaySiguiente()){
		if(this->posicionesHippies[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] != " " && TodasOcupadas(campus().Vecinos(itC.Siguiente())) && AlMenosUnAgente(campus().Vecinos(itC.Siguiente()) )){
			np.nombre = this->posicionesHippies[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x];
			np.pos = itC.Siguiente();
			
			res.AgregarRapido(np);

			Conj<As> conjAgPremiar = AgParaPremSanc(campus().Vecinos(itC.Siguiente()));
			PremiarAgentes(conjAgPremiar);
		}
		itC.Avanzar();
	}

	return res;

}

Conj<CampusSeguro::As> CampusSeguro::AgParaPremSanc(Conj<Posicion> c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<As> res;

	while(itC.HaySiguiente()){
		if(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x].datos.haySiguiente()){
			res.AgregarRapido(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x]);
		}
		itC.Avanzar();
	}

	return res;
}

void CampusSeguro::PremiarAgentes(Conj<CampusSeguro::As> c){
	typename Conj<As>::Iterador itC = c.CrearIt();
	
	while(itC.HaySiguiente()){
		itC.Siguiente().datos.siguiente().significado.cantAtrapados++;

		if(this->masVigilante.datos.siguiente().significado.cantAtrapados < itC.Siguiente().datos.siguiente().significado.cantAtrapados){
			this->masVigilante = itC.Siguiente();
		}
		itC.Avanzar();
	}

}

Nat CampusSeguro::CantHippiesVecinos(Conj<Posicion> c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Nat res = 0;

	while(itC.HaySiguiente()){
		typename DiccString(Posicion)::Iterador itDic = this->hippies.CrearIt();

		while(itDic.HaySiguiente()){
			if(itDic.Siguiente() = itC.Siguiente()) res++;
			itDic.Avanzar();
		}
		itC.Avanzar();
	}

	return res;
}

bool CampusSeguro::TodosEstudiantes(Conj<Posicion> c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	bool res = true;

	while(itC.HaySiguiente()){
		if(this->posicionesEstudiantes[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] == " ") res = false;

		itC.Avanzar();
	}
	return res;
}

bool CampusSeguro::TodasOcupadas(Conj<Posicion> c){
	bool res = false;
	typename Conj<Posicion>::Iterador itC = c.CrearIt();

	while(itC.HaySiguiente() && !res){
		if(this->posicionesHippies[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] != " ") res = true;

		if(this->posicionesEstudiantes[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] != " ") res = true;
		
		itC.Avanzar();
	}

	itC = c.CrearIt();
	while(itC.HaySiguiente() && !res){
		if(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x].datos.haySiguiente()) res = true;
	}

	itC = c.CrearIt();
	while(itC.HaySiguiente() && !res){
		if(campus().Ocupada(itC.Siguiente())) res = true;
		itC.Avanzar();
	}

	return res;
}

Conj<NombrePosicion> CampusSeguro::HippiesRodeadosEstudiantes(Conj<Posicion> c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<NombrePosicion> conjuntoRetorno;
	NombrePosicion np;

	while(itC.HaySiguiente()){
		if(this->posicionesHippies[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] != " " && TodasOcupadas(campus().Vecinos(itC.Siguiente())) && TodosEstudiantes(campus().Vecinos(itC.Siguiente()))){
			np.nombre = this->posicionesHippies[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x];
			np.pos = itC.Siguiente();

			conjuntoRetorno.AgregarRapido(np);
		}

		itC.Avanzar();
	}
	
	return conjuntoRetorno;
}

bool CampusSeguro::AlMenosUnAgente(Conj<Posicion> c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	bool res = false;

	while(itC.HaySiguiente()){
		if(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x].datos.haySiguiente()) res =  true;

		itC.Avanzar();
	}

	return res;
}

/*
void CampusSeguro::IngresarHippie(Nombre h, Posicion pos){
	if(TodasOcupadas(campus().Vecinos(pos)) && AlMenosUnAgente(this->campus.Vecinos(pos))){
		Conj<As> conjAgParaPrem = AgParaPremSanc(campus().Vecinos(pos));
		PremiarAgentes(conjAgParaPrem);
	} else if(TodasOcupadas(campus().Vecinos(pos)) && TodosEstudiantes(campus().Vecinos(pos))){
		this->estudiantes.Definir(h,pos);
		this->posicionesEstudiantes[pos.y * campus().Columnas() + pos.x] = h;
	} else {
		this->hippies.Definir(h,pos);
		this->posicionesHippies[pos.y * campus().Columnas() + pos.x] = h;
	}

	
	Conj<NombrePosicion> conjHippiesRodEst = HippiesRodeadosEstudiantes(campus().Vecinos(pos));

	if (conjHippiesRodEst.Cardinal() > 0) {
		typename Conj<NombrePosicion>::Iterador itHEst = conjHippiesRodEst.CrearIt();
		while(itHEst.HaySiguiente()){
			this->estudiantes.Definir(itHEst.Siguiente().nombre, itHEst.Siguiente().pos);
			this->hippies.Eliminar(itHEst.Siguiente().nombre);

			this->posicionesEstudiantes[itHEst.Siguiente().pos.y * campus().Columnas() + itHEst.Siguiente().pos.x] = this->posicionesHippies[itHEst.Siguiente().pos.y * campus().Columnas() + itHEst.Siguiente().pos.x];

			this->posicionesHippies[itHEst.Siguiente().pos.y * campus().Columnas() + itHEst.Siguiente().pos.x] = " ";

			itHEst.Avanzar();
		}
	}


	//Las Capturas se actualizan en HippiesRodeadosAs
	Conj<NombrePosicion> conjHippiesRodAs = HippiesRodeadosAs(campus().Vecinos(pos));

	if(conjHippiesRodAs.Cardinal() > 0){
		typename Conj<NombrePosicion>::Iterador itHAs = conjHippiesRodAs.CrearIt();
		while(itHAs.HaySiguiente()){
			this->hippies.Eliminar(itHAs.Siguiente().nombre);
			this->posicionesHippies[itHAs.Siguiente().pos.y * campus().Columnas() + itHAs.Siguiente().pos.x];

			itHAs.Avanzar();
		}
	}


	Conj<NombrePosicion> conjEstRodHip = EstudiantesRodeadosHippies(campus().Vecinos(pos));

	if(conjEstRodHip.Cardinal() > 0){
		typename Conj<NombrePosicion>::Iterador itEstH = conjEstRodHip.CrearIt();
		while(itEstH.HaySiguiente()){
			this->estudiantes.Eliminar(itEstH.Siguiente().nombre);
			this->posicionesEstudiantes[itEstH.Siguiente().pos.y * campus().Columnas() + itEstH.Siguiente().pos.x] = " ";

			this->hippies.Definir(itEstH.Siguiente().nombre, itEstH.Siguiente().pos);
			this->posicionesHippies[itEstH.Siguiente().pos.y * campus().Columnas() + itEstH.Siguiente().pos.x] = itEstH.Siguiente().nombre;

			itEstH.Avanzar();
		}
	}

	
	Conj<Posicion> conjEstRodAs = EstudiantesRodeadosAs(campus().Vecinos(pos));

	if(conjEstRodAs.Cardinal() > 0){
		typename Conj<Posicion>::Iterador itEstAs = conjEstRodAs.CrearIt();

		while(itEstAs.HaySiguiente()){
			if(TodasOcupadas(campus().Vecinos(itEstAs.Siguiente())) && AlMenosUnAgente(campus().Vecinos(itEstAs.Siguiente()))){
				Conj<As> conjAgParaSanc = AgParaPremSanc(campus().Vecinos(itEstAs.Siguiente()));
				SancionarAgentes(conjAgParaSanc);
			}
		}
	}

}
*/

void CampusSeguro::MoverEstudiante(Nombre e, Direccion d){
}

void CampusSeguro::MoverHippie(Nombre h){
}

// TODO: testear
void CampusSeguro::MoverAgente(Agente a){
	typename diccNat<datosAgente>::itDiccNat it = busqBinPorPlaca(this->agentesOrdenados, a);

	// Actualizo la posicion del agente
	Posicion nuevaPos = proxPos(it.siguiente().posicion, this->hippies);
	posicionesAgente[it.siguiente().posicion.y * this->grilla.Columnas() + it.siguiente().posicion.x].datos = Conj().CrearIt();

	As as;
	as.agente = a;
	as.datos = it;
	posicionesAgente[nuevaPos.y * this->grilla.Columnas() + nuevaPos.x] = as;

	it.siguiente().posicion = nuevaPos;

	// Me fijo a quienes atrapa
	Posicion posArr = this->grilla.MoverDir(nuevaPos, arriba);
	actualizarAgente(posArr, a, it);

	Posicion posAba = this->grilla.MoverDir(nuevaPos, abajo);
	actualizarAgente(posAba, a, it);

	Posicion posDer = this->grilla.MoverDir(nuevaPos, der);
	actualizarAgente(posDer, a, it);

	Posicion posIzq = this->grilla.MoverDir(nuevaPos, izq);
	actualizarAgente(posIzq, a, it);
}

// TODO: testear
typename diccNat<datosAgente>::itDiccNat CampusSeguro::busqBinPorPlaca(Agente a, Vector<As> v){
	Nat inf = 0;
	Nat sup = v.Longitud();
	Nat med;

	while(inf != sup - 1){
		med = (inf + sup) / 2;
		if(v[med].agente <= a)
			inf = med + 1;
		else
			sup = med;
	}

	return(v[inf].datos);
}

// TODO: testear
// Tengo que ponerle return fuera del if?
Posicion CampusSeguro::proxPos(Posicion pos, DiccString<Posicion> dicc){
	Nat distCorta = distanciaMasCorta(pos, dicc);

	Conj<Posicion> conjDondeIr = dondeIr(pos, distCorta, dicc);
	Conj<Posicion> conjLugaresPosibles = lugaresPosibles(pos, conjDondeIr);

	if(conjLugaresPosibles.EsVacio())
		return pos;
	else{
		typename Conj<Posicion>::Iterador it = conjLugaresPosibles.CrearIt();
		return it.Siguiente();
	}

}

// TODO: testear
Nat CampusSeguro::distanciaMasCorta(Posicion pos, DiccString<Posicion> dicc){
	typename DiccString<Posicion>::Iterador it = dicc.CrearIt();

	Nat dist = distancia(pos, it.Siguiente());
	it.Avanzar();

	while(it.HaySiguiente()){
		if (dist > distancia(pos, it.Siguiente()))
			dist = distancia(pos, it.Siguiente());
		it.Avanzar();
	}

	return dist;
}

Nat CampusSeguro::distancia(Posicion p1, Posicion p2){
	return(modulo(pos1.x - pos2.x) + modulo(pos1.y - pos2.y));
}

Nat CampusSeguro::modulo(int val){
	if(val >= 0)
		return val;
	else
		return -val;
}

// TODO: testear
Conj<Posicion> CampusSeguro::dondeIr(Posicion pos, Nat dist, DiccString<Posicion> dicc){
	Conj<Posicion> posiciones = Conj();
	typename DiccString<Posicion>::Iterador it = dicc.CrearIt();

	while(it.HaySiguiente()){
		if(dist = distancia(pos, it.Siguiente()))
			posiciones.AgregarRapido(it.Siguiente());
		it.Avanzar();
	}

	return(posiciones);
}

// TODO: testear
Conj<Posicion> CampusSeguro::lugaresPosibles(Posicion pos, Conj<Posicion> posiciones){
	typename Conj<Posicion>::Iterador it = posiciones.CrearIt();
	Conj<Posicion> lugares = Conj();

	while(it.HaySiguiente()){
		if(hayAlgoEnPos(it.Siguiente())){
			if(it.Siguiente().x > pos.x)
				lugares.AgregarRapido(Posicion(pos.x + 1, pos.y));
			else if(it.Siguiente().x < pos.x)
				lugares.AgregarRapido(Posicion(pos.x - 1, pos.y));

			if(it.Siguiente().y > pos.y)
				lugares.AgregarRapido(Posicion(pos.x, pos.y + 1));
			else if(it.Siguiente().x < pos.x)
				lugares.AgregarRapido(Posicion(pos.x, pos.y - 1));
		}
	}

	return lugares;
}

// TODO: testear
bool CampusSeguro::hayAlgoEnPos(Posicion pos){
	if(this->posicionesAgente[pos.y * this->grilla.Columnas() + pos.x].datos.haySiguiente())
		return true;

	if(this->posicionesHippies[pos.y * this->grilla.Columnas() + pos.x] != " ")
		return true;
	if(this->posicionesEstudiantes[pos.y * this->grilla.Columnas() + pos.x] != " ")
		return true;

	if(this->grilla.Ocupada(pos))
		return true;

	return false;
}

// TODO: testear
void CampusSeguro::actualizarAgente(Posicion pos, Agente a, typename diccNat<datosAgente>::itDiccNat it){
	if(this->grilla.PosValida(pos)){

		if(this->posicionesHippies[pos.y * this->grilla.Columnas() + pos.x] != " "){
			if(atrapado(pos)){
				// Incremento sus capturas, actualizo masVigilante y mato al hippie
				it.siguiente().cantAtrapados++;
				if(it.siguiente().cantAtrapados > this->masVigilante.datos.siguiente().cantAtrapados){
					As tup;
					tup.agente = a;
					tup.datos = it;
					this->masVigilante = tup;
				}
				this->hippies.Eliminar(this->posicionesHippies[pos.y * this->grilla.Columnas() + pos.x]);
				posicionesHippies[pos.y * this->grilla.Columnas() + pos.x] = " ";
			}
		}

		if(posicionesEstudiantes[pos.y * this->grilla.Columnas() + pos.x] != " "){
			if(atrapado(pos)){
				//Actualizo las sanciones y las estructuras relacionadas
				this->mismasSancModificado = true;

				typename Conj<Agente>::Iterador iterConj = it.siguiente().itConjMismasSanc;
				iterConj.EliminarSiguiente();

				typename Lista<kSanc>::Iterador iterLista = it.siguiente().itMismasSanc;

				// Me guardo un iterador para borrar el nodo de la lista si es que queda sin agentes
				typename Lista<kSanc>::Iterador iterListaAnterior = it.siguiente().itMismasSanc;			// Esto va a funcionar? Que estos dos iteradores sean iguales no genera algun problema de referencia o algo asi?

				if(iterLista.HaySiguiente()){
					// Me fijo si el siguiente es la siguiente sancion
					Nat sanciones = iterLista.Siguiente().sanc;
					iterLista.Avanzar();

					if(iterLista.Siguiente().sanc = sanciones + 1){
						// Lo agrego al conjunto
						it.siguiente().itConjMismasSanc = iterLista.Siguiente().agentes.AgregarRapido(a);
					}
					else{
						// Creo un nuevo nodo en el medio
						Conj<Agente> conj = Conj();
						it.siguiente().itConjMismasSanc = conj.AgregarRapido(a);

						kSanc nodo;
						nodo.sanc = sanciones + 1;
						nodo.agentes = conj;
						iterLista.AgregarComoAnterior(nodo);

						iterLista.Retroceder();

						it.siguiente().itMismasSanc = iterLista;
					}
				}
				else{
					//Creo un nuevo nodo
					Conj<Agente> conj = Conj();
					it.siguiente().itConjMismasSanc = conj.AgregarRapido(a);

					kSanc nodo;
					nodo.sanc = sanciones + 1;
					nodo.agentes = conj;
					iterLista.AgregarComoSiguiente(nodo);

					iterLista.Avanzar();

					it.siguiente().itMismasSanc = iterLista;
				}

				if(!iterConj.HaySiguiente()){
					// Borro el nodo anterior de la lista porque no tiene agentes
					iterListaAnterior.EliminarSiguiente();
				}

				it.siguiente().cantSanc++;
			}
		}
	}
}

bool CampusSeguro::atrapado(Posicion pos){
	return(TodasOcupadas(this->grilla.Vecinos(pos)));
}

Campus CampusSeguro::campus() const{
	return this->grilla;
}

typename Conj<Nombre>::Iterador CampusSeguro::Estudiantes(){
}

typename Conj<Nombre>::Iterador CampusSeguro::Hippies(){
}

typename Conj<Agente>::Iterador CampusSeguro::Agentes(){
}

Posicion CampusSeguro::PosEstudianteYHippie(Nombre id){
}

Posicion CampusSeguro::PosAgente(Agente a){
}

Nat CampusSeguro::CantSanciones(Agente a){
}

Nat CampusSeguro::CantHippiesAtrapados(Agente a){
}

Agente CampusSeguro::MasVigilante(){
}

Conj<Agente> CampusSeguro::ConMismasSanciones(Agente a){
}

Conj<Agente> CampusSeguro::ConKSanciones(Nat k){
}

