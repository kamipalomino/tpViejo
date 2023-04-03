#include "include/memoria.h"
#include "estructurasMemoria.c"
#include "conexionesMemoria.c"
#include <commons/log.h>
#include <commons/collections/list.h>
#include "funciones_Kernel.c"
#include "funciones_Swap.c"
#include <biblio/include/main.h>
//#include <matelib.h>

void inicializar_configuracion(){
	config_memoria=malloc(sizeof(t_config_memoria));
	config_memoria->ip=config_get_string_value(configMemoria, "IP");
		log_info(logger, config_memoria->ip);

		config_memoria->puerto_swap=string_duplicate(config_get_string_value(configMemoria, "PUERTO_SWAP"));
		log_info(logger, config_memoria->puerto_swap);

		config_memoria->puerto_memo=string_duplicate(config_get_string_value(configMemoria, "PUERTO_MEMORIA"));
		log_info(logger, config_memoria->puerto_memo);

		config_memoria->tamanio=config_get_int_value(configMemoria, "TAMANIO");
		log_info(logger, "%d",config_memoria->tamanio);

		config_memoria->tamanio_pag=config_get_int_value(configMemoria, "TAMANIO_PAG");
		log_info(logger, "%d",config_memoria->tamanio_pag);


		config_memoria->tipo_asignacion=string_duplicate(config_get_string_value(configMemoria, "TIPO_ASIGNACION"));
		log_info(logger, config_memoria->tipo_asignacion);

		config_memoria->marcos_maximos=config_get_int_value(configMemoria, "MARCOS_POR_CARPINCHO");
		log_info(logger, "%d",config_memoria->marcos_maximos);

		config_memoria->alg_reemplazo_memoria=string_duplicate(config_get_string_value(configMemoria, "ALGORITMO_REEMPLAZO_MMU"));
		log_info(logger, config_memoria->alg_reemplazo_memoria);

		config_memoria->cant_entradas_tlb=config_get_int_value(configMemoria, "CANTIDAD_ENTRADAS_TLB");
		log_info(logger, "%d",config_memoria->cant_entradas_tlb);

		config_memoria->alg_reemplazo_tlb=string_duplicate(config_get_string_value(configMemoria, "ALGORITMO_REEMPLAZO_TLB"));
		log_info(logger, config_memoria->alg_reemplazo_tlb);

		config_memoria->retardo_acierto=config_get_int_value(configMemoria, "RETARDO_ACIERTO_TLB");
		log_info(logger, "%d",config_memoria->retardo_acierto);


		config_memoria->retardo_fallo=config_get_int_value(configMemoria, "RETARDO_FALLO_TLB");
		log_info(logger, "%d",config_memoria->retardo_fallo);

		if(string_equals_ignore_case(config_memoria->tipo_asignacion,"FIJA")){
			asignacion = ASIGNACION_FIJA;
		}
		else{
			asignacion = ASIGNACION_GLOBAL;
		}
}


void inicializar_memoria(){
	tlb_miss_totales=0;
	tlb_hit_totales=0;
	lista_procesos = list_create();
	memoria_size = config_memoria->tamanio;
	memoria_principal = malloc(memoria_size);
	

	config_memoria->cant_bytes_libres = config_memoria->tamanio;
	//int cant_frames = config_memoria->tamanio / config_memoria->tamanio_pag;
	frames_memoria = list_create();
	lista_de_heaps = list_create();
	t_heap_nuestro* primer_heap = malloc(sizeof(t_heap_nuestro));
	primer_heap->heap = malloc(sizeof(heap_metadata));
	int indice = 0 ;


	primer_heap->heap->prev_alloc =(int) NULL;
	primer_heap->heap->next_alloc =(int) NULL;
	primer_heap->heap->is_free = true;
	primer_heap->size = 0;

	list_add_in_index(lista_de_heaps,indice,primer_heap);

	config_memoria->cant_frames = config_memoria->tamanio / config_memoria->tamanio_pag;


	for(int i=0 ; i < config_memoria->cant_frames;i++){
		marco = malloc(sizeof(t_mem_frame));
		marco->pagina = -1;
		marco->pid = -1;
		marco->libre = 1;

		list_add_in_index(frames_memoria,i,marco);
	}
	free(primer_heap->heap);
	free(primer_heap);

	log_info(logger,"Memoria Lista\n ");
}

void liberar_heap_nuestro(t_heap_nuestro* un_heap){
	free(un_heap->heap);
	free(un_heap);
}

void inicializar_lista_heap(){
	t_heap_nuestro* heap = malloc(sizeof(t_heap_nuestro));
	heap->heap->is_free = true;
	heap->heap->next_alloc = NULL;
	heap->heap->prev_alloc = NULL;
	heap->size = 0;
	list_add_in_index(lista_de_heaps,0,heap);
    liberar_heap_nuestro(heap);
}

void inicializar_tlb(){
	lista_tlb=list_create();
	for (int i = 0; i <= config_memoria->cant_entradas_tlb; i++)
	{
		tlb=malloc(sizeof(t_tlb));
		tlb->frame=i;
		tlb->pagina=-1;
		tlb->pid=-1;
		tlb->referenceBit=-1;
		tlb->time=-1;

		list_add_in_index(lista_tlb,i,tlb);
	}
	
		log_info(logger,"TLB Lista\n ");
}

void inicializar_tablas_paginas(int pags,t_carpincho* proceso){
	int i;

	for(i=0;i<pags;i++){
		t_tabla_pagina* entrada = malloc(sizeof(t_tabla_pagina));
		entrada->presentBit = 0;
		entrada->accessedBit = 0;
		entrada->dirtyBit = 0;
		entrada->frame = -1;
		entrada->page=i;

		list_add_in_index(proceso->tabla_paginas,i,entrada);
	}
	log_info(logger,"paginas Listas\n ");
}
void inicializar_marcos_carpincho(t_carpincho* proceso){
	for (size_t i = 0; i < config_memoria->marcos_maximos; i++)
	{
		list_add_in_index(proceso->lista_marcos,i,-1);
	}
	if(list_size(proceso->lista_marcos)==config_memoria->marcos_maximos){
		log_trace(logger, "inicializacion de carpincho correcta");
	}else
	{
		log_error(logger,"algo falló");
	}
	
}

t_carpincho* inicializar_proceso(int pid,int frames){
	t_carpincho* proceso = buscar_carpincho(pid);

	if(proceso){
	 log_error(logger,"Ya existe un proceso con ese PID");
		pid=-1;//return EXIT_FAILURE;
	}

	proceso = malloc(sizeof(t_carpincho));
	proceso->pid = pid;
	proceso->tabla_paginas = list_create();
	proceso->lista_marcos = list_create();
	proceso->hit=25;
	proceso->miss=30;
	proceso->frame_libres=config_memoria->marcos_maximos;
	

	inicializar_tablas_paginas(config_memoria->cant_paginas_maximas,proceso);
	inicializar_marcos_carpincho(proceso);
	
	proceso->iterar_marco=list_iterator_create(proceso->lista_marcos);
	//list_add(lista_procesos,proceso);

	return proceso;
}
