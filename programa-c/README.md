# SafeHouse em C — Central de Segurança Residencial

Programa de console em linguagem C que funciona como a **central do alarme** do SafeHouse. Ele cadastra os sensores da casa, controla o modo do alarme, simula invasões e emergências, guarda tudo em **arquivos binários** e gera um **relatório em TXT**.

É a mesma lógica do simulador 3D (https://codestorm-dev.github.io/safehouse/), escrita em C.

**Grupo:** Luiz Gustavo Silva Prestes, Marcos Eduardo Martins da Silva, Gilmar Victor Gama Da Silva, Joao Gabryel Ferreira
**Professora:** Flavia

## As 5 funcionalidades

| # | Funcionalidade | Opções do menu | Onde está no código |
|---|---|---|---|
| 1 | **Gerenciar sensores:** cadastrar, listar, buscar (por código ou tipo), alterar (ligar/desligar, nome, local, tipo) e excluir | 1 a 5 | `sensores.c` |
| 2 | **Modo do alarme:** Desarmado, Em casa ou Ausente, com a regra de quais sensores ficam armados | 6 | `mudarModo()` e `tipoArmado()` em `sensores.c` |
| 3 | **Simular detecção:** escolher o sensor que percebeu algo; a central decide se ignora, só registra ou dispara, e mostra a resposta automática (sirene, luzes, fechaduras, câmeras, polícia 190 ou bombeiros 193) | 7 | `simularDeteccao()` em `eventos.c` |
| 4 | **Histórico de eventos** em arquivo binário, com data e hora, e opção de apagar | 8 e 10 | `listarHistorico()` e `limparHistorico()` em `eventos.c` |
| 5 | **Relatório TXT** com resumo, sensores por tipo, lista de sensores, histórico e pontos de atenção | 9 | `gerarRelatorio()` em `eventos.c` |

## Structs

```c
typedef struct {            /* um sensor da casa */
    int  codigo;
    char nome[50];
    char comodo[20];
    int  tipo;              /* porta/janela, movimento, vidro, câmera, barreira, fumaça, gás */
    int  ligado;
} Sensor;

typedef struct {            /* a central inteira */
    Sensor lista[100];
    int    quantidade;
    int    modo;            /* Desarmado, Em casa ou Ausente */
} Sistema;

typedef struct {            /* um registro do histórico */
    int  id;
    char dataHora[20];
    int  codigoSensor;
    char nomeSensor[50];
    char comodo[20];
    int  tipo, modo, resultado;  /* resultado: ignorado, registrado ou disparo */
} Evento;
```

## Arquivos

| Arquivo | Tipo | O que guarda | Funções usadas |
|---|---|---|---|
| `sensores.dat` | binário | modo do alarme + lista de sensores | `fwrite` / `fread` em `salvarSistema()` e `carregarSistema()` |
| `eventos.dat` | binário | histórico de eventos (um `Evento` atrás do outro) | `fwrite` com `"ab"` e `fread` em laço |
| `relatorio.txt` | texto | relatório legível para abrir no Bloco de Notas | `fprintf` |

Esses três arquivos são criados quando o programa roda e não vão para o GitHub. Na primeira execução, o programa já cadastra os 16 sensores da casa do projeto.

## Código-fonte

| Arquivo | Conteúdo |
|---|---|
| `main.c` | menu principal e laço do programa |
| `sensores.h` / `sensores.c` | structs `Sensor` e `Sistema`, regras do alarme, cadastro e arquivo `sensores.dat` |
| `eventos.h` / `eventos.c` | struct `Evento`, simulação, histórico `eventos.dat` e relatório `relatorio.txt` |
| `util.h` / `util.c` | leitura segura do teclado (validações) e data/hora |
| `compilar.bat` | compila e abre o programa com dois cliques |
| `APRESENTACAO.md` | roteiro da apresentação, testes e divisão do grupo |

## Validações

Todas as leituras usam `fgets` (não sobra lixo no buffer) e são conferidas:

- texto onde se pede número → `[ERRO] "abc" não é um número`
- campo vazio → `[ERRO] Campo vazio` / `Campo obrigatório`
- número negativo → `[ERRO] Número negativo não é permitido`
- fora da faixa (menu, tipo, local) → `[ERRO] Digite um número entre X e Y`
- código repetido → `[ERRO] Já existe um sensor com o código X`
- código que não existe → aviso, sem travar
- exclusão pede confirmação S/N
- arquivo que não abre ou está corrompido → mensagem de erro, sem fechar o programa

## Como compilar e rodar

**Jeito fácil (Windows):** dê dois cliques em `compilar.bat`.

**Pelo terminal:**

```bash
gcc -Wall -Wextra -std=c99 -o safehouse main.c sensores.c eventos.c util.c
./safehouse
```

Compila **sem nenhum aviso** com `-Wall -Wextra` (testado no gcc 4.9.2 do Dev-C++).

**No Dev-C++:** Arquivo › Novo › Projeto › Console Application (C), adicione os 4 arquivos `.c` e os 3 `.h` ao projeto e aperte F11.
