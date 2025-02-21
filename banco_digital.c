#include <stdio.h>    // Biblioteca padrão de entrada e saída, usada para funções como printf() e scanf().
#include <stdlib.h>   // Biblioteca para gerenciamento de memória, controle de processos e conversões.
#include <locale.h>   // Biblioteca para configurar localizações, como suporte a acentuação e formatos regionais.
#include <unistd.h>   // Biblioteca para manipulação de chamadas do sistema Unix (Mac e Linux), como sleep().
#include <string.h>   // Biblioteca para manipulação de strings, com funções como strlen(), strcpy(), strcat(), etc.

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

// Funções
void terminal_Clear();
int  cadastrar_Cliente(Cliente clientes[], int numClientes);
void ativar_Conta_Cliente(Cliente clientes[], int numClientes, int quantidade);
int  login_Cliente(Cliente clientes[], int numClientes);
void menu_Cliente(Cliente clientes[], int clienteIndex);
void consultar_Saldo_Extrato(Cliente clientes[], int clienteIndex);
void ver_Clientes(Cliente clientes[], int numClientes);
void bloqueio_e_Desbloqueio_de_Clientes(Cliente clientes[], int quantidade, int n);
void sacar(Cliente clientes[], int quantidade, int clienteIndex);
void transferir(Cliente clientes[], int quantidade, int clienteIndex);
void depositar(Cliente clientes[], int clienteIndex);
int  remover_Conta(Cliente clientes[], int numClientes, const char cpf[]);
void entrar_Conta_Administrador(Administrador *adm, Cliente clientes[], int numClientes);

int main() {
    setlocale(LC_ALL, "Portuguese");

    Administrador adm = {"Pedro Monteiro", "senha990"};

    Cliente clientes[NCLIENTES] = {
        {"João Silva", "12345678901", "senha123", 1000.50, "Nenhuma", 0, 0}, // Ativa e desbloquada
        {"Maria Oliveira", "98765432100", "senha456", 2500.00, "Nenhuma", 0, 1}, // Ativada e bloqueaada
        {"Carlos Souza", "11223344556", "senha789", 0.00, "Nenhuma", 0, 0}, 
        {"Ana Pereira", "22334455667", "senha321", 500.75, "Nenhuma", 1, 0},
        {"Lucas Costa", "99887766554", "senha654", 100.25, "Nenhuma", 1, 1} // Não Ativa e Bloqueada
    };

    int opcao;
    int numClientes = 10;
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
                sleep(0.7);terminal_Clear();
                int clienteIndex = login_Cliente(clientes, numClientes);
                if (clienteIndex != -1) {
                    menu_Cliente(clientes, clienteIndex);
                }
                break;
            case 2:
                sleep(0.7);terminal_Clear();
                numClientes = cadastrar_Cliente(clientes, numClientes);
                break;
            case 3:
                sleep(0.7);terminal_Clear();
                entrar_Conta_Administrador(&adm, clientes, numClientes);
                break;
            case 4:
                sleep(0.7);terminal_Clear();
                printf("Saindo...\n");
                break;
            default:
                sleep(0.7);terminal_Clear();
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}

