#ifndef CLIENTE_H
#define CLIENTE_H


typedef struct Cliente {
    int codCliente;
    char nomeCliente[100];
    int estadoOcupacao; // 1 para ocupado, 0 para livre
    struct Cliente *proximo; // Ponteiro para o próximo cliente na lista encadeada
} Cliente;

// Funções públicas para manipulação de clientes
void imprimeCliente(Cliente *cliente);
Cliente *criarCliente(int codCliente, const char *nomeCliente);
void inserirCliente(Cliente **lista, Cliente *novoCliente);
Cliente *buscarCliente(Cliente *lista, int codCliente);

#endif // CLIENTE_H
