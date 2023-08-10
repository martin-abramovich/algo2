#include "src/hash.h"
#include "src/lista.h"
#include "pa2m.h"
#include <stdio.h>
#include <stdlib.h>

struct hash {
	size_t capacidad;
	size_t cantidad;
	lista_t **tabla;
};

void crear_un_hash_me_devuelve_un_hash_con_la_capacidad_recibida_cantidad_0_y_una_tabla_de_hash()
{
	hash_t *hash = hash_crear(13);
	pa2m_afirmar(hash_cantidad(hash) == 0, "Se crea un hash, la cantidad de elementos es 0");
	pa2m_afirmar((hash->capacidad) == 13, "Se crea un hash con la capacidad recibida (mayor a 3)");
	hash_destruir(hash);
}

void al_recibir_una_capacidad_menor_a_la_minima_se_crea_un_hash_de_capacidad_3()
{
	hash_t *hash = hash_crear(1);
	pa2m_afirmar((hash->capacidad) == 3, "La capacidad recibida es menor a 3, se crea un hash de capacidad 3");
	hash_destruir(hash);
}

void insertar_un_elemento_devuelve_el_hash_con_el_elemento_insertado()
{
	pa2m_afirmar(hash_insertar(NULL, "AAAAA", "Juan", NULL) == NULL, "No puedo insertar en un hash nulo");
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_insertar(hash, NULL, "Juan", NULL) == NULL, "No puedo insertar con una clave nula");
	hash_insertar(hash, "AAAAA", "Juan", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 1, "Al insertar un elemento la cantidad aumenta");
	pa2m_afirmar(hash_obtener(hash, "AAAAA") == (void *)"Juan", "Al buscar la clave encuentro el elemento insertado");
	hash_insertar(hash, "BBBBB", "Juan", NULL);
	pa2m_afirmar(hash_insertar(hash, "CCCCC", NULL, NULL) != NULL, "Puedo insertar un elemento nulo");
	pa2m_afirmar(hash_cantidad(hash) == 3, "La cantidad de elementos es 3");
	hash_destruir(hash);
}

void al_insertar_una_clave_repetida_se_sobreescribe_el_valor()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "AAAAA", "Juan", NULL);
	hash_insertar(hash, "AAAAA", "Flor", NULL);
	pa2m_afirmar(hash_obtener(hash, "AAAAA") == (void *)"Flor", "Al insertar un elemento con clave repetida se sobreescribe el valor");
	pa2m_afirmar(hash_cantidad(hash) == 1, "Al insertar un elemento con clave repetida la cantidad no aumenta");
	hash_destruir(hash);
}

void si_anterior_no_es_nulo_se_guarda_un_puntero_al_elemento_reemplazado_en_caso_de_clave_repetida()
{
	hash_t *hash = hash_crear(3);
	void *anterior = (void *)"Tomas";
	hash_insertar(hash, "AAAAA", "Juan", NULL);
	hash_insertar(hash, "AAAAA", "Hector", &anterior);
	pa2m_afirmar(anterior == (void *)"Juan", "Anterior no es nulo y la clave es repetida, se almacena en anterior un puntero al elemento reemplazado");
	hash_destruir(hash);
}


void si_anterior_no_es_nulo_y_la_clave_no_existia_se_almacena_NULL_en_anterior()
{
	hash_t *hash = hash_crear(3);
	void *anterior = (void *)"Tomas";
	hash_insertar(hash, "AAAAA", "Juan", &anterior);
	pa2m_afirmar(anterior == NULL, "Anterior no es nulo y la clave insertada no existia, se almacena NULL en anterior");
	hash_destruir(hash);
}

void quitar_un_elemento_disminuye_la_cantidad_del_hash_y_devuelve_el_elemento_quitado()
{
	pa2m_afirmar(hash_quitar(NULL, "AAAAA") == NULL, "Quitar un elemento de un hash null devuelve NULL");
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "AAAAA", "Juan", NULL);
	hash_insertar(hash, "BBBBB", "Flor", NULL);
	pa2m_afirmar(hash_quitar(hash, "AAAAA") == (void *)"Juan", "Quitar un elemento devuelve el elemento quitado");
	pa2m_afirmar(hash_cantidad(hash) == 1, "Al quitar un elemento la cantidad disminuye");
	hash_destruir(hash);
}

void quitar_un_elemento_que_no_esta_en_el_hash_devuelve_null()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "AAAAA", "Juan", NULL);
	hash_insertar(hash, "BBBBB", "Flor", NULL);
	pa2m_afirmar(hash_quitar(hash, "CCCCC") == NULL, "Al no encontrar el elemento a quitar devuelve NULL");
	hash_destruir(hash);	
}

void hash_obtener_devuelve_el_elemento_con_la_clave_dada()
{
	pa2m_afirmar(hash_obtener(NULL, "CCCCC") == NULL, "Si busco un elemento en un hash nulo devuelve NULL");
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_obtener(hash, NULL) == NULL, "No puedo obtener un elemento con una clave nula");
	hash_insertar(hash, "AAAAA", "Juan", NULL);
	hash_insertar(hash, "BBBBB", "Flor", NULL);
	hash_insertar(hash, "CCCCC", "Matias", NULL);
	hash_insertar(hash, "DDDDD", "Raul", NULL);
	hash_insertar(hash, "EEEEE", "Dante", NULL);
	hash_insertar(hash, "AAAAA", "Diego", NULL);
	pa2m_afirmar(hash_obtener(hash, "CCCCC") == (void *)"Matias", "Obtengo el elemento a partir de su clave");
	pa2m_afirmar(hash_obtener(hash, "AAAAA") != (void *)"Juan", "Si se sobreescribe un elemento y lo busco no lo encuentro");
	pa2m_afirmar(hash_obtener(hash, "AAAAA") == (void *)"Diego", "Busco elemento con clave repetida, encuentro el último elemento insertado con esa clave");
	hash_destruir(hash);
}

