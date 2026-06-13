/**
 * @file InimigoCrabmeat.c
 * @brief Implementacao do inimigo Crabmeat.
 */
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoCrabmeat.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void inicializarAnimacaoCrabmeat( Animacao *anim, int quantidadeQuadros, bool executarUmaVez );
static Animacao *getAnimacaoAtualInimigoCrabmeat( InimigoCrabmeat *inimigo );
static void desenharQuadroAnimacaoInimigoCrabmeat( InimigoCrabmeat *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoCrabmeatMorrendo( InimigoCrabmeat *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static void dispararTirosCrabmeat( InimigoCrabmeat *inimigo );
static void atualizarTirosCrabmeat( InimigoCrabmeat *inimigo, GameWorld *gw, float delta );
static void desenharTirosCrabmeat( InimigoCrabmeat *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

InimigoCrabmeat *criarInimigoCrabmeat( Rectangle ret, Color cor ) {

    InimigoCrabmeat *novoInimigo = (InimigoCrabmeat*) malloc( sizeof( InimigoCrabmeat ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 55;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_CRABMEAT_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = true;

    novoInimigo->contadorAtaque = 0.0f;
    novoInimigo->tempoEntreAtaques = 2.2f;
    novoInimigo->contadorEstadoAtaque = 0.0f;
    novoInimigo->duracaoEstadoAtaque = 0.8f;
    novoInimigo->atirouNoAtaqueAtual = false;

    for ( int i = 0; i < 4; i++ ) {
        novoInimigo->tiros[i] = (TiroCrabmeat) { 0 };
    }

    inicializarAnimacaoCrabmeat( &novoInimigo->animacaoParado, 1, false );
    novoInimigo->animacaoParado.quadros[0] = (QuadroAnimacao) {
        .fonte = { 24, 202, 48, 32 },
        .duracao = 300,
        .retColisao = { 10, 10, 76, 54 }
    };

    inicializarAnimacaoCrabmeat( &novoInimigo->animacaoAndando, 4, false );
    novoInimigo->animacaoAndando.quadros[0] = (QuadroAnimacao) { { 24, 202, 48, 32 }, 180, { 10, 10, 76, 54 } };
    novoInimigo->animacaoAndando.quadros[1] = (QuadroAnimacao) { { 80, 202, 48, 32 }, 180, { 10, 10, 76, 54 } };
    novoInimigo->animacaoAndando.quadros[2] = (QuadroAnimacao) { { 136, 198, 48, 40 }, 180, { 10, 10, 76, 54 } };
    novoInimigo->animacaoAndando.quadros[3] = (QuadroAnimacao) { { 192, 198, 48, 40 }, 180, { 10, 10, 76, 54 } };

    inicializarAnimacaoCrabmeat( &novoInimigo->animacaoAtacando, 2, false );
    novoInimigo->animacaoAtacando.quadros[0] = (QuadroAnimacao) { { 24, 202, 48, 32 }, 220, { 10, 10, 76, 54 } };
    novoInimigo->animacaoAtacando.quadros[1] = (QuadroAnimacao) { { 248, 202, 48, 32 }, 220, { 10, 10, 76, 54 } };

    inicializarAnimacaoCrabmeat( &novoInimigo->animacaoMorrendo, 4, true );
    novoInimigo->animacaoMorrendo.quadros[0] = (QuadroAnimacao) { { 169, 1, 32, 32 }, 100, { 0, 0, 0, 0 } };
    novoInimigo->animacaoMorrendo.quadros[1] = (QuadroAnimacao) { { 202, 1, 32, 32 }, 100, { 0, 0, 0, 0 } };
    novoInimigo->animacaoMorrendo.quadros[2] = (QuadroAnimacao) { { 235, 1, 32, 32 }, 100, { 0, 0, 0, 0 } };
    novoInimigo->animacaoMorrendo.quadros[3] = (QuadroAnimacao) { { 268, 1, 32, 32 }, 100, { 0, 0, 0, 0 } };

    novoInimigo->animacoes[ESTADO_INIMIGO_CRABMEAT_PARADO] = &novoInimigo->animacaoParado;
    novoInimigo->animacoes[ESTADO_INIMIGO_CRABMEAT_ANDANDO] = &novoInimigo->animacaoAndando;
    novoInimigo->animacoes[ESTADO_INIMIGO_CRABMEAT_ATACANDO] = &novoInimigo->animacaoAtacando;
    novoInimigo->animacoes[ESTADO_INIMIGO_CRABMEAT_MORRENDO] = &novoInimigo->animacaoMorrendo;
    novoInimigo->quantidadeAnimacoes = 4;

    return novoInimigo;

}

void destruirInimigoCrabmeat( InimigoCrabmeat *inimigo ) {
    if ( inimigo != NULL ) {
        for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( inimigo->animacoes[i] );
        }
        free( inimigo );
    }
}

void atualizarInimigoCrabmeat( InimigoCrabmeat *inimigo, GameWorld *gw, float delta ) {

    if ( inimigo->ativo ) {

        atualizarTirosCrabmeat( inimigo, gw, delta );

        if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_ANDANDO ) {

            atualizarAnimacao( &inimigo->animacaoAndando, delta );

            Inimigo ini = {
                .objeto = inimigo,
                .tipo = TIPO_INIMIGO_CRABMEAT
            };

            inimigo->vel.x = inimigo->olhandoParaDireita ? inimigo->velAndando : -inimigo->velAndando;
            inimigo->ret.x += inimigo->vel.x * delta;
            resolverColisaoInimigoObstaculosMapaX( &ini, gw->mapa );

            inimigo->vel.y += gw->gravidade * delta;
            if ( inimigo->vel.y > inimigo->velMaxQueda ) {
                inimigo->vel.y = inimigo->velMaxQueda;
            }

            inimigo->ret.y += inimigo->vel.y * delta;
            resolverColisaoInimigoObstaculosMapaY( &ini, gw->mapa );

            inimigo->contadorAtaque += delta;
            if ( inimigo->contadorAtaque >= inimigo->tempoEntreAtaques ) {
                inimigo->contadorAtaque = 0.0f;
                inimigo->contadorEstadoAtaque = 0.0f;
                inimigo->atirouNoAtaqueAtual = false;
                inimigo->estado = ESTADO_INIMIGO_CRABMEAT_ATACANDO;
                reiniciarAnimacao( &inimigo->animacaoAtacando );
            }

        } else if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_ATACANDO ) {

            atualizarAnimacao( &inimigo->animacaoAtacando, delta );

            if ( !inimigo->atirouNoAtaqueAtual ) {
                dispararTirosCrabmeat( inimigo );
                inimigo->atirouNoAtaqueAtual = true;
            }

            inimigo->contadorEstadoAtaque += delta;
            if ( inimigo->contadorEstadoAtaque >= inimigo->duracaoEstadoAtaque ) {
                inimigo->contadorEstadoAtaque = 0.0f;
                inimigo->estado = ESTADO_INIMIGO_CRABMEAT_ANDANDO;
            }

        } else if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_MORRENDO ) {
            atualizarAnimacao( &inimigo->animacaoMorrendo, delta );

            if ( inimigo->animacaoMorrendo.finalizada ) {
                inimigo->ativo = false;
            }
        }

    }

}

void desenharInimigoCrabmeat( InimigoCrabmeat *inimigo ) {

    if ( inimigo->ativo ) {
        if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_MORRENDO ) {
            desenharQuadroAnimacaoInimigoCrabmeatMorrendo( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoMorrendo ), 2.0f, WHITE );
        } else {
            QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoCrabmeat( inimigo );
            desenharQuadroAnimacaoInimigoCrabmeat( inimigo, qa, WHITE );
        }
        desenharTirosCrabmeat( inimigo );

        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( inimigo->ret, Fade( inimigo->cor, 0.5f ) );
            DrawRectangleLines( inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK );
        }
    }

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoCrabmeat( InimigoCrabmeat *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoCrabmeat( inimigo ) );
}

