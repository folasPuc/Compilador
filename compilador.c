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
            printf("%c", ch);
        }
        //LETRA - TRATA IDENTIFICADOR E PALAVRA RESERVADA
        else if (isalpha(ch)){
            char buffer[50] = {0};
            char simbolo[50];
            char lexema[50];
            int counter = 0;
            while(isdigit(ch) || isalpha(ch) || ch == '_' && counter < 50){
                printf("%c", ch);
                buffer[counter++] = ch;
                ch = fgetc(fptr);
            }
            printf("[%c]", ch);
            buffer[counter] = '\0'; // Termina a string com o caractere nulo
            strcpy(lexema, buffer);


            if (strcmp(buffer, "programa") == 0){
                strcpy(simbolo, "sprograma");
            }
            else if (strcmp(buffer, "se") == 0){
                strcpy(simbolo, "sse");
            }
            else if (strcmp(buffer, "entao") == 0){
                strcpy(simbolo, "sentao");
            }
            else if (strcmp(buffer, "senao") == 0){
                strcpy(simbolo, "ssenao");
            }
            else if (strcmp(buffer, "enquanto") == 0){
                strcpy(simbolo, "senquanto");
            }
            else if (strcmp(buffer, "faca") == 0){
                strcpy(simbolo, "sfaca");
            }
            else if (strcmp(buffer, "inicio") == 0){
                strcpy(simbolo, "sinicio");
            }
            else if (strcmp(buffer, "fim") == 0){
                strcpy(simbolo, "sfim");
            }
            else if (strcmp(buffer, "escreva") == 0){
                strcpy(simbolo, "sescreva");
            }
            else if (strcmp(buffer, "leia") == 0){
                strcpy(simbolo, "sleia");
            }
            else if (strcmp(buffer, "var") == 0){
                strcpy(simbolo, "svar");
            }
            else if (strcmp(buffer, "inteiro") == 0){
                strcpy(simbolo, "sinteiro");
            }
            else if (strcmp(buffer, "booleano") == 0){
                strcpy(simbolo, "sbooleano");
            }
            else if (strcmp(buffer, "verdadeiro") == 0){
                strcpy(simbolo, "sverdadeiro");
            }
            else if (strcmp(buffer, "falso") == 0){
                strcpy(simbolo, "sfalso");
            }
            else if (strcmp(buffer, "procedimento") == 0){
                strcpy(simbolo, "sprocedimento");
            }
            else if (strcmp(buffer, "funcao") == 0){
                strcpy(simbolo, "sfuncao");
            }
            else if (strcmp(buffer, "div") == 0){
                strcpy(simbolo, "sdiv");
            }
            else if (strcmp(buffer, "e") == 0){
                strcpy(simbolo, "se");
            }
            else if (strcmp(buffer, "ou") == 0){
                strcpy(simbolo, "sou");
            }
            else if (strcmp(buffer, "nao") == 0){
                strcpy(simbolo, "snao");
            }
            else {
                strcpy(simbolo, "sidentificador");
            }

            adicionarNo(&listaTokens, lexema, simbolo);

            ungetc(ch, fptr); // Devolve o caractere lido que não pertence ao identificador


        }
        //TRATA ATRIBUICAO
        else if (ch == ':'){
            printf("\nATRIBUICAO: -%c-", ch);
        }
        //TRATA OPERADOR ARITMETICO
        else if (ch == '+' || ch == '-' || ch == '*'){
            printf("\nOPERADOR ARITMETICO: -%c-", ch);
            char simbolo_arit[7];
            char lexema_arit[5] = {0};

            lexema_arit[0] = ch;

            switch(ch){

                case '+':
                    strcpy(simbolo_arit, "smais");
                    adicionarNo(&listaTokens, lexema_arit, simbolo_arit);
                    break;

                case '-':
                    strcpy(simbolo_arit, "smenos");
                    adicionarNo(&listaTokens, lexema_arit, simbolo_arit);
                    break;

                case '*':
                    strcpy(simbolo_arit, "smult");
                    adicionarNo(&listaTokens, lexema_arit, simbolo_arit);
                    break;

                default:
                    printf("\n\nERRO AO ATRIBUIR SIMBOLO");
                    break;
            }

        }
        //TRATA OPERADOR RELACIONAL
        else if (ch == '!' || ch == '<' || ch == '>' || ch == '='){
            printf("\nOPERADOR RELACIONAL: -%c-", ch);
            //!=, <, <=, >, >=, =
            char simbolo_opr[10];
            char lexema_opr[5] = {0};
            char temp_next_char;

            lexema_opr[0] = ch;

            switch(ch){

                case '!':
                    temp_next_char = fgetc(fptr);
                    if(temp_next_char == '='){
                        strcpy(simbolo_opr, "sdif");
                        lexema_opr[1] = '=';
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                    } else {
                        ungetc(temp_next_char, fptr);
                        printf("\n\nTOKEN INVALIDO");
                    }
                    break;

                case '<':
                    temp_next_char = fgetc(fptr);
                    if(temp_next_char == '='){
                        strcpy(simbolo_opr, "smenorig");
                        lexema_opr[1] = '=';
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                    } else {
                        strcpy(simbolo_opr, "smenor");
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                        ungetc(temp_next_char, fptr);
                    }
                    break;

                case '>':
                    temp_next_char = fgetc(fptr);
                    if(temp_next_char == '='){
                        strcpy(simbolo_opr, "smaiorig");
                        lexema_opr[1] = '=';
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                    } else {
                        strcpy(simbolo_opr, "smaior");
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                        ungetc(temp_next_char, fptr);
                    }
                    break;

                case '=':
                    strcpy(simbolo_opr, "sig");
                    lexema_opr[1] = '\0';
                    adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                    break;

                default:
                    printf("\n\nERRO NA LEITURA DOS OPERADORES RELACIONAIS");
            }

        }
        //TRATA PONTUACAO
        else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '.'){
            printf("\nPONTUACAO: -%c-", ch);

            char simbolo_pont[20];
            char lexema_pont[5] = {0};

            lexema_pont[0] = ch;

            switch(ch){

                case ';':
                    strcpy(simbolo_pont, "sponto_virgula");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    break;

                case ',':
                    strcpy(simbolo_pont, "svirgula");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    break;

                case '(':
                    strcpy(simbolo_pont, "sabre_parenteses");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    break;

                case ')':
                    strcpy(simbolo_pont, "sfecha_parenteses");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    break;

                case '.':
                    strcpy(simbolo_pont, "sponto");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    break;
            }

        }
        //ERRO
        else {
            printf("ERRO");
        }
        // printf("%c", ch);
    }

    // FECHA O ARQUIVO
    fclose(fptr);


    // adicionarNo(&listaTokens, "teste1", "PONTUACAO");
    // adicionarNo(&listaTokens, "teste2", "DIGITO");
    // adicionarNo(&listaTokens, "teste3", "LETRA");

    // Imprime os tokens armazenados
    Node* temp = listaTokens;
    while(temp != NULL) {
        printf("\nLexema: %s, Simbolo: %s\n", temp->token.lexema, temp->token.simbolo);
        temp = temp->proximo;
    }
    return 0;
}
