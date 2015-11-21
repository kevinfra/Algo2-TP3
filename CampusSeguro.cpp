#include "CampusSeguro.h"
#include "Tipos.h"

using namespace aed2;

//ComenzarRastrillaje
CampusSeguro::CampusSeguro(const class Campus& c, diccNat<datosAgente>& d) : grilla(c), personalAS(d) {
	this->listaMismasSanc = generarListaMismasSanc(d);
	this-> posicionesAgente = vectorizarPos(d,campus().Filas(), campus().Columnas());
	this->masVigilante = menorPlaca(d);
	this->mismasSancModificado = true;
	//los diccionarios con hippies y estudiantes deben iniciar vacios

	Nat i = 0;
	while(i < campus().Filas()*campus().Columnas()){
		this->posicionesHippies.AgregarAtras(" ");
		this->posicionesEstudiantes.AgregarAtras(" ");
		i++;
	}

	typename diccNat<datosAgente>::itDiccNat it = d.crearIt();
	bool ordenado;
	while(it.haySiguiente()){
		i = 0;
		ordenado = true;
		while(i < this->agentesOrdenados.Longitud() && ordenado){
			if (this->agentesOrdenados[i].agente > it.siguiente().clave){
				ordenado = false;
			}
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
	typename diccNat<datosAgente>::itDiccNat itVacio;

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

		/* 
		 *	Creo que si le pongo Clave() deberia devolverme eso mismo.
		 *	Hay que testear esto bien.
		 */

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

void CampusSeguro::IngresarHippie(const Nombre h, const Posicion pos){
}

void CampusSeguro::IngresarEstudiante(const Nombre e, const Posicion pos){
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

Conj<Posicion> CampusSeguro::EstudiantesRodeadosAs(const Conj<Posicion>& c){
	typename Conj<Posicion>::const_Iterador itC = c.CrearIt();
	Conj<Posicion> res;

	while(itC.HaySiguiente()){
		if(TodasOcupadas(campus().Vecinos(itC.Siguiente())) && AlMenosUnAgente(campus().Vecinos(itC.Siguiente()))){
			res.AgregarRapido(itC.Siguiente());
		}
		itC.Avanzar();
	}
	
	return res;
}

Conj<NombrePosicion> CampusSeguro::EstudiantesRodeadosHippies(const Conj<Posicion>& c){
	typename Conj<Posicion>::const_Iterador itC = c.CrearIt();
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

bool CampusSeguro::HippiesAtrapando(const Conj<Posicion>& c){
	Nat i = 0;
	typename Conj<Posicion>::const_Iterador itC = c.CrearIt();

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

Conj<NombrePosicion> CampusSeguro::HippiesRodeadosAs(const Conj<Posicion>& c){
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

Conj<As> CampusSeguro::AgParaPremSanc(const Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<As> res;

	while(itC.HaySiguiente()){
		if(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x].datos.HaySiguiente()){
			res.AgregarRapido(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x]);
		}
		itC.Avanzar();
	}

	return res;
}

void CampusSeguro::PremiarAgentes(const Conj<As>& c){
	typename Conj<As>::Iterador itC = c.CrearIt();
	
	while(itC.HaySiguiente()){
		itC.Siguiente().datos.Siguiente().cantAtrapados++;

		if(this->masVigilante.datos.Siguiente().cantAtrapados < itC.Siguiente().cantAtrapados){
			this->masVigilante = itC.Siguiente();
		}
		itC.Avanzar();
	}

}

Nat CampusSeguro::CantHippiesVecinos(const Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Nat res = 0;

	while(itC.HaySiguiente()){
		typename DiccString(posicion)::Iterador itDic = this->hippies.CrearIt();

		while(itDic.HaySiguiente()){
			if(itDic.Siguiente() = itC.Siguiente()) res++;
			itDic.Avanzar();
		}
		itC.Avanzar();
	}

	return res;
}

bool CampusSeguro::TodosEstudiantes(const Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	bool res = true;

	while(itC.HaySiguiente()){
		if(this->posicionesEstudiantes[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] == " ") res = false;

		itC.Avanzar();
	}
	return res;
}

bool CampusSeguro::TodasOcupadas(const Conj<Posicion>& c){
	bool res = false;
	typename Conj<Posicion>::Iterador itC = c.CrearIt();

	while(itC.HaySiguiente() && !res){
		if(this->posicionesHippies[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] != " ") res = true;

		if(this->posicionesEstudiantes[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x] != " ") res = true;
		
		itC.Avanzar();
	}

	itC = c.CrearIt();
	while(itC.HaySiguiente() && !res){
		if(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x].datos.HaySiguiente()) res = true;
	}

	itC = c.CrearIt();
	while(itC.HaySiguiente() && !res){
		if(campus().Ocupada(itC.Siguiente())) res = true;
		itC.Avanzar();
	}

	return res;
}

Conj<NombrePosicion> CampusSeguro::HippiesRodeadosEstudiantes(const Conj<Posicion>& c){
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

bool CampusSeguro::AlMenosUnAgente(const Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	bool res = false;

	while(itC.HaySiguiente()){
		if(this->posicionesAgente[itC.Siguiente().y * campus().Columnas() + itC.Siguiente().x].datos.HaySiguiente()) res =  true;

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

void CampusSeguro::MoverAgente(Agente a){
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

