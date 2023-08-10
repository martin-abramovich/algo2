#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include "pa2m.h"

void se_crea_una_lista_y_al_hacerlo_esta_vacia()
{
  lista_t *lista = lista_crear();
  pa2m_afirmar(lista_vacia(lista), "La lista está vacia");
  lista_destruir(lista);
}

void crear_una_lista_me_devuelve_una_lista_de_cantidad_0_y_nodos_nulos()
{
  lista_t *lista = lista_crear();
  pa2m_afirmar(lista != NULL, "La lista creada es válida");
  pa2m_afirmar((lista->cantidad) == 0, "La lista creada tiene 0 elementos");
  pa2m_afirmar(((lista->nodo_inicio) == NULL), "El nodo inicial es nulo");
  pa2m_afirmar(((lista->nodo_fin) == NULL), "El nodo final es nulo");
  lista_destruir(lista);
}

void si_la_lista_es_nula_no_puedo_insertar()
{
  lista_t *lista = NULL;
  int elemento = 1;
  pa2m_afirmar(!(lista_insertar(lista, &elemento)), "No puedo insertar en una lista NULL");
}

void al_insertar_un_elemento_nodo_fin_apunta_a_dicho_elemento()
{
  lista_t *lista = lista_crear();
  int elemento1 = 1;
  char elemento2 = 'M';
  int elemento3 = 4;
  lista_insertar(lista, &elemento1);
  pa2m_afirmar(lista->nodo_fin->elemento == (&elemento1), "Inserto un 1 al final de la lista");
  lista_insertar(lista, &elemento2);
  pa2m_afirmar(lista->nodo_fin->elemento == (&elemento2), "Inserto una M al final de la lista");
  lista_insertar(lista, &elemento3);
  pa2m_afirmar(lista->nodo_fin->elemento == (&elemento3), "Inserto un 4 al final de la lista");
  pa2m_afirmar(lista->cantidad == 3,"Luego de insertar 3 elementos en una lista vacía, la cantidad de elementos es 3");
  lista_destruir(lista);
}

void al_insertar_elemento_en_lista_vacia_nodo_inicio_y_nodo_fin_apuntan_a_dicho_elemento()
{
  lista_t *lista = lista_crear();
  int elemento1 = 1;
  lista_insertar(lista, &elemento1);
  pa2m_afirmar(lista->nodo_inicio->elemento == (&elemento1), "Inserto en lista vacía, nodo inicio apunta al elemento insertado");
  pa2m_afirmar(lista->nodo_fin->elemento == (&elemento1), "Inserto en lista vacía, nodo fin apunta al elemento insertado");
  lista_destruir(lista);
}

void si_la_posicion_no_existe_inserta_el_elemento_al_final()
{
  lista_t *lista = lista_crear();
  int elemento = 1;
  lista_insertar_en_posicion(lista, &elemento, 2);
  pa2m_afirmar(lista->nodo_fin->elemento == (&elemento), "La posicion no existe, el elemento se inserta al final");
  lista_destruir(lista);
}

void puedo_insertar_un_elemento_nulo_y_la_cantidad_de_elementos_se_incrementa()
{
  lista_t *lista = lista_crear();
  int elemento = 4;
  lista_insertar_en_posicion(lista, NULL, 0);
  pa2m_afirmar(lista->cantidad == 1, "Puedo agregar un elemento nulo");
  pa2m_afirmar(lista->cantidad == 1, "Puedo agregar elementos en la primera posición correctamente");
  lista_insertar(lista, NULL);
  lista_insertar(lista, NULL);
  lista_insertar(lista, &elemento);
  pa2m_afirmar(lista_ultimo(lista) == &elemento, "Puedo agregar elementos en la última posición correctamente");
  lista_destruir(lista);
}

void al_insertar_en_la_posicion_0_nodo_inicio_apunta_al_nodo_insertado()
{
  lista_t *lista = lista_crear();
  char d = 'd';
  lista_insertar_en_posicion(lista, &d, 0);
  pa2m_afirmar(lista->nodo_inicio->elemento == &d, "Al insertar en la posición 0, nodo inicio apunta a dicho elemento");
  lista_destruir(lista);
}

