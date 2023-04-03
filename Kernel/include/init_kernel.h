#ifndef INIT_KERNEL_H_
#define INIT_KERNEL_H_

#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <commons/collections/queue.h>
#include <biblio/include/sockets.h>
#include <biblio/include/utils.h>
#include "log.h"
#include "config.h"

t_log* main_log;
t_config_kernel* KERNEL_CFG;

uint8_t cargar_configuracion(t_config_kernel*);
uint8_t generar_conexiones(int* fd_memoria, t_config_kernel* cfg);
void cerrar_programa(t_log* a, t_config_kernel*);

// extern
t_queue* COLA_NEW;
t_queue* COLA_READY;
t_queue* COLA_EXIT;

#endif
