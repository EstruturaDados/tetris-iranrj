#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Definições Globais ---
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3
#define TIPOS_PECA "IOTL"

// Contador global para o ID único das peças
int contador_id_peca = 0;

// --- Estrutura da Peça ---
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // ID único da peça
} Peca;

// --- Estrutura da Fila Circular ---
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// --- Estrutura da Pilha ---
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; // Índice do topo da pilha (-1 se vazia)
} Pilha;

// --------------------------------------------------------------------------------------------------
// --- Funções de Suporte (Inicialização e Geração) ---
// --------------------------------------------------------------------------------------------------

/**
 * @brief Inicializa a fila circular.
 * @param f Ponteiro para a FilaCircular a ser inicializada.
 */
void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = CAPACIDADE_FILA - 1; // Fila cheia, tras aponta para o último elemento
    f->tamanho = 0; // O tamanho será atualizado no preenchimento
    // Não é necessário limpar o array, pois o preenchimento inicial fará isso.
}

/**
 * @brief Inicializa a pilha.
 * @param p Ponteiro para a Pilha a ser inicializada.
 */
void inicializarPilha(Pilha *p) {
    p->topo = -1; // Pilha vazia
}

/**
 * @brief Gera uma nova peça com um tipo aleatório e ID único.
 * @return A nova peça gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    int num_tipos = 4; // 'I', 'O', 'T', 'L'
    int indice_tipo = rand() % num_tipos;

    novaPeca.nome = TIPOS_PECA[indice_tipo];
    novaPeca.id = contador_id_peca++; // Atribui o ID atual e incrementa o contador

    return novaPeca;
}

/**
 * @brief Preenche a fila com peças iniciais.
 * @param f Ponteiro para a FilaCircular.
 */
void preencherFilaInicial(FilaCircular *f) {
    printf("Preenchendo a fila com %d pecas iniciais...\n", CAPACIDADE_FILA);
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        Peca novaPeca = gerarPeca();
        f->itens[i] = novaPeca;
    }
    f->frente = 0;
    f->tras = CAPACIDADE_FILA - 1;
    f->tamanho = CAPACIDADE_FILA;
}

// --------------------------------------------------------------------------------------------------
// --- Funções da Fila Circular (Interface) ---
// --------------------------------------------------------------------------------------------------

/**
 * @brief Verifica se a fila está vazia.
 * @param f Ponteiro para a FilaCircular.
 * @return 1 se vazia, 0 caso contrário.
 */
int filaVazia(FilaCircular *f) {
    return (f->tamanho == 0);
}

/**
 * @brief Verifica se a fila está cheia.
 * @param f Ponteiro para a FilaCircular.
 * @return 1 se cheia, 0 caso contrário.
 */
int filaCheia(FilaCircular *f) {
    return (f->tamanho == CAPACIDADE_FILA);
}

/**
 * @brief Adiciona uma peça ao final da fila (enqueue).
 * @param f Ponteiro para a FilaCircular.
 * @param peca A peça a ser adicionada.
 * @return 1 se a peça foi adicionada, 0 caso contrário (fila cheia).
 */
