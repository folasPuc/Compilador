// C program to read a file using fgetc()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>


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


int line_counter = 1;
FILE *fptr;
char ch;

typedef struct {
    char lexema[50];
    char simbolo[50];
} Token;

Token token;

char tratarEspacoComentario() {
    while ((ch == '{' || isspace(ch) || ch == '\t' || ch == '\n') && ch != EOF) {
        if (ch == ' ') {
            ch = fgetc(fptr);
        } else if (ch == '\n') {
            line_counter++;
            ch = fgetc(fptr);
        } else if (ch == '{') {
            int comentario_counter = 1;
            int line_holder = line_counter;
            while (comentario_counter != 0) {
                ch = fgetc(fptr);
                if (ch == '{') {
                    comentario_counter++;
                }
                if (ch == '}') {
                    comentario_counter--;
                }
                if (ch == '\n') {
                    line_counter++;
                }
                if (ch == EOF) {
                    printf(" Feche o comentario da linha: %d ", line_holder);
                    return EOF;
                }
            }
            ch = fgetc(fptr);
        } else if (ch == '\t') {
            ch = fgetc(fptr);
        }
    }
    return ch;  // Retorna o próximo caractere após o tratamento
}

void TratarDigito(){
    char simbolo[50] = {0};
    char numero[50] = {0};
    int index = 0;
    while(isdigit(ch)){
        numero[index] = ch;
        index++;
        ch = fgetc(fptr);
    }
    strcpy(simbolo, "snumero");
    strcpy(token.lexema, numero);
    strcpy(token.simbolo, simbolo);
    return;
}

void TratarIdentificador_PalavraReservada(){
    char buffer[50] = {0};
    char simbolo[50];
    char lexema[50];
    int counter = 0;

    while(isdigit(ch) || isalpha(ch) || ch == '_' && counter < 50){
        buffer[counter++] = ch;
        ch = fgetc(fptr);
    }
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

    strcpy(token.lexema, lexema);
    strcpy(token.simbolo, simbolo);

    return;
}

void TrataAtribuicao(){
    char buffer[50] = {0};
    char simbolo[50];
    char lexema[50];
    int counter = 0;

    buffer[counter] = ch; //coloca o = na primeira posicao do buffer
    ch = fgetc(fptr); //le mais um caractere para ver se encontra o =

    if (ch == '='){ //encontrou o =, concatenar o montar o token
        buffer[++counter] = ch;
        strcpy(lexema, buffer); //copia o conteudo do buffer para o lexema
        strcpy(simbolo, "satribuicao"); //copia "satribuicao" para o simbolo
        ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
    }
    else { //nao achou o =, montar o token somente
        strcpy(lexema, buffer); //copia o conteudo do buffer para o lexema
        strcpy(simbolo, "sdoispontos"); //copia "sdoispontos" para o simbolo
    }

    strcpy(token.lexema, lexema);
    strcpy(token.simbolo, simbolo);
            
    return;
}

