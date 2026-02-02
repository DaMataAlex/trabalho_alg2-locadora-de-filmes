#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições de constantes para facilitar manutenção
#define TAMANHO_MAXIMO_LISTA 50
#define TAMANHO_STRING_PADRAO 50

// --- Definição das Estruturas de Dados ---

typedef struct {
  char nome_completo[TAMANHO_STRING_PADRAO];
  char cpf[20];
  char telefone[20];
  char email[TAMANHO_STRING_PADRAO];
  long int id_registro;
} Cliente;

typedef struct {
  char nome_plataforma[TAMANHO_STRING_PADRAO];
  char categoria[TAMANHO_STRING_PADRAO];
  char url_site[TAMANHO_STRING_PADRAO];
  long int id_registro;
  float preco_mensal;
} Plataforma;

// --- Variáveis Globais (Banco de Dados em Memória) ---

Cliente lista_de_clientes[TAMANHO_MAXIMO_LISTA];
Plataforma lista_de_plataformas[TAMANHO_MAXIMO_LISTA];

int total_clientes_cadastrados = 0;
int total_plataformas_cadastradas = 0;

long int gerador_id_clientes = 1000;
long int gerador_id_plataformas = 5000;

// Códigos para identificar a operação
const int OPERACAO_CONSULTAR = 1;
const int OPERACAO_ALTERAR = 2;
const int OPERACAO_EXCLUIR = 3;

// --- Funções Auxiliares de Sistema ---

void limpar_tela_terminal() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// ler_entrada_usuario(cpf, 50, "Insira seu CPF")
void ler_entrada_usuario(char *buffer_destino, int tamanho_buffer, const char *mensagem_exibida) {
  if (mensagem_exibida) {
    // Insira seu cpf
    printf("%s", mensagem_exibida);
  }

  // Tenta ler o texto
  if (fgets(buffer_destino, tamanho_buffer, stdin) == NULL) {
    return;
  }

  // Remove o \n usando strcspn
  buffer_destino[strcspn(buffer_destino, "\n")] = '\0';
}

void limpar_numero(const char *entrada, char *saida) {
  char *p = saida;

  for (int i = 0; *(entrada + i) != '\0'; i++) {
    if (*(entrada + i) >= 48 && *(entrada + i) <= 57) {
      *p++ = *(entrada + i);
    }
  }

  *p = '\0';
}

// --- Funções de Validação de Formato ---

// algoritmo de validação de CPF retirado de: https://www.macoratti.net/alg_cpf.htm
int validar_formato_cpf(const char *cpf) {
  char cpf_limpo[12];

  limpar_numero(cpf, cpf_limpo);

  if (strlen(cpf_limpo) != 11) return 0;

  // crio uma copia do ponteiro da string para poder iterar sobre ela sem perder a ref para a string original
  char *p = cpf_limpo;

  // isso aqui vai ver se o cpf não é 111.111.111-11
  int iguais = 1;
  while (*(p + 1)) {
    if (*p != *(p + 1)) {
      iguais = 0;
      break;
    }
    p++;
  }
  if (iguais) return 0;

  int soma, resto, digito1, digito2;
  int peso;

  soma = 0;
  peso = 10;
  p = cpf_limpo;

  for (int i = 0; i < 9; i++) {

    //soma é um int, então ele pega o valor char de p e subtrai '0', ou seja, '8' (em ascii é 56) - '0' que é igual a 48, retornando o int 8
    soma += (*p - '0') * peso;
    p++;
    peso--;
  }

  resto = soma % 11;
  if (resto < 2) digito1 = 0;
  else digito1 = 11 - resto;

  soma = 0;
  peso = 11;
  p = cpf_limpo;

  for (int i = 0; i < 10; i++) {
    soma += (*p - '0') * peso;
    p++;
    peso--;
  }

  resto = soma % 11;
  if (resto < 2) digito2 = 0;
  else digito2 = 11 - resto;

  // escrever assim é o mesmo que escrever cpf_limpo[9] e cpf_limpo[10]
  if ((*(cpf_limpo + 9) - '0') != digito1) return 0;
  if ((*(cpf_limpo + 10) - '0') != digito2) return 0;

  return 1;
}


int validar_formato_telefone(const char *telefone) {
  char numero_formatado[12];
  char tipo; // f = fixo, m = móvel

  limpar_numero(telefone, numero_formatado);

  if (strlen(numero_formatado) == 10) tipo = 'f';
  else if (strlen(numero_formatado) == 11) tipo = 'm';
  else return 0;

  if (tipo == 'm' && *(numero_formatado + 2) != '9') return 0;
  if (tipo == 'f' && *(numero_formatado + 2) < '2' || *(numero_formatado + 2) > '5') return 0;

  return 1;
}

