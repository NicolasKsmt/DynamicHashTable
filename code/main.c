#include <stdio.h>
#include <stdlib.h>
#include "clientes.h"
#include "compartimento_hash.h"


void selecao(FILE *hash, FILE *meta, FILE *clientes){
    int escolha, codCliente;
    char nomeCliente[100];
    Cliente *novo;


    //printf("");
    //printf("");
    scanf("%d", &escolha);
    switch (escolha)
    {
    case (1) :
        printf("Digite o número da codCliente do cliente que você deseja inserir: \n");
        scanf("%d", &codCliente);
        printf("Digite o nomeCliente do cliente que você deseja inserir:\n");
        scanf("%s", nomeCliente);
        novo = criarCliente(codCliente, nomeCliente);
        inserir(hash, meta, clientes, novo);
        free(novo);
        fclose(hash);
        fclose(meta);
        fclose(clientes);
        //printf("fechando arquivos e ponteiros");
        break;
    //zerar tabela
    case(2):
        zerarTabela(hash, meta, clientes);
        free(novo);
        break;
    case (3):
        printf("Digite a codCliente do cliente que você quer buscar:\n");
        scanf("%d", &codCliente);

        novo = busca(hash, clientes, codCliente);
        if(novo->codCliente == -1){
            printf("Cliente não encontrado,por favor insira ele na Hash primeiro");
        }
        else{
            printf("> codCliente do cliente: %d \n", novo->codCliente);
            printf("> Nome do cliente: %s \n", novo->nomeCliente);
            printf("> Proximo cliente dessa faixa se encontra no ' %d ' lugar", novo->ponteiroProx);
        }
        free(novo);
        fclose(hash);
        fclose(meta);
        fclose(clientes);
        break;
    case(5):
        mostrarTabela(hash, meta);
        break;
    case (6):
        free(novo);
        fclose(hash);
        fclose(meta);
        fclose(clientes);
        exit(1);
        break;
    default:
        printf("Escolha um dos números do menu\n");
        break;
    }

}

int main(){
    FILE *hash;
    FILE *meta;
    FILE *clientes;

    if ((hash = fopen("tabHash.dat", "r+b")) == NULL){
    printf("Erro ao abrir o arquivo da tabela Hash");
    exit(1);
    }
    if ((meta = fopen("meta.dat", "r+b")) == NULL){
    printf("Erro ao abrir o arquivo da tabela Meta");
    exit(1);
    }

    if ((clientes = fopen("clientes.dat", "r+b")) == NULL){
    printf("Erro ao abrir o arquivo da tabela Clientes");
    exit(1);
    }

    selecao(hash, meta, clientes);

    return 0;
}
