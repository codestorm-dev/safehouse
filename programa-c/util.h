/*
 * util.h - Funções de apoio: leitura segura do teclado e data/hora.
 * Todas as leituras usam fgets, então nunca sobra lixo no buffer do teclado.
 */
#ifndef UTIL_H
#define UTIL_H

/* Lê um texto obrigatório (não aceita campo vazio). */
void lerTexto(const char *pergunta, char *destino, int tamanho);

/* Lê um número inteiro entre minimo e maximo.
 * Recusa texto, campo vazio, número negativo e valores fora da faixa. */
int lerInteiro(const char *pergunta, int minimo, int maximo);

/* Pergunta S/N e devolve 1 para sim e 0 para não. */
int confirmar(const char *pergunta);

/* Espera o usuário apertar ENTER. */
void pausar(void);

/* Escreve a data e a hora atuais no formato dd/mm/aaaa hh:mm:ss. */
void dataHoraAtual(char *destino, int tamanho);

#endif
