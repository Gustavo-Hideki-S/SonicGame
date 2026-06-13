#ifndef ITEM_MONITOR_INVENCIVEL_H
#define ITEM_MONITOR_INVENCIVEL_H

#include "raylib/raylib.h"
#include <stdbool.h>
#include "Animacao.h"

typedef enum {
    ESTADO_MONITOR_INVENCIVEL_PARADO = 0,
    ESTADO_MONITOR_INVENCIVEL_COLETADO = 1
} EstadoMonitorInvencivel;

typedef struct {
    Rectangle ret;
    Color cor;
    int estado;
    bool ativo;
    Animacao animacaoParado;
    Animacao animacaoColetando;
    Animacao *animacoes[2];
    int quantidadeAnimacoes;
} ItemMonitorInvencivel;

ItemMonitorInvencivel *criarMonitorInvencivel( Rectangle ret, Color cor );
void destruirMonitorInvencivel( ItemMonitorInvencivel *item );
void atualizarMonitorInvencivel( ItemMonitorInvencivel *item, float delta );
void desenharMonitorInvencivel( ItemMonitorInvencivel *item );
QuadroAnimacao *getQuadroAnimacaoAtualMonitorInvencivel( ItemMonitorInvencivel *item );

#endif // ITEM_MONITOR_INVENCIVEL_H