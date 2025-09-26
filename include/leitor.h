#ifndef LEITOR_H
#define LEITOR_H

#define TAM_MAX_TOTAL_LINHA 200

//Order-datetime,Order-ID,Purchased-product-ID,Quantity-of-SKU-in-the-order,Category-ID,Category-alias,Brand-ID,Price-in-USD,User-ID,Product-gender,color,metal,gem
enum {
    DATA_PEDIDO,
    ID_PEDIDO,
    ID_PRODUTO,
    QUANTIDADE_SKU,
    ID_CATEGORIA,
    NOME_CATEGORIA,
    ID_MARCA,
    PRECO,
    ID_USUARIO,
    GENERO_PRODUTO,
    COR,
    METAL,
    GEMA
};

typedef struct linha {

}Linha;

#endif