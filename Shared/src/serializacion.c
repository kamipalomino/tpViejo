#include "../include/serializacion.h"

void enviar_mem_alloc(int socket, int32_t pid,int32_t memo,void* info){
	log_error(logger,"entre en enviar mem alloc");

	t_paquete* p = crear_paquete_carpincho(ALOCAR_EN_MEMORIA,sizeof(info),info,pid,memo);
	enviar_mensaje_carpincho(socket,p);
}

t_paquete* recibir_mem_alloc(int socket){
	t_paquete* p = recibir_mensaje_carpincho(socket);
	return p;
}


t_paquete* crear_paquete_carpincho(int codigo_operacion, int size, void* stream,int32_t pid,int32_t memo) {
    t_paquete* paquete = malloc(sizeof(t_paquete));
	log_error(logger,"antes de buffer create");
    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer = buffer_create_carpincho(size, stream, pid, memo);
    log_error(logger,"despues de buffer create");
	paquete->codigo_operacion = codigo_operacion;
    paquete->buffer = buffer;

	return paquete;
}


int enviar_mensaje_carpincho (int socket, t_paquete* paquete) {

	log_info(logger,"entre en enviar mensaje");

	//log_info(logger,"%d",paquete->codigo_operacion);

	int size_paquete_serializado = sizeof(paquete->codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size + sizeof(paquete->buffer->pid) + sizeof(paquete->buffer->memoria_a_alocar);

	void *paquete_serializado = malloc(size_paquete_serializado);
	int bytes_escritos = 0;

	memcpy(paquete_serializado + bytes_escritos, &(paquete->codigo_operacion), sizeof(paquete->codigo_operacion));
	bytes_escritos += sizeof(paquete->codigo_operacion);

	memcpy(paquete_serializado + bytes_escritos, &(paquete->buffer->size), sizeof(paquete->buffer->size));
	bytes_escritos += sizeof(paquete->buffer->size);

	memcpy(paquete_serializado + bytes_escritos, paquete->buffer->stream, paquete->buffer->size);
	bytes_escritos += paquete->buffer->size;
	////
	memcpy(paquete_serializado + bytes_escritos, &(paquete->buffer->pid), sizeof(paquete->buffer->pid));
	bytes_escritos += sizeof(paquete->buffer->pid);

	memcpy(paquete_serializado + bytes_escritos, &(paquete->buffer->memoria_a_alocar), sizeof(paquete->buffer->memoria_a_alocar));
	bytes_escritos += sizeof(paquete->buffer->memoria_a_alocar);

	int status = send(socket, paquete_serializado, size_paquete_serializado, 0);

	log_info(logger,"%d",status);

	free(paquete_serializado);
	return status;
}

t_paquete* recibir_mensaje_carpincho (int socket) {
	int codigo_operacion;


	if (recv(socket, &codigo_operacion, sizeof(int), MSG_WAITALL) <= 0) {
		log_error(logger, "Ocurrio un error al recibir op_code");
		codigo_operacion = ERROR;
	}

	int size;
	if (recv(socket, &size, sizeof(int32_t), MSG_WAITALL) <= 0) {
		log_error(logger, "Ocurrio un error al recibir size");
		codigo_operacion = ERROR;
	}

	void* stream = malloc(size);
	if (recv(socket, stream, size, MSG_WAITALL) <= 0) {
		log_error(logger, "Ocurrio un error al recibir stream");
		codigo_operacion = ERROR;
	}

	int32_t pid;
	if (recv(socket, &pid, sizeof(int32_t), MSG_WAITALL) <= 0) {
		log_error(logger, "Ocurrio un error al recibir size");
		codigo_operacion = ERROR;
		}

	int32_t memo;
	if (recv(socket, &memo, sizeof(int32_t), MSG_WAITALL) <= 0) {
		log_error(logger, "Ocurrio un error al recibir size");
		codigo_operacion = ERROR;
		}


	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	paquete->codigo_operacion = codigo_operacion;
	paquete->buffer->stream = stream;
	paquete->buffer->size = size;
	paquete->buffer->pid = pid;
	paquete->buffer->memoria_a_alocar = memo;

	log_info(logger,"tamanio %d",size);
	free(stream);
	return paquete;
}

void enviar_paquete_carpincho(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 4*sizeof(int32_t);
	void* a_enviar = serializar_paquete_carpincho(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

t_buffer* buffer_create_carpincho(int size, void* stream,int32_t pid, int32_t memoria) {
    log_debug(logger,"entre en buffer create");
	t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer->stream = malloc(size);//(sizeof(stream));
    log_debug(logger,"entre en buffer create, antes del memcpy");
	memcpy(buffer->stream, &stream, size); //sizeof(stream));
	log_debug(logger,"pase el memcpy de buffercreate");
	buffer->size = size;
	buffer->pid = pid;
	buffer->memoria_a_alocar = memoria;

	return buffer;
}

void* serializar_paquete_carpincho(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int32_t));
	desplazamiento+= sizeof(int32_t);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int32_t));
	desplazamiento+= sizeof(int32_t);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	memcpy(magic + desplazamiento, &(paquete->buffer->pid), sizeof(int32_t));
	desplazamiento+= sizeof(int32_t);
	memcpy(magic + desplazamiento, &(paquete->buffer->memoria_a_alocar), sizeof(int32_t));
	desplazamiento+= sizeof(int32_t);


	return magic;
}

int conectarMatelib(int32_t pid,char* ip, char* puerto, t_log* logger){
	int conexion = crear_conexion(ip, puerto);

	char* stream = string_new();
	t_paquete* paquete = crear_paquete_carpincho(0,sizeof(stream),stream,pid,0);

	log_info(logger,"cod op: %d",paquete->codigo_operacion);

	enviar_mensaje_carpincho(conexion,paquete);
	//log_trace(logger,"%d",conexion);
	log_trace(logger,"envié mensaje al puerto %s",puerto);

	eliminar_paquete(paquete);
	free(stream);

	paquete = recibir_mensaje_carpincho(conexion);
	if(paquete->codigo_operacion==TODO_OK){
		log_trace(logger,"Me conecte\n");
		eliminar_paquete(paquete);
		return conexion;
	}else{
		log_error(logger,"Falló la conexión");
		return ALGO_SALIO_MAL;
	}
	

	
}
