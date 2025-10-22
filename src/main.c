#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estruturas.h"
#include "leitor.h"
#include "indice.h"
#include "consultas.h"

// ORGANIZACAO DO ARQUIVOS CSV INICIAL
// Order-datetime,Order-ID,Purchased-product-ID,Quantity-of-SKU-in-the-order,Category-ID,Category-alias,Brand-ID,Price-in-USD,User-ID,Product-gender,color,metal,gem

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

    imprimir_indices(&header_indices);
    // mostrar_joias("data/joias.bin");
    // mostrar_pedidos("data/pedidos.bin");

    printf("%d", busca_binaria_indice(header_indices.pedidos, header_indices.quant_indice_pedido, "2687306061257376013"));

    return 0;
}