int enfileirar(FilaCircular *f, Peca peca) {
    if (filaCheia(f)) {
        // No contexto do problema, a fila não deve encher pois
        // tentamos manter o tamanho fixo, mas é bom ter a checagem.
        return 0; // Fila cheia
    }

    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->itens[f->tras] = peca;
    f->tamanho++;
    return 1;
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 * @param f Ponteiro para a FilaCircular.
 * @param peca_removida Ponteiro para armazenar a peça removida.
 * @return 1 se a peça foi removida, 0 caso contrário (fila vazia).
 */
int desenfileirar(FilaCircular *f, Peca *peca_removida) {
    if (filaVazia(f)) {
        return 0; // Fila vazia
    }

    *peca_removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

/**
 * @brief Obtém a peça da frente da fila sem removê-la (peek).
 * @param f Ponteiro para a FilaCircular.
 * @return A peça da frente.
 */
Peca frenteFila(FilaCircular *f) {
    // Não checa por fila vazia, assume que a função chamadora fará isso
    return f->itens[f->frente];
}

// --------------------------------------------------------------------------------------------------
// --- Funções da Pilha (Interface) ---
// --------------------------------------------------------------------------------------------------

/**
 * @brief Verifica se a pilha está vazia.
 * @param p Ponteiro para a Pilha.
 * @return 1 se vazia, 0 caso contrário.
 */
int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

/**
 * @brief Verifica se a pilha está cheia.
 * @param p Ponteiro para a Pilha.
 * @return 1 se cheia, 0 caso contrário.
 */
int pilhaCheia(Pilha *p) {
    return (p->topo == CAPACIDADE_PILHA - 1);
}

/**
 * @brief Adiciona uma peça ao topo da pilha (push).
 * @param p Ponteiro para a Pilha.
 * @param peca A peça a ser adicionada.
 * @return 1 se a peça foi adicionada, 0 caso contrário (pilha cheia).
 */
int empilhar(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) {
        return 0; // Pilha cheia
    }

    p->topo++;
    p->itens[p->topo] = peca;
    return 1;
}

/**
 * @brief Remove a peça do topo da pilha (pop).
 * @param p Ponteiro para a Pilha.
 * @param peca_removida Ponteiro para armazenar a peça removida.
 * @return 1 se a peça foi removida, 0 caso contrário (pilha vazia).
 */
int desempilhar(Pilha *p, Peca *peca_removida) {
    if (pilhaVazia(p)) {
        return 0; // Pilha vazia
    }

    *peca_removida = p->itens[p->topo];
    p->topo--;
    return 1;
}

/**
 * @brief Obtém a peça do topo da pilha sem removê-la (peek).
 * @param p Ponteiro para a Pilha.
 * @return A peça do topo.
 */
Peca topoPilha(Pilha *p) {
    // Não checa por pilha vazia, assume que a função chamadora fará isso
    return p->itens[p->topo];
}

// --------------------------------------------------------------------------------------------------
// --- Funções de Exibição (Usabilidade) ---
// --------------------------------------------------------------------------------------------------

/**
 * @brief Exibe o estado atual da fila e da pilha. (Requisito de Usabilidade)
 * @param f Ponteiro para a FilaCircular.
 * @param p Ponteiro para a Pilha.
 */
void exibirEstadoAtual(FilaCircular *f, Pilha *p) {
    printf("\n--- Estado Atual ---\n\n");

    // Exibição da Fila
    printf("Fila de pecas (Frente -> Tras): ");
    if (filaVazia(f)) {
        printf("[VAZIA]\n");
    } else {
        printf("[");
        // Itera pela fila começando da frente
        for (int i = 0; i < CAPACIDADE_FILA; i++) {
            int indice_atual = (f->frente + i) % CAPACIDADE_FILA;
            // A fila é sempre mantida cheia no contexto deste problema,
            // mas exibimos todos os 5 slots para clareza
            if (i < f->tamanho) {
                 printf(" %c %d ", f->itens[indice_atual].nome, f->itens[indice_atual].id);
            } else {
                // Se a fila não estivesse cheia e o tamanho fosse menor que a capacidade
                // printf(" --- "); 
            }
        }
        printf("]\n");
    }

    // Exibição da Pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[VAZIA]\n");
    } else {
        printf("(Topo -> Base): ");
        printf("[");
        // Itera da cima para baixo (do topo para a base)
        for (int i = p->topo; i >= 0; i--) {
            printf(" %c %d ", p->itens[i].nome, p->itens[i].id);
        }
        printf("]\n");
    }

    printf("\n--------------------\n");
}

/**
 * @brief Exibe o menu de opções.
 */
