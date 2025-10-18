#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
  Tetris Stack — Nível Aventureiro
  - Fila circular fixa (5) + Pilha de reserva (3)
  - Opções:
      1 Jogar peça (dequeue) + reposição automática
      2 Reservar peça (push da frente da fila) + reposição automática
      3 Usar peça reservada (pop)
      0 Sair
  - Exibir fila e pilha após cada ação
*/

#define CAP_FILA  5
#define CAP_PILHA 3

typedef struct { char tipo; int id; } Peca;

/* --------- FILA CIRCULAR --------- */
typedef struct {
    Peca v[CAP_FILA];
    int ini;     // frente
    int tam;     // quantidade
    int proxId;  // gerador sequencial de ids
} Fila;

static char TIPOS[4] = {'I','O','T','L'};

static Peca gerarPeca(int id){
    Peca p; p.tipo = TIPOS[rand()%4]; p.id = id; return p;
}

static void filaInit(Fila *f){
    f->ini = 0; f->tam = 0; f->proxId = 0;
    for (int i=0;i<CAP_FILA;i++){
        int pos = (f->ini + f->tam) % CAP_FILA;
        f->v[pos] = gerarPeca(f->proxId++);
        f->tam++;
    }
}
static bool filaVazia (const Fila *f){ return f->tam==0; }
static bool filaCheia (const Fila *f){ return f->tam== CAP_FILA; }
static bool enqueue(Fila *f, Peca p){
    if (filaCheia(f)) return false;
    int pos = (f->ini + f->tam) % CAP_FILA;
    f->v[pos] = p; f->tam++; return true;
}
static bool dequeue(Fila *f, Peca *out){
    if (filaVazia(f)) return false;
    if (out) *out = f->v[f->ini];
    f->ini = (f->ini + 1) % CAP_FILA;
    f->tam--; return true;
}
static void mostrarFila(const Fila *f){
    printf("\nFila (%d/%d): ", f->tam, CAP_FILA);
    if (f->tam==0){ printf("(vazia)\n"); return; }
    for (int i=0;i<f->tam;i++){
        int idx = (f->ini + i) % CAP_FILA;
        printf("[%c %d] ", f->v[idx].tipo, f->v[idx].id);
    }
    printf("\n");
}

/* --------- PILHA LINEAR --------- */
typedef struct {
    Peca v[CAP_PILHA];
    int topo; // -1 vazia, 0..CAP_PILHA-1
} Pilha;

static void pilhaInit(Pilha *p){ p->topo = -1; }
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

/* --------- MENU --------- */
static void menu(void){
    printf("\n=== TETRIS STACK — NIVEL AVENTUREIRO ===\n");
    printf("1 - Jogar peca (dequeue) [repor automaticamente]\n");
    printf("2 - Reservar peca (push da frente da fila) [repor automaticamente]\n");
    printf("3 - Usar peca reservada (pop)\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

/* --------- MAIN --------- */
int main(void){
    srand((unsigned)time(NULL));

    Fila fila;  filaInit(&fila);
    Pilha pilha; pilhaInit(&pilha);

    mostrarFila(&fila);
    mostrarPilha(&pilha);

    int op;
    do{
        menu();
        if (scanf("%d",&op)!=1){ fflush(stdin); op=-1; }

        switch(op){
            case 1: { // jogar
                Peca jogada;
                if (dequeue(&fila, &jogada)){
                    printf("Jogou: [%c %d]\n", jogada.tipo, jogada.id);
                    Peca nova = gerarPeca(fila.proxId++);
                    enqueue(&fila, nova);
                    printf("Reposicao: [%c %d]\n", nova.tipo, nova.id);
                } else {
                    printf("Fila vazia.\n");
                }
                mostrarFila(&fila); mostrarPilha(&pilha);
            } break;

            case 2: { // reservar
                if (pilhaCheia(&pilha)){
                    printf("Reserva cheia. Use pecas reservadas antes.\n");
                } else {
                    Peca frente;
                    if (dequeue(&fila, &frente)){
                        push(&pilha, frente);
                        printf("Reservada: [%c %d]\n", frente.tipo, frente.id);
                        Peca nova = gerarPeca(fila.proxId++);
                        enqueue(&fila, nova);
                        printf("Reposicao: [%c %d]\n", nova.tipo, nova.id);
                    } else {
                        printf("Fila vazia.\n");
                    }
                }
                mostrarFila(&fila); mostrarPilha(&pilha);
            } break;

            case 3: { // usar reserva
                Peca usada;
                if (pop(&pilha, &usada))
                    printf("Usou da reserva: [%c %d]\n", usada.tipo, usada.id);
                else
                    printf("Reserva vazia.\n");
                mostrarFila(&fila); mostrarPilha(&pilha);
            } break;

            case 0:
                printf("Saindo.\n"); break;

            default:
                printf("Opcao invalida.\n");
        }

    } while(op!=0);

    return 0;
}
