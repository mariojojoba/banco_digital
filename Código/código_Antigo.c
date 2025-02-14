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

// Cria o database com os administradores bases
void inicializarAdministradores() {
    FILE *file = fopen("administradores.txt", "w");
    if (file == NULL) {printf("Erro ao abrir o arquivo de administradores!\n"); return;}
    fprintf(file, "1,Vitoria Sofia,vitoriaIFMA2025\n");
    fprintf(file, "2,Lucas Macario,macarioIFMA2025\n");
    fprintf(file, "3,Mario Felipe,felipeIFMA2025\n");
    fprintf(file, "4,Carlos Eduardo,eduardoIFMA2025\n");
    fprintf(file, "5,Saulo Ferro,ferroIFMA2025\n");
    fclose(file);
}

// Cria o database para clientes
void inicializarClientes() {
    FILE *file = fopen("clientes.txt", "r");
    if (file == NULL) {
        file = fopen("clientes.txt", "w");
        if (file == NULL) {printf("Erro ao criar o arquivo de clientes!\n");return;}
        fprintf(file, "1,Eddynara Costa,90496494082,12345678910,0,00\n");
        fprintf(file, "2,Emanuel Costa,45345244049,111222333,0,00\n");
        fclose(file);
    } else {fclose(file);}
    file = fopen("bloqueados.txt", "w");
    if (file != NULL) fclose(file); // Cria o arquivo de bloqueados se não existir
}

// Verifica conta bloqueada do cliente
int verificarBloqueio(int idCliente) {
    FILE *file = fopen("bloqueados.txt", "r");
    if (file == NULL) return 0;
    int idBloqueado;
    while (fscanf(file, "%d\n", &idBloqueado) != EOF) {
        if (idBloqueado == idCliente) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Idealiza o bloqueio da conta do cliente
void bloquearCliente(int n) {
    int idCliente;
    if (n==0){
        printf("Digite o ID do cliente a ser bloqueado: ");
        scanf("%d", &idCliente);
        FILE *file = fopen("bloqueados.txt", "a");
        if (file == NULL) {printf("Erro ao acessar o arquivo de bloqueados!\n"); return;}
        fprintf(file, "%d\n", idCliente);
        fclose(file);
        printf("Cliente com ID %d bloqueado com sucesso!\n", idCliente);
    } else {
        FILE *file = fopen("bloqueados.txt", "a");
        if (file == NULL) {printf("Erro ao acessar o arquivo de bloqueados!\n"); return;}
        fprintf(file, "%d\n", n);
        fclose(file);
    }
}

void registrarTransacao(int idCliente, const char *tipo, float valor) {
    FILE *file = fopen("transacoes.txt", "a");
    if (file == NULL) {printf("Erro ao registrar a transação!\n");return;}
    fprintf(file, "%d,%s,%.2f\n", idCliente, tipo, valor);
    fclose(file);
}

void sacar(Cliente *cliente) {
    if (verificarBloqueio(cliente->id)) { printf("Operação inválida. Sua conta está bloqueada.\n"); return;}

    float valor;
    printf("Digite o valor para saque: R$ ");
    scanf("%f", &valor);

    if (valor <= 0) { printf("Valor inválido!\n"); return;}
    if (valor > cliente->saldo) { printf("Saldo insuficiente!\n"); return;}

    cliente->saldo -= valor;
    printf("Saque realizado com sucesso! Novo saldo: R$ %.2f\n", cliente->saldo);
    registrarTransacao(cliente->id, "Saque", valor);

    // Atualiza o saldo no arquivo clientes.txt
    FILE *file = fopen("clientes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes!\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário!\n");
        fclose(file);
        return;
    }

    Cliente tempCliente;
    char linha[256];
    int encontrado = 0;

    // Lê e reescreve o arquivo, atualizando o cliente correspondente ao ID
    while (fgets(linha, sizeof(linha), file) != NULL) {
        sscanf(linha, "%d,%49[^,],%14[^,],%15[^,],%f", 
               &tempCliente.id, tempCliente.nome, tempCliente.cpf, tempCliente.senha, &tempCliente.saldo);

        if (tempCliente.id == cliente->id) {
            // Atualiza o saldo do cliente correspondente
            tempCliente.saldo = cliente->saldo;
            encontrado = 1;
        }

        // Escreve a linha (modificada ou não) no arquivo temporário
        fprintf(tempFile, "%d,%s,%s,%s,%.2f\n", tempCliente.id, tempCliente.nome, tempCliente.cpf, tempCliente.senha, tempCliente.saldo);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        // Substitui o arquivo original pelo temporário
        remove("clientes.txt");
        rename("temp.txt", "clientes.txt");
    } else {
        printf("Erro: Cliente não encontrado no arquivo!\n");
        remove("temp.txt");
    }
}

void depositar(Cliente *cliente) {
    if (verificarBloqueio(cliente->id)) {
        printf("Operação não permitida. Sua conta está bloqueada.\n");
        return;
    }

    float valor;
    printf("Digite o valor para depósito: R$ ");
    scanf("%f", &valor);

    if (valor <= 0) {
        printf("Valor inválido!\n");
        return;
    }

    cliente->saldo += valor;
    printf("Depósito realizado com sucesso! Novo saldo: R$ %.2f\n", cliente->saldo);
    registrarTransacao(cliente->id, "Depósito", valor);

    // Atualiza o saldo no arquivo clientes.txt
    FILE *file = fopen("clientes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes!\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário!\n");
        fclose(file);
        return;
    }

    Cliente tempCliente;
    char linha[256];
    int encontrado = 0;

    // Lê e reescreve o arquivo, atualizando o cliente correspondente ao ID
    while (fgets(linha, sizeof(linha), file) != NULL) {
        sscanf(linha, "%d,%49[^,],%14[^,],%15[^,],%f", 
               &tempCliente.id, tempCliente.nome, tempCliente.cpf, tempCliente.senha, &tempCliente.saldo);

        if (tempCliente.id == cliente->id) {
            // Atualiza o saldo do cliente correspondente
            tempCliente.saldo = cliente->saldo;
            encontrado = 1;
        }

        // Escreve a linha (modificada ou não) no arquivo temporário
        fprintf(tempFile, "%d,%s,%s,%s,%.2f\n", tempCliente.id, tempCliente.nome, tempCliente.cpf, tempCliente.senha, tempCliente.saldo);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        // Substitui o arquivo original pelo temporário
        remove("clientes.txt");
        rename("temp.txt", "clientes.txt");
    } else {
        printf("Erro: Cliente não encontrado no arquivo!\n");
        remove("temp.txt");
    }
}

