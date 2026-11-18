#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100
#define TAM_HASH 10
#define MAX_VERTICES 20

// Struct base
typedef struct {
    int id;          
    char nome[50];   
    float valor;     
} Dado;

Dado criar_dado(int id, const char* nome, float valor) {
    Dado d;
    d.id = id;
    strcpy(d.nome, nome);
    d.valor = valor;
    return d;
}

void imprimir_dado(Dado d) {
    printf("[%d | %s | %.2f]", d.id, d.nome, d.valor);
}

// Vetor
typedef struct {
    Dado elementos[MAX];
    int tam;
} Vetor;

void vet_inicializar(Vetor* v) {
    v->tam = 0;
}

int vet_inserir(Vetor* v, Dado d) {
    if (v->tam >= MAX) return 0;
    v->elementos[v->tam] = d;
    v->tam++;
    return 1;
}

int vet_buscar_por_id(Vetor* v, int id) {
    for (int i = 0; i < v->tam; i++) {
        if (v->elementos[i].id == id) return i;
    }
    return -1;
}

int vet_remover(Vetor* v, int posicao) {
    if (posicao < 0 || posicao >= v->tam) return 0;
    for (int i = posicao; i < v->tam - 1; i++) {
        v->elementos[i] = v->elementos[i + 1];
    }
    v->tam--;
    return 1;
}

int vet_atualizar(Vetor* v, int posicao, Dado novo_dado) {
    if (posicao < 0 || posicao >= v->tam) return 0;
    v->elementos[posicao] = novo_dado;
    return 1;
}

void vet_imprimir(Vetor* v) {
    printf("Vetor (%d): ", v->tam);
    for (int i = 0; i < v->tam; i++) {
        imprimir_dado(v->elementos[i]);
        printf(" ");
    }
    printf("\n");
}

// Lista
typedef struct No {
    Dado dado;
    struct No* prox;
} No;

typedef struct {
    No* inicio;
} Lista;

void lista_inicializar(Lista* l) {
    l->inicio = NULL;
}

void lista_inserir_inicio(Lista* l, Dado d) {
    No* novo = (No*) malloc(sizeof(No));
    novo->dado = d;
    novo->prox = l->inicio;
    l->inicio = novo;
}

No* lista_buscar_por_id(Lista* l, int id) {
    No* atual = l->inicio;
    while (atual != NULL) {
        if (atual->dado.id == id) return atual;
        atual = atual->prox;
    }
    return NULL;
}

