/*
 ============================================================================
 Name        : test_carpincho.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <matelib/include/matelib.h>
#include <matelib/include/mate_init.h>
#include <commons/config.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <biblio/include/id_unico.h>
#include <commons/memory.h>
int main (void){
	logger = log_create("log.log","soy carpincho",true,LOG_LEVEL_TRACE);
	mate_instance* mate = malloc(sizeof(mate_instance));

	mate_init(mate,"../carp.config");

	log_trace(mate->logger, "pase mateinit");

	mate_pointer puntero =-5;

	log_debug(mate->logger,"antes del stream");
	char* stream = string_new("mando a guardar memo!!!");
	memcpy(mate->group_info,stream,strlen(stream));
	log_debug(mate->logger,"despues del memcpy");
	log_trace(mate->logger, "pid:%d",mate->mid);
	puntero =mate_memalloc(mate,24);

	log_trace(mate->logger, "me llegó el puntero %d", puntero);
	log_trace(mate->logger, "me voy a dormir");
	
	mate_close(mate);
	free(stream);
	return 0;
}
