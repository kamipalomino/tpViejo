#ifndef SERIALIZACION_MAIN_H
#define SERIALIZACION_MAIN_H

#include <errno.h>
#include "../include/serializacion.h"
#include "../include/main.h"
#include "../include/server.h"
#include "../include/client.h"
#include "../include/utils.h"

int conectarMatelib(int32_t pid,char* ip, char* puerto, t_log* logger);
void enviar_mem_alloc(int socket,int32_t pid,int32_t memo,void* info);
t_paquete* crear_paquete_carpincho(int codigo_operacion, int size, void* stream,int32_t pid,int32_t memo);
void enviar_paquete_carpincho(t_paquete* paquete, int socket_cliente);
t_paquete* recibir_mensaje_carpincho (int socket);
int enviar_mensaje_carpincho (int socket, t_paquete* paquete);
t_buffer* buffer_create_carpincho(int size, void* stream,int32_t pid, int32_t memoria);
void* serializar_paquete_carpincho(t_paquete* paquete, int bytes);







#endif