int validar_email(const char *email) {
  int arroba = 0, ponto = 0, a_ponto = 0, d_ponto = 0;

  char *p = email;

  for (int i = 0 ; *(p + 1) != '\0'; i++) {
    if (*(p + i) == '@') {
      if (arroba == 1 || i < 3) return 0;

      arroba = 1;
    }

    if (arroba) {
      if (ponto) {
        d_ponto++;
        continue;
      } 
      
      if (*(p + i) == '.') {
        ponto = 1;

        if (a_ponto < 3) {
          return 0;
        }
      } else {
        a_ponto++;
      }
    }
  }

  if (d_ponto < 1) return 0;

  return 1;
}

int validar_formato_url(const char *url) {
  return (strchr(url, '.') != NULL);
}

void ler_e_validar_entrada(char *buffer, int tamanho, const char *msg, const char *msg_erro, int (*funcao_validadora)(const char *)) {
  ler_entrada_usuario(buffer, tamanho, msg);

  while (!funcao_validadora(buffer)) {
    printf("%s\n", msg_erro);
    ler_entrada_usuario(buffer, tamanho, msg);
  }
}

// --- Funções de Busca ---

int buscar_indice_cliente_por_cpf(const char *cpf_buscado) {
  for (int i = 0; i < total_clientes_cadastrados; i++) {
    if (strcmp(lista_de_clientes[i].cpf, cpf_buscado) == 0) {
      return i;
    }
  }
  return -1;
}

int buscar_indice_plataforma_por_nome(const char *nome_buscado) {
  for (int i = 0; i < total_plataformas_cadastradas; i++) {
    if (strcasecmp(lista_de_plataformas[i].nome_plataforma, nome_buscado) == 0) {
      return i; //Netflix == netflix
    }
  }
  return -1;
}

// --- Funções de Cadastro ---

void realizar_cadastro_cliente() {
  if (total_clientes_cadastrados >= TAMANHO_MAXIMO_LISTA) {
    printf("Erro: Limite de clientes atingido!\n");
    getchar();
    return;
  }

  Cliente *novo_cliente = &lista_de_clientes[total_clientes_cadastrados];

  limpar_tela_terminal();
  printf("--- Cadastro de Novo Cliente ---\n\n");

  ler_entrada_usuario(novo_cliente->nome_completo, TAMANHO_STRING_PADRAO, "Nome Completo: ");

  ler_e_validar_entrada(novo_cliente->cpf, 20, "CPF (000.000.000-00): ", "CPF Inválido!", validar_formato_cpf);

  ler_e_validar_entrada(novo_cliente->telefone, 20, "Telefone (00) 00000-0000: ", "Número Inválido!", validar_formato_telefone);

  ler_e_validar_entrada(novo_cliente->email, TAMANHO_STRING_PADRAO, "E-mail: ", "E-mail invalido!", validar_email);

  // Salva ID e incrementa
  novo_cliente->id_registro = gerador_id_clientes++;
  total_clientes_cadastrados++;

  printf("\nSucesso! Cliente cadastrado com ID: %ld\n", novo_cliente->id_registro);
  printf("Pressione ENTER para continuar...");
  getchar();
}

void realizar_cadastro_plataforma() {
  if (total_plataformas_cadastradas >= TAMANHO_MAXIMO_LISTA) {
    printf("Erro: Limite de plataformas atingido!\n");
    getchar();
    return;
  }

  Plataforma *nova_plataforma = &lista_de_plataformas[total_plataformas_cadastradas];

  limpar_tela_terminal();
  printf("--- Cadastro de Nova Plataforma ---\n\n");

  ler_entrada_usuario(nova_plataforma->nome_plataforma, TAMANHO_STRING_PADRAO, "Nome da Plataforma: ");

  ler_entrada_usuario(nova_plataforma->categoria, TAMANHO_STRING_PADRAO, "Categoria (ex: Filmes, Musica): ");

  printf("Valor da Mensalidade: R$ ");
  scanf("%f", &nova_plataforma->preco_mensal);
  getchar(); // Consumir a quebra de linha do scanf

  ler_e_validar_entrada(nova_plataforma->url_site, TAMANHO_STRING_PADRAO, "Site Oficial: ", "URL invalida!", validar_formato_url);

  // Salva ID e incrementa
  nova_plataforma->id_registro = gerador_id_plataformas++;
  total_plataformas_cadastradas++;

  printf("\nSucesso! Plataforma cadastrada com ID: %ld\n", nova_plataforma->id_registro);
  printf("Pressione ENTER para continuar...");
  getchar();
}

