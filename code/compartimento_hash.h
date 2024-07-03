#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "clientes.h"

#define FATOR_CARGA 0.7
#define TAMANHO_HASH 7
#define REGISTRO_CLIENTE "clientes.dat"
#define TABELA_HASH "tabHash.dat"
#define METADADOS "meta.dat"

Cliente *criarCliente(int codigo, char *nome) {
    Cliente *novo = (Cliente *) malloc(sizeof(Cliente));
    if (novo == NULL) {
        fprintf(stderr, "Erro na alocação de memória para novo cliente.\n");
        return NULL;
    }
    memset(novo, 0, sizeof(Cliente));
    // copia valores para os campos de novo
    novo->codCliente = codigo;
    strcpy(novo->nomeCliente, nome);
    novo->ponteiroProx = -1;
    novo->estadoOcupacao = 1;
    printf("\n> Novo cliente criado\n");
    return novo;
}

Cliente *busca(FILE *tabhash, FILE *clientes, FILE *meta, int codCliente) {
    int qtd, p, l, posicao, mod;
    //printf("entrei na busca\n");
    Cliente *procurado = (Cliente *) malloc(sizeof(Cliente));
    if (!procurado) {
        fprintf(stderr, "Erro na alocação de memória para busca.\n");
        return NULL;
    }

    fread(&qtd, sizeof(int), 1, meta);
    fread(&p, sizeof(int), 1, meta);
    fread(&l, sizeof(int), 1, meta);

    mod = (int)(TAMANHO_HASH * pow(2, l));
    posicao = codCliente % mod;
    if (posicao < p) {
        mod = (int)(TAMANHO_HASH * pow(2, l + 1));
        posicao = codCliente % mod;
    }

    //printf("entrando no loop\n");
    rewind(tabhash);
    fseek(tabhash, sizeof(int) * posicao, SEEK_SET);
    fread(&posicao, sizeof(int), 1, tabhash);

    if (posicao != -1) {
        while (1) {
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            fread(&procurado->codCliente, sizeof(int), 1, clientes);
            fread(procurado->nomeCliente, sizeof(char), sizeof(procurado->nomeCliente), clientes);
            fread(&procurado->estadoOcupacao, sizeof(int), 1, clientes);
            fread(&procurado->ponteiroProx, sizeof(int), 1, clientes);

            /*
            printf("procurado->codCliente = %d\n", procurado->codCliente);
            printf("proc->nomeCliente = %s\n", procurado->nomeCliente);
            printf("procurado->ponteiroProx = %d\n", procurado->ponteiroProx);
            printf("procurado->estadoOcupacao = %d\n", procurado->estadoOcupacao);
            */
            
            if (procurado->codCliente == codCliente) {
                return procurado;
            } else if (procurado->ponteiroProx == -1) {
                break;
            } else {
                posicao = procurado->ponteiroProx;
            }
        }
    }

    procurado->codCliente = -1;
    return procurado;
}

void expandir(FILE *tabhash, FILE *meta, FILE *clientes) {
    int p, l, contador, posicao, valor, f_carga;
    Cliente *checagem = (Cliente *) malloc(sizeof(Cliente));

    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    fread(&p, sizeof(int), 1, meta);
    fread(&l, sizeof(int), 1, meta);

    valor = 0;

    while (valor < p) {
        rewind(tabhash);
        fseek(tabhash, sizeof(int) * valor, SEEK_SET);
        fread(&posicao, sizeof(int), 1, tabhash);

        if (posicao != -1) {
            int raiz = 0;
            int consec = 0;
            int pos_ant, pos_new;

            while (posicao != -1) {
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                fread(checagem, sizeof(Cliente), 1, clientes);

                int pos_hash = checagem->codCliente % (int)(TAMANHO_HASH * pow(2, l + 1));

                if (pos_hash != valor) {
                    if (raiz == 0) {
                        fseek(tabhash, sizeof(int) * valor, SEEK_SET);
                        fwrite(&checagem->ponteiroProx, sizeof(int), 1, tabhash);
                    }
                    if (consec != 0) {
                        fseek(clientes, sizeof(Cliente) * pos_ant, SEEK_SET);
                        fwrite(&posicao, sizeof(int), 1, clientes);
                    }
                    fseek(tabhash, sizeof(int) * pos_hash, SEEK_SET);
                    fwrite(&posicao, sizeof(int), 1, tabhash);

                    pos_new = posicao;
                    consec = 1;
                } else {
                    raiz = 1;
                    pos_ant = posicao;
                }
                posicao = checagem->ponteiroProx;
            }
        }
        valor++;
    }

    free(checagem);
}

