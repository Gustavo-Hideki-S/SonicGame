/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "Jogador.h"
#include "Macros.h"
#include "Mapa.h"
#include "Obstaculo.h"
#include "Tipos.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

static void desenharFundo( GameWorld *gw );
static void atualizarCamera( GameWorld *gw );
static void desenharSpriteHud( Rectangle fonte, Rectangle destino );
static void desenharDigitoHud( int digito, float x, float y, float escala );
static void desenharNumeroHud( int numero, int quantidadeDigitos, float x, float y, float escala );
static void desenharHud( GameWorld *gw );

static void inicializar( GameWorld *gw );
static void reiniciar( GameWorld *gw );


/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld( void ) {
    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    inicializar( gw );
    return gw;
}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw ) {
    if ( gw != NULL ) {
        destruirMapa( gw->mapa );
        destruirJogador( gw->jogador );
        free( gw );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    Music *musicaAtual = ( gw->faseAtual == 1 )
        ? &rm.musicaFase01
        : &rm.musicaFase02;

    if ( !IsMusicStreamPlaying( *musicaAtual ) ) {
        PlayMusicStream( *musicaAtual );
    }
    UpdateMusicStream( *musicaAtual );

    gw->tempoJogo += delta;
    if ( gw->tempoPiscarRings > 0.0f ) {
        gw->tempoPiscarRings -= delta;
        if ( gw->tempoPiscarRings < 0.0f ) {
            gw->tempoPiscarRings = 0.0f;
        }
    }

    if ( IsKeyPressed( KEY_R ) ) {
        reiniciar( gw );
        return;
    }

    Jogador *j = gw->jogador;
    atualizarMapa( gw->mapa, gw, delta );
    entradaJogador( j, delta );
    atualizarJogador( j, gw, delta );
    atualizarCamera( gw );
    if ( gw->faseAtual == 1 &&
        gw->jogador->ret.x > calcularLarguraMapa( gw->mapa ) - 200 ) {

        destruirMapa( gw->mapa );

        if ( IsMusicStreamPlaying( rm.musicaFase01 ) ) {
            StopMusicStream( rm.musicaFase01 );
        }
        PlayMusicStream( rm.musicaFase02 );

        rm.texturaTerreno = rm.texturaTerreno2;
        gw->mapa = carregarMapa( "resources/mapas/mapa02.txt" );

        gw->jogador->ret.x = 100;
        gw->jogador->ret.y = 100;

        gw->faseAtual = 2;
    }

}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( (Color) { 36, 0, 180, 225 } );

    BeginMode2D( gw->camera );
    if ( gw->faseAtual == 2 ) {

        DrawRectangle(
            -10000,
            -10000,
            20000,
            20000,
            (Color){119, 221, 221, 255}
        );
    }
    desenharFundo( gw );
    desenharMapa( gw->mapa );
    desenharJogador( gw->jogador );
    EndMode2D();

    desenharHud( gw );

    EndDrawing();

}

static void desenharHud( GameWorld *gw ) {

    float escalaHud = 2.0f;
    float escalaRostoSonic = 3.0f;
    float margemHud = 24.0f;
    float alturaItemHud = 16 * escalaHud;
    float alturaRostoSonic = 16 * escalaRostoSonic;
    float xRotulos = 4 * escalaHud;
    float xValores = xRotulos + 48 * escalaHud;
    float yScore = margemHud;
    float yTime = yScore + 16 * escalaHud;
    float yRings = yTime + 16 * escalaHud;
    float ySonic = GetScreenHeight() - margemHud - alturaRostoSonic;
    int minutos = (int) gw->tempoJogo / 60;
    int segundos = (int) gw->tempoJogo % 60;
    bool ringsVermelho = gw->tempoPiscarRings > 0.0f && ( (int) ( gw->tempoPiscarRings * 10 ) % 2 == 0 );

    desenharSpriteHud(
        (Rectangle) { 24, 432, 40, 16 },
        (Rectangle) { xRotulos, yScore, 40 * escalaHud, alturaItemHud }
    );
    desenharNumeroHud( gw->pontuacao, 6, xValores, yScore, escalaHud );

    desenharSpriteHud(
        (Rectangle) { 24, 456, 32, 16 },
        (Rectangle) { xRotulos, yTime, 32 * escalaHud, alturaItemHud }
    );
    desenharNumeroHud( minutos, 1, xValores, yTime, escalaHud );
    desenharSpriteHud(
        (Rectangle) { 88, 224, 8, 16 },
        (Rectangle) { xValores + 16, yTime, 8 * escalaHud, alturaItemHud }
    );
    desenharNumeroHud( segundos, 2, xValores + 32, yTime, escalaHud );

    desenharSpriteHud(
        ringsVermelho ? (Rectangle) { 72, 480, 40, 16 } : (Rectangle) { 24, 480, 40, 16 },
        (Rectangle) { xRotulos, yRings, 40 * escalaHud, alturaItemHud }
    );
    desenharNumeroHud( gw->jogador->quantidadeAneis, 3, xValores, yRings, escalaHud );

    desenharSpriteHud(
        (Rectangle) { 40, 400, 16, 16 },
        (Rectangle) { margemHud, ySonic, 16 * escalaRostoSonic, alturaRostoSonic }
    );
    desenharSpriteHud(
        (Rectangle) { 57, 401, 31, 7 },
        (Rectangle) { margemHud + 60, ySonic + 2, 31 * escalaHud, 7 * escalaHud }
    );
    desenharSpriteHud(
        (Rectangle) { 62, 410, 6, 6 },
        (Rectangle) { margemHud + 60, ySonic + 26, 6 * escalaHud, 6 * escalaHud }
    );
    desenharNumeroHud( gw->jogador->quantidadeVidas, 1, margemHud + 80, ySonic + 18, escalaHud );

}