// ==========================================================
// MÓDULO ESPECÍFICO: GERENCIAMENTO DE CLIENTES
// ==========================================================

void gerenciar_clientes(int tipo_operacao) {
  char termo_busca[20];
  int indice_encontrado = -1;

  limpar_tela_terminal();

  ler_entrada_usuario(termo_busca, 20, "Digite o CPF do cliente para busca: ");
  indice_encontrado = buscar_indice_cliente_por_cpf(termo_busca);

  if (indice_encontrado == -1) {
    printf("\nErro: Cliente nao encontrado.\nENTER para voltar.");
    getchar();
    return;
  }

  Cliente *cliente_alvo = &lista_de_clientes[indice_encontrado];

  // --- CONSULTAR ---
  if (tipo_operacao == OPERACAO_CONSULTAR) {
    printf("\n=== Detalhes do Cliente ===\n");
    printf("ID: %ld\n", cliente_alvo->id_registro);
    printf("Nome: %s\n", cliente_alvo->nome_completo);
    printf("CPF: %s\n", cliente_alvo->cpf);
    printf("Telefone: %s\n", cliente_alvo->telefone);
    printf("E-mail: %s\n", cliente_alvo->email);

    printf("\nPressione ENTER para voltar.");
    getchar();
  }

  // --- ALTERAR ---
  else if (tipo_operacao == OPERACAO_ALTERAR) {
    int opcao_submenu;

    do {
      limpar_tela_terminal();
      printf("--- Editando: %s ---\n\n", cliente_alvo->nome_completo);
      printf("1. Nome\n");
      printf("2. CPF\n");
      printf("3. Telefone\n");
      printf("4. E-mail\n");
      printf("5. Finalizar Edicao\n\n");
      printf("Opcao: ");

      scanf("%d", &opcao_submenu);
      getchar();

      if (opcao_submenu == 1) {
        ler_entrada_usuario(cliente_alvo->nome_completo, TAMANHO_STRING_PADRAO, "Novo Nome: ");
      } else if (opcao_submenu == 2) {
        ler_e_validar_entrada(cliente_alvo->cpf, 20, "Novo CPF: ", "Invalido!", validar_formato_cpf);
      } else if (opcao_submenu == 3) {
        ler_e_validar_entrada(cliente_alvo->telefone, 20, "Novo Telefone: ", "Invalido!", validar_formato_telefone);
      } else if (opcao_submenu == 4) {
        ler_e_validar_entrada(cliente_alvo->email, TAMANHO_STRING_PADRAO, "Novo E-mail: ", "Invalido!", validar_email);
      }

      if (opcao_submenu >= 1 && opcao_submenu <= 4) {
        printf("\nSucesso! ENTER para continuar.");
        getchar();
      }

    } while (opcao_submenu != 5);
  }

  // --- EXCLUIR ---
  else if (tipo_operacao == OPERACAO_EXCLUIR) {
    for (int i = indice_encontrado; i < total_clientes_cadastrados - 1; i++) {
      lista_de_clientes[i] = lista_de_clientes[i + 1];
    }

    total_clientes_cadastrados--;

    printf("\nCliente excluido!\nENTER para continuar.");
    getchar();
  }
}

// ==========================================================
// MÓDULO ESPECÍFICO: GERENCIAMENTO DE PLATAFORMAS
// ==========================================================

