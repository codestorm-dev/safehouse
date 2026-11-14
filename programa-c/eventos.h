/*
 * eventos.h - Simulação de detecção, histórico (binário) e relatório (TXT).
 */
#ifndef EVENTOS_H
#define EVENTOS_H

#include "sensores.h"

#define ARQ_EVENTOS    "eventos.dat"     /* arquivo binário */
#define ARQ_RELATORIO  "relatorio.txt"   /* arquivo texto */

/* O que a central decidiu quando o sensor detectou algo */
enum Resultado {
    RES_IGNORADO = 0,   /* sensor desligado: nada acontece */
    RES_REGISTRADO,     /* ligado mas não armado: só registra */
    RES_DISPARO         /* armado: dispara o alarme */
};

/* Um evento gravado no histórico */
typedef struct {
    int  id;                    /* número sequencial do evento */
    char dataHora[20];          /* dd/mm/aaaa hh:mm:ss */
    int  codigoSensor;
    char nomeSensor[TAM_NOME];
    char comodo[TAM_COMODO];
    int  tipo;                  /* tipo do sensor */
    int  modo;                  /* modo do alarme no momento */
    int  resultado;             /* enum Resultado */
} Evento;

/* Obrigatória 3: exportar todos os registros para texto (relatorio.txt) */
void exportarTXT(const Sistema *s);

/* Extras: simular uma invasão ou emergência e histórico (eventos.dat) */
void simularDeteccao(const Sistema *s);
void listarHistorico(void);
void limparHistorico(void);

#endif
