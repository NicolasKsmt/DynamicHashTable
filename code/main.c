#include <stdio.h>
#include <stdlib.h>
#include "clientes.h"
#include "compartimento_hash.h"

// Função para selecionar uma ação do menu
void selecao(FILE *hash, FILE *meta, FILE *clientes) {
    int escolha, codCliente;
    char nomeCliente[100];
    Cliente *novo;

    // Menu de seleção
    printf("\n\nSelecione uma opcao:\n\n");
    printf("1 - Inserir cliente\n");
    printf("2 - Zerar tabela\n");
    printf("3 - Buscar cliente\n");
    printf("4 - Mostrar tabela\n");
    printf("5 - Sair\n");
    printf("\n");
    printf("Sua escolha: ");
    scanf("%d", &escolha);
    printf("\n");

    switch (escolha) {
        case 1:
            printf("Digite o codigo do cliente que voce deseja inserir: ");
            scanf("%d", &codCliente);
            printf("Digite o nome do cliente que voce deseja inserir: ");
            scanf("%s", nomeCliente);
            novo = criarCliente(codCliente, nomeCliente);
            inserir(hash, meta, clientes, novo);
            free(novo);
            printf("Cliente inserido com sucesso.\n");
            break;
        case 2:
            zerarTabela(hash, meta, clientes);
            printf("Tabela zerada com sucesso.\n");
            break;
        case 3:
            printf("Digite o codigo a ser buscado: ");
            scanf("%d", &codCliente);
            novo = busca(hash, clientes, meta, codCliente);
            if (novo->codCliente == -1) {
                printf("Cliente nao encontrado. Por favor, insira-o na Hash primeiro.\n");
            } else {
                printf("> Codigo do cliente: %d\n", novo->codCliente);
                printf("> Nome do cliente: %s\n", novo->nomeCliente);
                printf("> Proximo cliente dessa faixa se encontra no %d lugar\n", novo->ponteiroProx);
            }
            free(novo);
            break;
        case 4:
            mostrarTabela(hash, meta);
            break;
        case 5:
            fclose(hash);
            fclose(meta);
            fclose(clientes);
            printf("Saindo...\n");
            exit(0);
            break;
        default:
            printf("Escolha um dos numeros do menu\n");
            break;
    }
}

int main() {
    FILE *hash;
    FILE *meta;
    FILE *clientes;

    // Abrindo arquivos
    if ((hash = fopen("tabHash.dat", "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela Hash.\n");
        exit(1);
    }
    if ((meta = fopen("meta.dat", "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela Meta.\n");
        fclose(hash); // Fechar o arquivo aberto anteriormente
        exit(1);
    }
    if ((clientes = fopen("clientes.dat", "r+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela Clientes.\n");
        fclose(hash); // Fechar os arquivos abertos anteriormente
        fclose(meta);
        exit(1);
    }

    char continuar = 'y';
    while (continuar == 'y' || continuar == 'Y') {
        selecao(hash, meta, clientes);

        // Verificar se a escolha foi sair
        printf("Deseja continuar? (y/N): ");
        scanf(" %c", &continuar);
    }

    // Fechar arquivos no final da execução
    fclose(hash);
    fclose(meta);
    fclose(clientes);

    return 0;
}