#include "../include/procesar_carpinchos.h"
#include  <biblio/include/main.h>
#include  <biblio/include/utils.h>
#include  <biblio/include/protocolo.h>
#include  <biblio/include/sockets.h>
#include "../include/log.h"


int generar_pid() { //TODO
    return 1;
}

void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    // Mientras la conexion este abierta
    op_code cop;
    while (cliente_socket != -1) {

        if (recv(cliente_socket, &cop, sizeof(op_code), 0) == 0) {
        	log_info(main_log, "DISCONNECT!");
            return;
        }

        switch (cop) {
            case DEBUG:
                log_info(main_log, "Me llego el debug!");
                break;
            case MATE_INIT:
                log_info(main_log, "Me llego el MATE INIT!");
                int a = rcv_mate_init2(cliente_socket);
                if(a) {
                    pcb_carpincho* crpcho = generar_estructuras();
                    planificar_nuevo_cpncho_FIFO(crpcho);
                } else {
                    log_error(main_log, "error al recibir el mate init");
                }
                break;
            default:
                log_error(main_log, "Algo anduvo mal en el kernel");
                return;
        }
    }

    log_warning(main_log, "El cliente se desconecto de %s server", server_name);
   // return 0;
}

int server_escuchar(const char* server_name, int server_fd) {
    int cliente_socket;
    cliente_socket = esperar_cliente(server_fd);
    // = esperar_cliente2(main_log, server_name, server_fd);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}

pcb_carpincho* generar_estructuras() {
    pcb_carpincho* carpincho = malloc(sizeof(pcb_carpincho));
    carpincho->pid = generar_pid();
    carpincho->status = NEW;
    carpincho->estimacion_anterior = KERNEL_CFG->ESTIMACION_INICIAL;
    carpincho->rafaga_anterior = 0;
    push_cola_new(carpincho);
    return carpincho;
}

void push_cola_new(pcb_carpincho* cpncho) {
    //mutexlock
    queue_push(COLA_NEW, cpncho);
    //mutexunlock
}

pcb_carpincho* pop_cola_new() {
    //mutexlock
    void* p = queue_pop(COLA_NEW);
    //mutexunlock
    return p;
}

void push_cola_ready(pcb_carpincho* cpncho) {
    //mutexlock
    queue_push(COLA_READY, cpncho);
    //mutexunlock
}

void planificar_nuevo_cpncho_FIFO(pcb_carpincho* carpinchoNuevo) {
    while(!queue_is_empty(COLA_NEW)){
        //wait(semGradoMultiprogramacion)
        pcb_carpincho* proximo_cpncho = pop_cola_new();
        proximo_cpncho->status = READY;
        push_cola_ready(proximo_cpncho);
        //signal(semGradoMultiprogramacion)
    }

}


void planificar_ejecucion_SJF(char* algoritmo, int grado_multiprocesamiento, int est_init, int alfa) {
    // while(!queue_is_empty(COLA_READY)){ 
    // }
    int readySize = queue_size(COLA_READY);
    t_list* listaReadyRafaga = list_create();
    for (int i = 0; i < readySize; i++){
        tupla_sjf tuple;
        tuple.carpincho = list_get(COLA_READY->elements, i);
        tuple.estimacion_actual = calcular_rafaga(tuple.carpincho);
       // list_add(listaReadyRafaga, tuple);  TODO: ver porq no lo toma
    }
    
    pcb_carpincho* carpMenorEst = list_get(listaReadyRafaga, 0);
    //TODO: iterar y comparar para quedarte con la menor estimacion

    
}

int calcular_rafaga(pcb_carpincho* carpincho){
     // prox_estimacion = estimacion_ant * alfa + rafaga_real_ant * (1 - alfa)
    return carpincho->estimacion_anterior * KERNEL_CFG->ALFA + carpincho->rafaga_anterior * (1 - KERNEL_CFG->ALFA);
}
int rcv_mate_init2(int fd){
    status n_status;
    if(recv(fd, &n_status, sizeof(status), 0) != 0)
        return n_status;
    else {
        close(fd);
        return -1;
    } 
}