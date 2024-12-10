# MATEUS JOSÉ WHITAKER FILIPE 21005080
# JOÃO LUIS BEATO CARDOSO 21012289
# RAFAEL RODRIGUES SATO 21006207

import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
import re
import time

# Declarações globais de parâmetros importantes utilizados pelas expressões, como contador do programa, pilha e flags de execução

stack_pointer = -1
program_instruction_counter = 0
pilha = {}
paused = False
flag_passo_a_passo = False
old_line = -1
flag_execucao = True
janela_rd = None


# Carrega o arquivo .obj selecionado
def carregar_arquivo():
    filepath = filedialog.askopenfilename(filetypes=[("Object Files", "*.obj")])
    if filepath:
        entrada_arquivo.set(filepath)
        with open(filepath, 'r') as file:
            conteudo = file.readlines()
        exibir_conteudo(conteudo)

# Reajusta os rótulos que estão no código objeto gerado, então quando um rótulo é "1", ele vai ser substituido por seu valor de linha, e irá armazenar esse valor em um array para futura substituição nas instruçoes JMP, JMPF e CALL
def reajuste_de_rotulos(conteudo):

    rotulos_para_linhas = []
    line_counter = 0

    for linha in conteudo:
        line_counter += 1
        linha = linha.strip()
        match = re.match(r"(\d*)\s*(\w+)\s*(\d*)\s*(\d*)", linha)

        if match:
            num_linha = match.group(1)
            instrucao = match.group(2)

            if instrucao == "NULL":
                rotulos_para_linhas.append([num_linha, str(line_counter)])

    return rotulos_para_linhas


# Separa o conteúdo do objeto, separando rótulo (que é substituido pela linha correspondente), instrução, atributo 1, atributo 2 e caso tenha, comentário
def exibir_conteudo(conteudo):
    line_counter = 0
    for row in tree.get_children():
        tree.delete(row)

    rotulos_para_linhas = reajuste_de_rotulos(conteudo)                     # Chamada para reatribuir os rótulos as linhas correspondentes

    for linha in conteudo:
        line_counter += 1
        linha = linha.strip()
        match = re.match(r"(\d*)\s*(\w+)\s*(\d*)\s*(\d*)", linha)           # Strip da linha de instrução para separar nas colunas
        if match:
            num_linha = match.group(1) or line_counter
            instrucao = match.group(2)
            atributo1 = match.group(3) or ""
            atributo2 = match.group(4) or ""
            comentario = "" 

            if instrucao == "NULL":
                num_linha = line_counter

            if instrucao == "JMP" or instrucao == "JMPF" or instrucao == "CALL":
                for sublista in rotulos_para_linhas:
                    if sublista[0] == str(atributo1):                       # Substituição efetiva nos casos de instrução CALL, JMP e JMPF dos atributos por linhas
                        atributo1 = sublista[1]
                        break

            tree.insert("", "end", values=(num_linha, instrucao, atributo1, atributo2, comentario))     # insere valores na janela principal

