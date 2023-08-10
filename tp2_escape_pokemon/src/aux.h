#ifndef AUX_H_
#define AUX_H_

#include <stdbool.h>
#include <stdlib.h>
#include "lista.h"
#include "hash.h"

struct sala {
	hash_t *objetos;
	lista_t *interacciones;
	hash_t *objetos_conocidos;
	hash_t *objetos_poseidos;
	lista_t *todos_los_objetos;
	bool escapo;
};

#endif