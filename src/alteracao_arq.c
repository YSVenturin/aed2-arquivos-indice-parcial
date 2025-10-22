#include <stdio.h>
#include <string.h>

#include "alteracao_arq.h"
#include "estruturas.h"
#include "consultas.h"

void inserir_pedido_ordenado(char *arquivo, Pedido novo) {
    FILE *f_orig = fopen(arquivo, "rb");
    FILE *f_temp = fopen("data/pedidos_temp.bin", "wb");

    Pedido p;
    int inserido = 0;

    while (fread(&p, sizeof(Pedido), 1, f_orig)) {
        if (!inserido && strcmp(novo.id_pedido, p.id_pedido) < 0) {
            fwrite(&novo, sizeof(Pedido), 1, f_temp);
            inserido = 1;
        }
        fwrite(&p, sizeof(Pedido), 1, f_temp);
    }

    if (!inserido) {
        fwrite(&novo, sizeof(Pedido), 1, f_temp);
    }

    fclose(f_orig);
    fclose(f_temp);

    remove(arquivo);
    rename("pedidos_temp.bin", arquivo);

    printf("Pedido inserido.\n");
}

void remover_pedido_logico(char *arquivo, char *id_busca, Indice *indice, int quant) {
    FILE *f = fopen(arquivo, "r+b");
    if (!f) {
        printf("Erro ao abrir arquivo para remover.\n");
        return;
    }

    Pedido p;
    while (fread(&p, sizeof(Pedido), 1, f)) {
        if (strcmp(p.id_pedido, id_busca) == 0) {
            p.id_pedido[0] = '*'; // marca como removido
            fseek(f, -sizeof(Pedido), SEEK_CUR);
            fwrite(&p, sizeof(Pedido), 1, f);
            fclose(f);
            printf("✅ Pedido %s removido logicamente.\n", id_busca);
            return;
        }
    }

    fclose(f);
    printf("Pedido %s nao encontrado.\n", id_busca);
}
