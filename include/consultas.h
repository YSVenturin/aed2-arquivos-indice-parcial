#include "indice.h"
#include "estruturas.h"

#ifndef CONSULTAS_H
#define CONSULTAS_H

void mostrar_pedidos(char *nome_arquivo);
void mostrar_joias(char *nome_arquivo);
int busca_binaria_indice(Indice *i, int quant, char *id_busca);
Pedido *busca_pedido_por_id(Indice *indice, int quant, char *id_busca);
Joia *busca_joia_por_id(Indice *indice, int quant, char *id_busca);
void listar_pedidos_por_data(char *nome_arquivo, char *data_busca);
void resumo_preco_joias(char *nome_arquivo);

#endif