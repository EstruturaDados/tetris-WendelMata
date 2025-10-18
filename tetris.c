#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
  Desafio Tetris Stack — arquivo único (tetris.c)

  BLOCO 1 — NÍVEL NOVATO (Fila circular de 5 peças)
    - struct Peca {tipo,id}
    - Fila circular com CAP_FILA=5
    - Ações: jogar (dequeue + reposição automática), inserir manual, visualizar

  BLOCO 2 — NÍVEL AVENTUREIRO (Reserva com Pilha)
    - Mantém a fila cheia (5)
    - Pilha linear CAP_PILHA=3
    - Ações: jogar, reservar (push da frente), usar reserva (pop)

  BLOCO 3 — NÍVEL MESTRE (Integração e trocas)
    - Além do Aventureiro:
      4) Trocar frente da fila com topo da pilha
      5) Trocar 3 primeiras da fila com as 3 da pilha
    - Todas as operações validam pré-condições e exibem estados
*/

/* ----------------- Modelos ----------------- */
#define CAP_FILA  5
#define CAP_PILHA 3

typedef struct { char tipo; int id; } Peca;

typedef struct {            // Fila circular
    Peca v[CAP_FILA];
    int ini;       // índice da frente
    int tam;       // quantidade atual
    int proxId;    // gerador sequencial de ids
} Fila;

typedef struct {            // Pilha linear
    Peca v[CAP_PILHA];
    int topo;      // -1 vazia; 0..CAP_PILHA-1
} Pilha;

/* ----------------- Util ----------------- */
static char TIPOS[4] = {'I','O','T','L'};

static Peca gerarPeca(int id){
    Peca p; p.tipo = TIPOS[rand()%4]; p.id = id; return p;
}

/* ----------------- Fila ----------------- */
static void filaInit(Fila *f){
    f->ini=0; f->tam=0; f->proxId=0;
    for (int i=0;i<CAP_FILA;i++){ // preenche 5 peças iniciais
        int pos = (f->ini + f->tam) % CAP_FILA;
        f->v[pos] = gerarPeca(f->proxId++);
        f->tam++;
    }
}
static bool filaVazia(const Fila *f){ return f->tam==0; }
static bool filaCheia(const Fila *f){ return f->tam==CAP_FILA; }
static bool enqueue(Fila *f, Peca x){
    if (filaCheia(f)) return false;
    int pos = (f->ini + f->tam) % CAP_FILA;
    f->v[pos] = x; f->tam++; return true;
}
static bool dequeue(Fila *f, Peca *out){
    if (filaVazia(f)) return false;
    if (out) *out = f->v[f->ini];
    f->ini = (f->ini + 1) % CAP_FILA;
    f->tam--; return true;
}
static void mostrarFila(const Fila *f){
    printf("Fila (%d/%d): ", f->tam, CAP_FILA);
    if (f->tam==0){ printf("(vazia)\n"); return; }
    for (int i=0;i<f->tam;i++){
        int idx = (f->ini + i) % CAP_FILA;
        printf("[%c %d] ", f->v[idx].tipo, f->v[idx].id);
    }
    printf("\n");
}

/* ----------------- Pilha ----------------- */
static void pilhaInit(Pilha *p){ p->topo=-1; }
static bool pilhaVazia(const Pilha *p){ return p->topo==-1; }
static bool pilhaCheia(const Pilha *p){ return p->topo==CAP_PILHA-1; }
static bool push(Pilha *p, Peca x){
    if (pilhaCheia(p)) return false;
    p->v[++p->topo] = x; return true;
}
static bool pop(Pilha *p, Peca *out){
    if (pilhaVazia(p)) return false;
    if (out) *out = p->v[p->topo];
    p->topo--; return true;
}
static void mostrarPilha(const Pilha *p){
    printf("Reserva (pilha %d/%d): ", p->topo+1, CAP_PILHA);
    if (pilhaVazia(p)){ printf("(vazia)\n"); return; }
    for (int i=p->topo;i>=0;i--) printf("[%c %d] ", p->v[i].tipo, p->v[i].id);
    printf("\n");
}

/* ----------------- Trocas (Mestre) ----------------- */
static void trocarTopoFila(Fila *f, Pilha *p){
    if (filaVazia(f) || pilhaVazia(p)){
        printf("Nao ha pecas suficientes para trocar.\n"); return;
    }
    int frente = f->ini;
    Peca tmp = f->v[frente];
    f->v[frente] = p->v[p->topo];
    p->v[p->topo] = tmp;
    printf("Trocou frente da fila com topo da pilha.\n");
}
static void trocarTres(Fila *f, Pilha *p){
    if (f->tam < 3 || p->topo != 2){
        printf("Necessario 3 na fila e 3 na pilha.\n"); return;
    }
    for (int i=0;i<3;i++){
        int idx = (f->ini + i) % CAP_FILA;
        Peca tmp = f->v[idx];
        f->v[idx] = p->v[i];
        p->v[i] = tmp;
    }
    printf("Trocou 3 primeiras da fila com as 3 da pilha.\n");
}

