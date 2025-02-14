#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define MAXIMO_CLIENTES 5

// Definição da estrutura de Cliente
typedef struct {
  char nome[50];
  char senha[15];
  float saldo;
  int transacoes;
  int contaAtiva; // 0 = Inativa, 1 = Ativa
  int bloqueada;  // 0 = Desbloqueada, 1 = Bloqueada
} Cliente;

// Definição da estrutura de Administrador
typedef struct {
  char nome[50];
  char senha[20];
} Administrador;

void entrarContaAdministrador(Administrador *adm,Cliente *clientes,int numClientes);
void ativarContaCliente(Cliente *cliente,int numC);
//void exibirContas();
//void bloquearDesbloquearConta();
//void removerConta();
void cadastrarCliente(Cliente *clientes, int *numCliente);
//void entrarContaCliente();

int main() {
  setlocale(LC_ALL, "Portuguese");
  Administrador admin = {"adm", "senha123"};  // Administrador pré-cadastrado
  Cliente clientes[MAXIMO_CLIENTES];
  int numClientes = 0;
  int opcao;

  while (1) {
      printf("\n----- Banco Digital -----\n");
      printf("1. Entrar na conta de administrador\n");
      printf("2. Cadastrar um novo cliente\n");
      printf("3. Entrar na conta de cliente\n");
      printf("4. Sair\n");
      printf("Escolha uma opcao: ");
      scanf("%d", &opcao);
      getchar(); // Limpar o buffer de entrada

      if (opcao == 1) {
          system("cls");
          entrarContaAdministrador(&admin, clientes, numClientes);
      } else if (opcao == 2) {
          system("cls");
          cadastrarCliente(clientes, &numClientes);
      } else if (opcao == 3) {
          system("cls");
          //entrarContaCliente();
          break;
      } else if (opcao == 4) {
          break;
      } else {
          system("cls");
          printf("Opcao invalida!\n");
      }
  }

  return 0;
}

void entrarContaAdministrador(Administrador *adm,Cliente *clientes,int numClientes){
  char nome[50], senha[20];
  int t = 1;
  printf("Digite o nome de usuario: ");
  fgets(nome, sizeof(nome), stdin);
  nome[strcspn(nome, "\n")] = 0;  // Remover a quebra de linha

  printf("Digite a senha: ");
  fgets(senha, sizeof(senha), stdin);
  senha[strcspn(senha, "\n")] = 0;

  system("cls");

  int opcao;
  if(strcmp(nome, adm->nome) == 0 && strcmp(senha, adm->senha) == 0){
    while(t == 1){
      printf("\n----- Ola %s. Esse e o seu Menu! -----\n", nome);
      printf("1) Ativa conta do cliente\n");
      printf("2) Exibir contas\n");
      printf("3) Bloquear ou desbloquear conta\n");
      printf("4) Remover conta\n");
      printf("5) Sair do Administrador\n");
      printf("Escolha uma opcao: ");
      scanf("%d", &opcao);
      switch(opcao){
        case 1:
          system("cls");
          ativarContaCliente(clientes, numClientes);
          break;
        case 2:
          system("cls");
          //exibirContas();
          break;
        case 3:
          system("cls");
          //bloquearDesbloquearConta();
          break;
        case 4: 
          system("cls");
          //removerConta();
          break;
        case 5: t = 0; system("cls"); break;
      }
    }
  }else{
    printf("Nome ou/e senha incorreto(s)!\n");
  }
  return;
}

void ativarContaCliente(Cliente *cliente,int numeroClientes){
  char senha[15];
  printf("Digite o senha do cliente para ativar a conta: ");
  fgets(senha, sizeof(senha), stdin);
  senha[strcspn(senha, "\n")] = 0;  // Remover a quebra de linha

  for (int i = 0; i < numeroClientes; i++) {
      if (strcmp(cliente[i].senha, senha) == 0) {
          cliente[i].contaAtiva = 1;
          printf("Conta de %s ativada com sucesso!\n", cliente[i].nome);
          return;
      }
  }
  system("cls");
  printf("Cliente nao encontrado ou nao existe!\n");

  return;
}

void cadastrarCliente(Cliente *clientes, int *numeroClientes){
  if (*numeroClientes >= MAXIMO_CLIENTES) {
    printf("Limite de clientes atingido.\n");
    return;
  }
  Cliente novoCliente;
  printf("Digite o nome do cliente: ");
  fgets(novoCliente.nome, sizeof(novoCliente.nome), stdin);
  novoCliente.nome[strcspn(novoCliente.nome, "\n")] = 0; // Remover a quebra de linha

  printf("Digite o senha do cliente: ");
  fgets(novoCliente.senha, sizeof(novoCliente.senha), stdin);
  novoCliente.senha[strcspn(novoCliente.senha, "\n")] = 0; // Remover a quebra de linha

  novoCliente.saldo = 0;
  novoCliente.transacoes = 0;
  novoCliente.contaAtiva = 0;
  novoCliente.bloqueada = 0;

  clientes[*numeroClientes] = novoCliente;
  (*numeroClientes)++;
  printf("Cliente cadastrado com sucesso!\n");
}