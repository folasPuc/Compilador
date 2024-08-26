// C program to read a file using fgetc()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(){
    FILE *fptr;

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
    return 0;
}
