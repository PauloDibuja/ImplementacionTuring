// Implementación del autómata de Turing en lenguaje C

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


int evaluar_transicion(Transicion *table, int n, int m, int estado, char simbolo, ResultadoTupla *resultado);
void imprimir_separador(int m);
void imprimir_tabla_transiciones(Transicion *table, char simbolos[], int n, int m);
void imprimir_maquina(Transicion *table, char simbolos[], int n, int m, int num_cadenas);
void imprimir_cinta(Cinta cinta, int apuntador, int estado);
int siguiente_paso_automata(Cinta cinta, int *apuntador, int *estado, Transicion *table, int n, int m);
int ejecutar_maquina(char *cadena, Transicion *table, char simbolos[], int n, int m);


int main() {
    // Lectura de archivo
    FILE *file = fopen("entrada.txt", "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    // Estados y Símbolos
    int n, m;
    fscanf(file, "%d %d", &n, &m);

    // Alfabeto
    char alfabeto[m + 1];  // consideramos también '\0'
    for (int i = 0; i < m; i++) {
        fscanf(file, " %c", &alfabeto[i]); 
    }
    alfabeto[m] = '\0';  // Terminar la cadena con un '\0'

    // Transiciones
    Transicion transiciones[n * m];
    Transicion *temp;
    for(int t = 0; t < n * m; t++){
            temp = &transiciones[t];
            fscanf(file, "%d %s %s %s %d", &temp->estado_actual, &temp->simbolo_entrada, &temp->simbolo_a_escribir, &temp->accion, &temp->nuevo_estado);
            //printf("Transicion f(%d, %c) -> (%c, %c, %d)\n", temp->estado_actual, temp->simbolo_entrada, temp->simbolo_a_escribir, temp->accion, temp->nuevo_estado);
    }

    // Leer cadenas;
    int num_cadenas;
    fscanf(file, "%d", &num_cadenas);
    char cadenas[num_cadenas][MAX_SIMBOLOS + 1];  // Se asume un máximo de MAX_SIMBOLOS caracteres por cadena
    for (int i = 0; i < num_cadenas; i++) {
        fscanf(file, "%s", cadenas[i]);
    }

    // Fin de la lectura
    fclose(file);

    // Ejecución de las cadenas en la máquina de Turing
    printf("-----------------------------------------------------------------\n");
    for(int i = 0; i < num_cadenas; i++){
        printf("Cadena %d: %s \n", i+1, cadenas[i]);
        ejecutar_maquina(cadenas[i], transiciones, alfabeto, n, m);
    }
    return 0;
}


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

void imprimir_cinta(Cinta cinta, int apuntador, int estado){
    int i = 0;
    int fin = 0;
    while(i < MAX_SIMBOLOS + 1){
        if(apuntador == i) printf("[ %c ]", cinta[i]);
        else printf("[ %c ]", cinta[i]);
        i++;
    }
    printf(" Estado: %d", estado);
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
    //imprimir_cinta(cinta, *apuntador, *estado);
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
    imprimir_cinta(mi_cinta, apuntador, estado);
    printf("\n");
    int estado_anterior = estado;
    char caracter_anterior = mi_cinta[apuntador];
    while(estado != -1){
        /*
        printf("El estado anterior es %d\n", estado_anterior);
        printf("Caracter anterior: %c\n", caracter_anterior);
        */
        estado_anterior = estado;
        caracter_anterior = mi_cinta[apuntador];
        if(siguiente_paso_automata(mi_cinta, &apuntador, &estado, table, n, m) != 0){
            printf("Error en la evaluacion de la transicion (%d, %c)\n", estado, mi_cinta[apuntador]);
            return -1;
        }
    }
    printf("Cinta final:\n");
    imprimir_cinta(mi_cinta, apuntador, estado);

    if(estado_anterior != 0 || caracter_anterior != '-'){
        printf("%s es RECHAZADO\n", cadena);
    }else{
        printf("%s es ACEPTADO\n", cadena);
    }
    printf("-----------------------------------------------------------------\n");
    return 0;
}