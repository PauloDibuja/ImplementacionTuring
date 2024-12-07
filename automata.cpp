/*
Implementación de una máquina de Turing en C++

Paulo Lara

IMPLMENTADO PARA EL TRABAJO NÚMERO 3 DE TEORÍA (No aplicable a los requerimientos del trabajo final)
*/


#include <iostream>
#include <wchar.h>
#include <locale.h>
#include <string.h>

using namespace std;

#define LARGO_CINTA 50

// 1. Implementación de Lista Enlazada para cualquier tipo de variable
template <typename TIPO>
struct Node{
    TIPO dato;
    Node* siguiente;
    Node(TIPO value) : dato(value), siguiente(nullptr) {}
};

template <typename TIPO>
class Lista{
    private:
        Node<TIPO>* cabecera;
    
    public:
        Lista() : cabecera(nullptr) {}

        int largo_lista(){
            if(!cabecera){
                return 0;
            }
            
            Node<TIPO>* nodo_temp = cabecera;
            int largo = 1;
            while(nodo_temp->siguiente){
                nodo_temp = nodo_temp->siguiente;
                largo++;
            }
            return largo;
        }

        bool hay_elemento(TIPO dato){
            if(!cabecera) return false;
            Node<TIPO>* node_temp = cabecera;
            while(node_temp){
                if(node_temp->dato == dato) return true;
                node_temp = node_temp->siguiente;
            }
            return false;
        }

        void agregar_al_inicio(TIPO dato){
            Node<TIPO>* nuevo_nodo = new Node<TIPO>(dato);
            nuevo_nodo->siguiente = cabecera;
            cabecera = nuevo_nodo;
        }

        void agregar_al_final(TIPO dato){
            Node<TIPO>* nuevo_nodo = new Node<TIPO>(dato);
            if(!cabecera){
                cabecera = nuevo_nodo;
                return;
            }
            Node<TIPO>* nodo_temp = cabecera;
            while(nodo_temp->siguiente){
                nodo_temp = nodo_temp->siguiente;
            }
            nodo_temp->siguiente = nuevo_nodo;
        }

        void agregar_en_posicion(TIPO dato, int posicion){
            if(0 > posicion || posicion >= largo_lista()){
                cout << "Error de índice: Valor fuera de rango" << endl;
                return;
            }
            Node<TIPO>* nuevo_nodo = new Node<TIPO>(dato);
            Node<TIPO>* nodo_temp = cabecera;

            if(posicion == 0) agregar_al_inicio(dato);

            for (int i = 0; i < posicion && nodo_temp; i++)
            {
                nodo_temp = nodo_temp->siguiente;
            }
            nuevo_nodo->siguiente = nodo_temp->siguiente;
            nodo_temp->siguiente = nuevo_nodo;
        }

        void cambiar_valor_en_posicion(TIPO dato, int posicion){
            if(0 > posicion || posicion >= largo_lista()){
                cout << "Error de índice: Valor fuera de rango" << endl;
                return;
            }
            Node<TIPO>* nuevo_nodo = new Node<TIPO>(dato);
            Node<TIPO>* nodo_temp = cabecera;

            for (int i = 0; i < posicion && nodo_temp; i++)
            {
                nodo_temp = nodo_temp->siguiente;
            }
            nodo_temp->dato = dato;
        }

        TIPO* devolver_valor_en_posicion(int posicion){
            if(0 > posicion || posicion >= largo_lista()){
                cout << "Error de índice: Valor fuera de rango" << endl;
                return NULL;
            }
            Node<TIPO>* nodo_temp = cabecera;

            for (int i = 0; i < posicion && nodo_temp; i++)
            {
                nodo_temp = nodo_temp->siguiente;
            }
            return &(nodo_temp->dato);
        }

        void mostrar(){
            if(!cabecera){
                cout << "< Lista vacía >" << endl;
                return;
            }

            Node<TIPO>* nodo_temp = cabecera;
            cout << "< ";
            while(nodo_temp){
                cout << nodo_temp->dato;
                nodo_temp = nodo_temp->siguiente;
                if(nodo_temp) cout << " - ";  
            }
            cout << " ..." << endl;
        }
};

