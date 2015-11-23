#include "Driver.h"
#include "mini_test.h"
#include "diccNat.h"

#include <string>
#include <iostream>

using namespace aed2;

void test_crearDic(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 10;i++){
    diccNat<Nat>::tupla t;
    t.clave = i;
    t.significado = i+10;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
}

void test_obtenerDefinidoYredefinir(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 10000;i++){
    diccNat<Nat>::tupla t;
    t.clave = i+i;
    t.significado = i+10;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  if(d.definido(6)){
    Nat res = *(d.obtener(9999+9999));
    d.redefinir(6, 8);
    ASSERT_EQ(*(d.obtener(6)),8);
  }else{
    ASSERT_EQ(2,3);
  }
}


void test_numeroClaves(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 10;i++){
    diccNat<Nat>::tupla t;
    t.clave = i;
    t.significado = i+10;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  ASSERT_EQ(d.cantClaves(), 10);
}

void test_iteradorClaves(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 10;i++){
    diccNat<Nat>::tupla t;
    t.clave = i;
    t.significado = i+10;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  Conj<Nat>::Iterador it = d.crearItClaves();
  bool res = false;
  while(it.HaySiguiente() && it.Siguiente() != 3){
    it.Avanzar();
  }
  ASSERT_EQ(it.Siguiente(), 3);
}

void test_itDiccNat(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 10;i++){
    diccNat<Nat>::tupla t;
    t.clave = i;
    t.significado = i+10;
    std::cout << t.clave << std::endl;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  diccNat<Nat>::itDiccNat it = d.crearIt();
  std::cout << "crea el it" << std::endl;
  it.haySiguiente();
  //std::cout << it.siguienteSignificado() << std::endl; //ESTO NO FUNCIONA

  while(it.haySiguiente()){
    std::cout << "entra" << std::endl;
    std::cout << (it.siguiente()).clave << std::endl; //ESTO NO FUNCIONA PORQUE TIRA SEGMENTATION FAULT
    it.avanzar();
    std::cout << "sale" << std::endl;
  }
  //ASSERT_EQ(it.siguiente(), 9);
}


int main(int argc, char **argv)
{
  RUN_TEST(test_crearDic);
  RUN_TEST(test_obtenerDefinidoYredefinir);
  RUN_TEST(test_numeroClaves);
  RUN_TEST(test_iteradorClaves);
  RUN_TEST(test_itDiccNat);
	return 0;
}
