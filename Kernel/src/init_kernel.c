#include "../include/init_kernel.h"
#include <biblio/include/sockets.h>
#include "../include/log.h"

char* KERNEL_CONFIG_PATH = "kernel.config";

uint8_t cargar_configuracion(t_config_kernel* config) {
    t_config* cfg = config_create(KERNEL_CONFIG_PATH);

    if(cfg == NULL) {
        log_error(main_log, "No se encontro kernel.config");
        return 0;
    }

    char* properties[] = {
        "IP_MEMORIA",
        "PUERTO_MEMORIA",
        "ALGORITMO_PLANIFICACION",
        "ESTIMACION_INICIAL",
        "ALFA",
        // "RETARDO_CPU",
        "GRADO_MULTIPROCESAMIENTO",
        "GRADO_MULTIPROGRAMACION",
        "DISPOSITIVOS_IO",
        "DURACIONES_IO",
        NULL
    };

    // Falta alguna propiedad
    if(!config_has_all_properties(cfg, properties)) {
        log_error(main_log, "Propiedades faltantes en el archivo de configuracion");
        config_destroy(cfg);
        return 0;
    }

    config->IP_MEMORIA = strdup(config_get_string_value(cfg, "IP_MEMORIA"));
    config->PUERTO_MEMORIA = config_get_int_value(cfg, "PUERTO_MEMORIA");
    config->ALGORITMO_PLANIFICACION = strdup(config_get_string_value(cfg, "ALGORITMO_PLANIFICACION"));
    config->ESTIMACION_INICIAL = config_get_int_value(cfg, "ESTIMACION_INICIAL");
    config->ALFA = config_get_int_value(cfg, "ALFA");
    // config->RETARDO_CPU = config_get_int_value(cfg, "RETARDO_CPU");
    config->GRADO_MULTIPROCESAMIENTO = config_get_int_value(cfg, "GRADO_MULTIPROCESAMIENTO");
    config->GRADO_MULTIPROGRAMACION = config_get_int_value(cfg, "GRADO_MULTIPROGRAMACION");
    
    char** dispositivos_io = config_get_array_value(cfg, "DISPOSITIVOS_IO");
    config->DISPOSITIVOS_IO = extraer_valores(dispositivos_io);
    config_free_array_value(&dispositivos_io);

    char** duraciones_io = config_get_array_value(cfg, "DURACIONES_IO");
    config->DURACIONES_IO = extraer_valores(duraciones_io);
    config_free_array_value(&duraciones_io);

    log_info(main_log, "Archivo de configuracion cargado correctamente");

    config_destroy(cfg);

    return true;
}

uint8_t generar_conexiones(int* fd_memoria, t_config_kernel* cfg) {
    char* port_memoria = string_itoa(cfg->PUERTO_MEMORIA);

    *fd_memoria = crear_conexion(cfg->IP_MEMORIA,port_memoria);
    //crear_conexion2(main_log, "MEMORIA", cfg->IP_MEMORIA, port_memoria);
    
    free(port_memoria);

    return *fd_memoria != 0;
}

void cerrar_programa(t_log* main_log, t_config_kernel* cfg) {
    log_destroy(main_log);

    queue_destroy(COLA_NEW);
    queue_destroy(COLA_READY);
    queue_destroy(COLA_EXIT);

    free(cfg->IP_MEMORIA);
    free(cfg->ALGORITMO_PLANIFICACION);
    free(cfg->DISPOSITIVOS_IO);
    free(cfg->DURACIONES_IO);
    free(cfg);

    rl_clear_history();
}
