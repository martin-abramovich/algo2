#include "estructuras.h"
#include "interaccion.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SON_IGUALES 0

void asignar_tipo_accion(char letra_accion, struct interaccion *interaccion)
{
	if (letra_accion == 'd') {
		interaccion->accion.tipo = DESCUBRIR_OBJETO;
	} else if (letra_accion == 'r') {
		interaccion->accion.tipo = REEMPLAZAR_OBJETO;
	} else if (letra_accion == 'e') {
		interaccion->accion.tipo = ELIMINAR_OBJETO;
	} else if (letra_accion == 'm') {
		interaccion->accion.tipo = MOSTRAR_MENSAJE;
	} else {
		interaccion->accion.tipo = ACCION_INVALIDA;
	}
}

struct interaccion *interaccion_crear_desde_string(const char *string)
{
	if (string == NULL) {
		return NULL;
	}

	char letra_accion;

	struct interaccion *interaccion = malloc(sizeof(struct interaccion));

	if (interaccion == NULL) {
		return NULL;
	}

	int elementos_leidos = sscanf(string, "%[^;];%[^;];%[^;];%c:%[^:]:%[^\n]\n", interaccion->objeto, interaccion->verbo, interaccion->objeto_parametro, &letra_accion, 
	interaccion->accion.objeto, interaccion->accion.mensaje);

	if (elementos_leidos != 6) {
		free(interaccion);
		return NULL;
	}

	if (strcmp(interaccion->objeto_parametro, "_") == SON_IGUALES) {
		strcpy(interaccion->objeto_parametro, "");
	}
	if (strcmp(interaccion->accion.objeto, "_") == SON_IGUALES) {
		strcpy(interaccion->accion.objeto, "");
	}

	asignar_tipo_accion(letra_accion, interaccion);	

	return interaccion;
}