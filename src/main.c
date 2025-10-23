#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estruturas.h"
#include "leitor.h"
#include "indice.h"
#include "consultas.h"
#include "alteracao_arq.h"

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

    return 0;
}

void menu_busca(HeaderIndice *header_indices) {
    int opcao;
    char id_busca[TAM_MAX];
    Pedido pedidonovo;

    printf("\n===== MENU DE BUSCA =====\n");
    printf("Escolha uma opcao:\n");
    printf("1 - Buscar Pedido por ID\n");
    printf("2 - Buscar Joia por ID\n");
    printf("3 - Buscar Pedidos por DIA/MES/ANO\n");
    printf("4 - Resumo/Estatisticas Precos Joias\n");
    printf("5 - Inserir novo pedido\n");
    printf("6 - Remover pedido\n");
    printf("999 - funcoes secretas\n");
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
        
        case 3:
            printf("Digite a data (formato YYYY-MM-DD): ");
            char data_busca[11];
            fgets(data_busca, sizeof(data_busca), stdin);
            data_busca[strcspn(data_busca, "\n")] = '\0'; // remove '\n'

            listar_pedidos_por_data("data/pedidos.bin", data_busca);

            break;
        
        case 4:
            resumo_preco_joias("data/joias.bin");
            break;
        
        case 5:
            printf("Digite o ID do pedido: ");
            scanf("%s", pedidonovo.id_pedido);
            printf("Digite a Data do Pedido (YYYY-MM-DD): ");
            scanf("%s", pedidonovo.data);
            printf("Digite o ID do usuário: ");
            scanf("%s", pedidonovo.id_usuario);

            inserir_pedido_ordenado("data/pedidos.bin", pedidonovo);

            break;

        case 6:
            printf("Digite o ID do pedido a ser removido: ");
            char id_busca[TAM_MAX];
            scanf("%s", id_busca);

            remover_pedido_logicamente("data/pedidos.bin", id_busca);

            break;

        case 999:
            printf("1 - Imprimir indices\n");
            printf("2 - Mostrar todos os pedidos\n");
            printf("3 - Mostrar todas as joias\n");
            printf("4 - Reorganizar arquivo de pedidos\n");
            printf("5 - Reconstruir indice de pedidos\n");
            printf("--> ");

            int opcao_secreta;
            scanf("%d", &opcao_secreta);
            getchar();

            switch (opcao_secreta) {

                case 1:
                    imprimir_indices(header_indices);
                    break;
                case 2:
                    mostrar_pedidos("data/pedidos.bin");
                    break;
                case 3:
                    mostrar_joias("data/joias.bin");
                    break;
                case 4:
                    reorganizararq_pedidos("data/pedidos.bin");
                    break;
                case 5:
                    header_indices->pedidos = construir_indice_pedidos(header_indices);
                    break;
                default:
                    printf("Opcao invalida.\n");
                    break;
            }

            break;

        case 0:
            printf("\nSaindo.\n");

            reorganizararq_pedidos("data/pedidos.bin");
            construir_indice_pedidos(header_indices);

            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }

        printf("\n===== MENU DE BUSCA =====\n");
        printf("Escolha uma opcao:\n");
        printf("1 - Buscar Pedido por ID\n");
        printf("2 - Buscar Joia por ID\n");
        printf("3 - Buscar Pedidos por DIA/MES/ANO\n");
        printf("4 - Resumo/Estatisticas Precos Joias\n");
        printf("5 - Inserir novo pedido\n");
        printf("6 - Remover pedido\n");
        printf("999 - funcoes secretas\n");
        printf("0 - Sair\n");
        printf("--> ");
        scanf("%d", &opcao);
        getchar();
    }
}
