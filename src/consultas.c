#include <stdio.h>
#include <string.h>

#include "consultas.h"

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

