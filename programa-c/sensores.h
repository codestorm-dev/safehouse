/*
 * sensores.h - Structs e funções dos sensores e do modo do alarme.
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

/* Um sensor instalado na casa */
typedef struct {
    int  codigo;                /* número único do sensor */
    char nome[TAM_NOME];        /* ex.: "Porta da frente" */
    char comodo[TAM_COMODO];    /* onde está instalado */
    int  tipo;                  /* enum TipoSensor */
    int  ligado;                /* 1 = ligado, 0 = desligado */
} Sensor;

/* Estado completo da central: lista de sensores + modo atual */
typedef struct {
    Sensor lista[MAX_SENSORES];
    int    quantidade;
    int    modo;                /* enum ModoAlarme */
} Sistema;

/* Textos para mostrar na tela */
const char *nomeTipo(int tipo);
const char *nomeModo(int modo);

/* Regra do alarme: 1 se esse tipo de sensor está armado no modo dado */
int tipoArmado(int tipo, int modo);

/* Procura o sensor pelo código; devolve a posição na lista ou -1 */
int buscarIndice(const Sistema *s, int codigo);

/* Arquivo binário */
int  carregarSistema(Sistema *s);           /* 1 = carregou, 0 = não existia */
int  salvarSistema(const Sistema *s);       /* 1 = salvou, 0 = erro */
void carregarSensoresPadrao(Sistema *s);    /* os sensores da casa do projeto */

/* Funcionalidade 1: gerenciar sensores */
void cadastrarSensor(Sistema *s);
void listarSensores(const Sistema *s);
void buscarSensor(const Sistema *s);
void alterarSensor(Sistema *s);
void excluirSensor(Sistema *s);

/* Funcionalidade 2: modo do alarme */
void mudarModo(Sistema *s);

/* Mostra uma linha da tabela de sensores */
void mostrarSensor(const Sistema *s, const Sensor *x);

#endif
