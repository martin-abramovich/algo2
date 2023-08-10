#include "hash.h"
#include "lista.h"
#include "string.h"
#include <stdlib.h>
#include <stddef.h>

#define CAPACIDAD_MINIMA 3
#define IGUALES 0
#define DISTINTOS 1

typedef struct {
	char *clave;
	void *valor;
} par_t;

struct hash {
	size_t capacidad;
	size_t cantidad;
	lista_t **tabla;
};

char *duplicar_clave(const char *clave)
{
	if (!clave)
		return NULL;

	char *copia = malloc(strlen(clave)+1);
	
	if(!copia)
		return NULL;

	strcpy(copia, clave);
	return copia;
}

void destruir_tabla_hash(lista_t **tabla, size_t capacidad)
{
	if (!tabla || !capacidad)
		return;

	for (int i = 0; i < capacidad; i++)
		lista_destruir(tabla[i]);

	free(tabla);
}

lista_t **crear_tabla_hash(size_t capacidad)
{
	lista_t **tabla = calloc(capacidad, sizeof(lista_t *));

	if (!tabla)
		return NULL;

	for (int i = 0; i < capacidad; i++){
		tabla[i] = lista_crear();
		if (!tabla[i]){
			destruir_tabla_hash(tabla, capacidad);
			return NULL;
		}
	}

	return tabla;
}

hash_t *hash_crear(size_t capacidad)
{
	if (!capacidad)
		return NULL;

	hash_t *hash = malloc(sizeof(hash_t));
	
	if (!hash)
		return NULL;

	hash->cantidad = 0;

	if (capacidad < CAPACIDAD_MINIMA)
		hash->capacidad = CAPACIDAD_MINIMA;
	else
		hash->capacidad = capacidad;
	
	lista_t **tabla = crear_tabla_hash(hash->capacidad);

	if (!tabla){
		free(hash);
		return NULL;
	}

	hash->tabla = tabla;
	return hash;
}

size_t funcion_hash(const char *clave)
{
	unsigned char *str = (unsigned char *)clave;
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + (unsigned long)c;

	return hash;
}

par_t *crear_par(const char *clave, void *valor)
{
	par_t *par = malloc(sizeof(par_t));
	
	if (!par)
		return NULL;

	par->clave = duplicar_clave(clave);
	par->valor = valor;

	return par; 
}

hash_t *rehash(hash_t *hash)
{
	
	size_t capacidad_nueva = hash->capacidad * 2;

	lista_t **tabla_rehash = crear_tabla_hash(capacidad_nueva);

	if (!tabla_rehash)
		return NULL;

	size_t i = 0;

	while (i < hash->capacidad){
		while (hash->tabla[i] && !lista_vacia(hash->tabla[i])){
			par_t *par_a_insertar = (par_t *)lista_primero(hash->tabla[i]);
			lista_quitar_de_posicion(hash->tabla[i], 0);
			size_t posicion = funcion_hash(par_a_insertar->clave) % capacidad_nueva;
			lista_insertar(tabla_rehash[posicion], par_a_insertar);
		}
		lista_destruir(hash->tabla[i]);
		i++;
	}
	
	hash->capacidad = capacidad_nueva;

	free(hash->tabla);
	hash->tabla = tabla_rehash;

	return hash;
}

int elemento_es_igual_a(void *par, void *clave)
{
  	par_t* par_a_comparar = (par_t *)par;

	if (strcmp(par_a_comparar->clave, clave) == 0)
		return IGUALES;

	return DISTINTOS;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;

	if(!hash->tabla[posicion])
		return NULL;
	
	par_t *par = (par_t *)lista_buscar_elemento(hash->tabla[posicion], elemento_es_igual_a, (void *)clave);

	if (par){
		if (anterior != NULL)
			*anterior = par->valor;
		par->valor = elemento;
		return hash;
	}

	par = crear_par(clave, elemento);
	lista_insertar(hash->tabla[posicion], par);
	hash->cantidad++;
	if (anterior != NULL)
		*anterior = NULL;	

	float factor_de_carga = (float)(hash->cantidad / hash->capacidad);
	if (factor_de_carga >= 0.75)
		rehash(hash);

	return hash;	
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	par_t *par;
	size_t pos_clave = 0;
	bool encontrado = false;

	size_t posicion = funcion_hash(clave) % hash->capacidad;

	lista_iterador_t *iterador = lista_iterador_crear(hash->tabla[posicion]);
	
	if (!iterador)
		return NULL;

	while (lista_iterador_tiene_siguiente(iterador) && encontrado == false){
		par = lista_iterador_elemento_actual(iterador);
		if (elemento_es_igual_a(par, (void *)clave) == 0)
			encontrado = true;
		else
			pos_clave++;
		lista_iterador_avanzar(iterador);
	}

	lista_iterador_destruir(iterador);

	if (encontrado == false)
		return NULL;

	lista_quitar_de_posicion(hash->tabla[posicion], pos_clave);
	void *valor_quitado = par->valor;

	free((void *)par->clave);
	free(par); 
	hash->cantidad--;

	return valor_quitado;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;

	par_t *par = (par_t *)lista_buscar_elemento(hash->tabla[posicion], elemento_es_igual_a, (void *)clave);

	if (!par)
		return NULL;
	
	return par->valor;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	return (hash_obtener(hash, clave));
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;
	
	for (int i = 0; i < hash->capacidad; i++){
		while(hash->tabla[i] && !lista_vacia(hash->tabla[i])){
			par_t *elemento_a_liberar = (par_t *)lista_quitar(hash->tabla[i]);
			if(destructor)
				destructor(elemento_a_liberar->valor);
			free((void *)elemento_a_liberar->clave);
			free(elemento_a_liberar);
		}
		lista_destruir(hash->tabla[i]);
	}

	free(hash->tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t iteraciones = 0;

	if (!hash || !f)
		return iteraciones;

	for (int i = 0; i < hash->capacidad; i++)
	{
		lista_iterador_t *iterador = lista_iterador_crear(hash->tabla[i]);
		
		while (lista_iterador_tiene_siguiente(iterador)){
			par_t *par = (par_t *)lista_iterador_elemento_actual(iterador);
			iteraciones++;
			if (!f(par->clave, par->valor, aux)){
				lista_iterador_destruir(iterador);
				return iteraciones;
			}
			lista_iterador_avanzar(iterador);
		}	
		lista_iterador_destruir(iterador);
	}

	return iteraciones;
}