def executar():
    global program_instruction_counter
    global stack_pointer
    global pilha
    global paused
    global flag_passo_a_passo                                               # chamada de todos os valores globais que servem como parâmetros para instruções
    global old_line
    global flag_execucao

    program_instruction_counter = 0
    stack_pointer = -1
    old_line = -1                                                           # reset para o START
    pilha = {}
    flag_execucao = True

    output_text.config(state=tk.NORMAL)                                     # Habilita escrita no campo de output do programa
    output_text.delete('1.0', tk.END)                                       # Deleta todo conteúdo que estava presente na janela de output, fazendo uma execução limpa

    modo = "Normal" if modo_execucao.get() == 1 else "Passo a Passo"        
    
    instrucoes = {
        "LDC": lambda x: LDC(x),
        "LDV": lambda x: LDV(x),
        "ADD": lambda: ADD(),
        "SUB": lambda: SUB(),
        "MULT": lambda: MULT(),
        "DIVI": lambda: DIVI(),
        "INV": lambda: INV(),
        "AND": lambda: AND(),
        "OR": lambda: OR(),
        "NEG": lambda: NEG(),
        "CME": lambda: CME(),
        "CMA": lambda: CMA(),
        "CEQ": lambda: CEQ(),                                               #sumario das instruções e seus equivalentes, então ao passar por uma instrução, comparando o primeiro elemento, ele sabe puxar a funçao correspondente
        "CDIF": lambda: CDIF(),
        "CMEQ": lambda: CMEQ(),
        "CMAQ": lambda: CMAQ(),
        "STR": lambda x: STR(x),
        "JMP": lambda x: JMP(x),
        "JMPF": lambda x: JMPF(x),
        "ALLOC": lambda x, y: ALLOC(x, y),
        "DALLOC": lambda x, y: DALLOC(x, y),
        "RD": lambda: RD(),
        "PRN": lambda: PRN(),
        "CALL": lambda x: CALL(x),
        "RETURN": lambda: RETURN()
    }

    items = tree.get_children()                                         #resgata todas as instruçoes do programa, organizada por itens

    while program_instruction_counter < len(items) and flag_execucao:   #loop principal de execução do programa

        if paused or flag_passo_a_passo:
            root.update()
            time.sleep(0.1)                                             # Caso o paused ou flag_passo_a_passo estiver ativa, é por conta de ou estar em RD(), aguardando entrada de dados (paused), ou está em execução passo a passo, aguardando nova passagem de instrução
            continue

        item = items[program_instruction_counter]                       #pega instrução atual
        linha, instrucao, atributo1, atributo2, _ = tree.item(item, "values")
        
        tree.item(item, tags=("highlight",))                            # grifa linha do item

        tree.item(items[program_instruction_counter - 1], tags=("default",)) # apaga o grifado da linha anterior
        tree.item(items[old_line], tags=("default",))                        # apaga grifado da linha anterior em casos de jump, call e jmpf, já que não estão em ordem, como sair da linha 5 para 20, ele não pode "apagar" o grifado da linha 19

        if instrucao in instrucoes:                                    
            if atributo1 and atributo2:
                instrucoes[instrucao](int(atributo1), int(atributo2))
            elif atributo1:
                instrucoes[instrucao](int(atributo1))                        # percorre se a instrução está na lista de instruções, e separa por quantos atributos contém
            else:
                instrucoes[instrucao]()
        
        if modo_execucao.get() == 2:
            root.update()                                                    # alterna flag do passo a passo, para liberar e destravar a execução ao identificar a escolha
            flag_passo_a_passo = True

        if instrucao == "RD":
            paused = True                                                    # sobe a flag de paused caso a instrução seja RD, para interromper o programa e aguardar o input do usuário

        if (instrucao != "JMP") or (instrucao != "JMPF") or (instrucao != "CALL") or (instrucao != "RETURN"):
            program_instruction_counter += 1                                                                    # Já que são instruções que vão alternar o contador do programa, outras instruções que não interferem simplesmente irão incrementar o contador do program counter
            print(program_instruction_counter)

    
    output_text.insert(tk.END, "Execução concluída.\n")
    output_text.config(state=tk.DISABLED)

def destrava_execucao():
    global flag_passo_a_passo
    flag_passo_a_passo = False                                               # Botão de passo destrava o programa para passar a execução, em executar, essa flag sobe e para a execução, aguardando chamar essa função de novo

def LDC(valor):
    global stack_pointer
    global pilha
    stack_pointer += 1                                                       # incrementa stack pointer e aplica valor constante no topo da pilha
    pilha[stack_pointer] = valor
    atualizar_pilha()

