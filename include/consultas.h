#include "indice.h"
#include "estruturas.h"

#ifndef CONSULTAS_H
#define CONSULTAS_H

void mostrar_pedidos(char *nome_arquivo);
void mostrar_joias(char *nome_arquivo);
int busca_binaria_indice(Indice *i, int quant, char *id_busca);
void consultar_pedido(const char *id_busca, Indice *indices, int n_indices);
void consultar_joia(const char *id_busca, Indice *indices, int n_indices);

#endif