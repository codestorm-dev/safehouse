/*
 * eventos.c - Simulação de detecção com resposta automática,
 *             histórico em eventos.dat (binário) e relatório em relatorio.txt (texto).
 */
#include <stdio.h>
#include <string.h>
#include "eventos.h"
#include "util.h"

static const char *nomeResultado(int r)
{
    switch (r) {
    case RES_DISPARO:    return "DISPARO";
    case RES_REGISTRADO: return "Registrado";
    default:             return "Ignorado";
    }
}

/* Conta quantos eventos já existem no arquivo, pelo tamanho dele */
static int contarEventos(void)
{
    FILE *arq = fopen(ARQ_EVENTOS, "rb");
    long bytes;

    if (arq == NULL) {
        return 0;
    }
    fseek(arq, 0, SEEK_END);
    bytes = ftell(arq);
    fclose(arq);
    return bytes > 0 ? (int)(bytes / (long)sizeof(Evento)) : 0;
}

/* Acrescenta um evento no final de eventos.dat */
static int gravarEvento(const Evento *e)
{
    FILE *arq = fopen(ARQ_EVENTOS, "ab");
    int ok;

    if (arq == NULL) {
        printf("[ERRO] Não foi possível gravar %s.\n", ARQ_EVENTOS);
        return 0;
    }
    ok = fwrite(e, sizeof(Evento), 1, arq) == 1;
    fclose(arq);
    return ok;
}

/* Mostra, passo a passo, o que a casa faz quando o alarme dispara */
static void respostaAutomatica(const Sensor *x)
{
    if (x->tipo == TIPO_FUMACA || x->tipo == TIPO_GAS) {
        printf("  0,3 s  Sirene ligada (120 dB)\n");
        printf("  0,8 s  Luzes acesas para facilitar a saída\n");
        printf("  1,3 s  Fechaduras DESTRAVADAS para a família sair\n");
        if (x->tipo == TIPO_GAS) {
            printf("  1,5 s  Válvula do botijão fechada e exaustor ligado\n");
        }
        printf("  2,6 s  Central de monitoramento recebeu o alerta\n");
        printf("  5,0 s  Bombeiros (193) acionados\n");
        printf("  App do morador: \"Emergência! %s em %s\"\n", nomeTipo(x->tipo), x->comodo);
    } else {
        printf("  0,3 s  Sirene ligada (120 dB) dentro e fora da casa\n");
        printf("  0,8 s  Todas as luzes e refletores acesos\n");
        printf("  1,3 s  Fechaduras travadas\n");
        printf("  1,7 s  Câmeras gravando clipe de 30 s e enviando foto ao celular\n");
        printf("  2,6 s  Central de monitoramento recebeu o alerta e ligou para o morador\n");
        printf("  5,0 s  Polícia (190) acionada\n");
        printf("  App do morador: \"Alarme! %s - %s\"\n", x->nome, x->comodo);
    }
}