def LDV(endereco):
    global stack_pointer
    stack_pointer += 1
    endereco = int(endereco)

    if endereco in pilha:                                                    # incrementa stack pointer, procura valor na pilha, caso ache, puxa cópia do valor pro topo
        valor = pilha[endereco]
        pilha[stack_pointer] = valor
        atualizar_pilha()
    else:
        output_text.insert(tk.END, f"Erro: Endereço {endereco} não encontrado na pilha\n")

def ADD():
    global stack_pointer
    global pilha

    resultado = int(pilha[stack_pointer - 1]) + int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado                                                # Pega o valor do topo da pilha, pega o valor abaixo do topo da pilha, soma, escreve o valor do resultado na posição abaixo do topo e desempilha
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def SUB():
    global stack_pointer
    global pilha

    resultado = int(pilha[stack_pointer - 1]) - int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado                                                # Pega o valor do topo da pilha, pega o valor abaixo do topo da pilha, subtrai, escreve o valor do resultado na posição abaixo do topo e desempilha
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def MULT():
    global stack_pointer
    global pilha

    resultado = int(pilha[stack_pointer - 1]) * int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado                                                # Pega o valor do topo da pilha, pega o valor abaixo do topo da pilha, multiplica, escreve o valor do resultado na posição abaixo do topo e desempilha
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def DIVI():
    global stack_pointer
    global pilha

    resultado = int(pilha[stack_pointer - 1]) / int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    del pilha[max(pilha.keys())]                                                        # Pega o valor do topo da pilha, pega o valor abaixo do topo da pilha, divide, escreve o valor do resultado na posição abaixo do topo e desempilha
    stack_pointer -= 1
    atualizar_pilha()

def INV():
    global stack_pointer
    global pilha

    pilha[stack_pointer] = -int(pilha[stack_pointer])                                  # Pega o valor do topo da pilha e transforma-o no inverso
    atualizar_pilha()

def AND():
    global stack_pointer
    global pilha

    if (str(pilha[stack_pointer - 1]) == "1") and (str(pilha[stack_pointer]) == "1"):                           # Pega o valor abaixo do topo da pilha, pega o valor do topo da pilha e verifica se ambos são 1, caso sim, a posição abaixo do topo da pilha é 1, se não , é 0. desempilha topo
        pilha[stack_pointer - 1] = 1
    else:
        pilha[stack_pointer - 1] = 0
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def OR():
    global stack_pointer
    global pilha

    if (str(pilha[stack_pointer - 1]) == "1") or (str(pilha[stack_pointer]) == "1"):
        pilha[stack_pointer - 1] = 1
    else:                                                                                                       # Pega o valor abaixo do topo da pilha, pega o valor do topo da pilha e verifica se um deles é 1, caso sim, a posição abaixo do topo da pilha é 1, se não , é 0. desempilha topo
        pilha[stack_pointer - 1] = 0
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def NEG():
    global stack_pointer
    global pilha

    pilha[stack_pointer] = 1 - int(pilha[stack_pointer])                                                        # Nega o topo da pilha
    atualizar_pilha()

