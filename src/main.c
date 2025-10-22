#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estruturas.h"
#include "leitor.h"
#include "indice.h"
#include "consultas.h"

// ORGANIZACAO DO ARQUIVOS CSV INICIAL
// Order-datetime,Order-ID,Purchased-product-ID,Quantity-of-SKU-in-the-order,Category-ID,Category-alias,Brand-ID,Price-in-USD,User-ID,Product-gender,color,metal,gem

void menu_busca(HeaderIndice *header_indices);

int main() {
    FILE *arquivo_fonte = fopen("data/jewelry.csv", "r");
    if (arquivo_fonte == NULL) {
        printf("Falha ao abrir arquivo fonte de dados.\n");
        exit(0);
    }

    if (arquivos_de_dados_ja_existem() == 0) {
        Linha *linhas = ler_linhas(arquivo_fonte);
        fclose(arquivo_fonte);
        gerar_arquivos_de_dados(linhas);
        desalocar_linhas(linhas);
    }
    
    HeaderIndice header_indices = carregar_indices();

    menu_busca(&header_indices);

    //imprimir_indices(&header_indices);
    // mostrar_joias("data/joias.bin");
    // mostrar_pedidos("data/pedidos.bin");

    // Pedido *p = busca_pedido_por_id(header_indices.pedidos, header_indices.quant_indice_pedido, "2719022379232658075");
    // Joia *j = busca_joia_por_id(header_indices.joias, header_indices.quant_indice_joia, "1515966222738746536");

    // if (p == NULL) {
    //     printf("Nao Encontrado.");
    // }
    // else {
    //     printf("\n%s %s\n", p->id_pedido, p->data);
    // }

    // if (j == NULL) {
    //     printf("Nao Encontrado.");
    // }
    // else {
    //     printf("\n%s %.2f\n", j->id_joia, j->preco);
    // }

    return 0;
}

void menu_busca(HeaderIndice *header_indices) {
    int opcao;
    char id_busca[TAM_MAX];

    printf("\n===== MENU DE BUSCA =====\n");
    printf("Escolha uma opcao:\n");
    printf("1 - Buscar Pedido por ID\n");
    printf("2 - Buscar Joia por ID\n");
    printf("0 - Sair\n");
    printf("--> ");
    scanf("%d", &opcao);
    getchar();

    while (opcao != 0) {
        switch (opcao) {
        case 1:
            printf("Digite o ID do pedido: ");
            fgets(id_busca, sizeof(id_busca), stdin);
            remover_novalinha(id_busca);

            Pedido *p = busca_pedido_por_id(header_indices->pedidos, header_indices->quant_indice_pedido, id_busca);
            if (p == NULL) {
                printf("Pedido nao encontrado.\n");
            } 
            else {
                printf("ID: %s\n", p->id_pedido);
                printf("Data: %s\n", p->data);
                printf("ID Usuario: %s\n", p->id_usuario);
                free(p);
            }
            
            break;

        case 2:
            printf("Digite o ID da joia: ");
            fgets(id_busca, sizeof(id_busca), stdin);
            remover_novalinha(id_busca);

            Joia *j = busca_joia_por_id(header_indices->joias, header_indices->quant_indice_joia, id_busca);
            if (j == NULL) {
                printf("Joia nao encontrada.\n");
            } else {
                printf("ID: %s\n", j->id_joia);
                printf("Preco: %.2f\n", j->preco);
                printf("Marca ID: %c\n", j->id_marca);
                printf("ID Categoria: %s\n", j->id_categoria);
                printf("Nome Categoria: %s\n", j->nome_categoria);
                free(j);
            }
            
            break;

        case 0:
            printf("\nSaindo.\n");
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }

        printf("\n===== MENU DE BUSCA =====\n");
        printf("Escolha uma opcao:\n");
        printf("1 - Buscar Pedido por ID\n");
        printf("2 - Buscar Joia por ID\n");
        printf("0 - Sair\n");
        printf("--> ");
        scanf("%d", &opcao);
        getchar();
    }
}
