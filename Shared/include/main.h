#ifndef SHARED_MAIN_H_
#define SHARED_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>


#define DESBLOQUEAR_SEM 101 // mate_sem_post()
#define BLOQUEAR_SEM 102 // mate_sem_wait()
#define INICIAR_SEM 103 // mate_sem_init
#define DESTRUIR_SEM 104 // mate_sem_destroy()

#define INCIAR_KERNEL 201 //  mate_init()
#define FINALIZAR_KERNEL 202 // mate_close()
#define KERNEL_OK 203 //    conexion con memoria OK empezar a ejecutar 

#define LLAMAR_IO 301 //mate_call_io()

#define ALOCAR_EN_MEMORIA 401 //  mate_memalloc() 
#define LIBERAR_MEMORIA 402 // mate_memfree()
#define LEER_EN_MEMORIA 403 // mate_memread()
#define ESCRIBIR_EN_MEMORIA 404 // mate_memwrite()
#define ASIGNACION_FIJA 405
#define ASIGNACION_GLOBAL 406
#define CANT_PAGS_MAXIMAS 407

#define PEDIR_PAGINA    501
#define LEER_PAGINA     502
#define ESCRIBIR_PAGINA 503
#define BORRAR_PAGINA   504
#define FINALIZAR_SWAP  505
#define TODO_OK			506
#define	CARPINCHO_NEW	507
#define CARPINCHO_KILL	508

#define ALGO_SALIO_MAL  -5
typedef enum{
    MATELIB,
    KERNEL,
    MEMORIA,
    SWAMP,
    PROCESO,
} op_modulos;


typedef struct{
	int32_t pid;
	int miss;
	int hit;
	int frame_libres;
	t_list* tabla_paginas;
	t_list* lista_marcos; // referencias a los marcos donde se encuentra la información de cada página.
	t_list_iterator* iterar_marco;
}t_carpincho;

#endif
