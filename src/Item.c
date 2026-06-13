/**
 * @file Item.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Item.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Item.h"
#include "ItemAnel.h"
#include "Tipos.h"
#include "ItemMonitorVida.h"
#include "ItemMonitorInvencivel.h"

/**
 * @brief Cria um novo Item.
 */
Item *criarItem( TipoItem tipo ) {

    Item *novoItem = (Item*) malloc( sizeof( Item ) );
    novoItem->objeto = NULL;
    novoItem->tipo = tipo;

    return novoItem;

}

/**
 * @brief Destroi um item.
 */
void destruirItem( Item *item ) {
    if ( item != NULL ) {
        switch ( item->tipo ) {
            case TIPO_ITEM_ANEL:
                destruirItemAnel( (ItemAnel*) item->objeto );
                break;
            case TIPO_ITEM_MONITOR_INVENCIBILIDADE:
                destruirMonitorInvencivel( (ItemMonitorInvencivel*) item->objeto );
                break;
            case TIPO_ITEM_MONITOR_VIDA:
                destruirMonitorVida( (ItemMonitorVida*) item->objeto );
                break;
            default:
                break;
        }
        free( item );
    }
}

/**
 * @brief Atualiza um item.
 */
void atualizarItem( Item *item, float delta ) {

    switch ( item->tipo ) {
        case TIPO_ITEM_ANEL:
            atualizarItemAnel( (ItemAnel*) item->objeto, delta );
            break;
        case TIPO_ITEM_MONITOR_INVENCIBILIDADE:
            atualizarMonitorInvencivel( (ItemMonitorInvencivel*) item->objeto, delta );
            break;
        case TIPO_ITEM_MONITOR_VIDA:
            atualizarMonitorVida( (ItemMonitorVida*) item->objeto, delta );
            break;
        default:
            return;
    }

}

/**
 * @brief Desenha um item.
 */
void desenharItem( Item *item ) {

    switch ( item->tipo ) {
        case TIPO_ITEM_ANEL:
            desenharItemAnel( (ItemAnel*) item->objeto );
            break;
        case TIPO_ITEM_MONITOR_INVENCIBILIDADE:
            desenharMonitorInvencivel( (ItemMonitorInvencivel*) item->objeto );
            break;
        case TIPO_ITEM_MONITOR_VIDA:
            desenharMonitorVida( (ItemMonitorVida*) item->objeto );
            break;
        default:
            return;
    }

}