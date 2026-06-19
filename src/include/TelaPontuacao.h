/**
 * @file TelaPontuacao.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações e tipos para a tela de pontuação de fim de fase.
 *
 * @copyright Copyright (c) 2026
 */
#ifndef TELA_PONTUACAO_H
#define TELA_PONTUACAO_H

#include <stdbool.h>

// ---------------------------------------------------------------------------
// Configurações da tela de pontuação
// ---------------------------------------------------------------------------

/**
 * @brief Tempo em segundos que a tela permanece exibindo apenas os dados
 *        da fase, antes de liberar a mensagem "PRESSIONE ENTER".
 */
#define TELA_PONTUACAO_TEMPO_ESPERA_MENSAGEM  2.5f

// ---------------------------------------------------------------------------
// Tipo público
// ---------------------------------------------------------------------------

/**
 * @brief Encapsula o estado completo da tela de pontuação exibida ao
 *        final de uma fase, contendo os dados consolidados do jogador
 *        e o controle de tempo para liberação da mensagem de continuação.
 */
typedef struct {
    int   score;              /**< Pontuação obtida na fase.                     */
    int   tempo;               /**< Tempo da fase em segundos (já consolidado).   */
    int   aneis;               /**< Quantidade de anéis coletados.                */
    int   vidas;                /**< Quantidade de vidas restantes.                */
    float tempoDecorrido;      /**< Tempo acumulado desde a criação da tela.      */
    bool  mensagemLiberada;     /**< true quando "PRESSIONE ENTER" deve aparecer.  */
} TelaPontuacao;

// ---------------------------------------------------------------------------
// Interface pública
// ---------------------------------------------------------------------------

/**
 * @brief Aloca e inicializa uma TelaPontuacao com os dados finais da fase.
 *
 * @param score Pontuação obtida na fase.
 * @param tempo Tempo da fase em segundos.
 * @param aneis Quantidade de anéis coletados.
 * @param vidas Quantidade de vidas restantes.
 * @return Ponteiro para a TelaPontuacao criada.
 */
TelaPontuacao *criarTelaPontuacao( int score, int tempo, int aneis, int vidas );

/**
 * @brief Libera os recursos internos e destrói a TelaPontuacao.
 */
void destruirTelaPontuacao( TelaPontuacao *tela );

/**
 * @brief Avança o cronômetro interno da tela, liberando a mensagem
 *        "PRESSIONE ENTER" após TELA_PONTUACAO_TEMPO_ESPERA_MENSAGEM segundos.
 *
 * @param tela  Ponteiro para a TelaPontuacao.
 * @param delta Tempo em segundos desde o último quadro.
 */
void atualizarTelaPontuacao( TelaPontuacao *tela, float delta );

/**
 * @brief Desenha a tela de pontuação centralizada na janela, utilizando
 *        apenas DrawText (sem texturas).
 *
 * @param tela          Ponteiro para a TelaPontuacao.
 * @param larguraJanela Largura da janela em pixels.
 * @param alturaJanela  Altura da janela em pixels.
 */
void desenharTelaPontuacao( TelaPontuacao *tela, int larguraJanela, int alturaJanela );

/**
 * @brief Indica se a mensagem "PRESSIONE ENTER" já foi liberada e,
 *        portanto, a entrada do jogador pode ser aceita.
 */
bool telaPontuacaoPronta( TelaPontuacao *tela );

#endif // TELA_PONTUACAO_H