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

    printf("==================================================\n");
    printf("  Cargando base de datos de Clinica Medica...    \n");
    printf("==================================================\n");
    cargarArchivo();

    do {
        printf("\n================================================\n");
        printf("│       SISTEMA DE GESTION DE CITAS MEDICAS      │\n");
        printf("================================================\n");
        printf("  [1] Registrar nueva cita\n");
        printf("  [2] Listar todas las citas\n");
        printf("  [3] Buscar cita por codigo\n");
        printf("  [4] Actualizar datos de una cita\n");
        printf("  [5] Eliminar cita del sistema\n");
        printf("  [6] Guardar cambios manualmente (CSV)\n");
        printf("  [7] Guardar y Salir\n");
        printf("--------------------------------------------------\n");
        printf("Seleccione una opcion (1-7): ");

        leerLinea(entrada, sizeof(entrada));
        opcion = atoi(entrada);

        switch (opcion) {
            case 1:
                printf("\n>>> MÓDULO: REGISTRO DE CITA <<<\n");
                registrarCita();
                break;
            case 2:
                printf("\n>>> MÓDULO: LISTADO GENERAL DE CITAS <<<\n");
                listarCitas();
                break;
            case 3:
                printf("\n>>> MÓDULO: BÚSQUEDA DE CITAS <<<\n");
                buscarCita();
                break;
            case 4:
                printf("\n>>> MÓDULO: MODIFICACIÓN DE CITAS <<<\n");
                actualizarCita();
                break;
            case 5:
                printf("\n>>> MÓDULO: ELIMINACIÓN DE CITAS <<<\n");
                eliminarCita();
                break;
            case 6:
                printf("\n>>> PERSISTENCIA: RESPALDANDO EN BASE DE DATOS <<<\n");
                guardarArchivo();
                break;
            case 7:
                printf("\n>>> CERRANDO SESIÓN <<<\n");
                guardarArchivo();
                printf("Guardando cambios automaticamente en 'citas.csv'...\n");
                printf("¡Hasta pronto, gracias por usar el sistema!\n");
                break;
            default:
                printf("\n[ERROR]: Opcion invalida. Ingrese un numero entre 1 y 7.\n");
        }
        printf("\n--------------------------------------------------\n");

    } while (opcion != 7);

    return 0;
}