// 2. Estructuras y atributos del automata
enum DESPLAZAMIENTOS{
    I = -1,
    D = 1,
    P = 0
};

typedef struct transition_data{
    // Entrada
    int estado_leido;
    char simbolo_leido;
    // Salida
    int estado_resultante;
    char simbolo_escribir_cinta;
    int desplazamiento;

    // Sobrecargar operador para imprimirlo bien;
    friend ostream& operator<<(ostream& os, const transition_data& obj);

    bool operator==(const transition_data& otro) const {
        bool resultado = true;
        resultado = resultado && estado_leido == otro.estado_leido;
        resultado = resultado && simbolo_leido == otro.simbolo_leido;
        resultado = resultado && estado_resultante == otro.estado_resultante;
        resultado = resultado && simbolo_escribir_cinta == otro.simbolo_escribir_cinta;
        resultado = resultado && desplazamiento == otro.desplazamiento;
        return (resultado);
    }
}Transition;

// 2.1 Sobrecarga de operador << para imprimir el struct Transition
ostream& operator<<(ostream& os, const Transition& obj) {
    os << "f(" << obj.estado_leido << ", " << obj.simbolo_leido << ") -> (" << obj.estado_resultante << ", " << obj.simbolo_escribir_cinta << ", " << obj.desplazamiento << ")";
    return os; // Permite el encadenamiento (os << ...)
}


// 3. Implementación Completo del autómata de Turing como clase
class Turing{
    private:
        Lista<char> cinta;
        Lista<int> estados;
        Lista<char> alfabeto_entrada;
        Lista<char> alfabeto_cinta;
        int estado_inicial;
        char espacio_vacio_cinta;
        Lista<int> estados_finales;
        Lista<Transition> transiciones;

        int selector;
        int estado_actual;
    public:
        // Inicializar autómata
        Turing(char espacio_blanco_cinta){
            espacio_vacio_cinta = espacio_blanco_cinta;
            alfabeto_cinta.agregar_al_final(espacio_blanco_cinta);
            for (int i = 0; i < LARGO_CINTA; i++){
                cinta.agregar_al_final(espacio_blanco_cinta);
            }
        }
        
        // Agregar estados
        void agregar_estado(int num){
            if(estados.hay_elemento(num)){
                cout << "Ya existe un estado con ese numero entero" << endl;
                return;
            }
            estados.agregar_al_final(num);
        }

        void agregar_estado_final(int num){
            if(!estados.hay_elemento(num)){
                cout << "Estas agregando un estado final que no existe" << endl;
                return;
            }
            estados_finales.agregar_al_final(num);
        }
        
        void agregar_estado_inicial(int num){
            if(!estados.hay_elemento(num)){
                cout << "Estas agregando un estado inicial que no existe" << endl;
                return;
            }
            estado_inicial = num;
        }

        // Agregar símbolos del alfabeto de entrada
        void agregar_alfabeto_entrada(char symbol){
            if(symbol == espacio_vacio_cinta){
                cout << "Ese simbolo esta definido como espacio blanco en la cinta" << endl;
                return;
            }
            if(alfabeto_entrada.hay_elemento(symbol)){
                cout << "Ya existe ese simbolo en el alfabeto" << endl;
                return;
            }
            alfabeto_entrada.agregar_al_final(symbol);
        }

        void agregar_alfabeto_cinta(char symbol){
            if(symbol == espacio_vacio_cinta){
                cout << "Esta símbolo ya representa el espacio blanco. Elige otro." << endl;
                return;
            }
            if(alfabeto_cinta.hay_elemento(symbol)){
                cout << "Ya existe ese simbolo en el alfabeto" << endl;
                return;
            }
            alfabeto_cinta.agregar_al_final(symbol);
        }

        void agregar_transicion(int estado_entrada, char simbolo_entrada, int estado_salida, char simbolo_escribir, int desplazamiento){
            Transition nueva_transicion = {estado_entrada, simbolo_entrada, estado_salida, simbolo_escribir, desplazamiento};
            if(transiciones.hay_elemento(nueva_transicion)){
                cout << "Esa transición ya existe" << endl;
                return;
            }
            transiciones.agregar_al_final(nueva_transicion);
        }

