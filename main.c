#include "cliente.h"

int main() {
    Cliente *listaClientes = NULL;

    Cliente *cliente1 = cliente(1, "Cliente Um");
    Cliente *cliente2 = cliente(2, "Cliente Dois");
    Cliente *cliente3 = cliente(3, "Cliente Três");

    inserirCliente(&listaClientes, cliente1);
    inserirCliente(&listaClientes, cliente2);
    inserirCliente(&listaClientes, cliente3);

    printf("Lista de Clientes:\n");
    Cliente *atual = listaClientes;
    while (atual) {
        imprimeCliente(atual);
        atual = atual->proximo;
    }

    int codigoBusca = 2;
    Cliente *clienteEncontrado = buscarCliente(listaClientes, codigoBusca);
    if (clienteEncontrado) {
        printf("\nCliente encontrado:\n");
        imprimeCliente(clienteEncontrado);
    } else {
        printf("\nCliente com código %d não encontrado.\n", codigoBusca);
    }

    // Liberação de memória
    Cliente *temp;
    while (listaClientes) {
        temp = listaClientes;
        listaClientes = listaClientes->proximo;
        free(temp);
    }

    return 0;
}
