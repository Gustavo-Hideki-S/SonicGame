#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "ItemMonitorVida.h"
#include "ResourceManager.h"
#include "Macros.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoMonitorVida( ItemMonitorVida *item, QuadroAnimacao *qa, Color tonalidade );
static Animacao *getAnimacaoAtualMonitorVida( ItemMonitorVida *item );

static const bool MOSTRAR_RETANGULOS = false;

ItemMonitorVida *criarMonitorVida( Rectangle ret, Color cor ) {
    ItemMonitorVida *novo = (ItemMonitorVida*) malloc( sizeof( ItemMonitorVida ) );
    if ( novo == NULL ) return NULL;

    novo->ret = ret;
    novo->cor = cor;
    novo->estado = ESTADO_MONITOR_VIDA_PARADO;
    novo->ativo = true;

    novo->animacaoParado.quantidadeQuadros = 1;
    novo->animacaoParado.quadroAtual = 0;
    novo->animacaoParado.contadorTempoQuadro = 0.0f;
    novo->animacaoParado.pararNoUltimoQuadro = false;
    novo->animacaoParado.executarUmaVez = false;
    novo->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novo->animacaoParado, 1 );
    novo->animacaoParado.quadros[0].fonte = (Rectangle){ 78.0f, 466.0f, 32.0f, 32.0f };
    novo->animacaoParado.quadros[0].retColisao = (Rectangle){ 0, 0, ret.width, ret.height };

    novo->animacaoColetando.quantidadeQuadros = 1;
    novo->animacaoColetando.quadroAtual = 0;
    novo->animacaoColetando.contadorTempoQuadro = 0.0f;
    novo->animacaoColetando.pararNoUltimoQuadro = true;
    novo->animacaoColetando.executarUmaVez = true;
    novo->animacaoColetando.finalizada = false;
    criarQuadrosAnimacao( &novo->animacaoColetando, 1 );
    novo->animacaoColetando.quadros[0].fonte = (Rectangle){ 78.0f, 466.0f, 32.0f, 32.0f };
    novo->animacaoColetando.quadros[0].retColisao = (Rectangle){ 0, 0, ret.width, ret.height };

    novo->animacoes[ESTADO_MONITOR_VIDA_PARADO] = &novo->animacaoParado;
    novo->animacoes[ESTADO_MONITOR_VIDA_COLETADO] = &novo->animacaoColetando;
    novo->quantidadeAnimacoes = 2;

    return novo;
}

void destruirMonitorVida( ItemMonitorVida *item ) {
    if ( item != NULL ) {
        for ( int i = 0; i < item->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( item->animacoes[i] );
        }
        free( item );
    }
}

void atualizarMonitorVida( ItemMonitorVida *item, float delta ) {
    if ( item->ativo ) {
        Animacao *animacaoAtual = getAnimacaoAtualMonitorVida( item );
        atualizarAnimacao( animacaoAtual, delta );
        if ( item->estado == ESTADO_MONITOR_VIDA_COLETADO && animacaoAtual->finalizada ) {
            item->ativo = false;
        }
    }
}

void desenharMonitorVida( ItemMonitorVida *item ) {
    if ( item->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualMonitorVida( item );
        desenharQuadroAnimacaoMonitorVida( item, qa, WHITE );
        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( item->ret, Fade( item->cor, 0.5f ) );
            DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
        }
    }
}

QuadroAnimacao *getQuadroAnimacaoAtualMonitorVida( ItemMonitorVida *item ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualMonitorVida( item ) );
}

static void desenharQuadroAnimacaoMonitorVida( ItemMonitorVida *item, QuadroAnimacao *qa, Color tonalidade ) {
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

static Animacao *getAnimacaoAtualMonitorVida( ItemMonitorVida *item ) {
    return item->animacoes[item->estado];
}