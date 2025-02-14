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
    char cpf[15];
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

// Função para limpar o terminal com atraso
void terminalClear(int clock) {
    sleep(clock + 0.7); // Retarda a limpeza em 7 milésimos de segundo.
    #ifdef _WIN32 // Detecta se o código está ronado em Windowns e passa o comando de limpeza
        system("cls"); 
    #else
        printf("\033[H\033[J"); // Em sistemas Unix-like (Linux, macOS), usa a sequência ANSI para limpar a tela
    #endif
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Ajuste para aceitar paramentros de escrita em português
    Cliente clientes[NCLIENTES]; // Cria um array de clientes
    Administrador admins[5]; // Cria um array de administradores
    int opcao; // Variável de controle do menu
    
    inicializarAdministradores(admins);
    inicializarClientes(clientes);
  
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
                printf("Função clienteLogin() ainda não implementada.\n");
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