void transferir(Cliente *cliente) {
    if (verificarBloqueio(cliente->id)) {
        printf("Operação não permitida. Sua conta está bloqueada.\n");
        return;
    }

    char cpfDestino[15];
    printf("Digite o CPF do destinatário: ");
    scanf("%14s", cpfDestino);

    FILE *file = fopen("clientes.txt", "r");
    if (file == NULL) {
        printf("Erro ao acessar o banco de clientes!\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário!\n");
        fclose(file);
        return;
    }

    Cliente tempCliente;
    Cliente destinatario;
    int destinatarioEncontrado = 0;
    int remetenteAtualizado = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), file) != NULL) {
        sscanf(linha, "%d,%49[^,],%14[^,],%15[^,],%f", 
               &tempCliente.id, tempCliente.nome, tempCliente.cpf, tempCliente.senha, &tempCliente.saldo);

        if (tempCliente.id == cliente->id) {
            tempCliente.saldo = cliente->saldo;  // Atualiza o saldo do remetente
            remetenteAtualizado = 1;
        }

        if (strcmp(tempCliente.cpf, cpfDestino) == 0) {
            destinatario = tempCliente;
            destinatarioEncontrado = 1;
        }

        fprintf(tempFile, "%d,%s,%s,%s,%.2f\n", tempCliente.id, tempCliente.nome, tempCliente.cpf, tempCliente.senha, tempCliente.saldo);
    }

    fclose(file);
    fclose(tempFile);

    if (!destinatarioEncontrado) {
        printf("Destinatário não encontrado!\n");
        remove("temp.txt");
        return;
    }

    float valor;
    printf("Transferir para %s? (1 - Sim, 2 - Não): ", destinatario.nome);
    int confirmar;
    scanf("%d", &confirmar);

    if (confirmar != 1) {
        printf("Operação cancelada.\n");
        return;
    }

    printf("Digite o valor para transferir: R$ ");
    scanf("%f", &valor);

    if (valor > 0 && valor <= cliente->saldo) {
        cliente->saldo -= valor;
        destinatario.saldo += valor;

        printf("Transferência realizada com sucesso! Novo saldo: R$ %.2f\n", cliente->saldo);
        registrarTransacao(cliente->id, "Transferência Enviada", valor);
        registrarTransacao(destinatario.id, "Transferência Recebida", valor);

        // Atualiza o arquivo novamente para refletir os novos saldos
        file = fopen("temp.txt", "r");
        tempFile = fopen("clientes.txt", "w");

        while (fgets(linha, sizeof(linha), file) != NULL) {
            sscanf(linha, "%d,%49[^,],%14[^,],%15[^,],%f", 
                   &tempCliente.id, tempCliente.nome, tempCliente.cpf, tempCliente.senha, &tempCliente.saldo);

            if (tempCliente.id == cliente->id) {
                tempCliente.saldo = cliente->saldo;
            } else if (tempCliente.id == destinatario.id) {
                tempCliente.saldo = destinatario.saldo;
            }

            fprintf(tempFile, "%d,%s,%s,%s,%.2f\n", tempCliente.id, tempCliente.nome, tempCliente.cpf, tempCliente.senha, tempCliente.saldo);
        }

        fclose(file);
        fclose(tempFile);
    } else {
        printf("Saldo insuficiente ou valor inválido!\n");
        remove("temp.txt");
    }
}

