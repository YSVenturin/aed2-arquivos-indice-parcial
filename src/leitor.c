#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "leitor.h"
#include "estruturas.h"

Linha *ler_linhas(FILE *fptr) {
    int i, j;
    char linhastr[TAM_MAX_LINHA];
    char colunaatual[TAM_MAX];
    int index_colunaatual;
    memset(linhastr, ' ', sizeof(linhastr));
    Linha *inicio = NULL, *ultima = NULL;

    while (fgets(linhastr, TAM_MAX_LINHA, fptr) != NULL) {
        remover_novalinha(linhastr);

        Linha *linha = (Linha *)malloc(sizeof(Linha));
        linha->prox = NULL;

        // Inicializar todas as colunas com string sem nada para que colunas sem valores nao alterem o CSV
        for (j = 0; j < NUM_COLUNAS; j++) {
            linha->colunas[j][0] = ' ';
        }

        i = 0;
        index_colunaatual = 0;
        for (j = 0; linhastr[j] != '\0'; j++) {
            if (linhastr[j] == ',') {
                colunaatual[index_colunaatual] = '\0';
                strcpy(linha->colunas[i++], colunaatual);
                index_colunaatual = 0;
            } 
            else {
                colunaatual[index_colunaatual++] = linhastr[j];
            }
        }
        colunaatual[index_colunaatual] = '\0';
        if (i < NUM_COLUNAS) {
            strcpy(linha->colunas[i++], colunaatual);
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

void insere_joia_ordenado(Joia nova_joia, int *quant_joias, Joia **vetor_joias, int *TAM_VETOR_JOIAS) {
    int i;

    int posicao = verifica_joia_no_vetor(nova_joia.id_joia, *quant_joias, *vetor_joias);

    // ja existe 
    if (posicao < *quant_joias && strcmp((*vetor_joias)[posicao].id_joia, nova_joia.id_joia) == 0) {
        return;
    }

    if (*quant_joias == *TAM_VETOR_JOIAS) {
        *TAM_VETOR_JOIAS *= 2;
        *vetor_joias = realloc(*vetor_joias, *TAM_VETOR_JOIAS * sizeof(Joia));
        if (*vetor_joias == NULL) {
            printf("Erro ao realocar..\n");
            exit(1);
        }
    }

    // Desloca os elementos para a direita
    for (i = *quant_joias; i > posicao; i--) {
        (*vetor_joias)[i] = (*vetor_joias)[i - 1];
    }

    // Insere a nova joia na posição correta
    (*vetor_joias)[posicao] = nova_joia;
    (*quant_joias)++;
}

int verifica_joia_no_vetor(char *id_joia, int quant_joias, Joia *vetor_joias) {
    if (quant_joias == 0) {
        return 0;
    }  

    int inf = 0;     // limite inferior (o primeiro índice de vetor em C é zero)
    int sup = quant_joias - 1; // limite superior (termina em um número a menos. 0 a 9 são 10 números)
    int meio;

    while (inf <= sup)
    {
        meio = (inf + sup) / 2;
        
        int comp = strcmp(vetor_joias[meio].id_joia, id_joia);

        if (comp == 0){
            return meio;
        }
        if (comp > 0)
            sup = meio-1;
        else
            inf = meio+1;
    }

    return inf;   // Não encontrado
}

void gerar_arquivos_de_dados(Linha *linhas) {
    FILE *f_joias = fopen("data/joias.bin", "wb");
    FILE *f_pedidos = fopen("data/pedidos.bin", "wb");
    FILE *f_joias_pedidos = fopen("data/joias_pedidos.bin", "wb");

    Linha *linha_atual = linhas;

    JoiaPedido joiapedido;
    Pedido pedido;
    Joia nova_joia;

    // Variaveis de controle para nao duplicar registros nos arquivos
    int i;
    char id_ultimo_pedido_lido[TAM_MAX];
    id_ultimo_pedido_lido[0] = '\0';
    char id_pedido_atual[TAM_MAX];
    int quant_joias = 0;
    int TAM_VETOR_JOIAS = 1000;
    Joia *vetor_joias = (Joia *)malloc(TAM_VETOR_JOIAS * sizeof(Joia));

    while (linha_atual != NULL) {
        memset(&joiapedido, 0, sizeof(JoiaPedido));
        memset(&pedido, 0, sizeof(Pedido));
        memset(&nova_joia, 0, sizeof(Joia));

        // Escrita no arquivo joias_pedidos.bin
        strcpy(joiapedido.id_joia, linha_atual->colunas[ID_PRODUTO]);
        strcpy(joiapedido.id_pedido, linha_atual->colunas[ID_PEDIDO]);
        joiapedido.quant = atoi(linha_atual->colunas[QUANTIDADE_SKU]);
        fwrite(&joiapedido, sizeof(JoiaPedido), 1, f_joias_pedidos);

        // Escrita no arquivo pedidos.bin
        strcpy(id_pedido_atual, linha_atual->colunas[ID_PEDIDO]);
        if (strcmp(id_ultimo_pedido_lido, id_pedido_atual) != 0) {
            strcpy(pedido.id_pedido, linha_atual->colunas[ID_PEDIDO]);
            strcpy(pedido.id_usuario, linha_atual->colunas[ID_USUARIO]);
            strcpy(pedido.data, linha_atual->colunas[DATA_PEDIDO]);
            fwrite(&pedido, sizeof(Pedido), 1, f_pedidos);
        }
        strcpy(id_ultimo_pedido_lido, id_pedido_atual);

        // Escrita no arquivo joias.bin
        strcpy(nova_joia.id_joia, linha_atual->colunas[ID_PRODUTO]);
        strcpy(nova_joia.id_categoria, linha_atual->colunas[ID_CATEGORIA]);
        strcpy(nova_joia.nome_categoria, linha_atual->colunas[NOME_CATEGORIA]);
        nova_joia.id_marca = (linha_atual->colunas[ID_MARCA])[0];
        nova_joia.preco = atof(linha_atual->colunas[PRECO]);
        insere_joia_ordenado(nova_joia, &quant_joias, &vetor_joias, &TAM_VETOR_JOIAS);

        linha_atual = linha_atual->prox;
    }

    for (i = 0; i < quant_joias; i++) {
        fwrite(&vetor_joias[i], sizeof(Joia), 1, f_joias);
    }

    free(vetor_joias);
    fclose(f_joias);
    fclose(f_pedidos);
    fclose(f_joias_pedidos);
}

int arquivos_de_dados_ja_existem() {
    FILE *f1, *f2, *f3;
    f1 = fopen("data/joias.bin", "rb");
    f2 = fopen("data/pedidos.bin", "rb");
    f3 = fopen("data/joias_pedidos.bin", "rb");

    if (f1 == NULL || f2 == NULL || f3 == NULL)
        return 0;
    
    return 1;
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