static void inicializarAnimacaoCrabmeat( Animacao *anim, int quantidadeQuadros, bool executarUmaVez ) {
    anim->quantidadeQuadros = quantidadeQuadros;
    anim->quadroAtual = 0;
    anim->contadorTempoQuadro = 0;
    anim->pararNoUltimoQuadro = false;
    anim->executarUmaVez = executarUmaVez;
    anim->finalizada = false;
    criarQuadrosAnimacao( anim, quantidadeQuadros );
}

static Animacao *getAnimacaoAtualInimigoCrabmeat( InimigoCrabmeat *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}

static void desenharQuadroAnimacaoInimigoCrabmeat( InimigoCrabmeat *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {
        DrawTexturePro(
            rm.texturaBadniksBase,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                inimigo->olhandoParaDireita ? qa->fonte.width : -qa->fonte.width,
                qa->fonte.height
            },
            inimigo->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );
    }

}

static void desenharQuadroAnimacaoInimigoCrabmeatMorrendo( InimigoCrabmeat *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

    if ( qa != NULL ) {
        DrawTexturePro(
            rm.texturaBadniks,
            qa->fonte,
            (Rectangle) {
                inimigo->ret.x + inimigo->ret.width / 2 - qa->fonte.width * escala / 2,
                inimigo->ret.y + inimigo->ret.height / 2 - qa->fonte.height * escala / 2,
                qa->fonte.width * escala,
                qa->fonte.height * escala
            },
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );
    }

}

