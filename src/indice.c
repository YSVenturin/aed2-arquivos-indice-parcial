#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "indice.h"
#include "estruturas.h"
#include "criptografia.h"

Indice *construir_indice_pedidos(HeaderIndice *hi) {
    FILE *entrada = fopen("data/pedidos.bin", "rb");
    FILE *indice = fopen("data/arq_indice_pedidos.i", "wb");

    fseek(entrada, 0, SEEK_END);
    long total_registros = ftell(entrada) / sizeof(Pedido);
    fseek(entrada, 0, SEEK_SET);

    int num_reg_indice = total_registros / TAM_BLOCO;
    if (total_registros % TAM_BLOCO > 0)
        num_reg_indice++;

    hi->pedidos = (Indice *)malloc(num_reg_indice * sizeof(Indice));
    hi->quant_indice_pedido = num_reg_indice;
    
    Indice *indice_pedido = hi->pedidos;
    int indice_atual = 0;
    
    Pedido p;
    int bloco;
    for (bloco = 0; bloco < num_reg_indice; bloco++) {
        indice_pedido[bloco].offset = ftell(entrada);

        long index_ultimo = (bloco + 1) * TAM_BLOCO - 1;
        if (index_ultimo >= total_registros){
            strcpy(indice_pedido[bloco].id, "");
            break;
        }

        fseek(entrada, index_ultimo * sizeof(Pedido), SEEK_SET);

        if (fread(&p, sizeof(Pedido), 1, entrada) == 1){
            cifra_xor(&p);
            strcpy(indice_pedido[bloco].id, p.id_pedido);
        }
            
    }

    int i;
    for (i = 0; i < num_reg_indice; i++) {
        fwrite(&indice_pedido[i], sizeof(Indice), 1, indice);
    }

    fclose(entrada);
    fclose(indice);

    return indice_pedido;
}

Indice *construir_indice_joias(HeaderIndice *hi) {
    FILE *entrada = fopen("data/joias.bin", "rb");
    FILE *indice = fopen("data/arq_indice_joias.i", "wb");

    fseek(entrada, 0, SEEK_END);
    long total_registros = ftell(entrada) / sizeof(Joia);
    fseek(entrada, 0, SEEK_SET);

    int num_reg_indice = total_registros / TAM_BLOCO;
    if (total_registros % TAM_BLOCO > 0)
        num_reg_indice++;
    
    hi->joias = (Indice *)malloc(num_reg_indice * sizeof(Indice));
    hi->quant_indice_joia = num_reg_indice;

    Indice *indice_joia = hi->joias;
    int indice_atual = 0;
    
    Joia j;
    int bloco;
    for (bloco = 0; bloco < num_reg_indice; bloco++) {
        // Offset do primeiro registro do bloco
        indice_joia[bloco].offset = ftell(entrada);

        // Calcular índice do último registro do bloco
        long index_ultimo = (bloco + 1) * TAM_BLOCO - 1;
        if (index_ultimo >= total_registros){
            strcpy(indice_joia[bloco].id, "");
            break;
        }

        fseek(entrada, index_ultimo * sizeof(Joia), SEEK_SET);

        if (fread(&j, sizeof(Joia), 1, entrada) == 1)
            strcpy(indice_joia[bloco].id, j.id_joia);
    }

    int i;
    for (i = 0; i < num_reg_indice; i++) {
        fwrite(&indice_joia[i], sizeof(Indice), 1, indice);
    }

    fclose(entrada);
    fclose(indice);

    return indice_joia;
}

IndiceHash **construir_indice_hash_pedidos() {
    LARGE_INTEGER inicio, fim, freq;
    QueryPerformanceFrequency(&freq);       // frequência do timer
    QueryPerformanceCounter(&inicio);

    FILE *entrada = fopen("data/pedidos.bin", "rb");

    IndiceHash **indice = (IndiceHash**)calloc(TAM_VETOR_HASH, sizeof(IndiceHash*));

    Pedido p;

    long offset = ftell(entrada);

    while (fread(&p, sizeof(Pedido), 1, entrada) == 1) {
        cifra_xor(&p);

        unsigned int valor_hash = calcula_hash(p.id_pedido);
        int posicao = valor_hash % TAM_VETOR_HASH;

        IndiceHash *i_novo = (IndiceHash*)malloc(sizeof(IndiceHash));
        strcpy(i_novo->id, p.id_pedido);
        i_novo->offset = offset;
        i_novo->prox = NULL;

        offset = ftell(entrada);

        if (indice[posicao] == NULL) {
            indice[posicao] = i_novo;
            continue;
        }

        IndiceHash *aux = indice[posicao];
        while (aux->prox != NULL) {
            aux = aux->prox;
        }

        aux->prox = i_novo;
    }

    fclose(entrada);

    QueryPerformanceCounter(&fim);

    double tempo = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart;

    printf("Tempo de execucao para criacao do indice dos pedidos por Hash: %f segundos\n", tempo);

    return indice;
}

unsigned int calcula_hash(char str[TAM_MAX]) { // Funcao Hash usada foi a djb2
    unsigned int hash = 5381;

    int i = 0;
    while (str[i] != '\0') {
        hash = ((hash << 5) + hash) + str[i++];
        hash += str[i++];
    }

    return hash;
}

HeaderIndice carregar_indices() {
    Indice *i_pedido;
    Indice *i_joia;
    HeaderIndice hi;
    int i;

    FILE *f_ipedido = fopen("data/arq_indice_pedidos.i", "rb");
    if (f_ipedido == NULL)
        i_pedido = construir_indice_pedidos(&hi);
    else {
        fseek(f_ipedido, 0, SEEK_END);
        long total_registros = ftell(f_ipedido) / sizeof(Indice);
        fseek(f_ipedido, 0, SEEK_SET);
        hi.quant_indice_pedido = total_registros;

        i_pedido = (Indice *)malloc(total_registros * sizeof(Indice));
        i = 0;

        while (i < total_registros && fread(&i_pedido[i++], sizeof(Indice), 1, f_ipedido) == 1) {
        }
    }

    FILE *f_ijoia = fopen("data/arq_indice_joias.i", "rb");
    if (f_ijoia == NULL)
        i_joia = construir_indice_joias(&hi);
    else {
        fseek(f_ijoia, 0, SEEK_END);
        long total_registros = ftell(f_ijoia) / sizeof(Indice);
        fseek(f_ijoia, 0, SEEK_SET);
        hi.quant_indice_joia = total_registros;

        i_joia = (Indice *)malloc(total_registros * sizeof(Indice));
        i = 0;

        while (i < total_registros && fread(&i_joia[i++], sizeof(Indice), 1, f_ijoia) == 1) {
        }
    }

    hi.joias = i_joia;
    hi.pedidos = i_pedido;
    hi.pedidos_hash = construir_indice_hash_pedidos();

    return hi;
}

void imprimir_indices(HeaderIndice *hi) {
    int i;

    for (i = 0; i< hi->quant_indice_joia; i++) {
        printf("ID J: %s | OFFSET: %ld\n", hi->joias[i].id, hi->joias[i].offset);
    }

    for (i = 0; i< hi->quant_indice_pedido; i++) {
        printf("ID P: %s | OFFSET: %ld\n", hi->pedidos[i].id, hi->pedidos[i].offset);
    }
}