// C program to read a file using fgetc()
// ESTAS SAO AS BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>



// Definição da struct Identificador
typedef struct Identificador
{
    char nome[50];                 // Nome do identificador (lexema)
    char escopo[50];               // Nome do escopo (procedimento ou função)
    char tipo[20];                 // Tipo do identificador (int, float, etc.)
    char memoria[20];                 // Endereço de memória alocado
    struct Identificador *proximo; // Ponteiro para o próximo identificador
} Identificador;


void limparArquivo() {
    FILE *arquivo = fopen("output.obj", "w"); // Abre o arquivo no modo "w", apagando todo o conteúdo
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fclose(arquivo); // Fecha o arquivo para concluir o processo de limpeza
}


void Gera(char* rotulo, char* instrucao, char* atributo_1, char* atributo_2){
    // Abrir arquivo em modo de adição ("append")
    FILE* arquivo = fopen("output.obj", "a");
    // Verificar se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arquivo, "%s%s%s%s\n", rotulo, instrucao, atributo_1, atributo_2);
    // Fechar o arquivo
    fclose(arquivo);
}



// Função para criar um novo nó de identificador
Identificador *criarIdentificador(const char *nome, const char *escopo, const char *tipo, const char *memoria)
{
    Identificador *novoIdentificador = (Identificador *)malloc(sizeof(Identificador));
    if (novoIdentificador != NULL)
    {
        strncpy(novoIdentificador->nome, nome, sizeof(novoIdentificador->nome));
        strncpy(novoIdentificador->escopo, escopo, sizeof(novoIdentificador->escopo));
        strncpy(novoIdentificador->tipo, tipo, sizeof(novoIdentificador->tipo));
        strncpy(novoIdentificador->memoria, memoria, sizeof(novoIdentificador->memoria));
    }
    return novoIdentificador;
}

// Função para inserir um identificador no início da lista (mais recente primeiro)
void inserirIdentificador(Identificador **head, const char *nome, const char *escopo, const char *tipo, const char *memoria)
{
    Identificador *novoIdentificador = criarIdentificador(nome, escopo, tipo, memoria);
    novoIdentificador->proximo = *head; // O novo identificador aponta para o antigo primeiro
    *head = novoIdentificador;          // O novo identificador se torna o primeiro da lista
}

// Função para buscar um identificador na lista (do mais recente ao mais antigo)
Identificador *buscarIdentificador(Identificador *head, const char *nome)
{
    Identificador *atual = head;
    while (atual != NULL)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL; // Se não encontrar, retorna NULL
}

void coloca_tipo(Identificador **head, const char *tipo)
{
    Identificador *atual = *head;
    Identificador *anterior = NULL;

    while (atual != NULL)
    {
        if (strcmp(atual->tipo, "variavel") == 0)
        {
            strcpy(atual->tipo, tipo);
        }

        anterior = atual;
        atual = atual->proximo;
    }
}

void coloca_tipo_func(Identificador **head, const char *lexema, const char *tipo)
{
    Identificador *atual = *head;
    Identificador *anterior = NULL;

    while (atual != NULL)
    {
        if (strcmp(atual->nome, lexema) == 0)
        {
            strcpy(atual->tipo, tipo);
            break;
        }

        anterior = atual;
        atual = atual->proximo;
    }
}

boolean buscaAteMarcaPrimeiraOcorrencia(Identificador **head, const char *lexema)
{

    Identificador *atual = *head;


    // Percorrer a lista até encontrar o lexema ou a "MARCA" (escopo == "L")
    while (atual != NULL)
    {
        // Verificar se o identificador atual tem o mesmo nome (lexema)
        if (strcmp(atual->nome, lexema) == 0)
        {
            return 1; // Verdadeiro, achou o lexema
        }
        // Verificar se atingimos a marca ("L")
        if (strcmp(atual->escopo, "L") == 0)
        {
            return 0; // Falso, encontrou a marca antes do lexema
        }

        // Ir para o próximo nó
        atual = atual->proximo;
    }

    return 0; // Falso, não achou nem o lexema nem a marca
}

// Função para imprimir a tabela de símbolos
void imprimirTabelaSimbolos(Identificador *head)
{
    Identificador *atual = head;
    while (atual != NULL)
    {
        printf("Nome: %s, Escopo: %s, Tipo: %s, Endereço: %s\n",
               atual->nome, atual->escopo, atual->tipo, atual->memoria);

        atual = atual->proximo;
    }
}

// Função para liberar a memória da lista de identificadores
void liberarTabelaSimbolos(Identificador *head)
{
    Identificador *atual = head;
    while (atual != NULL)
    {
        Identificador *temp = atual;
        atual = atual->proximo;
        free(temp); // Libera a memória do nó atual
    }
}

void analisa_chamada_procedimento();
void analisa_chamada_funcao();
void analisa_escreva();
void analisa_leia();
void analisa_atribuicao();
void analisa_atrib_chprocedimento();
void analisa_comando_simples();
void analisa_comandos();
void analisa_tipo();
void analisa_variaveis();
void analisa_et_variaveis();
void analisa_bloco();
void analisa_enquanto();
void analisa_se();
void analisa_subrotinas();
void analisa_declaracao_procedimento();
void analisa_declaracao_funcao();
void analisa_expressao();
void analisa_expressao_simples();
void analisa_termo();
void analisa_fator();


// tava aqui as variavel
int line_counter = 1;
char lista_infix[50][50]; // Agora uma matriz para armazenar até 50 lexemas
char lista_postfix_global[50][50];
int listIndex = 0;
int len_lista_postfix = 0;
int end_disp_momento = 0;
int end_var = 1;
int aloca_var = 1;
const char* resp;
FILE *fptr;
char ch;
Identificador *tabelaSimbolos = NULL;
int ROTULO = 1;

typedef struct
{
    char lexema[50];
    char simbolo[50];
} Token;

Token token;

void addListaInFix(const char *termo)
{
    if (listIndex < 50)
    {                                               // Verifica se a lista tem espaço para mais lexemas
        strncpy(lista_infix[listIndex], termo, 50); // Copia o termo para a posição da lista
        listIndex++;
    }
    else
    {
        printf("[ERRO]: Lista de infix está cheia!\n");
        exit(0);
    }
}

// Função para desempilhar até a primeira marca (tipo == "L")
void desempilharAteMarca(Identificador **head)
{
    Identificador *atual = *head;
    Identificador *anterior = NULL;

    int count = 0;

    // Percorrer a lista até encontrar a "MARCA" ("L")
    while (atual != NULL && strcmp(atual->escopo, "L") != 0)
    {

        if(strcmp(atual->tipo, "procedimento") != 0 && strcmp(atual->tipo, "funcao inteiro") != 0 && strcmp(atual->tipo, "funcao booleano") != 0) {
            count++;
        } else {
        free(anterior); // Liberar a memória do identificador desempilhado
        }

        anterior = atual;
        atual = atual->proximo;
    }

    // Se encontramos a marca
    if (atual != NULL)
    {
        strcpy(atual->escopo, ""); // Remover a marca, não o nó
    }
    else
    {

    }

    // Atualizar o head para o nó atual (marca ou NULL se esvaziado)
    *head = atual;
    end_var = count;

}

// Função para listar todos os lexemas armazenados na lista de infix
void listarListaInfix()
{
    printf("Lista infix: \n");
    for (int i = 0; i < listIndex; i++)
    {
        printf("%s\n", lista_infix[i]); // Imprime cada lexema
    }
}

