#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
  Desafio Tetris Stack — Nível Novato
  Fila circular de 5 peças com:
  - visualizar fila
  - jogar peça (dequeue) + reposição automática (enqueue)
  - inserir nova peça (enqueue manual)
*/

#define CAP 5

typedef struct {
    char tipo; // 'I','O','T','L'
    int  id;   // sequencial
} Peca;

typedef struct {
    Peca v[CAP];
    int ini;   // índice da frente
    int tam;   // quantidade atual
    int proxId;
} Fila;

// -------- util --------
static char tipos[4] = {'I','O','T','L'};

static Peca gerarPeca(int id){
    Peca p;
    p.tipo = tipos[rand()%4];
    p.id   = id;
    return p;
}

// -------- fila circular --------
static void inicializarFila(Fila *f){
    f->ini = 0; f->tam = 0; f->proxId = 0;
    for(int i=0;i<CAP;i++){ // preencher com 5 peças
        int pos = (f->ini + f->tam) % CAP;
        f->v[pos] = gerarPeca(f->proxId++);
        f->tam++;
    }
}

static bool filaVazia(const Fila *f){ return f->tam==0; }
static bool filaCheia(const Fila *f){ return f->tam==CAP; }

static bool enqueue(Fila *f, Peca p){
    if (filaCheia(f)) return false;
    int pos = (f->ini + f->tam) % CAP;
    f->v[pos] = p;
    f->tam++;
    return true;
}

static bool dequeue(Fila *f, Peca *out){
    if (filaVazia(f)) return false;
    if (out) *out = f->v[f->ini];
    f->ini = (f->ini + 1) % CAP;
    f->tam--;
    return true;
}

static void mostrarFila(const Fila *f){
    printf("\nFila de pecas (%d/%d):\n", f->tam, CAP);
    if (f->tam==0){ printf("(vazia)\n"); return; }
    for(int i=0;i<f->tam;i++){
        int idx = (f->ini + i) % CAP;
        printf("[%c %d] ", f->v[idx].tipo, f->v[idx].id);
    }
    printf("\n");
}

// -------- menu --------
static void menu(void){
    printf("\n=== TETRIS STACK — NIVEL NOVATO ===\n");
    printf("1 - Jogar peca (dequeue) [insere nova automaticamente]\n");
    printf("2 - Inserir nova peca (enqueue manual)\n");
    printf("3 - Visualizar fila\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

int main(void){
    srand((unsigned)time(NULL));

    Fila fila;
    inicializarFila(&fila);
    mostrarFila(&fila);

    int op;
    do{
        menu();
        if (scanf("%d",&op)!=1){ fflush(stdin); op=-1; }
        switch(op){
            case 1: {
                Peca jogada;
                if (dequeue(&fila, &jogada)){
                    printf("Jogou: [%c %d]\n", jogada.tipo, jogada.id);
                    // reposição automática para manter 5
                    Peca nova = gerarPeca(fila.proxId++);
                    if (!enqueue(&fila, nova)){
                        // impossível aqui, pois sempre liberamos 1 antes
                        printf("Fila cheia ao repor.\n");
                    } else {
                        printf("Reposicao automatica: [%c %d]\n", nova.tipo, nova.id);
                    }
                } else {
                    printf("Fila vazia.\n");
                }
                mostrarFila(&fila);
            } break;

            case 2: {
                Peca nova = gerarPeca(fila.proxId++);
                if (enqueue(&fila, nova)){
                    printf("Inserida nova peca: [%c %d]\n", nova.tipo, nova.id);
                } else {
                    printf("Fila cheia. Nao foi possivel inserir.\n");
                }
                mostrarFila(&fila);
            } break;

            case 3:
                mostrarFila(&fila);
                break;

            case 0:
                printf("Saindo.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }
    }while(op!=0);

    return 0;
}
