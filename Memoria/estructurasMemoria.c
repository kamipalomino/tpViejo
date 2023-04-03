#include "include/memoria.h"
#include <biblio/include/main.h>
#include <commons/log.h>


void actualizar_estructuras(t_carpincho* un_carpincho, int32_t dir){

	int pagina = obtener_pag_dir_logica(dir);
	//tlb
	//tabla de pag
	//marco
	//memoria libre
	//frames_memoria

	list_add(lista_procesos,un_carpincho);
}
int buscar_pagina_tlb(int32_t pid, int pag){
	t_tlb* tlb1=NULL;
	
	if(buscar_proceso_tlb(pid)== PROCESO_EN_TLB){
		bool buscar_p(t_tlb* t){
		if(t->pid == pid && t->pagina ==pag){

			return(true);
		}else{
			return(false);
			}
		}
		tlb1=list_find(lista_tlb,(void*)buscar_p);	
		if(tlb1!=NULL){
			sumarHit(pid);
			return PAGINA_EN_TLB;
		}else {
			sumarMiss(pid);
			return PAGINA_NO_TLB;}
	}else{
		return PROCESO_NO_TLB;
	}
	
	
}
int buscar_proceso_tlb(int pid){
	t_tlb* tlb1=NULL;

	bool buscar_p(t_tlb* t){
		if(t->pid == pid){
			return(true);
		}
		return(false);
	}

	tlb1=list_find(lista_tlb,(void*)buscar_p);

	if(tlb1 == NULL){
		return PROCESO_NO_TLB;
	}else{

		return PROCESO_EN_TLB;
	}
	
}

int buscar_tabla_pagina(int pid, int pag){
	int p =-2;
	t_carpincho* carp=buscar_carpincho(pid);
	if(carp == NULL){
		return PROCESO_NO_MEMO;
	}else{
		t_tabla_pagina* tb=NULL;

		bool buscar_p(t_tabla_pagina* t){
		if(t->page == pag){
			return(true);
		}else{
			return(false);
			}
		}
		tb=list_find(carp->tabla_paginas,(void*)buscar_p);
		if(tb->presentBit ==1){
			sumarMiss(pid);
			p =tb->frame;
			liberar_tabla_paginas(tb);
			liberar_proceso(carp);
		return p;
	}else {
		liberar_tabla_paginas(tb);
		liberar_proceso(carp);
		return PAGINA_NO_TP;
		}

	}

}

int esta_cargada_marco(int pid, int pag){

	int info_tlb= buscar_pagina_tlb(pid, pag);
	int page;
	switch (info_tlb){
		case PAGINA_EN_TLB:
			return true;
		break;
		case PAGINA_NO_TLB:
			page =buscar_tabla_pagina(pid, pag);
			if(page==PROCESO_NO_MEMO){
				return PROCESO_NO_MEMO;
			}else{
				return page;
			}
		break;
		case PROCESO_NO_TLB:
			page =buscar_tabla_pagina(pid, pag);
			if(page==PROCESO_NO_MEMO){
				return PROCESO_NO_MEMO;
			}else{
				return page;
			}
		break;
	}
	
	
}
bool hay_espacio_segun_tamanio(int tamanio_alloc_carpincho){
	if(tamanio_alloc_carpincho <= config_memoria->cant_bytes_libres){
		return true;
	}
	else{
		return false;
	}
}

int obtener_pag_dir_logica(int32_t dir){
	int pag= dir / config_memoria->tamanio_pag;

	return pag;
}

int obtener_offset_dir_logica(int32_t dir){
	int offset= dir % config_memoria->tamanio_pag;

	return offset;

}



