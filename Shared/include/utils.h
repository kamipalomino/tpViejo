#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include "server.h"
#include <pthread.h>
#include <commons/config.h>

#define MENSAJE 11
#define PAQUETE 10
#define ERROR 33

typedef enum
{	MATE_INIT = 1,
	MENSAJES,
	PAQUETES,
	DEBUG = 69
	
}op_code;

typedef struct
{
	int32_t size;
	void* stream;
	int32_t pid;
	int32_t memoria_a_alocar;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;



int crear_conexion(char* ip, char* puerto);
void enviar_mensaje2(int mensaje, int socket_cliente);
t_paquete* crear_paquete(int codigo_operacion, int32_t size, void* stream);
t_paquete* crear_super_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
int enviar_mensaje (int socket, t_paquete* paquete);
t_paquete* recibir_mensaje (int socket);
t_list* deserializar(t_paquete* buffer);
t_buffer* buffer_create(int32_t size, void* stream);
void buffer_destroy(t_buffer* buffer);
void paquete_destroy(t_paquete* paquete);
void semaforos(pthread_mutexattr_t atributos, pthread_mutex_t hilo);
void setearHilo(pthread_t *thread_name, void* function_name, void* params);
bool config_has_all_properties(t_config*, char**);
t_list* extraer_valores(char**);
void config_free_array_value(char*** arr);
int string_split_len(char**);

#endif /* UTILS_H_ */
