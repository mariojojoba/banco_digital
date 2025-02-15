
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
    int transacoes[NTRANS];
    int contaAtiva; // 0 = Inativa, 1 = Ativa
    int bloqueada;  // 0 = Desbloqueada, 1 = Bloqueada
} Cliente;

// Estrutura para representar um administrador
typedef struct {
    char usuario[20];
    char senha[20];
} Administrador;

void terminal_Clear(int clock);
int login_Cliente(Cliente clientes[], int numClientes);
void menu_Cliente(Cliente clientes[], int clienteIndex);
void consultar_Saldo_Extrato(Cliente clientes[], int clienteIndex);
void ver_Clientes(Cliente clientes[], int numClientes);
void bloqueio_e_Desbloqueio_de_Clientes(Cliente clientes[], int quantidade, int n);
void sacar(Cliente clientes[], int quantidade, int clienteIndex);
void transferir(Cliente clientes[], int quantidade, int clienteIndex);

int main() {
    setlocale(LC_ALL, "Portuguese");

    Cliente clientes[NCLIENTES] = {
        {"João Silva", "12345678901", "senha123", 1000.50, "Nenhuma", 1, 0},
        {"Maria Oliveira", "98765432100", "senha456", 2500.00, "Nenhuma", 1, 0},
        {"Carlos Souza", "11223344556", "senha789", 0.00, "Nenhuma", 1, 0},
        {"Ana Pereira", "22334455667", "senha321", 500.75, "Nenhuma", 1, 0},
        {"Lucas Costa", "99887766554", "senha654", 100.25, "Nenhuma", 1, 0}
    };

    int opcao;
    int numClientes = 5;
    do {
        printf("\n---------- Bem-vindo ao Banco Digital ----------\n");
        printf("1. Login como Cliente\n2. Criar Conta\n3. Login como ADM\n4. Sair\nEscolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            terminal_Clear(0);
            printf("\nEntrada inválida! Digite um número.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                terminal_Clear(0);
                int clienteIndex = login_Cliente(clientes, numClientes);
                if (clienteIndex != -1) {
                    menu_Cliente(clientes, clienteIndex);
                }
                break;
            case 2:
                terminal_Clear(0);
                printf("Função criarContaCliente() ainda não implementada.\n");
                break;
            case 3:
                terminal_Clear(0);
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

void terminal_Clear(int clock) {
    sleep(clock + 0.7);
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
    #endif
}

int login_Cliente(Cliente clientes[], int numClientes) {
    char cpf[12], senha[15];

    printf("Digite seu CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    printf("Digite sua senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = 0;

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0 && strcmp(clientes[i].senha, senha) == 0) {
            if (clientes[i].contaAtiva && !clientes[i].bloqueada) {
                printf("Login realizado com sucesso!\n");
                return i;
            } else {
                printf("Conta inativa ou bloqueada!\n");
                return -1;
            }
        }
    }

    printf("Cliente não encontrado! CPF ou senha incorretos.\n");
    return -1;
}

void menu_Cliente(Cliente clientes[], int clienteIndex) {
    int opcao;
    do {
        printf("\n---------- Menu Cliente ----------\n");
        printf("1. Consultar saldo e extrato\n2. Realizar depósito\n3. Realizar saque\n4. Transferir dinheiro\n5. Sair\nEscolha uma opção: ");
                
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                terminal_Clear(0);
                consultar_Saldo_Extrato(clientes, clienteIndex);
                break;
            case 2:
                terminal_Clear(0);
                //realizarDeposito(clientes, clienteIndex);
                break;
            case 3:
                terminal_Clear(0);
                sacar(clientes, NCLIENTES, clienteIndex);
                break;
            case 4:
                terminal_Clear(0);
                transferir(clientes, NCLIENTES, clienteIndex);
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

void consultar_Saldo_Extrato(Cliente clientes[], int clienteIndex) {
    printf("\n------------- Saldo e Extrato -------------\n");
    printf("Nome: %s\n", clientes[clienteIndex].nome);
    printf("CPF: %s\n", clientes[clienteIndex].cpf);
    printf("Saldo atual: R$ %.2f\n", clientes[clienteIndex].saldo);
    printf("Total de transações realizadas: %s\n", clientes[clienteIndex].transacoes);
    printf("\n------------------------------------------\n");
}

void ver_Clientes(Cliente clientes[], int numClientes) {
    if (numClientes == 0) {
        printf("\nNenhum cliente cadastrado no sistema.\n");
        return;
    }

    printf("\n------------- Lista de Clientes -------------\n");
    for (int i = 0; i < numClientes; i++) {
        printf("\nCliente %d:\n", i + 1);
        printf("Nome: %s\n", clientes[i].nome);
        printf("CPF: %s\n", clientes[i].cpf);
        printf("Saldo: R$ %.2f\n", clientes[i].saldo);
        printf("Conta Ativa: %s\n", clientes[i].contaAtiva ? "Sim" : "Não");
        printf("Conta Bloqueada: %s\n", clientes[i].bloqueada ? "Sim" : "Não");
        printf("\n------------------------------------------\n");
    }
}

void bloqueio_e_Desbloqueio_de_Clientes(Cliente clientes[], int quantidade, int n) {
    char entrada[50];
    printf("------------- Contas bloqueadas -------------\n");
    for (int i = 0; i < quantidade; i++) {
        if (clientes[i].bloqueada) {
            printf("Nome: %s | CPF: %s\n", clientes[i].nome, clientes[i].cpf);
        }
    }
    printf("\n------------- Contas desbloqueadas -------------\n");
    for (int i = 0; i < quantidade; i++) {
        if (!clientes[i].bloqueada) {
            printf("Nome: %s | CPF: %s\n", clientes[i].nome, clientes[i].cpf);
        }
    }
    
    if (n == 0) {
        printf("Digite o nome ou CPF da conta para desbloquear: ");
        scanf("%s", entrada);
        for (int i = 0; i < quantidade; i++) {
            if (strcmp(clientes[i].nome, entrada) == 0 || strcmp(clientes[i].cpf, entrada) == 0) {
                clientes[i].bloqueada = 0;
                printf("Conta de %s desbloqueada.\n", clientes[i].nome);
                return;
            }
        }
        printf("Conta não encontrada.\n");
    } else {
        printf("\nDigite o nome ou CPF da conta para bloquear: ");
        scanf("%s", entrada);
        for (int i = 0; i < quantidade; i++) {
            if (strcmp(clientes[i].nome, entrada) == 0 || strcmp(clientes[i].cpf, entrada) == 0) {
                clientes[i].bloqueada = 1;
                printf("Conta de %s bloqueada.\n", clientes[i].nome);
                return;
            }
        }
        printf("Conta não encontrada.\n");
    }
}

void sacar(Cliente clientes[], int quantidade, int clienteIndex) {
    float valor;
    printf("Digite o valor do saque: ");
    scanf("%f", &valor);

    if (clientes[clienteIndex].saldo < valor) {
        printf("Saldo insuficiente!\n");
        return;
    }

    clientes[clienteIndex].saldo -= valor;
    printf("Saque realizado com sucesso! Novo saldo: R$ %.2f\n", clientes[clienteIndex].saldo);
}

void transferir(Cliente clientes[], int quantidade, int clienteIndex) {
    char cpfDestino[12];
    float valor;
    
    printf("Digite o CPF do destinatário: ");
    scanf("%s", cpfDestino);

    int destinoIndex = -1;
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(clientes[i].cpf, cpfDestino) == 0) {
            destinoIndex = i;
            break;
        }
    }

    if (destinoIndex == -1) {
        printf("Conta de destino não encontrada.\n");
        return;
    }

    printf("Digite o valor a ser transferido: ");
    scanf("%f", &valor);

    if (clientes[clienteIndex].saldo < valor) {
        printf("Saldo insuficiente!\n");
        return;
    }

    // Registrar a transação no extrato de ambos os clientes
    char transacao[100];
    snprintf(transacao, sizeof(transacao), "Transferência de R$ %.2f para %s (CPF: %s)\n", valor, clientes[destinoIndex].nome, clientes[destinoIndex].cpf);
    
    // Adiciona a transação no extrato do cliente remetente
    strcat(clientes[clienteIndex].transacoes, transacao);
    
    // Adiciona a transação no extrato do cliente destinatário
    snprintf(transacao, sizeof(transacao), "Recebido R$ %.2f de %s (CPF: %s)\n", valor, clientes[clienteIndex].nome, clientes[clienteIndex].cpf);
    strcat(clientes[destinoIndex].transacoes, transacao);

    // Realizar a transferência
    clientes[clienteIndex].saldo -= valor;
    clientes[destinoIndex].saldo += valor;

    printf("Transferência realizada com sucesso!\n");
    printf("Novo saldo: R$ %.2f\n", clientes[clienteIndex].saldo);
    printf("Novo saldo do destinatário: R$ %.2f\n", clientes[destinoIndex].saldo);
}
