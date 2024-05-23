#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct Cliente{
    int codCliente;
    char nomeCliente[100];
    int estadoOcupacao;
    int ponteiroProx;
}Cliente;

#endif

