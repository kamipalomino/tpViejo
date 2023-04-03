#include "include/swap.h"
#include <commons/bitarray.h>

/*

Caso asignación fija: Para que el manejo del espacio libre
y ocupado en esta partición sea sencillo, 
se utilizará un esquema de asignación contigua, donde la cantidad 
máxima de marcos en swap por carpincho estará definida por archivo de configuración

*/
void inicializar_asignacion_fija(){
    tabla_de_marcos_fija = list_create();
   	estructura_carpincho* carpincho=malloc(sizeof(estructura_carpincho));
    char* path=string_new("");
    t_list* lis = list_create();
   /*  t_pages* pag;
    for (int i = 0; i <= config_swap->marco_carpincho; i++)
    {
        pag->time=-1;
        pag->presente=0;
        pag->modificado=0;
        pag->nro_marco=i;

        list_add_in_index(lis,i,pag);
    }
    free(pag); */

	for(int i=0;i<=config_swap->cant_marcos;i++){
		carpincho=crear_carpincho_estructura(-1,0,0,i,config_swap->marco_carpincho,-1,path, -1,lis);
        
		list_add_in_index(tabla_de_marcos_fija,i,carpincho);
		eliminar_carpincho(carpincho); 
	}
    list_destroy(lis);
    free(path);
    log_warning(logger,"asignacion fija lista");
	
}
void inicializar_asignacion_global(){

    tabla_de_marcos_global = list_create();
	estructura_carpincho* carpincho;
    char* path=string_new("");
    t_list* lis = list_create();
	for(int i=0;i<=config_swap->cant_marcos;i++){
		carpincho=crear_carpincho_estructura(-1,0,0,i,config_swap->marco_carpincho,-1,path,-1,lis);
        
		list_add_in_index(tabla_de_marcos_global,i,carpincho);
		eliminar_carpincho(carpincho); 
	}
    list_destroy(lis);
    free(path);
}
estructura_carpincho* crear_carpincho_estructura(int32_t pid, int32_t modificado, int32_t presencia, int32_t marco, int libres, int32_t numero_pag,char* path,int32_t tiempo, t_list* tabla ){
    estructura_carpincho* carpincho = malloc(sizeof(estructura_carpincho));

		carpincho->pid = pid;
		carpincho->bit_modificado = modificado;
		carpincho->bit_presencia = presencia;
		carpincho->marco = marco;
		carpincho->marcos_libres = libres;
		carpincho->pagina = numero_pag;
		carpincho->path=string_duplicate(path);
        carpincho->tabla_de_pag=list_duplicate(tabla);
    return carpincho;
}

void asignar_marcos(t_paquete* p){
    int marcos = config_swap->marco_carpincho;

}