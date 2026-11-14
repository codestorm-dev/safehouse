# Apresentação — SafeHouse em C

Entrega: link do repositório no Google Classroom, **enviado por cada integrante, até 22/11/2026**.
Apresentação prática: **23/11/2026**.

## Onde cada critério é atendido

| Critério | Pontos | Onde mostrar |
|---|---|---|
| 5 funcionalidades obrigatórias | 0,8 | 1 Cadastrar · 2 Consultar por código · 3 Exportar TXT · 4 Excluir (exclusão lógica) · contagem de registros no topo do menu |
| Structs, funções e arquivos binários/TXT | 0,4 | Structs `Sensor` (com campo `status`), `Sistema` e `Evento`; `sensores.dat` e `eventos.dat` com `fwrite`/`fread`; `relatorio.txt` com `fprintf` |
| Organização e boas práticas | 0,3 | Código em módulos (`.c` + `.h`), funções curtas e comentadas, validação de todas as entradas, compila sem avisos com `-Wall -Wextra` |
| Demonstração prática | 0,3 | Roteiro abaixo |
| Domínio e participação | 0,2 | Divisão do grupo e perguntas abaixo |

## Roteiro da demonstração (6 a 8 minutos)

Antes de começar, apague `sensores.dat`, `eventos.dat` e `relatorio.txt` da pasta para mostrar o programa do zero.

1. **Compilar e abrir (Luiz, 1 min):**
   - rodar `gcc -Wall -Wextra -std=c99 -o safehouse main.c sensores.c eventos.c util.c` e mostrar que não aparece nenhum aviso;
   - abrir o programa: ele avisa "Primeira execução: 16 sensores";
   - mostrar no topo do menu: **Registros ativos: 16 | excluídos: 0 | no arquivo: 16** (funcionalidade 5).
2. **Cadastrar, com erros tratados (Marcos, 1 min 30 s):** opção **1**.
   - No código, digitar `abc` (erro: não é número), depois `-5` (erro: negativo) e depois `1` (erro: código repetido). Por fim, `20`.
   - No nome, apertar ENTER vazio (erro: campo obrigatório) e depois digitar `Sensor da varanda`.
   - No tipo, digitar `9` (erro: fora da faixa) e depois `1`. Local: `4` (Sala).
   - Mostrar que o menu passou para **ativos: 17**.
3. **Consultar por código (Marcos, 40 s):** opção **2**, código **20**: aparece a ficha completa. Depois, código **777**: aviso de que não existe.
4. **Exclusão lógica (Joao, 1 min 30 s):** opção **4**, código **20**, responder `x` (erro: só S ou N) e depois `S`.
   - Mostrar o menu: **ativos: 16 | excluídos: 1 | no arquivo: 17**. O registro continua no arquivo.
   - Opção **2**, código **20**: o programa avisa que ele foi excluído e continua no arquivo binário.
   - Opção **1**, tentar cadastrar o código **20** de novo: erro, porque o código pertence a um registro excluído. Digitar **21** e completar: nome `Portao lateral`, tipo **1**, local **7**. O menu fica **ativos: 17 | excluídos: 1 | no arquivo: 18**.
5. **Modo e simulação (Gilmar, 1 min 30 s):** opção **8**, escolher **2** (Em casa). Depois, opção **9**:
   - sensor **6** (movimento da sala) → só registra (no modo Em casa o movimento não está armado);
   - sensor **12** (barreira do muro) → ALARME DISPARADO, com sirene, luzes, fechaduras, câmeras e polícia 190;
   - sensor **16** (gás) → dispara em qualquer modo, fecha a válvula e chama os bombeiros 193.
6. **Exportar TXT e persistência (Joao, 1 min 30 s):**
   - opção **3**: exporta; abrir `relatorio.txt` no Bloco de Notas e mostrar os sensores ativos e a seção "REGISTROS EXCLUIDOS";
   - opção **10**: histórico de eventos vindo do `eventos.dat`;
   - opção **0**: sair. Abrir o programa de novo: o menu continua **17 ativos | 1 excluído | 18 no arquivo**, o que prova que o arquivo binário guardou tudo, inclusive o registro excluído.

## Testes realizados