void si_la_posicion_no_existe_devuelve_null()
{
  lista_t *lista = lista_crear();
  pa2m_afirmar(!lista_elemento_en_posicion(lista, 3), "La posición no existe, devuelve NULL");
  lista_destruir(lista);
}

void al_buscar_una_posicion_existente_devuelve_el_elemento_de_dicha_posicion()
{
  lista_t *lista = lista_crear();
  char a = 'a', b = 'b', c = 'c', d = 'd';
  lista_insertar(lista, &a);
  lista_insertar(lista, &c);
  lista_insertar_en_posicion(lista, &d, 100);
  lista_insertar_en_posicion(lista, &b, 120);
  lista_insertar_en_posicion(lista, &b, 1);
  lista_insertar_en_posicion(lista, &b, 4);
  pa2m_afirmar(lista_elemento_en_posicion(lista, 2) == &c, "En la posicion 2 hay una 'c'");
  pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == &a, "En la posicion 0 hay una 'a'");
  pa2m_afirmar(lista_elemento_en_posicion(lista, 8) == NULL, "En la posicion 8 devuelve NULL");
  lista_destruir(lista);
}

int elemento_es_igual_a(void *_a, void *_b)
{
  char *a = _a;
  char *b = _b;

  if(a && b && *a == *b)
    return 0;

  return -1;
}

void si_el_elemento_no_existe_devuelve_null_de_lo_contrario_devuelve_el_elemento_buscado()
{
  lista_t *lista = lista_crear();
  char a = 'a', b = 'b';
  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  char buscado1 = 'c';
  char *elemento = lista_buscar_elemento(lista, elemento_es_igual_a, &buscado1);
  pa2m_afirmar(elemento == NULL, "El elemento buscado no está en la lista, retorna NULL");
  char buscado2 = 'a';
  elemento = lista_buscar_elemento(lista, elemento_es_igual_a, &buscado2);
  pa2m_afirmar(elemento == &a, "El elemento buscado 'a' está en la lista, devuelve 'a'");
  lista_destruir(lista);
}

void si_la_lista_es_null_no_puedo_quitar_elementos()
{
  lista_t *lista = NULL;
  pa2m_afirmar(!lista_quitar(lista), "No puedo quitarle elementos a una lista nula");
}

void si_la_lista_esta_vacia_no_puedo_quitar_elementos()
{
  lista_t *lista = lista_crear();
  pa2m_afirmar(!lista_quitar(lista), "No puedo quitarle elementos a una lista vacía");
  lista_destruir(lista);
}

void quitar_un_elemento_me_devuelve_el_elemento_y_reduce_la_cantidad_de_la_lista()
{
  lista_t *lista = lista_crear();
  char a = 'a', c = 'c';
  lista_insertar(lista, &a);
  lista_insertar(lista, &c);
  pa2m_afirmar(lista_quitar(lista) == &c, "Quitar un elemento devuelve el elemento quitado");
  pa2m_afirmar(lista_tamanio(lista) == 1, "Quitar un elemento reduce la cantidad de la lista");
  lista_destruir(lista);
}

void si_quito_el_unico_elemento_nodo_inicio_es_nulo_y_nodo_fin_tambien()
{
  lista_t *lista = lista_crear();
  char a = 'a';
  lista_insertar(lista, &a);
  lista_quitar(lista);
  pa2m_afirmar(lista->nodo_inicio == NULL, "Al quitar el único elemento de la lista, nodo inicio es NULL");
  pa2m_afirmar(lista->nodo_fin == NULL, "Al quitar el único elemento de la lista, nodo fin es NULL");
  lista_destruir(lista);
}

void si_la_posicion_no_existe_quita_el_ultimo_elemento()
{
  lista_t *lista = lista_crear();
  char a = 'a';
  lista_insertar(lista, &a);
  lista_quitar_de_posicion(lista, 2);
  pa2m_afirmar(lista->cantidad == 0, "La posicion no existe, se quita el último elemento");
  lista_destruir(lista);
}

void si_quito_el_unico_elemento_nodo_inicio_es_nulo_y_nodo_fin_tambien_version_2()
{
  lista_t *lista = lista_crear();
  char a = 'a';
  lista_insertar(lista, &a);
  lista_quitar_de_posicion(lista, 0);
  pa2m_afirmar(lista->nodo_inicio == NULL, "Al quitar el único elemento de la lista, nodo inicio es NULL");
  pa2m_afirmar(lista->nodo_fin == NULL, "Al quitar el único elemento de la lista, nodo fin es NULL");
  lista_destruir(lista);
}