void desbloquearCliente() {
    int idCliente;
    printf("Digite o ID do cliente a ser desbloqueado: ");
    scanf("%d", &idCliente);

    FILE *file = fopen("bloqueados.txt", "r");
    if (file == NULL) {
        printf("Erro ao acessar o arquivo de bloqueados!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(file);
        return;
    }

    int idBloqueado;
    int encontrado = 0;
    while (fscanf(file, "%d\n", &idBloqueado) != EOF) {
        if (idBloqueado == idCliente) {
            encontrado = 1;
            continue;
        }
        fprintf(temp, "%d\n", idBloqueado);
    }
    fclose(file);
    fclose(temp);
    remove("bloqueados.txt");
    rename("temp.txt", "bloqueados.txt");

    if (encontrado) {
        printf("Cliente com ID %d desbloqueado com sucesso!\n", idCliente);
    } else {
        printf("Cliente com ID %d não encontrado na lista de bloqueados!\n", idCliente);
    }
}

void clienteLogin() {
    terminalClear(1);
    char nome[50], cpf[15], senha[16];
    int encontrado = 0;
    Cliente cliente;

    printf("Nome: ");
    scanf(" %49[^\n]", nome);
    printf("CPF: ");
    scanf(" %14s", cpf);
    printf("Senha (15 dígitos): ");
    scanf(" %15s", senha);

    FILE *file = fopen("clientes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o banco de clientes!\n");
        return;
    }
    
    while (fscanf(file, "%d,%49[^,],%14[^,],%15[^,],%f\n", &cliente.id, cliente.nome, cliente.cpf, cliente.senha, &cliente.saldo) == 5) {
        if (strcmp(cliente.nome, nome) == 0 && strcmp(cliente.cpf, cpf) == 0 && strcmp(cliente.senha, senha) == 0) {
            encontrado = 1;
            break;
        }
    }
    fclose(file);

    if (encontrado) {
        terminalClear(1);
        printf("Olá, %s!\n", cliente.nome);
        int opcao;
        do {
            printf("\nMenu de opções:\n");
            printf("1. Ver Saldo\n2. Sacar\n3. Depositar\n4. Transferir\n5. Sair\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao);
            switch (opcao) {
                case 1:
                    terminalClear(1);
                    printf("Seu saldo atual é: R$ %.2f\n", cliente.saldo);
                    break;
                case 2:
                    terminalClear(1);
                    sacar(&cliente);
                    break;
                case 3:
                    terminalClear(1);
                    depositar(&cliente);
                    break;
                case 4:
                    terminalClear(1);
                    transferir(&cliente);
                    break;
                case 5:
                    printf("\nSaindo...\n");
                    terminalClear(1);
                    break;
                default:
                    printf("Opção inválida! Tente novamente.\n");
                    break;
            }
        } while (opcao != 5);
    } else {
        printf("Nome, CPF ou senha incorretos!\n");
        terminalClear(1);
    }
}

void criarNovoAdministrador() {
    terminalClear(1);
    FILE *file = fopen("administradores.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de administradores!\n");
        return;
    }

    Administrador novoAdmin;
    printf("Digite o ID do novo administrador: ");
    scanf("%d", &novoAdmin.id);
    getchar();  // Limpa o buffer do teclado
    printf("Digite o nome do novo administrador: ");
    fgets(novoAdmin.nome, sizeof(novoAdmin.nome), stdin);
    novoAdmin.nome[strcspn(novoAdmin.nome, "\n")] = '\0';  // Remove o caractere de nova linha
    printf("Digite a senha do novo administrador: ");
    scanf("%s", novoAdmin.senha);

    fprintf(file, "%d,%s,%s\n", novoAdmin.id, novoAdmin.nome, novoAdmin.senha);
    fclose(file);
    printf("Novo administrador criado com sucesso!\n");
}

