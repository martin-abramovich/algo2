#include "src/abb.h"
#include <stdio.h>
#include "pa2mm.h"
#include "string.h"

typedef struct cosa {
  int clave;
  char contenido[10];
} cosa;

int comparar_cosas(void *c1, void *c2)
{
  cosa *cosa1 = c1;
  cosa *cosa2 = c2;
  return cosa1->clave - cosa2->clave;
}

void al_crear_un_arbol_se_crea_un_arbol_de_tamanio_0_y_nodo_raiz_nulo_y_comparador_valido()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  pa2m_afirmar(arbol->tamanio == 0, "Se crea un arbol de tamaño 0");
  pa2m_afirmar(arbol->nodo_raiz == NULL, "La raiz del arbol creado es nula");
  pa2m_afirmar(arbol->comparador != NULL, "El comparador del arbol no es nulo");
  abb_destruir(arbol);
}

void al_insertar_el_primer_elemento_este_es_la_raiz_del_arbol()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100;
  abb_insertar(arbol, &n1);
  pa2m_afirmar(arbol->tamanio == 1, "Se inserta un elemento correctamente");
  pa2m_afirmar(arbol->nodo_raiz->elemento == &n1, "El primer elemento insertado es la raiz del arbol");
  abb_destruir(arbol);
}

void los_elementos_menores_se_insertan_a_izquierda_y_los_mayores_a_derecha()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, n2 = 90, n3 = 110, n4 = 80, n5 = 95;
  abb_insertar(arbol, &n1);
  abb_insertar(arbol, &n2);
  abb_insertar(arbol, &n3);
  abb_insertar(arbol, &n4);
  abb_insertar(arbol, &n5);
  pa2m_afirmar(arbol->tamanio == 5, "Se insertaron 5 elementos");
  pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &n2, "El elemento insertado menor se inserta a la izquierda");
  pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &n3, "El elemento insertado mayor se inserta a la derecha");
  pa2m_afirmar(arbol->nodo_raiz->izquierda->izquierda->elemento == &n4, "El elemento insertado menor se inserta a la izquierda");
  pa2m_afirmar(arbol->nodo_raiz->izquierda->derecha->elemento == &n5, "El elemento insertado mayor se inserta a la derecha");
  abb_destruir(arbol);
}

void no_puedo_quitar_de_un_arbol_nulo_o_vacio()
{
  int quitar = 10;
  abb_t *arbol = NULL;
  pa2m_afirmar(abb_quitar(arbol, &quitar) == NULL, "No puedo quitar nada de un abb nulo");
  arbol = abb_crear(comparar_cosas);
  pa2m_afirmar(abb_quitar(arbol, &quitar) == NULL, "No puedo quitar nada de un abb vacío");
  abb_destruir(arbol);
}

void si_no_encuentra_el_elemento_a_quitar_devuelve_null()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, n2 = 90, n3 = 110, n4 = 80, n5 = 95, quitar = 8;
  abb_insertar(arbol, &n1);
  abb_insertar(arbol, &n2);
  abb_insertar(arbol, &n3);
  abb_insertar(arbol, &n4);
  abb_insertar(arbol, &n5);
  pa2m_afirmar(abb_quitar(arbol, &quitar) == NULL, "Si se intenta quitar un elemento que no está en el abb devuelve NULL");
  abb_destruir(arbol);
}

void al_borrar_un_elemento_el_tamanio_del_arbol_disminuye()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, quitar = 100;
  abb_insertar(arbol, &n1);
  abb_quitar(arbol, &quitar);
  pa2m_afirmar(abb_vacio(arbol), "Elimino el único elemento correctamente");
  pa2m_afirmar(abb_tamanio(arbol) == 0, "El tamaño del arbol disminuye al quitar elemento");
  abb_destruir(arbol);
}

void si_borro_un_nodo_con_un_hijo_el_hijo_sube_al_lugar_del_borrado()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, n2 = 90, quitar = 100;
  abb_insertar(arbol, &n1);
  abb_insertar(arbol, &n2);
  abb_quitar(arbol, &quitar);
  pa2m_afirmar(arbol->nodo_raiz->elemento == &n2, "Si elimino un elemento con un solo hijo, el hijo ocupa su lugar");
  abb_destruir(arbol);
}

