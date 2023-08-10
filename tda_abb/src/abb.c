#include "abb.h"
#include <stddef.h>
#include <stdlib.h>

abb_t *abb_crear(abb_comparador comparador)
{
	if(!comparador)
		return NULL;

	abb_t *arbol = calloc(1, sizeof(abb_t));
	if (!arbol)
		return NULL;

	arbol->comparador = comparador;

	return arbol;
}

nodo_abb_t *abb_insertar_recursivo(nodo_abb_t *nodo_raiz, void *elemento, int (*comparador)(void *, void *))
{
	if (nodo_raiz == NULL) {
		nodo_abb_t *nodo_a_insertar = calloc(1, sizeof(struct nodo_abb));
		if (!nodo_a_insertar)
			return NULL;
		nodo_a_insertar->elemento = elemento;
		return nodo_a_insertar;
	}

	int comparacion = comparador(elemento, nodo_raiz->elemento);

	if (comparacion <= 0)
		nodo_raiz->izquierda = abb_insertar_recursivo(nodo_raiz->izquierda, elemento, comparador);
	else
		nodo_raiz->derecha = abb_insertar_recursivo(nodo_raiz->derecha, elemento, comparador);

	return nodo_raiz;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	arbol->nodo_raiz = abb_insertar_recursivo(arbol->nodo_raiz, elemento, arbol->comparador);
	(arbol->tamanio)++;

	return arbol;
}

nodo_abb_t *extraer_mas_derecho(nodo_abb_t *nodo_raiz, nodo_abb_t **extraido, size_t *tamanio)
{
	if(!nodo_raiz->derecha) {
		*extraido = nodo_raiz;
		(*tamanio)--;
		return nodo_raiz->izquierda;
	}

	nodo_raiz->derecha = extraer_mas_derecho(nodo_raiz->derecha, extraido, tamanio);
	return nodo_raiz;
}

nodo_abb_t *abb_quitar_recursivo(nodo_abb_t *nodo_raiz, void *elemento, int (*comparador)(void *, void *), size_t *tamanio, void **elemento_extraido)
{
	if (!nodo_raiz)
		return NULL;

	int comparacion = comparador(elemento, nodo_raiz->elemento);

	if (comparacion == 0) {

		*elemento_extraido = nodo_raiz->elemento;
		nodo_abb_t *derecha = nodo_raiz->derecha;
		nodo_abb_t *izquierda = nodo_raiz->izquierda;
		
		if (izquierda && derecha) {
			nodo_abb_t *extraido = NULL;
			nodo_raiz->izquierda = extraer_mas_derecho(nodo_raiz->izquierda, &extraido, tamanio);
			nodo_raiz->elemento = extraido->elemento;
			free(extraido);
			return nodo_raiz;
		} 

		if (derecha) {
			free(nodo_raiz);
			(*tamanio)--;
			return derecha;
		} else {
			free(nodo_raiz);
			(*tamanio)--;	
			return izquierda;
		}
	}

	if (comparacion < 0)
		nodo_raiz->izquierda =  abb_quitar_recursivo(nodo_raiz->izquierda, elemento, comparador, tamanio, elemento_extraido);
	else nodo_raiz->derecha = abb_quitar_recursivo(nodo_raiz->derecha, elemento, comparador, tamanio, elemento_extraido);

	return nodo_raiz;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol))
		return NULL;

	void *extraido = NULL;

	arbol->nodo_raiz = abb_quitar_recursivo(arbol->nodo_raiz, elemento, arbol->comparador, &(arbol->tamanio), &extraido);

	return extraido;
}

void *abb_buscar_recursivo(nodo_abb_t *nodo_raiz, void *elemento, int (*comparador)(void *, void *))
{
	if (!nodo_raiz)
		return NULL;

	int comparacion = comparador(elemento, nodo_raiz->elemento);

	if (comparacion == 0)
		return nodo_raiz->elemento;

	if (comparacion < 0)
		return abb_buscar_recursivo(nodo_raiz->izquierda, elemento, comparador);

	return abb_buscar_recursivo(nodo_raiz->derecha, elemento, comparador);
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;
	return abb_buscar_recursivo(arbol->nodo_raiz, elemento, arbol->comparador);	
}

bool abb_vacio(abb_t *arbol)
{
	return (arbol == NULL || abb_tamanio(arbol) == 0);
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return 0;
	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	abb_destruir_todo(arbol, NULL);
}

void abb_destruir_todo_recursivo(nodo_abb_t *nodo_raiz, void (*destructor)(void *))
{
	if (!nodo_raiz)
		return;
	abb_destruir_todo_recursivo(nodo_raiz->izquierda, destructor);
	abb_destruir_todo_recursivo(nodo_raiz->derecha, destructor);
	if (destructor)
		destructor(nodo_raiz->elemento);
	free(nodo_raiz);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if(!arbol)
		return;
	abb_destruir_todo_recursivo(arbol->nodo_raiz, destructor);
	free(arbol);
}

