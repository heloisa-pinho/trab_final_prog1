#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

/* Cria Nodo Cabeça e retorna NUll se nao foi alocado */
struct fila *fila_cria (){

    struct fila *f;
    
    f = malloc(sizeof(struct fila));

    if(!f)
        return NULL;
    
    f->ini = NULL;       
    f->fim = NULL;
    f->tamanho = 0;
    
    return f;
}

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi (struct fila **fila){

    struct nodo *f,*aux;
     
    f = (*fila)->ini;
     
    while (f != NULL){
       aux = f;
       f = f->prox;
       free(aux);
    }
    
    free (*fila);
    *fila = NULL;
}

/* Insere dado no fim fila. 
 * Retorna 1 em caso de sucesso e 0 em caso de falha. */
int enqueue (struct fila *fila, int dado){

    struct nodo *novo;
    
    novo = malloc(sizeof(struct nodo));
    
    if (!novo)
        return 0;
        
    if (fila_vazia(fila))
        fila->ini = novo;
    else
        (fila->fim)->prox = novo;
          
    novo->chave = dado;
    novo->prox = NULL;
 
    fila->fim = novo;
    fila->tamanho++;
    return 1;
}
 
/* Remove dado no inicio da fila  e retorna o elemento no parametro dado. 
 * A funcao retorna 1 em caso de sucesso e 0 no caso da fila estar vazia. */
int dequeue (struct fila *fila, int *dado){

    struct nodo *aux;
    
    if (fila_vazia(fila)) 
        return 0;
        
    aux = fila->ini;
    fila->ini = aux->prox;
    *dado = aux->chave;
    free(aux);
 
    fila->tamanho--;
       
    return 1;
}

/* Retorna o numero de elementos da fila, que pode ser 0. */
int fila_tamanho (struct fila *fila){
 
    return fila->tamanho;
}

/* Retorna 1 se fila vazia, 0 em caso contrario. */
int fila_vazia (struct fila *fila){
 
    if (fila->tamanho == 0)
        return 1;
        
    return 0;
}    

void fila_imprime (struct fila *fila){

    struct nodo *aux;
    
    aux = fila->ini;
    printf("[ ");
    
    while(aux){
        printf("%d ",aux->chave);
        aux = aux->prox;
    }
    
    printf("]");
}
