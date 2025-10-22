#include "estruturas.h"
#include "indice.h"

#ifndef ALTERACAO_ARQ_H
#define ALTERACAO_ARQ_H

void inserir_pedido_ordenado(char *arquivo, Pedido novo);
void remover_pedido_logico(char *arquivo, char *id_busca);
void reconstruir_indice_pedidos(char *nome_arquivo, Indice **indice, int *quant_indice);

#endif