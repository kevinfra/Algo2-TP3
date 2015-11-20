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





int main(int argc, char **argv)
{
  RUN_TEST(test_crearDic);
	return 0;
}