int lista_remover_por_id(Lista* l, int id) {
    No* atual = l->inicio;
    No* anterior = NULL;

    while (atual != NULL && atual->dado.id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) return 0;

    if (anterior == NULL) {
        l->inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    return 1;
}

void lista_imprimir(Lista* l) {
    No* atual = l->inicio;
    printf("Lista: ");
    while (atual != NULL) {
        imprimir_dado(atual->dado);
        printf(" -> ");
        atual = atual->prox;
    }
    printf("NULL\n");
}

// Transformacoes entre vetor e lista
void vet_copiar(Vetor* origem, Vetor* destino) {
    vet_inicializar(destino);
    for (int i = 0; i < origem->tam; i++) {
        vet_inserir(destino, origem->elementos[i]);
    }
}

void lista_copiar(Lista* origem, Lista* destino) {
    lista_inicializar(destino);
    if (origem->inicio == NULL) return;

    Vetor temp;
    vet_inicializar(&temp);
    
    No* atual = origem->inicio;
    while (atual != NULL) {
        vet_inserir(&temp, atual->dado);
        atual = atual->prox;
    }

    // Insere no destino na ordem certa
    for (int i = temp.tam - 1; i >= 0; i--) {
        lista_inserir_inicio(destino, temp.elementos[i]);
    }
}

void converter_vetor_para_lista(Vetor* v, Lista* l) {
    lista_inicializar(l);
    for (int i = v->tam - 1; i >= 0; i--) {
        lista_inserir_inicio(l, v->elementos[i]);
    }
}

void converter_lista_para_vetor(Lista* l, Vetor* v) {
    vet_inicializar(v);
    No* atual = l->inicio;
    while (atual != NULL && v->tam < MAX) {
        vet_inserir(v, atual->dado);
        atual = atual->prox;
    }
}

// Pilha e Fila
// --- 4.1 Pilha com Vetor ---
typedef struct {
    Dado elementos[MAX];
    int topo;
} PilhaVetor;

void pilha_vet_inicializar(PilhaVetor* p) { p->topo = -1; }

int pilha_vet_empilhar(PilhaVetor* p, Dado d) {
    if (p->topo >= MAX - 1) return 0;
    p->elementos[++(p->topo)] = d;
    return 1;
}

Dado pilha_vet_desempilhar(PilhaVetor* p, int* sucesso) {
    Dado vazio = {0, "", 0.0};
    if (p->topo < 0) {
        *sucesso = 0;
        return vazio;
    }
    *sucesso = 1;
    return p->elementos[(p->topo)--];
}

// --- 4.2 Pilha com Lista ---
typedef struct {
    No* topo;
} PilhaLista;

void pilha_lista_inicializar(PilhaLista* p) { p->topo = NULL; }

void pilha_lista_empilhar(PilhaLista* p, Dado d) {
    No* novo = (No*) malloc(sizeof(No));
    novo->dado = d;
    novo->prox = p->topo;
    p->topo = novo;
}

Dado pilha_lista_desempilhar(PilhaLista* p, int* sucesso) {
    Dado vazio = {0, "", 0.0};
    if (p->topo == NULL) {
        *sucesso = 0;
        return vazio;
    }
    No* temp = p->topo;
    Dado d = temp->dado;
    p->topo = p->topo->prox;
    free(temp);
    *sucesso = 1;
    return d;
}

// --- 4.3 Fila com Vetor ---
typedef struct {
    Dado elementos[MAX];
    int inicio, fim, tam;
} FilaVetor;

void fila_vet_inicializar(FilaVetor* f) {
    f->inicio = 0; f->fim = 0; f->tam = 0;
}

int fila_vet_enfileirar(FilaVetor* f, Dado d) {
    if (f->tam >= MAX) return 0;
    f->elementos[f->fim] = d;
    f->fim = (f->fim + 1) % MAX;
    f->tam++;
    return 1;
}

Dado fila_vet_desenfileirar(FilaVetor* f, int* sucesso) {
    Dado vazio = {0, "", 0.0};
    if (f->tam == 0) {
        *sucesso = 0;
        return vazio;
    }
    Dado d = f->elementos[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->tam--;
    *sucesso = 1;
    return d;
}

// --- 4.4 Fila com Lista ---
typedef struct {
    No* inicio;
    No* fim;
} FilaLista;

void fila_lista_inicializar(FilaLista* f) {
    f->inicio = NULL; f->fim = NULL;
}

void fila_lista_enfileirar(FilaLista* f, Dado d) {
    No* novo = (No*) malloc(sizeof(No));
    novo->dado = d;
    novo->prox = NULL;
    if (f->fim == NULL) {
        f->inicio = novo; f->fim = novo;
    } else {
        f->fim->prox = novo; f->fim = novo;
    }
}

Dado fila_lista_desenfileirar(FilaLista* f, int* sucesso) {
    Dado vazio = {0, "", 0.0};
    if (f->inicio == NULL) {
        *sucesso = 0;
        return vazio;
    }
    No* temp = f->inicio;
    Dado d = temp->dado;
    f->inicio = f->inicio->prox;
    if (f->inicio == NULL) f->fim = NULL;
    free(temp);
    *sucesso = 1;
    return d;
}

//Arvores
// --- 5.1 Árvore Binária de Busca (Ponteiros) ---
typedef struct NoArvore {
    Dado dado;
    struct NoArvore* esq;
    struct NoArvore* dir;
} NoArvore;

typedef struct {
    NoArvore* raiz;
} ArvoreBinaria;

void arv_inicializar(ArvoreBinaria* a) { a->raiz = NULL; }

NoArvore* _arv_inserir_rec(NoArvore* atual, Dado d) {
    if (atual == NULL) {
        NoArvore* novo = (NoArvore*) malloc(sizeof(NoArvore));
        novo->dado = d;
        novo->esq = NULL; novo->dir = NULL;
        return novo;
    }
    if (d.id < atual->dado.id) atual->esq = _arv_inserir_rec(atual->esq, d);
    else if (d.id > atual->dado.id) atual->dir = _arv_inserir_rec(atual->dir, d);
    return atual;
}

void arv_inserir(ArvoreBinaria* a, Dado d) {
    a->raiz = _arv_inserir_rec(a->raiz, d);
}

NoArvore* _arv_buscar_rec(NoArvore* atual, int id) {
    if (atual == NULL || atual->dado.id == id) return atual;
    if (id < atual->dado.id) return _arv_buscar_rec(atual->esq, id);
    return _arv_buscar_rec(atual->dir, id);
}

NoArvore* arv_buscar(ArvoreBinaria* a, int id) {
    return _arv_buscar_rec(a->raiz, id);
}

void _arv_em_ordem(NoArvore* atual) {
    if (atual != NULL) {
        _arv_em_ordem(atual->esq);
        imprimir_dado(atual->dado);
        printf(" ");
        _arv_em_ordem(atual->dir);
    }
}

void arv_imprimir_em_ordem(ArvoreBinaria* a) {
    _arv_em_ordem(a->raiz);
    printf("\n");
}

// --- 5.2 Árvore Binária com Vetor ---
typedef struct {
    Dado elementos[MAX];
    int ocupado[MAX]; // 0 = vazio, 1 = ocupado
} ArvoreVetor;

void arv_vet_inicializar(ArvoreVetor* av) {
    for (int i = 0; i < MAX; i++) av->ocupado[i] = 0;
}

int arv_vet_inserir_raiz(ArvoreVetor* av, Dado d) {
    av->elementos[0] = d;
    av->ocupado[0] = 1;
    return 0;
}

int arv_vet_inserir_esquerdo(ArvoreVetor* av, int pai_idx, Dado d) {
    int filho = 2 * pai_idx + 1;
    if (filho >= MAX) return -1;
    av->elementos[filho] = d;
    av->ocupado[filho] = 1;
    return filho;
}

int arv_vet_inserir_direito(ArvoreVetor* av, int pai_idx, Dado d) {
    int filho = 2 * pai_idx + 2;
    if (filho >= MAX) return -1;
    av->elementos[filho] = d;
    av->ocupado[filho] = 1;
    return filho;
}

// Ordenacao
void vet_trocar(Dado* a, Dado* b) {
    Dado temp = *a;
    *a = *b;
    *b = temp;
}

void vet_bubble_sort(Vetor* v) {
    for (int i = 0; i < v->tam - 1; i++) {
        for (int j = 0; j < v->tam - i - 1; j++) {
            if (v->elementos[j].id > v->elementos[j + 1].id) {
                vet_trocar(&v->elementos[j], &v->elementos[j + 1]);
            }
        }
    }
}

void vet_insertion_sort(Vetor* v) {
    for (int i = 1; i < v->tam; i++) {
        Dado chave = v->elementos[i];
        int j = i - 1;
        while (j >= 0 && v->elementos[j].id > chave.id) {
            v->elementos[j + 1] = v->elementos[j];
            j--;
        }
        v->elementos[j + 1] = chave;
    }
}

void vet_selection_sort(Vetor* v) {
    for (int i = 0; i < v->tam - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < v->tam; j++) {
            if (v->elementos[j].id < v->elementos[min_idx].id) min_idx = j;
        }
        if (min_idx != i) vet_trocar(&v->elementos[i], &v->elementos[min_idx]);
    }
}

void _vet_merge(Dado arr[], int esq, int meio, int dir) {
    int n1 = meio - esq + 1, n2 = dir - meio;
    Dado L[MAX], R[MAX];
    for (int i = 0; i < n1; i++) L[i] = arr[esq + i];
    for (int j = 0; j < n2; j++) R[j] = arr[meio + 1 + j];

    int i = 0, j = 0, k = esq;
    while (i < n1 && j < n2) {
        if (L[i].id <= R[j].id) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void _vet_merge_rec(Dado arr[], int esq, int dir) {
    if (esq < dir) {
        int meio = esq + (dir - esq) / 2;
        _vet_merge_rec(arr, esq, meio);
        _vet_merge_rec(arr, meio + 1, dir);
        _vet_merge(arr, esq, meio, dir);
    }
}

void vet_merge_sort(Vetor* v) {
    if (v->tam > 1) _vet_merge_rec(v->elementos, 0, v->tam - 1);
}

int _vet_particao(Dado arr[], int baixo, int alto) {
    int pivo = arr[alto].id;
    int i = (baixo - 1);
    for (int j = baixo; j <= alto - 1; j++) {
        if (arr[j].id < pivo) {
            i++;
            vet_trocar(&arr[i], &arr[j]);
        }
    }
    vet_trocar(&arr[i + 1], &arr[alto]);
    return (i + 1);
}

void _vet_quick_rec(Dado arr[], int baixo, int alto) {
    if (baixo < alto) {
        int pi = _vet_particao(arr, baixo, alto);
        _vet_quick_rec(arr, baixo, pi - 1);
        _vet_quick_rec(arr, pi + 1, alto);
    }
}

void vet_quick_sort(Vetor* v) {
    if (v->tam > 1) _vet_quick_rec(v->elementos, 0, v->tam - 1);
}

// --- Ordenação em Lista ---
void lista_bubble_sort(Lista* l) {
    if (l->inicio == NULL) return;
    int trocado;
    No *ptr1, *lptr = NULL;
    do {
        trocado = 0;
        ptr1 = l->inicio;
        while (ptr1->prox != lptr) {
            if (ptr1->dado.id > ptr1->prox->dado.id) {
                Dado temp = ptr1->dado;
                ptr1->dado = ptr1->prox->dado;
                ptr1->prox->dado = temp;
                trocado = 1;
            }
            ptr1 = ptr1->prox;
        }
        lptr = ptr1;
    } while (trocado);
}

void lista_insertion_sort(Lista* l) {
    if (l->inicio == NULL) return;
    No* atual = l->inicio->prox;
    while (atual != NULL) {
        No* interno = l->inicio;
        while (interno != atual) {
            if (interno->dado.id > atual->dado.id) {
                Dado temp = interno->dado;
                interno->dado = atual->dado;
                atual->dado = temp;
            }
            interno = interno->prox;
        }
        atual = atual->prox;
    }
}

void lista_selection_sort(Lista* l) {
    No* i = l->inicio;
    while (i != NULL) {
        No* min = i;
        No* j = i->prox;
        while (j != NULL) {
            if (j->dado.id < min->dado.id) min = j;
            j = j->prox;
        }
        if (min != i) {
            Dado temp = i->dado;
            i->dado = min->dado;
            min->dado = temp;
        }
        i = i->prox;
    }
}

void lista_merge_sort(Lista* l) {
    Vetor v; vet_inicializar(&v);
    converter_lista_para_vetor(l, &v);
    vet_merge_sort(&v);
    converter_vetor_para_lista(&v, l);
}

void lista_quick_sort(Lista* l) {
    Vetor v; vet_inicializar(&v);
    converter_lista_para_vetor(l, &v);
    vet_quick_sort(&v);
    converter_vetor_para_lista(&v, l);
}

//Pesquisa e Hash
int vet_pesquisa_sequencial(Vetor* v, int id) {
    return vet_buscar_por_id(v, id);
}

int vet_pesquisa_binaria(Vetor* v, int id) {
    int esq = 0, dir = v->tam - 1;
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        if (v->elementos[meio].id == id) return meio;
        if (v->elementos[meio].id < id) esq = meio + 1;
        else dir = meio - 1;
    }
    return -1;
}

// --- Hash com Vetor (Tentativa Linear) ---
typedef struct {
    Dado tabela[TAM_HASH];
    int ocupado[TAM_HASH];
} HashVetor;

void hash_vet_inicializar(HashVetor* h) {
    for (int i = 0; i < TAM_HASH; i++) h->ocupado[i] = 0;
}

int _funcao_hash(int id) { return (id % TAM_HASH); }

int hash_vet_inserir(HashVetor* h, Dado d) {
    int inicio = _funcao_hash(d.id);
    int idx = inicio;
    do {
        if (!h->ocupado[idx]) {
            h->tabela[idx] = d;
            h->ocupado[idx] = 1;
            return 1;
        }
        idx = (idx + 1) % TAM_HASH;
    } while (idx != inicio);
    return 0;
}

int hash_vet_buscar(HashVetor* h, int id) {
    int inicio = _funcao_hash(id);
    int idx = inicio;
    do {
        if (!h->ocupado[idx]) return -1;
        if (h->tabela[idx].id == id) return idx;
        idx = (idx + 1) % TAM_HASH;
    } while (idx != inicio);
    return -1;
}

// --- Pesquisa e Hash com Lista ---
No* lista_pesquisa_sequencial(Lista* l, int id) {
    return lista_buscar_por_id(l, id);
}

int lista_pesquisa_binaria(Lista* l, int id) {
    Vetor v; vet_inicializar(&v);
    converter_lista_para_vetor(l, &v);
    vet_selection_sort(&v);
    return (vet_pesquisa_binaria(&v, id) != -1);
}

typedef struct {
    Lista baldes[TAM_HASH];
} HashLista;

void hash_lista_inicializar(HashLista* hl) {
    for (int i = 0; i < TAM_HASH; i++) lista_inicializar(&hl->baldes[i]);
}

void hash_lista_inserir(HashLista* hl, Dado d) {
    int idx = _funcao_hash(d.id);
    lista_inserir_inicio(&hl->baldes[idx], d);
}

No* hash_lista_buscar(HashLista* hl, int id) {
    int idx = _funcao_hash(id);
    return lista_buscar_por_id(&hl->baldes[idx], id);
}

// Grafos
// Nota: Os vértices são identificados por índices inteiros (0 a num_vertices-1).
// Se precisar associar um 'Dado' a cada vértice, basta ter um vetor Dado vertices[MAX_VERTICES].

typedef struct {
    int matriz[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
} GrafoMatriz;

void grafo_mat_inicializar(GrafoMatriz* g, int num_vertices) {
    g->num_vertices = num_vertices;
    for (int i = 0; i < num_vertices; i++)
        for (int j = 0; j < num_vertices; j++)
            g->matriz[i][j] = 0;
}

void grafo_mat_adicionar_aresta(GrafoMatriz* g, int origem, int destino) {
    if (origem >= 0 && origem < g->num_vertices && destino >= 0 && destino < g->num_vertices) {
        g->matriz[origem][destino] = 1;
        g->matriz[destino][origem] = 1; // Comente para grafo DIRECIONADO
    }
}

void grafo_mat_bfs(GrafoMatriz* g, int inicio) {
    int visitado[MAX_VERTICES] = {0};
    int fila[MAX_VERTICES], frente = 0, tras = 0;

    visitado[inicio] = 1;
    fila[tras++] = inicio;

    printf("BFS (Matriz): ");
    while (frente < tras) {
        int atual = fila[frente++];
        printf("%d ", atual);
        for (int i = 0; i < g->num_vertices; i++) {
            if (g->matriz[atual][i] == 1 && !visitado[i]) {
                visitado[i] = 1;
                fila[tras++] = i;
            }
        }
    }
    printf("\n");
}

void _grafo_mat_dfs_rec(GrafoMatriz* g, int atual, int visitado[]) {
    visitado[atual] = 1;
    printf("%d ", atual);
    for (int i = 0; i < g->num_vertices; i++) {
        if (g->matriz[atual][i] == 1 && !visitado[i]) {
            _grafo_mat_dfs_rec(g, i, visitado);
        }
    }
}

void grafo_mat_dfs(GrafoMatriz* g, int inicio) {
    int visitado[MAX_VERTICES] = {0};
    printf("DFS (Matriz): ");
    _grafo_mat_dfs_rec(g, inicio, visitado);
    printf("\n");
}

// --- Grafo com Lista de Adjacência (Usa a struct Lista da Parte 2) ---
typedef struct {
    Lista adj[MAX_VERTICES];
    int num_vertices;
} GrafoLista;

void grafo_lista_inicializar(GrafoLista* g, int num_vertices) {
    g->num_vertices = num_vertices;
    for (int i = 0; i < num_vertices; i++) lista_inicializar(&g->adj[i]);
}

void grafo_lista_adicionar_aresta(GrafoLista* g, int origem, int destino) {
    if (origem >= 0 && origem < g->num_vertices && destino >= 0 && destino < g->num_vertices) {
        lista_inserir_inicio(&g->adj[origem], criar_dado(destino, "", 0));
        lista_inserir_inicio(&g->adj[destino], criar_dado(origem, "", 0)); // Comente se DIRECIONADO
    }
}

void grafo_lista_bfs(GrafoLista* g, int inicio) {
    int visitado[MAX_VERTICES] = {0};
    int fila[MAX_VERTICES], frente = 0, tras = 0;

    visitado[inicio] = 1;
    fila[tras++] = inicio;

    printf("BFS (Lista): ");
    while (frente < tras) {
        int atual = fila[frente++];
        printf("%d ", atual);
        No* vizinho = g->adj[atual].inicio;
        while (vizinho != NULL) {
            int v_id = vizinho->dado.id;
            if (!visitado[v_id]) {
                visitado[v_id] = 1;
                fila[tras++] = v_id;
            }
            vizinho = vizinho->prox;
        }
    }
    printf("\n");
}

void _grafo_lista_dfs_rec(GrafoLista* g, int atual, int visitado[]) {
    visitado[atual] = 1;
    printf("%d ", atual);
    No* vizinho = g->adj[atual].inicio;
    while (vizinho != NULL) {
        int v_id = vizinho->dado.id;
        if (!visitado[v_id]) {
            _grafo_lista_dfs_rec(g, v_id, visitado);
        }
        vizinho = vizinho->prox;
    }
}

void grafo_lista_dfs(GrafoLista* g, int inicio) {
    int visitado[MAX_VERTICES] = {0};
    printf("DFS (Lista): ");
    _grafo_lista_dfs_rec(g, inicio, visitado);
    printf("\n");
}

//exemplo
int main() {
    // 1. Testando Vetor e Ordenação
    Vetor v;
    vet_inicializar(&v);
    vet_inserir(&v, criar_dado(30, "Carlos", 8.5));
    vet_inserir(&v, criar_dado(10, "Ana", 9.2));
    vet_inserir(&v, criar_dado(20, "Bruno", 7.0));

    printf("--- ANTES DE ORDENAR ---\n");
    vet_imprimir(&v);

    vet_quick_sort(&v);
    printf("--- DEPOIS DO QUICK SORT (POR ID) ---\n");
    vet_imprimir(&v);

    // 2. Testando Conversão para Lista e Hash
    Lista l;
    converter_vetor_para_lista(&v, &l);
    printf("--- CONVERTIDO PARA LISTA ---\n");
    lista_imprimir(&l);

    // 3. Testando Grafo
    GrafoLista gl;
    grafo_lista_inicializar(&gl, 4);
    grafo_lista_adicionar_aresta(&gl, 0, 1);
    grafo_lista_adicionar_aresta(&gl, 0, 2);
    grafo_lista_adicionar_aresta(&gl, 1, 3);
    grafo_lista_bfs(&gl, 0);
    grafo_lista_dfs(&gl, 0);

    return 0;
}