void inserir(FILE *tabhash, FILE *meta, FILE *clientes, Cliente *info) {
    int posicao, contador, valor, f_carga, mod;
    int validade = 0;
    int qtd, p, l;

    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    fread(&p, sizeof(int), 1, meta);
    fread(&l, sizeof(int), 1, meta);

    //printf("ta aqui\n");
    mod = (int)(TAMANHO_HASH * pow(2, l));
    Cliente *checagem = (Cliente *) malloc(sizeof(Cliente));
    posicao = info->codCliente %  mod; // cuidado ao relacionar int com double

    printf("ta aqui 2\n");
    //ERRO: travou aqui
    //printf("ta aqui 3\n");

    if (posicao < p) {
        printf("ta aqui if posicao < p\n");
        mod =  (int)(TAMANHO_HASH * pow(2, l + 1));
        posicao = info->codCliente % mod;
        printf("ta aqui tam  %d\n", posicao); // ERRO: ultimo print antes de travar
        
    }
    // printf("Posicao na hash eh %d", posicao);

    checagem = busca(tabhash, clientes, meta, info->codCliente);
    printf("ta aqui 4\n");
    if (checagem->codCliente == info->codCliente) {
        printf("A codigo escolhido ja e cadastrado pelo cliente %s, por favor escolha uma que nao esteja em uso \n", checagem->nomeCliente);
        free(checagem);
        free(info);
        return;
    }

    printf("ta aqui pre rewind\n");
    rewind(tabhash);
    printf("ta aqui pos rewind\n");

    if (posicao != 0) {
        fseek(tabhash, sizeof(int) * (posicao), SEEK_SET);
        fread(&posicao, sizeof(int), 1, tabhash);
    } else {
        fread(&posicao, sizeof(int), 1, tabhash);
    }
    printf("ta aqui pos fseek\n");

     printf("pos: %d \n", posicao);
    
    // printf("Contador %d \n", contador);
    if (posicao != -1) {
        printf("Hash com dados \n");
        while (validade == 0) {
            //printf("ta aqui no while");
            rewind(clientes);
            fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
            // printf("pulo de %d \n", posicao);
            fread(&checagem->codCliente, sizeof(int), 1, clientes);
            fread(checagem->nomeCliente, sizeof(char), sizeof(checagem->nomeCliente), clientes);
            // printf("nomeCliente na fila: %s \n", checagem->nomeCliente);
            fread(&checagem->estadoOcupacao, sizeof(int), 1, clientes);
            fread(&checagem->ponteiroProx, sizeof(int), 1, clientes);

             printf("%d\n", checagem->ponteiroProx);
             printf("%d\n", posicao);

            if (checagem->estadoOcupacao == 0) {
                validade = 2;
            } else if (checagem->ponteiroProx == -1) {
                // printf("final da fila encontrado");
                validade = 1;
                rewind(clientes);
          
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                // reescrevendo ponteiro prox, para o final do arquivo
                fread(&checagem->codCliente, sizeof(int), 1, clientes);
                fread(checagem->nomeCliente, sizeof(char), sizeof(checagem->nomeCliente), clientes);
                // printf("nomeCliente: %s \n", checagem->nomeCliente);
                fread(&checagem->estadoOcupacao, sizeof(int), 1, clientes);
                fwrite(&contador, sizeof(int), 1, clientes);

                // lendo o cliente que eu acabei de reescrever
                rewind(clientes);
                fseek(clientes, sizeof(Cliente) * posicao, SEEK_SET);
                fread(&checagem->codCliente, sizeof(int), 1, clientes);
                fread(checagem->nomeCliente, sizeof(char), sizeof(checagem->nomeCliente), clientes);
                // printf("nomeCliente: %s \n", checagem->nomeCliente);
                fread(&checagem->estadoOcupacao, sizeof(int), 1, clientes);
                fread(&checagem->ponteiroProx, sizeof(int), 1, clientes);


                printf("x codCliente: %d\n", checagem->codCliente);
                printf("x nomeCliente: %s\n", checagem->nomeCliente);
                printf("x estadoOcupacao: %d\n", checagem->estadoOcupacao);
                printf("x ponteiroProx: %d\n", checagem->ponteiroProx);
                printf("x contador: %d\n", contador);
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
            printf("Cliente cadastrado com sucesso em uma posicao vazia\n");
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
    f_carga = contador / (int)(TAMANHO_HASH * pow(2, l));
    if (f_carga > FATOR_CARGA) {
        p = p + 1;
        fwrite(&p, sizeof(int), 1, meta);
        expandir(tabhash, meta, clientes);
    }
    if (p == (int)(TAMANHO_HASH * pow(2, l))) {
        l = l + 1;
        fwrite(&l, sizeof(int), 1, meta);
    }

    free(checagem);
    // free(info);
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

void zerarTabela() {
    FILE *tabhash;
    FILE *meta;
    FILE *clientes;
    int contador = 0;
    int a = -1;
    int p = 0;
    int l = 0;

    if ((tabhash = fopen(TABELA_HASH, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo da tabela hash\n");
        exit(1);
    }

    if ((clientes = fopen(REGISTRO_CLIENTE, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo de clientes\n");
        fclose(tabhash);  // Certifique-se de fechar tabhash se ocorrer um erro
        exit(1);
    }

    if ((meta = fopen(METADADOS, "wb")) == NULL) {
        printf("Erro ao abrir o arquivo de metadados\n");
        fclose(tabhash);  // Certifique-se de fechar tabhash e clientes se ocorrer um erro
        fclose(clientes);
        exit(1);
    }

    rewind(tabhash);
    for (int i = 0; i < TAMANHO_HASH; i++) {
        fwrite(&a, sizeof(int), 1, tabhash);
    }

    printf("\n> Arquivo hash zerado\n");

    rewind(meta);
    fwrite(&contador, sizeof(int), 1, meta);
    fwrite(&p, sizeof(int), 1, meta);
    fwrite(&l, sizeof(int), 1, meta);

    rewind(meta);
    fread(&contador, sizeof(int), 1, meta);
    fread(&p, sizeof(int), 1, meta);
    fread(&l, sizeof(int), 1, meta);
    printf("> Contador: %d\n", contador);
    printf("> Tabela Clientes zerada\n");
    printf("Existem %d listas estendidas, e a tabela foi estendida %d vezes\n", p, l);

    fclose(tabhash);
    fclose(meta);
    fclose(clientes);
}
