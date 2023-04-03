#include "include/memoria.h"
#include <biblio/include/main.h>
#include <commons/log.h>

void liberar_tlb(){
	list_destroy_and_destroy_elements(lista_tlb,(void*)free);
	log_info(logger,"Se liberó la TLB\n ");
}
void liberar_proceso(t_carpincho* p){
	list_destroy(p->lista_marcos);
	list_iterator_destroy(p->iterar_marco);
	list_destroy_and_destroy_elements(p->tabla_paginas,(void*)liberar_tabla_paginas);
	
	log_info(logger,"Se liberó la memoria del carpincho %d ",p->pid);
	free(p);
}
void liberar_tabla_paginas(t_tabla_pagina* pag){
	free(pag);
}
void liberar_memoria(){
	int cant_frames = config_memoria->tamanio / config_memoria->tamanio_pag;

	for (int i = 0; i < cant_frames; i++) {
		free(marco);
	}
	list_destroy(frames_memoria);
	list_destroy_and_destroy_elements(lista_procesos,(void*)liberar_proceso);
	free(memoria_principal);
	log_info(logger,"se liberó toda la memoria\n");
}
void liberar_t_config(){
	free(config_memoria->alg_reemplazo_memoria);
	free(config_memoria->alg_reemplazo_tlb);
	free(config_memoria->ip);
	free(config_memoria->puerto_swap);
	free(config_memoria->puerto_memo);
	free(config_memoria->tipo_asignacion);
	free(config_memoria->dump);
	free(config_memoria);
}


