#ifndef MATE_INIT_H_
#define MATE_INIT_H_

#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <readline/readline.h>


#include <biblio/include/sockets.h>
#include <biblio/include/utils.h>
#include <biblio/include/id_unico.h>


typedef struct {
    // Backend porque puede ser el kernel o memoria
    char* IP_BACKEND;
    uint16_t PUERTO_BACKEND;
} t_config_matelib;


t_config_matelib* MATELIB_CFG;
t_log* main_log;
t_config_matelib* initialize_cfg();

uint8_t cargar_configuracion(t_config_matelib* config, char* config_path);

uint8_t generar_conexiones(int* fd_memoria, t_config_matelib* cfg);

#endif
