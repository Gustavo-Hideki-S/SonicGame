/**
 * @file Macros.h
 * @author Prof. Dr. David Buzatto
 * @brief Macros utilizados no jogo.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#define FUNDO_FASE2_X 424
#define FUNDO_FASE2_Y 167
#define FUNDO_FASE2_W (663 - 424)  // 239
#define FUNDO_FASE2_H (387 - 167)  // 220
/**
 * @brief Macro que empacota a função TraceLog, gerando uma chamada a TraceLog com o nível LOG_INFO
 */
#define trace( ... ) TraceLog( LOG_INFO, __VA_ARGS__ );