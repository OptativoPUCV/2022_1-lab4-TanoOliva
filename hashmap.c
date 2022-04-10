#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}
/*
2.- Implemente la función void insertMap(HashMap * map, char * key, void * value). Esta función inserta un nuevo dato (key,value) en el mapa y
actualiza el índice current a esa posición.
Recuerde que para insertar un par (clave,valor) debe:

a - Aplicar la función hash a la clave para obtener la posición donde debería insertar el nuevo par

b - Si la casilla se encuentra ocupada, avance hasta una casilla disponible (*método de resolución de colisiones*). Una casilla disponible es una casilla nula, pero también una que tenga un par inválido (key==NULL).

c - Ingrese el par en la casilla que encontró.
*/

void insertMap(HashMap * map, char * key, void * value) {
   long idx = hash(key,map->capacity);
   while (map->buckets[idx]!=NULL && map->buckets[idx]->key!=NULL)
   {

       if (is_equal(key,map->buckets[idx]->key) == 1) return;
       idx = (idx + 1) % map->capacity; 
   }
   if (map->buckets[idx]!=NULL){
       map->buckets[idx]->key  = key;
       map->buckets[idx]->value = value;
   } else map->buckets[idx] = createPair(key,value);
   map->size++;
       
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}
/*
1.- Implemente la función *createMap* en el archivo hashmap.c. Esta función crea una variable de tipo HashMap,
    inicializa el arreglo de buckets con casillas nulas, inicializa el resto de variables y retorna el mapa.
    Inicialice el índice current a -1.
*/
HashMap * createMap(long capacity) {
    HashMap* array= (HashMap *) malloc (sizeof(HashMap));
    array->buckets=(Pair**) calloc (capacity,sizeof(Pair*));
    array->current=-1;
    array->capacity=capacity;
    array->size=0;
    return array;
}
/*
4.- Implemente la función void eraseMap(HashMap * map,  char * key). Está función elimina el dato correspondiente a la clave key.
 Para hacerlo debe buscar el dato y luego *marcarlo* para que no sea válido.
**No elimine el par**, sólo invalídelo asignando NULL a la clave (pair->key=NULL).
Recuerde actualizar la variable size.
*/
void eraseMap(HashMap * map,  char * key) {    
    long idx = hash(key,map->capacity);
    while (map->buckets[idx] != NULL && map->buckets[idx]->key!=NULL){

        if (is_equal(key,map->buckets[idx]->key) == 1){
            map->current = idx;
            map->size--;
            map->buckets[idx]->key=NULL;
            return;
            
        }
        idx = ((idx + 1) % map->capacity); 
    }

    return;

}
/*
    a - Usar la función hash para obtener la posición donde puede encontrarse el par con la clave 
    b - Si la clave no se encuentra avance hasta encontrarla (*método de resolución de colisiones*)
    c - Si llega a una casilla nula, retorne NULL inmediatamente (no siga avanzando, la clave no está)
*/
Pair * searchMap(HashMap * map,  char * key) {   
    long idx = hash(key,map->capacity);
    while (map->buckets[idx] != NULL && map->buckets[idx]->key!=NULL){

        if (is_equal(key,map->buckets[idx]->key) == 1){
            map->current = idx;
            return map->buckets[idx];
            
        }
        idx = ((idx + 1) % map->capacity); 
    }

    return NULL;
}
/*
5.- Implemente las funciones para recorrer la estructura: Pair * firstMap(HashMap * map) retorna el primer **Pair** válido del arreglo buckets.
    Pair * nextMap(HashMap * map) retorna el siguiente **Pair** del arreglo buckets a partir índice current. Recuerde actualizar el índice.
*/

Pair * firstMap(HashMap * map) {
    long idx=0;
    while (idx<map->capacity){
        if (map->buckets[idx] != NULL && map->buckets[idx]->key != NULL){
            map->current = idx;
            return map->buckets[idx];
        }
        idx = ((idx + 1) % map->capacity); 
    }
    //si retorna null significa que no habia ningun elemento en el arrgelo 
    return NULL;
}

Pair * nextMap(HashMap * map) {
    long idx = map->current;
    while (idx<map->capacity){
        if (map->buckets[idx] != NULL && map->buckets[idx]->key != NULL) {
            map->current=idx;
            return map->buckets[idx];
        }
        idx = ((idx + 1) % map->capacity);
    }
    return NULL;
}