// PARA DEVOLVER LA LOGICA
int32_t calcular_direccion_logica(uint32_t prev){
	int32_t dir;
    log_error(logger,"prev: %d",prev);
	if(prev == -1){
		dir =25;
		return dir;
	}else{
        dir = prev - 9;
        return dir;
	}
}
int calcular_marco(int32_t direccion){
//	int offset = obtener_offset_dir_logica(direccion);
//	int pag = obtener_pag_dir_logica(direccion);
//	int marco =
	return 8;
}
t_carpincho* buscar_carpincho(int pid){
	t_carpincho* proceso = NULL;
	bool buscar_proc(t_carpincho* p){
		return (p->pid == pid);
	}

	proceso = list_find(lista_procesos,(void*)buscar_proc);

	return proceso;
}
heap_metadata* crear_heap(uint8_t is_free, uint32_t prev, uint32_t next){
	heap_metadata* heap =malloc(sizeof(heap_metadata));
	heap->is_free=is_free;
	heap->next_alloc=next;
	heap->prev_alloc=prev;

	return heap;
}
t_heap_nuestro* crear_heap_nuestro(int32_t pid, int32_t size, uint32_t prev, uint32_t next, uint8_t is_free){
	t_heap_nuestro* un_heap = malloc(sizeof(t_heap_nuestro));
	un_heap->heap = malloc(sizeof(heap_metadata));
	//log_trace(logger, "antes del heap");
	un_heap->heap->is_free = is_free;
	log_trace(logger, "%d",is_free);
    un_heap->heap->next_alloc =next;
    log_trace(logger, "%d",next);
    un_heap->heap->prev_alloc = prev;
    log_trace(logger, "%d",prev);
    un_heap->size = size;
    log_trace(logger, "%d",size);
    un_heap->pid = pid;
    log_trace(logger, "%d",pid);
    //log_trace(logger, "despues del heap");
    return un_heap;
}

t_heap_nuestro* buscar_heap(uint8_t is_free){

	t_heap_nuestro* heap = NULL;
	bool buscar_heap(t_heap_nuestro* p){
		return (p->heap->is_free == is_free);
	}

	heap = list_find(lista_de_heaps,(void*)buscar_heap);

	return heap;
}

void prueba(){
	t_carpincho* p1 = inicializar_proceso(1,3);

	t_carpincho* p2= inicializar_proceso(2,3);
	


	list_add(lista_procesos,p1);
	list_add(lista_procesos,p2);

//	log_trace(logger,"Carpincho: %d		Miss: %d",p3->pid, p3->miss);

}
void sumarHit(int pid){
	t_carpincho* proceso = malloc(sizeof(t_carpincho));
	bool buscar_proc(t_carpincho* p){
		if(p->pid == pid){
			p->hit++;
			return (true);
		}else{
			return(false);
		}
	}

	proceso = list_find(lista_procesos,(void*)buscar_proc);

	liberar_proceso(proceso);
}
void sumarMiss(int pid){
	t_carpincho* proceso = malloc(sizeof(t_carpincho));
	bool buscar_proc(t_carpincho* p){
		if(p->pid == pid){
			p->miss++;
			return (true);
		}else{
			return(false);
		}
	}

	proceso = list_find(lista_procesos,(void*)buscar_proc);

	liberar_proceso(proceso);
}
void imprimirHit(t_carpincho* p){
		if(p != NULL){
		log_trace(logger,"Carpincho: %d		HIT: %d",p->pid, p->hit);

		tlb_hit_totales=tlb_hit_totales+p->hit;
		}
	}
void imprimirMiss(t_carpincho* p){
		if(p != NULL){
		log_trace(logger,"Carpincho: %d		Miss: %d",p->pid, p->miss);
		tlb_miss_totales=tlb_miss_totales+p->miss;
		}
	}
void metricas(){
	list_iterate(lista_procesos,(void*)imprimirMiss);
	list_iterate(lista_procesos,(void*)imprimirHit);
	log_debug(logger,"TLB Hit Total: %d \n",tlb_hit_totales);
	
	log_debug(logger,"TLB Miss Total: %d \n",tlb_miss_totales);
} 



int cargar_pagina_en_tlb(int nro_pag){

}

