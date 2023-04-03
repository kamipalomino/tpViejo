#include "../include/id_unico.h"

// Retorna un timestamp.
int crear_id_unico(){
  return (int)time(NULL);
}