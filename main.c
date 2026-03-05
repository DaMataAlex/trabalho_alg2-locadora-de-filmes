#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 50
#define MAX_CHAR 50

//=========================
//  PRINCIPAIS  STRUCTS
//=========================
typedef struct {
  char nome[MAX];
  char cpf[16]; //campo unico da struct
  long int id_usuario;
  char phone[15];
  char email[MAX];

}USUARIOS;

typedef struct {
  char nome_plataforma[MAX]; //campo unico da struct
  long int id_plataforma;
  char categoria[MAX];
  float preco;
  char site_url[MAX];

}PLATAFORMAS;

typedef struct {
  USUARIOS usuario_assinante;
  PLATAFORMAS plataforma_contratada;
  int id_assinatura; //campo unico da struct
  int ativa;
  float valor_pago;

}ASSINATURAS;

//=========================
//   FUNCOES AUXILIARES
//=========================
void LimparTerminal(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void LimparBuffer(){
    int c;

    while((c = getchar()) != '\n' && c != EOF);
}

//============================
//   FUNCOES DE VALIDACAO
//============================
//essa funcao de validar cpf ta dando problema quando eu digito mais que 16 digitos
void ValidarCpf(char *cpf){
    //000.000.000-00
    //usuario vai ficar aqui ate digitar nesse formato
    while(1){
        cpf[strcspn(cpf, "\n")] = '\0';

        if(strlen(cpf) == 14 && cpf[3] == '.' && cpf[7] == '.' && cpf[11] == '-'){
            break;
        }

        LimparTerminal();
        printf("CPF invalido.\n");
        printf("Digite novamente (000.000.000-00): ");
        fgets(cpf, 16, stdin);
    }
    LimparTerminal();
}

void ValidarTelefone(char *telefone){
    //(00)0000-0000
    while(1){
        telefone[strcspn(telefone, "\n")] = '\0';

        if(strlen(telefone) == 13 && telefone[0] == '(' && telefone[3] == ')' && telefone[8] == '-'){
            break;
        }

        LimparTerminal();
        printf("Telefone invalido.\n");
        printf("Digite novamente (00)0000-0000: ");
        fgets(telefone, 15, stdin);
    }
    LimparTerminal();

}

void ValidarEmail(char *email){
    //precisa ter um @
    int arroba = 0;

    while(1){
        email[strcspn(email, "\n")] = '\0';
        for(int i = 0 ; i < strlen(email); i++){
            if(email[i] == '@'){
                arroba = 1;
                return;
            }
        }


        LimparTerminal();
        printf("Por favor, insira um Email valido!\n");
        fgets(email, MAX_CHAR, stdin);

    }

    LimparTerminal();

}

void ValidarURL(char *URL){

    int tem_ponto = 0;
    for(int i = 0; URL[i] != '\n'; i++){
        if(URL[i] == '.'){
            tem_ponto = 1;
            break;
        }
    }

    while(tem_ponto == 0){
        LimparTerminal();
        printf("Digite um site valido:\n");
        fgets(URL, MAX_CHAR, stdin);
        for(int i = 0; URL[i] != '\n'; i++){
            if(URL[i] == '.'){
                tem_ponto = 1;
                break;
            }
        }
    }

    URL[strcspn(URL,"\n")] = '\0';

    LimparTerminal();
}

//===================================
//    GERENCIAMENTO DE CLIENTES
//===================================
void CadastrarNovoCliente(USUARIOS *BancoUsuarios, int *total_clientes){

    //cadastro do nome
    printf("-- Cadastro de clientes --\n\n");
    printf("Digite o nome do cliente: ");
    fgets(BancoUsuarios[*total_clientes].nome, MAX_CHAR, stdin);
    BancoUsuarios[*total_clientes].nome[strcspn(BancoUsuarios[*total_clientes].nome, "\n")] = '\0';
    LimparTerminal();

    //cadastro do cpfcpf
    printf("Digite o CPF do cliente (000.000.000-00): ");
    fgets(BancoUsuarios[*total_clientes].cpf, 16, stdin);
    ValidarCpf(BancoUsuarios[*total_clientes].cpf);

    //cadastro do telefone
    printf("Digite o telefone do usuario (00)0000-0000: ");
    fgets(BancoUsuarios[*total_clientes].phone, 15, stdin);
    ValidarTelefone(BancoUsuarios[*total_clientes].phone);

    //cadastro email
    printf("Digite o Email do usuario: ");
    fgets(BancoUsuarios[*total_clientes].email, MAX_CHAR, stdin);
    ValidarEmail(BancoUsuarios[*total_clientes].email);

    //cadastrando o id do usuario
    BancoUsuarios[*total_clientes].id_usuario = 1000 + *total_clientes;

    //mensagem de cadastro concluido
    LimparTerminal();
    printf("Usuario cadastrado com sucesso!\n");
    printf("Id do usuario: %ld\n\n", BancoUsuarios[*total_clientes].id_usuario);
    printf("Pressione ENTER para continuar.\n\n");
    getchar();
    LimparTerminal();
}

void BuscarCliente(USUARIOS *BancoUsuarios, int *total_clientes){
    LimparTerminal();

    printf("Digite o CPF do cliente que deseja buscar: ");
    char CPF[16];
    fgets(CPF, 16, stdin);
    ValidarCpf(CPF);

    int encontrado = 0;

    for(int i = 0; i < *total_clientes; i++){
        if(strcmp(CPF, BancoUsuarios[i].cpf) == 0){
            LimparTerminal();
            encontrado = 1;
            printf("Usuario encontrado!\n\n");
            printf("Nome: %s\n", BancoUsuarios[i].nome);
            printf("CPF: %s\n", BancoUsuarios[i].cpf);
            printf("Id do usuario: %ld\n", BancoUsuarios[i].id_usuario);
            printf("Telefone: %s\n", BancoUsuarios[i].phone);
            printf("Email: %s\n\n", BancoUsuarios[i].email);
            printf("Pressione ENTER para retornar.\n\n");
            getchar();
            LimparTerminal();

        }
    }

    if(encontrado == 0){
        LimparTerminal();
        printf("Cliente nao encontrado!\n");
        printf("Pressione ENTER para retornar.\n\n");
        getchar();
        LimparTerminal();
    }

}

void AlterarDadosClientes(USUARIOS *BancoUsuarios, int *total_clientes){
    printf("Digite o CPF do cliente o qual os dados serao alterados: ");
    char CPF[16];
    fgets(CPF, 16, stdin);
    ValidarCpf(CPF);

    int encontrado = 0;
    int opcao_alteracao = 0;
    int localizador;

    for(localizador = 0; localizador < *total_clientes; localizador++){
        if(strcmp(CPF, BancoUsuarios[localizador].cpf) == 0){
            LimparTerminal();
            encontrado = 1;
            printf("Usuario encontrado!\n");

            do{
                printf("Qual dado do usuario sera alterado?\n\n");
                printf("1 - Nome: %s\n", BancoUsuarios[localizador].nome);
                printf("2 - CPF: %s\n", BancoUsuarios[localizador].cpf);
                printf("3 - Telefone: %s\n", BancoUsuarios[localizador].phone);
                printf("4 - Email: %s\n", BancoUsuarios[localizador].email);
                printf("5 - Retornar.\n\n");
                printf("Escolha uma opcao: ");

                scanf("%d", &opcao_alteracao);
                getchar();

                LimparTerminal();
            }while(opcao_alteracao < 1 || opcao_alteracao > 5);

            if(opcao_alteracao == 1){
                LimparTerminal();
                char novo_nome[MAX_CHAR];

                printf("Digite o novo nome do usuario:\n");
                fgets(novo_nome, MAX_CHAR, stdin);
                novo_nome[strcspn(novo_nome, "\n")] = '\0';  // Remover '\n'

                if(strlen(novo_nome) > 0){
                    strcpy(BancoUsuarios[localizador].nome, novo_nome);
                    LimparTerminal();
                    printf("Nome do usuario alterado!\n\n");
                } else {
                    LimparTerminal();
                    printf("Nome nao pode ser vazio.\n\n");
                }
            }else if(opcao_alteracao == 2){
                LimparTerminal();

                printf("Digite o novo CPF do usuario (000.000.000-00): ");
                fgets(BancoUsuarios[localizador].cpf, 16, stdin);
                ValidarCpf(BancoUsuarios[localizador].cpf);

                LimparTerminal();
                printf("CPF do usuario alterado!\n\n");

            }else if(opcao_alteracao == 3){
                LimparTerminal();
                printf("Digite o novo telefone do usuario (00)0000-0000: ");
                fgets(BancoUsuarios[localizador].phone, 15, stdin);
                ValidarTelefone(BancoUsuarios[localizador].phone);

                LimparTerminal();
                printf("Telefone do usuario alterado!\n\n");

            }else if(opcao_alteracao == 4){
                LimparTerminal();
                printf("Digite o novo email do usuario: ");
                fgets(BancoUsuarios[localizador].email, MAX_CHAR, stdin);
                ValidarEmail(BancoUsuarios[localizador].email);

                LimparTerminal();
                printf("Email do usuario alterado!\n\n");
            }else{
                LimparTerminal();
                return;
            }

            printf("Pressione ENTER para retornar.\n");
            getchar();

            LimparTerminal();
            return;
        }
    }

    if(encontrado == 0){
        LimparTerminal();
        printf("Usuario nao encontrado!\n");
        printf("Pressione ENTER para retornar.\n\n");

        getchar();
        LimparTerminal();

    }
}

void ExcluirCliente(USUARIOS *BancoUsuarios, int *total_clientes){
    char CPF[16];
    int localizador;
    int encontrado = 0;
    int confirmacao;

    printf("Digite o CPF do cliente que sera excluido: ");
    fgets(CPF, 16, stdin);
    ValidarCpf(CPF);

    for(localizador = 0; localizador < *total_clientes; localizador++){
        if(strcmp(CPF, BancoUsuarios[localizador].cpf) == 0){
            LimparTerminal();
            encontrado = 1;
            printf("Usuario encontrado!\n");

            do{
                printf("Nome: %s\n", BancoUsuarios[localizador].nome);
                printf("CPF: %s\n", BancoUsuarios[localizador].cpf);
                printf("Telefone: %s\n", BancoUsuarios[localizador].phone);
                printf("Email: %s\n\n", BancoUsuarios[localizador].email);
                printf("Confirma a exclusao?\n");
                printf("1 - Sim\n");
                printf("2 - Nao\n\n");

                scanf("%d", &confirmacao);
                getchar();

                LimparTerminal();
            }while(confirmacao < 1 || confirmacao > 2);

            if(confirmacao == 2){
            LimparTerminal();
            printf("Operacao cancelada!\n");
            printf("Pressione ENTER para continuar.\n\n");

            getchar();

            LimparTerminal();
            return;

            }else{
                for(int i=localizador; i < *total_clientes - 1; i++){ //O localizador tem a posicao do usuario a ser excluido, entao todos os usuarios que estao na frente dele (na direita), serao jogados uma casa a esquerda
                    BancoUsuarios[i] = BancoUsuarios[i + 1];
                }

                (*total_clientes)--; //Diminui uma unidade de cliente cadastrado na vari�vel l� na main

                LimparTerminal();
                printf("Cliente excluido com sucesso!\n");
                printf("Pressione ENTER para retornar.\n\n");
                getchar();
                LimparTerminal();
                return;
            }

        }


    }

    if(encontrado == 0){
        LimparTerminal();
        printf("Usuario nao encontrado!\n");
        printf("Pressione ENTER para retornar.\n\n");

        LimparBuffer();
        getchar();
        LimparTerminal();

    }

}
//===================================
//    GERENCIAMENTO DE PLATAFORMAS
//===================================
void CadastrarNovaPlataforma(PLATAFORMAS *pBancoPlataformas, int *total_plataformas){

    printf("-- Cadastro de plataformas --\n\n");
    printf("Digite o nome da plataforma: ");
    fgets(pBancoPlataformas[*total_plataformas].nome_plataforma, MAX_CHAR, stdin);
    pBancoPlataformas[*total_plataformas].nome_plataforma[strcspn(pBancoPlataformas[*total_plataformas].nome_plataforma, "\n")] = '\0';
    LimparTerminal();

    printf("Digite a categoria da plataforma(filmes, musica, etc): ");
    fgets(pBancoPlataformas[*total_plataformas].categoria, MAX_CHAR, stdin);
    pBancoPlataformas[*total_plataformas].categoria[strcspn(pBancoPlataformas[*total_plataformas].categoria, "\n")] = '\0';
    LimparTerminal();

    printf("Digite o valor do servico: R$");
    scanf("%f", &pBancoPlataformas[*total_plataformas].preco);
    LimparBuffer();
    LimparTerminal();

    printf("Digite o site da plataforma: ");
    fgets(pBancoPlataformas[*total_plataformas].site_url, MAX_CHAR, stdin);
    ValidarURL(pBancoPlataformas[*total_plataformas].site_url);

    pBancoPlataformas[*total_plataformas].id_plataforma = 5000 + *total_plataformas;

    LimparTerminal();
    printf("Plataforma cadastrada com sucesso!\n");
    printf("Id da plataforma: %ld\n\n", pBancoPlataformas[*total_plataformas].id_plataforma);
    getchar();
    LimparTerminal();
}

//--- MENUS ---
int MenuPrincipal(int opcao){
    printf("== Plataforma de Gerenciamento de Assinaturas ==\n\n");
    printf("1 - Gerenciamento de clientes.\n");
    printf("2 - Gerenciamento de plataformas.\n");
    printf("3 - Gerenciamento de assinaturas.\n");
    printf("4 - Sair.\n\n");
    printf("Escolha uma opcao: ");

    scanf("%d", &opcao);
    getchar();//consumindo a quebra de linha

    while(opcao < 1 || opcao > 4){
        LimparTerminal();
        printf("Digite uma opcao valida!\n");
        return MenuPrincipal(opcao);
    }

    LimparTerminal();
    return opcao;
}

int GerenciamentoDeClientes(){
    printf("-- Gerenciamento de clientes --\n\n");
    printf("1 - Cadastrar um novo cliente.\n");
    printf("2 - Buscar um cliente.\n");
    printf("3 - Alterar dados de um cliente.\n");
    printf("4 - Excluir um cliente.\n");
    printf("5 - Voltar.\n\n");
    printf("Escolha uma opcao: ");

    int opcao;
    scanf("%d", &opcao);
    getchar();//consumindo a quebra de linha

    while(opcao < 1 || opcao > 5){
        LimparTerminal();
        printf("Escolha uma opcao valida!\n");
        return GerenciamentoDeClientes();
    }

    LimparTerminal();

    return opcao;

}

int GerenciamentoDePlataformas(){
    printf("-- Gerenciamento de plataformas --\n\n");
    printf("1 - Cadastrar uma nova plataforma.\n");
    printf("2 - Buscar uma plataforma.\n");
    printf("3 - Alterar dados de uma plataforma.\n");
    printf("4 - Excluir uma plataforma.\n");
    printf("5 - Voltar.\n\n");
    printf("Escolha uma opcao: ");

    int opcao;
    scanf("%d", &opcao);
    getchar();//consumindo a quebra de linha

    while(opcao < 1 || opcao > 5){
        LimparTerminal();
        printf("Escolha uma opcao valida!\n");
        return GerenciamentoDePlataformas();
    }

    LimparTerminal();

    return opcao;

}

//===========================================
//    FUNCOES DE GERENCIAMENTO DE ARQUIVOS
//===========================================
int procurar_arquivo(USUARIOS **BancoUsuarios, int *total_clientes, PLATAFORMAS **BancoPlataformas, int *total_plataformas, ASSINATURAS **BancoAssinaturas, int *total_assinaturas){

    FILE *fArquivoBin = fopen("BancoDeDados.bin", "rb");
    int opcao_arquivo = 0;

    if(fArquivoBin == NULL){
        printf("Nao foi possivel localizar/abrir um arquivo existente nesse computador.\n");
        printf("Um novo sera criado.\n");
        printf("Pressione ENTER para continuar.\n\n");
        getchar();
        LimparTerminal();

        return 0;// retornando 0 pq deu errado
    }else{

        while(opcao_arquivo < 1 || opcao_arquivo > 2){
            printf("Foi localizado um arquivo existente nesse computador, deseja utiliza-lo?\n\n");
            printf("1 - Sim, irei utilizar o arquivo.\n");
            printf("2 - Nao, irei sobrescrever o arquivo.\n\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao_arquivo);
            getchar();
            LimparTerminal();
        }
    }

    if(opcao_arquivo == 2){
        fclose(fArquivoBin);
        return 0; //return 0 se nao abriu o arquivo
    }

    //lendo os clientes que estao no arquivo
    fread(total_clientes, sizeof(int), 1, fArquivoBin);

    if(*total_clientes > 0){
        *BancoUsuarios = (USUARIOS*) malloc(*total_clientes * sizeof(USUARIOS));
        fread(*BancoUsuarios, sizeof(USUARIOS), *total_clientes, fArquivoBin);
    }

    //lendo as plataformas que estao no arquivo
    fread(total_plataformas, sizeof(int), 1, fArquivoBin);

    if(*total_plataformas > 0){
        *BancoPlataformas = (PLATAFORMAS*) malloc(*total_plataformas * sizeof(PLATAFORMAS));
        fread(*BancoPlataformas, sizeof(PLATAFORMAS), *total_plataformas, fArquivoBin);
    }

    //lendo as assinaturas que estao no arquivo
    fread(total_assinaturas, sizeof(int), 1, fArquivoBin);

    if(*total_assinaturas > 0){
        *BancoAssinaturas = (ASSINATURAS*) malloc(*total_assinaturas * sizeof(ASSINATURAS));
        fread(*BancoAssinaturas, sizeof(ASSINATURAS), *total_assinaturas, fArquivoBin);
    }

    fclose(fArquivoBin);
    LimparTerminal();
    printf("Dados carregados com sucesso!\n");
    printf("Pressione ENTER para continuar.\n\n");
    getchar();

    return 1;//retornando 1 pq o programa conseguiu abrir e ler o arquivo com sucesso
}

//funcao principal
int main(){

    int total_clientes = 0;
    int total_plataformas = 0;
    int total_assinaturas = 0;

    USUARIOS *pBancoUsuarios = NULL;
    PLATAFORMAS *pBancoPlataformas = NULL;
    ASSINATURAS *pBancoAssinaturas = NULL;

    procurar_arquivo(&pBancoUsuarios, &total_clientes, &pBancoPlataformas, &total_plataformas,&pBancoAssinaturas, &total_assinaturas);

    while(1){
        int opcao = MenuPrincipal(opcao);

        if(opcao == 1){
            opcao = GerenciamentoDeClientes();
            if(opcao == 1){
                pBancoUsuarios = realloc(pBancoUsuarios, (total_clientes + 1) * sizeof(USUARIOS));
                CadastrarNovoCliente(pBancoUsuarios, &total_clientes);
                total_clientes++;
            }else if(opcao == 2){
                BuscarCliente(pBancoUsuarios, &total_clientes);
            }else if(opcao == 3){
                LimparTerminal();
                AlterarDadosClientes(pBancoUsuarios, &total_clientes);
            }else{
                LimparTerminal();
                ExcluirCliente(pBancoUsuarios, &total_clientes);
                if(total_clientes > 0){
                    pBancoUsuarios = realloc(pBancoUsuarios, total_clientes * sizeof(USUARIOS));

                }else if(total_clientes == 0 && pBancoUsuarios == NULL){
                    free(pBancoUsuarios);
                    pBancoAssinaturas == NULL; //Devo deixar o ponteiro como nulo, para quando houver o cadastro do primeiro cliente, o realloc nao da problema
                }
            }
        }else if(opcao == 2){
            opcao = GerenciamentoDePlataformas();
            if(opcao == 1){
                pBancoPlataformas = realloc(pBancoPlataformas, (total_plataformas + 1) * sizeof(PLATAFORMAS));
                CadastrarNovaPlataforma(pBancoPlataformas, &total_plataformas);
                total_plataformas++;
            }else if(opcao == 2){
                exit(0);
            }else if(opcao == 3){
                exit(0);
            }
        }else if(opcao == 3){
            exit(0);
        }else{
            exit(0);
        }
    }

    return 0;
}
