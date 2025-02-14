#include <stdio.h>
#include <string.h>
#include <locale.h>

#define MAX_CLIENTES 5
#define MAX_TRANSACOES 100

// Definição da estrutura de Cliente
typedef struct {
    char nome[50];
    char cpf[15];
    char senha[15];
    float saldo;
    int transacoes;
    int contaAtiva; // 0 = Inativa, 1 = Ativa
    int bloqueada;  // 0 = Desbloqueada, 1 = Bloqueada
} Cliente;

// Definição da estrutura de Administrador
typedef struct {
    char usuario[20];
    char senha[20];
} Administrador;

// Funções para o banco digital
void cadastrarCliente(Cliente* clientes, int* numClientes);
void entrarContaAdministrador(Administrador* admin, Cliente* clientes, int numClientes);
void entrarContaCliente(Cliente* clientes, int numClientes);
void exibirContas(Cliente* clientes, int numClientes);
void ativarContaCliente(Cliente* clientes, int numClientes);
void bloquearDesbloquearConta(Cliente* clientes, int numClientes);
void removerConta(Cliente* clientes, int* numClientes);
void consultarSaldo(Cliente* clientes, int numClientes);
void realizarDeposito(Cliente* clientes, int numClientes);
void realizarSaque(Cliente* clientes, int numClientes);
void transferirDinheiro(Cliente* clientes, int numClientes);

int main() {
    setlocale(LC_ALL, "Portuguese");
    Administrador admin = {"admin", "senha123"};  // Administrador pré-cadastrado
    Cliente clientes[MAX_CLIENTES];
    int numClientes = 0;
    int opcao;

    while (1) {
        printf("\nBanco Digital\n");
        printf("1. Entrar na conta de administrador\n");
        printf("2. Cadastrar um novo cliente\n");
        printf("3. Entrar na conta de cliente\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer de entrada

        if (opcao == 1) {
            entrarContaAdministrador(&admin, clientes, numClientes);
        } else if (opcao == 2) {
            cadastrarCliente(clientes, &numClientes);
        } else if (opcao == 3) {
            entrarContaCliente(clientes, numClientes);
        } else if (opcao == 4) {
            break;
        } else {
            printf("Opção inválida!\n");
        }
    }

    return 0;
}

void cadastrarCliente(Cliente* clientes, int* numClientes) {
    if (*numClientes >= MAX_CLIENTES) {
        printf("Limite de clientes atingido.\n");
        return;
    }
    Cliente novoCliente;
    printf("Digite o nome do cliente: ");
    fgets(novoCliente.nome, sizeof(novoCliente.nome), stdin);
    novoCliente.nome[strcspn(novoCliente.nome, "\n")] = 0; // Remover a quebra de linha

    printf("Digite o CPF do cliente (somente números): ");
    fgets(novoCliente.cpf, sizeof(novoCliente.cpf), stdin);
    novoCliente.cpf[strcspn(novoCliente.cpf, "\n")] = 0; // Remover a quebra de linha

    novoCliente.saldo = 0;
    novoCliente.transacoes = 0;
    novoCliente.contaAtiva = 0;
    novoCliente.bloqueada = 0;

    clientes[*numClientes] = novoCliente;
    (*numClientes)++;
    printf("Cliente cadastrado com sucesso!\n");
}

void entrarContaAdministrador(Administrador* admin, Cliente* clientes, int numClientes) {
    char usuario[20], senha[20];
    printf("Digite o nome de usuário: ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = 0;  // Remover a quebra de linha

    printf("Digite a senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = 0;

    if (strcmp(usuario, admin->usuario) == 0 && strcmp(senha, admin->senha) == 0) {
        int opcao;
        while (1) {
            printf("\nMenu Administrador\n");
            printf("1. Ativar conta bancária de um cliente\n");
            printf("2. Exibir todas as contas\n");
            printf("3. Bloquear/desbloquear/remover conta\n");
            printf("4. Sair da conta de administrador\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao);
            getchar(); // Limpar o buffer

            if (opcao == 1) {
                ativarContaCliente(clientes, numClientes);
            } else if (opcao == 2) {
                exibirContas(clientes, numClientes);
            } else if (opcao == 3) {
                bloquearDesbloquearConta(clientes, numClientes);
            } else if (opcao == 4) {
                break;
            } else {
                printf("Opção inválida!\n");
            }
        }
    } else {
        printf("Usuário ou senha incorretos!\n");
    }
}

void entrarContaCliente(Cliente* clientes, int numClientes) {
    char cpf[15];
    printf("Digite o CPF do cliente (somente números): ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remover a quebra de linha

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            if (clientes[i].contaAtiva && !clientes[i].bloqueada) {
                int opcao;
                while (1) {
                    printf("\nMenu Cliente\n");
                    printf("1. Consultar saldo e extrato\n");
                    printf("2. Realizar depósito\n");
                    printf("3. Realizar saque\n");
                    printf("4. Transferir dinheiro\n");
                    printf("5. Sair\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcao);
                    getchar(); // Limpar o buffer

                    if (opcao == 1) {
                        consultarSaldo(clientes, numClientes);
                    } else if (opcao == 2) {
                        realizarDeposito(clientes, numClientes);
                    } else if (opcao == 3) {
                        realizarSaque(clientes, numClientes);
                    } else if (opcao == 4) {
                        transferirDinheiro(clientes, numClientes);
                    } else if (opcao == 5) {
                        break;
                    } else {
                        printf("Opção inválida!\n");
                    }
                }
            } else {
                printf("Conta não ativa ou bloqueada!\n");
            }
            return;
        }
    }
    printf("Cliente não encontrado!\n");
}

void exibirContas(Cliente* clientes, int numClientes) {
    float totalDepositos = 0;
    for (int i = 0; i < numClientes; i++) {
        printf("Nome: %s\n", clientes[i].nome);
        printf("CPF: %s\n", clientes[i].cpf);
        printf("Saldo: %.2f\n", clientes[i].saldo);
        printf("Transações: %d\n", clientes[i].transacoes);
        printf("Conta Ativa: %s\n", clientes[i].contaAtiva ? "Sim" : "Não");
        printf("Conta Bloqueada: %s\n", clientes[i].bloqueada ? "Sim" : "Não");
        totalDepositos += clientes[i].saldo;
    }
    printf("Total de depósitos no banco: %.2f\n", totalDepositos);
}

void ativarContaCliente(Cliente* clientes, int numClientes) {
    char cpf[15];
    printf("Digite o CPF do cliente para ativar a conta: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remover a quebra de linha

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            clientes[i].contaAtiva = 1;
            printf("Conta de %s ativada com sucesso!\n", clientes[i].nome);
            return;
        }
    }
    printf("Cliente não encontrado!\n");
}

void bloquearDesbloquearConta(Cliente* clientes, int numClientes) {
    char cpf[15];
    printf("Digite o CPF do cliente para bloquear/desbloquear: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remover a quebra de linha

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            clientes[i].bloqueada = !clientes[i].bloqueada;
            printf("Conta de %s %s com sucesso!\n", clientes[i].nome, clientes[i].bloqueada ? "bloqueada" : "desbloqueada");
            return;
        }
    }
    printf("Cliente não encontrado!\n");
}

void removerConta(Cliente* clientes, int* numClientes) {
    char cpf[15];
    printf("Digite o CPF do cliente para remover a conta: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remover a quebra de linha

    for (int i = 0; i < *numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            // Remover cliente
            for (int j = i; j < *numClientes - 1; j++) {
                clientes[j] = clientes[j + 1];
            }
            (*numClientes)--;
            printf("Conta removida com sucesso!\n");
            return;
        }
    }
    printf("Cliente não encontrado!\n");
}

void consultarSaldo(Cliente* clientes, int numClientes) {
    char cpf[15];
    printf("Digite o CPF para consultar o saldo: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remover a quebra de linha

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            printf("Saldo de %s: %.2f\n", clientes[i].nome, clientes[i].saldo);
            return;
        }
    }
    printf("Cliente não encontrado!\n");
}

void realizarDeposito(Cliente* clientes, int numClientes) {
    char cpf[15];
    float deposito;
    printf("Digite o CPF do cliente para depósito: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remover a quebra de linha

    printf("Digite o valor do depósito: ");
    scanf("%f", &deposito);

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            clientes[i].saldo += deposito;
            clientes[i].transacoes++;
            printf("Depósito de %.2f realizado com sucesso!\n", deposito);
            return;
        }
    }
    printf("Cliente não encontrado!\n");
}

