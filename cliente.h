#ifndef CLIENTE_H
#define CLIENTE_H

#include<stdio.h>
#include<stdlib.h>

typedef struct Cliente {

    int codCliente;
    char nomeCliente[100];
    int estadoOcupacao; // true || false
    int ponteiroProx;

}Cliente;

// Perguntar

#endif