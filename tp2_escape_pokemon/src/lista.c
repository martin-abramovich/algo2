#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if(!lista)
		return NULL;

	struct nodo *nodo = malloc(sizeof(struct nodo));
	
	if(!nodo)
		return NULL;

	nodo->elemento = elemento;
	nodo->siguiente = NULL;
	
	if (lista->cantidad == 0) {
		lista->nodo_inicio = nodo;
	} else {
		lista->nodo_fin->siguiente = nodo;
	}

	lista->nodo_fin = nodo;
	lista->cantidad++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista)
		return NULL;

	if (lista->cantidad <= posicion)
		return lista_insertar(lista, elemento);

	struct nodo *nodo_a_insertar = malloc(sizeof(struct nodo));

	if (!nodo_a_insertar)
		return NULL;

	if (posicion == 0) {
		nodo_a_insertar->elemento = elemento;
		nodo_a_insertar->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nodo_a_insertar;
		lista->cantidad++;
		return lista;
	}

	struct nodo *anterior = lista->nodo_inicio;
	for (size_t i = 0; i < posicion-1; i++) {
		anterior = anterior->siguiente;
	}

	struct nodo *proximo = anterior->siguiente;
	
	nodo_a_insertar->elemento = elemento;
	nodo_a_insertar->siguiente = proximo;
	anterior->siguiente = nodo_a_insertar;
	lista->cantidad++;

	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return NULL;

	struct nodo *nuevo_tope = lista->nodo_inicio;
	void *elemento = lista->nodo_fin->elemento;

	if (lista->cantidad == 1) {
		free(lista->nodo_fin);
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
		lista->cantidad--;
		return elemento;
	}

	for (int i = 0; i < lista->cantidad-2; i++) {
		nuevo_tope = nuevo_tope->siguiente;
	}

	free(lista->nodo_fin);
	nuevo_tope->siguiente = NULL;
	lista->nodo_fin = nuevo_tope;
	lista->cantidad--;
	
	return elemento;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista)
		return NULL;

	if (lista->cantidad <= posicion || posicion+1 == lista->cantidad)
		return lista_quitar(lista);

	void *elemento;
	struct nodo *nodo_a_quitar;

	if (lista->cantidad == 1) {
		elemento = lista->nodo_fin->elemento;
		free(lista->nodo_fin);
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
		lista->cantidad--;
		return elemento;
	}

	if (posicion == 0) {
		nodo_a_quitar = lista->nodo_inicio;
		elemento = nodo_a_quitar->elemento;
		lista->nodo_inicio = nodo_a_quitar->siguiente;
		free(nodo_a_quitar);
		lista->cantidad--;
		return elemento;
	}

	struct nodo *anterior = lista->nodo_inicio;
	for (size_t i = 0; i < posicion-1; i++) {
		anterior = anterior->siguiente;
	}

	nodo_a_quitar = anterior->siguiente;
	elemento = nodo_a_quitar->elemento;
	anterior->siguiente = nodo_a_quitar->siguiente;
	free(nodo_a_quitar);
	lista->cantidad--;

	return elemento;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista)
		return NULL;

	if (posicion >= lista->cantidad)
		return NULL;

	struct nodo *aux = lista->nodo_inicio;
	for (int i = 0; i < posicion; i++) {
		aux = aux->siguiente;
	}

	return aux->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || !comparador || (lista->cantidad == 0))
		return NULL;

	struct nodo *aux = lista->nodo_inicio;
	for (int i = 0; i < (lista->cantidad); i++) {
		if (comparador(aux->elemento, contexto) == 0)
			return aux->elemento;
		aux = aux->siguiente;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return NULL;

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if(!lista || lista->cantidad == 0)
		return NULL;

	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	return lista == NULL || lista->cantidad == 0;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista)
		return 0;

	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;

	while (lista->nodo_inicio) {
		struct nodo *aux1 = lista->nodo_inicio->siguiente;
		free(lista->nodo_inicio);
		lista->nodo_inicio = aux1;
		lista->cantidad--;
	}
    
	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista || !funcion) {
		lista_destruir(lista);
		return;
	}

	while (lista->nodo_inicio) {
		struct nodo *aux1 = lista->nodo_inicio->siguiente;
		funcion(lista->nodo_inicio->elemento);
		free(lista->nodo_inicio);
		lista->nodo_inicio = aux1;
	}
    
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;
	
	struct lista_iterador *iterador = malloc(sizeof(struct lista_iterador));
	if (!iterador)
		return NULL;

	iterador->corriente = lista->nodo_inicio;
	iterador->lista = lista;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || !(iterador->corriente))
		return false;

	return (iterador->corriente != NULL);
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || !(iterador->corriente))
		return NULL;

	iterador->corriente = iterador->corriente->siguiente;
	return iterador->corriente;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !(iterador->corriente) || lista_vacia(iterador->lista))
		return NULL;

	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion)
		return 0;

	struct nodo *aux = lista->nodo_inicio;

	for (size_t i = 1; i < (lista->cantidad)+1; i++) {
		if (!funcion(aux->elemento, contexto))
			return i;
		aux = aux->siguiente;
	}

	return lista->cantidad;
}