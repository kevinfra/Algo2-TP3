#include "CampusSeguro.h"
#include "Tipos.h"

using namespace aed2;

CampusSeguro::CampusSeguro() {}

//ComenzarRastrillaje
CampusSeguro::CampusSeguro(Campus& c, diccNat<datosAgente>& d){
	this->campus = c;
	this->listaMismasSanc = generarListaMismasSanc(d);
	this->personalAS = d;
	this-> posicionesAgente = vectorizarPos(d,this->campus.Filas(),this->campus.Columnas());
	this->masVigilante = menorPlaca(d);
	this->mismasSancModificado = true;
	//los diccionarios con hippies y estudiantes deben iniciar vacios

	Nat i = 0;
	while(i < this->campus.Filas()*this->campus.Columnas()){
		this->posicionesHippies.AgregarAtras(" ");
		this->posicionesEstudiantes.AgregarAtras(" ");
		i++;
	}

	typename diccNat<datosAgente>::Iterador it = d.CrearIt();
	bool ordenado;
	while(it.HaySiguiente()){
		i = 0;
		ordenado = true;
		while(i < this->agentesOrdenados.Longitud() && ordenado){
			if (this->agentesOrdenados[i].agente > it.Siguiente().agente){
				ordenado = false;
			}
		}

		As tupla;
		tupla.agente = it.Siguente().agente;
		tupla.datos = it;
		this->agentesOrdenados.AgregarAtras(tupla);
		it.Avanzar();
	}
}

Vector<As>& CampusSeguro::vectorizarPos(diccNat<datosAgente>& d, Nat f, Nat c){
	Vector<As> res;
	Nat i = 0;
	diccNat<datosAgente> it = d.CrearIt();

	/*	Pruebo creando un iterador vacio de esta forma.
	 *	Hay que testearlo bien esto.
	 */
	typename diccNat<datosAgente>::Iterador itVacio;

	As tuplaVacia;
	tuplaVacia.agente = 0;
	tuplaVacia.datos = itVacio;

	while(i < f*c){
		res.AgregarAtras(tuplaVacia);
		i++;
	}
	
	As tuplaAAgregar;
	while(it.HaySiguiente()){
		typename diccNat(datosAgente)::Iterador itPos = it;

		/* 
		 *	Creo que si le pongo Clave() deberia devolverme eso mismo.
		 *	Hay que testear esto bien.
		 */

		tuplaAAgregar.agente = it.Clave();
		tuplaAAgregar.datos = itPos;
		res[it.Siguiente().posicion.y * c + it.Siguiente().posicion.x] = tuplaAAgregar;
		it.Avanzar();
	}
	
	return res;
}

As CampusSeguro::menorPlaca(diccNat<datosAgente>& d){
	typename diccNat<datosAgente>::Iterador it = d.CrearIt();
	//idem que en funcion anterior.
	Nat placaMenor = it.Clave();

	while(it.HaySiguiente()){
		if (it.Clave() < placaMenor){
			placaMenor = it.Clave();
			typename diccNat<datosAgente>::Iterador itMenor = it;
		}
		it.Avanzar();
	}
	As res;
	res.agente = placaMenor;
	res.datos = itMenor;
	return res;
}

Lista<kSanc>& CampusSeguro::generarListaMismasSanc(diccNat<datosAgente>& d){
	typename diccNat<datosAgente>::Iterador itDic = d.CrearIt();
	Lista<kSanc> res;
	typename Lista<kSanc>::Iterador itL = res.CrearIt();

	Conj<Agente> conj;
	kSanc tupla;
	tupla.sanc = 0;
	tupla.agentes = conj;
	res.AgregarAdelante(tupla);

	while(itDic.HaySiguiente()){
		typename Conj<Agente>::Iterador itC = res.agentes.AgregarRapido(itDic.Clave());
		itDic.Siguiente().itConjMismasSanc = itC;
		itDic.Siguiente().itMismasSanc = itL;
		
		itDic.Avanzar();
	}

}

