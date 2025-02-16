#include <stdio.h>    // Biblioteca padrão de entrada e saída, usada para funções como printf() e scanf().
#include <stdlib.h>   // Biblioteca para gerenciamento de memória, controle de processos e conversões.
#include <locale.h>   // Biblioteca para configurar localizações, como suporte a acentuação e formatos regionais.
#include <unistd.h>   // Biblioteca para manipulação de chamadas do sistema Unix (Mac e Linux), como sleep().
#include <string.h>   // Biblioteca para manipulação de strings, com funções como strlen(), strcpy(), strcat(), etc.

// Variavéis Globais de Controle de Array
#define NCLIENTES 999
#define NTRANS 999

// Estrutura para representar um cliente
typedef struct {
    char nome[50];
    char cpf[12];
    char senha[15];
    float saldo;
    char transacoes[NTRANS];
    int contaAtiva; // 0 = Ativa, 1 = Desativada
    int bloqueada;  // 0 = Desbloqueada, 1 = Bloqueada
} Cliente;

// Estrutura para representar um administrador
typedef struct {
    char usuario[20];
    char senha[20];
} Administrador;

// Declaração das funções no inicio
void terminal_Clear();
int  login_Cliente(Cliente clientes[], int numClientes);
void menu_Cliente(Cliente clientes[], int clienteIndex);
void consultar_Saldo_Extrato(Cliente clientes[], int clienteIndex);
void ver_Clientes(Cliente clientes[], int numClientes);
void bloqueio_e_Desbloqueio_de_Clientes(Cliente clientes[], int quantidade, int n);
void sacar(Cliente clientes[], int quantidade, int clienteIndex);
void transferir(Cliente clientes[], int quantidade, int clienteIndex);
void depositar(Cliente clientes[], int clienteIndex);
void remover_Conta(Cliente clientes[], int numClientes);

