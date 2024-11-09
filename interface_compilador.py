import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess

def abrir_arquivo():
    caminho_arquivo = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
    if caminho_arquivo:
        entrada_arquivo.set(caminho_arquivo)
        
        with open(caminho_arquivo, 'r') as arquivo:
            conteudo = arquivo.read()
            saida_texto_principal.delete(1.0, tk.END)
            saida_texto_principal.insert(tk.END, conteudo)

def compilar():
    caminho_arquivo = entrada_arquivo.get()
    if not caminho_arquivo:
        messagebox.showwarning("Aviso", "Selecione um arquivo primeiro!")
        return
    
    compilacao = subprocess.run([r"C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe", "-o", "compilador", "compilador.c"])

    if compilacao.returncode == 0:
        print("Compilação bem-sucedida!")
    
    try:
        saida_texto_resultado.config(state=tk.NORMAL)
        resultado = subprocess.run(['./compilador'], capture_output=True, text=True, encoding='latin1')
        saida_texto_resultado.delete(1.0, tk.END)
        saida_texto_resultado.insert(tk.END, "Saída do Compilador:\n" + resultado.stdout)
        saida_texto_resultado.config(state=tk.DISABLED)
    except Exception as e:
        messagebox.showerror("Erro", f"Ocorreu um erro ao compilar: {e}")

def salvar_arquivo():
    caminho_arquivo = entrada_arquivo.get()
    if not caminho_arquivo:
        caminho_arquivo = filedialog.asksaveasfilename(defaultextension=".txt", filetypes=[("Text files", "*.txt")])
        if not caminho_arquivo:
            return
    
    try:
        with open(caminho_arquivo, 'w') as arquivo:
            conteudo = saida_texto_principal.get(1.0, tk.END)
            arquivo.write(conteudo.strip())
        entrada_arquivo.set(caminho_arquivo)
        messagebox.showinfo("Sucesso", "Arquivo salvo com sucesso!")
    except Exception as e:
        messagebox.showerror("Erro", f"Ocorreu um erro ao salvar o arquivo: {e}")


def atualizar_posicao_cursor(event):
    posicao = saida_texto_principal.index(tk.INSERT)
    linha, coluna = posicao.split('.')
    label_posicao.config(text=f"Linha: {linha}, Coluna: {coluna}")

janela = tk.Tk()
janela.title("Interface de Compilação")
janela.minsize(500, 500)

label_posicao = tk.Label(janela, text="Linha: 1, Coluna: 1")
label_posicao.pack(side="bottom", anchor="w", padx=10, pady=5)

frame_inferior = tk.Frame(janela)
frame_inferior.pack(side="bottom", fill="x")

frame_superior = tk.Frame(janela)
frame_superior.pack(side="top", fill="x")

entrada_arquivo = tk.StringVar()

l = tk.Label(frame_superior, text = "Path do arquivo: ")
l.config(font =("Courier", 14))
l.pack(side= "left", padx=30, pady= 10)

btn_abrir = tk.Button(frame_inferior, text="Abrir Arquivo", command=abrir_arquivo)
btn_abrir.pack(side="left", padx=10, pady=10)

entrada_exibir = tk.Entry(frame_superior, textvariable=entrada_arquivo, width=50)
entrada_exibir.pack(side="left", padx=50, pady=10)

btn_compilar = tk.Button(frame_inferior, text="Compilar", command=compilar)
btn_compilar.pack(side="right", padx=10, pady=10)

btn_salvar = tk.Button(frame_inferior, text="Salvar", command=salvar_arquivo)
btn_salvar.pack(side="right", padx=10, pady=10)

saida_texto_principal = tk.Text(janela, wrap='none')
saida_texto_principal.pack(side='left', fill='both', expand=True)

scroll_x = tk.Scrollbar(frame_inferior, orient='horizontal', command=saida_texto_principal.xview)
scroll_x.pack(side='top', fill='x')

saida_texto_resultado = tk.Text(frame_inferior, height=15, width=100)
saida_texto_resultado.pack(padx=50)
saida_texto_resultado.config(state=tk.DISABLED)


saida_texto_principal.configure( xscrollcommand=scroll_x.set)

saida_texto_principal.bind("<KeyRelease>", atualizar_posicao_cursor)
saida_texto_principal.bind("<ButtonRelease-1>", atualizar_posicao_cursor) 


janela.mainloop()
