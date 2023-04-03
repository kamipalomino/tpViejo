#include "include/main.h"
#include "include/init_kernel.h"
#include <commons/collections/queue.h>

//t_log* main_log;
//t_config_kernel* KERNEL_CFG;
//t_queue* COLA_NEW;
//t_queue* COLA_READY;
//t_queue* COLA_EXIT;

static t_config_kernel* initialize_cfg() {
    t_config_kernel* cfg = malloc(sizeof(t_config_kernel));
    cfg->ALGORITMO_PLANIFICACION = NULL;
    cfg->IP_MEMORIA = NULL;
	cfg->DURACIONES_IO = NULL;
	cfg->DISPOSITIVOS_IO = NULL;
    return cfg;
}

int main() {

	main_log = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_DEBUG);
	KERNEL_CFG = initialize_cfg();
	COLA_NEW = queue_create();
	COLA_READY = queue_create();
	COLA_EXIT = queue_create();

	int fd_memoria = 0; //file descriptor
	if(!cargar_configuracion(KERNEL_CFG) || !generar_conexiones(&fd_memoria, KERNEL_CFG)) {
		cerrar_programa(main_log, KERNEL_CFG);
		return EXIT_FAILURE;
	}

	// TODO: escuchar matelib
	// while (server_escuchar("MATELIB", server_fd));
	
	// do shit 
	log_debug(main_log, "doing shit");
	
	cerrar_programa(main_log, KERNEL_CFG);
	return EXIT_SUCCESS;
}
