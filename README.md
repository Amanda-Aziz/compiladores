# 🔍 Mini-Projetos: Analisadores Léxico e Sintático
> Status: ✅ Finalizado

Este repositório contém duas implementações em C relacionadas às etapas
iniciais de um compilador: **análise léxica** e **análise sintática**, para
uma linguagem simples de expressões aritméticas envolvendo identificadores,
números e os operadores `+`, `*`, `**` e parênteses.

## Gramática utilizada nos dois projetos

```
<expr>     ::= <termo> '+' <expr> | <termo>
<termo>    ::= <fator> '*' <termo> | <fator>
<fator>    ::= <primario> '**' <fator> | <primario>
<primario> ::= IDENT | NUMERO | '(' <expr> ')'
```

## 🎱 Projetos

### 1. `sintatico.c` — Analisador Sintático
Recebe a expressão **já convertida em uma sequência de tokens** (fixada no
código) e verifica se ela obedece à gramática, usando um analisador
descendente recursivo. Roda 10 casos de teste (válidos e inválidos) numa
única execução. Detalhes em [`README-sintatico.md`](https://github.com/Amanda-Aziz/compiladores/blob/main/analisador1/Sobre-sinatico.md).

### 2. `lexico.c` — Analisador Léxico + Sintático
Evolução do anterior: lê uma **expressão em texto digitada pelo usuário**,
faz o reconhecimento dos tokens (análise léxica) e em seguida valida a
estrutura da expressão com o mesmo analisador sintático. Detalhes em
[`README-lexico.md`](https://github.com/Amanda-Aziz/compiladores/blob/main/analisador2/Sobre-lexico.md).

## Relação entre os dois 🗯

`lexico.c` pode ser entendido como o `sintatico.c` "completo": em vez de
tokens prontos, ele adiciona a etapa de transformar texto em tokens antes de
validá-los, formando o pipeline clássico:

```
texto digitado → análise léxica (tokens) → análise sintática → expressão válida ou erro
```

## Como compilar ❓

```bash
gcc sintatico.c -o s && ./s
gcc lexico.c -o l && ./l
```
# Desenvolvedores 🔖

Dupla responsável pelo desenvolvimento do trabalho.

| [<img src="https://github.com/Amanda-Aziz.png" width="115"><br><sub>Amanda Aziz</sub>](https://github.com/Amanda-Aziz) | [<img src="https://github.com/Sofiafs.png" width="115"><br><sub>Sofia Farias</sub>](https://github.com/Sofiafs) |
| :---: | :---: |

---
