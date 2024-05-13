#include"cliente.h"
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>

// Funcoes a se utilizar

void imprimeCliente(){
    // codigo
}

// Criar cliente
Cliente *cliente(int codCliente, char *nomeCliente){
    Cliente *novo = (Cliente *) malloc(sizeof(Cliente));
    if (novo) memset(novo, 0, sizeof(Cliente));
    novo->codCliente = codCliente;
    strcpy(novo->nomeCliente, nomeCliente);
    novo->estadoOcupacao = 1;
    novo->ponteiroProx = -1;
    // printf("\n> Cliente Criado!\n");
    // codigo
    return novo;
}

void inserirCliente(){
    // codigo
}

void buscarCliente(){
    // codigo
}