#include "estructuras.h"
#include "sala.h"
#include "objeto.h"
#include "interaccion.h"
#include "lista.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINEA 1024
#define SON_IGUALES 0

struct sala {
	hash_t *objetos;
	hash_t *objetos_conocidos;
	hash_t *objetos_poseidos;
	lista_t *interacciones;
	lista_t *todos_los_objetos;
	bool escapo;
};

struct vector_con_indice{
	void **vector;
	size_t tamanio;
	size_t indice;
};

sala_t *sala_crear_desde_archivos(const char *objetos, const char *interacciones)
{
    struct sala *sala_creada = malloc(sizeof(struct sala));

	if (!sala_creada)
		return NULL;

	sala_creada->objetos = hash_crear(10);
	sala_creada->interacciones = lista_crear();
	sala_creada->todos_los_objetos = lista_crear();
	sala_creada->objetos_conocidos = hash_crear(10);
	sala_creada->objetos_poseidos = hash_crear(10);
	sala_creada->escapo = false;

	if (!sala_creada->objetos || !sala_creada->interacciones || !sala_creada->objetos_poseidos || !sala_creada->objetos_conocidos){
		sala_destruir(sala_creada);
		return NULL;
	}
		
    FILE *archivo_objetos = fopen(objetos, "r");

	if (!archivo_objetos){
		sala_destruir(sala_creada);
	    return NULL;
	}
		
	FILE *archivo_interacciones = fopen(interacciones, "r");

	if (!archivo_interacciones){
		sala_destruir(sala_creada);
		fclose(archivo_objetos);
		return NULL;
	}

	char linea[MAX_LINEA];

	int i = 0;
	while (fgets(linea, MAX_LINEA, archivo_objetos) != NULL){
		struct objeto *objeto = objeto_crear_desde_string(linea);
		if (objeto != NULL){
			hash_insertar(sala_creada->objetos, objeto->nombre, objeto, NULL);
			lista_insertar(sala_creada->todos_los_objetos, objeto);
			if (i == 0)
				hash_insertar(sala_creada->objetos_conocidos, objeto->nombre, objeto, NULL);
			i++;
		}
	}

	fclose(archivo_objetos);

	if (hash_cantidad(sala_creada->objetos) == 0){
		sala_destruir(sala_creada);
		fclose(archivo_interacciones);
		return NULL;
	}

	while (fgets(linea, MAX_LINEA, archivo_interacciones) != NULL){
		struct interaccion *interaccion = interaccion_crear_desde_string(linea);
		if (interaccion != NULL)
			lista_insertar(sala_creada->interacciones, interaccion);
	}

	fclose(archivo_interacciones);

	if (lista_tamanio(sala_creada->interacciones) == 0){
		sala_destruir(sala_creada);
		return NULL;
	}

	return sala_creada;
}

bool agregar_clave_a_vector(const char *clave, void *valor, void *aux)
{
	struct vector_con_indice *mi_vector = aux;
	mi_vector->vector[mi_vector->indice] = (char*)clave;
	mi_vector->indice++;

	return true;
}

char **sala_obtener_nombre_objetos(sala_t *sala, int *cantidad)
{
	if (!sala){
		if (cantidad != NULL)
			(*cantidad) = -1;
		return NULL;
	}

	char **vector_nombres = malloc((unsigned) hash_cantidad(sala->objetos) * sizeof(char*));

	if (!vector_nombres){
		if (cantidad != NULL)
			(*cantidad) = -1;
		return NULL;
	}

	struct vector_con_indice mi_vector;
	mi_vector.vector = (void*)vector_nombres;
	mi_vector.tamanio = hash_cantidad(sala->objetos);
	mi_vector.indice = 0;
	hash_con_cada_clave(sala->objetos, agregar_clave_a_vector, &mi_vector);

	if (cantidad != NULL)
		(*cantidad) = (int)hash_cantidad(sala->objetos);

	return vector_nombres;
}

