/*
 * sensores.h - Structs e funções dos sensores (os registros do sistema)
 *              e do modo do alarme.
 */
#ifndef SENSORES_H
#define SENSORES_H

#define MAX_SENSORES   100
#define TAM_NOME       50
#define TAM_COMODO     20
#define ARQ_SENSORES   "sensores.dat"   /* arquivo binário */

/* Tipos de sensor (os mesmos 7 do simulador 3D) */
enum TipoSensor {
    TIPO_ABERTURA = 1,  /* magnético de porta/janela */
    TIPO_MOVIMENTO,     /* infravermelho passivo (PIR) */
    TIPO_VIDRO,         /* quebra de vidro (acústico) */
    TIPO_CAMERA,        /* câmera com detecção de pessoas */
    TIPO_BARREIRA,      /* barreira infravermelha do muro */
    TIPO_FUMACA,        /* detector de fumaça (24 h) */
    TIPO_GAS,           /* detector de gás GLP (24 h) */
    TOTAL_TIPOS = TIPO_GAS
};

/* Modos do alarme */
enum ModoAlarme {
    MODO_DESARMADO = 1, /* só fumaça e gás armados */
    MODO_EM_CASA,       /* perímetro armado, movimento interno desligado */
    MODO_AUSENTE        /* tudo armado */
};

/* Status do registro: usado na exclusão lógica */
enum StatusRegistro {
    STATUS_EXCLUIDO = 0,    /* continua no arquivo, mas não aparece mais */
    STATUS_ATIVO    = 1
};

/* Um sensor instalado na casa (o registro do sistema) */
typedef struct {
    int  codigo;                /* código único do sensor */
    char nome[TAM_NOME];        /* ex.: "Porta da frente" */
    char comodo[TAM_COMODO];    /* onde está instalado */
    int  tipo;                  /* enum TipoSensor */
    int  ligado;                /* 1 = ligado, 0 = desligado */
    int  status;                /* enum StatusRegistro (exclusão lógica) */
} Sensor;

/* Estado completo da central: registros + modo atual */
typedef struct {
    Sensor lista[MAX_SENSORES];
    int    quantidade;          /* registros no arquivo (ativos + excluídos) */
    int    modo;                /* enum ModoAlarme */
} Sistema;

/* Textos para mostrar na tela */
const char *nomeTipo(int tipo);
const char *nomeModo(int modo);

/* Regra do alarme: 1 se esse tipo de sensor está armado no modo dado */
int tipoArmado(int tipo, int modo);

/* Procura um sensor ATIVO pelo código; devolve a posição ou -1 */
int buscarIndice(const Sistema *s, int codigo);

/* Contagem de registros (aparece no menu) */
int contarAtivos(const Sistema *s);
int contarExcluidos(const Sistema *s);

/* Arquivo binário */
int  carregarSistema(Sistema *s);           /* 1 = carregou, 0 = não existia */
int  salvarSistema(const Sistema *s);       /* 1 = salvou, 0 = erro */
void carregarSensoresPadrao(Sistema *s);    /* os sensores da casa do projeto */

/* Funcionalidades obrigatórias */
void cadastrarSensor(Sistema *s);           /* 1. cadastrar */
void consultarPorCodigo(const Sistema *s);  /* 2. consultar por código */
void excluirSensor(Sistema *s);             /* 4. exclusão lógica */
/* 3. exportar TXT: exportarTXT() em eventos.c
 * 5. contabilizar: contarAtivos() mostrado no menu (main.c) */

/* Funcionalidades extras */
void listarSensores(const Sistema *s);
void buscarPorTipo(const Sistema *s);
void alterarSensor(Sistema *s);
void mudarModo(Sistema *s);

/* Mostra uma linha da tabela de sensores */
void mostrarSensor(const Sistema *s, const Sensor *x);

#endif