void al_quitar_una_posicion_valida_la_cantidad_de_la_lista_disminuye_y_el_siguiente_del_nodo_anterior_al_quitado_apunta_al_siguiente_del_quitado()
{
  lista_t *lista = lista_crear();
  char a = 'a', b = 'b', c = 'c', d = 'd';
  lista_insertar(lista, &a);
  lista_insertar(lista, &c);
  lista_insertar(lista, &d);
  lista_insertar(lista, &b);
  lista_insertar(lista, &b);
  lista_insertar(lista, &b);
  lista_quitar_de_posicion(lista, 0);
  pa2m_afirmar(lista->nodo_inicio->elemento == &c, "Al quitar de la posición 0, nodo inicio apunta al siguiente del quitado");
  pa2m_afirmar(lista->cantidad == 5, "Al quitar un elemento la cantidad de elementos se reduce en 1");
  pa2m_afirmar(lista_quitar_de_posicion(lista, 2) == &b, "Se quita el elemento 'b' en la posición 2");
  pa2m_afirmar(lista_quitar_de_posicion(lista, 1) == &d, "Se quita el elemento 'd' en la posicion 1");
  lista_destruir(lista);
}

void al_quitar_la_ultima_posicion_nodo_fin_apunta_al_anterior_del_quitado()
{
  lista_t *lista = lista_crear();
  char a = 'a', c = 'c', d = 'd';
  lista_insertar(lista, &a);
  lista_insertar(lista, &c);
  lista_insertar(lista, &d);
  lista_quitar_de_posicion(lista, 2);
  pa2m_afirmar(lista->nodo_fin->elemento == &c, "Se quita la última posición, nodo fin apunta al anterior del quitado");
  lista_destruir(lista);
}

void si_la_lista_esta_vacia_lista_primero_devuelve_null()
{
  lista_t *lista = lista_crear();
  pa2m_afirmar(!lista_primero(lista), "La lista esta vacía, al pedir el primer elemento devuelve NULL");
  lista_destruir(lista);
}

void si_la_lista_es_nula_lista_primero_devuelve_null()
{
  lista_t *lista = NULL;
  pa2m_afirmar(!lista_primero(lista), "La lista es nula, al pedir el primer elemento devuelve NULL");
}

void lista_primero_devuelve_el_primer_elemento_de_una_lista_que_contiene_elementos()
{
  lista_t *lista = lista_crear();
  char a = 'a', b = 'b', c = 'c', d = 'd';
  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_insertar(lista, &c);
  lista_insertar(lista, &d);
  pa2m_afirmar(lista_primero(lista) == &a, "El primer elemento de la lista es 'a'");
  lista_destruir(lista);
}

void si_la_lista_esta_vacia_lista_ultimo_devuelve_null()
{
  lista_t *lista = lista_crear();
  pa2m_afirmar(!lista_ultimo(lista), "La lista esta vacía, al pedir el ultimo elemento devuelve NULL");
  lista_destruir(lista);
}

void si_la_lista_es_nula_lista_ultimo_devuelve_null()
{
  lista_t *lista = NULL;
  pa2m_afirmar(!lista_ultimo(lista), "La lista es nula, al pedir el ultimo elemento devuelve NULL");
}

void lista_ultimo_devuelve_el_ultimo_elemento_de_una_lista_que_contiene_elementos()
{
  lista_t *lista = lista_crear();
  char a = 'a', b = 'b', c = 'c', d = 'd';
  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_insertar(lista, &c);
  lista_insertar(lista, &d);
  pa2m_afirmar(lista_ultimo(lista) == &d, "El último elemento de la lista es 'd'");
  lista_destruir(lista);
}

void la_lista_esta_vacia_entonces_devuelve_true()
{
  lista_t *lista = lista_crear();
  pa2m_afirmar(lista_vacia(lista) == true, "La lista está vacía");
  lista_destruir(lista);
}
void la_lista_no_existe_entonces_devuelve_true()
{
  lista_t *lista = NULL;
  pa2m_afirmar(lista_vacia(lista) == true, "La lista no existe, devuelve que la lista está vacía");
}
  