static void desenharSpriteHud( Rectangle fonte, Rectangle destino ) {
    DrawTexturePro(
        rm.texturaHud,
        fonte,
        destino,
        (Vector2) { 0, 0 },
        0.0f,
        WHITE
    );
}

static void desenharDigitoHud( int digito, float x, float y, float escala ) {

    if ( digito < 0 || digito > 9 ) {
        return;
    }

    desenharSpriteHud(
        (Rectangle) { 72 + digito * 16, 432, 8, 16 },
        (Rectangle) { x, y, 8 * escala, 16 * escala }
    );

}

static void desenharNumeroHud( int numero, int quantidadeDigitos, float x, float y, float escala ) {

    char texto[16];
    int larguraDigito = (int) ( 8 * escala );

    if ( numero < 0 ) {
        numero = 0;
    }

    snprintf( texto, sizeof( texto ), "%0*d", quantidadeDigitos, numero );

    for ( int i = 0; texto[i] != '\0'; i++ ) {
        desenharDigitoHud( texto[i] - '0', x + i * larguraDigito, y, escala );
    }

}

static void desenharFundo( GameWorld *gw ) {

    Texture2D fundo = ( gw->faseAtual == 1 )
        ? rm.texturaFundoFase1
        : rm.texturaFundoFase2;

    int larguraFundo = fundo.width;
    int larguraMapa = calcularLarguraMapa( gw->mapa );
    int alturaMapa = calcularAlturaMapa( gw->mapa );

    int repeticoes = larguraMapa / larguraFundo + 1;

    int deslocamentoParallax =
        (int)(( gw->camera.target.x / (float)larguraMapa ) * 200);

    for ( int i = 0; i < repeticoes; i++ ) {

        DrawTexture(
            fundo,
            larguraFundo * i - deslocamentoParallax,
            alturaMapa - fundo.height - 47,
            WHITE
        );

    }

}

static void atualizarCamera( GameWorld *gw ) {

    Jogador *j = gw->jogador;
    Camera2D *c = &gw->camera;

    c->offset.x = GetScreenWidth() / 2;
    c->offset.y = GetScreenHeight() / 2;

    // O target é arredondado para o inteiro mais próximo para garantir que a
    // translação da câmera ocorra sempre em posições inteiras de pixel. Sem esse
    // arredondamento, o valor float contínuo de ret.x faz os tiles serem
    // renderizados em posições subpixel, causando frestas visíveis entre eles.
    c->target.x = roundf( j->ret.x + j->ret.width / 2.0f );
    c->target.y = roundf( j->ret.y + j->ret.height / 2.0f );

    int minX = GetScreenWidth() / 2;
    int maxX = calcularLarguraMapa( gw->mapa ) - GetScreenWidth() / 2;
    int maxY = calcularAlturaMapa( gw->mapa ) - GetScreenHeight() / 2;

    if ( c->target.x < minX ) {
        c->target.x = minX;
    } else if ( c->target.x > maxX ) {
        c->target.x = maxX;
    }

    if ( c->target.y > maxY ) {
        c->target.y = maxY;
    }

}

static void inicializar( GameWorld *gw ) {

    gw->faseAtual = 1;
    gw->mapa = carregarMapa("resources/mapas/mapa01.txt");
    
    //gw->mapa = carregarMapa( "resources/mapas/mapa02.txt" );
    gw->jogador = criarJogador( GetScreenWidth() / 2 + 144, calcularAlturaMapa( gw->mapa ) - 196, 96, 96 );

    gw->camera = (Camera2D) {
        .offset = { 0 },    // deslocamento relativo da câmera em relação ao alvo
        .target = { 0 },    // o alvo da câmera, ou seja, a coordenada em que ela está centralizada
        .rotation = 0.0f,   // rotação da câmera em graus. o pivô é o alvo.
        .zoom = 1.0f        // zoom da câmera. 1.0f significa sem escala
    };

    gw->gravidade = 900;
    gw->pontuacao = 0;
    gw->tempoJogo = 0.0f;
    gw->tempoPiscarRings = 0.0f;

}

static void reiniciar( GameWorld *gw ) {

    destruirMapa( gw->mapa );
    destruirJogador( gw->jogador );

    if ( IsMusicStreamPlaying( rm.musicaFase01 ) ) {
        StopMusicStream( rm.musicaFase01 );
    }
    if ( IsMusicStreamPlaying( rm.musicaFase02 ) ) {
        StopMusicStream( rm.musicaFase02 );
    }

    inicializar( gw );

}