void exibirMenu() {
    printf("\nOpcoes disponiveis:\n");
    printf("Codigo | Acao\n");
    printf("-------|----------------------------------------------\n");
    printf("   1   | Jogar peca da frente da fila\n");
    printf("   2   | Enviar peca da fila para a pilha de reserva\n");
    printf("   3   | Usar peca da pilha de reserva\n");
    printf("   4   | Trocar peca da frente da fila com o topo da pilha\n");
    printf("   5   | Trocar os %d primeiros da fila com as %d pecas da pilha\n", CAPACIDADE_PILHA, CAPACIDADE_PILHA);
    printf("   0   | Sair\n");
    printf("----------------------------------------------\n");
    printf("Opcao escolhida: ");
}

// --------------------------------------------------------------------------------------------------
// --- Funções de Ações Estratégicas (Requisitos Funcionais) ---
// --------------------------------------------------------------------------------------------------

/**
 * @brief Remove a peça da frente da fila e gera uma nova peça para manter o tamanho fixo.
 * @param f Ponteiro para a FilaCircular.
 */
void jogarPeca(FilaCircular *f) {
    Peca peca_jogada;
    if (desenfileirar(f, &peca_jogada)) {
        Peca nova_peca = gerarPeca();
        enfileirar(f, nova_peca); // Adiciona a nova peça no final
        printf("\nAcao: Peca [%c %d] jogada (removida). Nova peca [%c %d] adicionada ao final da fila.\n",
               peca_jogada.nome, peca_jogada.id, nova_peca.nome, nova_peca.id);
    } else {
        printf("\nAcao: Falha ao jogar peca. Fila esta vazia (nao deve ocorrer se mantiver tamanho fixo).\n");
    }
}

/**
 * @brief Move a peça da frente da fila para o topo da pilha.
 * @param f Ponteiro para a FilaCircular.
 * @param p Ponteiro para a Pilha.
 */
void reservarPeca(FilaCircular *f, Pilha *p) {
    if (pilhaCheia(p)) {
        printf("\nAcao: Falha ao reservar. Pilha de reserva esta cheia (%d).\n", CAPACIDADE_PILHA);
        return;
    }

    Peca peca_reservar;
    if (desenfileirar(f, &peca_reservar)) {
        if (empilhar(p, peca_reservar)) {
            Peca nova_peca = gerarPeca();
            enfileirar(f, nova_peca); // Adiciona a nova peça ao final da fila
            printf("\nAcao: Peca [%c %d] reservada (movida da fila para a pilha). Nova peca [%c %d] adicionada ao final da fila.\n",
                   peca_reservar.nome, peca_reservar.id, nova_peca.nome, nova_peca.id);
        } else {
            // Caso de falha de empilhamento (já checado no topo, mas para segurança)
            printf("\nAcao: Erro interno ao empilhar. Operacao cancelada.\n");
            // Nota: Em um sistema real, aqui você teria que reverter o desenfileiramento.
        }
    } else {
        printf("\nAcao: Falha ao reservar. Fila esta vazia (nao deve ocorrer se mantiver tamanho fixo).\n");
    }
}

/**
 * @brief Remove a peça do topo da pilha, simulando seu uso.
 * @param p Ponteiro para a Pilha.
 */
void usarPecaReservada(Pilha *p) {
    Peca peca_usada;
    if (desempilhar(p, &peca_usada)) {
        printf("\nAcao: Peca reservada [%c %d] usada (removida do topo da pilha).\n", peca_usada.nome, peca_usada.id);
    } else {
        printf("\nAcao: Falha ao usar peca. Pilha de reserva esta vazia.\n");
    }
}

/**
 * @brief Troca a peça da frente da fila com a peça do topo da pilha.
 * @param f Ponteiro para a FilaCircular.
 * @param p Ponteiro para a Pilha.
 */