// Função para limpar a tela
void terminal_Clear() {
    sleep(0.7);
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Função para cadastrar um cliente
int cadastrar_Cliente(Cliente clientes[], int numClientes) {
    if (numClientes >= NCLIENTES) {
        printf("Limite de clientes atingido.\n");
        return numClientes;
    }

    // Limpa o buffer
    while (getchar() != '\n');

    char nome[50], cpf[12], senha[15];

    // Solicita o nome do cliente
    printf("Digite o nome do cliente: ");
    scanf("%49[^\n]", nome);  // Lê o nome até o final da linha
    getchar();  // Limpa o \n que fica no buffer após o scanf

    // Solicita o CPF
    printf("Digite seu CPF: ");
    scanf("%11s", cpf);  // Lê até 11 caracteres, sem considerar espaços

    // Validação básica de CPF
    if (strlen(cpf) != 11) {
        printf("CPF inválido. Deve ter 11 dígitos.\n");
        return numClientes;
    }

    // Solicita a senha
    printf("Digite a senha do cliente(deve ter 6 caracteres): ");
    scanf("%14s", senha);  // Lê até 14 caracteres


    // Valida a senha
    if (strlen(senha) < 6) {
        printf("Senha muito curta! A senha deve ter no mínimo 6 caracteres.\n");
        return numClientes;
    }

    for(int i = 0; i < NCLIENTES; i++){
        if(strcmp(clientes[i].nome, nome) == 0 && strcmp(clientes[i].cpf, cpf) == 0 && strcmp(clientes[i].senha, senha) == 0){
            printf("Essa conta já existe!");
            return numClientes;
        }
    }

    // Criação do novo cliente
    Cliente novoCliente;
    strcpy(novoCliente.nome, nome);
    strcpy(novoCliente.cpf, cpf);
    strcpy(novoCliente.senha, senha);
    novoCliente.saldo = 0.00;
    strcpy(novoCliente.transacoes, "Nenhuma");
    novoCliente.contaAtiva = 1;
    novoCliente.bloqueada = 0;

    // Adiciona o novo cliente à lista de clientes
    clientes[numClientes] = novoCliente;
    
    sleep(0.7);terminal_Clear();

    printf("\nCliente cadastrado com sucesso!\n");

    // Retorna o novo número de clientes
    return numClientes + 1;
}

// Função para ativar conta de cliente
void ativar_Conta_Cliente(Cliente clientes[], int numClientes,int quantidade) {
    char senha[12];
    printf("------------- Contas Inativas -------------\n");
    for (int i = 0; i < quantidade; i++) {
        if (clientes[i].contaAtiva != 0 && clientes[i].nome[0] != '\0') {  
            printf("Nome: %s | CPF: %s\n", clientes[i].nome, clientes[i].cpf);
        }
    }

    printf("\nDigite a CPF do cliente para ativar a conta: ");
    scanf("%11s", senha);
    while (getchar() != '\n');

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, senha) == 0) {
            clientes[i].contaAtiva = 0;
            printf("Conta de %s ativada com sucesso!\n", clientes[i].nome);
            return;
        }
    }
    terminal_Clear();
    printf("Cliente não encontrado ou não existe!\n");
}
// Função de login do cliente
int login_Cliente(Cliente clientes[], int numClientes) {
    char cpf[12], senha[15];

    while (getchar() != '\n'); // Limpa qualquer caractere residual do buffer
    printf("Digite seu CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;  // Remove a quebra de linha se houver

    while (getchar() != '\n');
    printf("Digite sua senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = 0;  // Remove a quebra de linha se houver

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0 && strcmp(clientes[i].senha, senha) == 0) {
            if (clientes[i].contaAtiva == 1) {
                sleep(1);  // Aguarda 1 segundo
                terminal_Clear();  // Limpa a tela
                printf("Conta desativada! Entre em contato com o administrador para reativá-la.\n");
                return -1;  // Conta desativada
            } else if (clientes[i].bloqueada==1) {
                sleep(1);  // Aguarda 1 segundo
                terminal_Clear();  // Limpa a tela
                printf("Conta bloqueada! Entre em contato com o administrador para desbloqueá-la.\n");
                return -1;  // Conta bloqueada
            } else {
                sleep(1);  // Aguarda 1 segundo
                terminal_Clear();  // Limpa a tela
                printf("Bem-vindo, %s! Login realizado com sucesso!\n", clientes[i].nome);
                return i;  // Retorna o índice do cliente no array
            }
        }
    }

    printf("Cliente não encontrado! CPF ou senha incorretos.\n");
    return -1;  // CPF ou senha incorretos
}

// Função para o menu do cliente
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
                sleep(0.7); terminal_Clear();
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 5);
}

// Função para consultar saldo e extrato
void consultar_Saldo_Extrato(Cliente clientes[], int clienteIndex) {
    printf("\n------------- Saldo e Extrato -------------\n");
    printf("Nome: %s\n", clientes[clienteIndex].nome);
    printf("CPF: %s\n", clientes[clienteIndex].cpf);
    printf("Saldo atual: R$ %.2f\n", clientes[clienteIndex].saldo);
    printf("Transações realizadas:\n%s", clientes[clienteIndex].transacoes);
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
    }
}