void CampusSeguro::IngresarEstudiante(Nombre e, Posicion pos){
	if(todasOcupadas(this->campus.Vecinos(pos)) && AlMenosUnAgente(this->campus.Vecinos(pos)){
		Conj<As> conjAgParaSanc = AgParaPremSanc(this->campus.Vecinos(pos));
		SancionarAgentes(conjAgParaSanc);
	}

	if(CantHippiesVecinos(this->campus.Vecinos(pos)) < 2){
		this->estudiantes.Definir(e,pos);
		this->posicionesEstudiantes[pos.y * this->campus.Columnas() + pos.x] = e;
	} else{
		this->hippies.Definir(e,pos);
		this->posicionesHippies[pos.y * this->campus.Columnas() + pos.x] = e;
	}


	Conj<NombrePosicion> conjHippiesRodEst = HippiesRodeadosEstudiantes(this->campus.Vecinos(pos));

	if (conjHippiesRodEst.Cardinal() > 0) {
		typename Conj<NombrePosicion>::Iterador itHEst = conjHippiesRodEst.CrearIt();
		while(itHEst.HaySiguiente()){
			this->estudiantes.Definir(itHEst.Siguiente().nombre, itHEst.Siguiente().pos);
			this->hippies.Eliminar(itHEst.Siguiente().nombre);

			this->posicionesEstudiantes[itHEst.Siguiente().pos.y * this->campus.Columnas() + itHEst.Siguiente().pos.x] = this->posicionesHippies[itHEst.Siguiente().pos.y * this->campus.Columnas() + itHEst.Siguiente().pos.x];

			this->posicionesHippies[itHEst.Siguiente().pos.y * this->campus.Columnas() + itHEst.Siguiente().pos.x] = " ";

			itHEst.Avanzar();
		}
	}


	//Las Capturas se actualizan en HippiesRodeadosAs
	Conj<NombrePosicion> conjHIppiesRodAs = HippiesRodeadosAs(this->campus.Vecinos(pos));

	if(conjHippiesRodAs.Cardinal() > 0){
		typename Conj<NombrePosicion>::Iterador itHAs = conjHippiesRodAs.CrearIt();
		while(itHAs.HaySiguiente()){
			this->hippies.Eliminar(itHAs.Siguiente().nombre);
			this->posicionesHippies[itHAs.Siguiente().pos.y * this->campus.Columnas() + itHAs.Siguiente().pos.x];

			itHAs.Avanzar();
		}
	}


	Conj<NombrePosicion> conjEstRodHip = EstudiantesRodeadosHippies(this->campus.Vecinos(pos));

	if(conjEstRodHip.Cardinal() > 0){
		typename Conj<NombrePosicion>::Iterador itEstH = conjEstRodHip.CrearIt();
		while(itEstH.HaySiguiente()){
			this->estudiantes.Eliminar(itEstH.Siguiente().nombre);
			this->posicionesEstudiantes[itEstH.Siguiente().pos.y * this->campus.Columnas() + itEstH.Siguiente().pos.x] = " ";

			this->hippies.Definir(itEstH.Siguiente().nombre, itEstH.Siguiente().pos);
			this->posicionesHippies[itEstH.Siguiente().pos.y * this->campus.Columnas() + itEstH.Siguiente().pos.x] = itEstH.Siguiente().nombre;

			itEstH.Avanzar();
		}
	}

	
	Conj<Posicion> conjEstRodAs = EstudiantesRodeadosAs(this->campus.Vecinos(pos));

	if(conjEstRodAs.Cardinal() > 0){
		typename Conj<Posicion>::Iterador itEstAs = conjEstRodAs.CrearIt();

		while(itEstAs.HaySiguiente()){
			if(todasOcupadas(this->campus.Vecinos(itEstAs.Siguiente())) && AlMenosUnAgente(this->campus.Vecinos(itEstAs.Siguiente()))){
				Conj<As> conjAgParaSanc = AgParaPremSanc(this->campus.Vecinos(itEstAs.Siguiente()));
				SancionarAgentes(conjAgParaSanc);
			}
		}
	}

}

Conj<Posicion>& CampusSeguro::EstudiantesRodeadosAs(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<Posicion> res;

	while(itC.HaySiguiente()){
		if(TodasOcupadas(this->campus.Vecinos(itC.Siguiente())) && AlMenosUnAgente(this->campus.Vecinos(itC.Siguiente()))){
			res.AgregarRapido(itC.Siguiente());
		}
		itC.Avanzar();
	}
	
	return res;
}

Conj<NombrePosicion>& EstudiantesRodeadosHippies(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<NombrePosicion> res;
	NombrePosicion np;

	while(itC.HaySiguiente()){
		if(this->posicionesEstudiantes[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x] != " " && TodasOcupadas(this->campus.Vecinos(itC.Siguiente())) && HippiesAtrapando(this->campus.Vecinos(itC.Siguiente()))){
			np.nombre = this->posicionesEstudiantes[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x];
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
		if(this->posicionesHippies[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x] != " "){
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
		itC.Siguiente().datos.Siguiente().cantSanc++;

		typename Lista<kSanc>::Iterador itLis = itC.Siguiente().datos.Siguiente().itMismasSanc;
		
		if(itC.Siguiente().datos.Siguiente().itMismasSanc.HaySiguiente()){
			itC.Siguiente().datos.Siguiente().itMismasSanc.Avanzar();
			
			if(itC.Siguiente().datos.Siguiente().itMismasSanc.Siguiente().sanc != itC.Siguiente().datos.Siguiente().cantSanc){
				tuplaksanc.agentes = conjVacio;
				tupla.sanc = itC.Siguiente().datos.Siguiente().cantSanc;
				itC.Siguiente().datos.Siguiente().itMismasSanc.AgregarComoAnterior(tuplaksanc);

				itC.Siguiente().datos.Siguiente().itMismasSanc.Retroceder();
			}

		} else {
			tuplaksanc.agentes = conjVacio;
			tuplaksanc.sanc = itC.Siguiente().datos.Siguiente().cantSanc;
			itC.Siguiente().datos.Siguiente().itMismasSanc.AgregarComoSiguiente(tuplaksanc);

			itC.Siguiente().datos.Siguiente().itMismasSanc.Avanzar();
		}
	
		itC.Siguiente().datos.Siguiente().itConjMismasSanc.EliminarSiguiente();
		itC.Siguiente().datos.Siguiente().itConjMismasSanc = itC.Siguiente().datos.Siguiente().itMismasSanc.Siguiente().agentes.AgregarRapido(itC.Siguiente().agente);
		itC.Avanzar();
	}

}

Conj<NombrePosicion>& CampusSeguro:: HippiesRodeadosAs(Conj<Posicion>& c){
	typename Conj<Posicion>:: Iterador itC = c.CrearIt();
	Conj<NombrePosicion> res;
	NombrePosicion np;

	while(itC.HaySiguiente()){
		if(this->posicionesHippies[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x] != " " && TodasOcupadas(this->campus.Vecinos(itC.Siguiente())) && AlMenosUnAgente(this->campus.Vecinos(itC.Siguiente()))){
			np.nombre = this->posicionesHippies[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x];
			np.pos = itC.Siguiente();
			
			res.AgregarRapido(np);

			Conj<As> conjAgPremiar = AgParaPremSanc(this->campus.Vecinos(itC.Siguiente()));
			PremiarAgentes(conjAgPremiar);
		}
		itC.Avanzar();
	}

	return res;

}

Conj<As>& CampusSeguro::AgParaPremSanc(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<As> res;

	while(itC.HaySiguiente()){
		if(this->posicionesAgente[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x].datos.HaySiguiente()){
			res.AgregarRapido(this->posicionesAgente[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x]);
		}
		itC.Avanzar();
	}

	return res;
}

void CampusSeguro::PremiarAgentes(Conj<As>& c){
	typename Conj<As>::Iterador itC = c.CrearIt();
	
	while(itC.HaySiguiente()){
		itC.Siguiente().datos.Siguiente().cantAtrapados++;

		if(this->masVigilante.datos.Siguiente().cantAtrapados < itC.Siguiente().cantAtrapados){
			this->masVigilante = itC.Siguiente();
		}
		itC.Avanzar();
	}

}

Nat CampusSeguro::CantHippiesVecinos(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Nat res = 0;

	while(itC.HaySiguiente()){
		typename DiccString(posicion)::Iterador itDic = this->hippies.CrearIt();

		while(itD.HaySiguiente()){
			if(itDic.Siguiente() = itC.Siguiente()) res++;
			itD.Avanzar();
		}
		itC.Avanzar();
	}

	return res;
}

bool CampusSeguro::TodosEstudiantes(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	bool res = true;

	while(itC.HaySiguiente()){
		if(this->posicionesEstudiantes[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x] = " ") res = false;

		itC.Avanzar();
	}
	return res;
}

bool CampusSeguro::TodasOcupadas(Conj<Posicion>& c){
	bool res = false;
	typename Conj<Posicion>::Iterador itC = c.CrearIt();

	while(itC.HaySiguiente() && !res){
		if(this->posicionesHippies[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x] != " ") res = true;

		if(this->posicionesEstudiantes[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x] != " ") res = true;
		
		itC.Avanzar();
	}

	itC = c.CrearIt();
	while(itC.HaySiguiente() && !res){
		if(this->posicionesAgente[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x].datos.HaySiguiente()) res = true;
	}

	itC = c.CrearIt();
	while(itC.HaySiguiente() && !res){
		if(this->campus.Ocupada(itC.Siguiente())) res = true;
		itC.Avanzar();
	}

	return res;
}

Conj<NombrePosicion>& HippiesRodeadosEstudiantes(Conj<Posicion>& c){
	typename Conj<Posicion>::Iterador itC = c.CrearIt();
	Conj<NombrePosicion> conjuntoRetorno;
	NombrePosicion np;

	while(itC.HaySiguiente()){
		if(this->posicionesHippies[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x] != " " && TodasOcupadas(this->campus.Vecinos(itC.Siguiente())) && TodosEstudiantes(this->campus.Vecinos(itC.Siguiente()))){
			np.nombre = this->posicionesHippies[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x];
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
		if(this->posicionesAgente[itC.Siguiente().y * this->campus.Columnas() + itC.Siguiente().x].datos.HaySiguiente()) res =  true;

		itC.Avanzar();
	}

	return res;
}

void CampusSeguro::IngresarHippie(Nombre h, Posicion pos){
	if(TodasOcupadas(this->campus.Vecinos(pos)) && AlMenosUnAgente(this->campus.Vecinos(pos))){
		Conj<As> conjAgParaPrem = AgParaPremSanc(this->campus.Vecinos(pos));
		PremiarAgentes(conjAgParaPrem);
	} else if(TodasOcupadas(this->campus.Vecinos(pos)) && TodosEstudiantes(this->campus.Vecinos(pos))){
		this->estudiantes.Definir(h,pos);
		this->posicionesEstudiantes[pos.y * this->campus.Columnas() + pos.x] = h;
	} else {
		this->hippies.Definir(h,pos);
		this->posicionesHippies[pos.y * this->campus.Columnas() + pos.x] = h;
	}

	
	Conj<NombrePosicion> conjHippiesRodEst = HippiesRodeadosEstudiantes(this->campus.Vecinos(pos));

	if (conjHippiesRodEst.Cardinal() > 0) {
		typename Conj<NombrePosicion>::Iterador itHEst = conjHippiesRodEst.CrearIt();
		while(itHEst.HaySiguiente()){
			this->estudiantes.Definir(itHEst.Siguiente().nombre, itHEst.Siguiente().pos);
			this->hippies.Eliminar(itHEst.Siguiente().nombre);

			this->posicionesEstudiantes[itHEst.Siguiente().pos.y * this->campus.Columnas() + itHEst.Siguiente().pos.x] = this->posicionesHippies[itHEst.Siguiente().pos.y * this->campus.Columnas() + itHEst.Siguiente().pos.x];

			this->posicionesHippies[itHEst.Siguiente().pos.y * this->campus.Columnas() + itHEst.Siguiente().pos.x] = " ";

			itHEst.Avanzar();
		}
	}


	//Las Capturas se actualizan en HippiesRodeadosAs
	Conj<NombrePosicion> conjHippiesRodAs = HippiesRodeadosAs(this->campus.Vecinos(pos));

	if(conjHippiesRodAs.Cardinal() > 0){
		typename Conj<NombrePosicion>::Iterador itHAs = conjHippiesRodAs.CrearIt();
		while(itHAs.HaySiguiente()){
			this->hippies.Eliminar(itHAs.Siguiente().nombre);
			this->posicionesHippies[itHAs.Siguiente().pos.y * this->campus.Columnas() + itHAs.Siguiente().pos.x];

			itHAs.Avanzar();
		}
	}


	Conj<NombrePosicion> conjEstRodHip = EstudiantesRodeadosHippies(this->campus.Vecinos(pos));

	if(conjEstRodHip.Cardinal() > 0){
		typename Conj<NombrePosicion>::Iterador itEstH = conjEstRodHip.CrearIt();
		while(itEstH.HaySiguiente()){
			this->estudiantes.Eliminar(itEstH.Siguiente().nombre);
			this->posicionesEstudiantes[itEstH.Siguiente().pos.y * this->campus.Columnas() + itEstH.Siguiente().pos.x] = " ";

			this->hippies.Definir(itEstH.Siguiente().nombre, itEstH.Siguiente().pos);
			this->posicionesHippies[itEstH.Siguiente().pos.y * this->campus.Columnas() + itEstH.Siguiente().pos.x] = itEstH.Siguiente().nombre;

			itEstH.Avanzar();
		}
	}

	
	Conj<Posicion> conjEstRodAs = EstudiantesRodeadosAs(this->campus.Vecinos(pos));

	if(conjEstRodAs.Cardinal() > 0){
		typename Conj<Posicion>::Iterador itEstAs = conjEstRodAs.CrearIt();

		while(itEstAs.HaySiguiente()){
			if(TodasOcupadas(this->campus.Vecinos(itEstAs.Siguiente())) && AlMenosUnAgente(this->campus.Vecinos(itEstAs.Siguiente()))){
				Conj<As> conjAgParaSanc = AgParaPremSanc(this->campus.Vecinos(itEstAs.Siguiente()));
				SancionarAgentes(conjAgParaSanc);
			}
		}
	}

}

void CampusSeguro::MoverEstudiante(Nombre e, Direccion d){
}

void CampusSeguro::MoverHippie(Nombre h){
}

void CampusSeguro::MoverAgente(Agente a){
}

Campus CampusSeguro::campus(){
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