        int ejecutar_transicion(int estado, char simbolo){
            // Buscar transicion
            int nuevo_estado;
            char nuevo_simbolo;
            int desplazamiento;
            Transition temp_transition;
            bool exists_transition = false;
            for(int i = 0; i < transiciones.largo_lista(); i++){
                temp_transition = *transiciones.devolver_valor_en_posicion(i);
                if(estado == temp_transition.estado_leido && simbolo == temp_transition.simbolo_leido){
                    nuevo_estado = temp_transition.estado_resultante;
                    nuevo_simbolo = temp_transition.simbolo_escribir_cinta;
                    desplazamiento = temp_transition.desplazamiento;
                    exists_transition = true;
                    break;
                }
            }
            if(!exists_transition) return -1;
            estado_actual = nuevo_estado;
            cinta.cambiar_valor_en_posicion(nuevo_simbolo, selector);
            if(desplazamiento == DESPLAZAMIENTOS::P) return -1;
            selector += desplazamiento;
            return 0;
        }

        // Ejecución (devuelve true, si es aceptada; false en el caso contrario)
        bool ejecutar_automata(string entrada){
            int inicio_entrada = LARGO_CINTA/2;
            selector = inicio_entrada;
            for(char& simbolo : entrada){
                if(simbolo == '\0') break;
                cinta.cambiar_valor_en_posicion(simbolo, selector);
                selector++;
            }
            selector = inicio_entrada;
            cout << "Cinta Inicial:" << endl;
            imprimir_cinta();
            cout << endl;

            estado_actual = estado_inicial;
            int ejec = 0;
            while(ejec == 0){
                ejec = ejecutar_transicion(estado_actual, *cinta.devolver_valor_en_posicion(selector));
                
            }
            return estados_finales.hay_elemento(estado_actual);
        }
        
        void imprimir_cinta(){
            cinta.mostrar();
        }

        void mostrar_automata(){
            cout << "Cinta:" << endl;
            cinta.mostrar();
            cout << endl;

            cout << "Estados:" << endl;
            estados.mostrar();
            cout << endl;

            cout << "Alfabeto de entrada:" << endl;
            alfabeto_entrada.mostrar();
            cout << endl;

            cout << "Alfabeto de cinta:" << endl;
            alfabeto_cinta.mostrar();
            cout << endl;

            cout << "Estado Inicial:" << estado_inicial << endl;
            cout << endl;

            cout << "Simbolo vacio: " << espacio_vacio_cinta << endl;
            cout << endl;

            cout << "Estados Finales:" << endl;
            estados_finales.mostrar();
            cout << endl;

            cout << "Transiciones:" << endl;
            for (int i = 0; i < transiciones.largo_lista(); i++){
                cout << *transiciones.devolver_valor_en_posicion(i) << endl;
            }
            
        }
};



int main(){
    setlocale(LC_ALL, "");

    Turing automata('.');
    
    // Estados
    automata.agregar_estado(0);
    automata.agregar_estado(1);

    // Alfabeto Entrada
    automata.agregar_alfabeto_entrada('0');
    automata.agregar_alfabeto_entrada('1');

    // Alfabeto Cinta
    automata.agregar_alfabeto_cinta('0');
    automata.agregar_alfabeto_cinta('1');

    // Estado Inicial
    automata.agregar_estado_inicial(0);

    // Estados Finales
    automata.agregar_estado_final(1);

    // Transiciones
    automata.agregar_transicion(0, '0', 0, '1', DESPLAZAMIENTOS::D);
    automata.agregar_transicion(0, '1', 0, '0', DESPLAZAMIENTOS::D);
    automata.agregar_transicion(0, 'b', 1, 'b', DESPLAZAMIENTOS::I);

    automata.ejecutar_automata("101100101");

    cout << "Cinta Final:" << endl;
    automata.imprimir_cinta();
    cout << endl;
    //automata.mostrar_automata();
    // Insertar código 
    return 0;
}