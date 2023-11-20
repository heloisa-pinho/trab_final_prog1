#include "stdlib.h"
#include "stdio.h"
#include "lef.h"

/*Cria os eventos com os parametros informados
 *Retorna um ponteiro para o evento ou NULL se falhar*/
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2){

    struct evento_t *evento;
   
    evento = malloc(sizeof(struct evento_t));
   
    if (!evento)
        return NULL;
   
    evento->tempo = tempo;
    evento->tipo = tipo;
    evento->dado1 = dado1;
    evento->dado2 = dado2;
       
    return evento;
}

/*Destroi um evento e retorna NULL.*/  
struct evento_t *destroi_evento (struct evento_t *e){
   
    free(e);
    e = NULL;
    return e;
}

/*Cria Nodo Cabeça e retorna NUll se nao foi alocado */
struct lef_t *cria_lef (){

    struct lef_t *lef;
   
    lef = malloc(sizeof(struct lef_t));
    if (!lef)
        return NULL;
   
    lef->primeiro = NULL;
   
    return lef;
}

/*Desaloca toda memoria da lef e faz lef receber NULL. */
struct lef_t *destroi_lef (struct lef_t *l){

    struct nodo_lef_t *nodo,*aux;
   
    nodo = l->primeiro;
   
    while ( nodo != NULL){
        aux = nodo;
        nodo = nodo->prox;
        free(aux->evento);
        free (aux);
    }
    
    free(l);
    l = NULL;
    return l;
}

/*Insere o evento apontado na LEF na ordem de tempos crescentes.
 *Caso sejam iguais respeitam FIFO
 *Retorna 1 em caso de sucesso ou 0 caso contrario.*/
int insere_lef (struct lef_t *l, struct evento_t *e){

    struct nodo_lef_t *novo,*aux;
   
    novo = malloc(sizeof(struct nodo_lef_t));
   
    if (!novo)
        return 0;
       
    novo->evento = e;
    aux = l->primeiro;

    /*fila fazia*/
    if (vazia_lef(l)){
        l->primeiro = novo;
        novo->prox = NULL;
        return 1;
    }
   
    /*inserir o menor tempo na fila*/
    if(e->tempo < aux->evento->tempo){
        novo->prox = aux;
        l->primeiro = novo;
        return 1;
    }
   
    while (aux->prox != NULL && aux->prox->evento->tempo <= novo->evento->tempo)
        aux = aux->prox;
       
    novo->prox = aux->prox;
    aux->prox = novo;
    return 1;    
}

/* Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.*/
struct evento_t *retira_lef (struct lef_t *l){

    struct nodo_lef_t *aux;
    struct evento_t *e;
   
    if (vazia_lef(l))
        return NULL;
       
    e = l->primeiro->evento;  
    aux = l->primeiro;
    l->primeiro = aux->prox;
    free(aux);
       
    return e;
}

/*Retorna 1 se fila estiver vazia, caso contrario retorna 0*/
int vazia_lef (struct lef_t *l){

    if (l->primeiro == NULL)
        return 1;
   
    return 0;
}    
   
void imprime_lef (struct lef_t *l){

    int evento;
    struct evento_t *e;
    struct nodo_lef_t *aux;
   
    evento = 0;
    aux = l->primeiro;
    
    while (aux != NULL){
        e = aux->evento;
        printf("tempo %d tipo %d d1 %d d2 %d\n", e->tempo,e->tipo,e->dado1,e->dado2);
        aux = aux->prox;
        evento++;
    }
   
    printf("total %d eventos\n", evento);
}
