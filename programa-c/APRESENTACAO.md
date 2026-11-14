# Apresentação — SafeHouse em C

## Onde cada critério é atendido

| Critério | Pontos | Onde mostrar |
|---|---|---|
| 5 funcionalidades obrigatórias | 0,8 | Opções 1–5 (sensores), 6 (modo), 7 (simulação), 8 (histórico), 9 (relatório) |
| Structs, funções e arquivos binários/TXT | 0,4 | Structs `Sensor`, `Sistema` e `Evento`; `sensores.dat` e `eventos.dat` com `fwrite`/`fread`; `relatorio.txt` com `fprintf` |
| Organização e boas práticas | 0,3 | Código em módulos (`.c` + `.h`), funções curtas e comentadas, validação de todas as entradas, compila sem avisos com `-Wall -Wextra` |
| Demonstração prática | 0,3 | Roteiro abaixo |
| Domínio e participação | 0,2 | Divisão do grupo e perguntas abaixo |

## Roteiro da demonstração (6 a 7 minutos)

Antes de começar, apague `sensores.dat`, `eventos.dat` e `relatorio.txt` da pasta para mostrar o programa do zero.

1. **Compilar (Luiz, 30 s):** rodar `gcc -Wall -Wextra -std=c99 -o safehouse main.c sensores.c eventos.c util.c` e mostrar que não aparece nenhum aviso.
2. **Abrir (Luiz, 20 s):** o programa avisa "Primeira execução: 16 sensores da casa do projeto cadastrados". Opção **2** lista os sensores.
3. **Cadastrar com erros tratados (Marcos, 1 min):** opção **1**.
   - No código, digitar `abc` (erro: não é número), depois `-5` (erro: negativo) e depois `1` (erro: código repetido). Por fim, `20`.
   - No nome, apertar ENTER vazio (erro: campo obrigatório) e depois digitar `Sensor da varanda`.
   - No tipo, digitar `9` (erro: fora da faixa) e depois `1`. Local: `4` (Sala).
4. **Buscar e alterar (Marcos, 40 s):** opção **3**, buscar por tipo **4** (Câmera). Opção **4**, sensor **4** (Janela da sala), opção **1**: desligar.
5. **Modo do alarme (Gilmar, 30 s):** opção **6**, escolher **2** (Em casa) e explicar que o movimento interno fica desligado para os moradores andarem.
6. **Simulações (Gilmar, 1 min 30 s):** opção **7** três vezes:
   - sensor **6** (movimento da sala) → só registra, porque no modo Em casa o movimento não está armado;
   - sensor **12** (barreira do muro) → ALARME DISPARADO, com sirene, luzes, fechaduras, câmeras e polícia 190;
   - sensor **4** (janela desligada) → ignorado, e a casa fica desprotegida (por isso nenhum sensor deve ficar desligado);
   - sensor **16** (gás) → dispara mesmo em qualquer modo (zona 24 h): fecha a válvula e chama os bombeiros 193.
7. **Excluir (Joao, 20 s):** opção **5**, sensor **20**, responder `x` (erro: só S ou N) e depois `S`.
8. **Arquivos (Joao, 1 min 30 s):**
   - opção **8**: histórico com data e hora vindo do `eventos.dat`;
   - opção **9**: gera o relatório; abrir `relatorio.txt` no Bloco de Notas;
   - opção **0**: sair. Abrir o programa de novo e listar (opção **2**): o sensor 4 continua desligado e o modo continua Em casa, o que prova que o arquivo binário guardou tudo.

## Testes realizados