void simularDeteccao(const Sistema *s)
{
    int i, codigo, pos;
    const Sensor *x;
    Evento e;

    printf("\n=== SIMULAR DETECÇÃO === (modo do alarme: %s)\n", nomeModo(s->modo));
    if (contarAtivos(s) == 0) {
        printf("Cadastre um sensor antes de simular.\n");
        return;
    }
    printf("Qual sensor percebeu algo? (intruso, fumaça ou gás)\n");
    for (i = 0; i < s->quantidade; i++) {
        if (s->lista[i].status != STATUS_ATIVO) {
            continue;
        }
        printf("  %4d - %s (%s)%s\n", s->lista[i].codigo, s->lista[i].nome,
               s->lista[i].comodo, s->lista[i].ligado ? "" : " [desligado]");
    }
    for (;;) {
        codigo = lerInteiro("Código do sensor: ", 1, 9999);
        pos = buscarIndice(s, codigo);
        if (pos >= 0) {
            break;
        }
        printf("  [ERRO] Não existe sensor ativo com o código %d.\n", codigo);
    }
    x = &s->lista[pos];

    /* a mesma decisão da função detect() do simulador 3D */
    if (!x->ligado) {
        e.resultado = RES_IGNORADO;
    } else if (tipoArmado(x->tipo, s->modo)) {
        e.resultado = RES_DISPARO;
    } else {
        e.resultado = RES_REGISTRADO;
    }

    e.id = contarEventos() + 1;
    dataHoraAtual(e.dataHora, sizeof(e.dataHora));
    e.codigoSensor = x->codigo;
    strncpy(e.nomeSensor, x->nome, TAM_NOME - 1);
    e.nomeSensor[TAM_NOME - 1] = '\0';
    strncpy(e.comodo, x->comodo, TAM_COMODO - 1);
    e.comodo[TAM_COMODO - 1] = '\0';
    e.tipo = x->tipo;
    e.modo = s->modo;

    printf("\n[%s] %s (%s)\n", e.dataHora, x->nome, nomeTipo(x->tipo));
    if (e.resultado == RES_IGNORADO) {
        printf("O sensor está DESLIGADO e não percebeu nada. A casa ficou desprotegida neste ponto!\n");
    } else if (e.resultado == RES_REGISTRADO) {
        printf("Sensor detectou, mas não está armado no modo %s.\n", nomeModo(s->modo));
        printf("O evento foi só registrado no histórico, sem disparar o alarme.\n");
    } else {
        printf("*** ALARME DISPARADO! ***\nResposta automática:\n");
        respostaAutomatica(x);
    }

    if (gravarEvento(&e)) {
        printf("\n[OK] Evento #%d salvo em %s.\n", e.id, ARQ_EVENTOS);
    }
}

void listarHistorico(void)
{
    FILE *arq = fopen(ARQ_EVENTOS, "rb");
    Evento e;
    int total = 0, disparos = 0;

    printf("\n=== HISTÓRICO DE EVENTOS ===\n");
    if (arq == NULL) {
        printf("Nenhum evento registrado ainda.\n");
        return;
    }
    printf("%-4s %-19s %-28s %-9s %-10s %s\n", "#", "DATA E HORA", "SENSOR", "LOCAL", "MODO", "RESULTADO");
    printf("---- ------------------- ---------------------------- --------- ---------- ----------\n");
    while (fread(&e, sizeof(Evento), 1, arq) == 1) {
        printf("%-4d %-19s %-28s %-9s %-10s %s\n", e.id, e.dataHora, e.nomeSensor,
               e.comodo, nomeModo(e.modo), nomeResultado(e.resultado));
        total++;
        if (e.resultado == RES_DISPARO) {
            disparos++;
        }
    }
    fclose(arq);
    if (total == 0) {
        printf("Nenhum evento registrado ainda.\n");
    } else {
        printf("\nTotal: %d eventos, %d disparos de alarme.\n", total, disparos);
    }
}

void limparHistorico(void)
{
    if (!confirmar("Apagar todo o histórico de eventos?")) {
        printf("Nada foi apagado.\n");
        return;
    }
    if (remove(ARQ_EVENTOS) == 0) {
        printf("[OK] Histórico apagado.\n");
    } else {
        printf("O histórico já estava vazio.\n");
    }
}