void si_borro_un_nodo_con_dos_hijos_se_reemplaza_con_el_predecesor_inorden()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, n2 = 90, n3 = 110, quitar = 100;
  abb_insertar(arbol, &n1);
  abb_insertar(arbol, &n2);
  abb_insertar(arbol, &n3);
  abb_quitar(arbol, &quitar);
  pa2m_afirmar(arbol->nodo_raiz->elemento == &n2, "Si elimino un nodo con dos hijos, se reemplaza por el predecesor inorden");
  abb_destruir(arbol);
}

void al_buscar_un_elemento_me_devuelve_el_elemento_encontrado()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, n2 = 90, n3 = 110, n4 = 80, n5 = 95, buscado1 = 70, buscado2 = 110, busc3 = 100;
  abb_insertar(arbol, &n1);
  abb_insertar(arbol, &n2);
  abb_insertar(arbol, &n3);
  abb_insertar(arbol, &n4);
  abb_insertar(arbol, &n5);
  int *elemento = abb_buscar(arbol, &buscado1);
  pa2m_afirmar(elemento == NULL, "El elemento buscado no está en el arbol. Retorna NULL");
  elemento = abb_buscar(arbol, &buscado2);
  pa2m_afirmar(elemento == &n3, "Se encontró el elemento, retorna el elemento.");
  elemento = abb_buscar(arbol, &busc3);
  pa2m_afirmar(elemento == &n1, "Se encontró el elemento, retorna el elemento.");
  abb_destruir(arbol);
}

void no_puedo_buscar_en_un_abb_vacio_o_nulo()
{
  int buscar = 10;
  abb_t *arbol = NULL;
  pa2m_afirmar(abb_buscar(arbol, &buscar) == NULL, "No puedo buscar en un abb nulo");
  arbol = abb_crear(comparar_cosas);
  pa2m_afirmar(abb_buscar(arbol, &buscar) == NULL, "No puedo buscar en un abb vacío");
  abb_destruir(arbol);
}

void el_arbol_tiene_cero_elementos_o_es_nulo_entonces_devuelve_false_de_lo_contrario_devuelve_true()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  pa2m_afirmar(abb_vacio(NULL), "El arbol no existe, la función devuelve true");
  pa2m_afirmar(abb_vacio(arbol), "El arbol no tiene elementos, la función devuelve true");
  int n1 = 232;
  abb_insertar(arbol, &n1);
  pa2m_afirmar(!abb_vacio(arbol), "El arbol tiene elementos, la función devuelve false");
  abb_destruir(arbol);
}

void si_el_arbol_esta_vacio_su_tamanio_es_cero()
{
 pa2m_afirmar(abb_tamanio(NULL) == 0, "El arbol es nulo. Su tamaño es 0.");
 abb_t *arbol = abb_crear(comparar_cosas);
 pa2m_afirmar(abb_tamanio(arbol) == 0, "El arbol esta vacío. Su tamaño es 0.");
 abb_destruir(arbol);
}

void el_tamanio_del_arbol_se_corresponde_con_la_cantidad_de_elementos_del_mismo()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, n2 = 90;
  abb_insertar(arbol, &n1);
  abb_insertar(arbol, &n2);
  pa2m_afirmar(abb_tamanio(arbol) == 2, "El arbol tiene 2 elementos. Su tamaño es 2.");
  abb_insertar(arbol, &n1);
  pa2m_afirmar(abb_tamanio(arbol) == 3, "Se inserta un elemento, ahora su tamaño es 3");
  abb_destruir(arbol);
}

bool existen_parametros(void *elemento, void *extra)
{
  return elemento && extra;
}