| # | Teste | Entrada | Resultado esperado | Resultado obtido |
|---|---|---|---|---|
| 1 | Texto no menu | `abc` | Mensagem de erro e pede de novo | OK |
| 2 | Menu vazio | ENTER | "Campo vazio" | OK |
| 3 | Número negativo | `-3` | "Número negativo não é permitido" | OK |
| 4 | Opção inexistente | `15` | "Digite um número entre 0 e 10" | OK |
| 5 | Código repetido | `1` | "Já existe um sensor com o código 1" | OK |
| 6 | Nome vazio | ENTER | "Campo obrigatório" | OK |
| 7 | Tipo fora da faixa | `9` | "Digite um número entre 1 e 7" | OK |
| 8 | Simular sensor inexistente | `99` | "Não existe sensor com o código 99" | OK |
| 9 | Movimento no modo Em casa | sensor 6 | Só registra | OK |
| 10 | Barreira no modo Em casa | sensor 12 | Dispara e chama a polícia | OK |
| 11 | Sensor desligado | sensor 4 | Ignorado | OK |
| 12 | Gás | sensor 16 | Dispara, fecha a válvula e chama os bombeiros | OK |
| 13 | Confirmação inválida | `x` | "Responda apenas S ou N" | OK |
| 14 | Fechar e abrir de novo | — | Sensores, modo e histórico carregados dos `.dat` | OK |
| 15 | Relatório | opção 9 | `relatorio.txt` com 16 sensores e 4 eventos | OK |
| 16 | Entrada termina no meio | fim do arquivo | Sai sem travar | OK |
| 17 | Compilação | `-Wall -Wextra` | Nenhum aviso | OK |

## Divisão do grupo

### Luiz Gustavo — `main.c` e organização do projeto
**Resumo:** o `main` carrega os dados do `sensores.dat` (ou cria os 16 sensores na primeira vez), mostra o menu num laço `do...while` e chama uma função para cada opção com `switch`. O projeto é separado em módulos: cada `.c` tem um `.h` com as declarações.

- **Por que separar em vários arquivos?** Cada arquivo cuida de uma parte (sensores, eventos, leitura do teclado). Fica mais fácil de achar, testar e dividir o trabalho.
- **Para que serve o `#ifndef` no `.h`?** Evita que o mesmo cabeçalho seja incluído duas vezes, o que daria erro de redefinição.
- **Por que a variável `sistema` é `static`?** Ela guarda 100 sensores e é grande. Com `static`, ela não fica na pilha e não corre risco de estourá-la.

### Marcos Eduardo — structs e cadastro dos sensores (`sensores.c`)
**Resumo:** a struct `Sensor` guarda código, nome, local, tipo e se está ligado. A struct `Sistema` guarda o vetor de sensores, a quantidade e o modo. As funções cadastrar, listar, buscar, alterar e excluir trabalham nesse vetor e salvam no arquivo depois de cada mudança.

- **Como funciona a exclusão?** Acha a posição do sensor e puxa os próximos uma posição para trás; depois diminui a quantidade.
- **Como impedem código repetido?** `buscarIndice()` percorre o vetor; se achar o código, mostra erro e pede outro.
- **Por que usar `enum` para os tipos?** Dá nome aos números (`TIPO_GAS` em vez de 7), o que deixa o código mais claro.

### Gilmar Victor — modos do alarme e simulação (`tipoArmado()` e `simularDeteccao()`)
**Resumo:** `tipoArmado()` diz se um tipo de sensor está armado no modo atual: fumaça e gás sempre; no modo Em casa, tudo menos movimento; no modo Ausente, tudo. A simulação decide entre três resultados: desligado → ignorado, ligado mas não armado → só registra, armado → dispara e mostra a resposta automática.

- **Por que o movimento não fica armado no modo Em casa?** Porque os moradores estão andando pela casa e o alarme dispararia à toa.
- **Por que fumaça e gás disparam até desarmado?** É a zona 24 horas: incêndio e vazamento são perigosos mesmo com gente em casa.
- **O que muda na resposta ao fogo?** As fechaduras destravam para a família sair, e quem é chamado são os bombeiros (193), não a polícia.

### Joao Gabryel — arquivos e validações (`eventos.c` e `util.c`)
**Resumo:** `sensores.dat` é binário e é regravado inteiro com `fwrite` (modo, quantidade e vetor de sensores). `eventos.dat` é binário e cada evento é acrescentado no final com o modo `"ab"`. `relatorio.txt` é texto e é escrito com `fprintf`. As leituras do teclado usam `fgets` + `strtol` para validar tudo.

- **Qual a diferença entre o arquivo binário e o TXT?** O binário grava os bytes da struct do jeito que estão na memória: é rápido e o programa lê de volta direto, mas não dá para ler no Bloco de Notas. O TXT é texto formatado para pessoas lerem.
- **Como sabem quantos eventos existem?** Pelo tamanho do arquivo: `ftell` dividido por `sizeof(Evento)`.
- **Por que não usaram `scanf`?** O `scanf` deixa o ENTER no buffer e trava em loop se a pessoa digitar letra. Com `fgets` a linha inteira é lida e depois conferida com `strtol`.