def CME():
    global stack_pointer
    global pilha

    if int(pilha[stack_pointer - 1]) < int(pilha[stack_pointer]):
        pilha[stack_pointer - 1] = "1"    
    else:                                                                                                       # pega abaixo do topo da pilha e compara com topo da pilha, se um for menor que o outro respectivamente, o valor abaixo do topo é 1, se não, é 0. desempilha topo
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CMA():
    global stack_pointer
    global pilha

    if int(pilha[stack_pointer - 1]) > int(pilha[stack_pointer]):
        pilha[stack_pointer - 1] = "1"    
    else:                                                                                                       # pega abaixo do topo da pilha e compara com topo da pilha, se um for maior que o outro respectivamente, o valor abaixo do topo é 1, se não, é 0. desempilha topo
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CEQ():
    global stack_pointer
    global pilha

    if int(pilha[stack_pointer - 1]) == int(pilha[stack_pointer]):
        pilha[stack_pointer - 1] = "1"    
    else:                                                                                                       # pega abaixo do topo da pilha e compara com topo da pilha, se um for igual o outro, o valor abaixo do topo é 1, se não, é 0. desempilha topo
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CDIF():
    global stack_pointer
    global pilha

    if int(pilha[stack_pointer - 1]) != int(pilha[stack_pointer]):
        pilha[stack_pointer - 1] = "1"    
    else:                                                                                                       # pega abaixo do topo da pilha e compara com topo da pilha, se um for diferente do outro, o valor abaixo do topo é 1, se não, é 0. desempilha topo
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CMEQ():
    global stack_pointer
    global pilha

    if int(pilha[stack_pointer - 1]) <= int(pilha[stack_pointer]):
        pilha[stack_pointer - 1] = "1"    
    else:                                                                                                       # pega abaixo do topo da pilha e compara com topo da pilha, se um for menor igual que o outro respectivamente, o valor abaixo do topo é 1, se não, é 0. desempilha topo
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CMAQ():
    global stack_pointer
    global pilha

    if int(pilha[stack_pointer - 1]) >= int(pilha[stack_pointer]):
        pilha[stack_pointer - 1] = "1"    
    else:                                                                                                       # pega abaixo do topo da pilha e compara com topo da pilha, se um for maior igual que o outro respectivamente, o valor abaixo do topo é 1, se não, é 0. desempilha topo
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def STR(endereco):
    global stack_pointer
    global pilha
    endereco = int(endereco)
    
    if endereco in pilha:
        pilha[endereco] = pilha[stack_pointer]
        del pilha[max(pilha.keys())]                                                                            # Procura o endereço na pilha, desempilha o topo e armazena o valor que anteiormente estava no topo na posição especificada pelo argumento do store
        stack_pointer -= 1
        atualizar_pilha()
    else:
        output_text.insert(tk.END, f"Erro: Endereço {endereco} não encontrado na pilha\n")

def JMP(linha):

    global old_line
    global program_instruction_counter
    global pilha                                                                                                # Armazena old_line como linha do jump justamente para apagar o grifado da execução e altera o program_counter para a posição desejada no atributo do jump. o -2 é por conta da correção das linhas e inicialização da execução para o grifado
    old_line = program_instruction_counter
    linha = int(linha)
    program_instruction_counter = linha - 2

def JMPF(linha):

    global old_line
    global program_instruction_counter
    global stack_pointer
    global pilha
    old_line = program_instruction_counter
    linha = int(linha)

    if(str(pilha[stack_pointer]) == "0"):                                                                       # Armazena old_line como linha do jmpf justamente para apagar o grifado da execução e altera o program_counter para a posição desejada no atributo do jmpf caso o valor do topo da pilha for 0, caso sim, desempilha topo e vai para linha, caso não, somente desempilha e segue execução. o -2 é por conta da correção das linhas e inicialização da execução para o grifado
        program_instruction_counter = linha - 2
    
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def ALLOC(endereco_inicial, quantidade):
    global stack_pointer
    global pilha

    k = 0
    
    while k <= quantidade - 1:
        stack_pointer += 1                                                                                      # Para o alloc, ele inicializa k = 0 e faz o loop até n - 1, incrementando o stackpointer, adicionando o valor do endereço inicial + k no topo da pilha e gerando espaços indefinidos nos espaços do alloc. "jogando os elementos nas posiçoes existentes que serão alocadas pelo ALLOC para o topo da pilha"
        pilha[stack_pointer] = ""
        pilha[stack_pointer] = pilha[endereco_inicial + k]
        
        if pilha[stack_pointer] == "":
            pilha[endereco_inicial + k] = "?"
        atualizar_pilha()
        k += 1

def DALLOC(endereco_inicial, quantidade):
    global stack_pointer
    global pilha

    k = quantidade - 1
    
    while k >= 0:
        pilha[endereco_inicial + k] = pilha[stack_pointer]                                                      # Para o dalloc, ele inicializa k = n - 1 e faz o loop descendo a pilha, decrementando o stackpointer, adicionando o valor do topo da pilha na posição e desempilhando.
        stack_pointer -= 1
        pilha[max(pilha.keys())]                                                                                            
        atualizar_pilha()
        k -= 1

