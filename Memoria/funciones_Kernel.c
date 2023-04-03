#include "include/memoria.h"
#include <biblio/include/main.h>
#include <bits/types.h>
#include <sys/mman.h>
#include <stddef.h>
#include <biblio/include/sockets.h>
#include <commons/memory.h>


void ejecutarKernel(int cliente){
	log_info(logger,"cliente: %d",cliente);
	t_paquete* respuesta=malloc(sizeof(t_paquete));
	char* stream=string_new("");
	respuesta=crear_paquete_carpincho(TODO_OK,sizeof(stream),stream,NULL,NULL);
	enviar_paquete_carpincho(respuesta,cliente);
	log_error(logger,"pase enviar paquete, entro a eliminar paquete");
	eliminar_paquete(respuesta);
	//respuesta=recibir_mensaje(cliente);
	respuesta = recibir_mensaje_carpincho(cliente);

	int cod= respuesta->codigo_operacion;
	switch (cod){
		case ALOCAR_EN_MEMORIA:
			//  mate_memalloc()
			log_info(logger,"el carpincho pide alocar %d",respuesta->buffer->memoria_a_alocar);
			mem_alloc(respuesta,cliente);
			//hasta aca funciona
			//eliminar_paquete(respuesta);
			respuesta=crear_paquete(KERNEL_OK,sizeof(stream),stream);
			enviar_paquete(respuesta,cliente);
		break;
		case LIBERAR_MEMORIA:
			//mate_memfree()
			eliminar_paquete(respuesta);
			respuesta=crear_paquete(KERNEL_OK,sizeof(stream),stream);
			enviar_paquete(respuesta,cliente);
		break;
		case LEER_EN_MEMORIA:
			// mate_memread()
			eliminar_paquete(respuesta);
			respuesta=crear_paquete(KERNEL_OK,sizeof(stream),stream);
			enviar_paquete(respuesta,cliente);
		break;
		case ESCRIBIR_EN_MEMORIA:
			//mate_memwrite()
			eliminar_paquete(respuesta);
			respuesta=crear_paquete(KERNEL_OK,sizeof(stream),stream);
			enviar_paquete(respuesta,cliente);
		break;

		default:
			eliminar_paquete(respuesta);
			break;
	}
	eliminar_paquete(respuesta);
	free(stream);
	close(conexion);
}
void alocar_memoria(int inicio,void* info){
	int heap_size =sizeof(heap_metadata);
	//memoria_principal 
	//ACA ES DONDE SOBREESCRIBIMOS EL HEAP QUE TENIAMOS POR EL NUEVO MODIFICADOOOOOOOOOOOO (PREV ALLOC, NEXT ALLOC)
	t_heap_nuestro* heap = crear_heap_nuestro(-1,0,inicio-heap_size+sizeof(info),heap_size+sizeof(info)+heap_size,true);
	
	void* copia= mmap((int)inicio, inicio+heap_size+sizeof(info), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_NORESERVE, NULL, 0);

	//int direccion_fisica = pagina * config_swap->tamanio_pag;

	memcpy(copia, &info,sizeof(info));
	memcpy(copia, &heap->heap,heap_size);


	memoria_principal = mmap(inicio, inicio+heap_size+heap->size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, NULL, 0);
	
	memcpy(memoria_principal,&copia,sizeof(copia));

	munmap(copia, inicio+heap_size+heap->size);
	free(copia);
}
int32_t primer_alloc(t_carpincho* un_carpincho,t_paquete* rta){
	 int pid = rta->buffer->pid;

	 int32_t direccion;

	 rta->codigo_operacion = CARPINCHO_NEW;

		if(hay_espacio_segun_tamanio(rta->buffer->memoria_a_alocar)){
		
		    log_debug(logger, "%d",rta->buffer->pid);
            log_debug(logger, "%d",rta->buffer->memoria_a_alocar);
			direccion = first_fit(pid, rta->buffer->memoria_a_alocar,rta->buffer->stream);


           log_debug(logger, "direccion logica: %d",direccion);

         //  enviar_paquete_carpincho(rta,conexion);



           return direccion;

		}else{
			// ACA HAY QUE LLAMAR A SWAP
		    // correr el algoritmo de reemplazo
			enviar_mensaje_carpincho(conexion,rta);

			eliminar_paquete(rta);
			rta=recibir_mensaje_carpincho(conexion);
			return -3;
	}
}


