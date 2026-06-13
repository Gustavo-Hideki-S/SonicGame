#ifndef ITEM_MONITOR_VIDA_H
#define ITEM_MONITOR_VIDA_H

#include "raylib/raylib.h"
#include <stdbool.h>
#include "Animacao.h"

typedef enum {
    ESTADO_MONITOR_VIDA_PARADO = 0,
    ESTADO_MONITOR_VIDA_COLETADO = 1
} EstadoMonitorVida;

typedef struct {
    Rectangle ret;
    Color cor;
    int estado;
    bool ativo;
    Animacao animacaoParado;
    Animacao animacaoColetando;
    Animacao *animacoes[2];
    int quantidadeAnimacoes;
} ItemMonitorVida;

ItemMonitorVida *criarMonitorVida( Rectangle ret, Color cor );
void destruirMonitorVida( ItemMonitorVida *item );
void atualizarMonitorVida( ItemMonitorVida *item, float delta );
void desenharMonitorVida( ItemMonitorVida *item );
QuadroAnimacao *getQuadroAnimacaoAtualMonitorVida( ItemMonitorVida *item );

#endif // ITEM_MONITOR_VIDA_H