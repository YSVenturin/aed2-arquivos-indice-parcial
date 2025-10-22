#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consultas.h"
#include "estruturas.h"
#include "indice.h"

void mostrar_pedidos(char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "rb");

    Pedido p;
    printf("\n=== PEDIDOS ===\n");
    while (fread(&p, sizeof(Pedido), 1, f) == 1) {
        printf("ID Pedido: %s | Data: %s | ID Usuario: %s\n", p.id_pedido, p.data, p.id_usuario);
    }

    fclose(f);
}

void mostrar_joias(char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "rb");

    Joia j;
    printf("\n=== JOIAS ===\n");
    while (fread(&j, sizeof(Joia), 1, f) == 1) {
        printf("ID Joia: %s | ID Categoria: %s | Nome Categoria: %s | ID Marca: %c | Preco: %.2f\n", j.id_joia, j.id_categoria, j.nome_categoria, j.id_marca, j.preco);
    }

    fclose(f);
}

int busca_binaria_indice(Indice *i, int quant, char *id_busca) {
    int inf = 0;     // limite inferior (o primeiro índice de vetor em C é zero)
    int sup = quant - 1; // limite superior (termina em um número a menos. 0 a 9 são 10 números)
    int meio;
    int pos = 0;

    while (inf <= sup)
    {
        meio = (inf + sup) / 2;
        
        int comp = strcmp(i[meio].id, id_busca);

        if (comp == 0){
            return meio; // Encontrado
        }
        if (comp > 0) {
            sup = meio-1;
            pos = meio;
        }
        else {
            inf = meio+1;
            pos = inf;
        }
    }

    if (pos >= quant)
        pos = quant - 1;

    return pos;   // Não encontrado
}

Joia *busca_joia_por_id(Indice *indice, int quant, char *id_busca) {
    FILE *f_joia = fopen("data/joias.bin", "rb");
    if (f_joia == NULL) {
        printf("Arquivo de dados nao existe.");
        exit(1);
    }
    
    int i;
    int pos =  busca_binaria_indice(indice, quant, id_busca);

    long offset = indice[pos].offset;
    fseek(f_joia, offset, SEEK_SET);

    Joia *j = (Joia *)malloc(sizeof(Joia));
    if (!j) {
        fclose(f_joia);
        return NULL;
    }

    for (i = 0; i < TAM_BLOCO; i++) {
        fread(j, sizeof(Joia), 1, f_joia);

        if (strcmp(j->id_joia, id_busca) == 0) {
            fclose(f_joia);
            return j;
        }
    }

    fclose(f_joia);

    return NULL;
}

Pedido *busca_pedido_por_id(Indice *indice, int quant, char *id_busca) {
    FILE *f_pedidos = fopen("data/pedidos.bin", "rb");
    if (f_pedidos == NULL) {
        printf("Arquivo de dados nao existe.");
        exit(1);
    }
    
    int i;
    int pos =  busca_binaria_indice(indice, quant, id_busca);

    long offset = indice[pos].offset;
    fseek(f_pedidos, offset, SEEK_SET);

    Pedido *p = (Pedido *)malloc(sizeof(Pedido));
    if (!p) {
        fclose(f_pedidos);
        return NULL;
    }

    for (i = 0; i < TAM_BLOCO; i++) {
        fread(p, sizeof(Pedido), 1, f_pedidos);

        if (strcmp(p->id_pedido, id_busca) == 0) {
            fclose(f_pedidos);
            return p;
        }
    }

    fclose(f_pedidos);

    return NULL;
}

