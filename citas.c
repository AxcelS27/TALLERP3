
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "citas.h"

/* Definicion de variables globales */
Cita citas[MAX_CITAS];
int totalCitas = 0;

/* 
 * limpiarSaltoLinea:
   Elimina el caracter '\n' final de una cadena, si existe.
   Se usa despues de leer lineas con fgets.
 */
void limpiarSaltoLinea(char *cadena) {
    size_t len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n') {
        cadena[len - 1] = '\0';
    }
}

/* 
 * leerLinea:
   Lee una linea de texto desde teclado de forma segura usando
   fgets (evita desbordamiento de buffer). Si el usuario escribe
   mas caracteres de los que caben en el buffer, se descartan
   los caracteres sobrantes que quedan en el stdin.
   Retorna 1 si pudo leer, 0 si hubo error (EOF).
 */
int leerLinea(char *destino, int tamano) {
    if (fgets(destino, tamano, stdin) == NULL) {
        destino[0] = '\0';
        return 0;
    }

    if (strchr(destino, '\n') != NULL) {
        limpiarSaltoLinea(destino);
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            ;
        }
    }
    return 1;
}

/*
 * contieneSubcadena:
   Compara si "subcadena" aparece dentro de "cadena" sin importar
   mayusculas/minusculas. Se usa para la busqueda de pacientes
   por nombre (busqueda parcial).
*/
int contieneSubcadena(const char *cadena, const char *subcadena) {
    char cadenaMin[LEN_NOMBRE];
    char subMin[LEN_NOMBRE];
    int i;

    for (i = 0; cadena[i] != '\0' && i < LEN_NOMBRE - 1; i++) {
        cadenaMin[i] = (char)tolower((unsigned char)cadena[i]);
    }
    cadenaMin[i] = '\0';

    for (i = 0; subcadena[i] != '\0' && i < LEN_NOMBRE - 1; i++) {
        subMin[i] = (char)tolower((unsigned char)subcadena[i]);
    }
    subMin[i] = '\0';

    return strstr(cadenaMin, subMin) != NULL;
}

/* 
 * buscarIndice:
   Recorre el arreglo de citas buscando una coincidencia exacta
   de codigo_cita. Retorna el indice si la encuentra, o -1 si no.
*/
int buscarIndice(const char *codigo) {
    int i;
    for (i = 0; i < totalCitas; i++) {
        if (strcmp(citas[i].codigo_cita, codigo) == 0) {
            return i;
        }
    }
    return -1;
}

/*
 * codigoExiste:
   Indica si ya existe una cita registrada con ese codigo.
*/
int codigoExiste(const char *codigo) {
    return buscarIndice(codigo) != -1;
}

/* 
 * horarioOcupado:
   Verifica si ya existe una cita en la misma fecha y hora.
   El parametro codigoExcluir permite ignorar la propia cita
   cuando se esta actualizando (para no chocar contra si misma).
   Si codigoExcluir es NULL, se compara contra todas las citas.
*/
int horarioOcupado(const char *fecha, const char *hora, const char *codigoExcluir) {
    int i;
    for (i = 0; i < totalCitas; i++) {
        if (strcmp(citas[i].fecha, fecha) == 0 && strcmp(citas[i].hora, hora) == 0) {
            if (codigoExcluir == NULL || strcmp(citas[i].codigo_cita, codigoExcluir) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

/*
 * validarFecha:
   Valida que la fecha tenga formato exacto DD/MM/AAAA, que los
   separadores esten en la posicion correcta, que todos los demas
   caracteres sean digitos, y que dia/mes/anio sean logicamente
   validos (incluye control de anios bisiestos para febrero).
*/
int validarFecha(const char *fecha) {
    int dia, mes, anio;
    int i;
    int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDias;

    if (strlen(fecha) != 10) {
        return 0;
    }

    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (fecha[i] != '/') {
                return 0;
            }
        } else {
            if (!isdigit((unsigned char)fecha[i])) {
                return 0;
            }
        }
    }

    if (sscanf(fecha, "%2d/%2d/%4d", &dia, &mes, &anio) != 3) {
        return 0;
    }

    if (anio < 1900 || anio > 2100) {
        return 0;
    }
    if (mes < 1 || mes > 12) {
        return 0;
    }

    maxDias = diasMes[mes - 1];
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)) {
        maxDias = 29;
    }

    if (dia < 1 || dia > maxDias) {
        return 0;
    }

    return 1;
}