char **sala_obtener_nombre_objetos_conocidos(sala_t *sala, int *cantidad)
{
	if (!sala){
		if (cantidad != NULL)
			(*cantidad) = -1;
		return NULL;
	}

	char **vector_nombres = malloc((unsigned) hash_cantidad(sala->objetos_conocidos) * sizeof(char*));

	if (!vector_nombres){
		if (cantidad != NULL)
			(*cantidad) = -1;
		return NULL;
	}

	struct vector_con_indice mi_vector;
	mi_vector.vector = (void*)vector_nombres;
	mi_vector.tamanio = hash_cantidad(sala->objetos);
	mi_vector.indice = 0;
	hash_con_cada_clave(sala->objetos_conocidos, agregar_clave_a_vector, &mi_vector);

	if (cantidad != NULL)
		(*cantidad) = (int)hash_cantidad(sala->objetos_conocidos);

	return vector_nombres;	
}

char **sala_obtener_nombre_objetos_poseidos(sala_t *sala, int *cantidad)
{
	if (!sala){
		if (cantidad != NULL)
			(*cantidad) = -1;
		return NULL;
	}

	char **vector_nombres = malloc((unsigned) hash_cantidad(sala->objetos_poseidos) * sizeof(char*));

	if (!vector_nombres){
		if (cantidad != NULL)
			(*cantidad) = -1;
		return NULL;
	}

	struct vector_con_indice mi_vector;
	mi_vector.vector = (void*)vector_nombres;
	mi_vector.tamanio = hash_cantidad(sala->objetos);
	mi_vector.indice = 0;
	hash_con_cada_clave(sala->objetos_poseidos, agregar_clave_a_vector, &mi_vector);

	if (cantidad != NULL)
		(*cantidad) = (int)hash_cantidad(sala->objetos_poseidos);

	return vector_nombres;		
}

bool sala_agarrar_objeto(sala_t *sala, const char *nombre_objeto)
{
	if (!sala || !nombre_objeto)
		return false;

	if (hash_contiene(sala->objetos_poseidos, nombre_objeto))
		return false;

	if (hash_contiene(sala->objetos_conocidos, nombre_objeto)){
		struct objeto *objeto = hash_obtener(sala->objetos_conocidos, nombre_objeto);
		if (objeto && objeto->es_asible){
			hash_insertar(sala->objetos_poseidos, nombre_objeto, objeto, NULL);
			hash_quitar(sala->objetos_conocidos, nombre_objeto);
			return true;	
		}
	}

	return false;
}

char* sala_describir_objeto(sala_t* sala, const char *nombre_objeto)
{
	if (!sala || !nombre_objeto)
		return NULL;

	struct objeto *objeto;

	if (hash_contiene(sala->objetos_conocidos, nombre_objeto)){
		objeto = hash_obtener(sala->objetos_conocidos, nombre_objeto);
		return objeto->descripcion;
	}

	if (hash_contiene(sala->objetos_poseidos, nombre_objeto)){
		objeto = hash_obtener(sala->objetos_poseidos, nombre_objeto);
		return objeto->descripcion;
	}

	return NULL;
}

bool conozco(sala_t *sala, const char *nombre){
	return hash_contiene(sala->objetos_conocidos, nombre) || strcmp(nombre, "") == 0 || hash_contiene(sala->objetos_poseidos, nombre);
}

bool interaccion_coincide(struct interaccion *interaccion, const char *verbo, const char *objeto1, const char *objeto2)
{
	if (!interaccion || !verbo || !objeto1 || !objeto2)
		return false;
	
	if ((strcmp(objeto1, interaccion->objeto) == SON_IGUALES) && (strcmp(objeto2, interaccion->objeto_parametro) == SON_IGUALES) && 
	(strcmp(verbo, interaccion->verbo) == SON_IGUALES))
		return true;

	return false;
}

