#ifndef COMPRESSAO_H
#define COMPRESSAO_H

#include <stdio.h>

typedef struct no_huffman {
    unsigned char caracter;
    int frequencia;
    struct no_huffman *esq, *dir;
} NoHuffman;

void comprimir_arquivo(const char *arquivo_entrada, const char *arquivo_saida);
void descomprimir_arquivo(const char *arquivo_entrada, const char *arquivo_saida);

#endif
