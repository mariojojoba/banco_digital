#include <stdio.h>   
#include <stdlib.h> 
#include <locale.h>  
#include <unistd.h>  
#include <string.h>  

typedef struct {
    int id;
    char nome[50];
    char cpf[15];
    char senha[16]; // Senha de 15 dígitos numéricos + 1 para o terminador nulo
    float saldo;
} Cliente;

typedef struct {
    int id;
    char nome[50];
    char senha[20];
} Administrador;

// Limpa o terminal/prompt de comando
void terminalClear(int clock) {
    sleep(clock + 0.7);
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
    #endif
}




int main() {
    setlocale(LC_ALL, "Portuguese");
    inicializarAdministradores();
    inicializarClientes();
    bloquearCliente(1);

    int opcao;
    
    do {
        printf("****** Bem-vindo ao Banco Digital ******\n");
        printf("1. Login como Cliente\n");
        printf("2. Criar Conta\n");
        printf("3. Login como ADM\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            terminalClear(0);
            printf("\nEntrada inválida! Digite um número.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                terminalClear(0);
                clienteLogin();
                break;
            case 2:
                terminalClear(0);
                criarContaCliente(1);
                break;
            case 3:
                terminalClear(0);
                adminLogin();
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