void la_lista_existe_y_tiene_elementos_entonces_devuelve_false()
{
  lista_t *lista = lista_crear();
  char a = 'a';
  lista_insertar(lista, &a);
  pa2m_afirmar(lista_vacia(lista) == false, "La lista tiene elementos, por ende no está vacía");
  lista_destruir(lista);
}

void si_la_lista_esta_vacia_su_tamanio_es_cero()
{
  lista_t *lista = lista_crear();
  pa2m_afirmar(lista_tamanio(lista) == 0, "Una lista vacía tiene tamaño 0");
  lista_destruir(lista);
}

void el_tamanio_de_la_lista_se_corresponde_con_la_cantidad_de_elementos_de_la_misma()
{
  lista_t *lista = lista_crear();
  char a = 'a';
  lista_insertar(lista, &a);
  pa2m_afirmar(lista_tamanio(lista) == 1, "La lista tiene 1 elemento, por ende tiene tamaño 1");
  lista_destruir(lista); 
}

void destruir_todo_destruye_la_lista_y_cada_bloque_de_la_misma()
{
  lista_t *lista = lista_crear();

  int *elemento1 = malloc(sizeof(int));
  *elemento1 = 1;
  int *elemento2 = malloc(sizeof(int));
  *elemento2 = 1;
  int *elemento3 = malloc(sizeof(int));
  *elemento3 = 1;
  lista_insertar(lista, elemento1);
  lista_insertar(lista, elemento2);
  lista_insertar(lista, elemento3);
  lista_destruir_todo(lista, free);
  pa2m_afirmar(true, "No pierde memoria al finalizar de destruir todo");
}

void crear_un_iterador_correctamente_me_devuelve_el_iterador_creado()
{
  lista_t *lista = lista_crear();
  lista_iterador_t *iterador = lista_iterador_crear(lista);
  pa2m_afirmar(iterador != NULL, "El iterador se crea correctamente");
  pa2m_afirmar(iterador->corriente == lista->nodo_inicio, "Al crearse el iterador, corriente es el nodo inicio de la lista");
  pa2m_afirmar(iterador->lista == lista, "Al crearse el iterador, el puntero a lista es la lista correspondiente");
  lista_iterador_destruir(iterador);
  lista_destruir(lista);
}

void no_es_posible_crear_un_iterador_si_la_lista_no_existe_o_es_invalida()
{
  lista_t *lista = NULL;
  pa2m_afirmar(!lista_iterador_crear(lista), "No se puede crear un iterador con una lista nula");
}

void iterador_tiene_siguiente_devuelve_true_si_hay_mas_elementos_sobre_los_cuales_iterar()
{
  lista_t *lista = lista_crear();
  char a = 'a', b = 'b';
  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_iterador_t *iterador = lista_iterador_crear(lista);
  pa2m_afirmar(lista_iterador_tiene_siguiente(iterador) == true, "Hay elementos sobre los cuales iterar, devuelve true");
  lista_iterador_destruir(iterador);
  lista_destruir(lista);
}

void iterador_tiene_siguiente_devuelve_false_si_no_hay_mas_elementos_sobre_los_cuales_iterar()
{
  lista_t *lista = lista_crear();
  char a = 'a';
  lista_insertar(lista, &a);
  lista_iterador_t *iterador = lista_iterador_crear(lista);
  lista_iterador_avanzar(iterador);
  pa2m_afirmar(lista_iterador_tiene_siguiente(iterador) == false, "No hay más elementos sobre los cuales iterar, devuelve false");
  lista_iterador_destruir(iterador);
  lista_destruir(lista);
}

void si_quedan_elementos_puedo_avanzar_el_iterador()
{
  lista_t *lista = lista_crear();
  char a = 'a', b = 'b', c = 'c';
  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_insertar(lista, &c);
  lista_iterador_t *iterador = lista_iterador_crear(lista);
  pa2m_afirmar(lista_iterador_avanzar(iterador) == true, "Puedo avanzar el iterador si quedan 2 elementos");
  pa2m_afirmar(lista_iterador_avanzar(iterador) == true, "Puedo avanzar el iterador si queda 1 elemento");
  pa2m_afirmar(lista_iterador_avanzar(iterador) == false, "No puedo avanzar el iterador si no quedan elementos");
  lista_iterador_destruir(iterador);
  lista_destruir(lista);
}

