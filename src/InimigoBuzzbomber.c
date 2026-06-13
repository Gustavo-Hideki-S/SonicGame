/**
 * @file InimigoBuzzbomber.c
 * @brief Implementacao do inimigo Buzzbomber.
 */
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "InimigoBuzzbomber.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void inicializarAnimacaoBuzzbomber( Animacao *anim, int quantidadeQuadros, bool executarUmaVez );
static Animacao *getAnimacaoAtualInimigoBuzzbomber( InimigoBuzzbomber *inimigo );
static void desenharQuadroAnimacaoInimigoBuzzbomber( InimigoBuzzbomber *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoBuzzbomberMorrendo( InimigoBuzzbomber *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static void dispararTiroBuzzbomber( InimigoBuzzbomber *inimigo );
static void atualizarTirosBuzzbomber( InimigoBuzzbomber *inimigo, GameWorld *gw, float delta );
static void desenharTirosBuzzbomber( InimigoBuzzbomber *inimigo );

InimigoBuzzbomber *criarInimigoBuzzbomber( Rectangle ret, Color cor ) {

    InimigoBuzzbomber *novoInimigo = (InimigoBuzzbomber*) malloc( sizeof( InimigoBuzzbomber ) );

    novoInimigo->ret = ret;
    novoInimigo->cor = cor;
    novoInimigo->xInicial = ret.x;
    novoInimigo->distanciaPatrulha = 260;
    novoInimigo->velVoando = 90;

    novoInimigo->estado = ESTADO_INIMIGO_BUZZBOMBER_VOANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    novoInimigo->contadorAtaque = 0.0f;
    novoInimigo->tempoEntreAtaques = 2.0f;
    novoInimigo->contadorEstadoAtaque = 0.0f;
    novoInimigo->duracaoPreparandoAtaque = 0.45f;
    novoInimigo->duracaoAtacando = 0.65f;
    novoInimigo->atirouNoAtaqueAtual = false;

    for ( int i = 0; i < 4; i++ ) {
        novoInimigo->tiros[i] = (TiroBuzzbomber) { 0 };
    }

    inicializarAnimacaoBuzzbomber( &novoInimigo->animacaoVoando, 4, false );
    novoInimigo->animacaoVoando.quadros[0] = (QuadroAnimacao) { { 24, 347, 48, 32 }, 120, { 8, 8, 80, 48 } };
    novoInimigo->animacaoVoando.quadros[1] = (QuadroAnimacao) { { 24, 387, 48, 24 }, 120, { 8, 8, 80, 48 } };
    novoInimigo->animacaoVoando.quadros[2] = (QuadroAnimacao) { { 80, 347, 48, 32 }, 120, { 8, 8, 80, 48 } };
    novoInimigo->animacaoVoando.quadros[3] = (QuadroAnimacao) { { 80, 385, 48, 26 }, 120, { 8, 8, 80, 48 } };

    inicializarAnimacaoBuzzbomber( &novoInimigo->animacaoPreparandoAtaque, 1, false );
    novoInimigo->animacaoPreparandoAtaque.quadros[0] = (QuadroAnimacao) { { 24, 279, 48, 34 }, 220, { 8, 8, 80, 48 } };

    inicializarAnimacaoBuzzbomber( &novoInimigo->animacaoAtacando, 3, false );
    novoInimigo->animacaoAtacando.quadros[0] = (QuadroAnimacao) { { 80, 275, 40, 32 }, 140, { 8, 8, 80, 48 } };
    novoInimigo->animacaoAtacando.quadros[1] = (QuadroAnimacao) { { 24, 423, 56, 48 }, 140, { 8, 8, 80, 48 } };
    novoInimigo->animacaoAtacando.quadros[2] = (QuadroAnimacao) { { 88, 419, 56, 56 }, 140, { 8, 8, 80, 48 } };

    inicializarAnimacaoBuzzbomber( &novoInimigo->animacaoMorrendo, 4, true );
    novoInimigo->animacaoMorrendo.quadros[0] = (QuadroAnimacao) { { 169, 1, 32, 32 }, 100, { 0, 0, 0, 0 } };
    novoInimigo->animacaoMorrendo.quadros[1] = (QuadroAnimacao) { { 202, 1, 32, 32 }, 100, { 0, 0, 0, 0 } };
    novoInimigo->animacaoMorrendo.quadros[2] = (QuadroAnimacao) { { 235, 1, 32, 32 }, 100, { 0, 0, 0, 0 } };
    novoInimigo->animacaoMorrendo.quadros[3] = (QuadroAnimacao) { { 268, 1, 32, 32 }, 100, { 0, 0, 0, 0 } };

    novoInimigo->animacoes[ESTADO_INIMIGO_BUZZBOMBER_VOANDO] = &novoInimigo->animacaoVoando;
    novoInimigo->animacoes[ESTADO_INIMIGO_BUZZBOMBER_PREPARANDO_ATAQUE] = &novoInimigo->animacaoPreparandoAtaque;
    novoInimigo->animacoes[ESTADO_INIMIGO_BUZZBOMBER_ATACANDO] = &novoInimigo->animacaoAtacando;
    novoInimigo->animacoes[ESTADO_INIMIGO_BUZZBOMBER_MORRENDO] = &novoInimigo->animacaoMorrendo;
    novoInimigo->quantidadeAnimacoes = 4;

    return novoInimigo;

}

void destruirInimigoBuzzbomber( InimigoBuzzbomber *inimigo ) {
    if ( inimigo != NULL ) {
        for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( inimigo->animacoes[i] );
        }
        free( inimigo );
    }
}

void atualizarInimigoBuzzbomber( InimigoBuzzbomber *inimigo, GameWorld *gw, float delta ) {

    if ( !inimigo->ativo ) {
        return;
    }

    atualizarTirosBuzzbomber( inimigo, gw, delta );

    if ( inimigo->estado == ESTADO_INIMIGO_BUZZBOMBER_VOANDO ) {
        atualizarAnimacao( &inimigo->animacaoVoando, delta );

        float direcao = inimigo->olhandoParaDireita ? 1.0f : -1.0f;
        inimigo->ret.x += direcao * inimigo->velVoando * delta;

        if ( inimigo->ret.x <= inimigo->xInicial - inimigo->distanciaPatrulha ) {
            inimigo->ret.x = inimigo->xInicial - inimigo->distanciaPatrulha;
            inimigo->olhandoParaDireita = true;
        } else if ( inimigo->ret.x >= inimigo->xInicial + inimigo->distanciaPatrulha ) {
            inimigo->ret.x = inimigo->xInicial + inimigo->distanciaPatrulha;
            inimigo->olhandoParaDireita = false;
        }

        inimigo->contadorAtaque += delta;
        if ( inimigo->contadorAtaque >= inimigo->tempoEntreAtaques ) {
            inimigo->contadorAtaque = 0.0f;
            inimigo->contadorEstadoAtaque = 0.0f;
            inimigo->atirouNoAtaqueAtual = false;
            inimigo->estado = ESTADO_INIMIGO_BUZZBOMBER_PREPARANDO_ATAQUE;
            reiniciarAnimacao( &inimigo->animacaoPreparandoAtaque );
        }

    } else if ( inimigo->estado == ESTADO_INIMIGO_BUZZBOMBER_PREPARANDO_ATAQUE ) {
        atualizarAnimacao( &inimigo->animacaoPreparandoAtaque, delta );
        inimigo->contadorEstadoAtaque += delta;

        if ( inimigo->contadorEstadoAtaque >= inimigo->duracaoPreparandoAtaque ) {
            inimigo->contadorEstadoAtaque = 0.0f;
            inimigo->estado = ESTADO_INIMIGO_BUZZBOMBER_ATACANDO;
            reiniciarAnimacao( &inimigo->animacaoAtacando );
        }

    } else if ( inimigo->estado == ESTADO_INIMIGO_BUZZBOMBER_ATACANDO ) {
        atualizarAnimacao( &inimigo->animacaoAtacando, delta );

        if ( !inimigo->atirouNoAtaqueAtual ) {
            dispararTiroBuzzbomber( inimigo );
            inimigo->atirouNoAtaqueAtual = true;
        }

        inimigo->contadorEstadoAtaque += delta;
        if ( inimigo->contadorEstadoAtaque >= inimigo->duracaoAtacando ) {
            inimigo->contadorEstadoAtaque = 0.0f;
            inimigo->estado = ESTADO_INIMIGO_BUZZBOMBER_VOANDO;
        }

    } else if ( inimigo->estado == ESTADO_INIMIGO_BUZZBOMBER_MORRENDO ) {
        atualizarAnimacao( &inimigo->animacaoMorrendo, delta );

        if ( inimigo->animacaoMorrendo.finalizada ) {
            inimigo->ativo = false;
        }
    }

}

void desenharInimigoBuzzbomber( InimigoBuzzbomber *inimigo ) {

    if ( !inimigo->ativo ) {
        return;
    }

    if ( inimigo->estado == ESTADO_INIMIGO_BUZZBOMBER_MORRENDO ) {
        desenharQuadroAnimacaoInimigoBuzzbomberMorrendo( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoMorrendo ), 2.0f, WHITE );
    } else {
        desenharQuadroAnimacaoInimigoBuzzbomber( inimigo, getQuadroAnimacaoAtualInimigoBuzzbomber( inimigo ), WHITE );
    }

    desenharTirosBuzzbomber( inimigo );

}