void TratarOperadorRelacional(){
    //!=, <, <=, >, >=, =
    char simbolo_opr[10];
    char lexema_opr[5] = {0};
    char temp_next_char;

    lexema_opr[0] = ch;

    switch(ch){
        case '!':
            ch = fgetc(fptr);
            if(ch == '='){
                strcpy(simbolo_opr, "sdif");
                lexema_opr[1] = '=';
                strcpy(token.lexema, lexema_opr);
                strcpy(token.simbolo, simbolo_opr);
                ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            
            } else {
                printf("\n\nTOKEN INVALIDO");
            }
            break;

        case '<':
            ch = fgetc(fptr);
            if(ch == '='){
                strcpy(simbolo_opr, "smenorig");
                lexema_opr[1] = '=';
                strcpy(token.lexema, lexema_opr);
                strcpy(token.simbolo, simbolo_opr);
                ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            } else {
                strcpy(simbolo_opr, "smenor");
                strcpy(token.lexema, lexema_opr);
                strcpy(token.simbolo, simbolo_opr);
            }
            break;

        case '>':
            ch = fgetc(fptr);
            if(ch == '='){
                strcpy(simbolo_opr, "smaiorig");
                lexema_opr[1] = '=';
                strcpy(token.lexema, lexema_opr);
                strcpy(token.simbolo, simbolo_opr);
                ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            } else {
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
            ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            break;

        default:
            printf("\n\nERRO NA LEITURA DOS OPERADORES RELACIONAIS");
        }
    
    return;
}

void TrataPontuacao(){
    char simbolo_pont[50];
    char lexema_pont[5] = {0};

    lexema_pont[0] = ch;

    switch(ch){
        case ';':
            strcpy(simbolo_pont, "sponto_virgula");
            strcpy(token.lexema, lexema_pont);
            strcpy(token.simbolo, simbolo_pont);
            ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            break;

        case ',':
            strcpy(simbolo_pont, "svirgula");
            strcpy(token.lexema, lexema_pont);
            strcpy(token.simbolo, simbolo_pont);
            ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            break;

        case '(':
            strcpy(simbolo_pont, "sabre_parenteses");
            strcpy(token.lexema, lexema_pont);
            strcpy(token.simbolo, simbolo_pont);
            ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            break;

        case ')':
            strcpy(simbolo_pont, "sfecha_parenteses");
            strcpy(token.lexema, lexema_pont);
            strcpy(token.simbolo, simbolo_pont);
            ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            break;

        case '.':
            strcpy(simbolo_pont, "sponto");
            strcpy(token.lexema, lexema_pont);
            strcpy(token.simbolo, simbolo_pont);
            ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto
            break;
    }
    
    return;         
}

void trataOperadorAritmetico(){
    char simbolo_arit[7];
    char lexema_arit[5] = {0};
    
    lexema_arit[0] = ch;
    switch(ch){
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
    
    ch = fgetc(fptr); //le mais um para deixar o proximo caractere pronto

    strcpy(token.lexema, lexema_arit);
    strcpy(token.simbolo, simbolo_arit);
            
    return;
}

void AnalisadorLexical(){
    //Token* token = (Token*)malloc(sizeof(Token));  // Aloca memória para o token

    tratarEspacoComentario();

    if(ch == EOF)
        return;
        
    // TRATA DIGITO    
    if (isdigit(ch)){
        TratarDigito();

    }
        
    //TRATA IDENTIFICADOR PALAVRA RESERVADA
    else if (isalpha(ch)){
        TratarIdentificador_PalavraReservada();
    }

    //TRATA ATRIBUICAO
    else if (ch == ':'){
        TrataAtribuicao();
    }
        //TRATA OPERADOR ARITMETICO
    else if (ch == '+' || ch == '-' || ch == '*'){
        trataOperadorAritmetico();
    } 

    //TRATA OPERADOR RELACIONAL
    else if (ch == '!' || ch == '<' || ch == '>' || ch == '='){
        TratarOperadorRelacional();
    }

    //TRATA PONTUACAO
    else if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '.'){
        TrataPontuacao();
    } else {
        printf("\nCaracter invalido: %c\nLinha %d", ch, line_counter);
    }

    return;
}

// Acima são os comentarios do analisador lexical


// Abaixo os procedimentos do sintatioc


void analisa_chamada_procedimento(){
    if(strcmp(token.simbolo, "sidentificador") == 0){
        AnalisadorLexical();
    }else{
        printf("ERRO! [ Analisa_chamada_funcao ]- diferente de indentificador linha:%d", line_counter);
    }
}

void analisa_chamada_funcao(){
    AnalisadorLexical();
    if(strcmp(token.simbolo, "sidentificador") == 0){
        AnalisadorLexical();
    }else{
        printf("ERRO! [ Analisa_chamada_funcao ]- diferente de indentificador linha:%d", line_counter);
    }
}



void analisa_escreva(){
    //Feito
    AnalisadorLexical();
    
    if(strcmp(token.simbolo, "sabre_parenteses") == 0){
        AnalisadorLexical();
        if(strcmp(token.simbolo, "sidentificador") == 0){
            AnalisadorLexical();
            if(strcmp(token.simbolo, "sfecha_parenteses") == 0){
                AnalisadorLexical(); //
            }else{
                printf("ERRO! [ analisa_escreva] Faltou fecha parenteses na linha %d", line_counter);
            }
        }else{
        printf("ERRO! [ analisa_escreva] Faltou identificador na linha %d", line_counter);
        }
    }else{
        printf("ERRO! [ analisa_escreva] Faltou abre parenteses na linha %d", line_counter);
    }

}






void analisa_leia(){
    //Feito
    AnalisadorLexical();
    if(strcmp(token.simbolo, "sabre_parenteses") == 0){

        AnalisadorLexical();
        if(strcmp(token.simbolo, "sidentificador") == 0){
            
            AnalisadorLexical();
            if(strcmp(token.simbolo, "sfecha_parenteses") == 0){
                AnalisadorLexical();
            }else{
                printf("ERRO!: [ analisa_leia] Diferente de fecha parenteses  Linha:%d", line_counter);
            }
            
        }else{
           printf("ERRO!: [ analisa_leia ] Diferente indentificador  Linha:%d", line_counter); 
        }
        
    }else{
        printf("ERRO!: [ analisa_leia ] Diferente de abre parenteses  Linha:%d", line_counter);
    }
}

void analisa_atribuicao(){
    //CONSIDERANDO QUE QUANDO CHEGAR AQUI EU TENHO O TOKEN :=
    AnalisadorLexical();

    analisa_expressao();
    printf("[%s] $$--$$ [%s]", token.lexema, token.simbolo);
}


void analisa_atrib_chprocedimento(){
    //Feito
    //CONSIDERANDO QUE TENHO UM SIDENTIFICADOR NO TOKEN
    AnalisadorLexical();
    if(strcmp(token.simbolo, "satribuicao") == 0){
        analisa_atribuicao();
    }else{
        analisa_chamada_procedimento();
    }

}


void analisa_comando_simples(){
    //Feito
    if(strcmp(token.simbolo, "sidentificador") == 0){
        analisa_atrib_chprocedimento();
    }

    else if(strcmp(token.simbolo, "sse") == 0){
        analisa_se();
    }
    else if(strcmp(token.simbolo, "senquanto") == 0){
        analisa_enquanto();
    }

    else if(strcmp(token.simbolo, "sleia") == 0){
        analisa_leia();
    }

    else if(strcmp(token.simbolo, "sescreva") == 0){
        analisa_escreva();
    }
    else{
        analisa_comandos();
    }
    
}



void analisa_comandos(){
    //Feito
    if(strcmp(token.simbolo, "sinicio") == 0){ 
        AnalisadorLexical();
        analisa_comando_simples();
        printf("sadlksadlkasd");
        while (strcmp(token.simbolo, "sfim") != 0)
        {
            Sleep(2);
            printf("[%s] [%s] -- ", token.lexema, token.simbolo);
            if(strcmp(token.simbolo, "sponto_virgula") == 0){
                AnalisadorLexical();
                if(strcmp(token.simbolo, "sfim") != 0){
                    analisa_comando_simples();
                    printf("\n\n [%s]   ~~~   [%s]\n\n", token.lexema, token.simbolo);
                } 
            }else{
                printf("ERRO! [ analisa_comandos ] - analisa comandos Esperava ponto e virgula na linha %d", line_counter);
            }
        }
        
        AnalisadorLexical();
        
    } else {
        printf("ERRO! [ analisa_comandos ] esperava sinicio na linha %d", line_counter);
    }
}


void analisa_tipo(){
    //Feito
    if( (strcmp(token.simbolo, "sinteiro") != 0) && (strcmp(token.simbolo, "sbooleano") != 0)){
        printf("ERRO!: [ analisa_tipo ]  tipo invalido : %d", line_counter);
        return;
    }

    AnalisadorLexical();
}


void analisa_variaveis(){
    //Feito
    do{
        if(strcmp(token.simbolo, "sidentificador") == 0){
            AnalisadorLexical();
            if(strcmp(token.simbolo, "svirgula") == 0 || strcmp(token.simbolo, "sdoispontos") == 0){

                if(strcmp(token.simbolo, "svirgula") == 0){
                    AnalisadorLexical();

                    if(strcmp(token.simbolo, "sdoispontos") == 0){
                        printf("ERRO!: [ Analisa_variaveis ] - diferente de dois pontos - Linha:%d", line_counter);
                    }
                }
            }else{
                printf("ERRO!: [ Analisa_variaveis ] - diferente de dois pontos e virgula - Linha:%d", line_counter);
            }

        }else{
            Sleep(2);
            printf("ERRO!: [ Analisa_variaveis ] - diferente de indentificador - Linha:%d TOKEN: %s", line_counter, token.simbolo);
        }
        
    }while(strcmp(token.simbolo, "sdoispontos") != 0);

    AnalisadorLexical();
    analisa_tipo();
}


void analisa_et_variaveis(){
    //FEITO
    if(strcmp(token.simbolo, "svar") == 0){

        AnalisadorLexical();
        if(strcmp(token.simbolo, "sidentificador") == 0){

            while(strcmp(token.simbolo, "sidentificador") == 0){
                analisa_variaveis();
                if(strcmp(token.simbolo, "sponto_virgula") == 0){
                    AnalisadorLexical();
                }else{
                    printf("ERRO!: [ analisa_et_variaveis ] - diferente de ponto e virgula - Linha:%d", line_counter);
                }
            }
            
        }else{
            printf("ERRO!: [ Analisa_et_variaveis ]- diferente de indetificador - Linha:%d", line_counter);
        }
    }
}

void analisa_bloco(){
    //Feito
    AnalisadorLexical();
    analisa_et_variaveis();
    analisa_subrotinas();
    analisa_comandos();
}

void analisa_enquanto(){
    //Feito
    AnalisadorLexical();
    //aqui tem o v
    analisa_expressao();
    if(strcmp(token.simbolo, "sfaca") == 0){
        AnalisadorLexical();
        analisa_comando_simples();
    }else{
        printf("ERRO! [ analisa_enquanto ] Esperava simbolo FACA na linha %d", line_counter);
    }

}

void analisa_se(){
    //Feito
    AnalisadorLexical();
    analisa_expressao();
    if(strcmp(token.simbolo, "sentao") == 0){
        AnalisadorLexical();
        analisa_comando_simples();
        if(strcmp(token.simbolo, "ssenao") == 0){
            AnalisadorLexical();
            analisa_comando_simples();
        }
    }else{
        printf("ERRO! [ analisa_se ] esperava ENTAO na linha %d", line_counter);
    }

}

void analisa_subrotinas(){
    //Feito
    while ((strcmp(token.simbolo, "sfuncao") == 0) || (strcmp(token.simbolo, "sprocedimento") == 0))
    {
        if(strcmp(token.simbolo, "sfuncao") == 0){
            analisa_declaracao_funcao();
        }
        else if(strcmp(token.simbolo, "sprocedimento") == 0){
            analisa_declaracao_procedimento();
        }

        if(strcmp(token.simbolo, "sponto_virgula") == 0){
            AnalisadorLexical();
        }else{
            printf("ERRO! [ analisa_subrotinas ] Esperava ponto e virgula na linha %d", line_counter);
        }
    }
    
    
}

void analisa_declaracao_procedimento(){
    //Feito
    AnalisadorLexical();
    if(strcmp(token.simbolo, "sidentificador") == 0){
        AnalisadorLexical();
        if(strcmp(token.simbolo, "sponto_virgula") == 0){
            analisa_bloco();
        }else{
            printf("ERRO! [analisa_declaracao_procedimento] esperava ponto e virgula na linha %d", line_counter);
        }
    }else{
        printf("ERRO! [analisa_declaracao_procedimento] identificador na linha %d", line_counter);
    }
}

void analisa_declaracao_funcao(){
    //Feito
    AnalisadorLexical();
    if(strcmp(token.simbolo, "sidentificador") == 0){
        AnalisadorLexical();
        if(strcmp(token.simbolo, "sdoispontos") == 0){
            AnalisadorLexical();
            if( (strcmp(token.simbolo, "sinteiro") == 0) || (strcmp(token.simbolo, "sbooleano") == 0) ){
                AnalisadorLexical();
                if(strcmp(token.simbolo, "sponto_virgula") == 0){
                    analisa_bloco();
                }
            }else {
                printf("ERRO! [ analisa_declaracao_funcao ] esperava tipo da funcao (inteiro ou booleano) na linha %d", line_counter);
            }
        }else {
            printf("ERRO!  [ analisa_declaracao_funcao ] esperava dois pontos na linha %d", line_counter);
        }
    }else {
        printf("ERRO! [ analisa_declaracao_funcao ] esperava identificador na linha %d", line_counter);
    }
}



// da pra fazer 
void analisa_expressao(){
    //feito
    analisa_expressao_simples();
    if( (strcmp(token.simbolo, "smaior") == 0) || (strcmp(token.simbolo, "smaiorig") == 0) || (strcmp(token.simbolo, "sig") == 0) || (strcmp(token.simbolo, "smenor") == 0) || (strcmp(token.simbolo, "smenorig") == 0) || (strcmp(token.simbolo, "sdif") == 0)){
        AnalisadorLexical();
        analisa_expressao_simples();
    }
}



// da pra fazer
void analisa_expressao_simples(){
    //feito
    if((strcmp(token.simbolo, "smais") == 0) || (strcmp(token.simbolo, "smenos") == 0))
        AnalisadorLexical();

    analisa_termo();

    while((strcmp(token.simbolo, "smais") == 0) || (strcmp(token.simbolo, "smenos") == 0) || (strcmp(token.simbolo, "sou") == 0)){
        AnalisadorLexical();
        analisa_termo();
    }
}


void analisa_termo(){
    //feito
    analisa_fator();
    
    while(strcmp(token.simbolo, "smult") == 0 || strcmp(token.simbolo, "sdiv") == 0 || strcmp(token.simbolo, "se") == 0){
        AnalisadorLexical();
        analisa_fator();
    }
}


// falata analisa_chamada_funcao (desafio)
void analisa_fator(){

    if(strcmp(token.simbolo, "sidentificador") == 0){
        //troço azul
        AnalisadorLexical();
    } else if (strcmp(token.simbolo, "snumero") == 0){

        AnalisadorLexical();

    } else if (strcmp(token.simbolo, "snao") == 0){
        AnalisadorLexical();
        analisa_fator();

    } else if (strcmp(token.simbolo, "sabre_parenteses") == 0){
        AnalisadorLexical();
        analisa_expressao();
        
        if(strcmp(token.simbolo, "sfecha_parenteses") == 0){
            AnalisadorLexical();
        } else {
            printf("\n\n ERRO -- Falta de fechar parenteses\n\n");
        }

    } else if ((strcmp(token.lexema, "verdadeiro") == 0) || (strcmp(token.lexema, "falso") == 0)){
        AnalisadorLexical();

    } else {
        printf("\n\n ERRO -- ");
    }
}




int main(){
    
    
    // Abre o arquivo "new 1.txt" com permissão de leitura
    fptr = fopen("new 1.txt", "r");

    // Faz a verificação do arquivo de leirura
    if (NULL == fptr) {
        printf("file can't be opened \n");
        return EXIT_FAILURE;
    }
    ch = fgetc(fptr);

    //Lembre de while != EOF
 
    AnalisadorLexical();

    if(strcmp(token.simbolo, "sprograma") == 0){

        AnalisadorLexical();
        if (strcmp(token.simbolo, "sidentificador") == 0){
            AnalisadorLexical();
            if(strcmp(token.simbolo, "sponto_virgula") == 0){
                analisa_bloco();
                if(strcmp(token.simbolo, "sponto") == 0){
                    
                    if(ch == EOF){
                        printf("Sucesso!");
                    }else{
                        printf("ERRO!: Diferente de End of File.  Linha:%d", line_counter);
                    }


                }else {
                    printf("ERRO!: Diferente de Ponto.  Linha:%d", line_counter);
                }
                
                
            }else{
                printf("ERRO!: Diferente de Ponto e virgula.  Linha:%d", line_counter);
            }
                

        }else{
            printf("\nERRO! Faltou identificador do programa na linha %d", line_counter);
        }
        
    }else{
        printf("\nERRO! Esperava PROGRAMA na linha %d", line_counter);
    }
    
    return 0;
}