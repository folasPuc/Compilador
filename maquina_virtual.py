import tkinter as tk
from tkinter import filedialog
from tkinter import ttk
import re
import time

    #instruçoes: /LDC, /LDV, /ADD, /SUB, /MULT, /DIVI, /INV, /AND, /OR, /NEG, /CME, /CMA, /CEQ, /CDIF, /CMEQ, /CMAQ, STR, JMP, JMPF, NULL, START, ALLOC, DALLOC, RD, PRN, HLT, CALL, RETURN

stack_pointer = -1

def carregar_arquivo():
    filepath = filedialog.askopenfilename(filetypes=[("Object Files", "*.obj")])
    if filepath:
        entrada_arquivo.set(filepath)
        with open(filepath, 'r') as file:
            conteudo = file.readlines()
        exibir_conteudo(conteudo)

def exibir_conteudo(conteudo):
    for row in tree.get_children():
        tree.delete(row)

    for linha in conteudo:
        linha = linha.strip()
        match = re.match(r"(\d*)\s*(\w+)\s*(\d*)\s*(\d*)", linha)
        if match:
            num_linha = match.group(1) or ""
            instrucao = match.group(2)
            atributo1 = match.group(3) or ""
            atributo2 = match.group(4) or ""
            comentario = "" 

            tree.insert("", "end", values=(num_linha, instrucao, atributo1, atributo2, comentario))

def executar():
    modo = "Normal" if modo_execucao.get() == 1 else "Passo a Passo"
    output_text.insert(tk.END, f"Executando em modo {modo}\n")
    
    # Exemplo de instruções (adicionar mais conforme necessário)
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
        "STR": lambda x: STR()
    }

    for item in tree.get_children():
        linha, instrucao, atributo1, atributo2, _ = tree.item(item, "values")
        
        # Executa a instrução se ela estiver definida
        if instrucao in instrucoes:

            if atributo2:
                instrucoes[instrucao](atributo1)(atributo2)
            elif atributo1:
                instrucoes[instrucao](atributo1)
            else:
                instrucoes[instrucao]()
            output_text.insert(tk.END, f"Executando {instrucao} {atributo1}\n")
            
            # Atualizar a visualização em "Passo a Passo"
            if modo_execucao.get() == 2: 
                root.update()  # Atualiza a interface
                time.sleep(1)  # Pausa para simular o passo a passo

def LDC(valor):
    global stack_pointer
    stack_pointer += 1
    pilha[stack_pointer] = valor
    atualizar_pilha()
    output_text.insert(tk.END, f"Carregado valor {valor} na posição {stack_pointer}\n")

def LDV(endereco):
    global stack_pointer
    stack_pointer += 1
    endereco = int(endereco)

    if endereco in pilha:
        valor = pilha[endereco]
        pilha[stack_pointer] = valor
        atualizar_pilha()
        output_text.insert(tk.END, f"Valor {valor} carregado do endereço {endereco}\n")
    else:
        output_text.insert(tk.END, f"Erro: Endereço {endereco} não encontrado na pilha\n")

def ADD():
    global stack_pointer

    resultado = int(pilha[stack_pointer - 1]) + int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    stack_pointer -= 1
    atualizar_pilha()

def SUB():
    global stack_pointer

    resultado = int(pilha[stack_pointer - 1]) - int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    stack_pointer -= 1
    atualizar_pilha()

def MULT():
    global stack_pointer

    resultado = int(pilha[stack_pointer - 1]) * int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    stack_pointer -= 1
    atualizar_pilha()

def DIVI():
    global stack_pointer

    resultado = int(pilha[stack_pointer - 1]) / int(pilha[stack_pointer])
    pilha[stack_pointer - 1] = resultado
    stack_pointer -= 1
    atualizar_pilha()

def INV():
    global stack_pointer

    pilha[stack_pointer] = -int(pilha[stack_pointer])
    atualizar_pilha()

def AND():
    global stack_pointer

    if (pilha[stack_pointer - 1] == "1") and (pilha[stack_pointer] == "1"):
        pilha[stack_pointer - 1] = 1
    else:
        pilha[stack_pointer - 1] = 0
    stack_pointer -= 1
    atualizar_pilha()

def OR():
    global stack_pointer

    if (pilha[stack_pointer - 1] == "1") or (pilha[stack_pointer] == "1"):
        pilha[stack_pointer - 1] = 1
    else:
        pilha[stack_pointer - 1] = 0
    stack_pointer -= 1
    atualizar_pilha()

def NEG():
    global stack_pointer

    pilha[stack_pointer] = 1 - int(pilha[stack_pointer])
    atualizar_pilha()

def CME():
    global stack_pointer

    if pilha[stack_pointer - 1] < pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    stack_pointer -= 1
    atualizar_pilha()

def CMA():
    global stack_pointer

    if pilha[stack_pointer - 1] > pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    stack_pointer -= 1
    atualizar_pilha()

def CEQ():
    global stack_pointer

    if pilha[stack_pointer - 1] == pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    stack_pointer -= 1
    atualizar_pilha()

def CDIF():
    global stack_pointer

    if pilha[stack_pointer - 1] != pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    stack_pointer -= 1
    atualizar_pilha()

def CMEQ():
    global stack_pointer

    if pilha[stack_pointer - 1] <= pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    stack_pointer -= 1
    atualizar_pilha()

def CMAQ():
    global stack_pointer

    if pilha[stack_pointer - 1] >= pilha[stack_pointer]:
        pilha[stack_pointer - 1] = "1"    
    else:
        pilha[stack_pointer - 1] = "0"
    stack_pointer -= 1
    atualizar_pilha()

def STR(endereco):
    print("a")

def atualizar_pilha():
    global stack_pointer
    for row in tree_pilha.get_children():
        tree_pilha.delete(row)
    
    for endereco, valor in pilha.items():
        tree_pilha.insert("", "end", values=(endereco, valor))

    output_text.insert(tk.END, f"Stack pointer atual: {stack_pointer}\n")

def parar():
    output_text.insert(tk.END, "Execução Parada\n")





















































# Inicializa a janela principal
root = tk.Tk()
root.title("Código de Máquina")
root.minsize(1000, 700)

# Inicializa a estrutura da pilha
pilha = {}

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

root.mainloop()