int main() {
    setlocale(LC_ALL, "Portuguese");

    Cliente clientes[NCLIENTES] = {
        {"João Silva", "12345678901", "senha123", 1000.50, "Nenhuma", 1, 1}, // Ativa e desbloquada
        {"Maria Oliveira", "98765432100", "senha456", 2500.00, "Nenhuma", 0, 1}, // Não Ativada e desbloqueaada
        {"Carlos Souza", "11223344556", "senha789", 0.00, "Nenhuma", 0, 0}, // Não Ativada e bloqueaada
        {"Ana Pereira", "22334455667", "senha321", 500.75, "Nenhuma", 1, 0}, // Ativa e bloqueada
        {"Lucas Costa", "99887766554", "senha654", 100.25, "Nenhuma", 1, 1} 
    };

    int opcao;
    int numClientes = 5;
    do {
        printf("\n---------- Bem-vindo ao Banco Digital ----------\n");
        printf("1. Login como Cliente\n2. Criar Conta\n3. Login como ADM\n4. Sair\nEscolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            terminal_Clear();
            printf("\nEntrada inválida! Digite um número.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                terminal_Clear();
                int clienteIndex = login_Cliente(clientes, numClientes);
                if (clienteIndex != -1) {
                    menu_Cliente(clientes, clienteIndex);
                }
                break;
            case 2:
                terminal_Clear();
                printf("Função criarContaCliente() ainda não implementada.\n");
                break;
            case 3:
                terminal_Clear();
                printf("Função adminLogin() ainda não implementada.\n");
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                terminal_Clear();
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}

void terminal_Clear() {
    sleep(0.7);
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
    #endif
}

int login_Cliente(Cliente clientes[], int numClientes) {
    char cpf[12], senha[15];

    // Limpeza do buffer de entrada
    while (getchar() != '\n'); // Limpa qualquer caractere residual do buffer

    printf("Digite seu CPF: ");
    scanf("%11s", cpf);  // Limita a leitura para 11 caracteres
    getchar();  // Consome o \n que ficou no buffer

    printf("Digite sua senha: ");
    scanf("%14s", senha);  // Limita a leitura para 14 caracteres
    getchar();  // Consome o \n que ficou no buffer

    // Verifica o login do cliente
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0 && strcmp(clientes[i].senha, senha) == 0) {
            if (clientes[i].contaAtiva == 0) {
                sleep(0.7);terminal_Clear();
                printf("Conta desativada! Entre em contato com o administrador para reativá-la.\n");
                return -1;  // Conta desativada
            } else if (clientes[i].bloqueada == 0) {
                sleep(0.7);terminal_Clear();
                printf("Conta bloqueada! Entre em contato com o administrador para desbloqueá-la.\n");
                return -1;  // Conta bloqueada
            } else {
                sleep(0.7);terminal_Clear();
                printf("Bem-vindo, %s! Login realizado com sucesso!\n", clientes[i].nome);
                return i;  // Retorna o índice do cliente no array
            }
        }
    }

    printf("Cliente não encontrado! CPF ou senha incorretos.\n");
    return -1;  // CPF ou senha incorretos
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
                terminal_Clear();
                consultar_Saldo_Extrato(clientes, clienteIndex);
                break;
            case 2:
                terminal_Clear();
                depositar(clientes, clienteIndex);
                break;
            case 3:
                terminal_Clear();
                sacar(clientes, NCLIENTES, clienteIndex);
                break;
            case 4:
                terminal_Clear();
                transferir(clientes, NCLIENTES, clienteIndex);
                break;  
            case 5:
                sleep(0.7);terminal_Clear();
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
    printf("Digite o valor que deseja sacar: ");
    if (scanf("%f", &valor) != 1 || valor <= 0) {
        printf("Valor inválido! Tente novamente.\n");
        while (getchar() != '\n');
        return;
    }

    if (clientes[clienteIndex].saldo < valor) {
        printf("Saldo insuficiente!\n");
        return;
    }

    clientes[clienteIndex].saldo -= valor;
    printf("Saque de R$ %.2f realizado com sucesso! Saldo atual: R$ %.2f\n", valor, clientes[clienteIndex].saldo);
}

void transferir(Cliente clientes[], int quantidade, int clienteIndex) {
    char cpf_destino[12];
    float valor;
    printf("Digite o CPF do destinatário: ");
    scanf("%11s", cpf_destino);
    getchar();

    printf("Digite o valor que deseja transferir: ");
    if (scanf("%f", &valor) != 1 || valor <= 0) {
        printf("Valor inválido! Tente novamente.\n");
        while (getchar() != '\n');
        return;
    }

    if (clientes[clienteIndex].saldo < valor) {
        printf("Saldo insuficiente!\n");
        return;
    }

    // Verifica se o destinatário existe
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(clientes[i].cpf, cpf_destino) == 0) {
            clientes[clienteIndex].saldo -= valor;
            clientes[i].saldo += valor;
            printf("Transferência de R$ %.2f realizada para %s. Saldo atual: R$ %.2f\n", valor, clientes[i].nome, clientes[clienteIndex].saldo);
            return;
        }
    }

    printf("Cliente destino não encontrado!\n");
}

void depositar(Cliente clientes[], int clienteIndex) {
    float valor;
   
    printf("Digite o valor a ser depositado: R$ ");
    scanf("%f", &valor);
   
    if (valor > 0) {
        clientes[clienteIndex].saldo += valor;  
        printf("Depósito realizado com sucesso! Novo saldo: R$ %.2f\n", clientes[clienteIndex].saldo);
    } else {
        printf("Erro: O valor do depósito deve ser positivo!\n");
    }
}

void remover_Conta(Cliente clientes[], int numClientes) {
    char cpf[15];
    printf("Digite o CPF do cliente para remover a conta: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remover a quebra de linha

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            // Remover cliente movendo os demais para a esquerda
            for (int j = i; j < numClientes - 1; j++) {
                clientes[j] = clientes[j + 1];
            }
            numClientes--;  // Atualiza o número de clientes
            printf("Conta removida com sucesso!\n");
            return;
        }
    }
    printf("Cliente não encontrado!\n");
}
