#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "ItemMonitorInvencivel.h"
#include "ResourceManager.h"
#include "Macros.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoMonitorInvencivel( ItemMonitorInvencivel *item, QuadroAnimacao *qa, Color tonalidade );
static Animacao *getAnimacaoAtualMonitorInvencivel( ItemMonitorInvencivel *item );

static const bool MOSTRAR_RETANGULOS = false;

ItemMonitorInvencivel *criarMonitorInvencivel( Rectangle ret, Color cor ) {
    ItemMonitorInvencivel *novo = (ItemMonitorInvencivel*) malloc( sizeof( ItemMonitorInvencivel ) );
    if ( novo == NULL ) return NULL;

    novo->ret = ret;
    novo->cor = cor;
    novo->estado = ESTADO_MONITOR_INVENCIVEL_PARADO;
    novo->ativo = true;

    novo->animacaoParado.quantidadeQuadros = 1;
    novo->animacaoParado.quadroAtual = 0;
    novo->animacaoParado.contadorTempoQuadro = 0.0f;
    novo->animacaoParado.pararNoUltimoQuadro = false;
    novo->animacaoParado.executarUmaVez = false;
    novo->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novo->animacaoParado, 1 );
    novo->animacaoParado.quadros[0].fonte = (Rectangle){ 24.0f, 485.0f, 32.0f, 32.0f };
    novo->animacaoParado.quadros[0].retColisao = (Rectangle){ 0, 0, ret.width, ret.height };

    novo->animacaoColetando.quantidadeQuadros = 1;
    novo->animacaoColetando.quadroAtual = 0;
    novo->animacaoColetando.contadorTempoQuadro = 0.0f;
    novo->animacaoColetando.pararNoUltimoQuadro = true;
    novo->animacaoColetando.executarUmaVez = true;
    novo->animacaoColetando.finalizada = false;
    criarQuadrosAnimacao( &novo->animacaoColetando, 1 );
    novo->animacaoColetando.quadros[0].fonte = (Rectangle){ 24.0f, 485.0f, 32.0f, 32.0f };
    novo->animacaoColetando.quadros[0].retColisao = (Rectangle){ 0, 0, ret.width, ret.height };

    novo->animacoes[ESTADO_MONITOR_INVENCIVEL_PARADO] = &novo->animacaoParado;
    novo->animacoes[ESTADO_MONITOR_INVENCIVEL_COLETADO] = &novo->animacaoColetando;
    novo->quantidadeAnimacoes = 2;

    return novo;
}

void destruirMonitorInvencivel( ItemMonitorInvencivel *item ) {
    if ( item != NULL ) {
        for ( int i = 0; i < item->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( item->animacoes[i] );
        }
        free( item );
    }
}

void atualizarMonitorInvencivel( ItemMonitorInvencivel *item, float delta ) {
    if ( item->ativo ) {
        Animacao *animacaoAtual = getAnimacaoAtualMonitorInvencivel( item );
        atualizarAnimacao( animacaoAtual, delta );
        if ( item->estado == ESTADO_MONITOR_INVENCIVEL_COLETADO && animacaoAtual->finalizada ) {
            item->ativo = false;
        }
    }
}

void desenharMonitorInvencivel( ItemMonitorInvencivel *item ) {
    if ( item->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualMonitorInvencivel( item );
        desenharQuadroAnimacaoMonitorInvencivel( item, qa, WHITE );
        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( item->ret, Fade( item->cor, 0.5f ) );
            DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
        }
    }
}

QuadroAnimacao *getQuadroAnimacaoAtualMonitorInvencivel( ItemMonitorInvencivel *item ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualMonitorInvencivel( item ) );
}

static void desenharQuadroAnimacaoMonitorInvencivel( ItemMonitorInvencivel *item, QuadroAnimacao *qa, Color tonalidade ) {
    if ( qa != NULL ) {
        DrawTexturePro(
            rm.texturaItens2,
            qa->fonte,
            item->ret,
            (Vector2){ 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = item->ret.x + qa->retColisao.x;
            float yDesenho = item->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }
    }
}

static Animacao *getAnimacaoAtualMonitorInvencivel( ItemMonitorInvencivel *item ) {
    return item->animacoes[item->estado];
}