/* 
 * validarHora:
   Valida que la hora tenga formato exacto HH:MM, con separador
   en la posicion correcta y valores logicos (0-23 para horas, 0-59 para minutos).
*/
int validarHora(const char *hora) {
    int hh, mm;
    int i;

    if (strlen(hora) != 5) {
        return 0;
    }

    for (i = 0; i < 5; i++) {
        if (i == 2) {
            if (hora[i] != ':') {
                return 0;
            }
        } else {
            if (!isdigit((unsigned char)hora[i])) {
                return 0;
            }
        }
    }

    if (sscanf(hora, "%2d:%2d", &hh, &mm) != 2) {
        return 0;
    }

    if (hh < 0 || hh > 23) {
        return 0;
    }
    if (mm < 0 || mm > 59) {
        return 0;
    }

    return 1;
}
/* 
 * registrarCita:
   Solicita al usuario los datos de una nueva cita, validando
   cada campo (codigo unico, fecha, hora y disponibilidad de
   horario) antes de agregarla al arreglo global de citas.
*/
void registrarCita(void) {
    Cita nueva;
    char buffer[LEN_NOMBRE];
    char horaBuffer[LEN_HORA];
    int valido;

    printf("\n===== REGISTRAR NUEVA CITA =====\n");

    if (totalCitas >= MAX_CITAS) {
        printf("No se pueden registrar mas citas. Se alcanzo el limite maximo (%d).\n", MAX_CITAS);
        return;
    }

    do {
        printf("Ingrese el codigo de la cita (maximo %d caracteres): ", LEN_CODIGO - 1);
        leerLinea(buffer, LEN_CODIGO);
        if (strlen(buffer) == 0) {
            printf("El codigo no puede estar vacio.\n");
            valido = 0;
        } else if (codigoExiste(buffer)) {
            printf("Ya existe una cita con ese codigo. Ingrese uno diferente.\n");
            valido = 0;
        } else {
            valido = 1;
        }
    } while (!valido);
    strcpy(nueva.codigo_cita, buffer);

    printf("Ingrese el nombre del paciente: ");
    leerLinea(nueva.nombre_paciente, LEN_NOMBRE);

    printf("Ingrese la especialidad: ");
    leerLinea(nueva.especialidad, LEN_ESPECIALIDAD);

    do {
        printf("Ingrese la fecha (DD/MM/AAAA): ");
        leerLinea(buffer, LEN_FECHA);
        if (!validarFecha(buffer)) {
            printf("Fecha invalida. Verifique el formato y los valores.\n");
            valido = 0;
        } else {
            valido = 1;
        }
    } while (!valido);
    strcpy(nueva.fecha, buffer);

    do {
        printf("Ingrese la hora (HH:MM): ");
        leerLinea(horaBuffer, LEN_HORA);
        if (!validarHora(horaBuffer)) {
            printf("Hora invalida. Verifique el formato y los valores.\n");
            valido = 0;
        } else if (horarioOcupado(nueva.fecha, horaBuffer, NULL)) {
            printf("Ya existe una cita registrada en esa fecha y hora.\n");
            valido = 0;
        } else {
            valido = 1;
        }
    } while (!valido);
    strcpy(nueva.hora, horaBuffer);

    printf("Ingrese el nombre del medico: ");
    leerLinea(nueva.medico, LEN_MEDICO);

    citas[totalCitas] = nueva;
    totalCitas++;

    printf("\nCita registrada exitosamente con el codigo %s.\n", nueva.codigo_cita);
}

/* 
 * listarCitas:
   Muestra todas las citas registradas en formato de tabla.
*/
void listarCitas(void) {
    int i;

    printf("\n===== LISTADO DE CITAS =====\n");

    if (totalCitas == 0) {
        printf("No hay citas registradas.\n");
        return;
    }

    printf("%-15s %-25s %-20s %-12s %-8s %-20s\n",
           "CODIGO", "PACIENTE", "ESPECIALIDAD", "FECHA", "HORA", "MEDICO");
    printf("-----------------------------------------------------------------------------------------------------\n");

    for (i = 0; i < totalCitas; i++) {
        printf("%-15s %-25s %-20s %-12s %-8s %-20s\n",
               citas[i].codigo_cita,
               citas[i].nombre_paciente,
               citas[i].especialidad,
               citas[i].fecha,
               citas[i].hora,
               citas[i].medico);
    }
}

/* 
 * buscarCita:
   Permite buscar una cita por codigo exacto o por nombre del
   paciente (coincidencia parcial, sin distinguir mayusculas).
*/
void buscarCita(void) {
    int opcion;
    char buffer[LEN_NOMBRE];
    char opcionStr[10];

    printf("\n===== BUSCAR CITA =====\n");
    printf("1. Buscar por codigo exacto\n");
    printf("2. Buscar por nombre del paciente\n");
    printf("Seleccione una opcion: ");
    leerLinea(opcionStr, sizeof(opcionStr));
    opcion = atoi(opcionStr);

    switch (opcion) {
        case 1: {
            int indice;
            printf("Ingrese el codigo de la cita: ");
            leerLinea(buffer, LEN_CODIGO);
            indice = buscarIndice(buffer);
            if (indice == -1) {
                printf("\nNo se encontro ninguna cita con el codigo %s.\n", buffer);
            } else {
                printf("\n----- CITA ENCONTRADA -----\n");
                printf("Codigo: %s\n", citas[indice].codigo_cita);
                printf("Paciente: %s\n", citas[indice].nombre_paciente);
                printf("Especialidad: %s\n", citas[indice].especialidad);
                printf("Fecha: %s\n", citas[indice].fecha);
                printf("Hora: %s\n", citas[indice].hora);
                printf("Medico: %s\n", citas[indice].medico);
            }
            break;
        }
        case 2: {
            int encontrados = 0;
            int i;
            printf("Ingrese el nombre o parte del nombre del paciente: ");
            leerLinea(buffer, LEN_NOMBRE);
            printf("\n----- RESULTADOS DE LA BUSQUEDA -----\n");
            for (i = 0; i < totalCitas; i++) {
                if (contieneSubcadena(citas[i].nombre_paciente, buffer)) {
                    printf("Codigo: %s | Paciente: %s | Especialidad: %s | Fecha: %s | Hora: %s | Medico: %s\n",
                           citas[i].codigo_cita, citas[i].nombre_paciente, citas[i].especialidad,
                           citas[i].fecha, citas[i].hora, citas[i].medico);
                    encontrados++;
                }
            }
            if (encontrados == 0) {
                printf("No se encontraron pacientes que coincidan con \"%s\".\n", buffer);
            }
            break;
        }
        default:
            printf("Opcion invalida.\n");
    }
}