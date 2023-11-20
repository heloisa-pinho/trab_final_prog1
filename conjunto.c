#include<stdio.h>
#include<stdlib.h>
#include"conjunto.h"

/*Seleciona um numero aleatorio*/
int aleatorio (int min, int max){

    return (min + rand()%(max+1-min));
}

/*Busca um elemento no vetor*/
int busca_binaria (struct conjunto *c, int elemento){

    int ini,meio,fim;
    ini = 1;
    fim = c->card;
    meio = (ini + fim)/2;
   
    while ((ini <= fim ) && ( elemento != c->v[meio])){
        if (elemento < c->v[meio])
            fim = meio - 1;
        else
            ini = meio + 1;
        meio = (ini + fim)/2;
    }
   
    if (fim < ini)
        return 0;
       
    return meio;
}        

/*Cria um conjunto vazio e o retorna, se falhar retorna NULL*/
struct conjunto *cria_cjt (int max){

    struct conjunto *c;
    int *v;

    c = malloc(sizeof(struct conjunto));
    v = malloc((max + 1) * sizeof(int));

    if ((!c) || (!v))
       return NULL;
   
    c->max = max;
    c->card = 0;
    c->v = v;
    inicia_iterador_cjt(c);
   
    return c;
}

/*Remove todos os elementos do conjunto, libera espaco e devolve NULL.*/
struct conjunto *destroi_cjt (struct conjunto *c){
   
    while(!vazio_cjt(c)){
        retira_um_elemento_cjt(c);
    }
    
    free(c->v);
    free(c);
    c = NULL;
   
    return c;  
}

/*retorna 1 caso o conjunto esta vazio,
 *caso contrario retorna 0*/
int vazio_cjt (struct conjunto *c){

    if (c->card == 0)
        return 1;
   
    return 0;
}

/*Retorna a cardinalidade do conjunto, isto eh, o numero de elementos presentes nele.*/
int cardinalidade_cjt (struct conjunto *c){

    int cardinalidade;
   
    cardinalidade = c->card;
   
    return cardinalidade;
}

/*Insere elementos no conjunto em ordem cerscente,
 *caso seja elemento repetido nao faz nada e retorna 1,
 *em caso de sucesso retorna 1, caso contrario retorna 0*/
int insere_cjt (struct conjunto *c, int elemento){

    int i = 0;
       
    if (c->card > c->max)
        return 0;
            
    if (pertence_cjt(c,elemento))
        return 1;
           
    c->v[0] = elemento; //sentinela
    i = c->card;    
    while (elemento < c->v[i]) {
        c->v[i + 1] = c->v[i];
        i--;
    }
       
    c->card++;
    c->v[i + 1] = elemento;
   
    return 1;
}    

/*Remove um elemnto escolhido do conjunto caso ele exista.
 *Retorna 1 se a operacao foi bem sucedida e 0 se o elemento nao existe.*/
int retira_cjt (struct conjunto *c, int elemento){

    if (!pertence_cjt(c,elemento))
        return 0;
   
    int i;
     
    for (i = busca_binaria(c,elemento); i < c->card; i++)
        c->v[i] = c->v[i+1];
   
    c->card = c->card - 1;
   
    return 1;    
}

/*Retorna 1 se o elemento pertence ao conjunto e 0 caso contrario.*/
int pertence_cjt (struct conjunto *c, int elemento){

    if (busca_binaria(c,elemento) != 0)
        return 1;
   
    return 0;
}

/*Retorna 1 se o conjunto c1 esta contido no conjunto c2 e 0 caso contrario.*/
int contido_cjt (struct conjunto *c1, struct conjunto *c2){

    int i,j;
    i = 1;
    j = 1;
     
    if (c1->card > c2->card)
        return 0;
       
    while ((i <= c1->card) && ( j <= c2->card)) {
        if (c1->v[i] < c2->v[j])
            return 0;
        if (c1->v[i] > c2->v[j])
            j++;
        else{
            i++;
            j++;
        }
    }
   
    if (i > c1->card)
        return 1;
   
    return 0;
}

/*Retorna 1 se o conjunto c1 eh igual ao conjunto c2 e 0 caso contrario.*/       
int sao_iguais_cjt (struct conjunto *c1, struct conjunto *c2){

    int i = 1;
   
    if (c1->card != c2->card)
        return 0;
    
    while ((i <= c1->card) && (pertence_cjt(c1,c2->v[i]))){
        i++;
    }
   
    if (i > c1->card)
        return 1;
   
    return 0;
}

