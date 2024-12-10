# MATEUS JOSÉ WHITAKER FILIPE 21005080
# JOÃO LUIS BEATO CARDOSO 21012289
# RAFAEL RODRIGUES SATO 21006207

import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess
import os

# Caminho local para o GCC embutido
gcc_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'MinGW', 'bin', 'gcc.exe')

# Verifica se o GCC está disponível no caminho embutido
if not os.path.isfile(gcc_path):
    raise EnvironmentError("O GCC embutido não foi encontrado. Verifique os arquivos do programa.")

def abrir_arquivo():
    # Abre um arquivo para leitura e exibe o conteúdo na área de texto
    caminho_arquivo = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
    if caminho_arquivo:
        entrada_arquivo.set(caminho_arquivo)
        with open(caminho_arquivo, 'r') as arquivo:
            conteudo = arquivo.read()
            saida_texto_principal.delete(1.0, tk.END)
            saida_texto_principal.insert(tk.END, conteudo)

def compilar():
    # Recebe arquivo que vai ser compilado, caso não, gera mensagem de aviso
    caminho_arquivo = entrada_arquivo.get()                                             
    if not caminho_arquivo:
        messagebox.showwarning("Aviso", "Selecione um arquivo primeiro!")
        return

    # Remove qualquer tipo de erro marcado
    saida_texto_principal.tag_remove("erro", "1.0", tk.END) 

    # Executa/Compila o código do compilador feito em C
    try:
        compilacao = subprocess.run(
            [gcc_path, "-o", "compilador", os.path.join(os.path.dirname(__file__), "compilador.c")],
            capture_output=True,
            text=True
        )

        # Caso falhe o início da compilação, gera erro
        if compilacao.returncode != 0:
            messagebox.showerror("Erro na Compilação", f"Erro ao compilar o código C:\n{compilacao.stderr}")
            return
        print("Compilação bem-sucedida!")
    except Exception as e:
        messagebox.showerror("Erro", f"Ocorreu um erro ao compilar com o GCC: {e}")
        return

    # Executa o compilador passando o nome do arquivo como argumento
    try:
        resultado = subprocess.run(
            [os.path.join(os.path.dirname(__file__), "compilador"), caminho_arquivo],
            capture_output=True,
            text=True,
            encoding='latin1'
        )
        # Declaração do campo de texto para habilitar, limpar, inserir os dados resultados do terminal e desabilitar edição do campo, respectivamente
        saida_texto_resultado.config(state=tk.NORMAL)
        saida_texto_resultado.delete(1.0, tk.END)
        saida_texto_resultado.insert(tk.END, "Saída do Compilador:\n" + resultado.stdout)
        saida_texto_resultado.config(state=tk.DISABLED)
    except Exception as e:
        messagebox.showerror("Erro", f"Ocorreu um erro ao executar o compilador: {e}")

# Função salva o arquivo escrito no editor principal
def salvar_arquivo():
    # Recebe o path do arquivo
    caminho_arquivo = entrada_arquivo.get()
    if not caminho_arquivo:
        caminho_arquivo = filedialog.asksaveasfilename(defaultextension=".txt", filetypes=[("Text files", "*.txt")])
        if not caminho_arquivo:
            return

    # Escreve os dados atualizados no arquivo com permissão de escrita, aplicando as alterações atuais
    try:
        with open(caminho_arquivo, 'w') as arquivo:
            conteudo = saida_texto_principal.get(1.0, tk.END)
            arquivo.write(conteudo.strip())
        entrada_arquivo.set(caminho_arquivo)
        messagebox.showinfo("Sucesso", "Arquivo salvo com sucesso!")
    except Exception as e:
        messagebox.showerror("Erro", f"Ocorreu um erro ao salvar o arquivo: {e}")

# Atualiza em qual linha e coluna o cursor está posicionado
def atualizar_posicao_cursor(event):
    posicao = saida_texto_principal.index(tk.INSERT)
    linha, coluna = posicao.split('.')
    label_posicao.config(text=f"Linha: {linha}, Coluna: {coluna}")