void bloqueio_e_Desbloqueio_de_Clientes(Cliente clientes[], int quantidade, int n) {
    char entrada[50];
    
    if (n == 0) {
        printf("------------- Contas bloqueadas -------------\n");
        for (int i = 0; i < quantidade; i++) {
            if ((clientes[i].bloqueada)==1) {
                printf("Nome: %s | CPF: %s\n", clientes[i].nome, clientes[i].cpf);
            }
        }
        printf("\nDigite o nome ou CPF da conta para desbloquear: ");
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
        printf("\n------------- Contas desbloqueadas -------------\n");
        for (int i = 0; i < quantidade; i++) {
            // Verifica se a conta não está bloqueada e se o nome e o CPF não estão vazios
            if (clientes[i].bloqueada!=1 && (strlen(clientes[i].nome) > 0 && strlen(clientes[i].cpf) > 0)) {
                printf("Nome: %s | CPF: %s\n", clientes[i].nome, clientes[i].cpf);
            }
        }
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
    char senha[15];
    float valor;

    // Confirmação de identidade - solicita senha do cliente de origem
    printf("---------------- Confirmação de Identidade ----------------\n");
    printf("Digite sua senha: ");
    scanf("%14s", senha);
    getchar();  // Limpa o \n que ficou no buffer

    // Verificar se a senha está correta
    if (strcmp(clientes[clienteIndex].senha, senha) != 0) {
        printf("Senha incorreta! Saque cancelado.\n");
        return;
    }

    // Solicitar valor do saque
    printf("Digite o valor que deseja sacar: ");
    if (scanf("%f", &valor) != 1 || valor <= 0) {
        printf("Valor inválido! Tente novamente.\n");
        while (getchar() != '\n');  // Limpa o buffer
        return;
    }

    // Verificar saldo
    if (clientes[clienteIndex].saldo < valor) {
        printf("Saldo insuficiente! Saldo atual: R$ %.2f\n", clientes[clienteIndex].saldo);
        return;
    }

    // Realizar o saque
    clientes[clienteIndex].saldo -= valor;

    // Formatar a transação
    char transacao[50];
    snprintf(transacao, sizeof(transacao), "Saque: R$ %.2f", valor);

    // Verificar se "Nenhum" existe na string de transações
    if (strstr(clientes[clienteIndex].transacoes, "Nenhum") != NULL) {
        // Substituir "Nenhum" pela transação de saque
        char *pos = strstr(clientes[clienteIndex].transacoes, "Nenhum");
        strncpy(pos, transacao, strlen(transacao));
        printf("Transação de saque registrada com sucesso.\n");
    } else {
        // Adicionar o saque ao final da string de transações
        strcat(clientes[clienteIndex].transacoes, " | ");
        strcat(clientes[clienteIndex].transacoes, transacao);
        printf("Transação de saque adicionada com sucesso.\n");
    }

    // Exibir mensagem final
    printf("Saque de R$ %.2f realizado com sucesso! Saldo atual: R$ %.2f\n", valor, clientes[clienteIndex].saldo);
}

void transferir(Cliente clientes[], int quantidade, int clienteIndex) {
    char cpf_destino[12], senha[15];
    float valor;
    
    // Verificar se a conta do cliente de origem está ativa e não bloqueada
    if ((clientes[clienteIndex].contaAtiva)==0 || (clientes[clienteIndex].bloqueada)==0) {
        printf("Sua conta está desativada ou bloqueada. Não é possível realizar a transferência.\n");
        return;
    }

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

    // Verifica se o destinatário existe e se a conta está ativa e não bloqueada
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(clientes[i].cpf, cpf_destino) == 0) {
            // Verificação da conta do destinatário
            if ((clientes[i].contaAtiva)==0) {
                printf("A conta do destinatário está desativada.\nNão é possível realizar a transferência.\n");
                return;
            }

            // Se a transação anterior do cliente de origem for "Nenhuma", limpa o campo de transações
            if (strcmp(clientes[clienteIndex].transacoes, "Nenhuma") == 0) {
                strcpy(clientes[clienteIndex].transacoes, "");  // Limpa a transação anterior
            }

            // Cria a string de transação para o cliente de origem
            char transacao_origem[100];
            snprintf(transacao_origem, sizeof(transacao_origem), "Transferência de R$ %.2f para %s\n", valor, clientes[i].nome);

            // Adiciona a nova transação no extrato do cliente origem
            if (strcmp(clientes[clienteIndex].transacoes, "Nenhuma") != 0) {
                strcat(clientes[clienteIndex].transacoes, " ");
            }
            strcat(clientes[clienteIndex].transacoes, transacao_origem);

            // Cria a string de transação para o destinatário
            char transacao_destino[100];
            snprintf(transacao_destino, sizeof(transacao_destino), "Recebido R$ %.2f de %s\n", valor, clientes[clienteIndex].nome);

            // Se a transação do destinatário for "Nenhuma", ele começa com a nova transação
            if (strcmp(clientes[i].transacoes, "Nenhuma") == 0) {
                strcpy(clientes[i].transacoes, transacao_destino);
            } else {
                strcat(clientes[i].transacoes, " ");  // Adiciona um espaço para separar as transações
                strcat(clientes[i].transacoes, transacao_destino);  // Adiciona a transação
            }

            // Confirmação de identidade - solicita senha do cliente de origem
            printf("---------------- Confirmação de Identidade ----------------\n");
            printf("Olá,%s.\nDigite sua senha: ",clientes[clienteIndex].nome);
            scanf("%14s", senha);
            getchar();  // Limpa o \n que ficou no buffer

            // Verificar se a senha está correta
            if (strcmp(clientes[clienteIndex].senha, senha) != 0) {
                printf("Senha incorreta! Transferência cancelada.\n");
                return;
            }

            // Realiza a transferência de valores
            clientes[clienteIndex].saldo -= valor;
            clientes[i].saldo += valor;

            printf("Transferência de R$ %.2f realizada para %s. Saldo atual: R$ %.2f\n", valor, clientes[i].nome, clientes[clienteIndex].saldo);
            return;
        }
    }

    printf("Cliente destinatário não encontrado!\n");
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

