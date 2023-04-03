  
#ifndef SOCKETS_H
#define SOCKETS_H

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <string.h>
#include <stdlib.h>

int iniciar_servidor2(t_log* logger, const char* name, char* ip, char* puerto);
int esperar_cliente2(t_log* logger, const char* name, int socket_servidor);
int crear_conexion2(t_log* logger, const char* server_name, char* ip, char* puerto);
void liberar_conexion2(int* socket_cliente);
int rcv_mate_init(int fd);
#endif /* SOCKETS_H */
