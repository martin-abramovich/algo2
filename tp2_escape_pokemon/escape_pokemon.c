#include "src/sala.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANT_PARAMETROS 3
#define ERROR -1
#define MAX_LINEA 1024
#define FORMATO "%s %s %s"

void mostrar_mensaje(const char *mensaje, enum tipo_accion accion, void *aux)
{
	printf("%s\n", mensaje);
}

void ayudar()
{
	printf("\nAl comenzar el juego solo conocés un objeto. Interactuá con el objeto conocido para descubrir más objetos.\n");
	printf("Interactuá con los objetos que descubrís para lograr escapar de la sala\n");
	printf("ayuda: comandos disponibles\n");
	printf("agarrar «objeto»: agarra algún objeto conocido\n");
	printf("describir «objeto»: describe algún objeto conocido\n");
	printf("salir: finaliza el juego\n");

	printf("\nPodrías empezar examinando el objeto conocido\n");

}

void mostrar_objetos_conocidos(sala_t *sala)
{
	int cantidad = 0;
	char **objetos_conocidos = sala_obtener_nombre_objetos_conocidos(sala, &cantidad);
	printf("\nObjetos conocidos\n");
	for (int i = 0; i < cantidad; i++)
		printf("%s\n", objetos_conocidos[i]);
	printf("\n");
}

void mostrar_objetos_poseidos(sala_t *sala)
{
	int cantidad = 0;
	char **objetos_poseidos = sala_obtener_nombre_objetos_poseidos(sala, &cantidad);
	printf("Objetos poseidos\n");
	for (int i = 0; i < cantidad; i++)
		printf("%s\n", objetos_poseidos[i]);
	printf("\n");
}

int main(int argc, char *argv[])
{
	
	if (argc != CANT_PARAMETROS)
		return ERROR;
	
	//Los archivos deben venir como parámetros del main
	sala_t *sala = sala_crear_desde_archivos(argv[1], argv[2]);

	if (sala == NULL) {
		printf("Error al crear la sala de escape\n");
		return ERROR;
	}

	char comando[MAX_LINEA];
	char objeto1[MAX_LINEA];
	char objeto2[MAX_LINEA];
	char aux[MAX_LINEA];

	char linea[MAX_LINEA];

	bool salir = false;
	bool pudo_escapar = false;

	while (!salir && !pudo_escapar){

		mostrar_objetos_conocidos(sala);
		mostrar_objetos_poseidos(sala);

		printf("Ingrese un comando, en minúscula por favor :)\n");
		printf("Comandos disponibles: ayuda - agarrar «objeto» - describir «objeto» - «accion» «objeto» - «accion» «objeto1» «objeto2» - salir\n\n");

		char* lectura = fgets(linea, MAX_LINEA, stdin);
		if (!lectura)
			return ERROR;

		int valor = system("clear");
		if (valor == ERROR)
			return ERROR;

		int leidos = sscanf(linea, FORMATO, comando, objeto1, objeto2);

		if (leidos == 1 && strcmp(comando, "ayuda") == 0)
			ayudar();

		if (leidos == 1 && strcmp(comando, "salir") == 0)
			salir = true;

		if (leidos == 2 && strcmp(comando, "agarrar") == 0)
			sala_agarrar_objeto(sala, objeto1);
		
		if (leidos == 2 && strcmp(comando, "describir") == 0) {
			char* descripcion = sala_describir_objeto(sala, objeto1);
			if (!descripcion)
				printf("No describe nada\n");
			printf("%s\n", descripcion);
		}
		
		if (leidos == 2 && strcmp(comando, "describir") != 0 && strcmp(comando, "agarrar") != 0)
			sala_ejecutar_interaccion(sala, comando, objeto1, "", mostrar_mensaje, aux);

		if (leidos == 3)
			sala_ejecutar_interaccion(sala, comando, objeto1, objeto2, mostrar_mensaje, aux);

		pudo_escapar = sala_escape_exitoso(sala);
	}

	if (pudo_escapar)
		printf("Lograste escapar ««««GANASTE»»»»\n");

	sala_destruir(sala);

	return 0;
}