void removerAdministrador() {
    int idRemover;
    printf("Digite o ID do administrador que deseja remover: ");
    scanf("%d", &idRemover);

    FILE *file = fopen("administradores.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    Administrador admin;
    int encontrado = 0;

    while (fscanf(file, "%d,%49[^,],%19s\n", &admin.id, admin.nome, admin.senha) == 3) {
        if (admin.id != idRemover) {
            fprintf(tempFile, "%d,%s,%s\n", admin.id, admin.nome, admin.senha);
        } else {
            encontrado = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        remove("administradores.txt");
        rename("temp.txt", "administradores.txt");
        printf("Administrador com ID %d foi removido com sucesso.\n", idRemover);
    } else {
        remove("temp.txt");
        printf("Administrador com ID %d não foi encontrado.\n", idRemover);
    }
}

void verTodosClientes() {
    // Abre o arquivo de clientes para leitura
    FILE *file = fopen("clientes.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes!\n");
        return;
    }

    Cliente cliente;
    // Exibe o cabeçalho da lista de todos os clientes
    printf("\nLista de Clientes:\n");
    printf("ID\tNome\t\t\tCPF\t\t\tSenha\n");
    printf("-------------------------------------------------------------\n");

    // Lê os dados de cada cliente no arquivo "clientes.txt"
    while (fscanf(file, "%d,%49[^,],%14[^,],%15[^,],%f\n", &cliente.id, cliente.nome, cliente.cpf, cliente.senha, &cliente.saldo) == 5) {
        // Remove possíveis quebras de linha no nome
        cliente.nome[strcspn(cliente.nome, "\r\n")] = '\0';
        
        // Exibe os dados do cliente
        printf("%d\t%s\t\t%s\t%s\n", cliente.id, cliente.nome, cliente.cpf, cliente.senha);
    }
    // Reinicia a leitura do arquivo de clientes
    rewind(file);

    // Abre o arquivo de contas bloqueadas para leitura
    FILE *file2 = fopen("bloqueados.txt", "r");
    if (file2 == NULL) {
        printf("Erro ao abrir o arquivo de clientes com bloqueio!\n");
        fclose(file);
        return;
    }

    // Exibe o cabeçalho da lista de contas bloqueadas
    printf("\nLista de Contas Bloqueadas:\n");
    printf("ID\tNome\t\t\tCPF\n");
    printf("-------------------------------------------------------------\n");

    int id_bloqueado;
    // Lê cada ID do arquivo "bloqueados.txt"
    while (fscanf(file2, "%d\n", &id_bloqueado) == 1) {
        rewind(file); // Reinicia a leitura do arquivo "clientes.txt" para procurar o cliente correspondente ao ID bloqueado
        // Procura o cliente com o ID correspondente
        while (fscanf(file, "%d,%49[^,],%14[^,],%15[^,],%f\n", &cliente.id, cliente.nome, cliente.cpf, cliente.senha, &cliente.saldo) == 5) {
            if (cliente.id == id_bloqueado) {
                // Exibe o cliente como bloqueado se o ID for encontrado
                printf("%d\t%s\t\t%s\n", cliente.id, cliente.nome, cliente.cpf);
                break; 
            }
        }
    }
    // Fecha os arquivos
    fclose(file);
    fclose(file2);
}

void criarContaCliente(int n) {
    terminalClear(1);
    Cliente novoCliente;
    novoCliente.saldo = 0.0;

    // Abrir o arquivo para verificar o último ID existente
    FILE *file = fopen("clientes.txt", "r");
    int ultimoId = 0;

    if (file != NULL) {
        Cliente clienteExistente;
        while (fscanf(file, "%d,%49[^,],%14[^,],%15[^,],%f\n", &clienteExistente.id, clienteExistente.nome, clienteExistente.cpf, clienteExistente.senha, &clienteExistente.saldo) == 5) {
            ultimoId = clienteExistente.id;  // Atualiza o último ID encontrado
        }
        fclose(file);
    }

    // Atribuir o próximo ID automaticamente
    novoCliente.id = ultimoId + 1;    
    if(n==0){ // Se N for igual a 0, exibi o menu para os ADM
        printf("Digite o nome do novo cliente: ");
        scanf(" %49[^\n]", novoCliente.nome);
        printf("Digite o CPF do novo cliente: ");
        scanf(" %14s", novoCliente.cpf);
        printf("Digite a senha do novo cliente (até 15 caracteres): ");
        scanf(" %15s", novoCliente.senha);
    } else{// Se N for diferente de 0, exibi o menu para cliente criar conta
        printf("Olá, vamos criar sua conta\nDigite o seu nome: ");
        scanf(" %49[^\n]", novoCliente.nome);
        printf("Digite o CPF do novo cliente: ");
        scanf(" %14s", novoCliente.cpf);
        printf("Digite a senha do novo cliente (até 15 caracteres): ");
        scanf(" %15s", novoCliente.senha);
    }

    // Verificar se o CPF já existe no arquivo
    file = fopen("clientes.txt", "r");
    if (file != NULL) {
        Cliente clienteExistente;
        while (fscanf(file, "%d,%49[^,],%14[^,],%15[^,],%f\n", &clienteExistente.id, clienteExistente.nome, clienteExistente.cpf, clienteExistente.senha, &clienteExistente.saldo) == 5) {
            if (strcmp(clienteExistente.cpf, novoCliente.cpf) == 0) {
                printf("Erro: Já existe uma conta com este CPF!\n");
                fclose(file);
                return;
            }
        }
        fclose(file);
    }

    // Adicionar o novo cliente ao arquivo
    file = fopen("clientes.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes para escrita!\n");
        return;
    }

    fprintf(file, "%d,%s,%s,%s,%.2f\n", novoCliente.id, novoCliente.nome, novoCliente.cpf, novoCliente.senha, novoCliente.saldo);
    fclose(file);

    printf("Conta criada com sucesso para o cliente %s (ID: %d)!\n", novoCliente.nome, novoCliente.id);
}