void gerenciar_plataformas(int tipo_operacao) {
  char termo_busca[TAMANHO_STRING_PADRAO];
  int indice_encontrado = -1;

  limpar_tela_terminal();

  ler_entrada_usuario(termo_busca, TAMANHO_STRING_PADRAO, "Digite o Nome da plataforma para busca: ");
  indice_encontrado = buscar_indice_plataforma_por_nome(termo_busca);

  if (indice_encontrado == -1) {
    printf("\nErro: Plataforma nao encontrada.\nENTER para voltar.");
    getchar();
    return;
  }

  Plataforma *plataforma_alvo = &lista_de_plataformas[indice_encontrado];

  // --- CONSULTAR ---
  if (tipo_operacao == OPERACAO_CONSULTAR) {
    printf("\n=== Detalhes da Plataforma ===\n");
    printf("ID: %ld\n", plataforma_alvo->id_registro);
    printf("Nome: %s\n", plataforma_alvo->nome_plataforma);
    printf("Categoria: %s\n", plataforma_alvo->categoria);
    printf("Valor: R$ %.2f\n", plataforma_alvo->preco_mensal);
    printf("Site: %s\n", plataforma_alvo->url_site);

    printf("\nPressione ENTER para voltar.");
    getchar();
  }

  // --- ALTERAR ---
  else if (tipo_operacao == OPERACAO_ALTERAR) {
    int opcao_submenu;

    do {
      limpar_tela_terminal();
      printf("--- Editando: %s ---\n\n", plataforma_alvo->nome_plataforma);
      printf("1. Nome\n");
      printf("2. Categoria\n");
      printf("3. Valor Mensal\n");
      printf("4. Site\n");
      printf("5. Finalizar Edicao\n\n");
      printf("Opcao: ");

      scanf("%d", &opcao_submenu);
      getchar();

      if (opcao_submenu == 1) {
        ler_entrada_usuario(plataforma_alvo->nome_plataforma, TAMANHO_STRING_PADRAO, "Novo Nome: ");
      } else if (opcao_submenu == 2) {
        ler_entrada_usuario(plataforma_alvo->categoria, TAMANHO_STRING_PADRAO, "Nova Categoria: ");
      } else if (opcao_submenu == 3) {
        printf("Novo Valor: R$ ");
        scanf("%f", &plataforma_alvo->preco_mensal);
        getchar();
      } else if (opcao_submenu == 4) {
        ler_e_validar_entrada(plataforma_alvo->url_site, TAMANHO_STRING_PADRAO, "Novo Site: ", "Invalido!", validar_formato_url);
      }

      if (opcao_submenu >= 1 && opcao_submenu <= 4) {
        printf("\nSucesso! ENTER para continuar.");
        getchar();
      }

    } while (opcao_submenu != 5);
  }

  // --- EXCLUIR ---
  else if (tipo_operacao == OPERACAO_EXCLUIR) {
    for (int i = indice_encontrado; i < total_plataformas_cadastradas - 1; i++) {
      lista_de_plataformas[i] = lista_de_plataformas[i + 1];
    }

    total_plataformas_cadastradas--;

    printf("\nPlataforma excluida!\nENTER para continuar.");
    getchar();
  }
}

// --- Menus do Sistema ---

int exibir_submenu_e_obter_escolha(const char *titulo_submenu) {
  limpar_tela_terminal();
  int escolha_usuario;

  printf("=== %s ===\n\n", titulo_submenu);
  printf("1. Gerenciar Clientes\n");
  printf("2. Gerenciar Plataformas\n");
  printf("3. Voltar ao Menu Principal\n\n");
  printf("Digite sua opcao: ");

  scanf("%d", &escolha_usuario);
  getchar(); // Consumir quebra de linha
  return escolha_usuario;
}

int main() {
  int opcao_menu_principal;

  while (1) {
    limpar_tela_terminal();
    printf("SISTEMA DE GESTAO DE ASSINATURAS\n\n");
    printf("1. Realizar Cadastro\n");
    printf("2. Consultar Dados\n");
    printf("3. Alterar Informacoes\n");
    printf("4. Excluir Dados\n");
    printf("5. Sair do Sistema\n\n");
    printf("Escolha uma opcao: ");

    scanf("%d", &opcao_menu_principal);
    getchar(); // Consumir quebra de linha

    if (opcao_menu_principal == 5) {
      printf("Encerrando sistema...\n");
      break;
    }

    if (opcao_menu_principal < 1 || opcao_menu_principal > 5) {
      printf("Opcao invalida! Tente novamente.");
      getchar();
      continue;
    }

    // --- Navegação ---

    if (opcao_menu_principal == 1) { // CADASTRO
      int escolha = exibir_submenu_e_obter_escolha("MENU DE CADASTRO");

      if (escolha == 1) {
        realizar_cadastro_cliente();
      } else if (escolha == 2) {
        realizar_cadastro_plataforma();
      }
    } else { // CONSULTAR (2), ALTERAR (3), EXCLUIR (4)
      char *titulos_menus[] = { "", "MENU DE CONSULTA", "MENU DE ALTERACAO", "MENU DE EXCLUSAO" };

      int escolha = exibir_submenu_e_obter_escolha(titulos_menus[opcao_menu_principal - 1]); // -1 pq array começa em 0

      // Define o tipo de operação baseado no menu principal
      int tipo_operacao = opcao_menu_principal - 1;

      if (escolha == 1) {
        gerenciar_clientes(tipo_operacao);
      } else if (escolha == 2) {
        gerenciar_plataformas(tipo_operacao);
      }
    }
  }
  return 0;
}