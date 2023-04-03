#include "include/memoria.h"
#include <biblio/include/main.h>
#include <commons/memory.h>
void conectarSwap(){
	t_paquete* respuesta = malloc(sizeof(t_paquete));
	int conexion = crear_conexion(config_memoria->ip, config_memoria->puerto_swap);
	paquete(conexion);
	respuesta=recibir_mensaje(conexion);

	char* stream = string_new("");
	config_memoria->cant_paginas_maximas = atoi(respuesta->buffer->stream);
	log_debug(logger,"cantidad pags max: ");
	log_info(logger,"%d",config_memoria->cant_paginas_maximas);

	eliminar_paquete(respuesta);
	respuesta=crear_paquete_carpincho(LEER_PAGINA, sizeof(stream), stream, 995, 365);
	enviar_mensaje_carpincho(conexion,respuesta);
	eliminar_paquete(respuesta);
	free(stream);
	respuesta=recibir_mensaje_carpincho(conexion);

	stream=mem_hexstring(respuesta->buffer->stream,sizeof(respuesta->buffer->stream));
	
	//memcpy(stream, (respuesta->buffer->stream), sizeof(respuesta->buffer->stream));
	log_warning(logger,"La pagina decía %s",stream);
	log_warning(logger,"size %d",sizeof(stream));
	mem_hexdump(respuesta->buffer->stream,sizeof(respuesta->buffer->stream));
	free(stream);
	respuesta=crear_paquete_carpincho(FINALIZAR_SWAP, sizeof(stream), stream, 995, 365);
	enviar_mensaje_carpincho(conexion,respuesta);

	log_trace(logger,"Bye Swap");
	eliminar_paquete(respuesta);
	pthread_exit(&hiloSwap);
}


void conexionesMemoria(){
	int mid;

	server_memo = iniciar_servidor(config_memoria->ip, config_memoria->puerto_memo);
	int k = 0;

	log_info(logger, "iniciando servidor Memoria");

	log_info(logger, "Esperando nueva conexion..");
	cliente_memoria[k] = esperar_cliente(server_memo);

	int running = 1;

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete = recibir_mensaje_carpincho(cliente_memoria[k]);
	while(running){

		int codigo = paquete->codigo_operacion;

		switch (codigo) {
		case KERNEL:
			log_info(logger,"Se conectó el kernel: %d \n",cliente_memoria[k]);
			eliminar_paquete(paquete);
			ejecutarKernel(cliente_memoria[k]);
			//recibir_mensaje(cliente_memo);


			/* METER SEMÁFOROS	*/


			//running=-1;
			//pthread_exit(&hiloConexion);
			break;
		case MATELIB:
			
			mid = paquete->buffer->pid;
			log_info(logger, "Se conectó el carpincho: %d con la Matelib\n",mid);
			eliminar_paquete(paquete);
			ejecutarKernel(cliente_memoria[k]);
			break;
		default:
			log_warning(logger,
					"Operacion desconocida. No quieras meter la pata");
					pthread_detach(hiloConexion);
					pthread_exit(&hiloConexion);
					liberar_conexion(cliente_memo);
			break;

			}
		/*LEAKS*/
		k++;
		eliminar_paquete(paquete);
		cliente_memoria[k] = esperar_cliente(server_memo);
		paquete = recibir_mensaje(cliente_memoria[k]);
		}
}
