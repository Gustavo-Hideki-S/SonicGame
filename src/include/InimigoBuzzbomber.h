/**
 * @file InimigoBuzzbomber.h
 * @brief Declaracoes das funcoes do inimigo Buzzbomber.
 */
#pragma once

#include "Tipos.h"

InimigoBuzzbomber *criarInimigoBuzzbomber( Rectangle ret, Color cor );
void destruirInimigoBuzzbomber( InimigoBuzzbomber *inimigo );
void atualizarInimigoBuzzbomber( InimigoBuzzbomber *inimigo, GameWorld *gw, float delta );
void desenharInimigoBuzzbomber( InimigoBuzzbomber *inimigo );
QuadroAnimacao *getQuadroAnimacaoAtualInimigoBuzzbomber( InimigoBuzzbomber *inimigo );