void lista_iterador_elemento_actual_devuelve_el_elemento_actual_del_iterador_y_null_si_no_existe_el_elemento()
{
  lista_t *lista = lista_crear();
  char a = 'a', b = 'b';
  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_insertar(lista, NULL);
  lista_iterador_t *iterador = lista_iterador_crear(lista);
  pa2m_afirmar(lista_iterador_elemento_actual(iterador) == &a, "El elemento actual es 'a'");
  lista_iterador_avanzar(iterador);
  pa2m_afirmar(lista_iterador_elemento_actual(iterador) == &b, "El elemento actual es 'b'");
  lista_iterador_avanzar(iterador);
  pa2m_afirmar(!lista_iterador_elemento_actual(iterador), "El elemento actual es NULL, devuelve NULL");
  lista_iterador_destruir(iterador);
  lista_destruir(lista);
}

void si_la_funcion_no_existe_no_puedo_iterar()
{
  int contador = 0;
  lista_t *lista = lista_crear();
  pa2m_afirmar(lista_con_cada_elemento(lista, NULL, (void *)&contador) == 0, "No puedo iterar si la función no existe");
  lista_destruir(lista);
}

bool sigue_iterando(void *elemento, void *contador)
{
  return (elemento && contador);
}

void lista_con_cada_elemento_devuelve_la_cantidad_de_elementos_iterados()
{
  int contador = 0;
  lista_t *lista = lista_crear();
  char a = 'a';
  lista_insertar(lista, &a);
  lista_insertar(lista, NULL);
  lista_insertar(lista, NULL);
  pa2m_afirmar(lista_con_cada_elemento(lista, sigue_iterando, (void *)&contador) == 2, "Itera la cantidad de veces que se esperaba");
  lista_destruir(lista); 
}

void crear_una_pila_me_devuelve_una_pila_vacia()
{
  pila_t *pila = pila_crear();
  pa2m_afirmar(pila_vacia(pila) == true, "Crear una pila me devuelve una pila vacía");
  pila_destruir(pila);
}

void apilar_un_elemento_incrementa_el_tamanio_de_la_pila_y_el_tope_es_el_ultimo_elemento_apilado(){
  pila_t *pila = pila_crear();
  int elemento1 = 10, elemento2 = 3, elemento3 = 6;
  pila_apilar(pila, &elemento1);
  pila_apilar(pila, &elemento2);
  pila_apilar(pila, &elemento3);
  pa2m_afirmar(pila_tamanio(pila) == 3, "Al apilar 3 elementos a una pila vacía el tamaño de la  pila es 3");
  pa2m_afirmar(pila_tope(pila) == &elemento3, "El tope de la pila es el correcto");
  pila_destruir(pila);
}

void apilar_en_una_pila_nula_es_un_error()
{
  int elemento = 5;
  pa2m_afirmar(!pila_apilar(NULL, &elemento), "No puedo apilar en una pila NULL");
}

void desapilar_una_pila_me_devuelve_el_elemento_desapilado_y_el_tamanio_de_la_pila_disminuye()
{
  pila_t *pila = pila_crear();
  int elemento1 = 10, elemento2 = 3, elemento3 = 6;
  pila_apilar(pila, &elemento1);
  pila_apilar(pila, &elemento2);
  pila_apilar(pila, &elemento3);
  pa2m_afirmar(pila_desapilar(pila) == &elemento3, "El elemento desapilado es 3");
  pa2m_afirmar(pila_tamanio(pila) == 2, "El tamaño de la pila despues de desapilar es 2");
  pila_destruir(pila);
}

void desapilar_en_una_pila_nula_es_un_error()
{
  pa2m_afirmar(!pila_desapilar(NULL), "No puedo desapilar en una pila NULL");
}

