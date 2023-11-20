#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "lef.h"
#include "conjunto.h"
#include <math.h>

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS/6
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define NULO -1
#define EV_CHEGA 0
#define EV_ESPERA 1
#define EV_DESISTE 2
#define EV_AVISA 3
#define EV_ENTRA 4
#define EV_SAI 5
#define EV_VIAJA 6
#define EV_MISSAO 7
#define EV_FIM 8

struct coordenada {
    int x;
    int y;
};

struct heroi {
    int ID;                   /*identificacao*/
    struct conjunto *hab;      /*habilidades*/
    int pac;                  /*paciencia*/
    int vel;                  /*velocidade*/
    int exp;                  /*experiencia*/
    int base;                 /*ID da base*/
};

struct base {
    int ID;                  
    int lot;                  
    struct conjunto *presente;    
    struct fila *espera;
    struct coordenada local;
};

struct missao {
    int ID;
    struct conjunto *hab;
    struct coordenada local;
};

struct mundo {
    int herois;
    struct heroi h[N_HEROIS];
    int bases;
    struct base b[N_BASES];
    int missoes;
    struct missao ms[N_MISSOES];
    int habilidades;
    struct conjunto *hab;
    struct lef_t *linha_do_tempo;
    int tam_mundo;
    int relogio;
};

int aleat (int min, int max){

    return (min + rand()%(max+1-min));
};

int distancia_cartesiana(struct coordenada B,struct coordenada D){

    int distancia;
   
    distancia = sqrt(pow(abs(D.x - B.x),2) + pow(abs(D.y - B.y),2));
   
    return distancia;
};
   
/*criacao das estruturas*/

struct heroi cria_heroi(int ID,struct mundo m){

    struct heroi h;
   
    h.ID = ID;
    h.exp = 0;
    h.pac = aleat(0,100);
    h.vel = aleat(50,5000);
    h.hab = cria_subcjt_cjt(m.hab,aleat(1,3));
    h.base = NULO;
   
    return h;
}  

struct base cria_base(int ID){

    struct base b;
   
    b.ID = ID;
    b.local.x = aleat(0,N_TAMANHO_MUNDO - 1);
    b.local.y = aleat(0,N_TAMANHO_MUNDO - 1);
    b.lot = aleat(3,10);
    b.presente = cria_cjt(b.lot);//perguntar pq se nao colocar o mais 1 da erro no evento sai
    b.espera = fila_cria();    
       
    return b;
}

struct missao cria_missao(int ID,struct mundo *m){

    struct missao ms;
   
    ms.ID = ID;
    ms.local.x = aleat(0,N_TAMANHO_MUNDO - 1);
    ms.local.y = aleat(0,N_TAMANHO_MUNDO - 1);
    ms.hab = cria_subcjt_cjt(m->hab,aleat(6,10));  
   
    return ms;
}        

struct conjunto *cria_hab(struct mundo *m){

    struct conjunto *hab;
    int i;
   
    hab = cria_cjt(m->habilidades);
   
    for (i = 0;i < m->habilidades; i++)
        insere_cjt(hab,i);
       
    return hab;
}  
   
struct mundo cria_mundo (){

    struct mundo m;
    int i;
   
    m.habilidades = N_HABILIDADES;
    m.hab = cria_hab(&m);
   
    m.herois = N_HEROIS;
    for (i = 0; i < m.herois; i++)
        m.h[i] = cria_heroi(i,m);
   
    m.bases = N_BASES;
    for (i = 0; i < m.bases; i++)
        m.b[i] = cria_base(i);
   
    m.missoes = N_MISSOES;
    for (i = 0; i < m.missoes; i++)
        m.ms[i] = cria_missao(i,&m);
       
    m.linha_do_tempo = cria_lef();
   
    m.tam_mundo = N_TAMANHO_MUNDO;
    m.relogio = T_INICIO;
   
    return m;
}

/*Imprime*/

void imprime_heroi(struct heroi h){

    printf("Heroi %d, pac %d, velo %d, exp %d, base %d \n",h.ID,h.pac,h.vel,h.exp,h.base);
    printf(":");
    imprime_cjt(h.hab);
    printf("\n");
}

