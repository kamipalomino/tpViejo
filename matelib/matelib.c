#include "include/matelib.h"
#include "include/mate_init.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <biblio/include/serializacion.h>
#include <biblio/include/id_unico.h>
#include <biblio/include/main.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/temporal.h>
#include <time.h>
//------------------General Functions---------------------/
typedef t_dictionary mate_sem_dict;

typedef struct mate_inner_structure
{
  void *memory;
  mate_sem_dict *semaphores;
  // sem_t *sem_instance;
} mate_inner_structure;

int mate_init2(mate_instance *lib_ref, char *config)
{
//  main_log = log_create("matelib.log", "MATELIB", true, LOG_LEVEL_DEBUG);
//	MATELIB_CFG = initialize_cfg();
//
//  int fd_backend = 0;
//  if(!cargar_configuracion(MATELIB_CFG, config) || !generar_conexiones(&fd_backend, MATELIB_CFG)) {
//		//cerrar_programa(main_log, MATELIB_CFG);
//		return EXIT_FAILURE;
//	}
//
//  lib_ref->group_info = malloc(sizeof(mate_inner_structure));
//  lib_ref->mid = (int) time(NULL);// atoi(temporal_get_string_time());	//crear_id_unico(); // todo: generate mate id
//
  return 0;
}


int mate_init(mate_instance *lib_ref, char *config)
{
  //char* file=string_new("");
  lib_ref->cofig_mate=config_create(config);
  //lib_ref->ip= string_new("");
  lib_ref->ip=string_duplicate(config_get_string_value(lib_ref->cofig_mate, "IP"));
  lib_ref->puerto=string_duplicate(config_get_string_value(lib_ref->cofig_mate, "PUERTO"));
  lib_ref->path_log =string_duplicate(config_get_string_value(lib_ref->cofig_mate, "PATH_LOG"));
  lib_ref->logger=log_create(lib_ref->path_log, "Carpincho", 1, LOG_LEVEL_TRACE);
  lib_ref->mid = (int) time(NULL);//atoi(temporal_get_string_time());
  lib_ref->socket =conectarMatelib(lib_ref->mid,lib_ref->ip, lib_ref->puerto, lib_ref->logger);
  if (lib_ref->socket==ALGO_SALIO_MAL)
  {
    log_error(lib_ref->logger,"Error al conectar, terminando programaea");
    mate_close(lib_ref);
  }
  
  log_trace(lib_ref->logger,"Todo listo para ejecutar");
  return 0;
}

int mate_close(mate_instance *lib_ref)
{
	close(lib_ref->socket);
	config_destroy(lib_ref->cofig_mate);
	log_destroy(lib_ref->logger);
	free(lib_ref->ip);
	free(lib_ref->path_log);
	free(lib_ref->puerto);
	free(lib_ref->group_info);
	free(lib_ref);
	return 0;
}

//--------------Memory Module Functions-------------------/

mate_pointer mate_memalloc(mate_instance *lib_ref, int size)
{
	mate_pointer ptr;
	enviar_mem_alloc(lib_ref->socket, lib_ref->mid, size, lib_ref->group_info);
	  t_paquete* paquete =malloc(sizeof(t_paquete));
	  paquete=recibir_mem_alloc(lib_ref->socket);
      log_warning(lib_ref->logger,"direccion: %d", paquete->buffer->memoria_a_alocar);
	  //memcpy(x,paquete->buffer->stream,sizeof(paquete->buffer->stream));
	  ptr = paquete->buffer->memoria_a_alocar;

	  eliminar_paquete(paquete);

	  return ptr;
}

int mate_memfree(mate_instance *lib_ref, mate_pointer addr)
{
  if (addr != 0)
  {
    return -1;
  }
  //free(((mate_inner_structure *)lib_ref->group_info)->memory);
  return 0;
}

int mate_memread(mate_instance *lib_ref, mate_pointer origin, void *dest, int size)
{
  if (origin != 0)
  {
    return -1;
  }
  //memcpy(dest, ((mate_inner_structure *)lib_ref->group_info)->memory, size);
  return 0;
}

int mate_memwrite(mate_instance *lib_ref, void *origin, mate_pointer dest, int size)
{
  if (dest != 0)
  {
    return -1;
  }
 // memcpy(((mate_inner_structure *)lib_ref->group_info)->memory, origin, size);
  return 0;
}




