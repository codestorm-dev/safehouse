/*
 * util.c - Leitura segura do teclado e data/hora.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include "util.h"

/* Lê uma linha inteira e tira o ENTER do final.
 * Se a pessoa digitar mais do que cabe, o resto é descartado.
 * Se a entrada acabar (fim do arquivo de teste), encerra o programa. */
static void lerLinha(char *destino, int tamanho)
{
    size_t n;
    int c;

    if (fgets(destino, tamanho, stdin) == NULL) {
        printf("\nEntrada encerrada. Saindo do programa.\n");
        exit(0);
    }
    n = strlen(destino);
    if (n > 0 && destino[n - 1] == '\n') {
        destino[n - 1] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta o que passou do tamanho */
        }
    }
}

/* Remove espaços do começo e do fim do texto. */
static void aparar(char *texto)
{
    size_t inicio = 0, fim = strlen(texto);

    while (texto[inicio] != '\0' && isspace((unsigned char)texto[inicio])) {
        inicio++;
    }
    while (fim > inicio && isspace((unsigned char)texto[fim - 1])) {
        fim--;
    }
    memmove(texto, texto + inicio, fim - inicio);
    texto[fim - inicio] = '\0';
}

void lerTexto(const char *pergunta, char *destino, int tamanho)
{
    do {
        printf("%s", pergunta);
        lerLinha(destino, tamanho);
        aparar(destino);
        if (destino[0] == '\0') {
            printf("  [ERRO] Campo obrigatório. Digite algum texto.\n");
        }
    } while (destino[0] == '\0');
}

int lerInteiro(const char *pergunta, int minimo, int maximo)
{
    char linha[64];
    char *fim;
    long valor;

    for (;;) {
        printf("%s", pergunta);
        lerLinha(linha, sizeof(linha));
        aparar(linha);

        if (linha[0] == '\0') {
            printf("  [ERRO] Campo vazio. Digite um número.\n");
            continue;
        }
        errno = 0;
        valor = strtol(linha, &fim, 10);
        if (*fim != '\0') {
            printf("  [ERRO] \"%s\" não é um número. Digite apenas algarismos.\n", linha);
            continue;
        }
        if (errno == ERANGE || valor > INT_MAX || valor < INT_MIN) {
            printf("  [ERRO] Número grande demais.\n");
            continue;
        }
        if (valor < 0 && minimo >= 0) {
            printf("  [ERRO] Número negativo não é permitido.\n");
            continue;
        }
        if (valor < minimo || valor > maximo) {
            printf("  [ERRO] Digite um número entre %d e %d.\n", minimo, maximo);
            continue;
        }
        return (int)valor;
    }
}

int confirmar(const char *pergunta)
{
    char linha[16];

    for (;;) {
        printf("%s (S/N): ", pergunta);
        lerLinha(linha, sizeof(linha));
        aparar(linha);
        if (linha[0] == 'S' || linha[0] == 's') {
            return 1;
        }
        if (linha[0] == 'N' || linha[0] == 'n') {
            return 0;
        }
        printf("  [ERRO] Responda apenas S ou N.\n");
    }
}

void pausar(void)
{
    char linha[8];

    printf("\nPressione ENTER para voltar ao menu...");
    lerLinha(linha, sizeof(linha));
}

void dataHoraAtual(char *destino, int tamanho)
{
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);

    if (t == NULL || strftime(destino, (size_t)tamanho, "%d/%m/%Y %H:%M:%S", t) == 0) {
        strncpy(destino, "sem data", (size_t)tamanho - 1);
        destino[tamanho - 1] = '\0';
    }
}
