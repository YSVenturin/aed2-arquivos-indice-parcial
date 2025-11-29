#include <stdio.h>

#include "criptografia.h"

void cifra_xor(Pedido *p) {
    unsigned char *bytes = (unsigned char *)p; // le o pedido como bytes e nao como struct

    // Percorre byte a byte do pedido fazer XOR com cada byte
    for (size_t i = 0; i < TAM_MAX; i++) {
        p->id_pedido[i] ^= CHAVE;
        p->id_usuario[i] ^= CHAVE;
        p->data[i] ^= CHAVE;
    }
    p->ativo ^= CHAVE;
}