void imprime_base(struct base b){

    printf("base %d, lot %d, coordenada %d,%d \n", b.ID,b.lot,b.local.x,b.local.y);
    printf("conjuto de herois presente:");
    imprime_cjt(b.presente);
    printf("imprime fila de espera");
    fila_imprime(b.espera);
    printf("\n");
}

void imprime_missao(struct missao m){

    printf("missao %d, localizacao %d,%d \n", m.ID, m.local.x,m.local.y);
    printf("conjunto de habilidades:");
    imprime_cjt(m.hab);
    printf("\n");
}

void imprime_mundo(struct mundo *m){

    int i;
   
    printf("numero de herois:%d \n", m->herois);
    for (i = 0; i < m->herois; i++)
        imprime_heroi(m->h[i]);
    printf("\n");
   
    printf("numero de bases:%d \n", m->bases);
    for (i = 0; i < m->bases; i++)
        imprime_base(m->b[i]);
    printf("\n");
   
    printf("numero de missoes:%d \n", m->missoes);
    for (i = 0; i < m->missoes; i++)
        imprime_missao(m->ms[i]);
    printf("\n");
   
    printf("numero de habilidades %d \n",m->habilidades);
    printf("conjunto de habilidades:");
    imprime_cjt(m->hab);
    
    printf("linha do tempo:");
    imprime_lef(m->linha_do_tempo);
   
    printf("tamanho do mundo:%d\n", m->tam_mundo);
   
    printf("relogio:%d\n", m->relogio);
}

/*destroi*/

void destroi_mundo(struct mundo *m){

    int i;
   
    /*destroi conjunto heroi*/
    for (i = 0; i < m->herois; i++)
        destroi_cjt(m->h[i].hab);
   
    /*destroi conjunto e fila base*/    
    for (i = 0; i < m->bases; i++){
        destroi_cjt(m->b[i].presente);
        fila_destroi(&m->b[i].espera);
    }
   
    /*destroi conjunto missao*/
    for (i = 0; i< m->missoes; i++)
        destroi_cjt(m->ms[i].hab);
   
    /*destroi conjunto hab mundo*/  
    destroi_cjt(m->hab);
    
    destroi_lef(m->linha_do_tempo);
    
}
       
       
/*eventos*/

void cria_evento_iniciais(struct mundo *m){

    int i,base,tempo;
    struct evento_t *ev;
   
    /*para cada heroi insere um evento chega na lef*/
    for(i = 0; i < m->herois; i++){
        base = aleat(0,m->bases-1);
        tempo = aleat(0,4320); //tres dias      
        ev = cria_evento(tempo,EV_CHEGA,i,base);
        insere_lef(m->linha_do_tempo,ev);
    }
    
    /*insere os eventos missoes na lef*/
    for(i = 0; i < m->missoes; i++){
        tempo = aleat(0,T_FIM_DO_MUNDO);
        ev = cria_evento(tempo,EV_MISSAO,i,NULO);
        insere_lef(m->linha_do_tempo,ev);
    }
    
    /*cria evento fim*/
    ev = cria_evento(T_FIM_DO_MUNDO,EV_FIM,NULO,NULO);
    insere_lef(m->linha_do_tempo,ev);
    
}

void chega(int T,int H,int B,struct mundo *m){

    int esp,card,lot;
    struct evento_t *ev;
    struct base base;
    struct heroi heroi;
   
    base = m->b[B];
    heroi = m->h[H];
    m->h[H].base = B;
    card = cardinalidade_cjt(base.presente);
    lot = base.lot;

    if (( card < lot ) && (fila_vazia(base.espera)))
       esp = 1;
    else
       esp = ((heroi.pac) > (10 * fila_tamanho(base.espera)));

    if (esp){
       ev = cria_evento(T,EV_ESPERA,H,B);
       insere_lef(m->linha_do_tempo,ev);
       printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n",T,H,B,card,lot);
    }
    else{
       ev = cria_evento(T,EV_DESISTE,H,B);
       insere_lef(m->linha_do_tempo,ev);
       printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n",T,H,B,card,lot);
       
    }
}

