# Analisador Léxico e Sintático — `lexico.c`

Evolução do `sintatico.c`: em vez de receber a expressão já pronta como um
vetor de tokens fixo no código, este programa lê uma **expressão em texto
digitada pelo usuário** e faz o **próprio reconhecimento dos tokens**
(análise léxica) antes de validar a estrutura da expressão (análise
sintática).

Ou seja, este arquivo junta duas etapas de um compilador na mesma execução:

1. **Análise léxica** — transforma caracteres (`'a'`, `'+'`, `'1'`, `'2'`...)
   em tokens (`IDENT`, `MAIS`, `NUMERO`...).
2. **Análise sintática** — verifica se a sequência de tokens gerada obedece à
   gramática da linguagem (a mesma gramática do `sintatico.c`).

---

## Gramática

```
<expr>     ::= <termo> '+' <expr> | <termo>
<termo>    ::= <fator> '*' <termo> | <fator>
<fator>    ::= <primario> '**' <fator> | <primario>
<primario> ::= IDENT | NUMERO | '(' <expr> ')'
```

Igual ao `sintatico.c`: cada não-terminal virou uma função C
(`expr()`, `termo()`, `fator()`, `primario()`).

## Tokens reconhecidos

| Token       | Código | Reconhecido a partir de |
|---|---|---|
| `IDENT`     | 1 | Letra seguida de letras/dígitos (ex: `x`, `abc1`) |
| `NUMERO`    | 2 | Sequência de dígitos (ex: `123`) |
| `MAIS`      | 3 | `+` |
| `MULTI`     | 4 | `*` |
| `POTENCIA`  | 5 | `**` |
| `ABRE_PAR`  | 6 | `(` |
| `FECHA_PAR` | 7 | `)` |
| `FIM`       | 8 | Fim da string (`'\0'`) |

---

## Variáveis globais

| Variável | Tipo | Papel |
|---|---|---|
| `simbolo_lido` | `int` | Guarda o **token atual** identificado pelo analisador léxico. É o que `expr()`, `termo()`, `fator()` e `primario()` consultam para decidir o que fazer — igual no `sintatico.c`. |
| `linha` | `char[300]` | Buffer que armazena a **string digitada pelo usuário** (a expressão em texto bruto, ex: `"a + 1 * b\n"`). Preenchida por `fgets()` em `main()`. |
| `posicao` | `int` | Índice / cursor indicando **em qual caractere de `linha`** a leitura léxica está. Avança conforme os caracteres vão sendo consumidos por `obtenha_simbolo()`. |

### Diferença importante em relação ao `sintatico.c`

No `sintatico.c`, a entrada já vinha pronta como um vetor de **tokens**
(`int *tokens`) e `posicao_token` andava de token em token. Aqui, a entrada é
uma **string de caracteres** (`char linha[300]`) e `posicao` anda de
**caractere em caractere** — é o próprio `obtenha_simbolo()` quem agrupa
vários caracteres (ex: `"123"` ou `"abc"`) em um único token.

Também não existe mais a flag `houve_erro`: as funções `erro_sintatico()` e
`erro_lexico()` chamam `exit(1)` diretamente, encerrando o programa assim que
um erro é detectado. Isso é possível (e mais simples) aqui porque o programa
roda **uma única expressão por execução** (não há bateria de testes dentro do
mesmo `main()` como no `sintatico.c`), então não há necessidade de manter o
processo vivo após um erro.

---

## Funções

### `void ANALISADOR_SINTATICO(void)`
Ponto de entrada: lê o primeiro token (`obtenha_simbolo()`), chama `expr()`
e, ao final, verifica se o token restante é `FIM`. Se for, imprime
`"Expressao valida."`; caso contrário, chama `erro_sintatico()`.

### `void obtenha_simbolo(void)`
**O analisador léxico propriamente dito.** Lê a partir de `linha[posicao]`
e decide qual token está ali, avançando `posicao` conforme consome
caracteres:

1. **Pula espaços em branco** (`' '`, `'\t'`, `'\n'`) — é assim que o `\n`
   deixado pelo `fgets()` no fim da string é tratado, sem causar erro.
2. **Fim da string** (`'\0'`) → `FIM`.
3. **Operadores/parênteses de um único caractere** (`+`, `(`, `)`) → token
   correspondente, avança um caractere.
4. **`*` ou `**`** → lê o `*`, espia o próximo caractere: se for outro `*`,
   é `POTENCIA` (avança mais um); senão, é `MULTI`.
5. **Números** → enquanto o caractere for dígito (`'0'`–`'9'`), avança;
   token vira `NUMERO`.
6. **Identificadores** → se começar com letra, avança enquanto for letra ou
   dígito (permite `abc123`, mas não um identificador começando com
   número); token vira `IDENT`.
7. **Qualquer outro caractere** (ex: `@`, `#`, `$`) → chama `erro_lexico()`.

### `void erro_sintatico(const char *mensagem)`
Imprime a mensagem de erro sintático e encerra o programa com `exit(1)`.

### `void erro_lexico(char caractere)`
Imprime qual caractere inválido foi encontrado e encerra o programa com
`exit(1)`.

### `void expr(void)`, `void termo(void)`, `void fator(void)`, `void primario(void)`
Implementação idêntica em estrutura ao `sintatico.c` (mesma gramática), com
uma diferença: como os erros agora usam `exit(1)` em vez de uma flag, essas
funções **não precisam mais checar `houve_erro`** antes de continuar — o
programa simplesmente encerra sozinho assim que `erro_sintatico()` ou
`erro_lexico()` são chamadas, então o código ficou mais enxuto que no
`sintatico.c`.

### `int main(void)`
1. Imprime o cabeçalho do programa.
2. Pede para o usuário digitar uma expressão (`fgets(linha, sizeof(linha),
   stdin)`).
3. Chama `ANALISADOR_SINTATICO()`, que dispara toda a análise léxica e
   sintática sobre o que foi digitado.

---

## Como compilar e executar

```bash
gcc lexico.c -o l
./l
```

Depois é só digitar a expressão quando solicitado, por exemplo:

```
Digite uma expressao: a + 1 * b
```

## Exemplos de uso

| Entrada digitada | Resultado |
|---|---|
| `abc + 123 * xyz` | `Expressao valida.` |
| `(a + 1) * b2` | `Expressao valida.` |
| `x ** 2 ** 3` | `Expressao valida.` (potência associa à direita) |
| `a + @` | `Erro lexico: caractere especial '@'` |
| `a +` | `Erro sintatico: Simbolo inesperado...` |
| `(a + 1` | `Erro sintatico: ERRO! Parentese nao fechado.` |
