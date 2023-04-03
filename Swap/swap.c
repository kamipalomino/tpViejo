#include "include/swap.h"
#include "inicializacion.c"
#include "finalizar.c"
#include "asignaciones.c"
#include <biblio/include/server.h>
#include <biblio/include/main.h>
#include <biblio/include/serializacion.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <commons/memory.h>

int main(void) {
	startSigHandlers_Sw();
	logger = log_create("swap.log", "Swamp", 1, LOG_LEVEL_TRACE);
	
	//configSwamp = config_create("configuracionSWAMP.config");    //DESCOMENTAR
	configSwamp =config_create("simple.config"); 	// COMENTAR
	inicializar_configuracion();
	log_info(logger, "se cargaron las config");
	

	inicializar_swap();

	setearHilo(&hiloConexion, (void*)conexionesSwamp2, NULL);
	pthread_join(hiloConexion,NULL);
	//pthread_exit(&hiloConexion);

//	pthread_exit(&hiloConexion);
	log_info(logger,"finalizar");
	terminarSwap(server_fd);
	
//	pthread_exit(hiloConexion);
	//pthread_detach(hiloConexion);
	return EXIT_SUCCESS;
}

void iterator(t_log* logger, char* value) {
	log_info(logger,"%s\n", value);
}



void conexionesSwamp(int cliente_fd){

}
void conexionesSwamp2(){
	server_fd = iniciar_servidor(config_swap->ip, config_swap->puerto_swap);
	log_info(logger, "iniciando servidor swamp");
	log_info(logger, "Esperando nueva conexion..");
	
	cliente_fd = esperar_cliente(server_fd);
	running = 1;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete = recibir_mensaje(cliente_fd);
		
	int codigo = paquete->codigo_operacion;

	if(codigo != MEMORIA){
			log_error(logger,"Operacion desconocida. No quieras meter la pata");
			online=-1;
			running =FINALIZAR_SWAP;
			eliminar_paquete(paquete);
			pthread_exit(&hiloConexion);
			liberar_conexion(cliente_fd);
			exit(EXIT_FAILURE);
		}else{
			
			log_info(logger,"Se conectó memoria\n");
			
			int x = atoi(paquete->buffer->stream);
			
			inicializar_asignacion(x);
			eliminar_paquete(paquete);
			
			while(running!=FINALIZAR_SWAP){

				paquete = recibir_mensaje_carpincho(cliente_fd);
				log_info(logger,"%d",paquete->codigo_operacion);
				codigo = paquete->codigo_operacion;
				switch (codigo) {

					case PAQUETES:

						log_info(logger, "Me llegaron los siguientes valores: %s\n",(char*)paquete->buffer->stream);//como lo printeamos?
						//log_info(logger, "entre en paquete");
						
						eliminar_paquete(paquete);
						pthread_exit(&hiloConexion);

					break;
					case PEDIR_PAGINA:
						pedir_pagina(paquete);
						eliminar_paquete(paquete);
					break;
					case LEER_PAGINA:
						leer_pagina(paquete);
						eliminar_paquete(paquete);
					break;
					case ESCRIBIR_PAGINA:
						escribir_pagina(paquete);
						eliminar_paquete(paquete);
					break;
					case BORRAR_PAGINA:
						borrar_pagina(paquete);
						eliminar_paquete(paquete);
					break;
					case CARPINCHO_NEW:
						asignar_marcos(paquete);
						eliminar_paquete(paquete);
					break;
					case CARPINCHO_KILL:
					break;
					case FINALIZAR_SWAP:
						log_trace(logger,"Me mandaron a dormir :( \n");
						running = FINALIZAR_SWAP;
						eliminar_paquete(paquete);
						liberar_conexion(cliente_fd);
						pthread_exit(hiloConexion);
						
					break;

						default:
							log_warning(logger,
									"Operacion desconocida. No quieras meter la pata");
							online=-1;
							running = FINALIZAR_SWAP;
							eliminar_paquete(paquete);
							pthread_exit(&hiloConexion);
							liberar_conexion(cliente_fd);
							break;

							}
					
			}

		}
}




void pruebaArchivo(){
	int f = open(config_swap->archivos[2], 02, (mode_t) 0777);
	char* s=string_repeat('\0', 20);
	write(f,s,20);
	free(s);
	s= string_duplicate("Hoy te busqué en la rima que duerme con todas las palabras, si algo callé es porque entendí todo, menos la distancia.");
	write(f,s,strlen(s)+1);
			close(f);
}

char* buscar_archivo(int i){
	char* path = string_new();
	return path;
}
int  obtener_offset(uint32_t dir){
	return 20;
}
void leer_pagina(t_paquete* proceso){ 
	/* TO DO */
	//char* f= buscar_archivo(proceso->buffer->pid);
	//f=string_duplicate(config_swap->archivos[2]);
	int dir =proceso->buffer->memoria_a_alocar;
	int file = open(config_swap->archivos[2], O_RDONLY);
	void* buf;
	int offset = obtener_offset(dir);
	//eliminar_paquete(proceso);
	lseek(file,offset , SEEK_SET);
	read(file,buf, 119);
	close(file);

	//t_paquete* respuesta=malloc(sizeof(t_paquete));

	proceso=crear_paquete_carpincho(LEER_PAGINA,sizeof(buf),buf,12,(int)2);
	enviar_paquete_carpincho(proceso,cliente_fd);
	log_warning(logger,"pase enviar paquete");
	//mem_hexstring(proceso->buffer->stream,sizeof(proceso->buffer->stream));
	//log_warning(logger,"Se respondio a Memoria %s",buf);
	mem_hexdump(proceso->buffer->stream,sizeof(proceso->buffer->stream));
	log_warning(logger,"pase mem hexdump");
	free(buf);
	//free(f);

}
void escribir_pagina(t_paquete* proceso){
	/* TO DO */
	int pid =proceso->buffer->pid;
	escribir_bytes(pid,proceso->buffer->memoria_a_alocar,proceso->buffer->stream);
	
	enviar_paquete_carpincho(proceso,cliente_fd);
	log_debug(logger,"Se respondio a Memoria %d",proceso->codigo_operacion);
	
	}
