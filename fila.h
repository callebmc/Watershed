#include <stdio.h>
#include <stdlib.h>

//FUN��ES PARA FILA

typedef struct node {
    int label;
    int cord_x;
    int cord_y;
    struct node* next;
} Lista;

Lista* criar (void);

Lista* inserir (Lista* lista, int label, int x, int y);

void imprimir (Lista *lista);

Lista* remover (Lista *lista, int elem);

void liberar (Lista *lista);

