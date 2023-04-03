#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct {
    char* IP_MEMORIA;
    uint16_t PUERTO_MEMORIA;
    char* ALGORITMO_PLANIFICACION;
    uint16_t ESTIMACION_INICIAL;
    uint16_t ALFA;
    // uint16_t RETARDO_CPU;
    t_list* DISPOSITIVOS_IO; 
    t_list* DURACIONES_IO;
    uint16_t GRADO_MULTIPROGRAMACION;
    uint16_t GRADO_MULTIPROCESAMIENTO;
} t_config_kernel;

#endif
