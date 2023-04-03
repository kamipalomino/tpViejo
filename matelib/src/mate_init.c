#include "../include/mate_init.h"
#include <biblio/include/utils.h>
t_config_matelib* initialize_cfg() {
  t_config_matelib* cfg = malloc(sizeof(t_config_matelib));

  cfg->IP_BACKEND = "";
  cfg->PUERTO_BACKEND = (uint16_t)0;
  return cfg;
}

uint8_t cargar_configuracion(t_config_matelib* config, char* path) {
    t_config* cfg = config_create(path);

    if(cfg == NULL) {
        log_error(main_log, "No se encontro el archivo");
        return 0;
    }

    char* properties[] = {
        "IP_BACKEND",
        "PUERTO_BACKEND",
        NULL
    };

    // Falta alguna propiedad
    if(!config_has_all_properties(cfg, properties)) {
        log_error(main_log, "Propiedades faltantes en el archivo de configuracion");
        config_destroy(cfg);
        return 0;
    }

    config->IP_BACKEND = strdup(config_get_string_value(cfg, "IP_BACKEND"));
    config->PUERTO_BACKEND = config_get_int_value(cfg, "PUERTO_BACKEND");

    log_info(main_log, "Archivo de configuracion cargado correctamente");

    config_destroy(cfg);

    return true;
}

uint8_t generar_conexiones(int* fd_backend, t_config_matelib* cfg) {
    char* port_backend = string_itoa(cfg->PUERTO_BACKEND);

    //*fd_backend = crear_conexion2(main_log,"BACKEND",cfg->IP_BACKEND, port_backend);
    *fd_backend = crear_conexion(cfg->IP_BACKEND, port_backend);
    free(port_backend);

    return *fd_backend != 0;
}
