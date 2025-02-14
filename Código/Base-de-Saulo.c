#include <stdio.h>    // Biblioteca padrão de entrada e saída, usada para funções como printf() e scanf().
#include <stdlib.h>   // Biblioteca para gerenciamento de memória, controle de processos e conversões.
#include <locale.h>   // Biblioteca para configurar localizações, como suporte a acentuação e formatos regionais.
#include <unistd.h>   // Biblioteca para manipulação de chamadas do sistema Unix (Mac e Linux), como sleep().
#include <string.h>   // Biblioteca para manipulação de strings, com funções como strlen(), strcpy(), strcat(), etc.

// Estrutura para representar um cliente de um sistema financeiro
typedef struct {
    int id;            // Identificador único do cliente
    char nome[50];     // Nome do cliente (até 49 caracteres + terminador nulo '\0')
    char cpf[15];      // CPF no formato "xxxxxxxxxxx" (até 14 caracteres + terminador nulo)
    char senha[16];    // Senha (até 15 dígitos + terminador nulo)
    float saldo;       // Saldo da conta
    char transf[999];  // Histórico de transferências
    int bloqueado;     // Indicador de conta bloqueada (0 = não bloqueado, 1 = bloqueado)
} Cliente;

// Estrutura para representar um administrador
typedef struct {
    int id;           // Identificador único
    char nome[50];    // Nome (até 49 caracteres + terminador nulo)
    char senha[20];   // Senha (até 19 caracteres + terminador nulo)
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

// Inicializa cinco administradores fixos
void inicializarAdministradores(Administrador admins[]) {
    admins[0] = (Administrador){1, "Vitoria Sofia", "vitoriaIFMA2025"};
    admins[1] = (Administrador){2, "Lucas Macario", "macarioIFMA2025"};
    admins[2] = (Administrador){3, "Mario Felipe", "felipeIFMA2025"};
    admins[3] = (Administrador){4, "Carlos Eduardo", "eduardoIFMA2025"};
    admins[4] = (Administrador){5, "Saulo Ferro", "ferroIFMA2025"};
}

// Inicializa dois clientes
void inicializarClientes(Cliente clientes[]) {
    clientes[0] = (Cliente){1, "Eddynara Costa", "90496494082", "12345678910", 0.00, "Nenhuma", 1}; // Bloqueado
    clientes[1] = (Cliente){2, "Emanuel Costa", "45345244049", "111222333", 0.00, "Nenhuma", 0};   // Não bloqueado
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Ajuste para aceitar paramentros de escrita em português
    Cliente clientes[999]; // Cria um array de clientes
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


