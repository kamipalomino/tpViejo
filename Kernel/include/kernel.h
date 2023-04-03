#ifndef MOD_KERNEL_KERNEL_H_
#define MOD_KERNEL_KERNEL_H_

#include <stdlib.h>
#include <stdio.h>

#include "funcs.h"
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <pthread.h> // ../../ para salir fuera de mi modulo
#include <biblio/include/main.h>
#include <biblio/include/utils.h>

char* ip, puertoM;
t_config* configKernel;
t_log* logger;
pthread_t hiloMemoria;
pthread_t hiloConexion;
void terminar_kernel(int conexion, t_log* logger, t_config* config);
void conectarMemoria();
void conexionesMatelib();
int conexion;
#endif /* MOD_KERNEL_KERNEL_H_ */
