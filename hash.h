#include "cliente.h"
#include <string.h>

// Função para imprimir os detalhes de um cliente
void imprimeCliente(Cliente *cliente) {
    if (cliente) {
        printf("Código do Cliente: %d\n", cliente->codCliente);
        printf("Nome do Cliente: %s\n", cliente->nomeCliente);
        printf("Estado de Ocupação: %s\n", cliente->estadoOcupacao ? "Ocupado" : "Livre");
        printf("Ponteiro Próximo: %p\n", (void*)cliente->proximo);
    } else {
        printf("Cliente não encontrado.\n");
    }
}

// Função para criar um cliente
Cliente *criarCliente(int codCliente, const char *nomeCliente) {
    Cliente *novo = (Cliente *)malloc(sizeof(Cliente));
    if (novo) {
        memset(novo, 0, sizeof(Cliente));
        novo->codCliente = codCliente;
        strncpy(novo->nomeCliente, nomeCliente, sizeof(novo->nomeCliente) - 1);
        novo->estadoOcupacao = 1; // Ocupado
        novo->proximo = NULL; // Para a lista encadeada
    }
    return novo;
}

// Função para inserir um cliente na lista de clientes
void inserirCliente(Cliente **lista, Cliente *novoCliente) {
    if (!lista || !novoCliente) return;
    Cliente *atual = *lista;
    if (!atual) {
        *lista = novoCliente;
    } else {
        while (atual->proximo) {
            atual = atual->proximo;
        }
        atual->proximo = novoCliente;
    }
}

// Função para buscar um cliente na lista pelo código do cliente
Cliente *buscarCliente(Cliente *lista, int codCliente) {
    Cliente *atual = lista;
    while (atual) {
        if (atual->codCliente == codCliente) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}