void trocarPecaAtual(FilaCircular *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nAcao: Falha na troca. Pilha de reserva esta vazia.\n");
        return;
    }
    // A fila é sempre considerada cheia neste modelo.

    Peca peca_fila = frenteFila(f); // Pega a peça da frente da fila
    Peca peca_pilha = topoPilha(p); // Pega a peça do topo da pilha

    // 1. Substitui na fila (mantém o índice 'frente')
    f->itens[f->frente] = peca_pilha;

    // 2. Substitui na pilha (mantém o índice 'topo')
    p->itens[p->topo] = peca_fila;

    printf("\nAcao: Troca realizada. Peca da fila [%c %d] trocada com peca da pilha [%c %d].\n",
           peca_fila.nome, peca_fila.id, peca_pilha.nome, peca_pilha.id);
}

/**
 * @brief Alterna as N primeiras peças da fila com as N peças da pilha, onde N=CAPACIDADE_PILHA.
 * @param f Ponteiro para a FilaCircular.
 * @param p Ponteiro para a Pilha.
 */
void trocaMultipla(FilaCircular *f, Pilha *p) {
    int N = CAPACIDADE_PILHA;

    // Condições: Pilha deve estar cheia (N peças) e a fila deve ter N elementos para a troca.
    // Como a fila é sempre mantida com CAPACIDADE_FILA=5, a checagem é só para a pilha.
    if (!filaCheia(f) || !pilhaCheia(p)) {
        printf("\nAcao: Falha na troca multipla. A fila deve estar cheia (%d) E a pilha deve estar cheia (%d).\n",
               CAPACIDADE_FILA, CAPACIDADE_PILHA);
        return;
    }

    // A troca é realizada entre os N primeiros da Fila e os N da Pilha.
    // A Fila Circular tem seus elementos em indices (frente, frente+1, ..., frente+N-1) % CAPACIDADE_FILA.
    // A Pilha tem seus elementos em indices (topo, topo-1, ..., 0).

    printf("\nAcao: Realizando troca em bloco das %d primeiras pecas da fila com as %d pecas da pilha.\n", N, N);

    for (int i = 0; i < N; i++) {
        // Índice da fila: Começa na frente e avança (circularmente)
        int indice_fila = (f->frente + i) % CAPACIDADE_FILA;
        // Índice da pilha: Começa na base (0) e avança (ou N-1-i a partir do topo)
        int indice_pilha = i; // i=0 é a base, i=N-1 é o topo.

        // Troca (swap)
        Peca temp = f->itens[indice_fila];
        f->itens[indice_fila] = p->itens[indice_pilha];
        p->itens[indice_pilha] = temp;
    }

    printf("Troca realizada entre os %d primeiros da fila e as %d pecas da pilha.\n", N, N);
}

// --------------------------------------------------------------------------------------------------
// --- Função Principal ---
// --------------------------------------------------------------------------------------------------

int main() {
    // Configuração inicial
    srand(time(NULL)); // Inicializa o gerador de números aleatórios para o tipo de peça

    FilaCircular fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Requisito: Inicializar a fila de peças com um número fixo de elementos.
    preencherFilaInicial(&fila);
    
    // Inicializa a pilha com 1 ou 2 peças para que não comece totalmente vazia, se desejar,
    // mas o requisito apenas pede inicialização com capacidade limitada.
    // Para o exemplo de troca múltipla funcionar, é útil preencher a pilha antes do teste 5.
    // Vamos deixar a pilha vazia para o usuário preencher via opção 2.
    // Se quiser preenchê-la para testes imediatos:
    // empilhar(&pilha, gerarPeca());
    // empilhar(&pilha, gerarPeca());
    // empilhar(&pilha, gerarPeca()); // Pilha cheia

    int opcao;

    do {
        exibirEstadoAtual(&fila, &pilha);
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada em caso de entrada não numérica
            while (getchar() != '\n');
            opcao = -1; // Valor inválido
            printf("\nEntrada invalida. Por favor, digite um numero de 0 a 5.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("\nEncerrando o Gerenciador de Pecas. Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida. Por favor, escolha uma opcao de 0 a 5.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}