def RD():
    global stack_pointer
    global pilha
    global paused
    global janela_rd

    def inserir_valor():
        global paused
        global stack_pointer
        global program_instruction_counter
        try:
            valor = int(entry.get())
            stack_pointer += 1                                                                                  #atribui valor, aumenta o stackpointer e coloca no topo da pilha o valor inputado
            pilha[stack_pointer] = valor
            
            atualizar_pilha()
            janela_rd.destroy()
            
            paused = False

        except ValueError:
            output_text.insert(tk.END, "Erro: Insira um número inteiro válido.\n")

    if janela_rd is None or not janela_rd.winfo_exists():
        janela_rd = tk.Toplevel(root)
        janela_rd.title("RD - Ler Valor")
        tk.Label(janela_rd, text="Digite um valor inteiro:").pack(pady=5)                                                   # inicializa a janela que vai atribuir o para RD 
        janela_rd.minsize(height=100, width=250)

        entry = tk.Entry(janela_rd)
        entry.pack(pady=5)                                                                                                  # inicializa campo de entrada do valor
        entry.focus()

        tk.Button(janela_rd, text="Inserir", command=inserir_valor).pack(pady=10)

        paused = True                                                                                                        #liga o flag para parar o loop principal


def PRN():

    global pilha
    global stack_pointer

    output_text.insert(tk.END, f"{pilha[stack_pointer]}\n")
    del pilha[max(pilha.keys())]                                                                                            # escreve o valor do topo da pilha no campo de output e desempilha

    stack_pointer -= 1
    atualizar_pilha()

def CALL(linha):

    global stack_pointer
    global pilha
    global program_instruction_counter
    global old_line

    old_line = program_instruction_counter

    linha = int(linha)                                                                                                      # armazena old_line para remover o grifado da linha em destaque, incrementa stackpointer, incrementa o program counter, coloca no topo da pilha o valor da linha (rotulo) da linha que está abaixo do CALL, para executar futuramente o return
    stack_pointer += 1
    program_instruction_counter += 1
    pilha[stack_pointer] = int(program_instruction_counter) + 1
    program_instruction_counter = linha - 2
    atualizar_pilha()

def RETURN():
    
    global stack_pointer
    global pilha
    global program_instruction_counter
    global old_line

    old_line = program_instruction_counter                                                                                  # pega o valor que está na pilha e atribui ao program counter, fazendo com que a linha abaixo do call armazenada anteriormente seja o endereço pego pelo return

    program_instruction_counter = pilha[stack_pointer] - 2
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()


def atualizar_pilha():
    global stack_pointer
    for row in tree_pilha.get_children():
        tree_pilha.delete(row)                                                                                              # limpa e recoloca os endereços na pilha, atualizando visualmente a pilha
    
    for endereco, valor in pilha.items():
        tree_pilha.insert("", "end", values=(endereco, valor))

def parar_execucao():
    global flag_execucao
    global program_instruction_counter
    global stack_pointer
    global pilha
    global paused
    global flag_passo_a_passo
    global old_line
    global flag_execucao
    global janela_rd

    items = tree.get_children()

    flag_execucao = False
                                                                                                                        # essa função chamada pelo botão "parar" reseta os parametros e destroi a janela do RD em caso de pausa nesse campo.
    for item in items:
        tree.item(item, tags=("default",))

    if janela_rd and janela_rd.winfo_exists():
        janela_rd.destroy()

    program_instruction_counter = 0
    stack_pointer = -1
    old_line = -1
    pilha = {}
    paused = False

    output_text.insert(tk.END, "Execução Parada\n")

    janela_rd = None

# inicializa a janela principal
root = tk.Tk()
root.title("Código de Máquina")
root.minsize(1000, 700)

