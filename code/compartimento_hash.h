#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"

#define TAMANHO_HASH 7
#define REGISTRO_CLIENTE "clientes.dat"
#define TABELA_HASH "tabHash.dat"
#define METADADOS "meta.dat"

Cliente *criarCliente(int codigo, char *nome) {
    Cliente *novo = (Cliente *) malloc(sizeof(Cliente));
    if (novo) memset(novo, 0, sizeof(Cliente));
    // copia valores para os campos de novo
    novo->codCliente = codigo;
    strcpy(novo->nomeCliente, nome);
    novo->codCliente = codigo;
    printf("\n> Novo cliente criado\n");
    return novo;
}

Cliente *busca(FILE *tabhash, FILE *clientes, FILE *meta, int codCliente) {
    int qtd, p, l, posicao;
    Cliente *procurado = (Cliente *) malloc(sizeof(Cliente));
    int validade = 0;

    fread(&qtd, sizeof(int), 1, meta);
    fread(&p, sizeof(int), 1, meta);
    fread(&l, sizeof(int), 1, meta);

    posicao = codCliente % (TAMANHO_HASH * pow(2, l));

    if (posicao < p) {
        posicao = codCliente % (TAMANHO_HASH * pow(2, l + 1));
    }

    rewind(tabhash);
    // printf("Fazendo busca\n");
    if (posicao == 0) {
        fread(&posicao, sizeof(int), 1, tabhash);
    } else {
        fseek(tabhash, sizeof(int) * posicao, SEEK_SET);
        fread(&posicao, sizeof(int), 1, tabhash);
        // printf("li posicao a posicao: %d\n", posicao);
    }
    // printf("li a posicao %d \n", posicao);
    if (posicao != -1) {
        while (validade == 0) {
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);

            fread(&procurado->codCliente, sizeof(int), 1, clientes);
            // printf("codCliente: %d \n", procurado->codCliente);
            fread(procurado->nomeCliente, sizeof(char), sizeof(procurado->nomeCliente), clientes);
            // printf("nomeCliente: %s \n", procurado->nomeCliente);
            fread(&procurado->estadoOcupacao, sizeof(int), 1, clientes);
            // printf("estadoOcupacao: %d \n", procurado->estadoOcupacao);
            fread(&procurado->ponteiroProx, sizeof(int), 1, clientes);
            // printf("ponteiroProximo: %d \n", procurado->ponteiroProx);

            if (procurado->codCliente == codCliente) {
                validade = 1;
            } else if (procurado->ponteiroProx == -1) {
                validade = -1;
                procurado->codCliente = -1;
                // printf("Busca: Chegamos ao fim da fila \n");
            } else {
                posicao = procurado->ponteiroProx;
            }
        }
        return procurado;
    } else {
        // printf("Não tem ninguem cadastrado \n");
        procurado->codCliente = -1;
        return procurado;
    }
}