QuadroAnimacao *getQuadroAnimacaoAtualInimigoBuzzbomber( InimigoBuzzbomber *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoBuzzbomber( inimigo ) );
}

static void inicializarAnimacaoBuzzbomber( Animacao *anim, int quantidadeQuadros, bool executarUmaVez ) {
    anim->quantidadeQuadros = quantidadeQuadros;
    anim->quadroAtual = 0;
    anim->contadorTempoQuadro = 0;
    anim->pararNoUltimoQuadro = false;
    anim->executarUmaVez = executarUmaVez;
    anim->finalizada = false;
    criarQuadrosAnimacao( anim, quantidadeQuadros );
}

static Animacao *getAnimacaoAtualInimigoBuzzbomber( InimigoBuzzbomber *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}

static void desenharQuadroAnimacaoInimigoBuzzbomber( InimigoBuzzbomber *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {
        DrawTexturePro(
            rm.texturaBadniksBase,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                inimigo->olhandoParaDireita ? -qa->fonte.width : qa->fonte.width,
                qa->fonte.height
            },
            inimigo->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );
    }

}

static void desenharQuadroAnimacaoInimigoBuzzbomberMorrendo( InimigoBuzzbomber *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

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

static void dispararTiroBuzzbomber( InimigoBuzzbomber *inimigo ) {

    for ( int i = 0; i < 4; i++ ) {
        TiroBuzzbomber *tiro = &inimigo->tiros[i];

        if ( !tiro->ativo ) {
            float direcao = inimigo->olhandoParaDireita ? 1.0f : -1.0f;

            tiro->ret = (Rectangle) {
                inimigo->ret.x + ( inimigo->olhandoParaDireita ? inimigo->ret.width - 18 : 2 ),
                inimigo->ret.y + inimigo->ret.height - 22,
                24,
                24
            };
            tiro->vel = (Vector2) { direcao * 230, 170 };
            tiro->ativo = true;
            tiro->virouBola = false;
            tiro->quadroAtual = 0;
            tiro->contadorAnimacao = 0.0f;
            tiro->contadorVirarBola = 0.0f;
            return;
        }
    }

}

static void atualizarTirosBuzzbomber( InimigoBuzzbomber *inimigo, GameWorld *gw, float delta ) {

    int larguraMapa = gw->mapa->colunas * gw->mapa->dimensaoPadraoElementos;
    int alturaMapa = gw->mapa->linhas * gw->mapa->dimensaoPadraoElementos;

    for ( int i = 0; i < 4; i++ ) {
        TiroBuzzbomber *tiro = &inimigo->tiros[i];

        if ( tiro->ativo ) {
            tiro->contadorVirarBola += delta;
            if ( tiro->contadorVirarBola >= 0.18f ) {
                tiro->virouBola = true;
            }

            tiro->contadorAnimacao += delta;
            if ( tiro->contadorAnimacao >= 0.1f ) {
                tiro->contadorAnimacao = 0.0f;
                tiro->quadroAtual = !tiro->quadroAtual;
            }

            tiro->ret.x += tiro->vel.x * delta;
            tiro->ret.y += tiro->vel.y * delta;

            if ( tiro->ret.x < -tiro->ret.width || tiro->ret.x > larguraMapa ||
                 tiro->ret.y > alturaMapa || tiro->ret.y < -tiro->ret.height ) {
                tiro->ativo = false;
            }
        }
    }

}

static void desenharTirosBuzzbomber( InimigoBuzzbomber *inimigo ) {

    for ( int i = 0; i < 4; i++ ) {
        TiroBuzzbomber *tiro = &inimigo->tiros[i];

        if ( tiro->ativo ) {
            Rectangle fonte = { 136, 299, 16, 16 };

            if ( tiro->virouBola ) {
                fonte = tiro->quadroAtual == 0
                    ? (Rectangle) { 136, 323, 16, 16 }
                    : (Rectangle) { 136, 347, 16, 16 };
            }

            DrawTexturePro(
                rm.texturaBadniksBase,
                fonte,
                tiro->ret,
                (Vector2) { 0 },
                0.0f,
                WHITE
            );
        }
    }

}
