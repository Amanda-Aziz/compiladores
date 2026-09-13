#include <stdio.h>
// #include <stdlib.h>   //optamos por trocar a função de erro() pela variável houve_erro

#define IDENT 1
#define NUMERO 2
#define MAIS 3
#define MULTI 4
#define POTENCIA 5
#define ABRE_PAR 6
#define FECHA_PAR 7
#define FIM 8

// =========================================================== VARIÁVEIS GLOBAIS
int simbolo_lido; // guardar o token atual
int *tokens; //ponteiro para o vetor de tokens da entrada
int posicao_token;
int houve_erro;

void erro(const char *mensagem);
void obtenha_simbolo(void);

void expr(void);
void termo(void);
void fator(void);
void primario(void);
// ===========================================================

void ANALISADOR_SINTATICO(){
    posicao_token = 0;
    houve_erro = 0;
    obtenha_simbolo();
    expr();

    if(houve_erro){
        return;
    }

    if(simbolo_lido == FIM){
        printf("Expressao valida.");
        printf("\n===============================\n");
    } else {
        erro("Erro na analise sintatica...");
    }
}

void obtenha_simbolo(void){
    simbolo_lido = tokens[posicao_token];
    if(tokens[posicao_token] != FIM){
        posicao_token++;
    }
}

void erro(const char *mensagem){
    printf("Erro sintatico: %s\n", mensagem);
    printf("\n===============================\n");
    houve_erro = 1; 
}

// =========================================================== MÉTODOS DAS EXPRESSOES

void expr(void){
    if(houve_erro){
        return;
    }
    termo();
    if(houve_erro){
        return;
    }
    if(simbolo_lido == MAIS){
        obtenha_simbolo();
        expr();
    }
}

void termo(void){
    if(houve_erro){
        return;
    }
    fator();
    if(houve_erro){
        return;
    }
    if(simbolo_lido == MULTI){
        obtenha_simbolo();
        termo();
    }
}

void fator(void){
    if(houve_erro){
        return;
    }
    primario();
    if(houve_erro){
        return;
    }
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

// ===========================================================

void roda_teste(int numero, int tokens_teste[]){
    printf("\n--- Teste %d ---\n", numero);
    tokens = tokens_teste;
    printf("Resultado: ");
    ANALISADOR_SINTATICO();
}

int main(){
   
    printf("\n===============================\n");
    printf("ANALISADOR SINTATICO\n");
    printf("===============================\n");

    int teste1[] = {IDENT, FIM};
    int teste2[] = {IDENT, MAIS, NUMERO, FIM};
    int teste3[] = {IDENT, MULTI, NUMERO, FIM};
    int teste4[] = {IDENT, POTENCIA, NUMERO, FIM};
    int teste5[] = {ABRE_PAR, IDENT, MAIS, NUMERO, FECHA_PAR, FIM};
    int teste6[] = {IDENT, MAIS, NUMERO, MULTI, IDENT, FIM};
    int teste7[] = {IDENT, MAIS, FIM};
    int teste8[] = {ABRE_PAR, IDENT, MAIS, NUMERO, FIM};
    int teste9[] = {IDENT, MULTI, MAIS, NUMERO, FIM};
    int teste10[] = {MAIS, IDENT, FIM};

    roda_teste(1, teste1);
    roda_teste(2, teste2);
    roda_teste(3, teste3);
    roda_teste(4, teste4);
    roda_teste(5, teste5);
    roda_teste(6, teste6);
    roda_teste(7, teste7);
    roda_teste(8, teste8);
    roda_teste(9, teste9);
    roda_teste(10, teste10);
    
    return 0;
}