int remover_Conta(Cliente clientes[], int numClientes, const char cpf[]) {
    // O CPF é agora um parâmetro passado diretamente para a função
    printf("CPF para remover a conta: %s\n", cpf);

    // Verifica se o CPF não é vazio
    if (strlen(cpf) == 0) {
        printf("CPF inválido! Tente novamente.\n");
        return numClientes;  // Retorna o número de clientes sem alteração
    }

    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            // Remover cliente movendo os demais para a esquerda
            for (int j = i; j < numClientes - 1; j++) {
                clientes[j] = clientes[j + 1];
            }
            numClientes--;  // Atualiza o número de clientes
            printf("Conta removida com sucesso!\n");
            return numClientes;  // Retorna o novo número de clientes
        }
    }
    printf("Cliente não encontrado!\n");
    return numClientes;  // Retorna o número de clientes sem alteração
}

void entrar_Conta_Administrador(Administrador *adm, Cliente clientes[], int numClientes) {
    char usuario[50], senha[15];
    printf("Digite o nome de usuário: ");
    scanf(" %[^\n]", usuario);  // Leitura do nome com espaços
    while (getchar() != '\n');   // Limpa o buffer

    printf("Digite a senha: ");
    scanf("%14s", senha);        // Leitura da senha
    while (getchar() != '\n');   // Limpa o buffer

    terminal_Clear();

    // Adicionando printf para depuração
    printf("Usuário digitado: %s\n", usuario); 
    printf("Senha digitada: %s\n", senha);

    if (strcmp(usuario, adm->usuario) == 0 && strcmp(senha, adm->senha) == 0) {
        sleep(0.7);terminal_Clear();
        int opcao;
        do {
            printf("Olá, %s, bem-vindo!\n---------- Menu do Administrador ----------\n\n", usuario);
            printf("1. Ativar conta do cliente\n2. Exibir contas\n3. Desbloquear conta de cliente\n4. Bloquear conta de cliente\n5. Remover conta\n6. Sair do Administrador\nEscolha uma opção: ");
            
            if (scanf("%d", &opcao) != 1) {
                terminal_Clear();
                printf("\nEntrada inválida! Digite um número.\n");
                while (getchar() != '\n');
                continue;
            }

            switch(opcao) {
                case 1:
                    sleep(0.7); terminal_Clear();
                    ativar_Conta_Cliente(clientes, numClientes,NCLIENTES);
                    break;
                case 2:
                    sleep(0.7); terminal_Clear();
                    ver_Clientes(clientes, numClientes);
                    break;
                case 3:
                    sleep(0.7); terminal_Clear();
                    bloqueio_e_Desbloqueio_de_Clientes(clientes, NCLIENTES, 0);
                    break;
                case 4:
                    sleep(0.7); terminal_Clear();
                    bloqueio_e_Desbloqueio_de_Clientes(clientes, NCLIENTES, 1);
                    break;
                case 5:
                    sleep(0.7);terminal_Clear();
                    char cpf[15];
                    printf("Digite o CPF do cliente para remover a conta: ");
                    scanf("%14s", cpf);
                    sleep(0.7); 
                    terminal_Clear();
                    numClientes = remover_Conta(clientes, numClientes, cpf);
                    break;
                case 6:
                    sleep(0.7); terminal_Clear();
                    printf("Saindo...\n");
                    break;
                default:
                    sleep(0.7); terminal_Clear();
                    printf("Opção inválida! Tente novamente.\n");
            }
        } while (opcao != 6);
    } else {
        printf("Nome ou/e senha incorretos!\n");
    }
}
