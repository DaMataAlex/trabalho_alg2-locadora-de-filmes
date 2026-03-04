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
  char cpf[15]; //campo unico da struct
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

void RetomarMenu(USUARIOS *pBancoUsuarios, PLATAFORMAS *pBancoPlataformas, ASSINATURAS *pBancoAssinaturas, int *total_clientes, int *total_plataformas){
    LimparTerminal();

    //retomar menu do cadastro de usuarios
    if(pBancoPlataformas == NULL && pBancoAssinaturas == NULL && total_plataformas == NULL){
        if(pBancoUsuarios[*total_clientes].phone[0] == '\0'){
            printf("-- Cadastro de clientes --\n\n");
            printf("Digite o nome do cliente: ");
            printf("%s\n", pBancoUsuarios[*total_clientes].nome);
            printf("Digite o CPF do cliente: ");
            printf("%s\n", pBancoUsuarios[*total_clientes].cpf);

        }else if(pBancoUsuarios[*total_clientes].email[0] == '\0'){
            printf("-- Cadastro de clientes --\n\n");
            printf("Digite o nome do cliente: ");
            printf("%s\n", pBancoUsuarios[*total_clientes].nome);
            printf("Digite o CPF do cliente: ");
            printf("%s\n", pBancoUsuarios[*total_clientes].cpf);
            printf("Digite o telefone do usuario: ");
            printf("%s\n", pBancoUsuarios[*total_clientes].phone);
        }
    }
}

//======================================
//   FUNCOES DE VALIDACAO E FORMATACAO
//======================================
char *ValidarEFormatarCpf(char cpf_nao_formatado[13]){
    //validando cpf
    //cpf recebido: 00000000000 -> 13 caracteres contando o \0
    //formato que a gente quer: 000.000.000-00 -> 15 caracteres contando o \0

    while(strlen(cpf_nao_formatado) != 12){
        LimparTerminal();
        printf("Por favor, insira um CPF valido!\n");
        printf("(Somente numeros)\n\n");
        fgets(cpf_nao_formatado, 13, stdin);
    }

    char cpf_formatado[15];

    int j = 0;
    for(int i = 0; i < 14; i++){
        if(i == 3 || i == 7){
            cpf_formatado[i] = '.';
        }else if(i == 11){
            cpf_formatado[i] = '-';
        }else{
            cpf_formatado[i] = cpf_nao_formatado[j];
            j++;
        }
    }

    cpf_formatado[strcspn(cpf_formatado, "\n")] = '\0';

    LimparTerminal();

    char *pCPF = cpf_formatado;
    return pCPF;
}

char *ValidarEFormatarTelefone(char telefone[15]){
    //validando telefone
    //formato: (00)90000-0000
    telefone[strcspn(telefone, "\n")] = '\0';
    
    while(telefone[0] != '(' || telefone[3] != ')' || telefone[9] != '-'){
        LimparTerminal();
        printf("Por favor, insira um telefone valido!\n");
        printf("(00)90000-0000\n\n");
        fgets(telefone, 15, stdin);
    }
    telefone[strcspn(telefone, "\n")] = '\0';

    LimparTerminal();

    char *pTelefone = telefone;
    return pTelefone;

}

char *ValidarEFormatarEmail(char email[MAX_CHAR]){
    //validando email
    //precisa ter um @
    int arroba = 0;

    while(1){
        for(int i = 0 ; i < strlen(email); i++){
            if(email[i] == '@'){
                arroba = 1;
                break;
            }
        }

        if(arroba  == 1){
            break;
        }

        LimparTerminal();
        printf("Por favor, insira um Email valido!\n");
        fgets(email, 15, stdin);
        
    }

    email[strcspn(email, "\n")] = '\0';

    LimparTerminal();

    char *pEmail = email;
    return pEmail;

}

char *ValidarURL(char URL[MAX_CHAR]){
    
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

    char *pURL = URL;
    return pURL;
}

//--- GERENCIAMENTO DE CLIENTES ---
void CadastrarNovoCliente(USUARIOS *pBancoUsuarios, int *total_clientes){

    //cadastro do nome
    printf("-- Cadastro de clientes --\n\n");
    printf("Digite o nome do cliente: ");
    fgets(pBancoUsuarios[*total_clientes].nome, MAX_CHAR, stdin);
    pBancoUsuarios[*total_clientes].nome[strcspn(pBancoUsuarios[*total_clientes].nome, "\n")] = '\0';

    //cadastro do cpf
    printf("Digite o CPF do cliente (Somente numeros): ");
    char CPF_nao_formatado[13];
    fgets(CPF_nao_formatado, 13, stdin);
    char *pCPF = ValidarEFormatarCpf(CPF_nao_formatado);
    strcpy(pBancoUsuarios[*total_clientes].cpf, pCPF);
    RetomarMenu(pBancoUsuarios, NULL, NULL, total_clientes, NULL);

    //cadastro do telefone
    printf("Digite o telefone do usuario (00)90000-0000: ");
    fgets(pBancoUsuarios[*total_clientes].phone, 15, stdin);
    char *pTelefoneFormatado = ValidarEFormatarTelefone(pBancoUsuarios[*total_clientes].phone);
    strcpy(pBancoUsuarios[*total_clientes].phone, pTelefoneFormatado);
    RetomarMenu(pBancoUsuarios, NULL, NULL, total_clientes, NULL);
    LimparBuffer();

    //cadastro email
    printf("Digite o Email do usuario: ");
    fgets(pBancoUsuarios[*total_clientes].email, MAX_CHAR, stdin);
    char *pEmailFormatado = ValidarEFormatarEmail(pBancoUsuarios[*total_clientes].email);
    strcpy(pBancoUsuarios[*total_clientes].email, pEmailFormatado);
    RetomarMenu(pBancoUsuarios, NULL, NULL, total_clientes, NULL);

    //cadastrando o id do usuario
    pBancoUsuarios[*total_clientes].id_usuario = 1000 + *total_clientes;

    //mensagem de cadastro concluido
    LimparTerminal();
    printf("Usuario cadastrado com sucesso!\n");
    printf("Id do usuario: %ld\n\n", pBancoUsuarios[*total_clientes].id_usuario);
    getchar();
    LimparTerminal();
}

