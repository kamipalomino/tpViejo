#include <biblio/include/server.h>
#include <biblio/include/main.h>
#include <biblio/include/serializacion.h>
#include "include/swap.h"
#include <pthread.h>
#include <fcntl.h>
#include <commons/memory.h>

void inicializar_configuracion(){
	config_swap=malloc(sizeof(t_config_swap));
	config_swap->ip=config_get_string_value(configSwamp, "IP");
		log_info(logger, config_swap->ip);

		config_swap->puerto_swap=string_duplicate(config_get_string_value(configSwamp, "PUERTO"));
		log_info(logger, config_swap->puerto_swap);

		config_swap->tamanio_swap=config_get_int_value(configSwamp, "TAMANIO_SWAP");
		log_info(logger, "%d",config_swap->tamanio_swap);

		config_swap->tamanio_pag =config_get_int_value(configSwamp, "TAMANIO_PAGINA");
		log_info(logger, "%d", config_swap->tamanio_pag);//64

		config_swap->archivos =config_get_array_value(configSwamp, "ARCHIVOS_SWAP");
		log_info(logger,"%s", config_swap->archivos[0]);

		config_swap->marco_carpincho=config_get_int_value(configSwamp, "MARCOS_MAXIMOS");
		log_info(logger, "%d",config_swap->marco_carpincho);

		config_swap->retardo_swap=config_get_int_value(configSwamp, "RETARDO_SWAP");
		log_info(logger, "%d",config_swap->retardo_swap);

		config_swap->cant_paginas = config_swap->tamanio_swap / config_swap->tamanio_pag;

		//log_trace(logger,"%d",config_swap->cant_paginas);

}
void iniciar_archivos(){
	int f =-2;
	int size =config_swap->tamanio_swap;
	char* s=string_repeat('\0', size);;
	int count = string_array_size(config_swap->archivos);
	for (size_t i = 0; i <= count; i++)
	{
		log_debug(logger, "%s",config_swap->archivos[i]);

		f = open(config_swap->archivos[i], O_CREAT, (mode_t) 0777);
		close(f);
		f = open(config_swap->archivos[i], 02, (mode_t) 0777);
		write(f,s,size);
		close(f);
		
	}
		free(s);
		pruebaArchivo();
}
void inicializar_swap(){
	iniciar_archivos();
	config_swap->cant_marcos = config_swap->tamanio_swap / config_swap->tamanio_pag;
	//inicializa_bitarray();
	
}
void inicializar_asignacion(int x){
	t_paquete* respuesta=malloc(sizeof(t_paquete));
	char* stream=string_new("");
	
	if(x == ASIGNACION_FIJA){
				config_swap->asignacion=ASIGNACION_FIJA;
				inicializar_asignacion_fija();
				log_info(logger,"La asignación es FIJA");
				stream = string_itoa(config_swap->cant_paginas);
				respuesta=crear_paquete(CANT_PAGS_MAXIMAS,sizeof(stream),stream);
				enviar_paquete(respuesta,cliente_fd);
				log_debug(logger,"Se respondio a Memoria %d",respuesta->codigo_operacion);
			}else if (x== ASIGNACION_GLOBAL){
				config_swap->asignacion=ASIGNACION_GLOBAL;
				inicializar_asignacion_global();
				log_info(logger,"La asignación es GLOBAL");
				stream = string_itoa(config_swap->cant_paginas);
				respuesta=crear_paquete(CANT_PAGS_MAXIMAS,sizeof(stream),stream);
				enviar_paquete(respuesta,cliente_fd);
				log_debug(logger,"Se respondio a Memoria %d",respuesta->codigo_operacion);
			}else {
				log_error(logger,"No se envió ninguna asignaciòn");
			}
	eliminar_paquete(respuesta);
	free(stream);
}
void inicializar_bitarray(){
   /// int x = list_size(config_swap->archivos);
    
    int i=0;
   // while(strlen(config_swap->archivos[i])!=0)
    while(i<4)
    {
        bitarray[i] =bitarray_create_with_mode(config_swap->archivos[i],config_swap->cant_marcos,MSB_FIRST);
        log_info("Iniciando bitarray: %d",bitarray_get_max_bit(bitarray[i]));
        i++;
    }    
    log_info("archivos: %d",i);
}