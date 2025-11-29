#include "estruturas.h"

#ifndef INDICE_H
#define INDICE_H

#define TAM_BLOCO 128
#define TAM_VETOR_HASH 90647 // Numero Primo mais proximo do total de elementos no arquivo de pedidos.bin (69728)

typedef struct indice {
    char id[TAM_MAX];
    long offset;
}Indice;

typedef struct indice_hash {
    char id[TAM_MAX];
    long offset;
    struct indice_hash *prox;
}IndiceHash;

typedef struct header {
    int quant_indice_joia;
    int quant_indice_pedido;
    Indice *joias;
    Indice *pedidos;
    IndiceHash **pedidos_hash;
}HeaderIndice;

Indice *construir_indice_pedidos(HeaderIndice *hi);
Indice *construir_indice_joias(HeaderIndice *hi);
HeaderIndice carregar_indices();
void imprimir_indices(HeaderIndice *hi);
IndiceHash **construir_indice_hash_pedidos();
unsigned int calcula_hash(char str[TAM_MAX]);

#endif