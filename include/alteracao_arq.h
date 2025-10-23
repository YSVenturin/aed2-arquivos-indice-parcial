#include "estruturas.h"
#include "indice.h"

#ifndef ALTERACAO_ARQ_H
#define ALTERACAO_ARQ_H

void inserir_pedido_ordenado(char *arquivo, Pedido novo);
void reorganizararq_pedidos(char *arquivo);
int remover_pedido_logicamente(char *arquivo, char *id_busca);

#endif