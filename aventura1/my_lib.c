#include "my_lib.h"

//**********************************************************************
//                             RETO 1
//**********************************************************************

// Calcula el nº de bytes de la cadena apuntada por str, sin incluir el carácter nulo de terminación ‘\0’ 
size_t my_strlen(const char *str){
    size_t len = 0;
    while (*str++) { // comprueba si no es el último carácter '\0'
       len++;
    }
    return len; // Devuelve la longitud de la cadena apuntada por str. 
}

 /*  Compara las cadenas apuntadas por str1 y str2 y devuelve un entero:
    < 0 indica que str1 < str2.
    > 0 indica que str2 < str1.
    = 0 indica que str1 = str2.              */
int my_strcmp(const char *str1, const char *str2){
    while (*str1 == *str2 && *str1 && *str2){  // compara carácter a carácter
        str1++;
        str2++;
    }  // sale del bucle si ha acabado de leer las dos cadenas, si una de las dos ha acabado
       // o si los carácteres no coinciden
    if (*str1==*str2){ // se compara que los dos tengan '\0' 
        return 0; //iguales
    } else{
        return *str1 - *str2; // si str1 > str2 devuelve > 0 y si st1 < str2 devuelve < 0
    } 
}

// Copia la cadena apuntada por src (con el carácter de terminación ‘\0’) en la memoria apuntada por dest.
char *my_strcpy(char *dest, const char *src){
    int i=0;
    while(*(src + i)){ // recorremos la cadena con la variable auxiliar 'i'
        *(dest + i) = *(src + i); // rellenamos dest con src
        i++;
    }
    *(dest + i)= '\0'; // añadimos el carácter de terminación '\0'
    return dest;
}

// Copia n caracteres de la cadena apuntada por src (con el carácter de terminación ‘\0’) en la memoria apuntada por dest.
// En caso de que strlen(src) < n, el resto de la cadena apuntada por dest ha de ser rellenado con 0s. 

// h o l a \0
//n= 3 
//i:    0 1 2 3  4  5 
//dest: h o l a \0
char *my_strncpy(char *dest, const char *src, size_t n){
    int i;
    for (i = 0; i < n && *(src + i); i++){ // mientras src no sea '\0' y i sea inferior a n
        *(dest + i) = *(src + i);
    }
    // si i!<n entonces quedan carácteres rellenar porque la longitud de src es inferior a n
    if (i == (n-1)){
        while (i<n){
            *(dest + i) = 0; // rellenamos con 0 
        }
    }
    return dest;
}

// Añade la cadena apuntada por src (terminada con el carácter nulo) a la cadena apuntada por dest (también terminada con el carácter nulo).
// El primer carácter de src sobreescribe el carácter nulo de dest. 
char *my_strcat(char *dest, const char *src){
    int i=0;
    while (*(dest + i)){ 
        i++;
    }
    my_strcpy((dest+i), src);  // pasamos la posición donde se encuentra el carácter nulo de dest
    return dest;
}


// Escanea la cadena apuntada por str (terminada con el carácter nulo) buscando la primera ocurrencia del carácter c. 
// Devuelve el puntero a la primera ocurrencia del carácter c en la cadena str o NULL si el carácter no se encuentra. 
char *my_strchr(const char *s, int c){
    for (int i = 0; *(s + i); i++){
        if(*(s + i) == c){
            return (char*) (s + i);
        }
    }
    return NULL; // no se ha encontrado el carácter
}


//**********************************************************************
//                             RETO 2
//**********************************************************************

// Funicón que inicializa la pila con el tamaño que le han pasado y la devuelve.
struct my_stack *my_stack_init(int size){
    struct my_stack *init;

    init = malloc(sizeof(struct my_stack));

    if (init == NULL){ // si no se ha podido reservar memoria
        perror("Error malloc, no se ha podido reservar memoria: ");
        return NULL;
    }
    init->size = size;
    init->top = NULL;

    return init;
}

