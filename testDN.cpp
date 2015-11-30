#include "Driver.h"
#include "mini_test.h"
#include "diccNat.h"
#include "aed2.h"

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
  for(Nat i = 0;i < 100;i++){
    diccNat<Nat>::tupla t;
    t.clave = i+i;
    t.significado = i+10;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  if(d.definido(6)){
    d.redefinir(6, 8);
    ASSERT_EQ((d.obtener(6)),8);
  }
}

void test_obtener(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 10;i++){
    diccNat<Nat>::tupla t;
    t.clave = i;
    t.significado = i+10;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  if(d.definido(6)){
    ASSERT_EQ(d.obtener(6),16);
    d.redefinir(6, 8);
    ASSERT_EQ((d.obtener(6)),8);
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
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  diccNat<Nat>::itDiccNat it = d.crearIt();

  Nat k = 0;
  while(it.haySiguiente()){
    ASSERT(it.siguiente().clave == k);
    ASSERT(it.siguiente().significado == k + 10);
    it.avanzar();
    k++;
  }
}

void test_Definido(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 10;i++){
    diccNat<Nat>::tupla t;
    t.clave = i;
    t.significado = i+10;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  assert(d.definido(0));
  assert(d.definido(1));
  assert(d.definido(2));
  assert(d.definido(3));
  assert(d.definido(4));
  assert(!d.definido(10));
  assert(!d.definido(355));
  Conj<Nat>::Iterador it = d.crearItClaves();
  bool res = false;
  while(it.HaySiguiente() && it.Siguiente() != 3){
    it.Avanzar();
  }
  ASSERT_EQ(it.Siguiente(), 3);
  for(int j = 0;j<=4;j++){
    it.Retroceder();
  }
  int i = 0;
  while(it.HaySiguiente()){
    i++;
    it.Avanzar();
  }
  assert(i==9);
}


void test_IteradorAFondo(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 6;i++){
    diccNat<Nat>::tupla t;
    t.clave = i;
    t.significado = i*2;
    v.AgregarAtras(t);
  }
  diccNat<Nat> d(v);
  assert(d.definido(0));
  assert(d.definido(1));
  assert(d.definido(2));
  assert(d.definido(3));
  assert(d.definido(5));
  assert(!d.definido(6));
  assert(!d.definido(355));
  diccNat<Nat>::itDiccNat it = d.crearIt();
  assert(it.siguienteSignificado() == 0);
  assert(it.siguiente().clave == 0);
  assert(it.siguiente().significado == 0);
  while(it.haySiguiente() && it.siguiente().clave != 3){
    it.avanzar();
  }
  ASSERT_EQ(it.siguiente().clave, 3);
  assert(it.siguiente().significado == 6);
  assert(it.siguienteSignificado() == 6);
  for(int j = 0;j<=2;j++){
    it.Retroceder();
  }
  int i = 0;
  while(it.haySiguiente()){
    i++;
    it.avanzar();
  }
  assert(i==6);
}

void test_DiccConNew(){
  Vector< diccNat<Nat>::tupla > v;
  for(Nat i = 0;i < 10;i++){
    diccNat<Nat>::tupla t;
    t.clave = i;
    t.significado = i+10;
    v.AgregarAtras(t);
  }
  diccNat<Nat> *d = new diccNat<Nat>(v);
  assert(d->definido(0));
  assert(d->definido(1));
  assert(d->definido(2));
  assert(d->definido(3));
  assert(d->definido(4));
  assert(!d->definido(10));
  assert(!d->definido(355));
  Conj<Nat>::Iterador it = d->crearItClaves();
  bool res = false;
  while(it.HaySiguiente() && it.Siguiente() != 3){
    it.Avanzar();
  }
  ASSERT_EQ(it.Siguiente(), 3);
  for(int j = 0;j<=4;j++){
    it.Retroceder();
  }
  int i = 0;
  while(it.HaySiguiente()){
    i++;
    it.Avanzar();
  }
  assert(i==9);
  delete d;
}

int main(int argc, char **argv)
{
  RUN_TEST(test_crearDic);
  RUN_TEST(test_obtener);
  RUN_TEST(test_obtenerDefinidoYredefinir);
  RUN_TEST(test_numeroClaves);
  RUN_TEST(test_iteradorClaves);
  RUN_TEST(test_itDiccNat);
  RUN_TEST(test_Definido);
  RUN_TEST(test_IteradorAFondo);
  RUN_TEST(test_DiccConNew);
	return 0;
}
