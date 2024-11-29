#include "libraries/list_library.h"

struct Lista{
    int valor;
    struct Lista *ant;
    struct Lista *sig;
};

typedef struct Lista LISTA;