void el_tope_de_la_pila_devuelve_el_ultimo_elemento_insertado()
{
  pila_t *pila = pila_crear();
  int elemento1 = 10, elemento2 = 3, elemento3 = 6;
  pila_apilar(pila, &elemento1);
  pila_apilar(pila, &elemento2);
  pila_apilar(pila, &elemento3);
  pa2m_afirmar(pila_tope(pila) == &elemento3, "El elemento que se encuentra en el tope es 3");
  pila_destruir(pila);
}

void si_el_elemento_del_tope_de_la_pila_es_nulo_devuelve_null()
{
  pila_t *pila = pila_crear();
  pila_apilar(pila, NULL);
  pa2m_afirmar(!pila_tope(pila), "El elemento del tope es nulo, devuelve NULL");
  pila_destruir(pila);  
}

void si_la_pila_no_existe_el_tamanio_es_cero_de_lo_contrario_el_tamanio_es_la_cantidad_de_elementos()
{
  pa2m_afirmar(pila_tamanio(NULL) == 0, "La pila no existe, su tamaño es 0");
  pila_t *pila = pila_crear();
  pila_apilar(pila, NULL);
  pa2m_afirmar(pila_tamanio(pila) ==  1, "El tamaño de la pila es 1 si tiene un elemento");
  pila_destruir(pila); 
}

void la_pila_esta_vacia_si_tiene_cero_elementos()
{
  pila_t *pila = pila_crear();
  pa2m_afirmar(pila_vacia(pila) == true, "La pila tiene 0 elementos, está vacía");
  pila_apilar(pila, NULL);
  pa2m_afirmar(pila_vacia(pila) == false, "La pila tiene elementos, no está vacía");
  pila_destruir(pila); 
}

void crear_una_cola_me_devuelve_una_cola_vacia()
{
  cola_t *cola = cola_crear();
  pa2m_afirmar(cola_vacia(cola) == true, "Crear una cola me devuelve una cola vacía");
  cola_destruir(cola);
}

void encolar_un_elemento_incrementa_el_tamanio_de_la_cola_y_el_frente_es_el_primer_elemento_encolado(){
  cola_t *cola = cola_crear();
  int elemento1 = 10, elemento2 = 3, elemento3 = 6;
  cola_encolar(cola, &elemento1);
  cola_encolar(cola, &elemento2);
  cola_encolar(cola, &elemento3);
  pa2m_afirmar(cola_tamanio(cola) == 3, "El tamaño de la  cola es 3 después de encolar 3 elementos a una cola vacía");
  pa2m_afirmar(cola_frente(cola) == &elemento1, "El frente de la cola es el correcto");
  cola_destruir(cola);
}

void encolar_en_una_cola_nula_es_un_error()
{
  int elemento = 5;
  pa2m_afirmar(!cola_encolar(NULL, &elemento), "No puedo encolar en una cola NULL");
}

void desencolar_una_cola_me_devuelve_el_elemento_desencoaldo_y_el_tamanio_de_la_cola_disminuye()
{
  cola_t *cola = cola_crear();
  int elemento1 = 10, elemento2 = 3, elemento3 = 6;
  cola_encolar(cola, &elemento1);
  cola_encolar(cola, &elemento2);
  cola_encolar(cola, &elemento3);
  pa2m_afirmar(cola_desencolar(cola) == &elemento1, "El elemento desencolado es 3");
  pa2m_afirmar(cola_tamanio(cola) == 2, "El tamaño de la cola despues de desencolar es 2");
  cola_destruir(cola);
}

void desencolar_en_una_cola_nula_es_un_error()
{
  pa2m_afirmar(!cola_desencolar(NULL), "No puedo desencolar en una cola nula");
}

void el_frente_de_la_cola_devuelve_el_primer_elemento_encolado()
{
  cola_t *cola = cola_crear();
  int elemento1 = 10, elemento2 = 3, elemento3 = 6;
  cola_encolar(cola, &elemento1);
  cola_encolar(cola, &elemento2);
  cola_encolar(cola, &elemento3);
  pa2m_afirmar(cola_frente(cola) == &elemento1, "El elemento que se encuentra en el frente es 10");
  cola_destruir(cola);
}

void si_el_elemento_del_frente_de_la_cola_es_nulo_devuelve_null()
{
  cola_t *cola = cola_crear();
  cola_encolar(cola, NULL);
  pa2m_afirmar(!cola_frente(cola), "El elemento del frente es nulo, devuelve NULL");
  cola_destruir(cola);  
}