/*Cria e retorna o conjunto diferenca entre c1 e c2, nesta ordem.
 *Retorna NULL se a operacao falhou.*/   
struct conjunto *diferenca_cjt (struct conjunto *c1, struct conjunto *c2){

    int i,j,k;
    struct conjunto *dif;
   
    dif = cria_cjt(c1->card);
   
    i = 1;
    j = 1;
    k = 0;
   
    while ((i <= c1->card) && (j <= c2->card)){
        if (c1->v[i] < c2->v[j]){
            k++;
            dif->v[k] = c1->v[i];
            i++;
        }
        else if (c1->v[i] > c2->v[j])
            j++;
        else{ //iguais
            i++;
            j++;
        }
    }      
     
    /* se ainda tem elementos no c1*/
    for(j = i; j <=c1->card; j++){
        k++;
        dif->v[k] = c1->v[j];
    }
       
    dif->card = k;
    return dif;
}                            

/*Cria e retorna o conjunto interseccao entre os conjuntos c1 e c2.
 *Retorna NULL se a operacao falhou.*/       
struct conjunto *interseccao_cjt (struct conjunto *c1, struct conjunto *c2){

    int i;
    struct conjunto *inter;  
   
    inter = cria_cjt(c1->max);
    
    if (!inter)
        return NULL;
    
    for (i = 1; i <= c2->card; i++)
        if (pertence_cjt(c1,c2->v[i]))
            insere_cjt(inter,c2->v[i]);
   
    return inter;
}                    

/*Cria e retorna o conjunto uniao entre os conjunto c1 e c2.
 *Retorna NULL se a operacao falhou.*/           
struct conjunto *uniao_cjt (struct conjunto *c1, struct conjunto *c2){

    struct conjunto *uni;
    int i;
   
    uni = cria_cjt (c1->card + c2->card);
   
    for (i = 1; i <= c1->card; i++)
        uni->v[i] = c1->v[i];
    uni->card = c1->card;
   
    for (i = 1; i <= c2->card; i++)
        if (!pertence_cjt(uni, c2->v[i]))
            insere_cjt(uni,c2->v[i]);
   
    return uni;            
}        

/*Cria e retorna uma copia do conjunto c e NULL em caso de falha.*/  
struct conjunto *copia_cjt (struct conjunto *c){

    int i;
    struct conjunto *copia;
   
    copia = cria_cjt(c->card);
    copia->card = c->card;
   
    for (i = 1;i <= c->card; i++){
        copia->v[i] = c->v[i];
    }
   
    return copia;
}

/*Cria e retorna um subconjunto com elementos aleatorios do conjunto c.
 *Se o conjunto for vazio, retorna um subconjunto vazio.
 *Se n >= cardinalidade (c) entao retorna o proprio conjunto c.*/              
struct conjunto *cria_subcjt_cjt (struct conjunto *c, int n){

    struct conjunto *subc;
    int aleat,i;
   
    if (n >= c->card){
        subc = copia_cjt(c);
        return subc;
    }
    
    subc = cria_cjt(n);
    
    if (n){
        for (i = 0; i < n; i++){
            aleat = aleatorio(1, c->card);
        while (pertence_cjt(subc,c->v[aleat]))
            aleat = aleatorio(1, c->card);
        insere_cjt(subc, c->v[aleat]);
        }
    }
    return subc;                      
}          

/*Imprime os elementos do conjunto sempre em ordem crescente*/
void imprime_cjt (struct conjunto *c){

    int i;
    i = 1;
    if (c->card == 0){
        printf("[ ]\n");
        return;
    }
    
    printf("[ ");
    while (i <= c->card - 1){
        printf("%d ",c->v[i]);
        i++;
    }
    if (c->card != 0)
    printf("%d", c->v[c->card]);
    
    printf(" ]\n");
}        

void inicia_iterador_cjt (struct conjunto *c){

    c->ptr = 1;
}

/*A funcao retorna 0 caso o iterador ultrapasse o ultimo elemento, ou retorna
 *1 caso o iterador aponte para um elemento valido (dentro do conjunto).*/
int incrementa_iterador_cjt (struct conjunto *c, int *ret_iterador){

    if (c->ptr <= c->card){
       *ret_iterador = c->v[c->ptr];
       c->ptr = c->ptr+1;
       return 1;
    }

    return 0;
}

/*Remove o ultimo elemento*/
int retira_um_elemento_cjt (struct conjunto *c){

    int elem_rem;
   
    elem_rem = c->v[c->card];
    c->card = c->card - 1;
   
    return elem_rem;
}
