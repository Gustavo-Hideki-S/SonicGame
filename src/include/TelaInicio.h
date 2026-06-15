/**
 * @file TelaInicio.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações e tipos para a animação da tela de início.
 *
 * @copyright Copyright (c) 2026
 */
#ifndef TELA_INICIO_H
#define TELA_INICIO_H

#include <stdbool.h>

#include "Animacao.h"

// ---------------------------------------------------------------------------
// Configurações do spritesheet da tela de início
// ---------------------------------------------------------------------------
#define TELA_INICIO_FRAME_COUNT       18
#define TELA_INICIO_FRAME_COLS        3
#define TELA_INICIO_FRAME_ROWS        6
#define TELA_INICIO_FRAME_W           319
#define TELA_INICIO_FRAME_H           223
#define TELA_INICIO_FRAME_DURACAO_MS  83   // ~12 fps (1000 / 12 ≈ 83 ms)

/**
 * @brief Índice do primeiro frame do trecho de loop (frame 17, base-0 = 16).
 *        Frames 0-15: intro (executada uma única vez).
 *        Frames 16-17: loop até o jogador pressionar ENTER.
 */
#define TELA_INICIO_LOOP_INICIO       16

// ---------------------------------------------------------------------------
// Tipo público
// ---------------------------------------------------------------------------

/**
 * @brief Encapsula o estado completo da animação da tela de início.
 */
typedef struct {
    Animacao anim;          /**< Animação com os 18 quadros do spritesheet. */
    bool introFinalizada;   /**< true quando o trecho de loop foi atingido.  */
} TelaInicio;

// ---------------------------------------------------------------------------
// Interface pública
// ---------------------------------------------------------------------------

/**
 * @brief Aloca e inicializa uma TelaInicio.
 *        Deve ser chamada após LoadTexture (recursos já carregados).
 */
TelaInicio *criarTelaInicio( void );

/**
 * @brief Libera os recursos internos e destrói a TelaInicio.
 */
void destruirTelaInicio( TelaInicio *tela );

/**
 * @brief Avança a animação conforme o tempo decorrido.
 *        Frames 0-15 são exibidos uma vez; frames 16-17 ficam em loop.
 *
 * @param tela   Ponteiro para a TelaInicio.
 * @param delta  Tempo em segundos desde o último quadro.
 */
void atualizarTelaInicio( TelaInicio *tela, float delta );

/**
 * @brief Desenha o frame atual esticado para preencher toda a janela.
 *
 * @param tela          Ponteiro para a TelaInicio.
 * @param larguraJanela Largura da janela em pixels.
 * @param alturaJanela  Altura da janela em pixels.
 */
void desenharTelaInicio( TelaInicio *tela, int larguraJanela, int alturaJanela );

/**
 * @brief Indica se a intro já terminou e o loop de espera está ativo.
 *        Enquanto retornar false, a entrada do jogador deve ser ignorada.
 */
bool telaInicioLoopAtivo( TelaInicio *tela );

#endif // TELA_INICIO_H