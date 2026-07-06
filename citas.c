
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