void adminLogin() {
    terminalClear(1);
    char nome[50], senha[20];
    int encontrado = 0;
    Administrador admin;

    printf("Nome: ");
    scanf(" %49[^\n]", nome);
    printf("Senha: ");
    scanf(" %19s", senha);

    FILE *file = fopen("administradores.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de administradores!\n");
        return;
    }

    while (fscanf(file, "%d,%49[^,],%19s\n", &admin.id, admin.nome, admin.senha) == 3) {
        admin.nome[strcspn(admin.nome, "\r\n")] = '\0';
        admin.senha[strcspn(admin.senha, "\r\n")] = '\0';

        if (strcmp(admin.nome, nome) == 0 && strcmp(admin.senha, senha) == 0) {
            encontrado = 1;
            break;
        }
    }
    fclose(file);

    if (encontrado) {
        terminalClear(1);
        printf("Olá, %s! Bem-vindo ao painel de administrador.\n", admin.nome);
        int opcao;
        do {
    printf("\nMenu de opções:\n");
    
    if (admin.id == 3) {
        printf("1. Criar novo administrador\n");
        printf("2. Bloquear cliente\n");
        printf("3. Desbloquear cliente\n");
        printf("4. Criar conta para cliente\n");
        printf("5. Ver todos os clientes\n");
        printf("6. Remover administrador\n");
        printf("7. Sair\n");
    } else {
        printf("1. Bloquear cliente\n");
        printf("2. Desbloquear cliente\n");
        printf("3. Criar conta para cliente\n");
        printf("4. Ver todos os clientes\n");
        printf("5. Sair\n");
    }
    
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            if (admin.id == 3) {
                terminalClear(0);
                criarNovoAdministrador();
            } else {
                terminalClear(0);
                bloquearCliente(0);
            }
            break;
        case 2:
            if (admin.id == 3) {
                terminalClear(0);
                bloquearCliente(0);
            } else {
                terminalClear(0);
                desbloquearCliente();
            }
            break;
        case 3:
            if (admin.id == 3) {
                terminalClear(0);
                desbloquearCliente();
            } else {
                terminalClear(0);
                criarContaCliente(0);
            }
            break;
        case 4:
            if (admin.id == 3) {
                terminalClear(0);
                criarContaCliente(0);
            } else {
                terminalClear(0);
                verTodosClientes();
            }
            break;
        case 5:
            if (admin.id == 3) {
                terminalClear(0);
                verTodosClientes();
            } else {
                terminalClear(0);
                printf("Saindo do painel de administrador... Até a próxima, %s!\n", admin.nome);
                opcao = 7;
            }
            break;
        case 6:
            if (admin.id == 3) {
                terminalClear(0);
                removerAdministrador();
            } else {
                printf("Opção inválida!\n");
            }
            break;
        case 7:
            if (admin.id == 3) {
                terminalClear(0);
                printf("Saindo do painel de administrador... Até a próxima, %s!\n", admin.nome);
            } else {
                printf("Opção inválida!\n");
            }
            break;
        default:
            printf("Opção inválida!\n");
            break;
    }
} while (opcao != 7);

    } else {
        printf("Nome ou senha inválidos!\n");
    }
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