static void dispararTirosCrabmeat( InimigoCrabmeat *inimigo ) {

    for ( int i = 0; i < 2; i++ ) {
        TiroCrabmeat *tiro = &inimigo->tiros[i];
        float direcao = i == 0 ? -1.0f : 1.0f;

        tiro->ret = (Rectangle) {
            inimigo->ret.x + ( i == 0 ? 8 : inimigo->ret.width - 24 ),
            inimigo->ret.y + 18,
            32,
            32
        };
        tiro->vel = (Vector2) {
            direcao * 110,
            -330
        };
        tiro->ativo = true;
        tiro->quadroAtual = 0;
        tiro->contadorAnimacao = 0.0f;
    }

}

static void atualizarTirosCrabmeat( InimigoCrabmeat *inimigo, GameWorld *gw, float delta ) {

    int larguraMapa = gw->mapa->colunas * gw->mapa->dimensaoPadraoElementos;
    int alturaMapa = gw->mapa->linhas * gw->mapa->dimensaoPadraoElementos;

    for ( int i = 0; i < 4; i++ ) {
        TiroCrabmeat *tiro = &inimigo->tiros[i];

        if ( tiro->ativo ) {
            tiro->contadorAnimacao += delta;
            if ( tiro->contadorAnimacao >= 0.1f ) {
                tiro->contadorAnimacao = 0.0f;
                tiro->quadroAtual = !tiro->quadroAtual;
            }

            tiro->vel.y += gw->gravidade * delta;
            tiro->ret.x += tiro->vel.x * delta;
            tiro->ret.y += tiro->vel.y * delta;

            if ( tiro->ret.x < -tiro->ret.width || tiro->ret.x > larguraMapa ||
                 tiro->ret.y > alturaMapa || tiro->ret.y < -tiro->ret.height ) {
                tiro->ativo = false;
            }
        }
    }

}

static void desenharTirosCrabmeat( InimigoCrabmeat *inimigo ) {

    for ( int i = 0; i < 4; i++ ) {
        TiroCrabmeat *tiro = &inimigo->tiros[i];

        if ( tiro->ativo ) {
            DrawTexturePro(
                rm.texturaBadniksBase,
                tiro->quadroAtual == 0
                    ? (Rectangle) { 304, 198, 16, 16 }
                    : (Rectangle) { 304, 222, 16, 16 },
                tiro->ret,
                (Vector2) { 0 },
                0.0f,
                WHITE
            );
        }
    }

}
