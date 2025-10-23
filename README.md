Para compilar:

```bash
gcc -I./include src/*.c -o program
```

Structs salvas nos arquivos binários:
```bash
typedef struct joia {
    char id_joia[TAM_MAX];
    char id_categoria[TAM_MAX];
    char nome_categoria[TAM_MAX];
    char id_marca;
    float preco;
}Joia;
```

```bash
typedef struct pedido {
    char id_pedido[TAM_MAX]; 
    char data[TAM_MAX];
    char id_usuario[TAM_MAX];
    char ativo;
}Pedido;
```

Struct para armazenar índices:
```bash
typedef struct indice {
    char id[TAM_MAX];
    long offset;
}Indice;
```

```bash
typedef struct header {
    int quant_indice_joia;
    int quant_indice_pedido;
    Indice *joias;
    Indice *pedidos;
}HeaderIndice;
```


