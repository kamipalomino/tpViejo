#ifndef SERVER_H_
#define SERVER_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>


t_log* logger;

void* recibir_buffer(int*, int);
void iterator(t_log* logger, char* value);
int iniciar_servidor(char*ip, char* puerto);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje2(int);
int recibir_operacion(int);


#endif /* SERVER_H_ */
