#include "estructuras.h"
#include "objeto.h"
#include "interaccion.h"
#include "sala.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINEA 1024
#define SON_IGUALES 0

void agregar_objeto(struct objeto ***vector, int *cantidad, struct objeto *obj)
{
        struct objeto **nuevo_vector = realloc(*vector, (long unsigned int)((*cantidad)+1)*(sizeof(struct objeto*)));

	if (nuevo_vector == NULL) {
	        return;
	}	

	nuevo_vector[*cantidad] = obj;
	(*cantidad)++;

	*vector = nuevo_vector;
}

void agregar_interaccion(struct interaccion ***vector, int *cantidad, struct interaccion *interaccion)
{
        struct interaccion **nuevo_vector = realloc(*vector, (long unsigned int)((*cantidad)+1)*(sizeof(struct interaccion*)));

	if (nuevo_vector == NULL) {
	        return;
	}	

	nuevo_vector[*cantidad] = interaccion;
	(*cantidad)++;

	*vector = nuevo_vector;
}

sala_t *sala_crear_desde_archivos(const char *objetos, const char *interacciones)
{
        FILE *archivo_objetos = fopen(objetos, "r");

	if (archivo_objetos == NULL) {
	        return NULL;
	}

	FILE *archivo_interacciones = fopen(interacciones, "r");

	if (archivo_interacciones == NULL) {
		fclose(archivo_objetos);
		return NULL;
	}

	struct sala *sala_creada = malloc(sizeof(struct sala));

	if (sala_creada == NULL) {
		return NULL;
	}

	sala_creada->objetos = NULL;
	sala_creada->cantidad_objetos = 0;
	sala_creada->interacciones = NULL;
	sala_creada->cantidad_interacciones = 0;

	char linea[MAX_LINEA];

	while (fgets(linea, MAX_LINEA, archivo_objetos) != NULL) {
		struct objeto *obj = objeto_crear_desde_string(linea);
		if (obj != NULL) {
			agregar_objeto(&(sala_creada->objetos), &(sala_creada->cantidad_objetos), obj);
		}
	}

	fclose(archivo_objetos);

	if (sala_creada->cantidad_objetos == 0) {
		free(sala_creada);
		fclose(archivo_interacciones);
		return NULL;
	}

	while (fgets(linea, MAX_LINEA, archivo_interacciones) != NULL) {
		struct interaccion *interaccion = interaccion_crear_desde_string(linea);
		if (interaccion != NULL) {
			agregar_interaccion(&(sala_creada->interacciones), &(sala_creada->cantidad_interacciones), interaccion);
		}
	}

	fclose(archivo_interacciones);

	if (sala_creada->cantidad_interacciones == 0) {
		for (int i = 0; i < (sala_creada->cantidad_objetos); i++) {
			free((sala_creada->objetos[i]));
		}
		free(sala_creada->objetos);
		free(sala_creada);
		return NULL;
	}

	return sala_creada;
}

char **sala_obtener_nombre_objetos(sala_t *sala, int *cantidad)
{
	if (sala == NULL) {
		if (cantidad != NULL) {
			(*cantidad) = -1;
		}
		return NULL;
	}

	char **vector_nombres = malloc((unsigned) sala->cantidad_objetos * sizeof(char*));

	if (vector_nombres == NULL) {
		if (cantidad != NULL) {
			(*cantidad) = -1;
		}
		return NULL;
	}

	for (int i = 0; i < (sala->cantidad_objetos); i++) {
		vector_nombres[i] = sala->objetos[i]->nombre;
	}

	if (cantidad != NULL) {
		(*cantidad) = sala->cantidad_objetos;
	}

	return vector_nombres;
}

bool sala_es_interaccion_valida(sala_t *sala, const char *verbo, const char *objeto1,
				const char *objeto2)
{
	if (sala == NULL || verbo == NULL || objeto1 == NULL || objeto2 == NULL) {
		return false;
	}

	bool valido = false;

	for (int i = 0; i < (sala->cantidad_interacciones); i++) {
		if ((strcmp(objeto1, sala->interacciones[i]->objeto) == SON_IGUALES) && (strcmp(objeto2, sala->interacciones[i]->objeto_parametro) == SON_IGUALES) && 
		(strcmp(verbo, sala->interacciones[i]->verbo) == SON_IGUALES)) {
			valido  = true;
		}
	}

	return valido;
}

void sala_destruir(sala_t *sala)
{
	for(int i = 0; i < (sala->cantidad_objetos); i++) {
		free(sala->objetos[i]);
	}

	free(sala->objetos);
	
	for (int j = 0; j < (sala->cantidad_interacciones); j++) {
		free(sala->interacciones[j]);
	}
	
	free(sala->interacciones);
	free(sala);
}