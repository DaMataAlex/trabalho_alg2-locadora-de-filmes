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

typedef struct {
  int id_usuario;
  int id_plataforma;
  int id_assinatura; //campo unico da struct
  int ativa;
  float valor_pago;

} Assinatura;

// --- Variáveis Globais (Banco de Dados em Memória) ---

Cliente lista_de_clientes[TAMANHO_MAXIMO_LISTA];
Plataforma lista_de_plataformas[TAMANHO_MAXIMO_LISTA];
Assinatura lista_de_assinaturas[TAMANHO_MAXIMO_LISTA];

int total_clientes_cadastrados = 0;
int total_plataformas_cadastradas = 0;
int total_assinaturas_cadastradas = 0;

long int gerador_id_clientes = 1000;
long int gerador_id_plataformas = 5000;
long int gerador_id_assinaturas = 10000;

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

// Função de leitura segura
void ler_entrada_usuario(char *buffer_destino, int tamanho_buffer, const char *mensagem_exibida) {
  if (mensagem_exibida) {
    printf("%s", mensagem_exibida);
  }

  // Tenta ler o texto
  if (fgets(buffer_destino, tamanho_buffer, stdin) == NULL) {
    return;
  }

  // Remove o \n usando strcspn
  buffer_destino[strcspn(buffer_destino, "\n")] = '\0';
}

// --- Funções de Validação de Formato ---

int validar_formato_cpf(const char *texto_cpf) {
  return(texto_cpf[3] == '.' && texto_cpf[7] == '.' && texto_cpf[11] == '-');
}

int validar_formato_telefone(const char *texto_telefone) {
  return (texto_telefone[0] == '(' && texto_telefone[3] == ')' && texto_telefone[10] == '-');
}

int validar_presenca_arroba(const char *texto_email) {
  return (strchr(texto_email, '@') != NULL);
}

int validar_formato_url(const char *texto_url) {
  return (strchr(texto_url, '.') != NULL);
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
      return i;
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

  ler_e_validar_entrada(novo_cliente->cpf, 20, "CPF (000.000.000-00): ", "Formato invalido!", validar_formato_cpf);

  ler_e_validar_entrada(novo_cliente->telefone, 20, "Telefone (00) 00000-0000: ", "Formato invalido!", validar_formato_telefone);

  ler_e_validar_entrada(novo_cliente->email, TAMANHO_STRING_PADRAO, "E-mail: ", "E-mail invalido!", validar_presenca_arroba);

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
  return -1;
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
        ler_e_validar_entrada(cliente_alvo->email, TAMANHO_STRING_PADRAO, "Novo E-mail: ", "Invalido!", validar_presenca_arroba);
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
      char *titulos_menus[] = { "MENU DE CONSULTA", "MENU DE ALTERACAO", "MENU DE EXCLUSAO" };

      // Reutiliza o menu
      int escolha = exibir_submenu_e_obter_escolha(titulos_menus[opcao_menu_principal - 2]);

      // Define o tipo de operação baseado no menu principal (2->1, 3->2, 4->3)
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