#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estruturas.h"

#define TAM_MAX_LINHA 200
#define ERRO -1

// ORGANIZACAO DO ARQUIVOS CSV INICIAL
// Order-datetime,Order-ID,Purchased-product-ID,Quantity-of-SKU-in-the-order,Category-ID,Category-alias,Brand-ID,Price-in-USD,User-ID,Product-gender,color,metal,gem

int main() {
    FILE *arquivo_fonte = fopen("../data/jewelry.csv", "r");
    if (arquivo_fonte == NULL) {
        printf("Falha ao abrir arquivo fonte de dados.\n");
        exit(0);
    }

    if (criacao_arquivos_iniciais(arquivo_fonte) == ERRO) {
        printf("Falha ao criar arquivos iniciais.\n");
        fclose(arquivo_fonte);
        exit(0);
    }

    fclose(arquivo_fonte);

    return 0;
}

int criacao_arquivos_iniciais(FILE *fonte) {
    FILE *joia = fopen("../data/joia.csv", "w");
    FILE *pedido = fopen("../data/pedido.csv", "w");
    FILE *joia_pedido = fopen("../data/joia_pedido.csv", "w");

    if (joia == NULL || pedido == NULL || joia_pedido == NULL) {
        printf("Falha ao criar arquivos de dados.\n");
        return ERRO;
    }

    char linha[TAM_MAX_LINHA];
    while (fgets(linha, TAM_MAX_LINHA, fonte) != NULL) {
        
    }
}