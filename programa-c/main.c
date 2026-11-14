/*
 * SafeHouse - Central de segurança residencial (versão em C)
 *
 * Grupo: Luiz Gustavo Silva Prestes, Marcos Eduardo Martins da Silva,
 *        Gilmar Victor Gama Da Silva, Joao Gabryel Ferreira
 * Professora: Flavia
 *
 * Compilar: gcc -Wall -Wextra -std=c99 -o safehouse main.c sensores.c eventos.c util.c
 */
#include <stdio.h>
#include "sensores.h"
#include "eventos.h"
#include "util.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void mostrarMenu(const Sistema *s)
{
    int i, ligados = 0;
    for (i = 0; i < s->quantidade; i++) {
        ligados += s->lista[i].ligado;
    }
    printf("\n==============================================\n");
    printf("   SAFEHOUSE - Central de Segurança\n");
    printf("   Modo: %-10s | Sensores: %d (%d ligados)\n", nomeModo(s->modo), s->quantidade, ligados);
    printf("==============================================\n");
    printf(" SENSORES\n");
    printf("   1 - Cadastrar sensor\n");
    printf("   2 - Listar sensores\n");
    printf("   3 - Buscar sensor\n");
    printf("   4 - Alterar sensor (ligar/desligar, nome, local, tipo)\n");
    printf("   5 - Excluir sensor\n");
    printf(" ALARME\n");
    printf("   6 - Mudar modo do alarme\n");
    printf("   7 - Simular detecção (invasão ou emergência)\n");
    printf(" ARQUIVOS\n");
    printf("   8 - Ver histórico de eventos\n");
    printf("   9 - Gerar relatório TXT\n");
    printf("  10 - Apagar histórico de eventos\n");
    printf("   0 - Sair\n");
}

int main(void)
{
    static Sistema sistema;     /* static: a struct é grande, fica fora da pilha */
    int opcao;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);    /* acentos no console do Windows */
    SetConsoleCP(CP_UTF8);
#endif

    if (carregarSistema(&sistema)) {
        printf("Dados carregados de %s: %d sensores, modo %s.\n",
               ARQ_SENSORES, sistema.quantidade, nomeModo(sistema.modo));
    } else {
        carregarSensoresPadrao(&sistema);
        salvarSistema(&sistema);
        printf("Primeira execução: %d sensores da casa do projeto cadastrados em %s.\n",
               sistema.quantidade, ARQ_SENSORES);
    }

    do {
        mostrarMenu(&sistema);
        opcao = lerInteiro("Escolha uma opção: ", 0, 10);
        switch (opcao) {
        case 1:  cadastrarSensor(&sistema);  break;
        case 2:  listarSensores(&sistema);   break;
        case 3:  buscarSensor(&sistema);     break;
        case 4:  alterarSensor(&sistema);    break;
        case 5:  excluirSensor(&sistema);    break;
        case 6:  mudarModo(&sistema);        break;
        case 7:  simularDeteccao(&sistema);  break;
        case 8:  listarHistorico();          break;
        case 9:  gerarRelatorio(&sistema);   break;
        case 10: limparHistorico();          break;
        default: break;
        }
        if (opcao != 0) {
            pausar();
        }
    } while (opcao != 0);

    salvarSistema(&sistema);
    printf("Dados salvos. Até logo!\n");
    return 0;
}