int ejecutar_interaccion(sala_t *sala, struct interaccion *interaccion,
			      void (*mostrar_mensaje)(const char *mensaje, enum tipo_accion accion, void *aux), void *aux)
{
	switch (interaccion->accion.tipo){

		case MOSTRAR_MENSAJE:

			if(conozco(sala, interaccion->objeto)){
				mostrar_mensaje(interaccion->accion.mensaje, MOSTRAR_MENSAJE, aux);
				return 1;
			}
				
			break;

		case DESCUBRIR_OBJETO:

			if (conozco(sala, interaccion->objeto) && conozco(sala, interaccion->objeto_parametro)){
				struct objeto *obj_descubierto = hash_obtener(sala->objetos, interaccion->accion.objeto);
				if (obj_descubierto && !conozco(sala, interaccion->accion.objeto)){
					hash_insertar(sala->objetos_conocidos, interaccion->accion.objeto, obj_descubierto, NULL);
					mostrar_mensaje(interaccion->accion.mensaje, DESCUBRIR_OBJETO, aux);
					return 1;
				}
			}

			break;

		case REEMPLAZAR_OBJETO:

			if (conozco(sala, interaccion->objeto_parametro)){

				hash_quitar(sala->objetos, interaccion->objeto_parametro);
				hash_quitar(sala->objetos_conocidos, interaccion->objeto_parametro);
				hash_quitar(sala->objetos_poseidos, interaccion->objeto_parametro);
				struct objeto *objeto = hash_obtener(sala->objetos, interaccion->accion.objeto);
				if (objeto)
					hash_insertar(sala->objetos_conocidos, interaccion->accion.objeto, objeto, NULL);
				mostrar_mensaje(interaccion->accion.mensaje, REEMPLAZAR_OBJETO, aux);
				return 1;
			}

			break;

		case ELIMINAR_OBJETO:

			if(conozco(sala, interaccion->objeto)){
				hash_quitar(sala->objetos, interaccion->objeto);
				hash_quitar(sala->objetos_conocidos, interaccion->objeto);
				hash_quitar(sala->objetos_poseidos, interaccion->objeto);
				mostrar_mensaje(interaccion->accion.mensaje, ELIMINAR_OBJETO, aux);
				return 1;
			}

			break;

		case ESCAPAR: ;
			
			if (hash_contiene(sala->objetos_conocidos, interaccion->objeto)){
				sala->escapo = true;
				mostrar_mensaje(interaccion->accion.mensaje, ESCAPAR, aux);
				return 1;
			}

			break;

		case ACCION_INVALIDA:
			return 0;
	}

	return 0;
}

int sala_ejecutar_interaccion(sala_t *sala, const char *verbo, const char *objeto1, const char *objeto2,
			      void (*mostrar_mensaje)(const char *mensaje, enum tipo_accion accion, void *aux), void *aux)
{
	if (!sala || !verbo || !objeto1 || !objeto2 || !mostrar_mensaje)
		return 0;

	int ejecutadas = 0;

	lista_iterador_t *it = lista_iterador_crear(sala->interacciones);

	for (; lista_iterador_tiene_siguiente(it); lista_iterador_avanzar(it)){
		struct interaccion *interaccion = lista_iterador_elemento_actual(it);
		if(interaccion_coincide(interaccion, verbo, objeto1, objeto2))
			ejecutadas += ejecutar_interaccion(sala, interaccion, mostrar_mensaje, aux);
	}
	lista_iterador_destruir(it);

	return ejecutadas;
}

bool sala_es_interaccion_valida(sala_t *sala, const char *verbo, const char *objeto1,
				const char *objeto2)
{
	if (!sala || !verbo || !objeto1 || !objeto2)
		return false;

	bool valido = false;

	struct interaccion *actual;

	for (int i = 0; i < lista_tamanio(sala->interacciones); i++){
		actual = lista_elemento_en_posicion(sala->interacciones, (size_t)i);
		if ((strcmp(objeto1, actual->objeto) == SON_IGUALES) && (strcmp(objeto2, actual->objeto_parametro) == SON_IGUALES) && 
		(strcmp(verbo, actual->verbo) == SON_IGUALES)){
			valido = true;
		}
	}

	return valido;
}

bool sala_escape_exitoso(sala_t *sala)
{
	if (!sala)
		return false;

	return sala->escapo;
}


void sala_destruir(sala_t *sala)
{
	hash_destruir(sala->objetos);
	lista_destruir_todo(sala->interacciones, free);
	lista_destruir_todo(sala->todos_los_objetos, free);
	hash_destruir(sala->objetos_conocidos);
	hash_destruir(sala->objetos_poseidos);
	free(sala);
}