// Função para resetar a lista de infix (limpar a lista)
void resetListaInfix()
{
    // Resetando o índice para 0, indicando que a lista está vazia
    listIndex = 0;
    len_lista_postfix = 0;

    // Opcional: Limpar a matriz de lexemas (não necessário se só resetarmos o índice)
    for (int i = 0; i < 50; i++)
    {
        lista_infix[i][0] = '\0'; // Limpa cada posição da matriz de lexemas
    }

}
// PORRA
//  Função para obter a precedência de um operador
int prec(const char *op)
{
    // Precedência de operadores aritméticos
    if (strcmp(op, "+u") == 0 || strcmp(op, "-u") == 0) // Precedência maior (positivo e negativo)
        return 7;
    else if (strcmp(op, "*") == 0 || strcmp(op, "div") == 0) // Precedência média (multiplicação e divisão)
        return 6;
    else if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
    { // Precedência menor (soma e subtração)
        return 5;
    }

    // Precedência de operadores relacionais (todos têm a mesma precedência)
    else if (strcmp(op, ">") == 0 || strcmp(op, ">=") == 0 ||
             strcmp(op, "<") == 0 || strcmp(op, "<=") == 0 ||
             strcmp(op, "=") == 0 || strcmp(op, "!=") == 0)
    {
        return 4;
    }

    // Precedência de operadores lógicos (nao, e, ou)
    else if (strcmp(op, "nao") == 0)
    { // Maior precedência (negação)
        return 3;
    }
    else if (strcmp(op, "e") == 0) // Precedência média (AND)
        return 2;
    else if (strcmp(op, "ou") == 0)
    { // Menor precedência (OR)
        return 1;
    }

    // Retorna -1 para tokens desconhecidos
    return -1;
}

// Função de conversão infix -> postfix
void infixToPostfix(char lista_infix[50][50])
{
    char result[50][50] = {0}; // Resultado final em notação postfix
    char stack[50][50];        // Pilha para operadores (máximo de 50 operadores, com até MAX_OPERADOR caracteres)
    int top = -1;              // Índice da pilha
    int j = 0;                 // Índice para o resultado
    int resultIndex = 0;
    // Usa listIndex para determinar o número de tokens e iterar até ele
    for (int i = 0; i < listIndex; i++)
    {
        char *token = lista_infix[i]; // Obtém o lexema do token

        // Se o token for um operando (letra ou número)
        if (isalnum(token[0]) && (strcmp(token, "ou") != 0 && strcmp(token, "e") != 0 && strcmp(token, "nao") != 0 && strcmp(token, "+u") != 0 && strcmp (token, "-u") != 0 && strcmp(token, "div") != 0))
        {
            strcpy(result[resultIndex++], token);
        }
        // Se o token for um parêntese de abertura
        else if (strcmp(token, "(") == 0)
        {
            strcpy(stack[++top], "("); // Empilha o parêntese
        }
        // Se o token for um parêntese de fechamento
        else if (strcmp(token, ")") == 0)
        {
            while (top != -1 && strcmp(stack[top], "(") != 0)
            {
                strcpy(result[resultIndex++], stack[top--]); // Desempilha até encontrar '('
            }
            top--; // Remove o '(' da pilha
        }
        // Caso o token seja um operador
        else
        {
            while (top != -1 && prec(token) <= prec(stack[top]))
            {
                strcpy(result[resultIndex++], stack[top--]); // Desempilha os operadores de maior ou igual precedência
            }
            strcpy(stack[++top], token); // Empilha o operador atual
        }
    }

    // Desempilha todos os operadores restantes
    while (top != -1)
    {
        strcpy(result[resultIndex++], stack[top--]);
    }

    // Imprime o resultado
    for (int i = 0; i < resultIndex; i++)
    {
        strcpy(lista_postfix_global[i], result[i]);
        printf(lista_postfix_global[i]);
        len_lista_postfix++;
    }
}

const char* avaliarPostfix(char lista_postfix[50][50])
{
    char stack[50][50] = {0};
    char num1[50];
    char num2[50];
    char tipo1[50];
    char tipo2[50];
    int top = -1;

    for (int i = 0; i < len_lista_postfix; i++)
    {


        // eh um operando, letra ou numero
        if (isalnum(lista_postfix[i][0]) && (strcmp(lista_postfix[i], "ou") != 0 && strcmp(lista_postfix[i], "e") != 0 && strcmp(lista_postfix[i], "nao") != 0 && strcmp(lista_postfix[i], "+u") != 0 && strcmp(lista_postfix[i], "-u") != 0 && strcmp(lista_postfix[i], "div") != 0))
        {
            strcpy(stack[++top], lista_postfix[i]);
        }
        // eh um operador que vai ter que voltar 2x para verificar os operandos
        else
        {
            // volta 2x, tem que ser inteiro + inteiro = inteiro
            if (strcmp(lista_postfix[i], "+") == 0 || strcmp(lista_postfix[i], "-") == 0 || strcmp(lista_postfix[i], "div") == 0 || strcmp(lista_postfix[i], "*") == 0)
            {

                strcpy(num1, stack[top]);
                strcpy(num2, stack[--top]);


                if (isdigit(num1[0])) {
                    strcpy(tipo1, "inteiro");
                }

                if (strcmp(num1, "inteiro") == 0) {
                    strcpy(tipo1, "inteiro");
                }

                if (isalpha(num1[0])) {
                    //tem que buscar na tabela essa merda
                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, num1);
                    if (encontrado != NULL) {
                        if (strcmp(encontrado->tipo, "inteiro") == 0) {

                            strcpy(tipo1, encontrado->tipo);
                            strcpy(stack[top], tipo1);
                        } else {
                            printf("LINHA %d: [ERRO] tipo1 incompativel no +-*", line_counter);
                            exit(0);
                        }

                    } else {

                    }
                }


                //para o num2

                if (isdigit(num2[0])) {
                    strcpy(tipo2, "inteiro");
                }

                if (strcmp(num2, "inteiro") == 0) {
                    strcpy(tipo2, "inteiro");
                }

                if (isalpha(num2[0])) {
                    //tem que buscar na tabela essa merda
                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, num2);
                    if (encontrado != NULL) {
                        if (strcmp(encontrado->tipo, "inteiro") == 0) {

                            strcpy(tipo2, encontrado->tipo);
                            strcpy(stack[top], tipo2);
                        } else {
                            printf("LINHA %d: [ERRO] tipo2 incompativel no +-*", line_counter);
                            exit(0);
                        }

                    } else {

                    }
                }


                if (strcmp(tipo1, "inteiro") == 0 && strcmp(tipo2, "inteiro") == 0) {

                    strcpy(stack[top], "inteiro");
                }


            }

            // aqui vai voltar 2x, tem que ser inteiro + inteiro = booleano
            if (strcmp(lista_postfix[i], "<") == 0 || strcmp(lista_postfix[i], "<=") == 0 || strcmp(lista_postfix[i], ">") == 0 || strcmp(lista_postfix[i], ">=") == 0 || strcmp(lista_postfix[i], "!=") == 0 || strcmp(lista_postfix[i], "=") == 0)
            {

                strcpy(num1, stack[top]);
                strcpy(num2, stack[--top]);

                if (isdigit(num1[0])) {
                    strcpy(tipo1, "inteiro");
                }

                if (strcmp(num1, "inteiro") == 0) {
                    strcpy(tipo1, "inteiro");
                }

                if (isalpha(num1[0])) {
                    //tem que buscar na tabela essa merda
                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, num1);
                    if (encontrado != NULL) {
                        if (strcmp(encontrado->tipo, "inteiro") == 0) {

                            strcpy(tipo1, encontrado->tipo);
                            strcpy(stack[top], tipo1);
                        } else {
                            printf("LINHA %d: [ERRO] tipo1 incompativel no relacional", line_counter);
                            exit(0);
                        }

                    } else {

                    }
                }


                //para o num2

                if (isdigit(num2[0])) {
                    strcpy(tipo2, "inteiro");
                }

                if (strcmp(num2, "inteiro") == 0) {
                    strcpy(tipo2, "inteiro");
                }

                if (isalpha(num2[0])) {
                    //tem que buscar na tabela essa merda
                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, num2);
                    if (encontrado != NULL) {
                        if (strcmp(encontrado->tipo, "inteiro") == 0) {

                            strcpy(tipo2, encontrado->tipo);
                            strcpy(stack[top], tipo2);
                        } else {
                            printf("LINHA %d: [ERRO] tipo2 incompativel no relacional", line_counter);
                            exit(0);
                        }

                    } else {

                    }
                }


                if (strcmp(tipo1, "inteiro") == 0 && strcmp(tipo2, "inteiro") == 0) {

                    strcpy(stack[top], "booleano");
                }

            }

            if (strcmp(lista_postfix[i], "e") == 0 || strcmp(lista_postfix[i], "ou") == 0)
            {

                strcpy(num1, stack[top]);
                strcpy(num2, stack[--top]);

                if (strcmp(num1, "booleano") == 0) {
                    strcpy(tipo1, "booleano");

                } else {

                if (isalpha(num1[0]) && strcmp(num1, "booleano") != 0) {
                    //tem que buscar na tabela essa merda
                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, num1);
                    if (encontrado != NULL) {
                        if (strcmp(encontrado->tipo, "booleano") == 0) {

                            strcpy(tipo1, encontrado->tipo);
                            strcpy(stack[top], tipo1);
                        } else {
                            printf("LINHA %d: [ERRO] tipo1 incompativel no e/ou", line_counter);
                        }

                    } else {

                    }
                } else {
                    printf("LINHA %d: [ERRO] Tipo1 incompativel", line_counter);
                }
            }


                //para o num2

                if (strcmp(num2, "booleano") == 0) {
                    strcpy(tipo2, "booleano");
                } else {

                if (isalpha(num2[0]) && strcmp(num2, "booleano") != 0) {
                    //tem que buscar na tabela essa merda
                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, num2);
                    if (encontrado != NULL) {
                        if (strcmp(encontrado->tipo, "booleano") == 0) {

                            strcpy(tipo2, encontrado->tipo);
                            strcpy(stack[top], tipo2);
                        } else {
                            printf("LINHA %d: [ERRO] tipo2 incompativel no e/ou prob numero", line_counter);
                        }

                    } else {

                    }
                } else {
                    printf("LINHA %d: [ERRO] Tipo2 incompativel", line_counter);
                }
            }


                if (strcmp(tipo1, "booleano") == 0 && strcmp(tipo2, "booleano") == 0) {

                    strcpy(stack[top], "booleano");
                }
            }

            if (strcmp(lista_postfix[i], "+-u") == 0)
            {
                strcpy(num1, stack[top]);

                if (isdigit(num1[0])) {
                    strcpy(tipo1, "inteiro");
                    strcpy(stack[top], tipo1);
                }

                if (strcmp(num1, "inteiro") == 0) {
                    strcpy(tipo1, "inteiro");
                    strcpy(stack[top], tipo1);
                }

                if (isalpha(num1[0])) {
                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, num1);
                    if (encontrado != NULL) {
                        if (strcmp(encontrado->tipo, "inteiro") == 0) {

                            strcpy(tipo1, encontrado->tipo);
                            strcpy(stack[top], tipo1);
                        } else {
                            printf("LINHA %d: [ERRO] tipo incompativel no +-u", line_counter);
                        }

                    } else {

                    }
                }

                if (strcmp(tipo1, "inteiro") == 0) {

                }

            }

            if (strcmp(lista_postfix[i], "nao") == 0)
            {

                strcpy(num1, stack[top]);

                if (strcmp(num1, "booleano") == 0) {
                    strcpy(tipo1, "booleano");
                } else {

                if (isalpha(num1[0])) {
                    //tem que buscar na tabela essa merda
                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, num1);
                    if (encontrado != NULL) {
                        if (strcmp(encontrado->tipo, "booleano") == 0) {

                            strcpy(tipo1, encontrado->tipo);
                            strcpy(stack[top], tipo1);
                        } else {
                            printf("LINHA %d: [ERRO] tipo1 incompativel no nao", line_counter);
                        }

                    } else {

                    }
                } else {
                    printf("LINHA %d: [ERRO] Tipo1 incompativel", line_counter);
                }
                }

                if (strcmp(tipo1, "booleano") == 0) {

                    //aqui tem que retornar
                }
            }
        }
    }

    if (strcmp(stack[top], "inteiro") != 0 && strcmp(stack[top], "booleano") != 0) {

        Identificador *encontrado = buscarIdentificador(tabelaSimbolos, stack[top]);
        if (encontrado != NULL) {
            strcpy(stack[top], encontrado->tipo);
        } else {

            char aux[50] = {0};
            strcpy(aux, stack[top]);
            if (isdigit(aux[0])) {

                strcpy(stack[top], "inteiro");
            } else {
                if (strcmp(aux, "verdadeiro") == 0 || strcmp(aux, "falso") == 0) {

                    strcpy(stack[top], "booleano");
                } else {
                    printf("Identificador irregular na postfix");
                    exit(0);
                }
            }
        }
    }

    static char answ[50];
    strcpy(answ, stack[top]);
    //printf("answ: %s", answ);
    return answ;
}