void abb_recorrer_elementos_inorden(nodo_abb_t *nodo_raiz, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *continuar)
{
	if(!nodo_raiz || !*continuar)
		return;

	abb_recorrer_elementos_inorden(nodo_raiz->izquierda, funcion, aux, contador, continuar);
	
	if(!*continuar)
		return;
	(*contador)++;
	*continuar = funcion(nodo_raiz->elemento, aux);
	
	abb_recorrer_elementos_inorden(nodo_raiz->derecha, funcion, aux, contador, continuar);
}

void abb_recorrer_elementos_preorden(nodo_abb_t *nodo_raiz, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *continuar)
{
	if(!nodo_raiz || !*continuar)
		return;

	if(!*continuar)
		return;
	(*contador)++;
	*continuar = funcion(nodo_raiz->elemento, aux);
	abb_recorrer_elementos_preorden(nodo_raiz->izquierda, funcion, aux, contador, continuar);
	abb_recorrer_elementos_preorden(nodo_raiz->derecha, funcion, aux, contador, continuar);
}

void abb_recorrer_elementos_postorden(nodo_abb_t *nodo_raiz, bool (*funcion)(void *, void *), void *aux, size_t *contador, bool *continuar)
{
	if(!nodo_raiz || !*continuar)
		return;

	abb_recorrer_elementos_postorden(nodo_raiz->izquierda, funcion, aux, contador, continuar);
	abb_recorrer_elementos_postorden(nodo_raiz->derecha, funcion, aux, contador, continuar);
	if(!*continuar)
		return;
	(*contador)++;
	*continuar = funcion(nodo_raiz->elemento, aux);
} 

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!funcion || !arbol)
		return 0;

	size_t contador = 0;
	bool continuar = true;

	switch (recorrido) {
		case INORDEN:
			abb_recorrer_elementos_inorden(arbol->nodo_raiz, funcion, aux, &contador, &continuar);
			break;
		case PREORDEN:
			abb_recorrer_elementos_preorden(arbol->nodo_raiz, funcion, aux, &contador, &continuar);
			break;
		case POSTORDEN:
			abb_recorrer_elementos_postorden(arbol->nodo_raiz, funcion, aux, &contador, &continuar);
			break;
		default:
			break;
	}

	return contador;
}

void insertar_elemento(void **array, size_t *elementos_almacenados, void *elemento, size_t tamanio_array)
{
	if ((*elementos_almacenados) < tamanio_array) {
		array[*elementos_almacenados] = elemento;
		(*elementos_almacenados)++;
	}	
}

void abb_almacenar_elementos_inorden(nodo_abb_t *nodo_raiz, void **array, size_t tamanio_array,
			size_t *elementos_almacenados)
{
	if(!nodo_raiz || (*elementos_almacenados) == tamanio_array)
		return;

	abb_almacenar_elementos_inorden(nodo_raiz->izquierda, array, tamanio_array, elementos_almacenados);
	insertar_elemento(array, elementos_almacenados, nodo_raiz->elemento, tamanio_array);
	abb_almacenar_elementos_inorden(nodo_raiz->derecha, array, tamanio_array, elementos_almacenados);
}

void abb_almacenar_elementos_preorden(nodo_abb_t *nodo_raiz, void **array, size_t tamanio_array,
			size_t *elementos_almacenados)
{
	if(!nodo_raiz || (*elementos_almacenados) == tamanio_array)
		return;

	insertar_elemento(array, elementos_almacenados, nodo_raiz->elemento, tamanio_array);
	abb_almacenar_elementos_preorden(nodo_raiz->izquierda, array, tamanio_array, elementos_almacenados);
	abb_almacenar_elementos_preorden(nodo_raiz->derecha, array, tamanio_array, elementos_almacenados);
}

void abb_almacenar_elementos_postorden(nodo_abb_t *nodo_raiz, void **array, size_t tamanio_array,
			size_t *elementos_almacenados)
{
	if(!nodo_raiz || (*elementos_almacenados) == tamanio_array)
		return;

	abb_almacenar_elementos_postorden(nodo_raiz->izquierda, array, tamanio_array, elementos_almacenados);
	abb_almacenar_elementos_postorden(nodo_raiz->derecha, array, tamanio_array, elementos_almacenados);
	insertar_elemento(array, elementos_almacenados, nodo_raiz->elemento, tamanio_array);
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol)
		return 0;

	size_t elementos_almacenados = 0;

	switch (recorrido) {
		case INORDEN:
			abb_almacenar_elementos_inorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);
			break;
		case PREORDEN:
			abb_almacenar_elementos_preorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);
			break;
		case POSTORDEN:
			abb_almacenar_elementos_postorden(arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados);
			break;
		default:
			break;
	}

	return elementos_almacenados;
}