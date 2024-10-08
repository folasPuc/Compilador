// C program to read a file using fgetc()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int line_counter = 1;

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
    ch = fgetc(fptr);
    //Loop principal, caractere por caractere
    while (ch != EOF) {

        //Descarta comentários '{}' e espaço
        while((ch == '{' || (isspace(ch) || ch == '\t' || ch == '\n')) && ch != EOF){

            if(ch == ' ' && ch != EOF){
                ch = fgetc(fptr);
            }

            if(ch == '\n'){
                line_counter++;
                printf("\n\nLeitura da linha %d\n\n", line_counter);
                ch = fgetc(fptr);
            }

            if(ch == '{'){
                int comentario_counter = 1;
                int line_holder = line_counter; //segura a linha para indicar para o usuario no final
                while(comentario_counter != 0){
                    ch = fgetc(fptr);
                    if(ch == '{'){ //verifica se encontra a abertura de outro comentario sem fechar o primeiro
                        comentario_counter++;
                    }
                    if(ch == '}') {
                        comentario_counter--;
                    }
                    if(ch == '\n'){
                        line_counter++;
                        printf("\n\nLeitura da linha %d\n\n", line_counter);
                    }
                    if(ch == EOF){ //verifica se abriu o comentario e esqueceu de fechar ate o final do arquivo
                        printf(" Feche o comentario da linha: %d ", line_holder);
                        return 0;
                    }
                }

                ch = fgetc(fptr);

            }if(ch == '\t'){
                ch = fgetc(fptr);
            }

        }
        //Verifica se é END OF FILE (EOF)
        if(ch == EOF)
            break;

        //TRATA DIGITO
        if (isdigit(ch)){
            printf("%c", ch);
            char simbolo[50] = {0};
            char numero[50] = {0};
            int index = 0;
            while(isdigit(ch)){
                printf("%c", ch);
                numero[index] = ch;
                index++;
                ch = fgetc(fptr);
            }
            strcpy(simbolo, "snumero");
            adicionarNo(&listaTokens, numero, simbolo);

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
            //printf("[%c]", ch);
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

        }
        //TRATA ATRIBUICAO
        else if (ch == ':'){
            printf("%c", ch);
            char buffer[50] = {0};
            char simbolo[50];
            char lexema[50];
            int counter = 0;

            buffer[counter] = ch; //coloca o = na primeira posicao do buffer
            ch = fgetc(fptr); //le mais um caractere para ver se encontra o =

            if (ch == '='){ //encontrou o =, concatenar o montar o token
                printf("%c", ch);
                buffer[++counter] = ch;
                strcpy(lexema, buffer); //copia o conteudo do buffer para o lexema
                strcpy(simbolo, "satribuicao"); //copia "satribuicao" para o simbolo
                adicionarNo(&listaTokens, lexema, simbolo); //faz a chamada para adicionar um novo token na lista de tokens
                ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            }
            else { //nao achou o =, montar o token somente
                strcpy(lexema, buffer); //copia o conteudo do buffer para o lexema
                strcpy(simbolo, "sdoispontos"); //copia "sdoispontos" para o simbolo
                adicionarNo(&listaTokens, lexema, simbolo); //faz a chamada para adicionar um novo token na lista de tokens

            }


        }

        //TRATA OPERADOR ARITMETICO
        else if (ch == '+' || ch == '-' || ch == '*'){
            printf("%c", ch);
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
             ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto

        }
        //TRATA OPERADOR RELACIONAL
        else if (ch == '!' || ch == '<' || ch == '>' || ch == '='){
            printf("%c", ch);
            //!=, <, <=, >, >=, =
            char simbolo_opr[10];
            char lexema_opr[5] = {0};
            char temp_next_char;

            lexema_opr[0] = ch;

            switch(ch){

                case '!':
                    ch = fgetc(fptr);
                    if(ch == '='){
                        printf("%c", ch);
                        strcpy(simbolo_opr, "sdif");
                        lexema_opr[1] = '=';
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                        ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    } else {
                        printf("\n\nTOKEN INVALIDO");
                    }
                    break;

                case '<':
                    ch = fgetc(fptr);
                    if(ch == '='){
                        printf("%c", ch);
                        strcpy(simbolo_opr, "smenorig");
                        lexema_opr[1] = '=';
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                        ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    } else {
                        strcpy(simbolo_opr, "smenor");
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                    }
                    break;

                case '>':
                    ch = fgetc(fptr);
                    if(ch == '='){
                        printf("%c", ch);
                        strcpy(simbolo_opr, "smaiorig");
                        lexema_opr[1] = '=';
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                        ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    } else {
                        strcpy(simbolo_opr, "smaior");
                        adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                    }
                    break;

                case '=':
                    strcpy(simbolo_opr, "sig");
                    lexema_opr[1] = '\0';
                    adicionarNo(&listaTokens, lexema_opr, simbolo_opr);
                    ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    break;

                default:
                    printf("\n\nERRO NA LEITURA DOS OPERADORES RELACIONAIS");
            }

        }
        //TRATA PONTUACAO
        else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '.'){
            printf("%c", ch);

            char simbolo_pont[20];
            char lexema_pont[5] = {0};

            lexema_pont[0] = ch;

            switch(ch){

                case ';':
                    strcpy(simbolo_pont, "sponto_virgula");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    break;

                case ',':
                    strcpy(simbolo_pont, "svirgula");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    break;

                case '(':
                    strcpy(simbolo_pont, "sabre_parenteses");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    break;

                case ')':
                    strcpy(simbolo_pont, "sfecha_parenteses");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    break;

                case '.':
                    strcpy(simbolo_pont, "sponto");
                    adicionarNo(&listaTokens, lexema_pont, simbolo_pont);
                    ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
                    break;
            }

        }
        //ERRO
        else {
            printf("\nCaracter invalido: %c\nLinha %d", ch, line_counter);
            char simbolo[50] = {0};
            char lexema[50] = {0};

            strcpy(simbolo, "serro");
            lexema[0] = ch;
            adicionarNo(&listaTokens, lexema, simbolo);
            ch = fgetc(fptr);
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
