#include "fila.h"

Lista* criar (void)
{
    return NULL;
}


Lista* inserir (Lista* lista, int label, int x, int y)
{
    Lista* novo =(Lista*)malloc(sizeof(Lista));
    novo->label = label;
    novo->cord_x = x;
    novo->cord_y = y;
    novo->next = lista;
    return novo;
}


void imprimir (Lista *lista)
{
    Lista *v; /*var. para percorrer a lista*/
    for (v = lista; v != NULL; v = v->next)
    {
        printf("Valor: %d \ %d - %d\n", v->cord_x, v->cord_y, v->label);
    }
}
Lista* remover (Lista *lista, int elem)
{
    Lista* prv = NULL;
    Lista* v = lista;
    while ((v != NULL) && (v->label != elem))
    {
        prv = v;
        v = v->next;
    }
    if (v == NULL)
    {
        return lista;
    }
    if (prv == NULL)
    {
        lista = v->next;
    }
    else
    {
        prv->next = v->next;
    }
    free (v);
    return lista;
}


void liberar (Lista *lista)
{
    while (lista != NULL)
    {
        Lista *aux = lista->next; /*guarda ref. p/ prox.*/
        free (lista); /*libera a memoria apontada por v*/
        lista = aux; /*faz v apontar p/ o prox. elem.*/
    }
}

