import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
import re
import time

    #instruçoes: //LDC, //LDV, //ADD, //SUB, //MULT, //DIVI, //INV, //AND, //OR, //NEG, //CME, //CMA, //CEQ, //CDIF, //CMEQ, //CMAQ, /STR, /JMP, /JMPF, /NULL, /START, //ALLOC, /DALLOC, //RD, //PRN, HLT, //CALL, RETURN

stack_pointer = -1
program_instruction_counter = 0
pilha = {}
paused = False
flag_passo_a_passo = False
old_line = -1

def carregar_arquivo():
    filepath = filedialog.askopenfilename(filetypes=[("Object Files", "*.obj")])
    if filepath:
        entrada_arquivo.set(filepath)
        with open(filepath, 'r') as file:
            conteudo = file.readlines()
        exibir_conteudo(conteudo)

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

def exibir_conteudo(conteudo):
    line_counter = 0
    for row in tree.get_children():
        tree.delete(row)

    rotulos_para_linhas = reajuste_de_rotulos(conteudo)

    for linha in conteudo:
        line_counter += 1
        linha = linha.strip()
        match = re.match(r"(\d*)\s*(\w+)\s*(\d*)\s*(\d*)", linha)
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
                    if sublista[0] == str(atributo1):
                        atributo1 = sublista[1]
                        break

            tree.insert("", "end", values=(num_linha, instrucao, atributo1, atributo2, comentario))

def executar():
    global program_instruction_counter
    global stack_pointer
    global pilha
    global paused
    global flag_passo_a_passo
    global old_line

    program_instruction_counter = 0
    stack_pointer = -1
    old_line = -1
    pilha = {}

    output_text.config(state=tk.NORMAL)
    output_text.delete('1.0', tk.END)

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
        "CEQ": lambda: CEQ(),
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

    items = tree.get_children()

    while program_instruction_counter < len(items):

        #print(stack_pointer)

        if paused or flag_passo_a_passo:  # Se pausado, aguarde
            root.update()  # Mantenha a interface responsiva
            time.sleep(0.1)
            continue

        item = items[program_instruction_counter]
        linha, instrucao, atributo1, atributo2, _ = tree.item(item, "values")
        
        tree.item(item, tags=("highlight",))

        tree.item(items[program_instruction_counter - 1], tags=("default",))
        tree.item(items[old_line], tags=("default",))

        
        # Executa a instrução se ela existir
        if instrucao in instrucoes:
            if atributo1 and atributo2:
                instrucoes[instrucao](int(atributo1), int(atributo2))
            elif atributo1:
                instrucoes[instrucao](int(atributo1))
            else:
                instrucoes[instrucao]()
        
        # Atualiza a interface e pausa no modo "Passo a Passo"
        if modo_execucao.get() == 2:
            root.update()  # Atualiza a interface
            flag_passo_a_passo = True

        if instrucao == "RD":
            paused = True

        if (instrucao != "JMP") or (instrucao != "JMPF") or (instrucao != "CALL") or (instrucao != "RETURN"):
            program_instruction_counter += 1
            print(program_instruction_counter)

    
    output_text.insert(tk.END, "Execução concluída.\n")
    output_text.config(state=tk.DISABLED)

def destrava_execucao():
    global flag_passo_a_passo
    flag_passo_a_passo = False

def LDC(valor):
    global stack_pointer
    global pilha
    stack_pointer += 1
    pilha[stack_pointer] = valor
    atualizar_pilha()

def LDV(endereco):
    global stack_pointer
    stack_pointer += 1
    endereco = int(endereco)

    if endereco in pilha:
        valor = pilha[endereco]
        pilha[stack_pointer] = valor
        atualizar_pilha()
    else:
        output_text.insert(tk.END, f"Erro: Endereço {endereco} não encontrado na pilha\n")

