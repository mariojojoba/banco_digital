#include <stdio.h>    // Biblioteca padrão de entrada e saída, usada para funções como printf() e scanf().
#include <stdlib.h>   // Biblioteca para gerenciamento de memória, controle de processos e conversões.
#include <locale.h>   // Biblioteca para configurar localizações, como suporte a acentuação e formatos regionais.
#include <unistd.h>   // Biblioteca para manipulação de chamadas do sistema Unix (Mac e Linux), como sleep().
#include <string.h>   // Biblioteca para manipulação de strings, com funções como strlen(), strcpy(), strcat(), etc.

#define NCLIENTES 999
#define NTRANS 999

// Estrutura para representar um cliente de um sistema financeiro
typedef struct {
    char nome[50];
    char cpf[12];
    char senha[15];
    float saldo;
    int transacoes;
    int contaAtiva; // 0 = Inativa, 1 = Ativa
    int bloqueada;  // 0 = Desbloqueada, 1 = Bloqueada
} Cliente;

// Estrutura para representar um administrador
typedef struct {
    char usuario[20];
    char senha[20];
} Administrador;

// Protótipos das funções
void terminalClear(int clock);
int loginCliente(Cliente clientes[], int numClientes);
void menuCliente(Cliente clientes[], int clienteIndex);
void consultarSaldoExtrato(Cliente clientes[], int clienteIndex);
void verClientes(Cliente clientes[], int numClientes);

int main() {
    setlocale(LC_ALL, "Portuguese"); // Ajuste para aceitar paramentros de escrita em português
    Cliente clientes[NCLIENTES]; // Cria um array de clientes
    int opcao; // Variável de controle do menu
    int numClientes = 0;
  
    do {
        printf("\n****** Bem-vindo ao Banco Digital ******\n");
        printf("1. Login como Cliente\n");
        printf("2. Criar Conta\n");
        printf("3. Login como ADM\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");

        // Condicional para ver se foi digitado apenas números, caso contrário repete o loop 
        if (scanf("%d", &opcao) != 1) {
            terminalClear(0);
            printf("\nEntrada inválida! Digite um número.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                terminalClear(0);
                int clienteIndex = loginCliente(clientes, numClientes);
                if (clienteIndex != -1) {
                    menuCliente(clientes, clienteIndex);
                }
                break;

            case 2:
                terminalClear(0);
                printf("Função criarContaCliente() ainda não implementada.\n");
                break;
            case 3:
                terminalClear(0);
                printf("Função adminLogin() ainda não implementada.\n");
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}

void terminalClear(int clock) {
    sleep(clock + 0.7); // Retarda a limpeza em 7 milésimos de segundo.
    #ifdef _WIN32 // Detecta se o código está ronado em Windowns e passa o comando de limpeza
        system("cls"); 
    #else
        printf("\033[H\033[J"); // Em sistemas Unix-like (Linux, macOS), usa a sequência ANSI para limpar a tela
    #endif
}

int loginCliente(Cliente clientes[], int numClientes) {
    char cpf[12], senha[15];

    printf("Digite seu CPF: ");
    fflush(stdin);  // Limpa o buffer do teclado
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remove a quebra de linha

    printf("Digite sua senha: ");
    fflush(stdin);  // Limpa o buffer do teclado novamente
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = 0;  // Remove a quebra de linha

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0 && strcmp(clientes[i].senha, senha) == 0) {
            if (clientes[i].contaAtiva && !clientes[i].bloqueada) {
                printf("Login realizado com sucesso!\n");
                return i;  // Retorna o índice do cliente autenticado
            } else {
                printf("Conta inativa ou bloqueada!\n");
                return -1;  // Conta não pode ser acessada
            }
        }
    }

    printf("Cliente não encontrado! CPF ou senha incorretos.\n");
    return -1;  // Cliente não encontrado
}

void menuCliente(Cliente clientes[], int clienteIndex) {
    int opcao;
    do {
        printf("\nMenu Cliente\n");
        printf("1. Consultar saldo e extrato\n");
        printf("2. Realizar depósito\n");
        printf("3. Realizar saque\n");
        printf("4. Transferir dinheiro\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n'); // Limpa o buffer
            continue;
        }

        switch (opcao) {
            case 1:
                consultarSaldoExtrato(clientes, clienteIndex);
                break;
            case 2:
                //realizarDeposito(clientes, clienteIndex);
                break;
            case 3:
                //realizarSaque(clientes, clienteIndex);
                break;
            case 4:
                //transferirDinheiro(clientes, clienteIndex);
                break;  
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 5);
}

void consultarSaldoExtrato(Cliente clientes[], int clienteIndex) {
    printf("\n=== Saldo e Extrato ===\n");
    printf("Nome: %s\n", clientes[clienteIndex].nome);
    printf("CPF: %s\n", clientes[clienteIndex].cpf);
    printf("Saldo atual: R$ %.2f\n", clientes[clienteIndex].saldo);
    printf("Total de transações realizadas: %d\n", clientes[clienteIndex].transacoes);
}

void verClientes(Cliente clientes[], int numClientes) {
    if (numClientes == 0) {
        printf("\nNenhum cliente cadastrado no sistema.\n");
        return;
    }

    printf("\n=== Lista de Clientes ===\n");
    for (int i = 0; i < numClientes; i++) {
        printf("\nCliente %d:\n", i + 1);
        printf("Nome: %s\n", clientes[i].nome);
        printf("CPF: %s\n", clientes[i].cpf);
        printf("Saldo: R$ %.2f\n", clientes[i].saldo);
        printf("Conta Ativa: %s\n", clientes[i].contaAtiva ? "Sim" : "Não");
        printf("Conta Bloqueada: %s\n", clientes[i].bloqueada ? "Sim" : "Não");
        printf("--------------------------");
    }
}