#Inicializa frame superior
frame_superior = tk.Frame(root)
frame_superior.pack(side="top", fill="x")

# Inicializa frame inferior
frame_inferior = tk.Frame(root)
frame_inferior.pack(side="bottom", fill="x")

# Inicializa frame da pilha
frame_pilha = tk.Frame(root)
frame_pilha.pack(side="right", fill="y", padx=30, pady=50)

# inicializa o frame do output
frame_saida = tk.Frame(frame_inferior)
frame_saida.pack(side="left", padx=10)

#inicializa o frame da escolha de modo de execução
frame_modo_execucao = tk.LabelFrame(frame_inferior, text="Modo de Execução")
frame_modo_execucao.pack(side="left", padx=10)

#inicializa frame de onde são colocados os botoes de executar, parar e passo
frame_botoes = tk.Frame(frame_inferior)
frame_botoes.pack(side="right", padx=10)

# inicializa botão responsável por abrir arquivo
abrir_botao = tk.Button(frame_superior, text="Abrir .obj", command=carregar_arquivo)
abrir_botao.pack(side="left", padx=30, pady=10)

#escreve no frame superior "path do arquivo:"
l = tk.Label(frame_superior, text = "Path do arquivo: ")
l.config(font =("Courier", 14))
l.pack(side= "left", padx=30, pady= 10)

# inicializa onde mostra o path do arquivo selecionado
entrada_arquivo = tk.StringVar()
entrada_exibir = tk.Entry(frame_superior, textvariable=entrada_arquivo, width=50)
entrada_exibir.pack(side="left", padx=50, pady=10)

# declara as colunas do treeview principal que recebe as instruções
columns = ("Linha", "Instrução", "Atributo 1", "Atributo 2", "Comentário")
tree = ttk.Treeview(root, columns=columns, show="headings")

# determina que o destaque é em amarelo e o não destaque é branco
tree.tag_configure("highlight", background="yellow")
tree.tag_configure("default", background="white")

# gera o heading e separa a janela principal (tree) em colunas
for col in columns:
    tree.heading(col, text=col)
    tree.column(col, width=100, anchor="center")

tree.pack(side="left", fill="both", expand=True, padx=50, pady=50)

# gera as colunas da janela que mostra endereço e valor da pilha
columns_pilha = ("Endereço", "Valor")
tree_pilha = ttk.Treeview(frame_pilha, columns=columns_pilha, show="headings")

for col in columns_pilha:
    tree_pilha.heading(col, text=col)
    tree_pilha.column(col, width=80, anchor="center")

tree_pilha.pack(fill="both", expand=True)

#inicializa campo de saida dos dados, output
label_saida = tk.Label(frame_saida, text="Saída de Dados:")
label_saida.pack()
output_text = tk.Text(frame_saida, width=70, height=5)
output_text.config(state=tk.DISABLED)
output_text.pack(pady=20, padx=30)

#gera os botões de escolha do tipo de execução do código, lido posteriormente em executar()
modo_execucao = tk.IntVar(value=1)
radio_normal = tk.Radiobutton(frame_modo_execucao, text="Normal", variable=modo_execucao, value=1)
radio_passo = tk.Radiobutton(frame_modo_execucao, text="Passo a Passo", variable=modo_execucao, value=2)
radio_normal.pack(anchor="w")
radio_passo.pack(anchor="w")

#coloca e atribui comandos para os botoes de parar e executar
botao_executar = tk.Button(frame_botoes, text="Executar", command=executar)
botao_parar = tk.Button(frame_botoes, text="Parar", command=parar_execucao)
botao_executar.pack(side="left", padx=5)
botao_parar.pack(side="left", padx=5)

#atribui comando para o botao de passe de execução
botao_passa = tk.Button(frame_botoes, text="Passo", command=destrava_execucao)
botao_passa.pack(side="left", padx=5)

root.mainloop()