void si_la_cola_no_existe_el_tamanio_es_cero_de_lo_contrario_el_tamanio_es_la_cantidad_de_elementos()
{
  pa2m_afirmar(cola_tamanio(NULL) == 0, "La cola no existe, devuelve tamaño 0");
  cola_t *cola = cola_crear();
  cola_encolar(cola, NULL);
  pa2m_afirmar(cola_tamanio(cola) ==  1, "El tamaño de la cola es 1 si tiene un elemento");
  cola_destruir(cola); 
}

void la_cola_esta_vacia_si_tiene_cero_elementos()
{
  cola_t *cola = cola_crear();
  pa2m_afirmar(cola_vacia(cola) == true, "La cola tiene 0 elementos, está vacía");
  cola_encolar(cola, NULL);
  pa2m_afirmar(cola_vacia(cola) == false, "La cola tiene elementos, no está vacía");
  cola_destruir(cola); 
}

void pruebas_lista()
{
  pa2m_nuevo_grupo("Crear una lista");
  crear_una_lista_me_devuelve_una_lista_de_cantidad_0_y_nodos_nulos();
  se_crea_una_lista_y_al_hacerlo_esta_vacia();

  pa2m_nuevo_grupo("Insertar elemento al final de una lista");
  si_la_lista_es_nula_no_puedo_insertar();
  al_insertar_un_elemento_nodo_fin_apunta_a_dicho_elemento();
  al_insertar_elemento_en_lista_vacia_nodo_inicio_y_nodo_fin_apuntan_a_dicho_elemento();

  pa2m_nuevo_grupo("Insertar en posición");
  si_la_posicion_no_existe_inserta_el_elemento_al_final();
  puedo_insertar_un_elemento_nulo_y_la_cantidad_de_elementos_se_incrementa();
  al_insertar_en_la_posicion_0_nodo_inicio_apunta_al_nodo_insertado();

  pa2m_nuevo_grupo("Lista quitar");
  si_la_lista_es_null_no_puedo_quitar_elementos();
  si_la_lista_esta_vacia_no_puedo_quitar_elementos();
  quitar_un_elemento_me_devuelve_el_elemento_y_reduce_la_cantidad_de_la_lista();
  si_quito_el_unico_elemento_nodo_inicio_es_nulo_y_nodo_fin_tambien();

  pa2m_nuevo_grupo("Quitar de posición");
  si_la_posicion_no_existe_quita_el_ultimo_elemento();
  si_quito_el_unico_elemento_nodo_inicio_es_nulo_y_nodo_fin_tambien_version_2();
  al_quitar_una_posicion_valida_la_cantidad_de_la_lista_disminuye_y_el_siguiente_del_nodo_anterior_al_quitado_apunta_al_siguiente_del_quitado();
  al_quitar_la_ultima_posicion_nodo_fin_apunta_al_anterior_del_quitado();

  pa2m_nuevo_grupo("Elemento en posición");
  si_la_posicion_no_existe_devuelve_null();
  al_buscar_una_posicion_existente_devuelve_el_elemento_de_dicha_posicion();

  pa2m_nuevo_grupo("Buscar elemento");
  si_el_elemento_no_existe_devuelve_null_de_lo_contrario_devuelve_el_elemento_buscado();

  pa2m_nuevo_grupo("Lista primero");
  si_la_lista_esta_vacia_lista_primero_devuelve_null();
  si_la_lista_es_nula_lista_primero_devuelve_null();
  lista_primero_devuelve_el_primer_elemento_de_una_lista_que_contiene_elementos();

  pa2m_nuevo_grupo("Lista ultimo");
  si_la_lista_esta_vacia_lista_ultimo_devuelve_null();
  si_la_lista_es_nula_lista_ultimo_devuelve_null();
  lista_ultimo_devuelve_el_ultimo_elemento_de_una_lista_que_contiene_elementos();

  pa2m_nuevo_grupo("Lista vacía");
  la_lista_esta_vacia_entonces_devuelve_true();
  la_lista_no_existe_entonces_devuelve_true();
  la_lista_existe_y_tiene_elementos_entonces_devuelve_false();

  pa2m_nuevo_grupo("Lista tamaño");
  si_la_lista_esta_vacia_su_tamanio_es_cero();
  el_tamanio_de_la_lista_se_corresponde_con_la_cantidad_de_elementos_de_la_misma();

  pa2m_nuevo_grupo("Lista destruir todo");
  destruir_todo_destruye_la_lista_y_cada_bloque_de_la_misma();

  pa2m_nuevo_grupo("Crear iterador");
  crear_un_iterador_correctamente_me_devuelve_el_iterador_creado();
  no_es_posible_crear_un_iterador_si_la_lista_no_existe_o_es_invalida();
  
  pa2m_nuevo_grupo("Iterador tiene siguiente");
  iterador_tiene_siguiente_devuelve_true_si_hay_mas_elementos_sobre_los_cuales_iterar();
  iterador_tiene_siguiente_devuelve_false_si_no_hay_mas_elementos_sobre_los_cuales_iterar();

  pa2m_nuevo_grupo("Iterador avanzar");
  si_quedan_elementos_puedo_avanzar_el_iterador();

  pa2m_nuevo_grupo("Iterador elemento actual");
  lista_iterador_elemento_actual_devuelve_el_elemento_actual_del_iterador_y_null_si_no_existe_el_elemento();

  pa2m_nuevo_grupo("Lista con cada elemento. Iterador interno.");
  si_la_funcion_no_existe_no_puedo_iterar();
  lista_con_cada_elemento_devuelve_la_cantidad_de_elementos_iterados();

}

