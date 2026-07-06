/* ===========================================================
 * main.c
 * Sistema de Agendamiento de Citas Medicas
 * ---------------------------------------------------------
 * Contiene unicamente el menu principal del sistema. Toda la
 * logica esta implementada en citas.c / citas.h
 * =========================================================== */

#include <stdio.h>
#include <stdlib.h>
#include "citas.h"

int main(void) {
    int opcion;
    char entrada[10];

    cargarArchivo();

    do {
        printf("\n============================================\n");
        printf("   SISTEMA DE AGENDAMIENTO DE CITAS MEDICAS\n");
        printf("============================================\n");
        printf("1. Registrar cita\n");
        printf("2. Listar citas\n");
        printf("3. Buscar cita\n");
        printf("4. Actualizar cita\n");
        printf("5. Eliminar cita\n");
        printf("6. Guardar archivo\n");
        printf("7. Salir\n");
        printf("--------------------------------------------\n");
        printf("Seleccione una opcion: ");

        leerLinea(entrada, sizeof(entrada));
        opcion = atoi(entrada);

        switch (opcion) {
            case 1:
                registrarCita();
                break;
            case 2:
                listarCitas();
                break;
            case 3:
                buscarCita();
                break;
            case 4:
                actualizarCita();
                break;
            case 5:
                eliminarCita();
                break;
            case 6:
                guardarArchivo();
                break;
            case 7:
                guardarArchivo();
                printf("\nGuardando cambios y saliendo del sistema...\n");
                printf("Hasta pronto.\n");
                break;
            default:
                printf("\nOpcion invalida. Intente nuevamente.\n");
        }

    } while (opcion != 7);

    return 0;
}