void inserir(FILE *tabhash, FILE *meta, FILE *clientes, Cliente *info) {
    int posicao, contador, valor, f_carga;
    int validade = 0;
    int qtd, p, l;

    fread(&qtd, sizeof(int), 1, meta);
    fread(&p, sizeof(int), 1, meta);
    fread(&l, sizeof(int), 1, meta);

    Cliente *checagem = (Cliente *) malloc(sizeof(Cliente));
    posicao = info->codCliente % (TAMANHO_HASH * pow(2, l)); // cuidado ao relacionar int com double

    if (posicao < p) {
        posicao = info->codCliente % (TAMANHO_HASH * pow(2, l + 1));
    }
    // printf("Posicao na hash eh %d", posicao);

    checagem = busca(tabhash, clientes, meta, info->codCliente);
    if (checagem->codCliente == info->codCliente) {
        printf("A codCliente escolhida já é cadastrada pelo cliente %s, por favor escolha uma que não esteja em uso \n", checagem->nomeCliente);
        free(checagem);
        // free(info);
        return;
    }

    rewind(tabhash);
    if (posicao != 0) {
        fseek(tabhash, sizeof(int) * (posicao), SEEK_SET);
        fread(&posicao, sizeof(int), 1, tabhash);
    } else {
        fread(&posicao, sizeof(int), 1, tabhash);
    }

    // printf("pos: %d \n", posicao);
    rewind(meta);

    fread(&contador, sizeof(int), 1, meta);
    // printf("Contador %d \n", contador);
    if (posicao != -1) {
        printf("Hash com dados \n");
        while (validade == 0) {
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            // printf("pulo de %d \n", posicao);
            fread(&checagem->codCliente, sizeof(int), 1, clientes);
            fread(checagem->nomeCliente, sizeof(char), sizeof(checagem->nomeCliente), clientes);
            // printf("nomeCliente na fila: %s \n", checagem->nomeCliente);
            fread(&checagem->estadoOcupacao, sizeof(int), 1, clientes);
            fread(&checagem->ponteiroProx, sizeof(int), 1, clientes);

            // printf("%d\n", checagem->ponteiroProx);

            if (checagem->estadoOcupacao == 0) {
                validade = 2;
            } else if (checagem->ponteiroProx == -1) {
                // printf("final da fila encontrado");
                validade = 1;
                rewind(clientes);
                if (posicao != 0) {
                    fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                }
                fread(&checagem->codCliente, sizeof(int), 1, clientes);
                fread(checagem->nomeCliente, sizeof(char), sizeof(checagem->nomeCliente), clientes);
                // printf("nomeCliente: %s \n", checagem->nomeCliente);
                fread(&checagem->estadoOcupacao, sizeof(int), 1, clientes);
                fwrite(&contador, sizeof(int), 1, clientes);
            } else {
                posicao = checagem->ponteiroProx;
            }
        }

        rewind(clientes);
        if (validade == 2) {
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fwrite(&info->codCliente, sizeof(int), 1, clientes);
            fwrite(info->nomeCliente, sizeof(char), sizeof(info->nomeCliente), clientes);
            fwrite(&info->estadoOcupacao, sizeof(int), 1, clientes);
            printf("Cliente cadastrado com sucesso em uma posição vazia");
        }
    }
    rewind(clientes);
    if (validade != 2) {
        printf("\n> Final da fila encontrado ou não existe um inicio da hash!\n");
        fseek(clientes, sizeof(Cliente) * contador, SEEK_SET);
        fwrite(&info->codCliente, sizeof(int), 1, clientes);
        fwrite(info->nomeCliente, sizeof(char), sizeof(info->nomeCliente), clientes);
        fwrite(&info->estadoOcupacao, sizeof(int), 1, clientes);
        fwrite(&info->ponteiroProx, sizeof(int), 1, clientes);

        rewind(clientes);
        fseek(clientes, sizeof(Cliente) * contador, SEEK_SET);
        fread(&checagem->codCliente, sizeof(int), 1, clientes);
        fread(checagem->nomeCliente, sizeof(char), sizeof(info->nomeCliente), clientes);
        fread(&checagem->estadoOcupacao, sizeof(int), 1, clientes);
        fread(&checagem->ponteiroProx, sizeof(int), 1, clientes);
        // printf("codCliente: %d\n", checagem->codCliente);
        // printf("codCliente: %s\n", checagem->nomeCliente);
        if (validade == 0) { // Não existe cliente naquela hash
            printf("> Aplicando posicao na hash...\n");
            rewind(tabhash);
            posicao = info->codCliente % TAMANHO_HASH;
            // printf("Na posicao %d, esta escrito: ", posicao);
            fseek(tabhash, sizeof(int) * (posicao), SEEK_SET);
            fwrite(&contador, sizeof(int), 1, tabhash);
            rewind(tabhash);
            fseek(tabhash, sizeof(int) * (posicao), SEEK_SET);
            fread(&valor, sizeof(int), 1, tabhash);
            // printf("%d \n", valor);
        }
        contador = contador + 1;
        // printf("contador: %d \n", contador);
        rewind(meta);
        fwrite(&contador, sizeof(int), 1, meta);
        rewind(meta);
        fread(&contador, sizeof(int), 1, meta);
        printf("> Contador do metadados: %d \n", contador);
        printf("\nCliente cadastrado(a) com sucesso! \n");
    }
    rewind(meta);
    fseek(meta, sizeof(int), SEEK_SET);
    f_carga = contador / (TAMANHO_HASH * pow(2, l));
    if (f_carga > 0.7) {
        p = p + 1;
        fwrite(&p, sizeof(int), 1, meta);
        expandir(tabhash, meta, clientes);
    }
    if (p == TAMANHO_HASH * pow(2, l)) {
        l = l + 1;
        fwrite(&l, sizeof(int), 1, meta);
    }

    free(checagem);
    // free(info);
}