# Quando ocorre um erro no compilador, ele será escrito em saida_texto_resultado. Essa função faz com que, ao clicar no erro, ele consiga interpretar que é um erro e extrair o int da linha, podendo grifar e destacar o erro, utilizando tag
def destacar_linha_erro(event):
    index = saida_texto_resultado.index(f"@{event.x},{event.y}")
    linha_clicada = int(index.split('.')[0])
    conteudo_linha = saida_texto_resultado.get(f"{linha_clicada}.0", f"{linha_clicada}.end")
    if conteudo_linha.startswith("LINHA") and "[ERRO]" in conteudo_linha:
        try:
            numero_linha = int(conteudo_linha.split()[1].strip(":"))
        except ValueError:
            messagebox.showerror("Erro", "Número de linha inválido no erro.")
            return
        saida_texto_principal.tag_remove("erro", "1.0", tk.END)
        saida_texto_principal.tag_add("erro", f"{numero_linha}.0", f"{numero_linha}.end")
        saida_texto_principal.see(f"{numero_linha}.0")

# Configuração da interface base
janela = tk.Tk()
janela.title("Interface de Compilação")
janela.minsize(500, 500)

# Linha e coluna do cursor
label_posicao = tk.Label(janela, text="Linha: 1, Coluna: 1")
label_posicao.pack(side="bottom", anchor="w", padx=10, pady=5)

# Frames superiores e inferiores
frame_inferior = tk.Frame(janela)
frame_inferior.pack(side="bottom", fill="x")

frame_superior = tk.Frame(janela)
frame_superior.pack(side="top", fill="x")

# Entrada e botões
entrada_arquivo = tk.StringVar()

# Texto e posicionamento de "Path de arquivo: "
l = tk.Label(frame_superior, text="Path do arquivo: ")
l.config(font=("Courier", 14))
l.pack(side="left", padx=30, pady=10)

# Criação e posicionamento do botão de abrir o arquivo
btn_abrir = tk.Button(frame_inferior, text="Abrir Arquivo", command=abrir_arquivo)
btn_abrir.pack(side="left", padx=10, pady=10)

# Criação do print do path do arquivo selecionado
entrada_exibir = tk.Entry(frame_superior, textvariable=entrada_arquivo, width=50)
entrada_exibir.pack(side="left", padx=50, pady=10)

# Criação do botão para compilar
btn_compilar = tk.Button(frame_inferior, text="Compilar", command=compilar)
btn_compilar.pack(side="right", padx=10, pady=10)

# Criação e posicionamento do botão de salvar arquivo
btn_salvar = tk.Button(frame_inferior, text="Salvar", command=salvar_arquivo)
btn_salvar.pack(side="right", padx=10, pady=10)

# Área de texto principal
saida_texto_principal = tk.Text(janela, wrap='none')
saida_texto_principal.pack(side='left', fill='both', expand=True)

# Barra de rolagem horizontal
scroll_x = tk.Scrollbar(frame_inferior, orient='horizontal', command=saida_texto_principal.xview)
scroll_x.pack(side='top', fill='x')

# Área de saída do resultado
saida_texto_resultado = tk.Text(frame_inferior, height=15, width=100)
saida_texto_resultado.pack(padx=50)
saida_texto_resultado.config(state=tk.DISABLED)

# Configuração de scroll e destaques
saida_texto_principal.configure(xscrollcommand=scroll_x.set)
saida_texto_principal.tag_configure("erro", background="yellow", foreground="red")

# Eventos
saida_texto_principal.bind("<KeyRelease>", atualizar_posicao_cursor)
saida_texto_principal.bind("<ButtonRelease-1>", atualizar_posicao_cursor)
saida_texto_resultado.bind("<Button-1>", destacar_linha_erro)

janela.mainloop()
