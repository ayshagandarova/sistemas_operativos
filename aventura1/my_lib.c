#include "my_lib.h"

/*
Calcula el nº de bytes de la cadena apuntada por str, sin incluir el carácter nulo de terminación ‘\0’ 
Devuelve la longitud de la cadena apuntada por str. No devuelve error.
*/
size_t my_strlen(const char *str){
    size_t len = 0;
    while (*str++) { // comprueba el caracter y avanza a la siguiente posición
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

}

/*
copia la cadena apuntada por src (con el carácter de terminación ‘\0’) en la memoria apuntada por dest.
Devuelve el puntero dest. No devuelve error.
*/
char *my_strcpy(char *dest, const char *src){

}

/*
copia n caracteres de la cadena apuntada por src (con el carácter de terminación ‘\0’) en la memoria apuntada por dest.
Devuelve el puntero dest. No devuelve error.
En caso de que strlen(src) < n, el resto de la cadena apuntada por dest ha de ser rellenado con 0s. 
*/
char *my_strncpy(char *dest, const char *src, size_t n){

}

/*
añade la cadena apuntada por src (terminada con el carácter nulo) a la cadena apuntada por dest (también terminada con el carácter nulo).
El primer carácter de src sobreescribe el carácter nulo de dest. 
Devuelve el puntero dest. No devuelve error.
*/
char *my_strcat(char *dest, const char *src){

}

/*
escanea la cadena apuntada por str (terminada con el carácter nulo) buscando la primera ocurrencia del carácter c. 
Devuelve el puntero a la primera ocurrencia del carácter c en la cadena str o NULL si el carácter no se encuentra. No devuelve error.
*/
char *my_strchr(const char *s, int c){

}