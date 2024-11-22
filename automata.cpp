/*
Implementación de una máquina de Turing en C++

Paulo Lara - PauloDibuja

SIN TERMINAR
*/

#include <iostream>

using namespace std;

#define LARGO_CINTA 40

// 1. Implementación de Lista Enlazada para cualquier tipo de variable
template <typename TIPO>
struct Node{
    TIPO dato;
    Node* siguiente;
    Node(TIPO value) : dato(value), siguiente(nullptr) {}
};

template <typename TIPO>
class ListaEnlazada{
    private:
        Node<TIPO>* cabecera;
    
    public:
        ListaEnlazada() : cabecera(nullptr) {}

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

        void mostrar(){
            if(!cabecera){
                cout << "< Lista vacía >" << endl;
                return;
            }

            Node<TIPO>* nodo_temp = cabecera;
            cout << "< ";
            while(nodo_temp){
                cout << nodo_temp->dato << ",";
                nodo_temp = nodo_temp->siguiente;  
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

}Transition;


// 3. Implementación Completo del autómata de Turing como clase
class Turing{
    private:
        ListaEnlazada<char> cinta;
        ListaEnlazada<int> estados;
        ListaEnlazada<char> alfabeto_entrada;
        ListaEnlazada<char> alfabeto_cinta;
        int estado_inicial;
        char espacio_vacio_cinta;
        ListaEnlazada<int> estados_finales;
    public:
        // Inicializar autómata
        Turing(char espacio_blanco_cinta){
            espacio_vacio_cinta = espacio_blanco_cinta;
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
};

int main(){
    // Insertar código 
    return 0;
}