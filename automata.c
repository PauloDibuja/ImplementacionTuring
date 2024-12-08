#include <stdio.h>
#include <stdlib.h>

#define MAX_SIMBOLOS 10

typedef char Cinta[MAX_SIMBOLOS+1];

typedef struct{
    int estado_actual;
    char simbolo_entrada;
    char simbolo_a_escribir;
    char accion;
    int nuevo_estado;
} Transicion;

typedef struct{
    char simbolo;
    char accion;
    int estado;
} ResultadoTupla;

int evaluar_transicion(Transicion *table, int n, int m, int estado, char simbolo, ResultadoTupla *resultado){
    int p = n * m;
    for(int i = 0; i < p; i++){
        if(table[i].estado_actual == estado && table[i].simbolo_entrada == simbolo){
            //printf("Transicion f(%d, %c) -> (%c, %c, %d)\n", table[i].estado_actual, table[i].simbolo_entrada, table[i].simbolo_a_escribir, table[i].accion, table[i].nuevo_estado);
            resultado->estado = table[i].nuevo_estado;
            resultado->accion = table[i].accion;
            resultado->simbolo = table[i].simbolo_a_escribir;
            return 0;
        }
    }
    printf("No se encontró una transición para la entrada (%d, %c).\n", estado, simbolo);
    return -1;
}

void imprimir_separador(int m){
    printf("|----------------");
    for (int h = 0; h < m; h++) {
        printf("|----------------");
    }
    printf("|\n");
}

void imprimir_tabla_transiciones(Transicion *table, char simbolos[], int n, int m) {
    ResultadoTupla resultado;

    // Encabezado 

    imprimir_separador(m);
    printf("| %-14s ", "Estado/Simbolo");
    for (int h = 0; h < m; h++) {
        printf("| %-14c ", simbolos[h]);
    }
    printf("|\n");
    
    imprimir_separador(m);

    // Imprimir las filas de la tabla
    for (int row = 0; row < n; row++) {
        printf("| %-14d ", row);
        for (int col = 0; col < m; col++) {
            // Evaluar si hay una transición para esta celda (0 si es verdadero, -1 es falso)
            if (evaluar_transicion(table, n, m, row, simbolos[col], &resultado) == -1) {
                printf("| %-14c ", "-");
            } else {
                printf("| %4c %4c %4d ", resultado.simbolo, resultado.accion, resultado.estado);
            }
        }
        printf("|\n");
        imprimir_separador(m);
    }
}

void imprimir_maquina(Transicion *table, char simbolos[], int n, int m, int num_cadenas){
    // Imprimir los valores leídos
    printf("Cantidad de estados: %d\n", n);
    printf("Cantidad de simbolos: %d\n", m);

    printf("Simbolos del alfabeto: ");
    int i = 0;
    while(simbolos[i] != '\0'){
        printf("%c ", simbolos[i]);
        i++;
    }
    printf("\n");
    imprimir_tabla_transiciones(table, simbolos, n, m);
    printf("\n");
    printf("Hay %d cadenas para probar\n", num_cadenas);
}

void imprimir_cinta(Cinta cinta, int apuntador){
    int i = 0;
    int fin = 0;
    while(i < MAX_SIMBOLOS + 1){
        if(apuntador == i) printf("[ %c ]", cinta[i]);
        else printf("[ %c ]", cinta[i]);
        i++;
    }
    printf("\n");

    for (int i = 0; i < apuntador; i++) {
        printf("     ");  // Espaciado: 6 caracteres por celda (incluyendo los corchetes y espacio)
    }
    printf("  ^\n");  // Apuntador alineado
}

int siguiente_paso_automata(Cinta cinta, int *apuntador, int *estado, Transicion *table, int n, int m){
    ResultadoTupla resultado;

    if(evaluar_transicion(table, n, m, *estado, cinta[*apuntador], &resultado) != 0){
        return -1;
    }

    cinta[*apuntador] = resultado.simbolo;

    switch (resultado.accion){
        case 'i':
            (*apuntador)--;
            break;
        case 'd':
            (*apuntador)++;
            break;
        default:
            break;
    }

    *estado = resultado.estado;
    return 0;
}

int ejecutar_maquina(char *cadena, Transicion *table, char simbolos[], int n, int m){
    Cinta mi_cinta;
    int estado = 0;
    int apuntador = 0;
    int fin = 0;
    for(int c = 0; c < MAX_SIMBOLOS + 1; c++){
        if(fin){
            mi_cinta[c] = simbolos[0];
            continue;
        }else if(cadena[c] == '\0' && fin == 0){
            mi_cinta[c] = simbolos[0];
            fin = 1;
            continue;
        }
        mi_cinta[c] = cadena[c];
    }
    printf("La cinta con la cadena de entrada:\n");
    //imprimir_cinta(mi_cinta, apuntador);
    printf("\n");

    while(estado != -1){
        if(siguiente_paso_automata(mi_cinta, &apuntador, &estado, table, n, m) != 0){
            printf("Error en la evaluacion de la transicion (%d, %c)\n", estado, mi_cinta[apuntador]);
            return -1;
        }
    }
    printf("Cinta final:\n");
    //imprimir_cinta(mi_cinta, apuntador);

    if(mi_cinta[apuntador] == simbolos[0]){
        printf("RECHAZADO\n");
    }else{
        printf("ACEPTADO\n");
    }
    printf("-------------------------------------\n");
    return 0;
}

int main() {
    FILE *file = fopen("entrada.txt", "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    int n, m;
    
    // Leer la cantidad de estados y símbolos
    fscanf(file, "%d %d", &n, &m);


    char alfabeto[m + 1];  // consideramos también '\0'
    for (int i = 0; i < m; i++) {
        fscanf(file, " %c", &alfabeto[i]); 
    }
    alfabeto[m] = '\0';  // Terminar la cadena con un '\0'


    Transicion transiciones[n * m];
    Transicion *temp;
    for(int t = 0; t < n * m; t++){
            temp = &transiciones[t];
            fscanf(file, "%d %s %s %s %d", &temp->estado_actual, &temp->simbolo_entrada, &temp->simbolo_a_escribir, &temp->accion, &temp->nuevo_estado);
            //printf("Transicion f(%d, %c) -> (%c, %c, %d)\n", temp->estado_actual, temp->simbolo_entrada, temp->simbolo_a_escribir, temp->accion, temp->nuevo_estado);
    }


    int num_cadenas;
    fscanf(file, "%d", &num_cadenas);

    imprimir_maquina(transiciones, alfabeto, n, m, num_cadenas);

    // Leer las cadenas a revisar y almacenarlas
    char cadenas[num_cadenas][MAX_SIMBOLOS + 1];  // Se asume un máximo de MAX_SIMBOLOS caracteres por cadena
    for (int i = 0; i < num_cadenas; i++) {
        fscanf(file, "%s", cadenas[i]);
    }
    fclose(file);

    printf("-------------------------------------\n");
    for(int i = 0; i < num_cadenas; i++){
        printf("Cadena %d: %s \n", i+1, cadenas[i]);
        ejecutar_maquina(cadenas[i], transiciones, alfabeto, n, m);
    }

    /*
    // Imprimir las cadenas leídas
    printf("\nCadenas a revisar:\n");
    for (int i = 0; i < num_cadenas; i++) {
        printf("%s\n", cadenas[i]);
    }
    */


    return 0;
}