def ADD():
    global stack_pointer
    global pilha

    resultado = int(pilha[stack_pointer - 1]) + int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def SUB():
    global stack_pointer
    global pilha

    resultado = int(pilha[stack_pointer - 1]) - int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def MULT():
    global stack_pointer
    global pilha

    resultado = int(pilha[stack_pointer - 1]) * int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def DIVI():
    global stack_pointer
    global pilha

    resultado = int(pilha[stack_pointer - 1]) / int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def INV():
    global stack_pointer
    global pilha

    pilha[stack_pointer] = -int(pilha[stack_pointer])
    atualizar_pilha()

def AND():
    global stack_pointer
    global pilha

    if (pilha[stack_pointer - 1] == "1") and (pilha[stack_pointer] == "1"):
        pilha[stack_pointer - 1] = 1
    else:
        pilha[stack_pointer - 1] = 0
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def OR():
    global stack_pointer
    global pilha

    if (pilha[stack_pointer - 1] == "1") or (pilha[stack_pointer] == "1"):
        pilha[stack_pointer - 1] = 1
    else:
        pilha[stack_pointer - 1] = 0
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def NEG():
    global stack_pointer
    global pilha

    pilha[stack_pointer] = 1 - int(pilha[stack_pointer])
    atualizar_pilha()

def CME():
    global stack_pointer
    global pilha

    if pilha[stack_pointer - 1] < pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CMA():
    global stack_pointer
    global pilha

    if pilha[stack_pointer - 1] > pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CEQ():
    global stack_pointer
    global pilha

    if pilha[stack_pointer - 1] == pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CDIF():
    global stack_pointer
    global pilha

    if pilha[stack_pointer - 1] != pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CMEQ():
    global stack_pointer
    global pilha

    if pilha[stack_pointer - 1] <= pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def CMAQ():
    global stack_pointer
    global pilha

    if pilha[stack_pointer - 1] >= pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
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
        del pilha[max(pilha.keys())]
        stack_pointer -= 1
        atualizar_pilha()
    else:
        output_text.insert(tk.END, f"Erro: Endereço {endereco} não encontrado na pilha\n")

def JMP(linha):

    global old_line
    global program_instruction_counter
    global pilha
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

    if(pilha[stack_pointer] == "0"):
        program_instruction_counter = linha - 2
    
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

def ALLOC(endereco_inicial, quantidade):
    global stack_pointer
    global pilha

    k = 0
    
    while k <= quantidade - 1:
        stack_pointer += 1
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
        pilha[endereco_inicial + k] = pilha[stack_pointer]
        stack_pointer -= 1
        pilha[max(pilha.keys())]
        atualizar_pilha()
        k -= 1

def RD():
    global stack_pointer
    global pilha
    global paused

    def inserir_valor():
        global paused
        global stack_pointer
        global program_instruction_counter
        try:
            valor = int(entry.get())
            stack_pointer += 1
            pilha[stack_pointer] = valor
            
            atualizar_pilha()
            janela.destroy()
            
            paused = False

        except ValueError:
            output_text.insert(tk.END, "Erro: Insira um número inteiro válido.\n")

    janela = tk.Toplevel(root)
    janela.title("RD - Ler Valor")

    tk.Label(janela, text="Digite um valor inteiro:").pack(pady=5)

    entry = tk.Entry(janela)
    entry.pack(pady=5)
    entry.focus()

    tk.Button(janela, text="Inserir", command=inserir_valor).pack(pady=10)

def PRN():

    global pilha
    global stack_pointer

    output_text.insert(tk.END, f"{pilha[stack_pointer]}\n")
    del pilha[max(pilha.keys())]

    stack_pointer -= 1
    atualizar_pilha()

def CALL(linha):

    global stack_pointer
    global pilha
    global program_instruction_counter
    global old_line

    old_line = program_instruction_counter

    linha = int(linha)
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

    old_line = program_instruction_counter

    program_instruction_counter = pilha[stack_pointer] - 2
    del pilha[max(pilha.keys())]
    stack_pointer -= 1
    atualizar_pilha()

    

































