#include <biblio/include/server.h>
#include <biblio/include/main.h>
#include <biblio/include/serializacion.h>
#include "include/swap.h"
#include <pthread.h>
#include <fcntl.h>
#include <commons/memory.h>

void terminarSwap(int conexion)
{
	liberar_conexion(conexion);
	//list_destroy_and_destroy_elements(tabla_de_marcos,(void*)eliminar_carpincho);
	//liberar_tabla_de_marcos();
//	liberar_bitarray();
	list_destroy(tabla_de_marcos_fija);
	liberar_configuracion();
	log_destroy(logger);
	printf("----------FIN------------\n");
}
void eliminar_carpincho(estructura_carpincho* carpi){
	list_destroy(carpi->tabla_de_pag);
	free(carpi->path);
	free(carpi);
}
void liberar_tabla_de_marcos_fija(){
	for(int i=0;i<=config_swap->cant_marcos;i++){
			list_remove_and_destroy_element(tabla_de_marcos_fija,i,(void*)(eliminar_carpincho));
		}
		//list_destroy(tabla_de_marcos);
}
void liberar_configuracion(){
	free(config_swap->ip);
	int i =0;
	/* while (config_swap->archivos[i]!= NULL){
		free(config_swap->archivos[i]);
		i++;
	} */
	string_array_destroy(config_swap->archivos);
	free(config_swap->archivos);
	free(config_swap->tipo_asignacion);
	free(config_swap->puerto_swap);
	free(config_swap);
	config_destroy(configSwamp);
}
void liberar_bitarray(){
     int x = list_size(config_swap->archivos);
    log_info("archivos: %d",x);

    for (int i = 0; i <= x; i++)
    {
       bitarray_destroy(bitarray[i]);
       log_info("Liberando bitarray: %d",bitarray_get_max_bit(bitarray[i]));
    }
}