void espera(int T,int H,int B,struct mundo *m){

    int tam;
    struct evento_t *ev;
    struct base base;
    base = m->b[B];
   
    tam = fila_tamanho(base.espera);
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",T,H,B,tam);
   
    enqueue(base.espera,H);
    ev = cria_evento(T,EV_AVISA,NULO,B);
    insere_lef(m->linha_do_tempo,ev);
}

void desiste(int T,int H,int B,struct mundo *m){

    int D;
    struct evento_t *ev;
   
    printf("%6d: DESIST HEROI %2d BASE %d\n",T,H,B);
   
    D = aleat(0,m->bases - 1);
    ev = cria_evento(T,EV_VIAJA,H,D);
    insere_lef(m->linha_do_tempo,ev);
}

void avisa(int T,int H,int B,struct mundo *m){
   
    int card,lot;
    struct evento_t *ev;
    struct base base;
    base = m->b[B];
    card = cardinalidade_cjt(base.presente);
    lot = base.lot;

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ",T,B,card,lot);
    fila_imprime(base.espera);
    printf("\n");
   
    while ((cardinalidade_cjt(base.presente) < base.lot) && (!fila_vazia(base.espera))){
       
        dequeue(base.espera,&H);
        insere_cjt(base.presente,H);
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d \n",T,B,H);
        ev = cria_evento(T,EV_ENTRA,H,B);
        insere_lef(m->linha_do_tempo,ev);
    }
}        

void entra(int T,int H,int B,struct mundo *m){

    int tpb,card,lot,T_saida;//tempo de permanecia na base
    struct heroi heroi;
    struct evento_t *ev;
    struct base base;
    base = m->b[B];
    heroi = m->h[H];
    card = cardinalidade_cjt(base.presente);
    lot = base.lot;
   
    tpb = (15 + heroi.pac * aleat(1,20));
    T_saida = T + tpb;
   
    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d \n",T,H,B,card,lot,T_saida);
   
    ev = cria_evento(T_saida,EV_SAI,H,B);
    insere_lef(m->linha_do_tempo,ev);
}

void sai(int T,int H,int B,struct mundo *m){
   
    int D,card,lot;
    struct evento_t *ev;
    struct base base;
    base = m->b[B];
    
    lot = base.lot;
    
    retira_cjt(base.presente,H);
    
    card = cardinalidade_cjt(base.presente);
    D = aleat(0,m->bases - 1);
    ev = cria_evento(T,EV_VIAJA,H,D);
    insere_lef(m->linha_do_tempo,ev);
    ev = cria_evento(T,EV_AVISA,NULO,B);
    insere_lef(m->linha_do_tempo,ev);
    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d) \n",T,H,B,card,lot);
    
}  

void viaja(int T,int H,int B,struct mundo *m){
   
    int distancia,duracao,chega;
    struct evento_t *ev;
    struct base D,B1;
    struct heroi heroi;
    D = m->b[B];
    heroi = m->h[H];
    B1 = m->b[heroi.base];
   
    distancia = distancia_cartesiana(B1.local,D.local);
    duracao = (distancia/heroi.vel);
    chega = T + duracao;
   
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d \n",T,H,B1.ID,B,distancia,heroi.vel,chega);
   
    ev = cria_evento(chega,EV_CHEGA,H,B);
    insere_lef(m->linha_do_tempo,ev);
}

int posicao_menor(int v[],int max){

    int pos_menor,i;
    
    pos_menor = 0;
    
    while ((pos_menor < max) && (v[pos_menor] == NULO))
        pos_menor++;
        
    if (pos_menor == max)
        return NULO;
        
    for (i = pos_menor + 1; i < max;i++){
        if ((v[i] < v[pos_menor]) && (v[i] != NULO))
            pos_menor = i;
    }
    
    return pos_menor;
    
}    

