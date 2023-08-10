#include "src/sala.h"
#include <stdio.h>
#include <stdlib.h>

#define CANT_PARAMETROS 3
#define ERROR -1

void validar_interaccion(sala_t *sala, const char *verbo, const char *objeto1, const char *objeto2, const char *string_a_mostrar)
{
	if (sala_es_interaccion_valida(sala, verbo, objeto1, objeto2)) {
		printf("%s: Válido\n", string_a_mostrar);
	} else {
		printf("%s: Inválido\n", string_a_mostrar);
	}
}

int main(int argc, char *argv[])
{
	if (argc != CANT_PARAMETROS) {
		return ERROR;
	}

	//Los archivos deben venir como parámetros del main
	sala_t *sala = sala_crear_desde_archivos(argv[1], argv[2]);

	if (sala == NULL) {
		printf("Error al crear la sala de escape\n");
		return ERROR;
	}

	//Mostrar todos los objetos en la sala

	int cant_obj = 0;
	char **objetos = sala_obtener_nombre_objetos(sala, &cant_obj);

	printf("Objetos...\n");
	for (int i = 0; i < cant_obj; i++){
		printf("%i: %s\n", i, objetos[i]);
	}

	//Mostrar si son válidas las siguientes interacciones
	//1. examinar habitacion
	//2. abrir pokebola
	//3. usar llave cajon
	//4. quemar mesa

	printf("Interacciones...\n");
	validar_interaccion(sala, "examinar", "habitacion", "", "Examinar la habitación");
	validar_interaccion(sala, "abrir", "pokebola", "", "Abrir pokebola");
	validar_interaccion(sala, "examinar", "habitacion", "", "Usar la llave en el cajón");
	validar_interaccion(sala, "quemar", "mesa", "", "Quemar la mesa");

	sala_destruir(sala);

	return 0;
}
