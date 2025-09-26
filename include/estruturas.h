#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define TAM_MAX 50

typedef struct joia {
    char id_joia[TAM_MAX]; 
    char id_categoria[TAM_MAX];
    char nome_categoria[TAM_MAX];
    float preco;
}Joia;

typedef struct pedido {
    char id_pedido[TAM_MAX]; 
    char data[TAM_MAX];
    char id_usuario[TAM_MAX];
}Pedido;

typedef struct joiapedido {
    char id_pedido[TAM_MAX]; 
    char id_joia[TAM_MAX];
    int quant;
}JoiaPedido;

#endif