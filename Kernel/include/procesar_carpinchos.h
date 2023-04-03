#ifndef PROCESAR_CARPINCHOS_H_
#define PROCESAR_CARPINCHOS_H_

#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <commons/config.h>
#include <commons/collections/queue.h>

#include <biblio/include/utils.h>
#include <biblio/include/sockets.h>
#include <biblio/include/protocolo.h>
#include  <biblio/include/main.h>
#include "config.h"
#include "log.h"
#include "init_kernel.h"

typedef struct {
    pcb_carpincho* carpincho;
    int estimacion_actual;
} tupla_sjf;

typedef struct {
    int fd;
    char* server_name;
} t_procesar_conexion_args;

int generar_pid();
pcb_carpincho* generar_estructuras();
void planificar_nuevo_cpncho_FIFO(pcb_carpincho*);

//extern t_queue* COLA_NEW;
//extern t_queue* COLA_READY;
//extern t_queue* COLA_EXIT;
t_config_kernel* KERNEL_CFG;


void push_cola_new(pcb_carpincho*);
pcb_carpincho* pop_cola_new();
// void* remover_cola_new(uint32_t tid);

void push_cola_ready(pcb_carpincho* cpncho);
void procesar_conexion(void* void_args);
int rcv_mate_init2(int fd);
int server_escuchar(const char* server_name, int server_socket);
int calcular_rafaga(pcb_carpincho* carpincho);
void planificar_ejecucion_SJF(char* algoritmo, int grado_multiprocesamiento, int est_init, int alfa);
void planificar_nuevo_cpncho_FIFO(pcb_carpincho* carpinchoNuevo);
void planificar_nuevo_cpncho_FIFO(pcb_carpincho* carpinchoNuevo);
void push_cola_ready(pcb_carpincho* cpncho);

#endif