// Añade un nuevo nodo con la data pasada por parámetro a la pila.
int my_stack_push(struct my_stack *stack, void *data){
    struct my_stack_node *nuevo_nodo;  // creamos el nodo

    //reservamos espacio para el nodo
    nuevo_nodo = malloc(sizeof(struct my_stack_node));

    if (nuevo_nodo != NULL){ // si se ha reservado bien el espacio
        // añadimos el nuevo nodo a la pila
        nuevo_nodo->data = data;
        nuevo_nodo->next = stack->top;  // el nuevo nodo apunta al que apuntaba top
        stack->top = nuevo_nodo;  // hacemos que top apunte al nuevo nodo 
        return 0;
    }

    perror("Error malloc, no se ha podido reservar memoria: "); //  si no se ha podido reservar memoria
    return -1;
}

// Elimina el nodo superior de la pila, liberando el espacio de memoria que ocupaba. 
void *my_stack_pop(struct my_stack *stack){
    if (stack->top !=NULL){  // si la pila no esta vacía
        // eliminamos el primer nodo
        struct my_stack_node *nodo_aux = stack->top; 
        void *data = nodo_aux->data;
        stack->top = nodo_aux->next;
        free(nodo_aux);  // liberamos el espacio de memoria del nodo.
        return data; // devolvemos el puntero a los dato del elemento eliminado
    }
    // si no hay elementos que eliminar devolvemos NULL
    return NULL; 
}


// Recorre la pila y retorna el número de nodos totales que hay en los elementos de la pila.
int my_stack_len(struct my_stack *stack){
    struct my_stack_node *nodo_aux = stack->top; 
    int i=0; 
    while (nodo_aux != NULL){ 
        nodo_aux = nodo_aux->next; 
        i++;
    }
    return i;
}

// Elimina toda la pila liberando el espacio reservado
int my_stack_purge(struct my_stack *stack){
    struct my_stack_node *nodo_aux = stack->top;
    int num_bytes = 0;
    while (nodo_aux != NULL && stack->top!=NULL){  // eliminamos todos los nodos
        num_bytes = num_bytes + sizeof(struct my_stack_node) + stack->size;
        stack->top = nodo_aux->next;
        free(nodo_aux);
        nodo_aux = stack->top;
    }
    num_bytes += sizeof(struct my_stack);
    free(stack);    // liberamos el espacio de la pila
    return num_bytes;  // devuelve el número de bytes liberados. 
}

// Función que guarda la pila en un fichero
int my_stack_write(struct my_stack *stack, char *filename){
    struct my_stack *stackAux = my_stack_init(stack->size);
	struct my_stack_node *nodoAux = stack->top; 
	int numNodos = 0;

    // abre el fichero
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)	{ // devuelve error si ha habido algún problema al crear el fichero
        perror("Error open:");
		return -1;
	}

    int size = stack->size;
    //el primer elemento del fichero será el size de la pila
	write(fd, &size, sizeof(int)); 

    //guarda todos los nodos en la pila auxiliar
	while (nodoAux != NULL)	{ 
		my_stack_push(stackAux, nodoAux->data); 
		numNodos++;
		nodoAux = nodoAux->next; 
	}
	
    // escribimos todos los punteros a los datos de los nodos
	for (int i = 0; i < numNodos; i++)	{
		write(fd, my_stack_pop(stackAux), stackAux->size);
	}

    // cerramos el fichero
	if (close(fd) == -1) { // devuelve error si ha habido algún problema al cerrar el fichero
        perror("Error close:");
		return -1;
	}
	return numNodos; // devuelve el número de nodos escritos
}

struct my_stack *my_stack_read(char *filename){
    // Creamos el fichero
    int fd = open(filename, O_RDONLY);
	if (fd == -1)	{  // devuelve error si ha habido algún problema al abrir el fichero
        perror("Error open:");
		return NULL;
	}
    int size= 0;
	read(fd, &size, sizeof(int)); //guardamos el size en la variable size
	struct my_stack *stack = my_stack_init(size);  // creamos la pila
    
    void *data = malloc(size); // reservamos el espacio de memoria para los datos
	while (read(fd, data, size) >0) {  
		my_stack_push(stack, data);	// creamos el nodo
		data = malloc(size);
	}
	if (close(fd) == -1)	{ // devuelve error si ha habido algún problema al cerrar el fichero
        perror("Error close:");
		return NULL;
	}
	return stack; // devuelve la pila
}

