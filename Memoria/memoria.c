#include "include/memoria.h"
//#include "estructurasMemoria.c"
//#include "conexionesMemoria.c"
#include "inicializar.c"
#include "finalizar.c"
#include <commons/log.h>
//#include "funciones_Kernel.c"
//#include "funciones_Swap.c"
#include <biblio/include/main.h>
//#include <matelib.h>


int main(void)
{
	int conexion;
	startSigHandlers();
	//char* valor;

	logger = log_create("memoria1.log","Memoria",log_info,0);

//	configMemoria = config_create("configuracionMEMORIA.config");

	configMemoria = config_create("simple.config");

	inicializar_configuracion();
	inicializar_memoria();
	inicializar_tlb();
	//inicializar_lista_heap();
	//log_error(logger, "heap -> %d",sizeof(heap_metadata));
	lista_de_heaps = list_create();

//	prueba();
	pthread_create(&hiloSwap, NULL, (void*) conectarSwap, NULL);
	pthread_join(hiloSwap, NULL);


	pthread_create(&hiloConexion, NULL, (void*) conexionesMemoria, NULL);
	pthread_join(hiloConexion,NULL);


	log_info(logger,"antes de terminar programa");
	terminar_programa(conexion, logger, configMemoria);
}


void paquete(int conexion){
	char* stream = string_itoa(ASIGNACION_FIJA);
	t_paquete* paquete = crear_paquete(2,sizeof(stream),stream);

	log_info(logger,"%d",atoi(paquete->buffer->stream));

	enviar_mensaje(conexion,paquete);
	log_info(logger,"pase enviar mensaje");

	eliminar_paquete(paquete);
	free(stream);
}
void paqueteConInfo(int conexion, int codigo, char* info){
	char* stream = string_new();
	t_paquete* paquete = crear_paquete(codigo,strlen(info)+1,info);

	log_info(logger,"entrePaqueteInfo");

	log_info(logger,"%d",paquete->codigo_operacion);

	log_info(logger,"tamanio buffer");

	log_info(logger,"%d",paquete->buffer->size);
	enviar_mensaje(conexion,paquete);
	log_info(logger,"pase enviar mensaje");

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
	free(stream);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/*LEAKS*/
	liberar_memoria();
	liberar_tlb();
	liberar_conexion(conexion);
	log_destroy(logger);
	liberar_t_config();
	config_destroy(configMemoria);
	printf("----------FIN------------\n");
}

///-------------------------------------------------------///




/*
mate_pointer mate_memalloc(mate_instance *lib_ref, int size);

int mate_memfree(mate_instance *lib_ref, mate_pointer addr);

int mate_memread(mate_instance *lib_ref, mate_pointer origin, void *dest, int size);

int mate_memwrite(mate_instance *lib_ref, void *origin, mate_pointer dest, int size);

*/




/*
int cant_paginas_segun_alloc(int tamanio_alloc_carpincho){
	// direc_logica = nro_de_pag * tam_pagina + offset;
	int cant_paginas =	;
}
*/


// ****CAMI DEL FUTURO: EN EL PRIMER HEAP TENGO LA DIRECCION DEL SEGUNDO HEAP EN EL NEXT
//EL SEGUNDO HEAP ESTA TODO VACIO Y EN NULL (-1),
//DESPUES DEL PRIMER ALLOC SE REPITE TODO PARA LA LISTA****

//
int32_t first_fit(int pid, int tamanio_a_alocar, void* info){
	t_heap_nuestro* heap = buscar_heap(1); //1 es que esta libre, 0 ocupado
	int32_t dir;
	if(heap == NULL && list_is_empty(lista_de_heaps)){
       primer_alocar_memoria(info);
       heap = crear_heap_nuestro(pid,tamanio_a_alocar,-1,sizeof(heap_metadata)+tamanio_a_alocar,false);
       list_add(lista_de_heaps,heap);
       log_trace(logger,"despues de list add");
       dir = calcular_direccion_logica(heap->heap->prev_alloc);
       liberar_heap_nuestro(heap);
       heap = crear_heap_nuestro(-1,0,0,-1,true);
       list_add(lista_de_heaps,heap);
       liberar_heap_nuestro(heap);
       return dir;
	}else if(heap->size >= tamanio_a_alocar){
		//alocar_memoria(heap,)
		// ACA HAY QUE MODIFICAR EL HEAP POR LOS DATOS NUEVOS
		return dir;
	}
//	int size = 0;
//	if(heap->heap->prev_alloc ==(int)NULL){
//		size = heap->heap->next_alloc - 9;
//	}else{
//		size = heap->heap->next_alloc - 18 - heap->heap->prev_alloc - heap->size;
//	}
//	if(size >= tamanio_a_alocar){
//			alocar_memoria(tamanio_a_alocar,info);
//		}
}




/*
t_carpincho* buscar_carpincho(int pid){
	t_carpincho* proceso = NULL;
	bool buscar_proc(t_carpincho* p){
		return (p->pid == pid);
	}

	proceso = list_find(lista_procesos,(void*)buscar_proc);

	return proceso;
}
*/




void startSigHandlers(void) {
	signal(SIGINT, sigHandler_sigint);
}

void sigHandler_sigint(int signo) {
	log_warning(logger, "Tiraste un CTRL+C, macho, abortaste el proceso.");
	metricas();
	log_debug(logger,"metricas");
	terminar_programa(conexion, logger, configMemoria);
	printf("-------------------FINAL POR CTRL+C-------------------");

	exit(-1);
}

