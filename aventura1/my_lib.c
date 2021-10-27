#include "my_lib.h"

/*
Calcula el nº de bytes de la cadena apuntada por str, sin incluir el carácter nulo de terminación ‘\0’ 
Devuelve la longitud de la cadena apuntada por str. No devuelve error.
*/
size_t my_strlen(const char *str){
    size_t len = 0;
    while (*str++) { // comprueba el carácter y avanza a la siguiente posición
       len++;
    }
    return len;
}

 /*
compara las cadenas apuntadas por str1 y str2.
Devuelve un entero:
< 0 indica que str1 < str2.
> 0 indica que str2 < str1.
= 0 indica que str1 = str2.
No se basa en la longitud de las cadenas sino en los códigos ASCII de cada carácter. 
Compara carácter a carácter y cuando encuentra uno diferente reporta el resultado calculado como la resta de los códigos ASCII de los caracteres diferentes.
 */
int my_strcmp(const char *str1, const char *str2){
    while (*str1 == *str2 && *str1 && *str2){
        str1++;
        str2++;
    }
    if (*str1==*str2){
        return 0;
    } else{
        return *str1 - *str2;
    } 
}

/*
copia la cadena apuntada por src (con el carácter de terminación ‘\0’) en la memoria apuntada por dest.
Devuelve el puntero dest. No devuelve error.
*/
char *my_strcpy(char *dest, const char *src){
    int i=0;
    while(*(src + i)){
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i)= '\0';
    return dest;
}

/*
copia n caracteres de la cadena apuntada por src (con el carácter de terminación ‘\0’) en la memoria apuntada por dest.
Devuelve el puntero dest. No devuelve error.
En caso de que strlen(src) < n, el resto de la cadena apuntada por dest ha de ser rellenado con 0s. 
*/
char *my_strncpy(char *dest, const char *src, size_t n){
    int i;
    for (i = 0; i < n; i++){
        *(dest + i) = *(src + i);
    }
    return dest;
}

/*
añade la cadena apuntada por src (terminada con el carácter nulo) a la cadena apuntada por dest (también terminada con el carácter nulo).
El primer carácter de src sobreescribe el carácter nulo de dest. 
Devuelve el puntero dest. No devuelve error.
*/
char *my_strcat(char *dest, const char *src){
    int i=0;
    while (*(dest + i)){ 
        i++;
    }
    my_strcpy((dest+i), src);
    return dest;
}

/*
escanea la cadena apuntada por str (terminada con el carácter nulo) buscando la primera ocurrencia del carácter c. 
Devuelve el puntero a la primera ocurrencia del carácter c en la cadena str o NULL si el carácter no se encuentra. No devuelve error.
*/
char *my_strchr(const char *s, int c){
    if(!*s){
        return NULL;
    }
    while (*s++){ // recorremos la cadena hasta encontrar el carácter c
        if(*s==c){
            return (char*) s;  // devolvemos el puntero a la cadena
        }
    }
    return NULL; // no se ha encontrado el carácter
}


// FUNCIONES DE LA PILA

/*
Reserva espacio para una variable de tipo struct my_stack, que contendrá el puntero top al 
nodo superior de la pila y el tamaño de los datos size. Inicializa top con NULL, como valor 
del puntero al nodo superior de la pila, y size con el tamaño de datos que nos pasan como parámetro.
Devuelve un puntero a la pila inicializada (struct my_stack).
*/
struct my_stack *my_stack_init(int size){

    struct my_stack *init;

    init = malloc(sizeof(struct my_stack));
    if (init == NULL){
        return EXIT_FAILURE;
    }
    init->size = size;
    init->top = NULL;

    return init;
}

/*
Inserta un nuevo nodo en los elementos de la pila (hay que reservar espacio de memoria 
para él). El puntero a los datos de ese nodo (data) nos lo pasan como parámetro .
*/
int my_stack_push(struct my_stack *stack, void *data){
    struct my_stack_node *nuevo_nodo;

    //reservamos espacio para el nodo
    nuevo_nodo = malloc(sizeof(struct my_stack_node));

    if (nuevo_nodo == NULL){ // comprobamos si hay espacio
        // Añadimos el nuevo nodo a la pila
        // el nuevo nodo apunta al que apuntaba top
        nuevo_nodo->data = data;
        nuevo_nodo->next = stack->top;
        // hacemos que top apunte al nuevo nodo 
        stack->top = nuevo_nodo;

        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/*
Elimina el nodo superior de los elementos de la pila (y libera la memoria que ocupaba ese nodo!). 
Devuelve el puntero a los datos del elemento eliminado. 
Si no existe nodo superior (pila vacía), retorna NULL.
*/
void *my_stack_pop(struct my_stack *stack){
    if (stack){ 
        struct my_stack_node *nodo_aux = stack->top; 
        void *data = nodo_aux->data;
        stack->top = nodo_aux->next;
        free(nodo_aux);
        return data;
    }
    // si no hay elementos que eliminar devolvemos NULL
    return NULL; 
}

/*
Recorre la pila y retorna el número de nodos totales que hay en los elementos de la pila.
*/
int my_stack_len(struct my_stack *stack){
    struct my_stack_node *nodo_aux = stack->top; 
    int i=1; // empezamos por el primer elemento
    if(stack == NULL){ // si no hay elementos en la pila
        return 0;
    }

    while (nodo_aux->next){ // 
        nodo_aux = nodo_aux->next; // avanzamos
        i++;
    }

    return i;
}

/*
Recorre la pila liberando la memoria que habíamos reservado para cada uno de los datos (data)
y la de cada nodo. Finalmente libera también la memoria que ocupa el struck my_stack. Es decir, 
toda la memoria que se reservó con malloc() en algún momento, se libera con la función free(). 
Devuelve el número de bytes liberados. 
*/
int my_stack_purge(struct my_stack *stack){
    struct my_stack_node *nodo_aux = stack->top;
    int num_bytes = 0;
    while (nodo_aux){ 
        nodo_aux = stack->top;
        stack->top = nodo_aux->next;
        free(nodo_aux->data);
        free(nodo_aux);
        num_bytes += sizeof(struct my_stack_node); 
    }

    free(stack);
    num_bytes += sizeof(struct my_stack);
    return num_bytes; 
}
struct my_stack *my_stack_read(char *filename){

}
int my_stack_write(struct my_stack *stack, char *filename){

}