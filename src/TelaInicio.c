/**
 * @file TelaInicio.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação da animação da tela de início do jogo.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdlib.h>
#include <stdbool.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "ResourceManager.h"
#include "TelaInicio.h"

// ---------------------------------------------------------------------------
// Funções privadas (static)
// ---------------------------------------------------------------------------

/**
 * @brief Preenche os quadros da animação da tela de início.
 *        As coordenadas Y são definidas explicitamente pois o espaçamento
 *        entre linhas (232 px) difere da altura do conteúdo (223 px);
 *        calcular por passo acumularia ~9 px de erro a cada linha.
 */
static void inicializarQuadros( Animacao *anim ) {

    const int xOrigem[TELA_INICIO_FRAME_COLS] = { 24, 352, 680 };
    const int yOrigem[TELA_INICIO_FRAME_ROWS]  = { 520, 752, 984, 1216, 1448, 1680 };

    anim->quantidadeQuadros   = TELA_INICIO_FRAME_COUNT;
    anim->quadroAtual         = 0;
    anim->contadorTempoQuadro = 0;
    anim->pararNoUltimoQuadro = false;
    anim->executarUmaVez      = false;
    anim->finalizada          = false;

    criarQuadrosAnimacao( anim, TELA_INICIO_FRAME_COUNT );

    for ( int linha = 0; linha < TELA_INICIO_FRAME_ROWS; linha++ ) {
        for ( int col = 0; col < TELA_INICIO_FRAME_COLS; col++ ) {

            int idx = linha * TELA_INICIO_FRAME_COLS + col;

            anim->quadros[idx] = (QuadroAnimacao) {
                .fonte = {
                    (float) xOrigem[col],
                    (float) yOrigem[linha],
                    (float) TELA_INICIO_FRAME_W,
                    (float) TELA_INICIO_FRAME_H
                },
                .duracao    = TELA_INICIO_FRAME_DURACAO_MS,
                .retColisao = (Rectangle){ 0 }
            };
        }
    }
}

// ---------------------------------------------------------------------------
// Interface pública
// ---------------------------------------------------------------------------

/**
 * @brief Aloca e inicializa uma TelaInicio.
 */
TelaInicio *criarTelaInicio( void ) {

    TelaInicio *tela = (TelaInicio*) malloc( sizeof( TelaInicio ) );

    tela->introFinalizada = false;
    inicializarQuadros( &tela->anim );

    return tela;
}

/**
 * @brief Libera os recursos internos e destrói a TelaInicio.
 */
void destruirTelaInicio( TelaInicio *tela ) {
    if ( tela != NULL ) {
        destruirQuadrosAnimacao( &tela->anim );
        free( tela );
    }
}

/**
 * @brief Avança a animação da tela de início.
 *        Frames 0-15: passam uma única vez (intro).
 *        Frames 16-17: ficam em loop até o ENTER ser pressionado.
 */
void atualizarTelaInicio( TelaInicio *tela, float delta ) {

    Animacao *anim = &tela->anim;

    anim->contadorTempoQuadro += (int)( delta * 1000 );

    QuadroAnimacao *qa = getQuadroAtualAnimacao( anim );

    if ( anim->contadorTempoQuadro >= qa->duracao ) {

        anim->contadorTempoQuadro = 0;

        if ( anim->quadroAtual < TELA_INICIO_LOOP_INICIO - 1 ) {
            // Intro: avança quadro a quadro.
            anim->quadroAtual++;

        } else if ( anim->quadroAtual == TELA_INICIO_LOOP_INICIO - 1 ) {
            // Último quadro da intro: entra no loop final e sinaliza.
            anim->quadroAtual     = TELA_INICIO_LOOP_INICIO;
            tela->introFinalizada = true;

        } else {
            // Loop final: alterna entre os dois últimos frames (16 e 17).
            anim->quadroAtual++;
            if ( anim->quadroAtual >= TELA_INICIO_FRAME_COUNT ) {
                anim->quadroAtual = TELA_INICIO_LOOP_INICIO;
            }
        }
    }
}

/**
 * @brief Desenha o frame atual esticado para preencher toda a janela.
 */
void desenharTelaInicio( TelaInicio *tela, int larguraJanela, int alturaJanela ) {

    QuadroAnimacao *qa = getQuadroAtualAnimacao( &tela->anim );
    if ( qa == NULL ) return;

    DrawTexturePro(
        rm.texturaStart,
        qa->fonte,
        (Rectangle){ 0, 0, (float)larguraJanela, (float)alturaJanela },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
}

/**
 * @brief Indica se a intro já terminou e o loop de espera está ativo.
 */
bool telaInicioLoopAtivo( TelaInicio *tela ) {
    return tela->introFinalizada;
}