void exportarTXT(const Sistema *s)
{
    FILE *txt, *dat;
    char agora[20];
    int i, t, ligados = 0, armados = 0, total = 0, disparos = 0, registrados = 0;
    int ativos = contarAtivos(s), excluidos = contarExcluidos(s);
    int porTipo[TOTAL_TIPOS + 1] = {0};
    Evento e;

    txt = fopen(ARQ_RELATORIO, "w");
    if (txt == NULL) {
        printf("[ERRO] Não foi possível criar %s.\n", ARQ_RELATORIO);
        return;
    }
    dataHoraAtual(agora, sizeof(agora));

    for (i = 0; i < s->quantidade; i++) {
        if (s->lista[i].status != STATUS_ATIVO) {
            continue;
        }
        if (s->lista[i].tipo >= 1 && s->lista[i].tipo <= TOTAL_TIPOS) {
            porTipo[s->lista[i].tipo]++;
        }
        if (s->lista[i].ligado) {
            ligados++;
            if (tipoArmado(s->lista[i].tipo, s->modo)) {
                armados++;
            }
        }
    }

    fprintf(txt, "==============================================================\n");
    fprintf(txt, "  SAFEHOUSE - RELATORIO DO SISTEMA DE SEGURANCA\n");
    fprintf(txt, "  Gerado em: %s\n", agora);
    fprintf(txt, "==============================================================\n\n");
    fprintf(txt, "Modo do alarme: %s\n", nomeModo(s->modo));
    fprintf(txt, "Registros: %d ativos | %d excluidos | %d no arquivo binario\n", ativos, excluidos, s->quantidade);
    fprintf(txt, "Sensores ativos: %d ligados | %d armados agora\n\n", ligados, armados);

    fprintf(txt, "SENSORES POR TIPO\n");
    for (t = 1; t <= TOTAL_TIPOS; t++) {
        fprintf(txt, "  %-16s %d\n", nomeTipo(t), porTipo[t]);
    }

    fprintf(txt, "\nSENSORES ATIVOS\n");
    fprintf(txt, "%-4s %-28s %-9s %-16s %s\n", "COD", "NOME", "LOCAL", "TIPO", "ESTADO");
    for (i = 0; i < s->quantidade; i++) {
        const Sensor *x = &s->lista[i];
        if (x->status != STATUS_ATIVO) {
            continue;
        }
        fprintf(txt, "%-4d %-28s %-9s %-16s %s\n", x->codigo, x->nome, x->comodo,
                nomeTipo(x->tipo), x->ligado ? "Ligado" : "Desligado");
    }

    fprintf(txt, "\nREGISTROS EXCLUIDOS (exclusao logica: continuam no arquivo binario)\n");
    if (excluidos == 0) {
        fprintf(txt, "  Nenhum registro excluido.\n");
    }
    for (i = 0; i < s->quantidade; i++) {
        const Sensor *x = &s->lista[i];
        if (x->status == STATUS_EXCLUIDO) {
            fprintf(txt, "%-4d %-28s %-9s %-16s EXCLUIDO\n", x->codigo, x->nome, x->comodo, nomeTipo(x->tipo));
        }
    }

    fprintf(txt, "\nHISTORICO DE EVENTOS\n");
    dat = fopen(ARQ_EVENTOS, "rb");
    if (dat != NULL) {
        while (fread(&e, sizeof(Evento), 1, dat) == 1) {
            fprintf(txt, "#%-3d %s  %-28s %-9s modo %-10s %s\n", e.id, e.dataHora,
                    e.nomeSensor, e.comodo, nomeModo(e.modo), nomeResultado(e.resultado));
            total++;
            if (e.resultado == RES_DISPARO) {
                disparos++;
            } else if (e.resultado == RES_REGISTRADO) {
                registrados++;
            }
        }
        fclose(dat);
    }
    if (total == 0) {
        fprintf(txt, "  Nenhum evento registrado.\n");
    }
    fprintf(txt, "\nRESUMO: %d eventos | %d disparos | %d so registrados | %d ignorados\n",
            total, disparos, registrados, total - disparos - registrados);

    /* pontos fracos: sensores desligados */
    fprintf(txt, "\nPONTOS DE ATENCAO\n");
    if (ligados == ativos) {
        fprintf(txt, "  Todos os sensores ativos estao ligados.\n");
    } else {
        for (i = 0; i < s->quantidade; i++) {
            if (s->lista[i].status == STATUS_ATIVO && !s->lista[i].ligado) {
                fprintf(txt, "  ! %s (%s) esta desligado\n", s->lista[i].nome, s->lista[i].comodo);
            }
        }
    }
    fclose(txt);

    printf("\n[OK] Registros exportados para %s (%d ativos, %d excluídos, %d eventos).\n",
           ARQ_RELATORIO, ativos, excluidos, total);
    printf("Abra o arquivo no Bloco de Notas para ver.\n");
}