int encontra_base (struct mundo *m,struct missao ms,int MS,int T){
    struct conjunto *uniao,*aux;
    struct base BMP;
    int pos_menor,H,v[m->bases],i;    
    H = 0;
    
    for(i = 0;i < m->bases;i++)
        v[i] = distancia_cartesiana(ms.local,m->b[i].local);      
    
        
    pos_menor = posicao_menor(v,m->bases);    
    while (pos_menor != NULO){   
        BMP = m->b[pos_menor];
        inicia_iterador_cjt(BMP.presente);
        uniao = cria_cjt(m->habilidades);
             
        while(incrementa_iterador_cjt(BMP.presente,&H)){
            aux = uniao;
            uniao = uniao_cjt(aux,m->h[H].hab);
            aux = destroi_cjt(aux);
        }
        
        printf("%6d: MISSAO %d HAB BASE %d:",T,MS,pos_menor);
        imprime_cjt(uniao);
        
        if (contido_cjt(ms.hab,uniao)){
            uniao = destroi_cjt(uniao);
            return pos_menor;
        }
        
        v[pos_menor] = NULO;    
        uniao = destroi_cjt(uniao);
        pos_menor = posicao_menor(v,m->bases);
    } 
    return NULO;
}
   
int missao (int T,int MS,struct mundo *m){

    int BMP,H;
    struct missao ms;
    struct evento_t *ev;
    
    ms = m->ms[MS];

    printf("%6d: MISSAO %d HAB REQ: ",T,MS);
    imprime_cjt(ms.hab);

    BMP = encontra_base(m,ms,MS,T);
    
    if (BMP == NULO){
        ev = cria_evento(T + 24*60,EV_MISSAO,MS,NULO);
        insere_lef(m->linha_do_tempo,ev);
        printf("%6d: MISSAO %d IMPOSSIVEL \n",T,MS);
        return 0;
    }
    else{
        inicia_iterador_cjt(m->b[BMP].presente);
        while(incrementa_iterador_cjt(m->b[BMP].presente,&H))
            m->h[H].exp = m->h[H].exp + 1;    
        
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ",T,MS,BMP);
        imprime_cjt(m->b[BMP].presente);
        return 1;
    }
        
}

void fim(int T,int mc,int mi,struct mundo *m){

    int i;
    float prct,media; 
    
    
    prct = (float) (mc * 100.0)/m->missoes;
    media = (float) ((mi * 1.0)/mc);
    printf("%d,%d \n",mi,mc);
    printf("%6d: FIM \n", T);
       
    for (i = 0; i < m->herois; i++){
        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ",i,m->h[i].pac,m->h[i].vel,m->h[i].exp);
        imprime_cjt(m->h[i].hab);
    }
    
    printf("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO",mc,m->missoes,prct,media);
}     
         
int main (){
   
    struct mundo m;
    struct evento_t *ev;
    int FIM,missao_c,missao_i;
    
    srand (0); /* use zero, nao faca com time (0) */

    m = cria_mundo();
    cria_evento_iniciais(&m);
    missao_i = m.missoes;
    missao_c = 0;
    m.relogio = 0;
    FIM = 0;
    
    while(!FIM){
        ev = retira_lef(m.linha_do_tempo);   
        switch(ev->tipo){
        	case EV_CHEGA:
        	    m.relogio = ev->tempo;
        		chega(ev->tempo,ev->dado1,ev->dado2,&m);
        		break;
        	case EV_ESPERA:
        	    m.relogio = ev->tempo;
        		espera(ev->tempo,ev->dado1,ev->dado2,&m);
        		break;
            case EV_DESISTE:
                m.relogio = ev->tempo;
        		desiste(ev->tempo,ev->dado1,ev->dado2,&m);
        		break;
        	case EV_AVISA:
        	    m.relogio = ev->tempo;
        		avisa(ev->tempo,ev->dado1,ev->dado2,&m);
        		break;
        	case EV_ENTRA:
        	    m.relogio = ev->tempo;
        		entra(ev->tempo,ev->dado1,ev->dado2,&m);        		
        		break;
        	case EV_SAI:
        	    m.relogio = ev->tempo;
        		sai(ev->tempo,ev->dado1,ev->dado2,&m);
        		break;
        	case EV_VIAJA:
        	    m.relogio = ev->tempo;
        		viaja(ev->tempo,ev->dado1,ev->dado2,&m);
        		break;
           	case EV_MISSAO:
                m.relogio = ev->tempo;
                if (missao(ev->tempo,ev->dado1,&m))
                    missao_c++;
                else
                    missao_i++;
                break;
            case EV_FIM:
                fim(ev->tempo,missao_c,missao_i,&m);
                FIM = 1;
        }
        ev = destroi_evento(ev);
    }
   
    destroi_mundo(&m);
    return 0;
}
