#include "include/memoria.h"

void comunicacionSwap(int codigo, t_carpincho* carpincho, int32_t dir){

	char* stream=string_new("");
	switch (codigo) {
					case PEDIR_PAGINA:

						pedir_pagina_memo(carpincho->pid,dir);

						free(stream);
					break;
					case LEER_PAGINA:
						leer_pagina_memo(carpincho->pid,dir);

					break;
					case ESCRIBIR_PAGINA:
						escribir_pagina_memo(carpincho->pid,dir,carpincho->lista_marcos);

					break;

					case BORRAR_PAGINA:
						borrar_pagina_memo(carpincho->pid,dir);

					break;
					case FINALIZAR_SWAP:
						log_trace(logger,"Me mandaron a dormir :( \n");

					break;
						default:
							log_warning(logger,
									"Operacion desconocida. No quieras meter la pata");


						break;

							}
			}


int pedir_pagina_memo(int32_t pid, int32_t dir){
	t_paquete* paquete;
	char* stream = string_new("");
//	int pag = obtener_pag_dir_logica(dir);
	paquete= crear_paquete_carpincho(PEDIR_PAGINA, sizeof(stream), stream,pid,dir);
	enviar_mensaje_carpincho(conexion, paquete);
	eliminar_paquete(paquete);
	free(stream);
}
int leer_pagina_memo(int32_t pid, int32_t dir){
	char* stream = string_new("");
	t_paquete* paquete;

	paquete= crear_paquete_carpincho(LEER_PAGINA, sizeof(stream), stream,pid,dir);
	enviar_mensaje_carpincho(conexion, paquete);
	eliminar_paquete(paquete);
	free(stream);
}
int escribir_pagina_memo(int32_t pid, int32_t dir, void* info){
	char* stream = string_new("");
	t_paquete* paquete;
	paqueteConInfo(conexion, ESCRIBIR_PAGINA, stream);
	paquete= crear_paquete_carpincho(ESCRIBIR_PAGINA, sizeof(info), info,pid,dir);
		enviar_mensaje_carpincho(conexion, paquete);
	free(stream);
	eliminar_paquete(paquete);
}
int borrar_pagina_memo(int32_t pid, int32_t dir){
	t_paquete* paquete;
	char* stream = string_new("");

	paquete= crear_paquete_carpincho(BORRAR_PAGINA, sizeof(stream), stream,pid,dir);
	enviar_mensaje_carpincho(conexion, paquete);
	free(stream);
	eliminar_paquete(paquete);
}
