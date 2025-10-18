#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
  Tetris Stack — Nível Mestre
  Integração total entre Fila circular (5) e Pilha de reserva (3)
  Novas funções:
    4 - Trocar a peça da frente da fila com o topo da pilha
    5 - Trocar 3 primeiros da fila com as 3 da pilha
*/

#define CAP_FILA  5
#define CAP_PILHA 3

typedef struct { char tipo; int id; } Peca;

/* ===== FILA ===== */
typedef struct {
    Peca v[CAP_FILA];
    int ini; int tam; int proxId;
} Fila;

static char TIPOS[4] = {'I','O','T','L'};

static Peca gerarPeca(int id){ Peca p; p.tipo = TIPOS[rand()%4]; p.id = id; return p; }

static void filaInit(Fila *f){
    f->ini=0; f->tam=0; f->proxId=0;
    for(int i=0;i<CAP_FILA;i++){ int pos=(f->ini+f->tam)%CAP_FILA;
        f->v[pos]=gerarPeca(f->proxId++); f->tam++; }
}
static bool filaVazia(const Fila *f){ return f->tam==0; }
static bool filaCheia(const Fila *f){ return f->tam==CAP_FILA; }
static bool enqueue(Fila *f,Peca x){ if(filaCheia(f))return false; int p=(f->ini+f->tam)%CAP_FILA; f->v[p]=x; f->tam++; return true; }
static bool dequeue(Fila *f,Peca *out){ if(filaVazia(f))return false; if(out)*out=f->v[f->ini]; f->ini=(f->ini+1)%CAP_FILA; f->tam--; return true; }
static void mostrarFila(const Fila *f){
    printf("Fila (%d/%d): ",f->tam,CAP_FILA);
    for(int i=0;i<f->tam;i++){int idx=(f->ini+i)%CAP_FILA;printf("[%c %d] ",f->v[idx].tipo,f->v[idx].id);}
    printf("\n");
}

/* ===== PILHA ===== */
typedef struct { Peca v[CAP_PILHA]; int topo; } Pilha;
static void pilhaInit(Pilha *p){p->topo=-1;}
static bool pilhaVazia(const Pilha *p){return p->topo==-1;}
static bool pilhaCheia(const Pilha *p){return p->topo==CAP_PILHA-1;}
static bool push(Pilha *p,Peca x){if(pilhaCheia(p))return false; p->v[++p->topo]=x; return true;}
static bool pop(Pilha *p,Peca *out){if(pilhaVazia(p))return false; if(out)*out=p->v[p->topo]; p->topo--; return true;}
static void mostrarPilha(const Pilha *p){
    printf("Reserva (pilha %d/%d): ",p->topo+1,CAP_PILHA);
    if(pilhaVazia(p)){printf("(vazia)\n");return;}
    for(int i=p->topo;i>=0;i--)printf("[%c %d] ",p->v[i].tipo,p->v[i].id);
    printf("\n");
}

/* ===== TROCAS ===== */
static void trocarTopoFila(Fila *f,Pilha *p){
    if(filaVazia(f)||pilhaVazia(p)){printf("Nao ha peças suficientes para trocar.\n");return;}
    int frente=f->ini;
    Peca tmp=f->v[frente];
    f->v[frente]=p->v[p->topo];
    p->v[p->topo]=tmp;
    printf("Trocou a peça da frente [%c %d] com o topo [%c %d].\n",
           f->v[frente].tipo,f->v[frente].id,p->v[p->topo].tipo,p->v[p->topo].id);
}

static void trocarTres(Fila *f,Pilha *p){
    if(p->topo!=2 || f->tam<3){printf("Necessário 3 peças na fila e 3 na pilha.\n");return;}
    for(int i=0;i<3;i++){
        int idx=(f->ini+i)%CAP_FILA;
        Peca tmp=f->v[idx];
        f->v[idx]=p->v[i];
        p->v[i]=tmp;
    }
    printf("Trocadas as 3 primeiras da fila com as 3 da pilha.\n");
}

/* ===== MENU ===== */
static void menu(void){
    printf("\n=== TETRIS STACK — NIVEL MESTRE ===\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Reservar peca (push da frente da fila)\n");
    printf("3 - Usar peca reservada (pop)\n");
    printf("4 - Trocar frente da fila com topo da pilha\n");
    printf("5 - Trocar 3 primeiras da fila com 3 da pilha\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

/* ===== MAIN ===== */
1int main(void){
    srand((unsigned)time(NULL));

    Fila fila; filaInit(&fila);
    Pilha pilha; pilhaInit(&pilha);

    mostrarFila(&fila);
    mostrarPilha(&pilha);

    int op;
    do{
        menu();
        if(scanf("%d",&op)!=1){fflush(stdin); op=-1;}
        switch(op){
            case 1:{ Peca j;
                if(dequeue(&fila,&j)){
                    printf("Jogou [%c %d]\n",j.tipo,j.id);
                    enqueue(&fila,gerarPeca(fila.proxId++));
                }else printf("Fila vazia.\n");
            }break;
            case 2:{
                if(pilhaCheia(&pilha)){printf("Reserva cheia.\n");break;}
                Peca frente;
                if(dequeue(&fila,&frente)){
                    push(&pilha,frente);
                    enqueue(&fila,gerarPeca(fila.proxId++));
                    printf("Reservada [%c %d]\n",frente.tipo,frente.id);
                }else printf("Fila vazia.\n");
            }break;
            case 3:{
                Peca usada;
                if(pop(&pilha,&usada))printf("Usou [%c %d] da reserva.\n",usada.tipo,usada.id);
                else printf("Reserva vazia.\n");
            }break;
            case 4: trocarTopoFila(&fila,&pilha); break;
            case 5: trocarTres(&fila,&pilha); break;
            case 0: printf("Saindo.\n"); break;
            default: printf("Opcao invalida.\n");
        }
        mostrarFila(&fila);
        mostrarPilha(&pilha);
    }while(op!=0);
    return 0;
}