| # | Teste | Entrada | Resultado esperado | Resultado obtido |
|---|---|---|---|---|
| 1 | Texto no menu | `abc` | Mensagem de erro e pede de novo | OK |
| 2 | Menu vazio | ENTER | "Campo vazio" | OK |
| 3 | Número negativo | `-3` | "Número negativo não é permitido" | OK |
| 4 | Opção inexistente | `99` | "Digite um número entre 0 e 11" | OK |
| 5 | Código repetido | `1` | "Já existe um sensor com o código 1" | OK |
| 6 | Nome vazio | ENTER | "Campo obrigatório" | OK |
| 7 | Tipo fora da faixa | `9` | "Digite um número entre 1 e 7" | OK |
| 8 | Consultar por código | `20` | Ficha completa do sensor | OK |
| 9 | Consultar código inexistente | `777` | "Nenhum sensor com o código 777" | OK |
| 10 | Confirmação inválida | `x` | "Responda apenas S ou N" | OK |
| 11 | Exclusão lógica | sensor 20 | Status EXCLUÍDO; menu: 16 ativos, 1 excluído, 17 no arquivo | OK |
| 12 | Consultar sensor excluído | `20` | Avisa que foi excluído e continua no arquivo | OK |
| 13 | Reusar código excluído | `20` | "Pertence a um sensor excluído" | OK |
| 14 | Simular sensor excluído | `20` | "Não existe sensor ativo com o código 20" | OK |
| 15 | Movimento no modo Em casa | sensor 6 | Só registra | OK |
| 16 | Barreira no modo Em casa | sensor 12 | Dispara e chama a polícia | OK |
| 17 | Exportar TXT | opção 3 | `relatorio.txt` com ativos, excluídos e histórico | OK |
| 18 | Fechar e abrir de novo | — | Registros, exclusões, modo e histórico carregados dos `.dat` | OK |
| 19 | Tamanho do arquivo | 18 registros | `sensores.dat` com 8 + 18 × 88 = 1.592 bytes (os excluídos continuam lá) | OK |
| 20 | Entrada termina no meio | fim do arquivo | Sai sem travar | OK |
| 21 | Compilação | `-Wall -Wextra` | Nenhum aviso | OK |

## Divisão do grupo

### Luiz Gustavo — `main.c`, menu, contagem e organização
**Resumo:** o `main` carrega os dados do `sensores.dat` (ou cria os 16 sensores na primeira vez), mostra o menu num laço `do...while` e chama uma função para cada opção com `switch`. No topo do menu aparece a contagem de registros, feita por `contarAtivos()` e `contarExcluidos()`. O projeto é separado em módulos: cada `.c` tem um `.h` com as declarações.

- **Como o programa conta os registros?** `contarAtivos()` percorre o vetor e soma os que têm status ATIVO. Os excluídos são o total no arquivo menos os ativos.
- **Para que serve o `#ifndef` no `.h`?** Evita que o mesmo cabeçalho seja incluído duas vezes, o que daria erro de redefinição.
- **Por que a variável `sistema` é `static`?** Ela guarda 100 sensores e é grande. Com `static`, ela não fica na pilha e não corre risco de estourá-la.

### Marcos Eduardo — structs, cadastro e consulta por código (`sensores.c`)
**Resumo:** a struct `Sensor` é o registro do sistema: código, nome, local, tipo, se está ligado e o status (ativo ou excluído). A struct `Sistema` guarda o vetor de registros, a quantidade e o modo. O cadastro valida tudo e não aceita código repetido. A consulta por código mostra a ficha completa do sensor.

- **Como impedem código repetido?** No cadastro, `buscarQualquer()` percorre todos os registros, inclusive os excluídos. Se achar o código, mostra o erro e pede outro.
- **O que a consulta mostra?** Código, nome, local, tipo, se está ligado, se dispara no modo atual, o status e a posição do registro no arquivo.
- **Por que usar `enum`?** Dá nome aos números (`STATUS_EXCLUIDO` em vez de 0, `TIPO_GAS` em vez de 7), o que deixa o código mais claro.

### Gilmar Victor — modos do alarme e simulação (`tipoArmado()` e `simularDeteccao()`)
**Resumo:** `tipoArmado()` diz se um tipo de sensor está armado no modo atual: fumaça e gás sempre; no modo Em casa, tudo menos movimento; no modo Ausente, tudo. A simulação decide entre três resultados: desligado → ignorado, ligado mas não armado → só registra, armado → dispara e mostra a resposta automática. Cada simulação vira um `Evento` gravado no histórico.

- **Por que o movimento não fica armado no modo Em casa?** Porque os moradores estão andando pela casa e o alarme dispararia à toa.
- **Por que fumaça e gás disparam até desarmado?** É a zona 24 horas: incêndio e vazamento são perigosos mesmo com gente em casa.
- **Um sensor excluído pode disparar?** Não. A simulação só aceita sensores com status ATIVO.

### Joao Gabryel — exclusão lógica, arquivos e exportação TXT (`eventos.c` e `util.c`)
**Resumo:** a exclusão é lógica: o registro não é apagado, só tem o campo `status` trocado para EXCLUÍDO, e o arquivo binário é regravado com ele marcado. `sensores.dat` e `eventos.dat` são binários (`fwrite`/`fread`). A exportação grava todos os registros em `relatorio.txt` com `fprintf`. As leituras do teclado usam `fgets` + `strtol`.

- **Por que exclusão lógica e não apagar de verdade?** Para manter a integridade do arquivo binário: nenhum registro é removido nem muda de posição, e o histórico continua apontando para sensores que existem. Também dá para saber o que foi excluído.
- **Qual a diferença entre o arquivo binário e o TXT?** O binário grava os bytes da struct do jeito que estão na memória: é rápido e o programa lê de volta direto, mas não dá para ler no Bloco de Notas. O TXT é texto formatado para pessoas lerem.
- **Por que não usaram `scanf`?** O `scanf` deixa o ENTER no buffer e trava em loop se a pessoa digitar letra. Com `fgets` a linha inteira é lida e depois conferida com `strtol`.