void BuscarCliente(USUARIOS *pBancoUsuarios, int *total_clientes){
    LimparTerminal();

    printf("Digite o CPF do cliente que deseja buscar: ");
    char CPF[15];
    char *pCPF = CPF;
    fgets(CPF, 15, stdin);
    pCPF = ValidarEFormatarCpf(pCPF);

    int encontrado = 0;



    for(int i = 0; i < *total_clientes; i++){
        if(strcmp(pCPF, pBancoUsuarios[i].cpf) == 0){
            LimparTerminal();
            encontrado = 1;
            printf("Usuario encontrado!\n\n");
            printf("%s\n", pBancoUsuarios[i].nome);
            printf("%s\n", pBancoUsuarios[i].cpf);
            printf("%ld\n", pBancoUsuarios[i].id_usuario);
            printf("%s\n", pBancoUsuarios[i].phone);
            printf("%s\n", pBancoUsuarios[i].email);
            getchar();
            LimparTerminal();

        }
    }

    if(encontrado == 0){
        LimparTerminal();
        printf("Cliente nao encontrado!\n");
        getchar();
        getchar();
        LimparTerminal();
    }

    
}
//--- GERENCIAMENTO DE PLATAFORMAS ---
void CadastrarNovaPlataforma(PLATAFORMAS *pBancoPlataformas, int *total_plataformas){

    //cadastro do nome
    printf("-- Cadastro de plataformas --\n\n");
    printf("Digite o nome da plataforma: ");
    fgets(pBancoPlataformas[*total_plataformas].nome_plataforma, MAX_CHAR, stdin);
    pBancoPlataformas[*total_plataformas].nome_plataforma[strcspn(pBancoPlataformas[*total_plataformas].nome_plataforma, "\n")] = '\0';

    //cadastrando a categoria
    printf("Digite a categoria da plataforma(filmes, musica, etc): ");
    fgets(pBancoPlataformas[*total_plataformas].categoria, MAX_CHAR, stdin);
    pBancoPlataformas[*total_plataformas].categoria[strcspn(pBancoPlataformas[*total_plataformas].categoria, "\n")] = '\0';

    //cadastrando o valor
    printf("Digite o valor do servico: R$");
    scanf("%f", &pBancoPlataformas[*total_plataformas].preco);
    LimparBuffer();

    //cadastrando o site
    printf("Digite o site da plataforma: ");
    fgets(pBancoPlataformas[*total_plataformas].site_url, MAX_CHAR, stdin);
    char *pURL = ValidarURL(pBancoPlataformas[*total_plataformas].site_url);
    strcpy(pBancoPlataformas[*total_plataformas].site_url, pURL);

    //cadastrando o id da plataforma
    pBancoPlataformas[*total_plataformas].id_plataforma = 5000 + *total_plataformas;

    //mensagem de cadastro concluido
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

//funcao principal
int main(){

    //criando o "banco de dados" dos clientes e alocando, inicialmente a quantidade MAX para esse banco
    //fazendo o mesmo para as plataformas e assinaturas, sem usar variaveis globais
    int total_clientes = 0;
    int *pTotalClientes = &total_clientes;
    USUARIOS *pBancoUsuarios = (USUARIOS*) calloc(MAX, sizeof(USUARIOS));

    int total_plataformas = 0;
    int *pTotalPlataformas = &total_plataformas;
    PLATAFORMAS *pBancoPlataformas = (PLATAFORMAS*) calloc(MAX, sizeof(PLATAFORMAS));

    int total_assinaturas = 0;
    int *pTotalAssinaturas = &total_assinaturas;
    ASSINATURAS *pBancoAssinaturas = (ASSINATURAS*) calloc(MAX, sizeof(ASSINATURAS));


    while(1){
        int opcao = MenuPrincipal(opcao);

        if(opcao == 1){
            opcao = GerenciamentoDeClientes();
            if(opcao == 1){
                pBancoUsuarios = realloc (pBancoUsuarios, (total_clientes + 1) * sizeof(USUARIOS));
                CadastrarNovoCliente(pBancoUsuarios, pTotalClientes);
                total_clientes++;
            }else if(opcao == 2){
                BuscarCliente(pBancoUsuarios, pTotalClientes);
            }
        }else if(opcao == 2){
            opcao = GerenciamentoDePlataformas();
            if(opcao == 1){
                pBancoPlataformas = realloc (pBancoPlataformas, (total_plataformas + 1) * sizeof(PLATAFORMAS));
                CadastrarNovaPlataforma(pBancoPlataformas, pTotalPlataformas);
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