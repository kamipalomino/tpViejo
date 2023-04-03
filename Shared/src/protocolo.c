#include "../include/protocolo.h"
#include "../include/utils.h"

static op_code recibir_cop(int fd) {
    op_code cop;
    if(recv(fd, &cop, sizeof(op_code), 0) != 0)
        return cop;
    else {
        close(fd);
        return -1;
    }    
}

bool send_codigo_op(int fd, op_code cop) {
    size_t size = sizeof(op_code);
    return send(fd,&cop,size,0) != -1;
}

bool send_debug(int fd) {
    op_code cop = DEBUG;
    if (send(fd, &cop, sizeof(op_code), 0) != sizeof(op_code)) {
        return false;
    }
    return true;
}

// MATELIB conexiones
bool send_mate_init(int fd){ //quien hace el send? matelib(?)
    status n_status = NEW;
    return send(fd, &n_status, sizeof(status), 0);
}

