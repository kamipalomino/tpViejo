#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <inttypes.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/log.h>

typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCKED,
    BLOCKED_SUSP,
    BLOCKED_READY,
    EXIT,             
} status;

typedef struct {
    int pid;
    status status;
    int estimacion_anterior;
    int rafaga_anterior;
} pcb_carpincho;


bool send_debug(int fd);

bool send_codigo_op();

//interfaz con matelib
bool send_mate_init(int fd);


#endif
