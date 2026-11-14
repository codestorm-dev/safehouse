/*
 * sensores.c - Cadastro dos sensores, modo do alarme e arquivo binário sensores.dat.
 */
#include <stdio.h>
#include <string.h>
#include "sensores.h"
#include "util.h"

static const char *COMODOS[] = {
    "Quarto", "Banheiro", "Cozinha", "Sala", "Garagem", "Quintal", "Frente", "Muro"
};
#define TOTAL_COMODOS ((int)(sizeof(COMODOS) / sizeof(COMODOS[0])))

const char *nomeTipo(int tipo)
{
    switch (tipo) {
    case TIPO_ABERTURA:  return "Porta/janela";
    case TIPO_MOVIMENTO: return "Movimento";
    case TIPO_VIDRO:     return "Quebra de vidro";
    case TIPO_CAMERA:    return "Camera";
    case TIPO_BARREIRA:  return "Barreira IR";
    case TIPO_FUMACA:    return "Fumaca";
    case TIPO_GAS:       return "Gas";
    default:             return "Desconhecido";
    }
}

const char *nomeModo(int modo)
{
    switch (modo) {
    case MODO_DESARMADO: return "Desarmado";
    case MODO_EM_CASA:   return "Em casa";
    case MODO_AUSENTE:   return "Ausente";
    default:             return "Desconhecido";
    }
}

int tipoArmado(int tipo, int modo)
{
    if (tipo == TIPO_FUMACA || tipo == TIPO_GAS) {
        return 1;                       /* zona 24 horas: sempre armada */
    }
    if (modo == MODO_AUSENTE) {
        return 1;                       /* tudo armado */
    }
    if (modo == MODO_EM_CASA) {
        return tipo != TIPO_MOVIMENTO;  /* moradores andando não disparam */
    }
    return 0;                           /* desarmado */
}

int buscarIndice(const Sistema *s, int codigo)
{
    int i;
    for (i = 0; i < s->quantidade; i++) {
        if (s->lista[i].codigo == codigo && s->lista[i].status == STATUS_ATIVO) {
            return i;
        }
    }
    return -1;
}

/* Procura o código entre TODOS os registros, inclusive os excluídos.
 * Usado no cadastro para não repetir o código de um registro que ainda está no arquivo. */