/* ----------------- Menus ----------------- */
static void menuNovato(void){
    printf("\n=== TETRIS STACK — NIVEL NOVATO ===\n");
    printf("1 - Jogar peca (dequeue) [repor automaticamente]\n");
    printf("2 - Inserir nova peca (enqueue manual)\n");
    printf("3 - Visualizar fila\n");
    printf("0 - Sair\nEscolha: ");
}
static void menuAventureiro(void){
    printf("\n=== TETRIS STACK — NIVEL AVENTUREIRO ===\n");
    printf("1 - Jogar peca (dequeue) [repor automaticamente]\n");
    printf("2 - Reservar peca (push da frente da fila) [repor]\n");
    printf("3 - Usar peca reservada (pop)\n");
    printf("0 - Sair\nEscolha: ");
}
static void menuMestre(void){
    printf("\n=== TETRIS STACK — NIVEL MESTRE ===\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Reservar peca (push da frente da fila)\n");
    printf("3 - Usar peca reservada (pop)\n");
    printf("4 - Trocar frente da fila com topo da pilha\n");
    printf("5 - Trocar 3 primeiras da fila com 3 da pilha\n");
    printf("0 - Sair\nEscolha: ");
}

/* ----------------- Programa ----------------- */
int main(void){
    srand((unsigned)time(NULL));

    Fila fila; filaInit(&fila);
    Pilha pilha; pilhaInit(&pilha);

    printf("Selecione o nivel (1=Novato, 2=Aventureiro, 3=Mestre): ");
    int nivel=1; if (scanf("%d",&nivel)!=1 || nivel<1 || nivel>3) nivel=1;

    int op;
    do{
        if (nivel==1) menuNovato();
        else if (nivel==2) menuAventureiro();
        else menuMestre();

        if (scanf("%d",&op)!=1){ op=-1; }

        switch(nivel){
            case 1: // NOVATO
                switch(op){
                    case 1:{ Peca j;
                        if (dequeue(&fila,&j)){
                            printf("Jogou [%c %d]\n", j.tipo, j.id);
                            enqueue(&fila, gerarPeca(fila.proxId++));
                        } else printf("Fila vazia.\n");
                        mostrarFila(&fila);
                    } break;
                    case 2:{ Peca n = gerarPeca(fila.proxId++);
                        if (enqueue(&fila,n)) printf("Inserida [%c %d]\n", n.tipo, n.id);
                        else printf("Fila cheia.\n");
                        mostrarFila(&fila);
                    } break;
                    case 3: mostrarFila(&fila); break;
                    case 0: printf("Saindo.\n"); break;
                    default: printf("Opcao invalida.\n");
                } break;

            case 2: // AVENTUREIRO
                switch(op){
                    case 1:{ Peca j;
                        if (dequeue(&fila,&j)){
                            printf("Jogou [%c %d]\n", j.tipo, j.id);
                            enqueue(&fila, gerarPeca(fila.proxId++));
                        } else printf("Fila vazia.\n");
                        mostrarFila(&fila); mostrarPilha(&pilha);
                    } break;
                    case 2:{
                        if (pilhaCheia(&pilha)) printf("Reserva cheia.\n");
                        else{
                            Peca fr;
                            if (dequeue(&fila,&fr)){
                                push(&pilha, fr);
                                enqueue(&fila, gerarPeca(fila.proxId++));
                                printf("Reservada [%c %d]\n", fr.tipo, fr.id);
                            } else printf("Fila vazia.\n");
                        }
                        mostrarFila(&fila); mostrarPilha(&pilha);
                    } break;
                    case 3:{ Peca u;
                        if (pop(&pilha,&u)) printf("Usou [%c %d]\n", u.tipo, u.id);
                        else printf("Reserva vazia.\n");
                        mostrarFila(&fila); mostrarPilha(&pilha);
                    } break;
                    case 0: printf("Saindo.\n"); break;
                    default: printf("Opcao invalida.\n");
                } break;

            case 3: // MESTRE
                switch(op){
                    case 1:{ Peca j;
                        if (dequeue(&fila,&j)){
                            printf("Jogou [%c %d]\n", j.tipo, j.id);
                            enqueue(&fila, gerarPeca(fila.proxId++));
                        } else printf("Fila vazia.\n");
                    } break;
                    case 2:{
                        if (pilhaCheia(&pilha)) printf("Reserva cheia.\n");
                        else{
                            Peca fr;
                            if (dequeue(&fila,&fr)){
                                push(&pilha, fr);
                                enqueue(&fila, gerarPeca(fila.proxId++));
                                printf("Reservada [%c %d]\n", fr.tipo, fr.id);
                            } else printf("Fila vazia.\n");
                        }
                    } break;
                    case 3:{ Peca u;
                        if (pop(&pilha,&u)) printf("Usou [%c %d]\n", u.tipo, u.id);
                        else printf("Reserva vazia.\n");
                    } break;
                    case 4: trocarTopoFila(&fila,&pilha); break;
                    case 5: trocarTres(&fila,&pilha);    break;
                    case 0: printf("Saindo.\n");         break;
                    default: printf("Opcao invalida.\n");
                }
                mostrarFila(&fila); mostrarPilha(&pilha);
                break;
        }
    } while(op!=0);

    return 0;
}