void primer_alocar_memoria(void* info){

	//memoria_principal
	t_heap_nuestro* un_heap = malloc(sizeof(t_heap_nuestro));
	un_heap->heap = malloc(sizeof(heap_metadata));
	//log_debug(logger,"%d",sizeof(info));


	//log_debug(logger,"%d",sizeof(info)+sizeof(heap_metadata)); //esta cuenta estamos sumando size de lo que hay que alocar + el heap para la direccion previa


	un_heap=crear_heap_nuestro(-1,sizeof(info),-1,sizeof(info)+sizeof(heap_metadata),false);

	int heap_size =sizeof(heap_metadata);
	void* copia= mmap(0, (heap_size+sizeof(info))+1, PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_ANONYMOUS, -1, 0);


	//log_debug(logger,"despues del mmap");
	memcpy(copia, &un_heap->heap,heap_size);
	//log_debug(logger,"despues del memcpy 1");
	memcpy(copia, &info,(sizeof(info)));
	//log_debug(logger,"despues del memcpy de alloc");
	liberar_heap_nuestro(un_heap);


	//int direccion_fisica = pagina * config_swap->tamanio_pag;

	un_heap=crear_heap_nuestro(-1,0,0,-1,true);

	memcpy(copia, &un_heap->heap,heap_size);
	liberar_heap_nuestro(un_heap);

	char* x = mem_hexstring(info,(strlen(info))+2);
    log_warning(logger,"%s",x);
    free(x);


	memoria_principal = mmap(0, heap_size+heap_size+sizeof(info), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, NULL, 0);

	memcpy(memoria_principal,&copia,sizeof(copia));

	munmap(copia, sizeof(copia));
	free(copia);
	char* y = mem_hexstring(memoria_principal,(sizeof(heap_size+heap_size+sizeof(info)))+2);
	log_warning(logger,"%s",y);
	mem_hexdump(memoria_principal,(sizeof(heap_size+heap_size+sizeof(info)))+2);


	free(y);
}

// ASIGNAR TODAS LAS PAGINAS A LA TABLA DE PAGINAS Y A LA TLB

//PARA CALCULAR LA PROXIMA DIRECCION LOGICA ES: EL NEXT DEL ULTIMO HEAP MENOS EL HEAP METADATA; SALVO EL PRIMER CASO
//
void* mem_alloc(t_paquete* rta, int cliente){
	//rta = recibir_mem_alloc(cliente);
	log_warning(logger,"%d",rta->buffer->pid);
	log_warning(logger,"%d",rta->buffer->memoria_a_alocar);
    log_info(logger,"%s",mem_hexstring(rta->buffer->stream,(rta->buffer->memoria_a_alocar)+1));
	int pid = rta->buffer->pid;
    int32_t direccion;
	t_carpincho* un_carpincho = buscar_carpincho(pid);
	if(un_carpincho == NULL){
		un_carpincho = inicializar_proceso(pid,config_memoria->cant_paginas_maximas);
		direccion = primer_alloc(un_carpincho, rta);
		//int pagina = obtener_pag_dir_logica(direccion);
		//int marco = calcular_marco(direccion);

		un_carpincho->miss++;
		log_debug(logger,"direccion:%d",direccion);
		actualizar_estructuras(un_carpincho,direccion);

	}

	log_debug(logger,"sarasa:%d",direccion);
	enviar_mem_alloc(cliente,rta->buffer->pid,direccion,NULL);


	log_debug(logger,"termino el memalloc");
	//eliminar_paquete(rta);
}

