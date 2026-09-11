#include <stdio.h>
#include <stdlib.h> 

#define IDENT 1
#define NUMERO 2
#define MAIS 3
#define MULTI 4
#define POTENCIA 5
#define ABRE_PAR 6
#define FECHA_PAR 7
#define FIM 8

int simbolo_lido; // simbolo lido não vai ler o caracter >>> vai ler o CÓDIGO TOKEN (MAIS, MULTI, POTENCIA...)
char linha[300]; //p guardar linha digitada
int posicao = 0;

void expr(void);
void termo(void);
void fator(void);
void primario(void);

void erro(const char *mensagem);
void obtenha_simbolo(void);

// ================================================================================ METODOS DAS EXPRESSOES

void ANALISADOR_SINTATICO(){
    obtenha_simbolo();
    expr();
    if(simbolo_lido == FIM){
        printf("Expressao valida.");
        printf("\n===============================\n");
    } else {
        erro("Erro na analise sintatica...");
    }
}

void expr(void){
    termo();
    if(simbolo_lido == MAIS){ 
        obtenha_simbolo();
        expr();
    }
}

void termo(void){
    fator();
    if(simbolo_lido == MULTI){ 
        obtenha_simbolo();
        termo();
    }
}

void fator(void){
    primario();
    if(simbolo_lido == POTENCIA){ 
        obtenha_simbolo();
        fator();
    }
}

void primario(void){
    if(simbolo_lido == IDENT){
        obtenha_simbolo();
    }
    else if(simbolo_lido == NUMERO){
        obtenha_simbolo();    
    }
    else if(simbolo_lido == ABRE_PAR){
        obtenha_simbolo();
        expr();
        if(simbolo_lido != FECHA_PAR){
            erro("ERRO! Parentese nao fechado.");
        } else{
            obtenha_simbolo();
        }
    } else{
        erro("Simbolo inesperado...");
    }
}

// ================================================================================ METODOS erro e obtenha simbolo

void erro(const char *mensagem){
    printf("Erro sintatico: %s\n", mensagem);
    printf("\n===============================\n");
    
    exit(1); //indicando q teve erro na execução do cod, o valor 1 é um cod de saída que indica falha
}

void obtenha_simbolo(void){
    
    //pular espaços em branco, primeiramente
    while(linha[posicao] == ' ' || linha[posicao] == '\t' || linha[posicao] == '\n'){
        posicao++; //avança p o proximo caractere
    }

    //identificar o fim
    if(linha[posicao] == '\0'){
        simbolo_lido = FIM;
        return;
    }

    // identificando operadores + parenteses
    if(linha[posicao] == '+'){
        simbolo_lido = MAIS;
        posicao++; // avançando para o prox caracter
        return;
    }
    if(linha[posicao] == '('){
        simbolo_lido = ABRE_PAR;
        posicao++;
        return;
    }
    if(linha[posicao] == ')'){
        simbolo_lido = FECHA_PAR;
        posicao++;
        return;
    }

    if(linha[posicao] == '*'){          // multi ou potencia
        posicao++;
        if(linha[posicao] == '*'){
            simbolo_lido = POTENCIA;
            posicao++;
        } else{
            simbolo_lido = MULTI;
        }
        return;
    }

    //reconhecer numeros
    if(linha[posicao] >= '0' && linha[posicao] <= '9'){
        while(linha[posicao] >= '0' && linha[posicao] <= '9'){
            posicao++;
        }
        simbolo_lido = NUMERO;
        return;
    }

    //reconhecer IDENT
    if((linha[posicao] >= 'a' && linha[posicao] <= 'z') || (linha[posicao] >= 'A' && linha[posicao] <= 'Z')){
        posicao++;

        while((linha[posicao] >= 'a' && linha[posicao] <= 'z') || (linha[posicao] >='A' && linha[posicao] <= 'Z') || (linha[posicao] >= '0' && linha[posicao] <= '9')){
            posicao++;
        }
        simbolo_lido = IDENT;
        return;        
    }   
    erro("Simbolo invalido na entrada");
}

// ================================================================================

int main(){
    
    printf("\n===============================\n");
    printf("ANALISADOR LEXICO E SINTATICO\n");
    printf("===============================\n");
    printf("\nDigite uma expressao: ");
    fgets(linha, sizeof(linha), stdin);
    printf("\n");

    ANALISADOR_SINTATICO();
    return 0;

}
