#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "indice.h"
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
    hi.raiz_joias = construir_indice_joias_btree(&hi); // Constrói a árvore B em memória
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

// Funções para Árvore B

NoB *criar_no_b() {
    NoB *novo = (NoB *)malloc(sizeof(NoB));
    novo->num_chaves = 0;
    novo->folha = 1;
    int i;
    for (i = 0; i < ORDEM; i++) {
        novo->filhos[i] = NULL;
    }
    return novo;
}

void split_child(NoB *x, int i) {
    NoB *y = x->filhos[i];
    NoB *z = criar_no_b();
    z->folha = y->folha;
    z->num_chaves = ORDEM / 2 - 1;

    int j;
    // Copia as chaves finais de y para z
    for (j = 0; j < z->num_chaves; j++) {
        strcpy(z->chaves[j], y->chaves[j + ORDEM / 2]);
        z->offsets[j] = y->offsets[j + ORDEM / 2];
    }

    // Se não for folha, copia os filhos também
    if (!y->folha) {
        for (j = 0; j < ORDEM / 2; j++) {
            z->filhos[j] = y->filhos[j + ORDEM / 2];
        }
    }

    y->num_chaves = ORDEM / 2 - 1;

    // Move filhos de x para abrir espaço para z
    for (j = x->num_chaves; j >= i + 1; j--) {
        x->filhos[j + 1] = x->filhos[j];
    }
    x->filhos[i + 1] = z;

    // Move chaves de x para abrir espaço para a chave mediana de y
    for (j = x->num_chaves - 1; j >= i; j--) {
        strcpy(x->chaves[j + 1], x->chaves[j]);
        x->offsets[j + 1] = x->offsets[j];
    }

    // Sobe a chave mediana
    strcpy(x->chaves[i], y->chaves[ORDEM / 2 - 1]);
    x->offsets[i] = y->offsets[ORDEM / 2 - 1];
    x->num_chaves++;
}

void insert_non_full(NoB *x, char *chave, long offset) {
    int i = x->num_chaves - 1;

    if (x->folha) {
        while (i >= 0 && strcmp(chave, x->chaves[i]) < 0) {
            strcpy(x->chaves[i + 1], x->chaves[i]);
            x->offsets[i + 1] = x->offsets[i];
            i--;
        }
        strcpy(x->chaves[i + 1], chave);
        x->offsets[i + 1] = offset;
        x->num_chaves++;
    } else {
        while (i >= 0 && strcmp(chave, x->chaves[i]) < 0) {
            i--;
        }
        i++;
        if (x->filhos[i]->num_chaves == ORDEM - 1) {
            split_child(x, i);
            if (strcmp(chave, x->chaves[i]) > 0) {
                i++;
            }
        }
        insert_non_full(x->filhos[i], chave, offset);
    }
}

void inserir_b(NoB **raiz, char *chave, long offset) {
    NoB *r = *raiz;
    if (r == NULL) {
        *raiz = criar_no_b();
        r = *raiz;
        strcpy(r->chaves[0], chave);
        r->offsets[0] = offset;
        r->num_chaves = 1;
    } else {
        if (r->num_chaves == ORDEM - 1) {
            NoB *s = criar_no_b();
            *raiz = s;
            s->folha = 0;
            s->num_chaves = 0;
            s->filhos[0] = r;
            split_child(s, 0);
            insert_non_full(s, chave, offset);
        } else {
            insert_non_full(r, chave, offset);
        }
    }
}

NoB *construir_indice_joias_btree(HeaderIndice *hi) {
    FILE *entrada = fopen("data/joias.bin", "rb");
    if (!entrada) {
        printf("Erro ao abrir arquivo de joias.\n");
        return NULL;
    }

    NoB *raiz = NULL;
    Joia j;
    long offset;

    while (1) {
        offset = ftell(entrada);
        if (fread(&j, sizeof(Joia), 1, entrada) != 1) break;
        inserir_b(&raiz, j.id_joia, offset);
    }

    fclose(entrada);
    hi->raiz_joias = raiz;
    return raiz;
}

void imprimir_arvore_b(NoB *raiz, int nivel) {
    if (raiz != NULL) {
        int i;
        for (i = 0; i < raiz->num_chaves; i++) {
            if (!raiz->folha)
                imprimir_arvore_b(raiz->filhos[i], nivel + 1);
            printf("Nivel %d: %s (Offset %ld)\n", nivel, raiz->chaves[i], raiz->offsets[i]);
        }
        if (!raiz->folha)
            imprimir_arvore_b(raiz->filhos[i], nivel + 1);
    }
}

void liberar_arvore_b(NoB *raiz) {
    if (raiz != NULL) {
        if (!raiz->folha) {
            int i;
            for (i = 0; i <= raiz->num_chaves; i++) {
                liberar_arvore_b(raiz->filhos[i]);
            }
        }
        free(raiz);
    }
}

long buscar_b(NoB *x, char *chave) {
    if (x == NULL) return -1;
    int i = 0;
    while (i < x->num_chaves && strcmp(chave, x->chaves[i]) > 0) {
        i++;
    }
    if (i < x->num_chaves && strcmp(chave, x->chaves[i]) == 0) {
        return x->offsets[i];
    } else if (x->folha) {
        return -1;
    } else {
        return buscar_b(x->filhos[i], chave);
    }
}