void expandir(FILE *tabhash, FILE *meta, FILE *clientes) {
    int p, l, contador, posicao, valor, validade, pos_hash, flag;
    int pos_ant, pos_new;
    Cliente *checagem = (Cliente *) malloc(sizeof(Cliente));
    Cliente *info = (Cliente *) malloc(sizeof(Cliente));
    rewind(meta);
    fwrite(&contador, sizeof(int), 1, meta);
    fwrite(&p, sizeof(int), 1, meta);
    fwrite(&l, sizeof(int), 1, meta);
    valor = 0;
    pos_new = -1;
    while (valor < p) {
        rewind(tabhash);
        fseek(tabhash, sizeof(int) * valor, SEEK_SET);
        fread(&posicao, sizeof(int), 1, tabhash);
        flag = 0;

        if (posicao != -1) {
            while (validade == 0) {
                rewind(clientes);
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                // printf("pulo de %d \n", posicao);
                fread(&checagem->codCliente, sizeof(int), 1, clientes);
                fread(checagem->nomeCliente, sizeof(char), sizeof(checagem->nomeCliente), clientes);
                // printf("nomeCliente na fila: %s \n", checagem->nomeCliente);
                fread(&checagem->estadoOcupacao, sizeof(int), 1, clientes);
                fread(&checagem->ponteiroProx, sizeof(int), 1, clientes);

                pos_hash = checagem->codCliente / (TAMANHO_HASH * pow(2, l + 1));

                // printf("%d\n", checagem->ponteiroProx);

                if (pos_hash != valor) {
                    if (flag == 0) {
                        flag = 1;

                        rewind(tabhash);
                        fseek(tabhash, sizeof(Cliente) * pos_hash, SEEK_SET);
                        fwrite(&posicao, sizeof(int), 1, tabhash);
                    }
                    pos_new = posicao;
                    info->codCliente = checagem->codCliente;
                    strcpy(info->nomeCliente, checagem->nomeCliente);
                    info->estadoOcupacao = checagem->estadoOcupacao;
                    info->ponteiroProx = checagem->ponteiroProx;
                } else if (pos_hash == valor) {
                    if (pos_new != -1) {
                        rewind(clientes);
                        fseek(clientes, sizeof(Cliente) * pos_ant, SEEK_SET);
                        fread(&info->codCliente, sizeof(int), 1, clientes);
                        fread(info->nomeCliente, sizeof(char), sizeof(info->nomeCliente), clientes);
                        fread(&info->estadoOcupacao, sizeof(int), 1, clientes);
                        fwrite(&posicao, sizeof(int), 1, clientes);
                    }
                    pos_ant = posicao;
                }
                posicao = checagem->ponteiroProx;
            }
        }
    }
}

// Mostrar o codigo do cliente em forma de estrutura Hash Exterior
void mostrarTabela() {
    FILE *tabhash;
    int valor;
    if ((tabhash = fopen(TABELA_HASH, "rb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela hash\n");
        exit(1);
    }
    rewind(tabhash);
    for (int i = 0; i < TAMANHO_HASH; i++) {
        fread(&valor, sizeof(int), 1, tabhash);
        printf("Posicao %d: %d \n", i, valor);
    }
    fclose(tabhash);
}

void zerarTabela(FILE *tabhash, FILE *meta, FILE *clientes) {
    FILE *nhash;
    FILE *nmeta;
    FILE *nclientes;
    int contador = 0;
    int novo;
    int a = -1;
    int b;

    fclose(tabhash);
    fclose(meta);

    if ((nhash = fopen(TABELA_HASH, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela hash\n");
        exit(1);
    }

    if ((nmeta = fopen(METADADOS, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    for (int i = 0; i < TAMANHO_HASH; i++) {
        fwrite(&a, sizeof(int), 1, nhash);
    }
    fclose(nhash);

    if ((nhash = fopen(TABELA_HASH, "a+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela clientes\n");
        exit(1);
    }

    rewind(nhash);

    printf("\n> Arquivo hash zerado\n");

    rewind(nmeta);
    fwrite(&contador, sizeof(int), 1, nmeta);

    fclose(nmeta);

    if ((nmeta = fopen(METADADOS, "a+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela meta\n");
        exit(1);
    }

    rewind(nmeta);

    fread(&novo, sizeof(int), 1, nmeta);
    printf("> Contador: %d\n", novo);
    printf("> Tabela Clientes zerada\n");
    printf("Arquivos zerados com sucesso!");
    fclose(clientes);

    if ((nclientes = fopen(REGISTRO_CLIENTE, "w+b")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela clientes\n");
        exit(1);
    }
    fclose(nclientes);
    fclose(nhash);
    fclose(nmeta);
}