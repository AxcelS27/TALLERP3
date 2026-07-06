/* ===========================================================
 * citas.h
 * Sistema de Agendamiento de Citas Medicas
 * ---------------------------------------------------------
 * Contiene: constantes globales, definicion del struct Cita,
 * variables extern y prototipos de todas las funciones que
 * implementa citas.c
 * =========================================================== */

#ifndef CITAS_H
#define CITAS_H

/* Constantes generales */
#define MAX_CITAS 100          /* Capacidad maxima de citas en el sistema      */
#define LEN_CODIGO 16          /* 15 caracteres utiles + caracter nulo         */
#define LEN_NOMBRE 100         /* Nombre del paciente                          */
#define LEN_ESPECIALIDAD 50    /* Especialidad medica                          */
#define LEN_FECHA 11           /* Formato DD/MM/AAAA (10 caracteres) + nulo    */
#define LEN_HORA 6             /* Formato HH:MM (5 caracteres) + nulo          */
#define LEN_MEDICO 50          /* Nombre del medico                            */
#define ARCHIVO_CSV "citas.csv"

/* Estructura principal */
typedef struct {
    char codigo_cita[LEN_CODIGO];
    char nombre_paciente[LEN_NOMBRE];
    char especialidad[LEN_ESPECIALIDAD];
    char fecha[LEN_FECHA];
    char hora[LEN_HORA];
    char medico[LEN_MEDICO];
} Cita;

/* Variables globales (definidas en citas.c)  */
extern Cita citas[MAX_CITAS];
extern int totalCitas;

/* Funciones de validacion */
int codigoExiste(const char *codigo);
int horarioOcupado(const char *fecha, const char *hora, const char *codigoExcluir);
int validarFecha(const char *fecha);
int validarHora(const char *hora);
int buscarIndice(const char *codigo);

/* Funciones de archivo*/
void guardarArchivo(void);
void cargarArchivo(void);

/*Funciones principales del sistema (menu)*/
void registrarCita(void);
void listarCitas(void);
void buscarCita(void);
void actualizarCita(void);
void eliminarCita(void);

/*Funciones utilitarias de entrada / cadenas */
void limpiarSaltoLinea(char *cadena);
int leerLinea(char *destino, int tamano);
int contieneSubcadena(const char *cadena, const char *subcadena);

#endif /* CITAS_H */
