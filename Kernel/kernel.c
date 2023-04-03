#include "include/kernel.h"
#include <biblio/include/utils.h>
#include <biblio/include/main.h>
#include <biblio/include/serializacion.h>
#include <commons/memory.h>
int main(){



	logger = log_create("kernel.log","Kernel",log_info,0);
	//configMemoria = config_create("configuracionKERNEL.config");


	//ip = config_get_string_value(configKernel, "IP");
	//puertoMemoria = config_get_string_value(configKernel, "PUERTO");

	pthread_create(&hiloMemoria, NULL, (void*) conectarMemoria, NULL);
	pthread_join(hiloMemoria, NULL);


	//pthread_create(&hiloConexion, NULL, (void*) conexionesMatelib, NULL);
	//pthread_join(hiloConexion,NULL);




	//pthread_create(&hiloMemoria, NULL, (void*) conectarMemoria, conexion);
	//pthread_detach(hiloMemoria);
	conectarMemoria(conexion);

	log_info(logger,"antes de terminar programa");
	terminar_kernel(conexion, logger, configKernel);
    
return 0;
}
void conectarMemoria(){
	puertoM = string_new("5002");
	int conexion = crear_conexion("127.0.0.1", "5002");
	log_info(logger,"%d",conexion);
	char* stream = string_new();
	t_paquete* paquete = crear_paquete_carpincho(KERNEL,sizeof(stream),stream,NULL,NULL);


	log_info(logger,"%d",paquete->codigo_operacion);

	enviar_mensaje_carpincho(conexion,paquete);
	log_info(logger,"%d",conexion);
	log_info(logger,"pase enviar mensaje");

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
	//free(stream);

	paquete = recibir_mensaje_carpincho(conexion);
	log_trace(logger,"el codigo de operacione es: ");
	log_trace(logger,"%d",paquete->codigo_operacion);
	eliminar_paquete(paquete);

	stream=string_duplicate("separarse de la especie por algo superior");


	enviar_mem_alloc(conexion,101,42,stream);
	free(stream);
	log_warning(logger,"Envie pid 101");
	log_error(logger,"%s",mem_hexstring(paquete->buffer->stream,42));

	paquete = recibir_mem_alloc(conexion);

}

void conexionesMatelib(){
	char* puerto = string_new("5003");
	int server_fd = iniciar_servidor(ip, puerto);
	int cliente_fd;

	log_info(logger, "iniciando servidor Kernel");

	log_info(logger, "Esperando nueva conexion..");
	cliente_fd = esperar_cliente(server_fd);

	int running = 1;

		t_paquete* paquete = malloc(sizeof(t_paquete));

		while(running){
		paquete = recibir_mensaje(cliente_fd);
		int codigo = paquete->codigo_operacion;

			switch (codigo) {
			case MATELIB:
				log_info(logger, "Se conectó Matelib\n");
				running=-1;
				pthread_exit(&hiloConexion);
				break;
			default:
				log_warning(logger,
						"Operacion desconocida. No quieras meter la pata");
						pthread_detach(hiloConexion);
						pthread_exit(&hiloConexion);
						liberar_conexion(cliente_fd);
				break;

				}
			free(paquete->buffer->stream);
			free(paquete->buffer);
			free(paquete);
			}
		free(puerto);
}

void terminar_kernel(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
	log_destroy(logger);
	free(ip);
	free(puertoM);
	//config_destroy(configKernel);
	printf("----------FIN------------\n");
}