void realizarSaque(Cliente* clientes, int numClientes) {
    char cpf[15];
    float saque;
    printf("Digite o CPF do cliente para saque: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remover a quebra de linha

    printf("Digite o valor do saque: ");
    scanf("%f", &saque);

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            if (clientes[i].saldo >= saque) {
                clientes[i].saldo -= saque;
                clientes[i].transacoes++;
                printf("Saque de %.2f realizado com sucesso!\n", saque);
            } else {
                printf("Saldo insuficiente!\n");
            }
            return;
        }
    }
    printf("Cliente não encontrado!\n");
}

void transferirDinheiro(Cliente* clientes, int numClientes) {
    char cpfOrigem[15], cpfDestino[15];
    float valor;
    printf("Digite o CPF da conta de origem: ");
    fgets(cpfOrigem, sizeof(cpfOrigem), stdin);
    cpfOrigem[strcspn(cpfOrigem, "\n")] = 0;

    printf("Digite o CPF da conta de destino: ");
    fgets(cpfDestino, sizeof(cpfDestino), stdin);
    cpfDestino[strcspn(cpfDestino, "\n")] = 0;

    printf("Digite o valor da transferência: ");
    scanf("%f", &valor);

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpfOrigem) == 0) {
            if (clientes[i].saldo >= valor) {
                for (int j = 0; j < numClientes; j++) {
                    if (strcmp(clientes[j].cpf, cpfDestino) == 0) {
                        clientes[i].saldo -= valor;
                        clientes[j].saldo += valor;
                        clientes[i].transacoes++;
                        clientes[j].transacoes++;
                        printf("Transferência de %.2f realizada com sucesso!\n", valor);
                        return;
                    }
                }
                printf("Conta de destino não encontrada!\n");
                return;
            } else {
                printf("Saldo insuficiente para transferência!\n");
                return;
            }
        }
    }
    printf("Conta de origem não encontrada!\n");
}
