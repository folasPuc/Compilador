// C program to read a file using fgetc()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char lexema[50];
    char simbolo[50];  // Supondo que o valor máximo de um token seja 50 caracteres
} Token;


typedef struct Node {
    Token token;
    struct Node* proximo;
} Node;


Node* criarNovoNo(const char* lexema, const char* simbolo) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    strcpy(novoNo->token.lexema, lexema);
    strcpy(novoNo->token.simbolo, simbolo);
    novoNo->proximo = NULL;
    return novoNo;
}


void adicionarNo(Node** cabeca, const char* lexema, const char* simbolo) {
    Node* novoNo = criarNovoNo(lexema, simbolo);
    if (*cabeca == NULL) {
        *cabeca = novoNo;
    } else {
        Node* temp = *cabeca;
        while (temp->proximo != NULL) {
            temp = temp->proximo;
        }
        temp->proximo = novoNo;
    }
}


int main(){
    FILE *fptr;
    Node* listaTokens = NULL;

    char ch;

    // Abre o arquivo "new 1.txt" com permissão de leitura
    fptr = fopen("new 1.txt", "r");

    if (NULL == fptr) {
        printf("file can't be opened \n");
          return EXIT_FAILURE;
    }

    printf("Conteudo do arquivo: \n");

    //Loop principal, caractere por caractere
    while ((ch = fgetc(fptr)) != EOF) {
        //Descarta comentários '{}' e espaço
        while(ch == '{' || (isspace(ch) && ch != EOF)){
            if(ch == '{'){
                while(ch != '}' && ch != EOF){
                    ch = fgetc(fptr);
                }
                ch = fgetc(fptr);
            }
            else if (isspace(ch) && ch != EOF){
                ch = fgetc(fptr);
            }
        }
        //Verifica se é END OF FILE (EOF)
        if(ch == EOF)
            return 0;
        //TRATA DIGITO
        if (isdigit(ch)){

        }
        //LETRA - TRATA IDENTIFICADOR E PALAVRA RESERVADA
        else if (isalpha(ch)){

        }
        //TRATA ATRIBUICAO
        else if (ch == ':'){

        }
        //TRATA OPERADOR ARITMETICO
        else if (ch == '+' || ch == '-' || ch == '*'){

        }
        //TRATA OPERADOR RELACIONAL
        else if (ch == '!' || ch == '<' || ch == '>' || ch == '='){

        }
        //TRATA PONTUACAO
        else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '.'){

        }
        //ERRO
        else {
            printf("ERRO");
        }
        printf("%c", ch);
    }

    // FECHA O ARQUIVO
    fclose(fptr);


    adicionarNo(&listaTokens, "teste1", "PONTUACAO");
    adicionarNo(&listaTokens, "teste2", "DIGITO");
    adicionarNo(&listaTokens, "teste3", "LETRA");

    // Imprime os tokens armazenados
    Node* temp = listaTokens;
    while(temp != NULL) {
        printf("\nLexema: %s, Simbolo: %s\n", temp->token.lexema, temp->token.simbolo);
        temp = temp->proximo;
    }
    return 0;
}