static int buscarQualquer(const Sistema *s, int codigo)
{
    int i;
    for (i = 0; i < s->quantidade; i++) {
        if (s->lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

int contarAtivos(const Sistema *s)
{
    int i, total = 0;
    for (i = 0; i < s->quantidade; i++) {
        if (s->lista[i].status == STATUS_ATIVO) {
            total++;
        }
    }
    return total;
}

int contarExcluidos(const Sistema *s)
{
    return s->quantidade - contarAtivos(s);
}

/* ---------- arquivo binário ---------- */

int carregarSistema(Sistema *s)
{
    FILE *arq = fopen(ARQ_SENSORES, "rb");

    s->quantidade = 0;
    s->modo = MODO_AUSENTE;
    if (arq == NULL) {
        return 0;                       /* primeira vez: arquivo ainda não existe */
    }
    if (fread(&s->modo, sizeof(int), 1, arq) != 1 ||
        fread(&s->quantidade, sizeof(int), 1, arq) != 1 ||
        s->quantidade < 0 || s->quantidade > MAX_SENSORES ||
        fread(s->lista, sizeof(Sensor), (size_t)s->quantidade, arq) != (size_t)s->quantidade) {
        printf("[AVISO] O arquivo %s está corrompido. Começando do zero.\n", ARQ_SENSORES);
        s->quantidade = 0;
        s->modo = MODO_AUSENTE;
        fclose(arq);
        return 0;
    }
    fclose(arq);
    return 1;
}

int salvarSistema(const Sistema *s)
{
    FILE *arq = fopen(ARQ_SENSORES, "wb");
    int ok;

    if (arq == NULL) {
        printf("[ERRO] Não foi possível gravar %s.\n", ARQ_SENSORES);
        return 0;
    }
    ok = fwrite(&s->modo, sizeof(int), 1, arq) == 1 &&
         fwrite(&s->quantidade, sizeof(int), 1, arq) == 1 &&
         fwrite(s->lista, sizeof(Sensor), (size_t)s->quantidade, arq) == (size_t)s->quantidade;
    fclose(arq);
    if (!ok) {
        printf("[ERRO] Falha ao gravar %s.\n", ARQ_SENSORES);
    }
    return ok;
}

/* Coloca na lista um sensor já preenchido */
static void adicionar(Sistema *s, int codigo, const char *nome, const char *comodo, int tipo)
{
    Sensor *x = &s->lista[s->quantidade++];
    x->codigo = codigo;
    strncpy(x->nome, nome, TAM_NOME - 1);
    x->nome[TAM_NOME - 1] = '\0';
    strncpy(x->comodo, comodo, TAM_COMODO - 1);
    x->comodo[TAM_COMODO - 1] = '\0';
    x->tipo = tipo;
    x->ligado = 1;
    x->status = STATUS_ATIVO;
}

void carregarSensoresPadrao(Sistema *s)
{
    s->quantidade = 0;
    s->modo = MODO_AUSENTE;
    adicionar(s, 1,  "Porta da frente",          "Sala",    TIPO_ABERTURA);
    adicionar(s, 2,  "Porta da cozinha",         "Cozinha", TIPO_ABERTURA);
    adicionar(s, 3,  "Portao da garagem",        "Garagem", TIPO_ABERTURA);
    adicionar(s, 4,  "Janela da sala",           "Sala",    TIPO_ABERTURA);
    adicionar(s, 5,  "Janela do quarto",         "Quarto",  TIPO_ABERTURA);
    adicionar(s, 6,  "Movimento da sala",        "Sala",    TIPO_MOVIMENTO);
    adicionar(s, 7,  "Movimento da cozinha",     "Cozinha", TIPO_MOVIMENTO);
    adicionar(s, 8,  "Vidro da sala",            "Sala",    TIPO_VIDRO);
    adicionar(s, 9,  "Vidro do quarto",          "Quarto",  TIPO_VIDRO);
    adicionar(s, 10, "Camera da frente",         "Frente",  TIPO_CAMERA);
    adicionar(s, 11, "Camera do quintal",        "Quintal", TIPO_CAMERA);
    adicionar(s, 12, "Barreira do muro dos fundos", "Muro", TIPO_BARREIRA);
    adicionar(s, 13, "Barreira do muro lateral", "Muro",    TIPO_BARREIRA);
    adicionar(s, 14, "Fumaca da cozinha",        "Cozinha", TIPO_FUMACA);
    adicionar(s, 15, "Fumaca da sala",           "Sala",    TIPO_FUMACA);
    adicionar(s, 16, "Gas da cozinha",           "Cozinha", TIPO_GAS);
}

/* ---------- telas ---------- */

static int escolherTipo(void)
{
    int i;
    printf("Tipos de sensor:\n");
    for (i = 1; i <= TOTAL_TIPOS; i++) {
        printf("  %d - %s\n", i, nomeTipo(i));
    }
    return lerInteiro("Escolha o tipo: ", 1, TOTAL_TIPOS);
}

static const char *escolherComodo(void)
{
    int i;
    printf("Locais da casa:\n");
    for (i = 0; i < TOTAL_COMODOS; i++) {
        printf("  %d - %s\n", i + 1, COMODOS[i]);
    }
    return COMODOS[lerInteiro("Escolha o local: ", 1, TOTAL_COMODOS) - 1];
}

static void cabecalhoTabela(void)
{
    printf("\n%-4s %-28s %-9s %-16s %-9s %s\n", "COD", "NOME", "LOCAL", "TIPO", "ESTADO", "NO MODO ATUAL");
    printf("---- ---------------------------- --------- ---------------- --------- -------------\n");
}

void mostrarSensor(const Sistema *s, const Sensor *x)
{
    const char *situacao;
    if (!x->ligado) {
        situacao = "nao detecta";
    } else if (tipoArmado(x->tipo, s->modo)) {
        situacao = "ARMADO";
    } else {
        situacao = "so registra";
    }
    printf("%-4d %-28s %-9s %-16s %-9s %s\n", x->codigo, x->nome, x->comodo,
           nomeTipo(x->tipo), x->ligado ? "Ligado" : "Desligado", situacao);
}

void cadastrarSensor(Sistema *s)
{
    Sensor novo;
    const char *comodo;

    printf("\n=== CADASTRAR SENSOR ===\n");
    if (s->quantidade >= MAX_SENSORES) {
        printf("[ERRO] Limite de %d sensores atingido.\n", MAX_SENSORES);
        return;
    }
    for (;;) {
        int pos;
        novo.codigo = lerInteiro("Código do sensor (1 a 9999): ", 1, 9999);
        pos = buscarQualquer(s, novo.codigo);
        if (pos < 0) {
            break;
        }
        if (s->lista[pos].status == STATUS_EXCLUIDO) {
            printf("  [ERRO] O código %d pertence a um sensor excluído que continua no arquivo. Escolha outro.\n", novo.codigo);
        } else {
            printf("  [ERRO] Já existe um sensor com o código %d. Escolha outro.\n", novo.codigo);
        }
    }
    lerTexto("Nome do sensor: ", novo.nome, TAM_NOME);
    novo.tipo = escolherTipo();
    comodo = escolherComodo();
    strncpy(novo.comodo, comodo, TAM_COMODO - 1);
    novo.comodo[TAM_COMODO - 1] = '\0';
    novo.ligado = 1;
    novo.status = STATUS_ATIVO;

    s->lista[s->quantidade++] = novo;
    if (salvarSistema(s)) {
        printf("\n[OK] Sensor \"%s\" cadastrado e salvo em %s.\n", novo.nome, ARQ_SENSORES);
    }
}

void listarSensores(const Sistema *s)
{
    int i, ligados = 0, armados = 0;

    printf("\n=== SENSORES INSTALADOS === (modo do alarme: %s)\n", nomeModo(s->modo));
    if (contarAtivos(s) == 0) {
        printf("Nenhum sensor ativo cadastrado.\n");
        return;
    }
    cabecalhoTabela();
    for (i = 0; i < s->quantidade; i++) {
        if (s->lista[i].status != STATUS_ATIVO) {
            continue;                   /* excluído: não aparece */
        }
        mostrarSensor(s, &s->lista[i]);
        if (s->lista[i].ligado) {
            ligados++;
            if (tipoArmado(s->lista[i].tipo, s->modo)) {
                armados++;
            }
        }
    }
    printf("\nTotal: %d sensores ativos | %d ligados | %d armados agora\n", contarAtivos(s), ligados, armados);
}

void consultarPorCodigo(const Sistema *s)
{
    int codigo, i, pos = -1;
    const Sensor *x;

    printf("\n=== CONSULTAR SENSOR POR CÓDIGO ===\n");
    codigo = lerInteiro("Código do sensor: ", 1, 9999);
    for (i = 0; i < s->quantidade; i++) {
        if (s->lista[i].codigo == codigo) {
            pos = i;
            break;
        }
    }
    if (pos < 0) {
        printf("[AVISO] Nenhum sensor com o código %d.\n", codigo);
        return;
    }
    x = &s->lista[pos];
    if (x->status == STATUS_EXCLUIDO) {
        printf("[AVISO] O sensor %d (%s) foi excluído.\n", codigo, x->nome);
        printf("        O registro continua no arquivo binário com status EXCLUÍDO.\n");
        return;
    }
    printf("\n+------------------------------------------------+\n");
    printf("  Código ............: %d\n", x->codigo);
    printf("  Nome ..............: %s\n", x->nome);
    printf("  Local .............: %s\n", x->comodo);
    printf("  Tipo ..............: %s\n", nomeTipo(x->tipo));
    printf("  Estado ............: %s\n", x->ligado ? "Ligado" : "Desligado");
    printf("  No modo %-10s: %s\n", nomeModo(s->modo),
           !x->ligado ? "não detecta (desligado)" :
           tipoArmado(x->tipo, s->modo) ? "ARMADO: dispara o alarme" : "só registra no histórico");
    printf("  Status do registro : Ativo\n");
    printf("  Posição no arquivo : registro %d de %d\n", pos + 1, s->quantidade);
    printf("+------------------------------------------------+\n");
}

void buscarPorTipo(const Sistema *s)
{
    int i, tipo, achou = 0;

    printf("\n=== BUSCAR SENSORES POR TIPO ===\n");
    tipo = escolherTipo();
    for (i = 0; i < s->quantidade; i++) {
        if (s->lista[i].status == STATUS_ATIVO && s->lista[i].tipo == tipo) {
            if (!achou) {
                cabecalhoTabela();
            }
            mostrarSensor(s, &s->lista[i]);
            achou++;
        }
    }
    if (!achou) {
        printf("[AVISO] Nenhum sensor ativo do tipo %s.\n", nomeTipo(tipo));
    } else {
        printf("\n%d sensor(es) do tipo %s.\n", achou, nomeTipo(tipo));
    }
}

void alterarSensor(Sistema *s)
{
    int codigo, pos, opcao;
    Sensor *x;
    const char *comodo;

    printf("\n=== ALTERAR SENSOR ===\n");
    codigo = lerInteiro("Código do sensor: ", 1, 9999);
    pos = buscarIndice(s, codigo);
    if (pos < 0) {
        printf("[AVISO] Nenhum sensor com o código %d.\n", codigo);
        return;
    }
    x = &s->lista[pos];
    cabecalhoTabela();
    mostrarSensor(s, x);

    printf("\nO que deseja alterar?\n");
    printf("1 - %s o sensor\n2 - Nome\n3 - Local\n4 - Tipo\n0 - Cancelar\n",
           x->ligado ? "Desligar" : "Ligar");
    opcao = lerInteiro("Opção: ", 0, 4);
    switch (opcao) {
    case 1:
        x->ligado = !x->ligado;
        break;
    case 2:
        lerTexto("Novo nome: ", x->nome, TAM_NOME);
        break;
    case 3:
        comodo = escolherComodo();
        strncpy(x->comodo, comodo, TAM_COMODO - 1);
        x->comodo[TAM_COMODO - 1] = '\0';
        break;
    case 4:
        x->tipo = escolherTipo();
        break;
    default:
        printf("Alteração cancelada.\n");
        return;
    }
    if (salvarSistema(s)) {
        printf("\n[OK] Sensor alterado:\n");
        mostrarSensor(s, x);
    }
}

void excluirSensor(Sistema *s)
{
    int codigo, pos;

    printf("\n=== EXCLUIR SENSOR (exclusão lógica) ===\n");
    codigo = lerInteiro("Código do sensor: ", 1, 9999);
    pos = buscarIndice(s, codigo);
    if (pos < 0) {
        printf("[AVISO] Nenhum sensor com o código %d.\n", codigo);
        return;
    }
    cabecalhoTabela();
    mostrarSensor(s, &s->lista[pos]);
    if (!confirmar("Tem certeza que deseja excluir este sensor?")) {
        printf("Exclusão cancelada.\n");
        return;
    }
    /* exclusão lógica: o registro NÃO sai do arquivo, só muda o status */
    s->lista[pos].status = STATUS_EXCLUIDO;
    if (salvarSistema(s)) {
        printf("[OK] Sensor %d marcado como EXCLUÍDO.\n", codigo);
        printf("     O registro continua no arquivo %s, mas não aparece mais nas listas.\n", ARQ_SENSORES);
        printf("     Registros ativos: %d | excluídos: %d\n", contarAtivos(s), contarExcluidos(s));
    }
}

void mudarModo(Sistema *s)
{
    printf("\n=== MODO DO ALARME === (atual: %s)\n", nomeModo(s->modo));
    printf("1 - Desarmado : só fumaça e gás ficam armados (zona 24 horas)\n");
    printf("2 - Em casa   : muro, câmeras, portas, janelas e vidros armados;\n");
    printf("                movimento interno desligado para os moradores andarem\n");
    printf("3 - Ausente   : todos os sensores armados\n");
    s->modo = lerInteiro("Novo modo: ", 1, 3);
    if (salvarSistema(s)) {
        printf("[OK] Sistema em modo %s.\n", nomeModo(s->modo));
    }
}
