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
    printf("\n====================================================\n");
    printf("   SAFEHOUSE - Central de Segurança\n");
    printf("   Modo do alarme: %s\n", nomeModo(s->modo));
    /* Obrigatória 5: contabilizar registros */
    printf("   Registros ativos: %d | excluídos: %d | no arquivo: %d\n",
           contarAtivos(s), contarExcluidos(s), s->quantidade);
    printf("====================================================\n");
    printf(" REGISTROS (SENSORES)\n");
    printf("   1 - Cadastrar sensor\n");
    printf("   2 - Consultar sensor por código\n");
    printf("   3 - Exportar registros para TXT\n");
    printf("   4 - Excluir sensor (exclusão lógica)\n");
    printf("   5 - Listar sensores ativos\n");
    printf("   6 - Buscar sensores por tipo\n");
    printf("   7 - Alterar sensor (ligar/desligar, nome, local, tipo)\n");
    printf(" ALARME\n");
    printf("   8 - Mudar modo do alarme\n");
    printf("   9 - Simular detecção (invasão ou emergência)\n");
    printf("  10 - Ver histórico de eventos\n");
    printf("  11 - Apagar histórico de eventos\n");
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
        printf("Dados carregados de %s: %d sensores ativos, modo %s.\n",
               ARQ_SENSORES, contarAtivos(&sistema), nomeModo(sistema.modo));
    } else {
        carregarSensoresPadrao(&sistema);
        salvarSistema(&sistema);
        printf("Primeira execução: %d sensores da casa do projeto cadastrados em %s.\n",
               sistema.quantidade, ARQ_SENSORES);
    }

    do {
        mostrarMenu(&sistema);
        opcao = lerInteiro("Escolha uma opção: ", 0, 11);
        switch (opcao) {
        case 1:  cadastrarSensor(&sistema);    break;
        case 2:  consultarPorCodigo(&sistema); break;
        case 3:  exportarTXT(&sistema);        break;
        case 4:  excluirSensor(&sistema);      break;
        case 5:  listarSensores(&sistema);     break;
        case 6:  buscarPorTipo(&sistema);      break;
        case 7:  alterarSensor(&sistema);      break;
        case 8:  mudarModo(&sistema);          break;
        case 9:  simularDeteccao(&sistema);    break;
        case 10: listarHistorico();            break;
        case 11: limparHistorico();            break;
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
