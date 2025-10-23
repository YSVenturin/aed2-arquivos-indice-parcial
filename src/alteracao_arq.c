#include <stdio.h>
#include <string.h>

#include "alteracao_arq.h"
#include "estruturas.h"
#include "consultas.h"

void inserir_pedido_ordenado(char *arquivo, Pedido novo) {
    FILE *f_orig = fopen(arquivo, "rb");
    FILE *f_temp = fopen("data/pedidos_temp.bin", "wb");

    if (f_temp == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        return;
    }

    printf("Inserindo pedido: %s %s %s\n", novo.id_pedido, novo.data, novo.id_usuario);

    Pedido p;
    int inserido = 0;

    novo.ativo = '1';

    if (f_orig != NULL) {
        while (fread(&p, sizeof(Pedido), 1, f_orig) == 1) {
            if (!inserido && strcmp(novo.id_pedido, p.id_pedido) < 0) {
                fwrite(&novo, sizeof(Pedido), 1, f_temp);
                inserido = 1;
            }
            fwrite(&p, sizeof(Pedido), 1, f_temp);
        }
        fclose(f_orig);
    }
    if (!inserido) {
        fwrite(&novo, sizeof(Pedido), 1, f_temp);
    }

    fclose(f_temp);

    remove(arquivo);
    rename("data/pedidos_temp.bin", arquivo);
}


int remover_pedido_logicamente(char *arquivo, char *id_busca) {
    FILE *f = fopen(arquivo, "rb+");

    Pedido p;
    int removido = 0;

    while (fread(&p, sizeof(Pedido), 1, f) == 1) {
        if (strcmp(p.id_pedido, id_busca) == 0) {
            p.ativo= '*';
            fseek(f, -(long)(sizeof(Pedido)), SEEK_CUR);
            fwrite(&p, sizeof(Pedido), 1, f);
            removido = 1;
            break;
        }
    }

    fclose(f);

    if (removido)
        printf("Pedido removido.\n", id_busca);
    else
        printf("Pedido %s não encontrado.\n", id_busca);

    return removido;
}

void reorganizararq_pedidos(char *arquivo) {
    FILE *f_orig = fopen(arquivo, "rb");
    FILE *f_temp = fopen("data/pedidos_compactado.bin", "wb");

    Pedido p;

    while (fread(&p, sizeof(Pedido), 1, f_orig) == 1) {
        if (p.ativo == '*') {
            continue;
        }
        fwrite(&p, sizeof(Pedido), 1, f_temp);
    }

    fclose(f_orig);
    fclose(f_temp);

    remove(arquivo);
    rename("data/pedidos_compactado.bin", arquivo);
}


