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

// variaveis globais
char linha[300];
int simbolo_lido; // token
int *tokens; //vetor de tokens da entrada
int posicao_token;


void erro(const char *mensagem);
void obtenha_simbolo(void);

void expr(void);
void termo(void);
void fator(void);
void primario(void);

// ================================================================================

void obtenha_simbolo(void){
    simbolo_lido = tokens[posicao_token];
    if(tokens[posicao_token] != FIM){
        posicao_token++;
    }
}

void erro(const char *mensagem){
    printf("Erro sintatico: %s\n", mensagem);
    printf("\n===============================\n");
    exit(1); //indicando q teve erro na execução do cod
}


// ================================================================================ METODOS DAS EXPRESSOES

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
            erro("Parenteses nao fechado.");
        } else{
            obtenha_simbolo();
        }
    } else{
        erro("Primario invalido...");
    }
}

// ================================================================================

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

// ================================================================================

int main(){   // pelo q eu li nao recebe pelo teclado digitado pelo usuario, e sim ja definimos na main os testes saca?
                // falta isso eu acho, dps da uma bizoiada pfv
    
    printf("\n===============================\n");
    printf("ANALISADOR SINTATICO\n");
    printf("===============================\n");
    printf("\nDigite uma expressao: ");
    fgets(linha, sizeof(linha), stdin);
    printf("\n");

    ANALISADOR_SINTATICO();
    return 0;

}