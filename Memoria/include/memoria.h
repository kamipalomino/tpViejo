#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <biblio/include/client.h>
#include <biblio/include/utils.h>
#include <biblio/include/main.h>
#include <biblio/include/serializacion.h>
#include <biblio/include/server.h>
#include <pthread.h>
#include <stdbool.h>

#define ONLINE	100	
#define PROCESO_EN_TLB	600
#define PROCESO_NO_TLB	601
#define PAGINA_EN_TLB	602
#define PAGINA_NO_TLB	603
#define PROCESO_EN_MEMO	604
#define PROCESO_NO_MEMO	605
#define PAGINA_EN_TP	606
#define PAGINA_NO_TP	607

typedef struct{
    char* ip;
    char* puerto_swap;
    char* puerto_memo;
	 char* dump;
    int tamanio;
    int tamanio_pag;
    char* tipo_asignacion;
    char* alg_reemplazo_memoria;
    int cant_entradas_tlb;
    char* alg_reemplazo_tlb;
    int retardo_acierto;
    int retardo_fallo;
    int cant_bytes_libres;
    int marcos_maximos;
	int cant_frames;
	int cant_paginas_maximas; //es el tamanio maximo del carpincho
} t_config_memoria;

t_config_memoria* config_memoria;

typedef struct {
	int pagina;
	int pid;
	int libre;
} t_mem_frame;

t_mem_frame* marco;

typedef struct {
	int page;
	int frame;
	int presentBit;
	int accessedBit; // bit de uso
	int dirtyBit; // bit de modif
} t_tabla_pagina;

typedef struct {
	int pagina;
	int frame;
	int pid;
	int referenceBit;
	int time;
} t_tlb;

t_tlb* tlb;

typedef struct{
	uint32_t prev_alloc;
	uint32_t next_alloc;
	uint8_t is_free;
}__attribute__((packed))
heap_metadata;

typedef struct{
	heap_metadata* heap;
	int size;
	int pid;
}t_heap_nuestro;


char* ip;
char* puertoSwap;

t_log* logger;
t_config* configMemoria;
pthread_t hiloConexion;
pthread_t hiloSwap;

t_list* frames_memoria;	//lista de marcos 
t_list* lista_procesos;///lista de carpichos
t_list* lista_tlb;
t_list* tabla_tlb;
t_list* lista_de_heaps;
int memoria_size, server_memo;
int cliente_memo;
int memoria_size, server_memo;
int cliente_memo;
int cliente_memoria[ONLINE];
int conexion;
int online;
int tlb_hit_totales;
int tlb_miss_totales;
int asignacion;

void* memoria_principal;

void inicializar_configuracion();
void inicializar_memoria();
void inicializar_lista_heap();
void inicializar_tablas_paginas(int pags,t_carpincho* proceso);
void inicializa_tlb();
void inicializar_marcos_carpincho(t_carpincho* carp);

void conexionesMemoria();
void paqueteConInfo(int conexion, int codigo, char* info);
void conectarSwap();

void paqueteConInfo(int conexion, int codigo, char* info);

void liberar_heap_nuestro(t_heap_nuestro* un_heap);
void liberar_memoria();
void liberar_tlb();
void liberar_t_config();
void liberar_proceso(t_carpincho* p);
void liberar_tabla_paginas(t_tabla_pagina* pag);

void imprimirMiss(t_carpincho* p);
void imprimirHit(t_carpincho* p);
void sigHandler_sigint(int signo);
void startSigHandlers(void);
void metricas();

void actualizar_estructuras(t_carpincho* un_carpincho,int32_t dir);
void ejecutarKernel(int c);
void* mem_alloc(t_paquete* p, int cliente);
void alocar_memoria(int inicio,void* info);
void primer_alocar_memoria(void* info);

int32_t first_fit(int pid, int tamanio_a_alocar, void* info);

void prueba();
//void liberar_tlb();
//void liberar_tabla();
void sumarMiss(int pid);
void sumarHit(int pid);
//void terminar_programa(int conexion, t_log* logger, t_config* config);

int pedir_pagina_memo(int32_t pid, int32_t dir);
int borrar_pagina_memo(int32_t pid, int32_t dir);
int escribir_pagina_memo(int32_t pid, int32_t dir, void* info);
int leer_pagina_memo(int32_t pid, int32_t dir);

int esta_cargada_marco(int pid, int pag);
int obtener_pag_dir_logica(int32_t dir);
int buscar_pagina_tlb(int32_t pid, int pag);
int buscar_proceso_tlb(int pid);
int calcular_marco(int32_t direccion);
// proceso -> dividido en páginas
// memoria -> dividida en marcos

int32_t calcular_direccion_logica(uint32_t prev);

heap_metadata* crear_heap(uint8_t is_free, uint32_t prev, uint32_t next);
t_heap_nuestro* crear_heap_nuestro(int32_t pid, int32_t size, uint32_t prev, uint32_t next, uint8_t is_free);
t_carpincho* buscar_carpincho(int pid);
t_carpincho* inicializar_proceso(int pid,int paginas);
t_list* deserializar(t_paquete* buffer);
#endif /* MEMORIA_MAIN_H */