void hash_contiene_devuelve_true_si_el_hash_contiene_el_elemento_con_la_clave_dada_de_lo_contrario_devuelve_false()
{
	pa2m_afirmar(!hash_contiene(NULL, "CCCCC"), "Hash nulo no contiene elementos, devuelve false");
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "EEEEE", "Dante", NULL);
	pa2m_afirmar(hash_contiene(hash, "EEEEE"), "Existe un elemento con la clave dada, devuelve true");
	pa2m_afirmar(!hash_contiene(hash, "QQQQQ"), "No existe un elemento con la clave dada, devuelve false");
	hash_destruir(hash);
}

void hash_cantidad_devuelve_la_cantidad_de_elementos_del_hash()
{
	pa2m_afirmar(hash_cantidad(NULL) == 0, "Un hash nulo tiene 0 elementos");
	hash_t *hash = hash_crear(3);
	pa2m_afirmar(hash_cantidad(hash) == 0, "La cantidad de elementos de un hash al que no se le insertaron elementos es 0");
	hash_insertar(hash, "EEEEE", "Dante", NULL);
	hash_insertar(hash, "CCCCC", "Matias", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 2, "Inserto 2 elementos, la cantidad ahora es 2");
	hash_quitar(hash, "EEEEE");
	pa2m_afirmar(hash_cantidad(hash) == 1, "Quito un elemento, la cantidad ahora es 1");
	hash_insertar(hash, "CCCCC", "Bianca", NULL);
	pa2m_afirmar(hash_cantidad(hash) == 1, "Si sobreescribo un elemento por tener claves repetidas la cantidad no se modifica");
	hash_destruir(hash);
}

bool mostrar_clave(const char *clave, void *valor, void *aux)
{
	if (!clave)
		return false;

	aux = aux;

	return true;
}

void iterador_interno_devuelve_cantidad_de_claves_iteradas_o_cero_en_caso_de_error()
{
	pa2m_afirmar(hash_con_cada_clave(NULL, mostrar_clave, NULL) == 0, "Si el hash es nulo devuelve 0 iteraciones");
	hash_t *hash = hash_crear(3);
	pa2m_afirmar(hash_con_cada_clave(hash, NULL, NULL) == 0, "Si la funcion recibida es nula devuelve 0 iteraciones");
	hash_insertar(hash, "AAAAA", "Juan", NULL);
	hash_insertar(hash, "BBBBB", "Flor", NULL);
	hash_insertar(hash, "CCCCC", "Matias", NULL);
	hash_insertar(hash, "DDDDD", "Raul", NULL);
	hash_insertar(hash, "EEEEE", "Dante", NULL);
	pa2m_afirmar(hash_con_cada_clave(hash, mostrar_clave, NULL) == 5, "Se recorren las 5 claves insertadas");
	hash_quitar(hash, "AAAAA");
	pa2m_afirmar(hash_cantidad(hash) == 4, "La cantidad de elementos es 4");
	pa2m_afirmar(hash_con_cada_clave(hash, mostrar_clave, NULL) == 4, "Se recorren los 4 elementos");
	hash_destruir(hash);

}

int main()
{
	pa2m_nuevo_grupo("Crear Hash");
	crear_un_hash_me_devuelve_un_hash_con_la_capacidad_recibida_cantidad_0_y_una_tabla_de_hash();
	al_recibir_una_capacidad_menor_a_la_minima_se_crea_un_hash_de_capacidad_3();

	pa2m_nuevo_grupo("Pruebas de inserción");
	insertar_un_elemento_devuelve_el_hash_con_el_elemento_insertado();
	al_insertar_una_clave_repetida_se_sobreescribe_el_valor();
	si_anterior_no_es_nulo_se_guarda_un_puntero_al_elemento_reemplazado_en_caso_de_clave_repetida();
	si_anterior_no_es_nulo_y_la_clave_no_existia_se_almacena_NULL_en_anterior();

	pa2m_nuevo_grupo("Pruebas de eliminacion");
	quitar_un_elemento_disminuye_la_cantidad_del_hash_y_devuelve_el_elemento_quitado();
	quitar_un_elemento_que_no_esta_en_el_hash_devuelve_null();

	pa2m_nuevo_grupo("Hash obtener");
	hash_obtener_devuelve_el_elemento_con_la_clave_dada();

	pa2m_nuevo_grupo("Hash contiene");
	hash_contiene_devuelve_true_si_el_hash_contiene_el_elemento_con_la_clave_dada_de_lo_contrario_devuelve_false();

	pa2m_nuevo_grupo("Hash contiene");
	hash_contiene_devuelve_true_si_el_hash_contiene_el_elemento_con_la_clave_dada_de_lo_contrario_devuelve_false();

	pa2m_nuevo_grupo("Hash cantidad");
	hash_cantidad_devuelve_la_cantidad_de_elementos_del_hash();

	pa2m_nuevo_grupo("Hash con cada clave");
	iterador_interno_devuelve_cantidad_de_claves_iteradas_o_cero_en_caso_de_error();

	return pa2m_mostrar_reporte();
}
