#include "estruturas.h"

#ifndef LEITOR_H
#define LEITOR_H

#define NUM_COLUNAS 13
#define TAM_MAX_LINHA 200

//Order-datetime,Order-ID,Purchased-product-ID,Quantity-of-SKU-in-the-order,Category-ID,Category-alias,Brand-ID,Price-in-USD,User-ID,Product-gender,color,metal,gem
typedef enum {
    DATA_PEDIDO,
    ID_PEDIDO,
    ID_PRODUTO,
    QUANTIDADE_SKU,
    ID_CATEGORIA,
    NOME_CATEGORIA,
    ID_MARCA,
    PRECO,
    ID_USUARIO,
    GENERO_PRODUTO,
    COR,
    METAL,
    GEMA
} Colunas;

typedef struct linha {
    char colunas[NUM_COLUNAS][TAM_MAX];
    struct linha *prox;
} Linha;

Linha *ler_linhas(FILE *fptr);
void remover_novalinha(char *str);
void desalocar_linhas(Linha *linha);
void gerar_arquivos_de_dados(Linha *linhas);

#endif