void pruebas_pila()
{
  pa2m_nuevo_grupo("Crear una pila");
  crear_una_pila_me_devuelve_una_pila_vacia();

  pa2m_nuevo_grupo("Pruebas apilar");
  apilar_un_elemento_incrementa_el_tamanio_de_la_pila_y_el_tope_es_el_ultimo_elemento_apilado();
  apilar_en_una_pila_nula_es_un_error();

  pa2m_nuevo_grupo("Pruebas desapilar");
  desapilar_una_pila_me_devuelve_el_elemento_desapilado_y_el_tamanio_de_la_pila_disminuye();
  desapilar_en_una_pila_nula_es_un_error();

  pa2m_nuevo_grupo("Tope de la pila");
  el_tope_de_la_pila_devuelve_el_ultimo_elemento_insertado();
  si_el_elemento_del_tope_de_la_pila_es_nulo_devuelve_null();

  pa2m_nuevo_grupo("Tamaño de la pila");
  si_la_pila_no_existe_el_tamanio_es_cero_de_lo_contrario_el_tamanio_es_la_cantidad_de_elementos();

  pa2m_nuevo_grupo("Pila vacía");
  la_pila_esta_vacia_si_tiene_cero_elementos();
}

void pruebas_cola()
{
  pa2m_nuevo_grupo("Crear una cola");
  crear_una_cola_me_devuelve_una_cola_vacia();

  pa2m_nuevo_grupo("Pruebas encolar");
  encolar_un_elemento_incrementa_el_tamanio_de_la_cola_y_el_frente_es_el_primer_elemento_encolado();
  encolar_en_una_cola_nula_es_un_error();

  pa2m_nuevo_grupo("Pruebas desencolar");
  desencolar_una_cola_me_devuelve_el_elemento_desencoaldo_y_el_tamanio_de_la_cola_disminuye();
  desencolar_en_una_cola_nula_es_un_error();

  pa2m_nuevo_grupo("Frente de la cola");
  el_frente_de_la_cola_devuelve_el_primer_elemento_encolado();
  si_el_elemento_del_frente_de_la_cola_es_nulo_devuelve_null();

  pa2m_nuevo_grupo("Tamaño de la cola");
  si_la_cola_no_existe_el_tamanio_es_cero_de_lo_contrario_el_tamanio_es_la_cantidad_de_elementos();
  
  pa2m_nuevo_grupo("Cola vacía");
  la_cola_esta_vacia_si_tiene_cero_elementos();
}

int main() {

  pa2m_nuevo_grupo("Pruebas de lista");
  pruebas_lista();

  pa2m_nuevo_grupo("Pruebas de pila");
  pruebas_pila();

  pa2m_nuevo_grupo("Pruebas de cola");
  pruebas_cola();  

  return pa2m_mostrar_reporte();
}