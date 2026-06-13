/**
 * @file InimigoCrabmeat.h
 * @brief Declaracoes das funcoes do inimigo Crabmeat.
 */
#pragma once

#include "Tipos.h"

InimigoCrabmeat *criarInimigoCrabmeat( Rectangle ret, Color cor );
void destruirInimigoCrabmeat( InimigoCrabmeat *inimigo );
void atualizarInimigoCrabmeat( InimigoCrabmeat *inimigo, GameWorld *gw, float delta );
void desenharInimigoCrabmeat( InimigoCrabmeat *inimigo );
QuadroAnimacao *getQuadroAnimacaoAtualInimigoCrabmeat( InimigoCrabmeat *inimigo );
