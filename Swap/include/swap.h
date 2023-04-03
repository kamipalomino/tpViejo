#ifndef SWAMP_MAIN_H
#define SWAMP_MAIN_H

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <biblio/include/server.h>
#include <biblio/include/main.h>
#include <biblio/include/serializacion.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <sys/stat.h>

typedef struct
{
    char *ip;
    char *puerto_swap;
    int32_t tamanio_swap;
    int32_t tamanio_pag;
    char *tipo_asignacion;
    int asignacion;
    int32_t cant_entradas_tlb;
    int32_t retardo_swap;
    int32_t marco_carpincho;
    char **archivos;
    int32_t cant_marcos;
    int32_t cant_paginas;
} t_config_swap;

typedef struct
{
    int32_t pid;
    int32_t marco;
    int32_t pagina;
    int32_t bit_presencia;
    int32_t bit_modificado;
    int32_t marcos_libres;
    int32_t time;
    t_list* tabla_de_pag;
    char* path;
} estructura_carpincho;

typedef struct 
{
    int32_t nro_marco;
    int32_t time;
    int32_t modificado;
    int32_t presente;

}t_pages;


estructura_carpincho *carp_global;
pthread_t hiloConexion;
t_config_swap *config_swap;
t_list *tabla_de_marcos_fija;
t_list *tabla_de_marcos_global;
t_list *lista;
t_log *logger;
t_config *configSwamp;
t_bitarray* bitarray[10];
FILE *file;
int32_t server_fd, cliente_fd;
void** archivos_mapeados;
int online, running;
int fd;
int obtener_marco(int pid, int pag);
int obtener_offset(uint32_t dir);
int escribir_bytes(int pid, int pagina, void * datos_a_escribir);
char* obtener_archivo(int pid, int pag);
void iniciar_archivos();
void leer_en_memoria();
void conexionesSwamp();
void conexionesSwamp2();
void terminarSwap(int conexion);
void inicializar_configuracion();
void inicializar_swap();
void inicializar_asignacion(int x);
void inicializar_bitarray();
void inicializar_asignacion_fija();
void inicializar_asignacion_global();
void leer_pagina(t_paquete *proceso);
void escribir_pagina(t_paquete *proceso);
void borrar_pagina(t_paquete *proceso);
void pedir_pagina(t_paquete *proceso);
void sigHandler_sigint_Sw(int signo);
void startSigHandlers_Sw(void);
void eliminar_carpincho(estructura_carpincho *carp);
void liberar_configuracion();
void liberar_tabla_de_marcos_fija();
void liberar_bitarray();
void pruebaArchivo();
void buscar_carpincho_borrar_pag(int32_t pid, int32_t pag);
void asignar_marcos(t_paquete* p);

estructura_carpincho* crear_carpincho_estructura(int32_t pid, int32_t modificado, int32_t presencia, int32_t marco, int libres, int32_t numero_pag,char* path,int32_t tim, t_list* tabla );
#endif /* SWAMP_MAIN_H */
