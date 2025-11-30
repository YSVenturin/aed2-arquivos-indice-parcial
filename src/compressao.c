#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compressao.h"

#define TAM_TABELA 256

typedef struct {
    char codigo[256];
    int tamanho;
} CodigoHuffman;

// Função auxiliar para criar um novo nó
NoHuffman* criar_no(unsigned char caracter, int frequencia) {
    NoHuffman* novo = (NoHuffman*)malloc(sizeof(NoHuffman));
    novo->caracter = caracter;
    novo->frequencia = frequencia;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// Função para liberar a árvore
void liberar_arvore_huffman(NoHuffman* raiz) {
    if (raiz) {
        liberar_arvore_huffman(raiz->esq);
        liberar_arvore_huffman(raiz->dir);
        free(raiz);
    }
}

// Função para encontrar o nó com menor frequência na lista
int encontrar_menor(NoHuffman** lista, int tamanho) {
    int menor = -1;
    int i;
    for (i = 0; i < tamanho; i++) {
        if (lista[i] != NULL) {
            if (menor == -1 || lista[i]->frequencia < lista[menor]->frequencia) {
                menor = i;
            }
        }
    }
    return menor;
}

// Constrói a árvore de Huffman
NoHuffman* construir_arvore(int* frequencias) {
    NoHuffman* lista[TAM_TABELA];
    int i, tamanho = 0;

    for (i = 0; i < TAM_TABELA; i++) {
        if (frequencias[i] > 0) {
            lista[tamanho++] = criar_no((unsigned char)i, frequencias[i]);
        }
    }

    // Se o arquivo estiver vazio ou tiver apenas 1 tipo de caractere
    if (tamanho == 0) return NULL;
    if (tamanho == 1) {
        NoHuffman* raiz = criar_no(0, lista[0]->frequencia);
        raiz->esq = lista[0];
        return raiz;
    }

    int qtd_nos = tamanho;
    while (qtd_nos > 1) {
        int i1 = encontrar_menor(lista, tamanho);
        NoHuffman* n1 = lista[i1];
        lista[i1] = NULL;

        int i2 = encontrar_menor(lista, tamanho);
        NoHuffman* n2 = lista[i2];
        lista[i2] = NULL;

        NoHuffman* pai = criar_no(0, n1->frequencia + n2->frequencia);
        pai->esq = n1;
        pai->dir = n2;

        // Encontra um slot vazio para colocar o pai
        for (i = 0; i < tamanho; i++) {
            if (lista[i] == NULL) {
                lista[i] = pai;
                break;
            }
        }
        qtd_nos--;
    }

    // O último nó restante é a raiz
    for (i = 0; i < tamanho; i++) {
        if (lista[i] != NULL) return lista[i];
    }
    return NULL;
}

// Gera a tabela de códigos
void gerar_codigos_recursivo(NoHuffman* raiz, char* codigo_atual, int profundidade, CodigoHuffman* tabela) {
    if (raiz->esq == NULL && raiz->dir == NULL) {
        codigo_atual[profundidade] = '\0';
        strcpy(tabela[raiz->caracter].codigo, codigo_atual);
        tabela[raiz->caracter].tamanho = profundidade;
        return;
    }

    if (raiz->esq) {
        codigo_atual[profundidade] = '0';
        gerar_codigos_recursivo(raiz->esq, codigo_atual, profundidade + 1, tabela);
    }
    if (raiz->dir) {
        codigo_atual[profundidade] = '1';
        gerar_codigos_recursivo(raiz->dir, codigo_atual, profundidade + 1, tabela);
    }
}

void comprimir_arquivo(const char *arquivo_entrada, const char *arquivo_saida) {
    FILE *entrada = fopen(arquivo_entrada, "rb");
    if (!entrada) {
        printf("Erro ao abrir arquivo de entrada: %s\n", arquivo_entrada);
        return;
    }

    int frequencias[TAM_TABELA] = {0};
    unsigned char buffer;
    long total_bytes = 0;

    // Conta frequências
    while (fread(&buffer, 1, 1, entrada) == 1) {
        frequencias[buffer]++;
        total_bytes++;
    }

    rewind(entrada);

    NoHuffman* raiz = construir_arvore(frequencias);
    if (!raiz) {
        fclose(entrada);
        // Arquivo vazio
        FILE *saida = fopen(arquivo_saida, "wb");
        long zero = 0;
        fwrite(&zero, sizeof(long), 1, saida); // Total bytes 0
        fwrite(frequencias, sizeof(int), TAM_TABELA, saida);
        fclose(saida);
        return;
    }

    CodigoHuffman tabela[TAM_TABELA];
    char codigo_temp[256];
    // Inicializa tabela
    int i;
    for(i=0; i<TAM_TABELA; i++) tabela[i].tamanho = 0;
    
    // Caso especial: apenas 1 tipo de caractere
    if (!raiz->esq && !raiz->dir) {
         // Isso acontece se a raiz for folha (arquivo com 1 byte repetido ou 1 byte unico)
         // Mas minha logica de construir_arvore cria um pai dummy se so tiver 1 no.
         // Se construir_arvore retornar raiz que é folha, algo deu errado ou logica diferente.
         // Minha logica: if (tamanho == 1) cria pai. Entao raiz sempre tem filhos se tamanho >= 1.
         // Se tamanho == 1, raiz->esq é o nó. raiz->dir é NULL? Nao, Huffman precisa de 2 filhos ou ser folha.
         // Ajuste na construir_arvore: if (tamanho == 1) ... raiz->esq = lista[0]; raiz->dir = NULL? 
         // Huffman tree normal para 1 simbolo: raiz -> filho(simbolo, bit 0).
         // Vamos assumir que construir_arvore lida com isso.
         // Na minha implementacao: if (tamanho == 1) ... raiz->esq = lista[0].
         // Entao gerar_codigos vai descer para esq com '0'. OK.
    }

    gerar_codigos_recursivo(raiz, codigo_temp, 0, tabela);

    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
        printf("Erro ao criar arquivo de saida: %s\n", arquivo_saida);
        fclose(entrada);
        liberar_arvore_huffman(raiz);
        return;
    }

    // Escreve cabeçalho: Total de bytes original e tabela de frequências
    fwrite(&total_bytes, sizeof(long), 1, saida);
    fwrite(frequencias, sizeof(int), TAM_TABELA, saida);

    unsigned char byte_saida = 0;
    int bit_count = 0;

    while (fread(&buffer, 1, 1, entrada) == 1) {
        char *cod = tabela[buffer].codigo;
        int len = tabela[buffer].tamanho;
        int i;
        for (i = 0; i < len; i++) {
            if (cod[i] == '1') {
                byte_saida |= (1 << (7 - bit_count));
            }
            bit_count++;
            if (bit_count == 8) {
                fwrite(&byte_saida, 1, 1, saida);
                byte_saida = 0;
                bit_count = 0;
            }
        }
    }

    // Escreve o último byte se houver bits pendentes
    if (bit_count > 0) {
        fwrite(&byte_saida, 1, 1, saida);
    }

    fclose(entrada);
    fclose(saida);
    liberar_arvore_huffman(raiz);
    printf("Arquivo comprimido com sucesso!\n");
}

