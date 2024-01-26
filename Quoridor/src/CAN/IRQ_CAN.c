#include <stdint.h>
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000)
// no previous prototype for function 'CAN_IRQHandler'
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "../GLCD/GLCD.h"
#include "../common.h"
#include "../game/game.h"
#include "../game/graphics.h"
#include "CAN.h"
#include <LPC17xx.h>
#include <stdlib.h>

extern uint8_t icr; // icr and result must be global in order to work with both
                    // real and simulated landtiger.
extern uint32_t result;
extern CAN_msg CAN_TxMsg; /* CAN message for sending */
extern CAN_msg CAN_RxMsg; /* CAN message for receiving */

union Move fetched_move;

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler(void)
{

    /* check CAN controller 1 */
    icr = 0;
    icr = (LPC_CAN1->ICR | icr) & 0xFF; /* clear interrupts */

    if (icr & (1 << 0))
    {                             /* CAN Controller #1 meassage is received */
        CAN_rdMsg(1, &CAN_RxMsg); /* Read the message */
        LPC_CAN1->CMR = (1 << 2); /* Release receive buffer */

        fetched_move.bytes[0] = CAN_RxMsg.data[0];
        fetched_move.bytes[1] = CAN_RxMsg.data[1];
        fetched_move.bytes[2] = CAN_RxMsg.data[2];
        fetched_move.bytes[3] = CAN_RxMsg.data[3];

        if (!connected && fetched_move.player_id == 0xFF)
        {
            connected = true;
            draw_color_selection_menu();

            CAN_wrMsg(1, &CAN_RxMsg);
            return; // handshake successful
        }

        if (connected)
        {
            if (fetched_move.player_id == 0xFC)
            {
                opponent = RED;
                if (AI_enabled) NPC = WHITE;
                draw_board();
                srand(LPC_RIT->RICOUNTER);
                change_turn();
            }
            else if (fetched_move.player_id == 0xFA)
            {
                opponent = WHITE;
                if (AI_enabled) NPC = RED;
                draw_board();
                srand(LPC_RIT->RICOUNTER);
                change_turn();
            }
        }

        if (current_player == opponent)
        {
            if (fetched_move.type == PLAYER_MOVE)
            {
                move_player(fetched_move.x, fetched_move.y);
                change_turn();
            }
            else if (fetched_move.type == WALL_PLACEMENT)
            {
                place_wall(
                    fetched_move.x, fetched_move.y, fetched_move.direction);
                change_turn();
            }
        }
    }
}
