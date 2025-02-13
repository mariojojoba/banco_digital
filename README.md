Aqui está o **README** reformulado de forma mais organizada e clara:  

---

# **Banco Digital (CLI)**  

Este é um sistema de banco digital baseado em **linha de comando (CLI)**, que permite a administração de contas bancárias e a realização de transações financeiras.  

## **Funcionalidades**  

### **Menu Principal**  
Ao iniciar o sistema, o usuário pode escolher entre três opções:  
1. **Entrar na conta de administrador** → Requer as credenciais do administrador pré-cadastrado.  
2. **Cadastrar um novo cliente** → Permite o registro de novos clientes no banco.  
3. **Entrar na conta de cliente** → Requer as credenciais do cliente para acesso.  

---

### **Funcionalidades do Administrador**  
Ao entrar como administrador, o sistema disponibiliza as seguintes opções:  

- **Ativar conta bancária de um cliente** → O administrador pode ativar contas de clientes já cadastrados para que possam realizar transações.  
- **Exibir todas as contas cadastradas** → Exibe informações detalhadas, incluindo:  
  - Saldo atual de cada conta.  
  - Número de transações realizadas.  
  - Valor total depositado no banco.  
- **Bloquear, desbloquear ou remover uma conta**:  
  - **Bloquear** → A conta pode ser acessada pelo cliente, mas ele não poderá realizar transações.  
  - **Desbloquear** → Restaura o acesso total à conta.  
  - **Remover** → Exclui permanentemente a conta do cliente.  
- **Sair da conta de administrador** → Retorna ao menu principal.  

---

### **Funcionalidades do Cliente**  
Ao entrar como cliente, o sistema oferece as seguintes opções:  

- **Consultar saldo e extrato** → O cliente pode visualizar:  
  - Saldo atual da conta.  
  - Histórico de transações realizadas.  
- **Realizar depósito** → Adicionar dinheiro à conta bancária.  
- **Realizar saque** → Retirar dinheiro da conta, desde que haja saldo suficiente.  
- **Transferir dinheiro** → Enviar um valor para outra conta bancária dentro do sistema.  
- **Sair da conta de cliente** → Retorna ao menu principal.  
