# SafeHouse em C — Central de Segurança Residencial

Programa de console em linguagem C que funciona como a **central do alarme** do SafeHouse. Os **registros** do sistema são os **sensores** da casa: o programa cadastra, consulta, exporta, exclui (exclusão lógica) e contabiliza esses registros, guardando tudo em **arquivo binário**. Além disso, controla o modo do alarme e simula invasões e emergências.

É a mesma lógica do simulador 3D (https://codestorm-dev.github.io/safehouse/), escrita em C.

**Grupo:** Luiz Gustavo Silva Prestes, Marcos Eduardo Martins da Silva, Gilmar Victor Gama Da Silva, Joao Gabryel Ferreira
**Professora:** Flavia

## As 5 funcionalidades obrigatórias

| # | Funcionalidade exigida | Opção do menu | Função no código |
|---|---|---|---|
| 1 | **Cadastrar registros:** código, nome, local e tipo do sensor, sem aceitar código repetido | 1 | `cadastrarSensor()` em `sensores.c` |
| 2 | **Consultar por código:** mostra a ficha completa do sensor (dados, estado, situação no modo atual, status e posição no arquivo) | 2 | `consultarPorCodigo()` em `sensores.c` |
| 3 | **Exportar para TXT:** grava todos os registros em `relatorio.txt` (ativos, excluídos, resumo por tipo e histórico) | 3 | `exportarTXT()` em `eventos.c` |
| 4 | **Excluir (exclusão lógica):** o registro **não sai** do arquivo binário; o campo `status` passa de ATIVO para EXCLUÍDO e ele deixa de aparecer nas listas | 4 | `excluirSensor()` em `sensores.c` |
| 5 | **Contabilizar registros:** o menu mostra sempre quantos registros estão ativos, quantos foram excluídos e quantos existem no arquivo | menu | `contarAtivos()` e `contarExcluidos()` em `sensores.c` |

### Funcionalidades extras

| Opção | O que faz |
|---|---|
| 5 | Listar todos os sensores ativos |
| 6 | Buscar sensores por tipo |
| 7 | Alterar sensor (ligar/desligar, nome, local, tipo) |
| 8 | Mudar o modo do alarme (Desarmado, Em casa, Ausente) |
| 9 | Simular uma detecção: a central ignora, só registra ou dispara e mostra a resposta automática (sirene, luzes, fechaduras, câmeras, polícia 190 ou bombeiros 193) |
| 10 e 11 | Ver ou apagar o histórico de eventos (arquivo binário `eventos.dat`) |

## Structs

```c
typedef struct {            /* um registro: um sensor da casa */
    int  codigo;
    char nome[50];
    char comodo[20];
    int  tipo;              /* porta/janela, movimento, vidro, câmera, barreira, fumaça, gás */
    int  ligado;            /* 1 = ligado, 0 = desligado */
    int  status;            /* 1 = ATIVO, 0 = EXCLUÍDO (exclusão lógica) */
} Sensor;

typedef struct {            /* a central inteira */
    Sensor lista[100];
    int    quantidade;      /* registros no arquivo (ativos + excluídos) */
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

## Exclusão lógica

```c
s->lista[pos].status = STATUS_EXCLUIDO;   /* não apaga, só marca */
salvarSistema(s);                          /* regrava o arquivo com o registro marcado */
```

- O registro continua em `sensores.dat` (o arquivo não perde nenhum registro).
- Listar, buscar, simular e consultar ignoram os registros com status EXCLUÍDO.
- Consultar o código de um sensor excluído avisa que ele foi excluído.
- O código de um sensor excluído não pode ser usado de novo, para não existirem dois registros com o mesmo código no arquivo.
- O TXT exportado mostra os excluídos numa seção separada.

## Arquivos

| Arquivo | Tipo | O que guarda | Funções usadas |
|---|---|---|---|
| `sensores.dat` | binário | modo do alarme, quantidade e todos os registros (ativos e excluídos) | `fwrite` / `fread` em `salvarSistema()` e `carregarSistema()` |
| `eventos.dat` | binário | histórico de eventos (um `Evento` atrás do outro) | `fwrite` com `"ab"` e `fread` em laço |
| `relatorio.txt` | texto | exportação legível de todos os registros | `fprintf` em `exportarTXT()` |

Esses arquivos são criados quando o programa roda e não vão para o GitHub. Na primeira execução, o programa já cadastra os 16 sensores da casa do projeto.

## Código-fonte

| Arquivo | Conteúdo |
|---|---|
| `main.c` | menu interativo, contagem de registros e laço principal |
| `sensores.h` / `sensores.c` | structs `Sensor` e `Sistema`; cadastrar, consultar, excluir, contar, listar, buscar, alterar; modo do alarme; arquivo `sensores.dat` |
| `eventos.h` / `eventos.c` | struct `Evento`; exportar TXT; simulação; histórico `eventos.dat` |
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
- código de sensor excluído → `[ERRO] O código X pertence a um sensor excluído`
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
