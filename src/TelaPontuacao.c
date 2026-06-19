/**
 * @file TelaPontuacao.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação da tela de pontuação de fim de fase.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "raylib/raylib.h"

#include "TelaPontuacao.h"

// ---------------------------------------------------------------------------
// Funções privadas (static)
// ---------------------------------------------------------------------------

/**
 * @brief Desenha uma linha de rótulo/valor centralizada horizontalmente,
 *        no estilo "RÓTULO: VALOR".
 */
static void desenharLinhaCentralizada( const char *rotulo, int valor, int y, int larguraJanela, int tamanhoFonte, Color cor ) {

    char texto[64];
    snprintf( texto, sizeof( texto ), "%s: %d", rotulo, valor );

    int largura = MeasureText( texto, tamanhoFonte );
    int x = ( larguraJanela - largura ) / 2;

    DrawText( texto, x, y, tamanhoFonte, cor );

}

/**
 * @brief Desenha um texto simples centralizado horizontalmente.
 */
static void desenharTextoCentralizado( const char *texto, int y, int larguraJanela, int tamanhoFonte, Color cor ) {

    int largura = MeasureText( texto, tamanhoFonte );
    int x = ( larguraJanela - largura ) / 2;

    DrawText( texto, x, y, tamanhoFonte, cor );

}

// ---------------------------------------------------------------------------
// Interface pública
// ---------------------------------------------------------------------------

/**
 * @brief Aloca e inicializa uma TelaPontuacao com os dados finais da fase.
 */
TelaPontuacao *criarTelaPontuacao( int score, int tempo, int aneis, int vidas ) {

    TelaPontuacao *tela = (TelaPontuacao*) malloc( sizeof( TelaPontuacao ) );

    tela->score = score;
    tela->tempo = tempo;
    tela->aneis = aneis;
    tela->vidas = vidas;

    tela->tempoDecorrido   = 0.0f;
    tela->mensagemLiberada = false;

    return tela;

}

/**
 * @brief Libera os recursos internos e destrói a TelaPontuacao.
 */
void destruirTelaPontuacao( TelaPontuacao *tela ) {
    if ( tela != NULL ) {
        free( tela );
    }
}

/**
 * @brief Avança o cronômetro interno da tela, liberando a mensagem
 *        "PRESSIONE ENTER" após o tempo de espera configurado.
 */
void atualizarTelaPontuacao( TelaPontuacao *tela, float delta ) {

    if ( tela->mensagemLiberada ) {
        return;
    }

    tela->tempoDecorrido += delta;

    if ( tela->tempoDecorrido >= TELA_PONTUACAO_TEMPO_ESPERA_MENSAGEM ) {
        tela->tempoDecorrido   = TELA_PONTUACAO_TEMPO_ESPERA_MENSAGEM;
        tela->mensagemLiberada = true;
    }

}

/**
 * @brief Desenha a tela de pontuação centralizada na janela.
 */
void desenharTelaPontuacao( TelaPontuacao *tela, int larguraJanela, int alturaJanela ) {

    int minutos = tela->tempo / 60;
    int segundos = tela->tempo % 60;
    char textoTempo[32];
    snprintf( textoTempo, sizeof( textoTempo ), "%02d:%02d", minutos, segundos );

    int tamanhoTitulo = 50;
    int tamanhoTexto  = 30;
    int espacamento   = 50;

    int yTitulo = alturaJanela / 2 - 160;
    int yScore  = yTitulo + espacamento * 2;
    int yTempo  = yScore  + espacamento;
    int yAneis  = yTempo  + espacamento;
    int yVidas  = yAneis  + espacamento;
    int yMensagem = yVidas + espacamento * 2;

    ClearBackground( BLACK );

    desenharTextoCentralizado( "FASE CONCLUIDA", yTitulo, larguraJanela, tamanhoTitulo, WHITE );

    desenharLinhaCentralizada( "SCORE", tela->score, yScore, larguraJanela, tamanhoTexto, YELLOW );

    int larguraTempo = MeasureText( "TEMPO: ", tamanhoTexto ) + MeasureText( textoTempo, tamanhoTexto );
    int xTempo = ( larguraJanela - larguraTempo ) / 2;
    DrawText( "TEMPO: ", xTempo, yTempo, tamanhoTexto, YELLOW );
    DrawText( textoTempo, xTempo + MeasureText( "TEMPO: ", tamanhoTexto ), yTempo, tamanhoTexto, YELLOW );

    desenharLinhaCentralizada( "ANEIS", tela->aneis, yAneis, larguraJanela, tamanhoTexto, YELLOW );
    desenharLinhaCentralizada( "VIDAS", tela->vidas, yVidas, larguraJanela, tamanhoTexto, YELLOW );

    if ( tela->mensagemLiberada ) {

        // Pisca a mensagem para chamar atenção, no mesmo espírito do
        // efeito de piscar dos anéis em desenharHud (GameWorld.c).
        bool visivel = ( (int) ( tela->tempoDecorrido * 2 ) % 2 == 0 );

        if ( visivel ) {
            desenharTextoCentralizado( "PRESSIONE ENTER", yMensagem, larguraJanela, tamanhoTexto, WHITE );
        }

    }

}

/**
 * @brief Indica se a mensagem "PRESSIONE ENTER" já foi liberada.
 */
bool telaPontuacaoPronta( TelaPontuacao *tela ) {
    return tela->mensagemLiberada;
}