void gera_expressao(char lista_posfix[50][50]) {
    // VER SER É IDENTIFICADOR E PROCURAR NA TABELE E ACHAR O ENDEREÇO DELE
    // eh um operando, letra ou numero

    char memoria_str[5] = {0};
    char auxiliar_str[5];
    for(int i=0; i < len_lista_postfix; i ++){

        if (strcmp(lista_posfix[i], "+") == 0) {
        //gera ADD
        Gera("    ", "ADD     ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "-") == 0) {
        //gera SUB
        Gera("    ", "SUB     ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "*") == 0) {
        //gera MULT
        Gera("    ", "MULT    ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "div") == 0) {
        //gera DIVI
        Gera("    ", "DIVI    ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "nao") == 0) {
        //gera NEG
        Gera("    ", "NEG     ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "e") == 0) {
        //gera AND
        Gera("    ", "AND     ", "    ", "    ");

    }

    if (strcmp(lista_posfix[i], "ou") == 0) {
        //gera OR
        Gera("    ", "OR      ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "<") == 0) {
        //gera CME
        Gera("    ", "CME     ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], ">") == 0) {
        //gera CMA
        Gera("    ", "CMA     ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "=") == 0) {
        //gera CEQ
        Gera("    ", "CEQ     ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "!=") == 0) {
        //gera CDIF
        Gera("    ", "CDIF    ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], "<=") == 0) {
        //gera CMEQ
        Gera("    ", "CMEQ    ", "    ", "    ");

    }


    if (strcmp(lista_posfix[i], ">=") == 0) {
        //gera CMAQ
        Gera("    ", "CMAQ    ", "    ", "    ");

    }

    if (strcmp(lista_posfix[i], "-u") == 0) {
        //gera INV
        Gera("    ", "INV     ", "    ", "    ");

    }

        if (isalnum(lista_posfix[i][0]) && (strcmp(lista_posfix[i], "ou") != 0 && strcmp(lista_posfix[i], "e") != 0 && strcmp(lista_posfix[i], "nao") != 0 && strcmp(lista_posfix[i], "+u") != 0 && strcmp(lista_posfix[i], "-u") != 0 && strcmp(lista_posfix[i], "div") != 0))
        {
            if(isalpha(lista_posfix[i][0])){

                if(strcmp(lista_posfix[i], "verdadeiro") == 0 || strcmp(lista_posfix[i], "falso") == 0){

                    if(strcmp(lista_posfix[i], "verdadeiro") == 0) {
                        Gera("    ", "LDC     ", "1   ", "    ");

                    }else if(strcmp(lista_posfix[i], "falso") == 0){
                        Gera("    ", "LDC     ", "0   ", "    ");
                    }

                }else{

                    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, lista_posfix[i]);

                    if(encontrado == NULL){
                        printf("[gera_LDC_LDV] - Indentificador nao encontrado");
                        exit(0);
                    }

                    if((strcmp(encontrado->tipo, "funcao inteiro") == 0 || strcmp(encontrado->tipo, "funcao booleano") == 0 || strcmp (encontrado->tipo, "procedimento") == 0)){

                    } else {


                    snprintf(memoria_str, sizeof(memoria_str), "%s", encontrado->memoria);
                    Gera("    ", "LDV     ", memoria_str, "    ");
                    }
                }
            }

            if(isdigit(lista_posfix[i][0])){

                snprintf(memoria_str, sizeof(memoria_str), "%s", lista_posfix[i]);
                Gera("    ", "LDC     ", memoria_str, "    ");
            }

        }


    }

}

char tratarEspacoComentario()
{
    while ((ch == '{' || isspace(ch) || ch == '\t' || ch == '\n') && ch != EOF)
    {
        if (ch == ' ')
        {
            ch = fgetc(fptr);
        }
        else if (ch == '\n')
        {
            line_counter++;
            ch = fgetc(fptr);
        }
        else if (ch == '{')
        {
            int comentario_counter = 1;
            int line_holder = line_counter;
            while (comentario_counter != 0)
            {
                ch = fgetc(fptr);
                if (ch == '{')
                {
                    comentario_counter++;
                }
                if (ch == '}')
                {
                    comentario_counter--;
                }
                if (ch == '\n')
                {
                    line_counter++;
                }
                if (ch == EOF)
                {
                    printf("LINHA %d: [ERRO] Feche o comentario", line_holder);
                    return EOF;
                }
            }
            ch = fgetc(fptr);
        }
        else if (ch == '\t')
        {
            ch = fgetc(fptr);
        }
    }
    return ch; // Retorna o próximo caractere após o tratamento
}

void TratarDigito()
{
    char simbolo[50] = {0};
    char numero[50] = {0};
    int index = 0;
    while (isdigit(ch))
    {
        numero[index] = ch;
        index++;
        ch = fgetc(fptr);
    }
    strcpy(simbolo, "snumero");
    strcpy(token.lexema, numero);
    strcpy(token.simbolo, simbolo);
    return;
}

void TratarIdentificador_PalavraReservada()
{
    char buffer[50] = {0};
    char simbolo[50];
    char lexema[50];
    int counter = 0;

    while (isdigit(ch) || isalpha(ch) || ch == '_' && counter < 50)
    {
        buffer[counter++] = ch;
        ch = fgetc(fptr);
    }
    buffer[counter] = '\0'; // Termina a string com o caractere nulo
    strcpy(lexema, buffer);

    if (strcmp(buffer, "programa") == 0)
    {
        strcpy(simbolo, "sprograma");
    }
    else if (strcmp(buffer, "se") == 0)
    {
        strcpy(simbolo, "sse");
    }
    else if (strcmp(buffer, "entao") == 0)
    {
        strcpy(simbolo, "sentao");
    }
    else if (strcmp(buffer, "senao") == 0)
    {
        strcpy(simbolo, "ssenao");
    }
    else if (strcmp(buffer, "enquanto") == 0)
    {
        strcpy(simbolo, "senquanto");
    }
    else if (strcmp(buffer, "faca") == 0)
    {
        strcpy(simbolo, "sfaca");
    }
    else if (strcmp(buffer, "inicio") == 0)
    {
        strcpy(simbolo, "sinicio");
    }
    else if (strcmp(buffer, "fim") == 0)
    {
        strcpy(simbolo, "sfim");
    }
    else if (strcmp(buffer, "escreva") == 0)
    {
        strcpy(simbolo, "sescreva");
    }
    else if (strcmp(buffer, "leia") == 0)
    {
        strcpy(simbolo, "sleia");
    }
    else if (strcmp(buffer, "var") == 0)
    {
        strcpy(simbolo, "svar");
    }
    else if (strcmp(buffer, "inteiro") == 0)
    {
        strcpy(simbolo, "sinteiro");
    }
    else if (strcmp(buffer, "booleano") == 0)
    {
        strcpy(simbolo, "sbooleano");
    }
    else if (strcmp(buffer, "verdadeiro") == 0)
    {
        strcpy(simbolo, "sverdadeiro");
    }
    else if (strcmp(buffer, "falso") == 0)
    {
        strcpy(simbolo, "sfalso");
    }
    else if (strcmp(buffer, "procedimento") == 0)
    {
        strcpy(simbolo, "sprocedimento");
    }
    else if (strcmp(buffer, "funcao") == 0)
    {
        strcpy(simbolo, "sfuncao");
    }
    else if (strcmp(buffer, "div") == 0)
    {
        strcpy(simbolo, "sdiv");
    }
    else if (strcmp(buffer, "e") == 0)
    {
        strcpy(simbolo, "se");
    }
    else if (strcmp(buffer, "ou") == 0)
    {
        strcpy(simbolo, "sou");
    }
    else if (strcmp(buffer, "nao") == 0)
    {
        strcpy(simbolo, "snao");
    }
    else
    {
        strcpy(simbolo, "sidentificador");
    }

    strcpy(token.lexema, lexema);
    strcpy(token.simbolo, simbolo);

    return;
}

void TrataAtribuicao()
{
    char buffer[50] = {0};
    char simbolo[50];
    char lexema[50];
    int counter = 0;

    buffer[counter] = ch; // coloca o = na primeira posicao do buffer
    ch = fgetc(fptr);     // le mais um caractere para ver se encontra o =

    if (ch == '=')
    { // encontrou o =, concatenar o montar o token
        buffer[++counter] = ch;
        strcpy(lexema, buffer);         // copia o conteudo do buffer para o lexema
        strcpy(simbolo, "satribuicao"); // copia "satribuicao" para o simbolo
        ch = fgetc(fptr);               // le mais um para deixar o proximo caractere pronto
    }
    else
    {                                   // nao achou o =, montar o token somente
        strcpy(lexema, buffer);         // copia o conteudo do buffer para o lexema
        strcpy(simbolo, "sdoispontos"); // copia "sdoispontos" para o simbolo
    }

    strcpy(token.lexema, lexema);
    strcpy(token.simbolo, simbolo);

    return;
}

void TratarOperadorRelacional()
{
    //!=, <, <=, >, >=, =
    char simbolo_opr[10];
    char lexema_opr[5] = {0};
    char temp_next_char;

    lexema_opr[0] = ch;

    switch (ch)
    {
    case '!':
        ch = fgetc(fptr);
        if (ch == '=')
        {
            strcpy(simbolo_opr, "sdif");
            lexema_opr[1] = '=';
            strcpy(token.lexema, lexema_opr);
            strcpy(token.simbolo, simbolo_opr);
            ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        }
        else
        {
            printf("\n\nTOKEN INVALIDO");
            exit(0);
        }
        break;

    case '<':
        ch = fgetc(fptr);
        if (ch == '=')
        {
            strcpy(simbolo_opr, "smenorig");
            lexema_opr[1] = '=';
            strcpy(token.lexema, lexema_opr);
            strcpy(token.simbolo, simbolo_opr);
            ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        }
        else
        {
            strcpy(simbolo_opr, "smenor");
            strcpy(token.lexema, lexema_opr);
            strcpy(token.simbolo, simbolo_opr);
        }
        break;

    case '>':
        ch = fgetc(fptr);
        if (ch == '=')
        {
            strcpy(simbolo_opr, "smaiorig");
            lexema_opr[1] = '=';
            strcpy(token.lexema, lexema_opr);
            strcpy(token.simbolo, simbolo_opr);
            ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        }
        else
        {
            strcpy(simbolo_opr, "smaior");
            strcpy(token.lexema, lexema_opr);
            strcpy(token.simbolo, simbolo_opr);
        }
        break;

    case '=':
        strcpy(simbolo_opr, "sig");
        lexema_opr[1] = '\0';
        strcpy(token.lexema, lexema_opr);
        strcpy(token.simbolo, simbolo_opr);
        ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        break;

    default:
        printf("\n\nERRO NA LEITURA DOS OPERADORES RELACIONAIS");
        exit(0);
    }

    return;
}

void TrataPontuacao()
{
    char simbolo_pont[50];
    char lexema_pont[5] = {0};

    lexema_pont[0] = ch;

    switch (ch)
    {
    case ';':
        strcpy(simbolo_pont, "sponto_virgula");
        strcpy(token.lexema, lexema_pont);
        strcpy(token.simbolo, simbolo_pont);
        ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        break;

    case ',':
        strcpy(simbolo_pont, "svirgula");
        strcpy(token.lexema, lexema_pont);
        strcpy(token.simbolo, simbolo_pont);
        ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        break;

    case '(':
        strcpy(simbolo_pont, "sabre_parenteses");
        strcpy(token.lexema, lexema_pont);
        strcpy(token.simbolo, simbolo_pont);
        ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        break;

    case ')':
        strcpy(simbolo_pont, "sfecha_parenteses");
        strcpy(token.lexema, lexema_pont);
        strcpy(token.simbolo, simbolo_pont);
        ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        break;

    case '.':
        strcpy(simbolo_pont, "sponto");
        strcpy(token.lexema, lexema_pont);
        strcpy(token.simbolo, simbolo_pont);
        ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto
        break;
    }

    return;
}

void trataOperadorAritmetico()
{
    char simbolo_arit[7];
    char lexema_arit[5] = {0};

    lexema_arit[0] = ch;
    switch (ch)
    {
    case '+':
        strcpy(simbolo_arit, "smais");
        break;

    case '-':
        strcpy(simbolo_arit, "smenos");
        break;

    case '*':
        strcpy(simbolo_arit, "smult");
        break;

    default:
        printf("\n\nERRO AO ATRIBUIR SIMBOLO");
    }

    ch = fgetc(fptr); // le mais um para deixar o proximo caractere pronto

    strcpy(token.lexema, lexema_arit);
    strcpy(token.simbolo, simbolo_arit);

    return;
}

void AnalisadorLexical()
{
    // Token* token = (Token*)malloc(sizeof(Token));  // Aloca memória para o token

    tratarEspacoComentario();

    if (ch == EOF)
        return;

    // TRATA DIGITO
    if (isdigit(ch))
    {
        TratarDigito();
    }

    // TRATA IDENTIFICADOR PALAVRA RESERVADA
    else if (isalpha(ch))
    {
        TratarIdentificador_PalavraReservada();
    }

    // TRATA ATRIBUICAO
    else if (ch == ':')
    {
        TrataAtribuicao();
    }
    // TRATA OPERADOR ARITMETICO
    else if (ch == '+' || ch == '-' || ch == '*')
    {
        trataOperadorAritmetico();
    }

    // TRATA OPERADOR RELACIONAL
    else if (ch == '!' || ch == '<' || ch == '>' || ch == '=')
    {
        TratarOperadorRelacional();
    }

    // TRATA PONTUACAO
    else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '.')
    {
        TrataPontuacao();
    }
    else
    {
        printf("LINHA %d: [ERRO] Caracter invalido: %c", ch, line_counter);
        exit(0);
    }

    return;
}

// Acima são os comentarios do analisador lexical

// Abaixo os procedimentos do sintatioc

void analisa_chamada_procedimento(char identificador[50])
{

    // if (strcmp(token.simbolo, "sponto_virgula") != 0)
    // {
    //     printf("ERRO! [ Analisa_chamada_procedimento ]- diferente de ponto e virgula linha:%d", line_counter);
    // }

    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, identificador);
    if (encontrado == NULL)
    {
        // nao encontrou, da erro
        printf("LINHA %d: [ERRO] procedimento nao declarado", line_counter);
        exit(0);
    }

    //GERA CALL
    char char_rotulo[5];
    snprintf(char_rotulo, sizeof(char_rotulo), "%-4s", encontrado->memoria);
    Gera("    ", "CALL    ", char_rotulo, "    ");
}

void analisa_chamada_funcao()
{
    // AnalisadorLexical();
    if (strcmp(token.simbolo, "sidentificador") == 0)
    {

        Identificador *encontrado = buscarIdentificador(tabelaSimbolos, token.lexema);
        if (encontrado == NULL)
        {
            // nao encontrou, da erro
            printf("LINHA %d: [ERRO] funcao nao declarado", line_counter);
            exit(0);
        }

        //GERA CALL

        char char_rotulo[5];
        snprintf(char_rotulo, sizeof(char_rotulo), "%-4s", encontrado->memoria);
        Gera("    ", "CALL    ", char_rotulo, "    ");
        Gera("    ", "LDV     ", "0   ", "    ");

        AnalisadorLexical();
    }
    else
    {
        printf("LINHA %d: [ERRO] diferente de identificador %s", line_counter, token.lexema);
        exit(0);
    }
}

void analisa_escreva()
{
    // Feito
    AnalisadorLexical();

    if (strcmp(token.simbolo, "sabre_parenteses") == 0)
    {
        AnalisadorLexical();
        if (strcmp(token.simbolo, "sidentificador") == 0)
        {
            Identificador *encontrado = buscarIdentificador(tabelaSimbolos, token.lexema);
            if (encontrado == NULL)
            {
                // nao encontrou, da erro
                printf("LINHA %d: [ERRO] Identificador nao declarado", line_counter);
                exit(0);
            }

            if (strcmp(encontrado->tipo, "inteiro") != 0) { //compara se nao eh penis
                printf("LINHA %d: [ERRO] Tipo diferente de inteiro", line_counter);
                exit(0);
            }
            AnalisadorLexical();
            if (strcmp(token.simbolo, "sfecha_parenteses") == 0)
            {
                AnalisadorLexical(); //
            }
            else
            {
                printf("LINHA %d: [ERRO] Faltou fecha parenteses", line_counter);
                exit(0);
            }

            char encontrado_end[4] = {0};
            snprintf(encontrado_end, sizeof(encontrado_end), "%s", encontrado->memoria);
            Gera("    ", "LDV     ", encontrado_end, "    ");
            Gera("    ", "PRN     ", "    ", "    ");
        }
        else
        {
            printf("LINHA %d: [ERRO] Faltou identificador", line_counter);
            exit(0);
        }
    }
    else
    {
        printf("LINHA %d: [ERRO] Faltou abre parenteses", line_counter);
        exit(0);
    }

}

void analisa_leia()
{

    // Feito
    AnalisadorLexical();
    if (strcmp(token.simbolo, "sabre_parenteses") == 0)
    {

        AnalisadorLexical();
        if (strcmp(token.simbolo, "sidentificador") == 0)
        {
            Identificador *encontrado = buscarIdentificador(tabelaSimbolos, token.lexema);
            if (encontrado == NULL)
            {
                // nao encontrou, da erro
                printf("LINHA %d: [ERRO] Identificador nao declarado", line_counter);
                exit(0);
            }

            if (strcmp(encontrado->tipo, "inteiro") != 0) { //compara se nao eh penis
                printf("LINHA %d: [ERRO] Tipo diferente de inteiro", line_counter);
                exit(0);
            }
            AnalisadorLexical();
            if (strcmp(token.simbolo, "sfecha_parenteses") == 0)
            {
                AnalisadorLexical();
            }
            else
            {
                printf("LINHA %d: [ERRO] Diferente de fecha parenteses", line_counter);
                exit(0);
            }

            Gera("    ", "RD      ", "    ", "    ");

            char encontrado_end[4];
            snprintf(encontrado_end, sizeof(encontrado_end), "%s", encontrado->memoria);
            Gera("    ", "STR     ", encontrado_end, "    ");

        }
        else
        {
            printf("LINHA %d: [ERRO] Diferente indentificador", line_counter);
            exit(0);
        }
    }
    else
    {
        printf("LINHA %d: [ERRO] Diferente de abre parenteses", line_counter);
        exit(0);
    }




}

void analisa_atribuicao(char identificador[50])
{

    Identificador *encontrado = buscarIdentificador(tabelaSimbolos, identificador);
    if (encontrado == NULL) {
        printf("LINHA %d: [ERRO] Identificador nao declarado", line_counter);
        exit(0);
    }
    // CONSIDERANDO QUE QUANDO CHEGAR AQUI EU TENHO O TOKEN :=
    AnalisadorLexical();

    analisa_expressao();

    if (strcmp(encontrado->tipo, "inteiro") == 0 || strcmp(encontrado->tipo, "funcao inteiro") == 0) { //se for igual
        if (strcmp(resp, "inteiro") == 0 || strcmp(resp, "funcao inteiro") == 0){
            //ok
        } else {
            printf("LINHA %d: [ERRO] 1 - Nao pode atribuir tipos diferentes", line_counter);
            exit(0);
        }
    }

    if (strcmp(encontrado->tipo, "booleano") == 0 || strcmp(encontrado->tipo, "funcao booleano") == 0) {
        if (strcmp(resp, "booleano") == 0 || strcmp(resp, "funcao booleano") == 0) {
            //ok
        } else {
            printf("LINHA %d: [ERRO] 2 - Nao pode atribuir tipos diferentes", line_counter);
            exit(0);
        }
    }

    if (strcmp(encontrado->tipo, "funcao inteiro") == 0 || strcmp(encontrado->tipo, "funcao booleano") == 0) {
        Gera("    ", "STR     ", "0   ", "    ");

    } else {


    char memoria_str[5];  // 4 caracteres + '\0'

    // Converte encontrado->memoria para uma string de 4 caracteres
    snprintf(memoria_str, sizeof(memoria_str), "%s", encontrado->memoria);

    Gera("    ", "STR     ", encontrado->memoria, "    ");

    }
}

void analisa_atrib_chprocedimento()
{
    // Feito
    // CONSIDERANDO QUE TENHO UM SIDENTIFICADOR NO TOKEN
    char auxiliar[50];
    strcpy(auxiliar, token.lexema);

    AnalisadorLexical();
    if (strcmp(token.simbolo, "satribuicao") == 0)
    {
        analisa_atribuicao(auxiliar);
    }
    else
    {
        analisa_chamada_procedimento(auxiliar);
    }
}

void analisa_comando_simples()
{
    // Feito
    if (strcmp(token.simbolo, "sidentificador") == 0)
    {
        // if (buscaAteMarcaPrimeiraOcorrencia(&tabelaSimbolos, token.lexema) == 0)
        // {
        //     printf("Erro [analisa comando simples] : identificador nao declarado na linha %d -> [%s]", line_counter, token.lexema);
        //     exit(0);
        // }

        analisa_atrib_chprocedimento();
    }

    else if (strcmp(token.simbolo, "sse") == 0)
    {
        analisa_se();
    }
    else if (strcmp(token.simbolo, "senquanto") == 0)
    {
        analisa_enquanto();
    }

    else if (strcmp(token.simbolo, "sleia") == 0)
    {
        analisa_leia();
    }

    else if (strcmp(token.simbolo, "sescreva") == 0)
    {
        analisa_escreva();
    }
    else
    {
        analisa_comandos();
    }
}

void analisa_comandos()
{
    // Feito
    if (strcmp(token.simbolo, "sinicio") == 0)
    {
        AnalisadorLexical();
        analisa_comando_simples();
        while (strcmp(token.simbolo, "sfim") != 0)
        {
            Sleep(2);
            if (strcmp(token.simbolo, "sponto_virgula") == 0)
            {
                AnalisadorLexical();
                if (strcmp(token.simbolo, "sfim") != 0)
                {
                    analisa_comando_simples();
                }
            }
            else
            {
                printf("LINHA %d: [ERRO] analisa comandos Esperava ponto e virgula", line_counter);
                exit(0);
            }
        }

        AnalisadorLexical();
    }
    else
    {
        printf("LINHA %d: [ERRO] esperava sinicio", line_counter);
        exit(0);
    }
}

void analisa_tipo()
{
    // Feito
    if ((strcmp(token.simbolo, "sinteiro") != 0) && (strcmp(token.simbolo, "sbooleano") != 0))
    {
        printf("LINHA %d: [ERRO] tipo invalido", line_counter);
        exit(0);
    }
    else
    {

        coloca_tipo(&tabelaSimbolos, token.lexema);

        // Identificador* encontrado = buscarIdentificador(tabelaSimbolos, token.lexema);
    }

    AnalisadorLexical();
}

void analisa_variaveis()
{
    int count = 0;
    // Feito
    do
    {
        if (strcmp(token.simbolo, "sidentificador") == 0)
        {
            count++;
            // Identificador* encontrado = buscarIdentificador(tabelaSimbolos, token.lexema);
            if (buscaAteMarcaPrimeiraOcorrencia(&tabelaSimbolos, token.lexema) == 0)
            {
                char end_string[30] = {0};
                char aloca_var_string[20] = {0};
                sprintf(end_string, "%d", end_disp_momento);
                sprintf(aloca_var_string, "%d", aloca_var);


                inserirIdentificador(&tabelaSimbolos, token.lexema, "", "variavel", aloca_var_string);
                aloca_var++;
                AnalisadorLexical();
                if (strcmp(token.simbolo, "svirgula") == 0 || strcmp(token.simbolo, "sdoispontos") == 0)
                {

                    if (strcmp(token.simbolo, "svirgula") == 0)
                    {
                        AnalisadorLexical();

                        if (strcmp(token.simbolo, "sdoispontos") == 0)
                        {
                            printf("LINHA %d: [ERRO] diferente de dois pontos", line_counter);
                            exit(0);
                        }
                    }
                }
                else
                {
                    printf("LINHA %d: [ERRO] diferente de dois pontos e virgula", line_counter);
                    exit(0);
                }
            }
            else
            {
                printf("ERRO SEMANTICO!: [Analisa_variaveis] - encontrou um identificador que não devia ");
                exit(0);
            }
        }
        else
        {
            printf("LINHA %d: [ERRO] diferente de identificador - TOKEN: %s", line_counter, token.simbolo);
            exit(0);
        }

    } while (strcmp(token.simbolo, "sdoispontos") != 0);

    char end_string[5]; // 4 caracteres + '\0'
    char alocado[5];    // 4 caracteres + '\0'

    // Formata end_string e alocado com largura fixa de 4 caracteres
    snprintf(end_string, sizeof(end_string), "%-4d", end_disp_momento);
    snprintf(alocado, sizeof(alocado), "%-4d", count);

    Gera("    ", "ALLOC   ", end_string, alocado);

    end_disp_momento += count;

    AnalisadorLexical();
    analisa_tipo();
}

void analisa_et_variaveis()
{
    // FEITO
    if (strcmp(token.simbolo, "svar") == 0)
    {

        AnalisadorLexical();
        if (strcmp(token.simbolo, "sidentificador") == 0)
        {

            while (strcmp(token.simbolo, "sidentificador") == 0)
            {
                analisa_variaveis();
                if (strcmp(token.simbolo, "sponto_virgula") == 0)
                {
                    AnalisadorLexical();
                }
                else
                {
                    printf("LINHA %d: [ERRO] diferente de ponto e virgula", line_counter);
                    exit(0);
                }
            }
        }
        else
        {
            printf("LINHA %d: [ERRO] diferente de indetificador", line_counter);
            exit(0);
        }
    }
}

void analisa_bloco()
{
    // Feito
    AnalisadorLexical();
    analisa_et_variaveis();
    analisa_subrotinas();
    analisa_comandos();
}

void analisa_enquanto()
{
    // ISSO AQUI FAZ PARTE
    //  DA GERACAO DE CODIGO !!!!!
    int auxiliar_rotulo_1, auxiliar_rotulo_2;

    // Variaveis que pegar o inteiro que viram strings
    char rotulo_str[5];
    char rotulo_str2[5];
    char auxiliar1_str[5];
    char auxiliar2_str[5];

    // Transforma rotulo 1 em string
    auxiliar_rotulo_1 = ROTULO;
    snprintf(rotulo_str, sizeof(rotulo_str), "%-4d", ROTULO);
    Gera(rotulo_str, "NULL    ", "    ", "    ");

    ROTULO = ROTULO + 1;


    // Feito
    AnalisadorLexical();
    // aqui tem o v
    analisa_expressao();

    if (strcmp(resp, "booleano") != 0) {
        printf("LINHA %d: [ERRO] tipo incompativel", line_counter);
        exit(0);
    }
    if (strcmp(token.simbolo, "sfaca") == 0)
    {
        auxiliar_rotulo_2 = ROTULO;
        snprintf(rotulo_str2, sizeof(rotulo_str), "%-4d", ROTULO);
        Gera("    ", "JMPF    ", rotulo_str2, "    ");


        ROTULO = ROTULO + 1;
        AnalisadorLexical();
        analisa_comando_simples();

        snprintf(auxiliar1_str, sizeof(auxiliar1_str), "%-4d", auxiliar_rotulo_1);
        Gera("    ", "JMP    ", auxiliar1_str, "    ");

        snprintf(auxiliar2_str, sizeof(auxiliar2_str), "%-4d", auxiliar_rotulo_2);
        Gera(auxiliar2_str, "NULL    ", "    ", "    ");

    }
    else
    {
        printf("LINHA %d: [ERRO] Esperava simbolo FACA", line_counter);
        exit(0);
    }
}

void analisa_se()
{
    // Feito
    int auxiliar1, auxiliar2;
    char rotulo_str[5];

    AnalisadorLexical();
    analisa_expressao();

    if (strcmp(resp, "booleano") != 0) {
        printf("LINHA %d: [ERRO] tipo incompativel", line_counter);
        exit(0);
    }

    snprintf(rotulo_str, sizeof(rotulo_str), "%-4d", ROTULO);
    Gera("    ", "JMPF    ", rotulo_str, "    ");
        auxiliar1 = ROTULO;
        ROTULO++;

    if (strcmp(token.simbolo, "sentao") == 0)
    {

        AnalisadorLexical();
        analisa_comando_simples();

        if (strcmp(token.simbolo, "ssenao") == 0)
        {
            snprintf(rotulo_str, sizeof(rotulo_str), "%-4d", ROTULO);
            Gera("    ", "JMP     ", rotulo_str, "    ");

            auxiliar2 = ROTULO;
            ROTULO++;

            snprintf(rotulo_str, sizeof(rotulo_str), "%-4d", auxiliar1);
            Gera(rotulo_str, "NULL    ", "    ", "    ");

            auxiliar1 = auxiliar2;

            AnalisadorLexical();
            analisa_comando_simples();


         }

        snprintf(rotulo_str, sizeof(rotulo_str), "%-4d", auxiliar1);
        Gera(rotulo_str, "NULL    ", "    ", "    ");


    }else{
        printf("LINHA %d: [ERRO] esperava ENTAO", line_counter);
        exit(0);
    }
}

void analisa_subrotinas()
{
    int auxiliar_rotulo = ROTULO, flag = 0;
    char rotulo_str[5] = {0};


    // ESSA PARTE É GERAÇAO DE CODIGO !!!!!!!!!
    if((strcmp(token.simbolo, "sprocedimento") == 0) || (strcmp(token.simbolo, "sfuncao") == 0)){
        auxiliar_rotulo = ROTULO;

        snprintf(rotulo_str, sizeof(rotulo_str), "%-4d", ROTULO);
        Gera("    ", "JMP     ", rotulo_str, "    ");
        ROTULO++;

        flag = 1;
    }

    // Feito
    while ((strcmp(token.simbolo, "sfuncao") == 0) || (strcmp(token.simbolo, "sprocedimento") == 0))
    {
        if (strcmp(token.simbolo, "sfuncao") == 0)
        {
            analisa_declaracao_funcao();
        }
        else //if (strcmp(token.simbolo, "sprocedimento") == 0)
        {
            analisa_declaracao_procedimento();
        }

        if (strcmp(token.simbolo, "sponto_virgula") == 0)
        {
            AnalisadorLexical();
        }
        else
        {
            printf("LINHA %d: [ERRO] Esperava ponto e virgula", line_counter);
            exit(0);
        }
    }

    // ESSA PARTE É GERAÇAO DE CODIGO !!!!!!!!!
    if(flag == 1){

        snprintf(rotulo_str, sizeof(rotulo_str), "%-4d", auxiliar_rotulo);
        Gera(rotulo_str, "NULL    ", "    ", "    ");
    }



}

void analisa_declaracao_procedimento()
{
    char rotulo_str[5] = {0};
    // Feito
    AnalisadorLexical();
    char nivel[3] = "L";
    if (strcmp(token.simbolo, "sidentificador") == 0)
    {
        Identificador *encontrado = buscarIdentificador(tabelaSimbolos, token.lexema);
        if (encontrado != NULL)
        {
            // Encontrou
            printf("LINHA %d: [ERRO] Procedimento ja declarado", line_counter);
            exit(0);
        }

        // ESSA PARTE É GERAÇAO DE CODIGO !!!!!!!!!

        snprintf(rotulo_str, sizeof(rotulo_str), "%-4d", ROTULO);

        inserirIdentificador(&tabelaSimbolos, token.lexema, nivel, "procedimento", rotulo_str);

        Gera(rotulo_str, "NULL    ", "    ", "    ");

        ROTULO = ROTULO + 1;
        AnalisadorLexical();
        if (strcmp(token.simbolo, "sponto_virgula") == 0)
        {
            analisa_bloco();
        }
        else
        {
            printf("LINHA %d: [ERRO] esperava ponto e virgula", line_counter);
            exit(0);
        }
    }
    else
    {
        printf("LINHA %d: [ERRO] identificador", line_counter);
        //TODO: ERRO NÃO ESPECIFICADO DIREITO
        exit(0);
    }

    // DESEMPILHA
    desempilharAteMarca(&tabelaSimbolos);

    if (end_var == 0) {
        Gera("    ", "RETURN  ", "    ", "    ");
    } else {
        char quant_desaloc[5] = {0};
        char pos_desaloc[5] = {0};
        snprintf(quant_desaloc, sizeof(quant_desaloc), "%-4d", end_var);
        int result_pos_dalloc = end_disp_momento - end_var;
        snprintf(pos_desaloc, sizeof(pos_desaloc), "%-4d", result_pos_dalloc);
        Gera("    ", "DALLOC  ", pos_desaloc, quant_desaloc);
        end_disp_momento = result_pos_dalloc;
        Gera("    ", "RETURN  ", "    ", "    ");
    }
}

void analisa_declaracao_funcao()
{
    // Feito
    AnalisadorLexical();
    char nome_funcao[50];
    char nivel[3] = "L";

    if (strcmp(token.simbolo, "sidentificador") == 0)
    {
        Identificador *encontrado = buscarIdentificador(tabelaSimbolos, token.lexema);
        if (encontrado == NULL)
        {
            char rotulo [5] = {0};
            snprintf(rotulo, sizeof(rotulo), "%-4d", ROTULO);
            inserirIdentificador(&tabelaSimbolos, token.lexema, nivel, "", rotulo);
            Gera(rotulo, "NULL    ", "    ", "    ");
            ROTULO++;
            strcpy(nome_funcao, token.lexema);
            AnalisadorLexical();

            if (strcmp(token.simbolo, "sdoispontos") == 0)
            {
                AnalisadorLexical();
                if ((strcmp(token.simbolo, "sinteiro") == 0) || (strcmp(token.simbolo, "sbooleano") == 0))
                {
                    if (strcmp(token.simbolo, "sinteiro") == 0)
                    {
                        coloca_tipo_func(&tabelaSimbolos, nome_funcao, "funcao inteiro");
                    }
                    else
                    {
                        coloca_tipo_func(&tabelaSimbolos, nome_funcao, "funcao booleano");
                    }
                    AnalisadorLexical();
                    if (strcmp(token.simbolo, "sponto_virgula") == 0)
                    {
                        analisa_bloco();
                    }
                }
                else
                {
                    printf("LINHA %d: [ERRO] esperava tipo da funcao (inteiro ou booleano)", line_counter);
                    exit(0);
                }
            }
            else
            {
                printf("LINHA %d: [ERRO] esperava dois pontos", line_counter);
                exit(0);
            }
        }
        else
        {
            printf("LINHA %d: [ERRO] Identificador nao foi encontrado na tabela", line_counter);
            exit(0);
        }
    }
    else
    {
        printf("LINHA %d: [ERRO] esperava identificador", line_counter);
        exit(0);
    }
    desempilharAteMarca(&tabelaSimbolos);
    if (end_var == 0) {
        Gera("    ", "RETURN  ", "    ", "    ");
    } else {
        char quant_desaloc[5] = {0};
        char pos_desaloc[5] = {0};
        snprintf(quant_desaloc, sizeof(quant_desaloc), "%-4d", end_var);
        int result_pos_dalloc = end_disp_momento - end_var;
        snprintf(pos_desaloc, sizeof(pos_desaloc), "%-4d", result_pos_dalloc);
        Gera("    ", "DALLOC  ", pos_desaloc, quant_desaloc);
        end_disp_momento = result_pos_dalloc;
        Gera("    ", "RETURN  ", "    ", "    ");
    }

}

// da pra fazer
void analisa_expressao()
{
    // feito

    // colocar na lista
    //  addListaInFix(token.lexema);

    analisa_expressao_simples();
    if ((strcmp(token.simbolo, "smaior") == 0) || (strcmp(token.simbolo, "smaiorig") == 0) || (strcmp(token.simbolo, "sig") == 0) || (strcmp(token.simbolo, "smenor") == 0) || (strcmp(token.simbolo, "smenorig") == 0) || (strcmp(token.simbolo, "sdif") == 0))
    {
        addListaInFix(token.lexema);
        AnalisadorLexical();
        // addListaInFix(token.lexema);
        analisa_expressao_simples();
    }

    // se tirar nao vai funcionar com parenteses estejam avisados
    if (strcmp(token.simbolo, "sfecha_parenteses") != 0)
    {
        listarListaInfix();
        infixToPostfix(lista_infix);
        resp =  avaliarPostfix(lista_postfix_global);

        gera_expressao(lista_postfix_global);
        resetListaInfix();
    }
}

// da pra fazer
void analisa_expressao_simples()
{
    // feito
    if ((strcmp(token.simbolo, "smais") == 0) || (strcmp(token.simbolo, "smenos") == 0))
    {
        // sinal

        if (strcmp (token.simbolo, "smais") == 0) {
            addListaInFix("+u");
            AnalisadorLexical();
        } else {
            addListaInFix("-u");
            AnalisadorLexical();
        }
        // addListaInFix(token.lexema);
    }

    analisa_termo();

    while ((strcmp(token.simbolo, "smais") == 0) || (strcmp(token.simbolo, "smenos") == 0) || (strcmp(token.simbolo, "sou") == 0))
    {

        addListaInFix(token.lexema);
        AnalisadorLexical();

        // coloca na lista
        //  addListaInFix(token.lexema);
        analisa_termo();
    }
}

void analisa_termo()
{
    // feito
    analisa_fator();
    // FREITAS, SE VOCE ESTÁ LENDO ISSO SAIBA QUE EU QUASE PULEI DA PONTE NESSE DIA
    while (strcmp(token.simbolo, "smult") == 0 || strcmp(token.simbolo, "sdiv") == 0 || strcmp(token.simbolo, "se") == 0)
    {
        addListaInFix(token.lexema);
        AnalisadorLexical();
        // coloca na lista
        //  addListaInFix(token.lexema);
        analisa_fator();
    }
}

// falata analisa_chamada_funcao (desafio)
void analisa_fator()
{

    if (strcmp(token.simbolo, "sidentificador") == 0)
    {

        Identificador *encontrado = buscarIdentificador(tabelaSimbolos, token.lexema);
        if (encontrado != NULL)
        {
            // Faz o strcpy de token.lexema para encontrado->tipo
            if (strcmp(encontrado->tipo, "funcao inteiro") == 0 || strcmp(encontrado->tipo, "funcao booleana") == 0)
            {
                addListaInFix(token.lexema);
                analisa_chamada_funcao();
            }
            else
            {
                addListaInFix(token.lexema);
                AnalisadorLexical();
                // coloca na lista

                // printf("Token que chega aqui %s", token.lexema);
            }
        }
        else
        {
            printf("LINHA %d: [ERRO] identificador nao declarado \n", line_counter);
            exit(0);
        }
    }
    else if (strcmp(token.simbolo, "snumero") == 0)
    {
        addListaInFix(token.lexema);
        AnalisadorLexical();
    }
    else if (strcmp(token.simbolo, "snao") == 0)
    {
        addListaInFix(token.lexema);
        AnalisadorLexical();
        // coloca na lista
        analisa_fator();
    }
    else if (strcmp(token.simbolo, "sabre_parenteses") == 0)
    {
        addListaInFix(token.lexema);
        AnalisadorLexical();
        // coloca na lista
        analisa_expressao();

        if (strcmp(token.simbolo, "sfecha_parenteses") == 0)
        {
            addListaInFix(token.lexema);
            AnalisadorLexical();
            // coloca na lista
        }
        else
        {
            printf("\n\n ERRO -- Falta de fechar parenteses\n\n");
            exit(0);
        }
    }
    else if ((strcmp(token.lexema, "verdadeiro") == 0) || (strcmp(token.lexema, "falso") == 0))
    {
        addListaInFix(token.lexema);
        AnalisadorLexical();
        // coloca lista
    }
    else
    {
        printf("\n\n ERRO -- ");
    }
}

int main(int argc, char *argv[])
{

    // Cabeça da lista de identificadores (tabela de símbolos)

    // Exemplo de variáveis a serem inseridas
    int x = 10;
    float y = 5.5;

    // Inserir identificadores com escopos (nome do procedimento ou função)
    // inserirIdentificador(&tabelaSimbolos, "variavelX", "main", "int", &x);
    // inserirIdentificador(&tabelaSimbolos, "variavelY", "funcaoA", "float", &y);

    // Imprimir a tabela de símbolos
    // printf("Tabela de Símbolos:\n");
    // imprimirTabelaSimbolos(tabelaSimbolos);

    // Buscar um identificador na tabela
    // Identificador* encontrado = buscarIdentificador(tabelaSimbolos, "variavelX");
    // if (encontrado != NULL) {
    //     printf("\nIdentificador encontrado:\n");
    //     printf("Nome: %s, Escopo: %s, Tipo: %s, Endereço: %p\n",
    //            encontrado->nome, encontrado->escopo, encontrado->tipo, encontrado->memoria);
    // } else {
    //     printf("\nIdentificador não encontrado!\n");
    // }

    // Liberar a memória da tabela de símbolos
    // liberarTabelaSimbolos(tabelaSimbolos);

    // Abre o arquivo "new 1.txt" com permissão de leitura
    /*fptr = fopen("CARALHO.txt", "r");

    // Faz a verificação do arquivo de leirura
    if (NULL == fptr)
    {
        printf("file can't be opened \n");
        return EXIT_FAILURE;
    }
    */
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        return EXIT_FAILURE;
    }

    ch = fgetc(fptr);

    // Gera("Teste", NULL, "1", "2");

    // Lembre de while != EOF

    limparArquivo();


    AnalisadorLexical();

    if (strcmp(token.simbolo, "sprograma") == 0)
    {

        AnalisadorLexical();
        if (strcmp(token.simbolo, "sidentificador") == 0)
        {

            // Formata end_string e alocado com largura fixa de 4 caracteres
            char end_string[5] = {0};
            char alocado[5] = {0};
            int func = 1;
            snprintf(end_string, sizeof(end_string), "%-4d", end_disp_momento);
            snprintf(alocado, sizeof(alocado), "%-4d", func);

            Gera("    ", "START   ", "    ", "    ");

            Gera("    ", "ALLOC   ", end_string, alocado);

            end_disp_momento += func;


            inserirIdentificador(&tabelaSimbolos, token.lexema, "L", "tipo - nomeDoPrograma", "penisprograma");
            AnalisadorLexical();
            if (strcmp(token.simbolo, "sponto_virgula") == 0)
            {
                analisa_bloco();
                if (strcmp(token.simbolo, "sponto") == 0)
                {
                    desempilharAteMarca(&tabelaSimbolos);
                    char quant_desaloc[5] = {0};
                    char pos_desaloc[5] = {0};
                    snprintf(quant_desaloc, sizeof(quant_desaloc), "%-4d", end_var);
                    int result_pos_dalloc = end_disp_momento - end_var;
                    snprintf(pos_desaloc, sizeof(pos_desaloc), "%-4d", result_pos_dalloc);
                    Gera("    ", "DALLOC  ", pos_desaloc, quant_desaloc);
                    end_disp_momento = result_pos_dalloc;


                    snprintf(quant_desaloc, sizeof(quant_desaloc), "%-4d", func);
                    result_pos_dalloc = end_disp_momento - func;
                    snprintf(pos_desaloc, sizeof(pos_desaloc), "%-4d", result_pos_dalloc);

                    Gera("    ", "DALLOC  ", pos_desaloc, quant_desaloc);
                    end_disp_momento = result_pos_dalloc;



                    Gera("    ", "HLT     ", "    ", "    ");

                    AnalisadorLexical();

                    if (ch == EOF)
                    {
                        printf("Sucesso!");
                    }
                    else
                    {
                        printf("LINHA %d: [ERRO] Diferente de End of File.", line_counter);
                        exit(0);
                    }
                }
                else
                {
                    printf("LINHA %d: [ERRO] Diferente de Ponto.", line_counter);
                    exit(0);
                }
            }
            else
            {
                printf("LINHA %d: [ERRO] Diferente de Ponto e virgula.", line_counter);
                exit(0);
            }
        }
        else
        {
            printf("\nLINHA %d: [ERRO] Faltou identificador do programa", line_counter);
            exit(0);
        }
    }
    else
    {
        printf("\nLINHA %d: [ERRO] Esperava PROGRAMA", line_counter);
        exit(0);
    }

    liberarTabelaSimbolos(tabelaSimbolos);

    // printf("\nLista Infix: \n");
    // listarListaInfix();
    return 0;
}