void devuelve_la_cantidad_de_veces_que_fue_invocada_la_funcion()
{
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, n2 = 90, acumulador = 0;
  pa2m_afirmar(abb_con_cada_elemento(NULL, INORDEN, existen_parametros, &acumulador) == 0, "Si el arbol es NULL devuelve 0");  
  abb_insertar(arbol, &n1);
  abb_insertar(arbol, &n2);
  pa2m_afirmar(abb_con_cada_elemento(arbol, INORDEN, NULL, &acumulador) == 0, "Si la función es nula devuelve 0");
  pa2m_afirmar(abb_con_cada_elemento(arbol, INORDEN, existen_parametros, &acumulador) == 2, "Devuelve la cantidad de veces que fue invocada la función");
  abb_destruir(arbol);
}

void se_almacenan_correctamente_los_elementos_en_el_vector()
{
  cosa *elementos[10];
  size_t cantidad = abb_recorrer(NULL, INORDEN, (void **)elementos, 10);
  pa2m_afirmar(cantidad == 0, "No puedo recorrer un árbol nulo");
  abb_t *arbol = abb_crear(comparar_cosas);
  int n1 = 100, n2 = 80, n3 = 110;
  bool orden_correcto = false;
  abb_insertar(arbol, &n1);
  abb_insertar(arbol, &n2);
  abb_insertar(arbol, &n3);
  cantidad = abb_recorrer(arbol, INORDEN, (void **)elementos, 10);
  pa2m_afirmar(cantidad == 3, "La cantidad de elementos almacenados es la correcta");
  if (elementos[0]->clave == n2 && elementos[1]->clave == n1 && elementos[2]->clave == n3)
    orden_correcto = true;
  pa2m_afirmar(orden_correcto, "Se almacenan los valores segun recorrido inorden");
  cantidad = abb_recorrer(arbol, PREORDEN, (void **)elementos, 10);
  if (elementos[0]->clave == n1 && elementos[1]->clave == n2 && elementos[2]->clave == n3)
    orden_correcto = true;
  pa2m_afirmar(orden_correcto, "Se almacenan los valores segun recorrido preorden");
  cantidad = abb_recorrer(arbol, POSTORDEN, (void **)elementos, 10);
  if (elementos[0]->clave == n2 && elementos[1]->clave == n3 && elementos[2]->clave == n1)
    orden_correcto = true;
  pa2m_afirmar(orden_correcto, "Se almacenan los valores segun recorrido postorden");
  abb_destruir(arbol);
}

int main()
{
  pa2m_nuevo_grupo("Pruebas de ABB");

  pa2m_nuevo_grupo("Crear ABB");
  al_crear_un_arbol_se_crea_un_arbol_de_tamanio_0_y_nodo_raiz_nulo_y_comparador_valido();

  pa2m_nuevo_grupo("ABB Insertar");
  al_insertar_el_primer_elemento_este_es_la_raiz_del_arbol();
  los_elementos_menores_se_insertan_a_izquierda_y_los_mayores_a_derecha();

  pa2m_nuevo_grupo("ABB Quitar");
  no_puedo_quitar_de_un_arbol_nulo_o_vacio();
  si_no_encuentra_el_elemento_a_quitar_devuelve_null();
  al_borrar_un_elemento_el_tamanio_del_arbol_disminuye();
  si_borro_un_nodo_con_un_hijo_el_hijo_sube_al_lugar_del_borrado();
  si_borro_un_nodo_con_dos_hijos_se_reemplaza_con_el_predecesor_inorden();

  pa2m_nuevo_grupo("ABB Buscar");
  al_buscar_un_elemento_me_devuelve_el_elemento_encontrado();
  no_puedo_buscar_en_un_abb_vacio_o_nulo();

  pa2m_nuevo_grupo("ABB Vacío");
  el_arbol_tiene_cero_elementos_o_es_nulo_entonces_devuelve_false_de_lo_contrario_devuelve_true();

  pa2m_nuevo_grupo("ABB Tamaño");
  si_el_arbol_esta_vacio_su_tamanio_es_cero();
  el_tamanio_del_arbol_se_corresponde_con_la_cantidad_de_elementos_del_mismo();

  pa2m_nuevo_grupo("ABB con cada elemento");  
  devuelve_la_cantidad_de_veces_que_fue_invocada_la_funcion();

  pa2m_nuevo_grupo("ABB recorrer");
  se_almacenan_correctamente_los_elementos_en_el_vector();

  return pa2m_mostrar_reporte();
}