char* obtener_archivo(int pid, int pag){
	char* file = string_new("");
	return file;
}
int obtener_marco(int pid, int pag){
	int marco;

	return marco;
}

/*
int32_t pid;
    int32_t marco;
    int32_t pagina;
    int32_t bit_presencia;
    int32_t bit_modificado;
    int32_t marcos_libres;
    int32_t time;
*/
void buscar_carpincho_borrar_pag(int32_t pid, int32_t pag){
	estructura_carpincho* carpi;
	bool buscar_p(estructura_carpincho* t){
		int marco;
		if(t->pid == pid && t->pagina ==pag){
			t->marco=-1;
			t->bit_presencia=0;
			t->marcos_libres++;
			if(t->bit_modificado==1){
				//escribir_bytes()
				t->bit_modificado=0;
				return(true);
			}

				return(true);
			}
			return(false);
		}
		carpi=list_find(tabla_de_marcos_global,(void*)buscar_p);
	}

void borrar_pagina(t_paquete* proceso){
	/* TO DO */

	int pag = proceso->buffer->memoria_a_alocar;
	int32_t pid = proceso->buffer->pid;

	char* archivo =obtener_archivo(pid,pag);
	int marco = obtener_marco(pid,pag);
	int offset=obtener_offset(pag);
	//int size, void* stream,uint32_t pid,uint32_t memo
	enviar_paquete_carpincho(proceso,cliente_fd);
	log_debug(logger,"Se respondio a Memoria %d",proceso->codigo_operacion);
	
	//free(stream);

	}
void pedir_pagina(t_paquete* proceso){
	/* TO DO */
	int pag = proceso->buffer->memoria_a_alocar;
	int32_t pid = proceso->buffer->pid;

	t_paquete* respuesta=malloc(sizeof(t_paquete));
	char* stream=string_new("");
	//respuesta=crear_paquete(PEDIR_PAGINA,sizeof(stream),stream);

	enviar_paquete(respuesta,cliente_fd);
	log_debug(logger,"Se respondio a Memoria %d",respuesta->codigo_operacion);
	eliminar_paquete(respuesta);
	free(stream);
	}
// int escribir_bytes(int pid, int pagina, void * datos_a_escribir) {

// 	struct stat buff;
// 	void * espacio_swap = mmap((caddr_t) 0, buf.st_size, PROT_READ | PROT_WRITE,
// 	MAP_SHARED | MAP_NORESERVE, fd, 0);
// }
/* void eliminar_paquete(t_paquete* paquete){
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}


*/

int32_t elegir_archivo(int size){
	
	//struct stat buf;
///	struct stat buf_ant;
	int size_next,size_ant,x=0;
	
	for (int i = 0; i < 5; ++i)
	{
		//ESTA MAL VER COMO CALCULAR ESPACIO LIBRE ARCHIVO
		size_ant=list_size(config_swap->archivos[i]);
		size_next=list_size(config_swap->archivos[i+1]);
		if(size_next>size_ant){
			//arch=size_next;
			x=i+1;
		}else{ x=i;}
	}
	size_ant=list_size(config_swap->archivos[x]);
	if(size<= size_ant){
		return x;
	}else{
		return -1;
	}

}
int escribir_bytes(int pid, int pagina, void * datos_a_escribir) {
	int x = elegir_archivo(sizeof(datos_a_escribir));
	if (x == -1)
	{
		log_error(logger,"No hay espacio suficiente");
		return -1;
	}
	
	int fd = open(config_swap->archivos[x], O_RDWR, __S_IWRITE| __S_IREAD);

	if (fd == -1)
		return -1;

	struct stat buf;

	if ((stat(config_swap->archivos[0], &buf)) == -1) {
		close(fd);
		return -1;
	}

	void * espacio_swap = mmap((caddr_t) 0, buf.st_size, PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_NORESERVE, fd, 0);


	if (espacio_swap == MAP_FAILED) {
		close(fd);
		return -1;
	}

	usleep(config_swap->retardo_swap * 1000);

	int direccion_fisica = pagina * config_swap->tamanio_pag;

	memcpy(espacio_swap + direccion_fisica, datos_a_escribir, config_swap->tamanio_pag);
	msync(espacio_swap, buf.st_size, 0);

	close(fd);
	munmap(espacio_swap, buf.st_size);
	free(espacio_swap);
	return 0;
}




void startSigHandlers_Sw(void) {
	signal(SIGINT, sigHandler_sigint_Sw);
}
void sigHandler_sigint_Sw(int signo) {
	log_error(logger, "Tiraste un CTRL+C, macho, abortaste el proceso.");


	terminarSwap(server_fd);
	printf("-------------------FINAL POR CTRL+C-------------------");

	exit(-1);
}
