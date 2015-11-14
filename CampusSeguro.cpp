#include "CampusSeguro.h"
#include "Tipos.h"

using namespace aed2;

CampusSeguro::CampusSeguro() {}

//ComenzarRastrillaje
CampusSeguro::CampusSeguro(Campus c, diccNat<datosAgente> d){
}

void CampusSeguro::IngresarEstudiante(Nombre e, Posicion p){
}

void CampusSeguro::IngresarHippie(Nombre h, Posicion p){
}

void CampusSeguro::MoverEstudiante(Nombre e, Direccion d){
}

void CampusSeguro::MoverHippie(Nombre h){
}

void CampusSeguro::MoverAgente(Agente a){
}

Campus CampusSeguro::campus(){
}

Conj<Nombre>::Iterador CampusSeguro::Estudiantes(){
}

Conj<Nombre>::Iterador CampusSeguro::Hippies(){
}

Conj<Agente>::Iterador CampusSeguro::Agentes(){
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

