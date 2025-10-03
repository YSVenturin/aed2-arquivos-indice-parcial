#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "leitor.h"
#include "estruturas.h"

Linha *ler_linhas(FILE *fptr) {
    int i;
    char linhastr[TAM_MAX_LINHA];
    memset(linhastr, ' ', sizeof(linhastr));
    Linha *inicio = NULL, *ultima = NULL;

    while (fgets(linhastr, TAM_MAX_LINHA, fptr) != NULL) {
        remover_novalinha(linhastr);

        Linha *linha = (Linha *)malloc(sizeof(Linha));
        linha->prox = NULL;

        i = 0;
        char *palavra_atual = strtok(linhastr, ",");
        while (palavra_atual != NULL && i < NUM_COLUNAS) {
            strcpy(linha->colunas[i++], palavra_atual);
            palavra_atual = strtok(NULL, ",");
        }
        memset(linhastr, ' ', sizeof(linhastr));

        if (inicio == NULL) {
            inicio = linha;
            ultima = linha;
        }
        else {
            ultima->prox = linha;
        }
        ultima = linha;
    }

    return inicio;
}

void gerar_arquivos_de_dados(Linha *linhas) {
    FILE *f_joias = fopen("data/joias.bin", "wb");
    FILE *f_pedidos = fopen("data/pedidos.bin", "wb");
    FILE *f_joias_pedidos = fopen("data/joias_pedidos.bin", "wb");

    Linha *linha_atual = linhas;

    JoiaPedido joiapedido;
    Pedido pedido;
    Joia joia;

    // Variaveis de controle para nao duplicar registros nos arquivos
    char id_ultimo_pedido_lido[TAM_MAX];
    char id_pedido_atual[TAM_MAX];
    int quant_joias = 0;
    int TAM_VETOR_JOIAS = 1000;
    Joia *joias = (Joia *)malloc(TAM_VETOR_JOIAS * sizeof(Joia));
    

    while (linha_atual != NULL) {
        memset(&joiapedido, 0, sizeof(JoiaPedido));
        memset(&pedido, 0, sizeof(JoiaPedido));
        memset(&joia, 0, sizeof(JoiaPedido));

        // Escrita no arquivo joias_pedidos.bin
        strcpy(joiapedido.id_joia, linha_atual->colunas[ID_PRODUTO]);
        strcpy(joiapedido.id_pedido, linha_atual->colunas[ID_PEDIDO]);
        joiapedido.quant = atoi(linha_atual->colunas[QUANTIDADE_SKU]);
        fwrite(&joiapedido, sizeof(JoiaPedido), 1, f_joias_pedidos);

        // Escrita no arquivo pedidos.bin
        strcpy(id_pedido_atual, linha_atual->colunas[ID_PEDIDO]);
        if (strcmp(id_ultimo_pedido_lido == id_pedido_atual) != 0) {
            strcpy(pedido.id_pedido, linha_atual->colunas[ID_PEDIDO]);
            strcpy(pedido.id_usuario, linha_atual->colunas[ID_USUARIO]);
            strcpy(pedido.data, linha_atual->colunas[DATA_PEDIDO]);
            fwrite(&pedido, sizeof(Pedido), 1, f_pedidos);
        }
        strcpy(id_ultimo_pedido_lido, id_pedido_atual);

        // Escrita no arquivo joias.bin

        linha_atual = linha_atual->prox;
    }

    fclose(joias);
    fclose(pedidos);
    fclose(joias_pedidos);
}

void desalocar_linhas(Linha *linha) {
    Linha *aux;
    while (linha != NULL) {
        aux = linha->prox;
        free(linha);
        linha = aux;
    }
}

void remover_novalinha(char *str) {
    int len = strlen(str);
    str[len-1] = '\0';

    return;
}