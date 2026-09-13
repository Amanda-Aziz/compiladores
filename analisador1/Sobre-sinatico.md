# Analisador Sintático — `sintatico.c`

Implementação em C de um **analisador sintático descendente recursivo** para uma
linguagem de expressões aritméticas simples, envolvendo identificadores,
números, `+`, `*`, `**` (potência) e parênteses.

O programa recebe a expressão já convertida em uma **sequência de tokens**
(não uma string de caracteres) e verifica se essa sequência pertence à
linguagem definida pela gramática abaixo.

---

## Gramática

```
<expr>     ::= <termo> '+' <expr> | <termo>
<termo>    ::= <fator> '*' <termo> | <fator>
<fator>    ::= <primario> '**' <fator> | <primario>
<primario> ::= IDENT | NUMERO | '(' <expr> ')'
```

Cada não-terminal da gramática foi traduzido para uma função C correspondente:

| Não-terminal | Função C     |
|---|---|
| `<expr>`     | `expr()`     |
| `<termo>`    | `termo()`    |
| `<fator>`    | `fator()`    |
| `<primario>` | `primario()` |

---

## Tokens reconhecidos

| Token       | Código |
|---|---|
| `IDENT`     | 1 |
| `NUMERO`    | 2 |
| `MAIS`      | 3 |
| `MULTI`     | 4 |
| `POTENCIA`  | 5 |
| `ABRE_PAR`  | 6 |
| `FECHA_PAR` | 7 |
| `FIM`       | 8 |

Esses valores são definidos como macros (`#define`) e usados tanto na
gramática quanto na montagem dos vetores de teste em `main()`.

---

## Variáveis globais

| Variável | Tipo | Papel |
|---|---|---|
| `simbolo_lido` | `int` | Guarda o **token atual** sendo analisado. Todas as funções da gramática consultam essa variável para decidir o que fazer. É atualizada a cada chamada de `obtenha_simbolo()`. |
| `tokens` | `int *` | Ponteiro para o **vetor de tokens de entrada** (a expressão a ser analisada, já convertida em códigos numéricos). Por ser um ponteiro, basta reatribuí-lo para trocar qual expressão está sendo analisada, sem copiar dados. |
| `posicao_token` | `int` | Índice / cursor indicando **em qual posição do vetor `tokens`** a leitura está. Avança a cada `obtenha_simbolo()`, exceto quando o token atual já é `FIM` (para não ultrapassar os limites do vetor). |
| `houve_erro` | `int` (booleano 0/1) | Flag que indica se **algum erro sintático já foi detectado** durante a análise atual. Ativada dentro de `erro()`. Consultada no início de `expr()`, `termo()` e `fator()` para interromper a recursão assim que um erro é encontrado, evitando processamento inválido ou mensagens de erro duplicadas. |

> **Por que uma flag e não `exit()`?**
> O pseudocódigo original do trabalho sugere que a função de erro encerraria a análise
> imediatamente. Optamos por usar uma variável de controle (`houve_erro`) em
> vez de `exit()` porque o programa roda **vários testes na mesma execução**
> (dentro de `main()`); com `exit()`, o processo inteiro seria encerrado no
> primeiro erro, impedindo que os testes seguintes rodassem. Com a flag, cada
> chamada a `ANALISADOR_SINTATICO()` reseta o estado e a análise atual é
> abortada sem afetar o restante do programa. Esse é o motivo do
> `#include <stdlib.h>` estar comentado — sem `exit()`, `malloc()` ou similar,
> ele não é necessário.

---

## Funções

### `void ANALISADOR_SINTATICO(void)`
Ponto de entrada da análise de uma expressão:
1. Reseta `posicao_token = 0` e `houve_erro = 0` (necessário porque essas
   variáveis são globais e persistiriam entre chamadas se não fossem
   reiniciadas).
2. Lê o primeiro token (`obtenha_simbolo()`).
3. Chama `expr()`, iniciando a descida recursiva pela gramática.
4. Se `houve_erro` foi ativada em algum ponto da recursão, apenas retorna
   (a mensagem de erro já foi impressa por `erro()`).
5. Caso contrário, verifica se o token restante é `FIM`. Se for, a expressão
   é válida; se sobrar qualquer outro token, é um erro (símbolos extras não
   esperados).

### `void obtenha_simbolo(void)`
Avança o "cursor" da leitura: atualiza `simbolo_lido` com
`tokens[posicao_token]` e incrementa `posicao_token`, exceto quando o token
atual já é `FIM` (para não ler além do vetor).

### `void erro(const char *mensagem)`
Reporta um erro sintático: imprime a mensagem recebida e ativa
`houve_erro = 1`, sinalizando para toda a cadeia de chamadas recursivas que a
análise deve ser interrompida.

### `void expr(void)`, `void termo(void)`, `void fator(void)`
Implementam a recursão à direita da gramática (`termo + expr`,
`fator * termo`, `primario ** fator`). Cada uma:
1. Verifica `houve_erro` logo no início — se já houve erro em uma chamada
   anterior, aborta (`return`) sem fazer nada.
2. Chama a função do nível abaixo (`fator()` chama `primario()`, etc.).
3. Verifica `houve_erro` novamente, pois o erro pode ter ocorrido dentro da
   chamada que acabou de ser feita.
4. Se o próximo token for o operador correspondente (`+`, `*`, `**`), consome
   o operador e chama a si mesma recursivamente (recursão à direita).

### `void primario(void)`
Caso base da gramática — não chama nenhuma outra função de expressão antes de
decidir o que fazer, por isso é a única que **não** verifica `houve_erro` no
início. Aceita:
- `IDENT` ou `NUMERO`: apenas consome o token.
- `(`: consome, chama `expr()` recursivamente (permitindo expressões
  aninhadas entre parênteses) e exige que o próximo token seja `)` — caso
  contrário, reporta `"Parenteses nao fechado."`.
- Qualquer outro token: reporta `"Primario invalido..."`.

### `void roda_teste(int numero, int tokens_teste[])`
Função auxiliar de teste: aponta a variável global `tokens` para o vetor
recebido e chama `ANALISADOR_SINTATICO()`, imprimindo o número do teste antes
do resultado.

### `int main(void)`
Monta 10 vetores de tokens representando diferentes expressões (6 válidas e 4
inválidas) e chama `roda_teste()` para cada uma, exercitando os principais
casos da gramática:

| Teste | Tokens | Esperado |
|---|---|---|
| 1 | `IDENT FIM` | Válida |
| 2 | `IDENT + NUMERO FIM` | Válida |
| 3 | `IDENT * NUMERO FIM` | Válida |
| 4 | `IDENT ** NUMERO FIM` | Válida |
| 5 | `( IDENT + NUMERO ) FIM` | Válida |
| 6 | `IDENT + NUMERO * IDENT FIM` | Válida |
| 7 | `IDENT + FIM` | Erro — falta o segundo operando |
| 8 | `( IDENT + NUMERO FIM` | Erro — parêntese não fechado |
| 9 | `IDENT * + NUMERO FIM` | Erro — operador `+` onde esperava um primário |
| 10 | `+ IDENT FIM` | Erro — expressão não pode começar com operador |

---

## Como compilar e executar

```bash
gcc sintatico.c -o sintatico
./sintatico
```

## Saída esperada

```
===============================
ANALISADOR SINTATICO
===============================

--- Teste 1 ---
Resultado: Expressao valida.
===============================
...
--- Teste 7 ---
Resultado: Erro sintatico: Primario invalido...

===============================
...
```