def atualizar_pilha():
    global stack_pointer
    for row in tree_pilha.get_children():
        tree_pilha.delete(row)
    
    for endereco, valor in pilha.items():
        tree_pilha.insert("", "end", values=(endereco, valor))

def parar():
    output_text.insert(tk.END, "Execução Parada\n")
    #TODO: ajustar parada


# Inicializa a janela principal
root = tk.Tk()
root.title("Código de Máquina")
root.minsize(1000, 700)

# Frame superior
frame_superior = tk.Frame(root)
frame_superior.pack(side="top", fill="x")

# Frame inferior
frame_inferior = tk.Frame(root)
frame_inferior.pack(side="bottom", fill="x")

# Frame para a pilha
frame_pilha = tk.Frame(root)
frame_pilha.pack(side="right", fill="y", padx=30, pady=50)

# Frame para a saída de dados
frame_saida = tk.Frame(frame_inferior)
frame_saida.pack(side="left", padx=10)

# Frame para o modo de execução
frame_modo_execucao = tk.LabelFrame(frame_inferior, text="Modo de Execução")
frame_modo_execucao.pack(side="left", padx=10)

# Frame para os botões de execução
frame_botoes = tk.Frame(frame_inferior)
frame_botoes.pack(side="right", padx=10)

# Botão para abrir arquivo .obj
abrir_botao = tk.Button(frame_superior, text="Abrir .obj", command=carregar_arquivo)
abrir_botao.pack(side="left", padx=30, pady=10)

l = tk.Label(frame_superior, text = "Path do arquivo: ")
l.config(font =("Courier", 14))
l.pack(side= "left", padx=30, pady= 10)

entrada_arquivo = tk.StringVar()
entrada_exibir = tk.Entry(frame_superior, textvariable=entrada_arquivo, width=50)
entrada_exibir.pack(side="left", padx=50, pady=10)

# Configuração do Treeview para exibir o código de máquina
columns = ("Linha", "Instrução", "Atributo 1", "Atributo 2", "Comentário")
tree = ttk.Treeview(root, columns=columns, show="headings")

# Configuração de tags para destacar linhas no Treeview
tree.tag_configure("highlight", background="yellow")
tree.tag_configure("default", background="white")

for col in columns:
    tree.heading(col, text=col)
    tree.column(col, width=100, anchor="center")

tree.pack(side="left", fill="both", expand=True, padx=50, pady=50)

# Configuração do Treeview para exibir a pilha
columns_pilha = ("Endereço", "Valor")
tree_pilha = ttk.Treeview(frame_pilha, columns=columns_pilha, show="headings")

for col in columns_pilha:
    tree_pilha.heading(col, text=col)
    tree_pilha.column(col, width=80, anchor="center")

tree_pilha.pack(fill="both", expand=True)

# Saída de dados
label_saida = tk.Label(frame_saida, text="Saída de Dados:")
label_saida.pack()
output_text = tk.Text(frame_saida, width=70, height=5)
output_text.config(state=tk.DISABLED)
output_text.pack(pady=20, padx=30)

# Opções de modo de execução
modo_execucao = tk.IntVar(value=1)
radio_normal = tk.Radiobutton(frame_modo_execucao, text="Normal", variable=modo_execucao, value=1)
radio_passo = tk.Radiobutton(frame_modo_execucao, text="Passo a Passo", variable=modo_execucao, value=2)
radio_normal.pack(anchor="w")
radio_passo.pack(anchor="w")

# Botões de execução e parada
botao_executar = tk.Button(frame_botoes, text="Executar", command=executar)
botao_parar = tk.Button(frame_botoes, text="Parar", command=parar)
botao_executar.pack(side="left", padx=5)
botao_parar.pack(side="left", padx=5)

botao_passa = tk.Button(frame_botoes, text="Passo", command=destrava_execucao)
botao_passa.pack(side="left", padx=5)


root.mainloop()
