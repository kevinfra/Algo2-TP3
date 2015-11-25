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
	tuplaVacia.datos = itVacio;

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

	//creo el iterador con el diccionario
	typename diccNat<datosAgente>::itDiccNat itMenor = d.crearIt();
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


Conj<Posicion> CampusSeguro::EstudiantesRodeadosAs(Conj<Posicion>& c){
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


Conj<NombrePosicion> CampusSeguro::EstudiantesRodeadosHippies(Conj<Posicion>& c){
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


bool CampusSeguro::HippiesAtrapando(Conj<Posicion>& c){
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


void CampusSeguro::SancionarAgentes(Conj<As>& c){
	typename Conj<As>::Iterador itC = c.CrearIt();
	typename diccNat<datosAgente>::itDiccNat itParaMod;

	if(c.Cardinal() > 0){
		this->mismasSancModificado = true;
	}

	kSanc tuplaksanc;
	Conj<Agente> conjVacio;

	while(itC.HaySiguiente()){
		itParaMod = itC.Siguiente().datos;
		itParaMod.siguiente().significado.cantSanc++;

		typename Lista<kSanc>::Iterador itLis = itParaMod.siguiente().significado.itMismasSanc;
		
		if(itParaMod.siguiente().significado.itMismasSanc.HaySiguiente()){
			itParaMod.siguiente().significado.itMismasSanc.Avanzar();
			
			if(itParaMod.siguiente().significado.itMismasSanc.Siguiente().sanc != itParaMod.siguiente().significado.cantSanc){
				tuplaksanc.agentes = conjVacio;
				tuplaksanc.sanc = itParaMod.siguiente().significado.cantSanc;
				itParaMod.siguiente().significado.itMismasSanc.AgregarComoAnterior(tuplaksanc);

				itParaMod.siguiente().significado.itMismasSanc.Retroceder();
			}

		} else {
			tuplaksanc.agentes = conjVacio;
			tuplaksanc.sanc = itParaMod.siguiente().significado.cantSanc;
			itParaMod.siguiente().significado.itMismasSanc.AgregarComoSiguiente(tuplaksanc);

			itParaMod.siguiente().significado.itMismasSanc.Avanzar();
		}
	
		itParaMod.siguiente().significado.itConjMismasSanc.EliminarSiguiente();
		itParaMod.siguiente().significado.itConjMismasSanc = itParaMod.siguiente().significado.itMismasSanc.Siguiente().agentes.AgregarRapido(itC.Siguiente().agente);
		itC.Avanzar();
	}

}


Conj<NombrePosicion> CampusSeguro::HippiesRodeadosAs(Conj<Posicion>& c){
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


Conj<CampusSeguro::As> CampusSeguro::AgParaPremSanc(Conj<Posicion>& c){
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


void CampusSeguro::PremiarAgentes(Conj<As>& c){
	typename Conj<As>::Iterador itC = c.CrearIt();
	typename diccNat<datosAgente>::itDiccNat itParaMod;
	
	while(itC.HaySiguiente()){
		itParaMod = itC.Siguiente().datos;
		itParaMod.siguiente().significado.cantAtrapados++;

		if(this->masVigilante.datos.siguiente().significado.cantAtrapados < itParaMod.siguiente().significado.cantAtrapados){
			this->masVigilante = itC.Siguiente();
		}
		itC.Avanzar();
	}

}


Nat CampusSeguro::CantHippiesVecinos(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Nat res = 0;

	while(itC.HaySiguiente()){
		typename DiccString<Posicion>::Iterador itDic = this->hippies.CrearIt();

		while(itDic.HaySiguiente()){
			if(itDic.SiguienteSignificado() == itC.Siguiente()) res++;
			itDic.Avanzar();
		}
		itC.Avanzar();
	}

	return res;
}


bool CampusSeguro::TodosEstudiantes(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	bool res = true;

	while(itC.HaySiguiente()){
		if(this->posicionesEstudiantes[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] == " ") res = false;

		itC.Avanzar();
	}
	return res;
}


bool CampusSeguro::TodasOcupadas(Conj<Posicion>& c){
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


Conj<NombrePosicion> CampusSeguro::HippiesRodeadosEstudiantes(Conj<Posicion>& c){
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


bool CampusSeguro::AlMenosUnAgente(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	bool res = false;

	while(itC.HaySiguiente()){
		if(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x].datos.haySiguiente()) res =  true;

		itC.Avanzar();
	}

	return res;
}


void CampusSeguro::IngresarHippie(Nombre h, Posicion pos){
	if(TodasOcupadas(campus().Vecinos(pos)) && AlMenosUnAgente(campus().Vecinos(pos))){
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


void CampusSeguro::MoverEstudiante(Nombre e, Direccion d){
	Posicion actualPos = this->estudiantes.Obtener(e);
	Posicion pos = actualPos;

	if(d == izq){
		pos.x = pos.x -1;
	} else if(d == der){
		pos.x = pos.x +1;
	} else if(d == arriba){
		pos.y = pos.y +1;
	} else if(d == abajo){
		pos.y = pos.y -1;
	}

	if(!(pos.y == 0 || pos.y == campus().Filas()+1)){
		if(CantHippiesVecinos(campus().Vecinos(pos)) < 2){
			this->estudiantes.Definir(e, pos);
			this->posicionesEstudiantes[actualPos.y * campus().Columnas() + actualPos.x] = " ";
			this->posicionesEstudiantes[pos.y * campus().Columnas() + pos.x] = e;
		} else {
			this->hippies.Definir(e, pos);
			this->posicionesEstudiantes[actualPos.y * campus().Columnas() + actualPos.x] = " ";
			this->posicionesHippies[pos.y * campus().Columnas() + pos.x] = e;
		}

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


void CampusSeguro::MoverHippie(Nombre h){
	Posicion actualPos = this->hippies.Obtener(h);
	Posicion pos = proxPos(actualPos, this->estudiantes);

	if (actualPos != pos){
		this->posicionesHippies[actualPos.y * campus().Columnas() + actualPos.x] = " ";
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


// TODO: testear
void CampusSeguro::MoverAgente(Agente a){
	typename diccNat<datosAgente>::itDiccNat it = busqBinPorPlaca(a, this->agentesOrdenados);

	// Actualizo la posicion del agente
	Posicion nuevaPos = proxPos(it.siguiente().significado.posicion, this->hippies);
	posicionesAgente[it.siguiente().significado.posicion.y * campus().Columnas() + it.siguiente().significado.posicion.x].datos = diccNat<datosAgente>().crearIt();

	As as;
	as.agente = a;
	as.datos = it;
	posicionesAgente[nuevaPos.y * campus().Columnas() + nuevaPos.x] = as;

	it.siguiente().significado.posicion = nuevaPos;

	// Me fijo a quienes atrapa
	Posicion posArr = campus().MoverDir(nuevaPos, arriba);
	actualizarAgente(posArr, a, it);

	Posicion posAba = campus().MoverDir(nuevaPos, abajo);
	actualizarAgente(posAba, a, it);

	Posicion posDer = campus().MoverDir(nuevaPos, der);
	actualizarAgente(posDer, a, it);

	Posicion posIzq = campus().MoverDir(nuevaPos, izq);
	actualizarAgente(posIzq, a, it);
}

// TODO: testear

typename diccNat<datosAgente>::itDiccNat CampusSeguro::busqBinPorPlaca(Agente a, Vector<As>& v){
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
Posicion CampusSeguro::proxPos(Posicion pos, DiccString<Posicion>& dicc){
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
Nat CampusSeguro::distanciaMasCorta(Posicion pos, DiccString<Posicion>& dicc){
	typename DiccString<Posicion>::Iterador it = dicc.CrearIt();

	Nat dist = distancia(pos, it.SiguienteSignificado());
	it.Avanzar();

	while(it.HaySiguiente()){
		if (dist > distancia(pos, it.SiguienteSignificado()))
			dist = distancia(pos, it.SiguienteSignificado());
		it.Avanzar();
	}

	return dist;
}

Nat CampusSeguro::distancia(Posicion pos1, Posicion pos2){
	return(modulo(pos1.x - pos2.x) + modulo(pos1.y - pos2.y));
}

Nat CampusSeguro::modulo(int val){
	if(val >= 0)
		return (Nat)val;
	else
		return (Nat)-val;
}

// TODO: testear

Conj<Posicion> CampusSeguro::dondeIr(Posicion pos, Nat dist, DiccString<Posicion>& dicc){
	Conj<Posicion> posiciones = Conj();
	typename DiccString<Posicion>::Iterador it = dicc.CrearIt();

	while(it.HaySiguiente()){
		if(dist == distancia(pos, it.SiguienteSignificado()))
			posiciones.AgregarRapido(it.SiguienteSignificado());
		it.Avanzar();
	}

	return(posiciones);
}

// TODO: testear
Conj<Posicion> CampusSeguro::lugaresPosibles(Posicion pos, Conj<Posicion>& posiciones){
	typename Conj<Posicion>::Iterador it = posiciones.CrearIt();
	Conj<Posicion> lugares;
	Posicion auxPos;

	while(it.HaySiguiente()){
		if(hayAlgoEnPos(it.Siguiente())){
			if(it.Siguiente().x > pos.x){
				auxPos.x = pos.x + 1;
				auxPos.y = pos.y;
				lugares.AgregarRapido(auxPos);
			}
			else if(it.Siguiente().x < pos.x){
				auxPos.x = pos.x - 1;
				auxPos.y = pos.y;
				lugares.AgregarRapido(auxPos);
			}

			if(it.Siguiente().y > pos.y){
				auxPos.x = pos.x;
				auxPos.y = pos.y + 1;	
				lugares.AgregarRapido(auxPos);
			}
			else if(it.Siguiente().x < pos.x){
				auxPos.x = pos.x;
				auxPos.y = pos.y - 1;	
				lugares.AgregarRapido(auxPos);
			}
		}
	}

	return lugares;
}

// TODO: testear
bool CampusSeguro::hayAlgoEnPos(Posicion pos){
	if(this->posicionesAgente[pos.y * campus().Columnas() + pos.x].datos.haySiguiente())
		return true;

	if(this->posicionesHippies[pos.y * campus().Columnas() + pos.x] != " ")
		return true;
	if(this->posicionesEstudiantes[pos.y * campus().Columnas() + pos.x] != " ")
		return true;

	if(campus().Ocupada(pos))
		return true;

	return false;
}

// TODO: testear
void CampusSeguro::actualizarAgente(Posicion pos, Agente a, typename diccNat<datosAgente>::itDiccNat it){
	if(campus().PosValida(pos)){

		if(this->posicionesHippies[pos.y * campus().Columnas() + pos.x] != " "){
			if(atrapado(pos)){
				// Incremento sus capturas, actualizo masVigilante y mato al hippie
				it.siguiente().significado.cantAtrapados++;
				if(it.siguiente().significado.cantAtrapados > this->masVigilante.datos.siguiente().significado.cantAtrapados){
					As tup;
					tup.agente = a;
					tup.datos = it;
					this->masVigilante = tup;
				}
				this->hippies.Eliminar(this->posicionesHippies[pos.y * campus().Columnas() + pos.x]);
				posicionesHippies[pos.y * campus().Columnas() + pos.x] = " ";
			}
		}

		if(posicionesEstudiantes[pos.y * campus().Columnas() + pos.x] != " "){
			if(atrapado(pos)){
				//Actualizo las sanciones y las estructuras relacionadas
				this->mismasSancModificado = true;

				typename Conj<Agente>::Iterador iterConj = it.siguiente().significado.itConjMismasSanc;
				iterConj.EliminarSiguiente();

				typename Lista<kSanc>::Iterador iterLista = it.siguiente().significado.itMismasSanc;

				// Me guardo un iterador para borrar el nodo de la lista si es que queda sin agentes
				typename Lista<kSanc>::Iterador iterListaAnterior = it.siguiente().significado.itMismasSanc;			// Esto va a funcionar? Que estos dos iteradores sean iguales no genera algun problema de referencia o algo asi?

				if(iterLista.HaySiguiente()){
					// Me fijo si el siguiente es la siguiente sancion
					Nat sanciones = iterLista.Siguiente().sanc;
					iterLista.Avanzar();

					if(iterLista.Siguiente().sanc == sanciones + 1){
						// Lo agrego al conjunto
						it.siguiente().significado.itConjMismasSanc = iterLista.Siguiente().agentes.AgregarRapido(a);
					}
					else{
						// Creo un nuevo nodo en el medio
						Conj<Agente> conj;
						it.siguiente().significado.itConjMismasSanc = conj.AgregarRapido(a);

						kSanc nodo;
						nodo.sanc = sanciones + 1;
						nodo.agentes = conj;
						iterLista.AgregarComoAnterior(nodo);

						iterLista.Retroceder();

						it.siguiente().significado.itMismasSanc = iterLista;
					}
				}
				else{
					//Creo un nuevo nodo
					Conj<Agente> conj;
					it.siguiente().significado.itConjMismasSanc = conj.AgregarRapido(a);

					kSanc nodo;
					nodo.sanc = 1; // Esto está mal
					nodo.agentes = conj;
					iterLista.AgregarComoSiguiente(nodo);

					iterLista.Avanzar();

					it.siguiente().significado.itMismasSanc = iterLista;
				}

				if(!iterConj.HaySiguiente()){
					// Borro el nodo anterior de la lista porque no tiene agentes
					iterListaAnterior.EliminarSiguiente();
				}

				it.siguiente().significado.cantSanc++;
			}
		}
	}
}

// TODO: testear
bool CampusSeguro::atrapado(Posicion pos){
	return(TodasOcupadas(campus().Vecinos(pos)));
}


Campus CampusSeguro::campus() const{
	return this->grilla;
}

typename Conj<Nombre>::Iterador CampusSeguro::Estudiantes(){
	return this->estudiantes.CrearItClaves();
}

typename Conj<Nombre>::Iterador CampusSeguro::Hippies(){
	return this->hippies.CrearItClaves();
}

typename Conj<Agente>::Iterador CampusSeguro::Agentes() {
	return this->personalAS.crearItClaves();
}

// Pre: id esta en hippies o en estudiantes
Posicion CampusSeguro::PosEstudianteYHippie(Nombre id){
	if(this->hippies.Definido(id))
		return this->hippies.Obtener(id);
	else
		return this->estudiantes.Obtener(id);
}

// Pre: a pertenece a personalAS
Posicion CampusSeguro::PosAgente(Agente a){
	return this->personalAS.obtener(a)->posicion;
}

// Pre: a pertenece a personalAS
Nat CampusSeguro::CantSanciones(Agente a) {
	return this->personalAS.obtener(a)->cantSanc;
}

// Pre: a pertenece a personalAS
Nat CampusSeguro::CantHippiesAtrapados(Agente a) {
	return this->personalAS.obtener(a)->cantAtrapados;
}

Agente CampusSeguro::MasVigilante(){
	return this->masVigilante.agente;
}

// Pre: a pertenece a personalAS
Conj<Agente> CampusSeguro::ConMismasSanciones(Agente a) {
	return personalAS.obtener(a)->itMismasSanc.Siguiente().agentes;
}

// TODO: hacer las auxiliares y descomentar
Conj<Agente> CampusSeguro::ConKSanciones(Nat k) {
	if(this->mismasSancModificado){
//		hacerArregloMismasSanc();
		this->mismasSancModificado = false;
	}

	Nat i = 0;
//	bool esta = busqBinAgente(k, i, this->arregloMismasSanc);
//	if(esta)
//		return this->arregloMismasSanc[i].Siguiente().agentes;
//	else
		return Conj<Agente>();
}

