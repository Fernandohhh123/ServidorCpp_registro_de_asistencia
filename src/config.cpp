#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/config.hpp"

// Funcion para eliminar espacios en blanco al inicio y final de una cadena
char* limpiar_espacios(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

void get_config(Cli_args *config, const char *ruta) {
    FILE *archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        perror("Error al abrir /etc/cerrusv.cfg");
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        // 1. Ignorar comentarios y líneas vacías
        char *ptr = limpiar_espacios(linea);
        if (ptr[0] == '#' || ptr[0] == '\0') {
            continue;
        }

        // 2. Dividir la línea en CLAVE y VALOR usando el delimitador '='
        char *token_clave = strtok(ptr, "=");
        char *token_valor = strtok(NULL, "=");

        if (token_clave && token_valor) {
            char *clave = limpiar_espacios(token_clave);
            char *valor = limpiar_espacios(token_valor);

            // --- ESPACIO PARA TU LÓGICA DE IMPLEMENTACIÓN ---

            if (strcmp(clave, "IP_SERVER") == 0) {
                // Tu código aquí para IP_SERVER
                printf("Configurando IP: %s\n", valor);
            }
            else if (strcmp(clave, "PORT_SERVER") == 0) {
                // Tu código aquí para PORT_SERVER
                printf("Configurando Puerto: %s\n", valor);
		config -> puerto = atoi(valor);
            }
            // Puedes agregar más opciones aquí sin tocar la lógica de lectura

            // ------------------------------------------------
        }
    }

    fclose(archivo);
}
