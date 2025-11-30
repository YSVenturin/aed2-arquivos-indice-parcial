#include "estruturas.h"

#ifndef INDICE_H
#define INDICE_H

#define TAM_BLOCO 128
#define TAM_VETOR_HASH 90647
#define TAM_PAGINA 1024 // Tamanho da página de disco simulada em bytes
#define ORDEM 22
typedef struct indice {
    char id[TAM_MAX];
    long offset;
}Indice;
typedef struct indice_hash {
    char id[TAM_MAX];
    long offset;
    struct indice_hash *prox;
}IndiceHash;

typedef struct no_b {
    int num_chaves;
    int folha; // 1 se for folha, 0 caso contrário
    char chaves[ORDEM - 1][TAM_MAX];
    long offsets[ORDEM - 1]; // Endereço do registro no arquivo
    struct no_b *filhos[ORDEM];
} NoB;

typedef struct header {
    int quant_indice_joia;
    int quant_indice_pedido;
    Indice *joias; // Mantendo compatibilidade se necessário, ou pode ser removido se substituído totalmente
    Indice *pedidos;
    NoB *raiz_joias; // Raiz da árvore B para joias
    IndiceHash **pedidos_hash;
} HeaderIndice;

Indice *construir_indice_pedidos(HeaderIndice *hi);
// Indice *construir_indice_joias(HeaderIndice *hi); // Antiga função
NoB *construir_indice_joias_btree(HeaderIndice *hi); // Nova função
HeaderIndice carregar_indices();
void imprimir_indices(HeaderIndice *hi);
// Funções para índice hash
IndiceHash **construir_indice_hash_pedidos();
unsigned int calcula_hash(char str[TAM_MAX]);

// Funções auxiliares para Árvore B
NoB *criar_no_b();
void inserir_b(NoB **raiz, char *chave, long offset);
void split_child(NoB *x, int i);
void insert_non_full(NoB *x, char *chave, long offset);
long buscar_b(NoB *raiz, char *chave); // Função de busca
void imprimir_arvore_b(NoB *raiz, int nivel);
void liberar_arvore_b(NoB *raiz);


#endif