#ifndef AED2_TIPOS_H_
#define AED2_TIPOS_H_

#include "aed2/TiposBasicos.h"

namespace aed2{

	typedef struct Posicion {Nat x,y;
					bool operator==(const Posicion& pos) const
					{
						return (x == pos.x && y == pos.y);
					}
				} Posicion;
	typedef enum {izq,der,arriba,abajo} Direccion;
	typedef Nat Agente;
	typedef String Nombre;

} // namespace aed2

#endif // AED2_TIPOS_H_
