#include "estruturas.h"

#ifndef INDICE_H
#define INDICE_H

#define TAM_BLOCO 128

typedef struct indice {
    char id[TAM_MAX];
    long offset;
}Indice;

typedef struct header {
    int quant_indice_joia;
    int quant_indice_pedido;
    Indice *joias;
    Indice *pedidos;
}HeaderIndice;

Indice *construir_indice_pedidos(HeaderIndice *hi);
Indice *construir_indice_joias(HeaderIndice *hi);
HeaderIndice carregar_indices();
void imprimir_indices(HeaderIndice *hi);

#endif