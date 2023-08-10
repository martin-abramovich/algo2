#include "objeto.h"
#include "estructuras.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CANT_LEIDOS 3
#define SON_IGUALES 0

struct objeto *objeto_crear_desde_string(const char *string)
{
	if (!string)
		return NULL;

	char es_asible[MAX_TEXTO];
	struct objeto *objeto = malloc(sizeof(struct objeto));

	if (!objeto)
		return NULL;

	int elementos_leidos = sscanf(string, "%[^;];%[^;];%[^\n]\n", objeto->nombre, objeto->descripcion, es_asible);

	if (elementos_leidos != CANT_LEIDOS){
		free(objeto);
		return NULL;
	}

	if (strcmp(es_asible, "true") == SON_IGUALES){
		objeto->es_asible = true;
	} else if (strcmp(es_asible, "false") == SON_IGUALES){
		objeto->es_asible = false;
	} else {
		free(objeto);
		return NULL;
	}

	return objeto;
}