void descomprimir_arquivo(const char *arquivo_entrada, const char *arquivo_saida) {
    FILE *entrada = fopen(arquivo_entrada, "rb");
    if (!entrada) {
        printf("Erro ao abrir arquivo comprimido: %s\n", arquivo_entrada);
        return;
    }

    long total_bytes;
    int frequencias[TAM_TABELA];

    if (fread(&total_bytes, sizeof(long), 1, entrada) != 1) {
        fclose(entrada);
        return;
    }
    if (fread(frequencias, sizeof(int), TAM_TABELA, entrada) != TAM_TABELA) {
        fclose(entrada);
        return;
    }

    if (total_bytes == 0) {
        FILE *saida = fopen(arquivo_saida, "wb");
        fclose(saida);
        fclose(entrada);
        printf("Arquivo descomprimido (vazio).\n");
        return;
    }

    NoHuffman* raiz = construir_arvore(frequencias);
    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
        printf("Erro ao criar arquivo de saida: %s\n", arquivo_saida);
        fclose(entrada);
        liberar_arvore_huffman(raiz);
        return;
    }

    NoHuffman* atual = raiz;
    unsigned char byte_lido;
    long bytes_escritos = 0;

    while (bytes_escritos < total_bytes && fread(&byte_lido, 1, 1, entrada) == 1) {
        int i;
        for (i = 0; i < 8; i++) {
            int bit = (byte_lido >> (7 - i)) & 1;
            
            if (bit == 0) atual = atual->esq;
            else atual = atual->dir;

            // Se for folha
            if (atual->esq == NULL && atual->dir == NULL) {
                fwrite(&(atual->caracter), 1, 1, saida);
                bytes_escritos++;
                atual = raiz;
                if (bytes_escritos == total_bytes) break;
            }
        }
    }

    fclose(entrada);
    fclose(saida);
    liberar_arvore_huffman(raiz);
    printf("Arquivo descomprimido com sucesso!\n");
}
